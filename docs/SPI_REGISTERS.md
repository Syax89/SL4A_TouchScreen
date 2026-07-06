# AMD FCH SPI V2 — Registri e Funzionamento

## Mappa Registri MMIO (base = 0xFEC10000)

| Offset | Nome | Size | R/W | Descrizione |
|--------|------|------|-----|-------------|
| 0x00 | CTRL0 | 32bit | R/W | Controllo principale |
| 0x1D | ALT_CS | 8bit | R/W | Chip select alternativo |
| 0x20 | ENA_REG | 32bit | R/W | Abilitazione (SPI100, ALT_SPD a bit 20-23) |
| 0x22 | REG_PREFIX | 16bit | R/W | Registro prefisso (salvato/ripristinato) |
| 0x44 | SPEED_OPCODE | 16bit | R/W | Speed config Windows-only (high byte speed + low byte opcode) |
| 0x45 | OPCODE_REG | 8bit | R/W | Opcode per path V2 |
| 0x47 | CMD_TRIGGER | 8bit | R/W | Trigger V2 (bit7 = start) |
| 0x48 | TX_COUNT | 8bit | R/W | Byte da trasmettere |
| 0x49 | — | 8bit | W | NON USATO da Windows (zero occorrenze in amdspi.sys). Il driver Linux lo scrive come strobe V2. |
| 0x4A | — | 8bit | W | NON USATO da Windows (zero occorrenze in amdspi.sys). Il driver Linux lo scrive come strobe V2. |
| 0x4B | RX_COUNT | 8bit | R/W | Byte da ricevere |
| 0x4C | STATUS | 32bit | R | Stato. Windows lo legge come **byte** e il poll bit31 e' no-op. Il busy affidabile e' CTRL0 bit31. |
| 0x6C | SPEED_REG | 32bit | R/W | Speed V2 (spd7 a bit 8-13) |
| 0x80 | FIFO_BASE | 70B | R/W | FIFO dati (70 byte, 0x80-0xC5) |

---

## CTRL0 (0x00) — Bit Map Completa

```
Bit 31    : BUSY (read-only) — 1 = trasferimento in corso
Bit 30    : SECRET_30 (0x40000000) — sempre settato da Windows
Bit 29    : SECRET_29 (0x20000000) — sempre settato da Windows
Bit 28-24 : Riservati / non documentati
Bit 23    : TXMODE (0x00800000) — forza controller in TX mode
Bit 22    : Riservato
Bit 21    : PRESERVED (0x00200000) — Windows lo preserva (non modifica)
Bit 20    : FIFO_CLEAR (0x00100000) — pulse per resettare ptr FIFO
Bit 19    : Riservato
Bit 18    : SECRET_18 (0x00040000) — sempre settato da Windows
Bit 17    : Riservato
Bit 16    : EXEC_CMD (0x00010000) — trigger V1 (start trasferimento)
Bit 15-12 : RX_COUNT_AUTO — auto-settati da hardware in base a RX_COUNT
Bit 11-8  : Riservati
Bit 7-0   : OPCODE_V1 — opcode per path V1
```

### Valori CTRL0 osservati

| Scenario | CTRL0 Value | Bit Significativi |
|----------|-------------|-------------------|
| TX write (0x0B) | 0x6fac100b | 30+29+21+20+opcode=0x0B |
| RX read (0x0B) | 0x6f8c080b | 30+29+opcode=0x0B, auto-set bits 15-12 |
| Dopo setup_v2_regs | 0x6fac0400 | 30+29+21+18 (base) |

### Dettaglio Bit

**Bit 30 (0x40000000):** Secret bit da decomp Windows. Funzione sconosciuta ma sempre presente.

**Bit 29 (0x20000000):** Secret bit da decomp Windows. Funzione sconosciuta ma sempre presente.

**Bit 23 (TXMODE, 0x00800000):** 
- Quando SET: il controller forza la linea MOSI attiva per TUTTI gli opcode
- Necessario per opcode come 0x00, 0x02, 0x70, 0xB0 (il controller non sa che sono write)
- Per opcode 0x0B: il controller ha hardware speciale che gestisce auto-read
- **PROBLEMA:** con TXMODE=1, gli opcode NON-0x0B non possono fare RX (il controller resta in TX)
- Windows **NON** setta mai TXMODE (usa altro meccanismo)
- Solo per firmware 0xB0 è veramente necessario

**Bit 21 (0x00200000):** Windows lo preserva (legge il valore corrente e lo mantiene). Funzione sconosciuta.

**Bit 20 (FIFO_CLEAR, 0x00100000):**
- **Richiede transizione 0→1** per resettare il puntatore FIFO
- Scrivere `(val & ~bit) | bit` NON garantisce il toggle
- Procedura corretta:
  ```
  ctrl0 &= ~BIT(20); write32(CTRL0, ctrl0);  // clear
  ctrl0 |= BIT(20);  write32(CTRL0, ctrl0);  // set (rising edge)
  ```

**Bit 18 (0x00040000):** Terzo secret bit. Funzione sconosciuta.

**Bit 16 (EXEC_CMD, 0x00010000):**
- Trigger per path V1
- **Richiede transizione 0→1** come FIFO_CLEAR
- Procedura corretta: clear, poi set

**Bit 15-12:** Auto-settati dall'hardware in base al valore scritto in RX_COUNT (0x4B). Non vanno modificati manualmente.

---

## Path V1 vs V2

### V1 Path (Originale)
```
1. Scrivi opcode in CTRL0[7:0]
2. Scrivi TX_COUNT (0x48)
3. Riempi FIFO (0x80+)
4. Scrivi RX_COUNT (0x4B)
5. Trigger: toggle CTRL0 bit16 (EXEC_CMD)
6. Busy wait: poll CTRL0 bit31
7. Leggi RX dal FIFO
```

### V2 Path (Linux spi-amd.c)
```
1. Scrivi opcode in OPCODE_REG (0x45)
2. Scrivi TX_COUNT (0x48)
3. Riempi FIFO (0x80+)
4. Scrivi RX_COUNT (0x4B)
5. Scrivi 0x00 a STROBE 0x49
6. Scrivi 0x00 a STROBE 0x4A
7. Trigger: toggle CMD_TRIGGER (0x47) bit7
8. Busy wait: poll CTRL0 bit31 o STATUS (0x4C) bit31 (ioread32)
9. Leggi RX dal FIFO (a offset +4)
```

### Differenze Chiave
| Aspetto | V1 | V2 |
|---------|----|----|
| Opcode register | CTRL0[7:0] | 0x45 |
| Trigger | CTRL0 bit16 | 0x47 bit7 |
| Strobe | nessuno | 0x49, 0x4A |
| Speed config | nessuna | ENA_REG (0x20) ALT_SPD + SPEED_REG (0x6C) spd7 |
| Secret bits | nessuno | CTRL0 bit 30,29,18 |
| 0x1D strobe | nessuno | AND 0xFC, OR 0x01 |

### V2 Path — Ordine Esatto (nostro driver spi-amd.c)
```
1.  read8(0x1D) → AND 0xFC → OR (cs & 0x03) → write8(0x1D)     [seleziona CS]
2.  Clear FIFO (CTRL0 bit20 toggle: clear→set)
3.  Speed config: ENA_REG (0x20) ALT_SPD + SPEED_REG (0x6C) spd7  [amd_set_spi_freq()]
4.  Write opcode to 0x45                                           [una sola scrittura]
5.  Secret bits: CTRL0 |= bits 30,29,18 → write CTRL0              [ONCE per messaggio, in amd_spi_setup_v2_regs()]
6.  write8(TX_COUNT, 0x48)
7.  Fill FIFO (0x80+)
8.  write8(RX_COUNT, 0x4B) — 0 per write, rx_len per read
9.  Trigger: write8(0x80, 0x47)
10. Wait busy: poll CTRL0 bit31
11. Read RX da FIFO a offset +4
```

**Nota:** Il driver Linux NON scrive l'opcode due volte e NON salva/ripristina 0x22 e 0x44. Il salvataggio di 0x22
e la speed config via 0x44 sono comportamenti Windows-only. La double-write dell'opcode (step 9 Windows) è
necessaria solo perché Windows sovrascrive 0x45 scrivendo 0x44 come 16-bit.

---

## FIFO Layout

```
Offset 0x80 (FIFO_BASE):
  Posizione 0-69: 70 byte di FIFO

Scrittura TX:
  writeb(data[i], base + 0x80 + i)  per i = 0..tx_len-1

Lettura RX:
  readb(base + 0x80 + 4 + i)  per i = 0..rx_len-1
  L'offset +4 è costante (verificato dal modulo diagnostico)
  Il controller sovrascrive le prime posizioni con dati RX

IMPORTANTE: Con TX_COUNT > 0, i primi TX_COUNT byte sono
  scritti nel FIFO. L'RX inizia da posizione TX_COUNT o
  sovrascrive da posizione 0 (dipende dal controller mode).
  Il modulo diagnostico legge sempre da +4 indipendentemente
  da TX_COUNT, suggerendo che RX sovrascrive da posizione 0.
```

---

## ALT_CS (0x1D) — Chip Select

```
Bit 1-0: CS_MASK (0x3) — quale chip select è attivo
Bit 7-2: Riservati / altri usi

Operazione Windows (fcn.0x3c20):
  read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
  Questo preserva bit 7-2, azzera CS (bit 1-0), setta bit 0.
  Il pattern AND 0xFC/OR 0x01 forza sempre cs=1 indipendentemente
  dal chip selezionato. Il bit 0 potrebbe essere un "enable" strobe,
  non un vero CS select.

Nostro driver (select_chip in spi-amd.c):
  read8(0x1D) → AND 0xFC → OR (cs & 0x03) → write8(0x1D)
  I bit 1-0 sono trattati come puro CS select con maschera 0x3,
  supportando cs=0,1,2,3 in base al chip SPI selezionato.
```

---

## GPIO — Interrupt Device

```
Pin: 0x15 (GPIO bank 0)
IRQ type: level-triggered, active-high (flags 0x409)
ISR: invocato dal GPIO Class Extension

Sequenza tipica:
  1. Host invia comando SPI
  2. Device processa il comando
  3. Device abbassa (o alza) la linea IRQ
  4. GPIO controller rileva l'interrupt
  5. ISR viene invocato (durata ~1-5 µs)
  6. Host fa 0x0B read per ottenere la risposta
```

---

## Speed Config (Linux — amd_set_spi_freq())

Il driver Linux usa due registri separati:

### ENA_REG (0x20) — ALT_SPD field (bit 20-23)
```
Velocità effettiva impostata in base a dev->speed_hz.
Il campo ALT_SPD determina lo speed tier (4 bit).
```

### SPEED_REG (0x6C) — spd7 field (bit 8-13)
```
Regolazione fine della velocità (6 bit).
```

### Tabella speed (da spi-amd.c righe 127-137)

| speed_hz (target) | enable_val (ENA_REG ALT_SPD) | spd7_val (SPEED_REG) |
|-------------------|------------------------------|----------------------|
| 12.5 MHz          | 0x0                          | 0x04                 |
| 25.0 MHz          | 0x1                          | 0x04                 |
| 33.0 MHz          | 0x2                          | 0x04                 |
| 50.0 MHz          | 0x3                          | 0x04                 |
| 66.0 MHz          | 0x4                          | 0x04                 |
| 80.0 MHz          | 0x5                          | 0x04                 |
| 100.0 MHz         | 0x6                          | 0x04                 |
| 133.0 MHz         | 0x7                          | 0x04                 |

### Registro 0x44 (Windows-only)

**ATTENZIONE:** Il registro a 16 bit 0x44 è usato **solo** dal driver Windows (amdspi.sys).
Scrivendolo come 16-bit (writew) sovrascrive anche l'opcode in 0x45, per questo Windows
deve riscrivere l'opcode dopo la speed config. Il driver Linux **NON** usa 0x44.

---

## Status e Busy Polling

```
BUSY POLL: ioread32(STATUS) o ioread32(CTRL0), controlla bit31
V1: poll CTRL0 bit31 (AMD_SPI_BUSY)
V2: poll CTRL0 bit31 o STATUS (0x4C) bit31 (entrambi leggibili come ioread32)
    Timeout: 2 secondi (2000000 µs), poll ogni 20 µs
    Dopo trigger: udelay(20) prima del primo poll
```

---

## Riepilogo Fix Applicati al Nostro Driver

| Fix | Registro | Prima | Dopo |
|-----|----------|-------|------|
| FIFO clear | CTRL0 bit20 | `setclear` (singolo write) | clear→set toggle |
| V2 trigger | 0x47 bit7 | CTRL0 bit16 | 0x47 bit7 toggle |
| Busy poll | — | switch su versione | unificato CTRL0 bit31 |
| RX offset | FIFO | `fifo_pos + tx_len` | `fifo_pos + 4` |
| RX_COUNT | 0x4B | `+1` per 0x0B | esatto `rx_len` |
| Strobe V2 | 0x49, 0x4A | assenti | `writeb(0x00)` a entrambi |
| V1 trigger | CTRL0 bit16 | `setclear` | clear→set toggle |

---

## PCI Config Space — FCH LPC Bridge (1022:790e, device 00:14.3)

Questi registri sono nello spazio di configurazione PCI del bridge LPC e influenzano
il comportamento del controller SPI Cezanne. **NON sono registri MMIO SPI** — sono
accessibili via PCI config space (setpci / RWEverything).

| Offset | Size | Nome | Windows Value | Linux Default | Scrivibile | Descrizione |
|--------|------|------|---------------|---------------|------------|-------------|
| 0xB4 | 32bit | FIFO_LAYOUT | 0x007DFFE0 | 0x00000000 | SI | FIFO data layout / sync byte count |
| 0xB8 | 32bit | FIFO_ACCESS | 0x33ED0084 | 0x33ED0004 | SI | bit7=1: 16-bit FIFO access mode |

### PCI 0xB8 bit7 — 16-bit FIFO Access Mode

Quando bit7=1 (valore Windows), il controller SPI Cezanne opera in modalita' FIFO a 16 bit.
I dati devono essere letti come word a 16 bit (`readw`) e i byte vanno estratti con la formula:

```c
for (i = 0; i < rx_len; i++) {
    u16 w = readw(base + read_off + (i/2)*2);
    dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
}
```

**Senza questo fix**, le letture a byte (`readb`) producono byte interlacciati (high/low).

### Comando setpci per applicare le impostazioni Windows

```bash
# Abilita 16-bit FIFO mode (scrive solo il byte basso di 0xB8)
setpci -s 00:14.3 B8.L=0x0084

# Scrive il valore completo di 0xB4
setpci -s 00:14.3 B4.L=0xFFE0
```

**Nota:** Il 16-bit FIFO mode risolve il layout dei dati in lettura ma NON le operazioni
di write (opcode 0x02), che continuano a fallire. Il meccanismo per le write e'
controllato da altri fattori non ancora identificati (possibilmente CTRL0 bits[15:8]).

### Verifica valore corrente

```bash
# Legge 0xB8
setpci -s 00:14.3 B8.L

# Legge 0xB4
setpci -s 00:14.3 B4.L
```

---

## PCI 0xB4 — FIFO Data Layout / Sync Byte Count