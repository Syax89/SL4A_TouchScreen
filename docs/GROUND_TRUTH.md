# GROUND TRUTH — Modello verificato incrociando CSV ETW × decomp amdspi × decomp hidspi V0 × spec PDF × test HW

> **Data ultimo aggiornamento: 2026-07-06 sera/notte (finale + chiusura piste software).**
> Fonti: decomp hidspi.sys (PDB), amdspi.sys (no PDB, objdump), CSV ETW,
> DSDT/SSDT ACPI, test su Linux con Surface Laptop 4 AMD (Cezanne), kernel 7.1.2-3-cachyos,
> dump MMIO Windows via RWEverything, dump PCI config space Windows via RWEverything
> (11 device), tracing WPP interno a hidspi.sys (tracepdb/traceview/tracefmt).

---

## 0. PCI Configuration Discovery (CRITICAL — da Windows dump LPC_00_14_3.BIN)

Il bridge FCH LPC (1022:790e, device 00:14.3) ha differenze nello spazio di
configurazione PCI tra Windows e Linux che influenzano il controller SPI:

| PCI Offset | Windows Value | Linux Value | Effetto |
|-----------|---------------|-------------|--------|
| 0xB4 | 0x7DFFE000 | 0x00000000 | FIFO data layout / sync byte count |
| 0xB8 | 0x33ED0084 | 0x33ED0004 | bit7 (0x80) = 16-bit FIFO access mode |

Entrambi i registri sono **SCRIVIBILI** via setpci su Linux. Impostando 0xB8 al
valore Windows si abilita il **16-bit FIFO mode**, che risolve il problema del
layout dei dati di lettura (vedi sezione 0.1).

---

## 0.1. 16-bit FIFO Read Formula

Con PCI 0xB8 bit7=1, il FIFO restituisce dati in word a 16 bit. L'estrazione
corretta dei byte e':

```c
for (i = 0; i < rx_len; i++) {
    u16 w = readw(base + read_off + (i/2)*2);
    dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
}
```

Questo produce il corretto ordinamento dei byte: `FF FF FF FF FF FF 32 10 00 5A`
(che corrisponde alla risposta HID attesa). Senza questa formula, `readb` produce
byte high/low interlacciati: `FF 10 FF 5A FF 00...`.

---

## 0.2. Header Parser Fix

Modificato `spi_hid_seq_hdr_type()` per cercare 0x5A dinamicamente invece che a
offset fisso. Questo supporta entrambi i layout con 3 e 6 sync byte.

---

## 0.3. Verifica ACPI: Tabelle IDENTICHE Windows/Linux

Il DSDT dumpato da Windows e da Linux ha lo stesso md5sum: `78046fa74c0282ee59db8b04a5204d88`.
Le tabelle ACPI (DSDT, SSDT1-7) sono **bit-identiche**. Non c'e' alcuna differenza di configurazione
hardware tra i due OS — il BIOS espone esattamente le stesse risorse, GPIO, indirizzi MMIO e _DSM.

SPI mode confermato: **MODE 0** (ClockPolarityLow, ClockPhaseFirst) a 33.33 MHz.
GPIO: POWER=0x5B, RESET=0x0103, IRQ=0x55.

**Conclusione**: il problema NON e' nelle tabelle ACPI. Qualsiasi differenza di comportamento e' a
livello di driver/controller o di configurazione PCI del bridge LPC.

---

## 1. Architettura driver Windows

```
┌─────────────────────────────────────────┐
│ hidspi.sys (143KB) — HID protocol layer │
│ Contiene TUTTO il codice MMIO per V0    │
│ ✅ DECOMPILATO (con PDB)                │
│ Funzioni: 0x3c20 (transfer), 0x4bac,    │
│ 0x54d0 (submit_handler), 0x3528 (sub)   │
│ Percorso V0: MMIO DIRETTO, no SpbCx     │
└────────────┬────────────────────────────┘
             │ (V0 path: direct MMIO)
             ▼
┌─────────────────────────────────────────┐
│ amdspi.sys (232KB) — AMD FCH SPI driver │
│ Contiene stesso codice MMIO + init WDF  │
│ ❌ MAI DECOMPILATO (no PDB)             │
│ MA: init = solo boilerplate WDF         │
│ NESSUN registro SPI toccato nell'init   │
│ Usato solo per path V1/SpbCx            │
└────────────┬────────────────────────────┘
             ▼
    AMD FCH SPI Controller HW
         (MMIO 0xFEC10000)
```

Il device **MSHW0231** usa il path **HidSpiDeviceV0** (pre-release).
Il decomp che abbiamo e' dal driver giusto. amdspi.sys ha codice init WDF
che non tocca registri SPI — verificato via objdump: nessun `add rax, 0x1D`
o simile nelle sezioni INIT/PAGE.

---

## 2. Sequenza esatta Windows write path (0x54d0/0x4bac) — MMIO trace

```
1.  save 0x22:       readw(base+0x22), store context
2.  ALT_CS:          read8(0x1D) → &0xFC → |0x01 → write8(0x1D)
3.  FIFO clear:      read32(CTRL0) → |= BIT20 → write32(CTRL0)   [single-set, no toggle]
4.  opcode #1:       write8(0x45, opcode)
5.  secret bits:     read32(CTRL0) → |= 0x60040000 → write32     [bit23 MAI toccato]
6.  0x44 dance:      read16(0x44); n = read8(0x20) & 0xF;
                    w = (w & 0x00FF) | (n<<8) | (n<<12);
                    write16(0x44, w)                              [clobbers 0x45!]
7.  opcode #2:       write8(0x45, opcode)                          [re-write after 0x44]
8.  TX_COUNT:        write8(0x48, tx_len)
9.  FIFO fill:       write8(0x80+i, data[i]) for i in 0..tx_len-1
10. RX_COUNT:        write8(0x4B, 0)                               [TX-only per write]
11. trigger:         write8(0x47, 0x80)
12. poll:            read8(0x4C), check bit31 busy [Windows: buggato/no-op]
13. restore 0x22:    writew(base+0x22, saved)
14. restore opcode:  write8(0x45, original_opcode)                 [0x4684 teardown]
15. ALT_CS exit:     read8(0x1D) → &0xFC → write8                  [deselect CS]
```

**Read path (0x3c20):** identico tranne:
- TX_COUNT=3 (3 byte indirizzo), NIENTE 0x44 dance
- RX_COUNT=rx_len, risposta a FIFO+TX_COUNT (0x83)

---

## 3. Stato driver Linux — 2026-07-06 (FINAL)

### Read path — FUNZIONANTE ✓
- `readb` (non `readw & 0xFF`), TX_COUNT=3, offset 0x83
- 6 sync byte (non 5), header read 10B, parser a `&hdr[6]`
- Body read: `blen + 6` byte, validazione header `0x5A`
- CTRL0 = 0x60040000, no TXMODE bit23, ALT_CS CS1 (0x01)
- **RESET_RSP header+body bit-identici a Windows**

### Write path — ESAURITO ✗
TUTTE le differenze con Windows sono state corrette — i registri sono bit-identici:
- 0x44 dance con mask `0x00FF` (match Windows), opcode re-write dopo 0x44
- FIFO clear single-set, trigger write8(0x47, 0x80)
- 0x22 save/restore, opcode restore 0x0B post-transfer
- RX_COUNT=0 write, full-duplex test, retry loop 0x80→0x4B+0x02→0x45
- Busy poll su CTRL0 (piu' affidabile di Windows che non aspetta davvero)
- **Il device ignora OGNI write (opcode 0x02) — solo read (0x0B) funzionano**

### Matrice completa dei test falliti

| Categoria | Test | Risultato |
|-----------|------|-----------|
| **SPI Mode** | Mode 0, 1, 2, 3 | Tutti falliti |
| **SPI Speed** | 800 KHz, 12.5 MHz, 25 MHz, 33 MHz | Tutti falliti |
| **Trigger V1** | CTRL0 bit16 toggle | Fallito |
| **Trigger V2** | 0x47 bit7 toggle | Fallito |
| **Trigger V2 hardcoded** | 0x80 fisso | Fallito |
| **CTRL1** | write 0x020006B5 (valore Windows) | Ignorato (register read-only) |
| **0x44 dance** | con mask 0xF0FF, 0x0FFF | Fallito |
| **TXMODE** | CTRL0 bit23 | Fallito |
| **CS1 hardcoded** | (reg & 0xFC) \| 0x01 | Fallito |
| **FIFO opcode prepend** | opcode in FIFO prima dei dati | Fallito |
| **Power cycle** | _PS3→_PS0→_RST via ACPI | Fallito |
| **GPIO power cycle** | M010 OFF/ON via SSDT | Fallito |
| **Vendor init** | 0x02 @0x04 (14 byte cold boot) | Fallito |
| **DESCREQ sync IRQ** | ~70us delay | Fallito |
| **DESCREQ workqueue** | deferred execution | Fallito |
| **PCI 0xB8 bit7** | 16-bit FIFO mode (Windows value) | Fixa layout dati read, NON write |
| **Combinazioni** | Tutte le combinazioni delle categorie sopra | Tutti falliti |

---

## 4. Diagnostica FIFO

Il modulo `mmio_write.c` esegue dump completi del FIFO da 70 byte dopo ogni operazione.

### Write (opcode 0x02) — FIFO pre/post write
- **FIFO pre-write**: contiene i dati TX corretti (primo byte = opcode 0x02, dati payload)
- **MOSI confermato**: i byte vengono scritti correttamente sul registro FIFO
- **MISO post-write**: **TUTTI ZERI** per l'intero dump di 70 byte
- Il device NON pilota MISO durante le write — la linea resta undriven/floating

### Read (opcode 0x0B) — FIFO pre/post read
- **FIFO pre-read**: 3 byte dummy (indirizzo), RX_COUNT programmato
- **MISO post-read**: contiene i dati HID corretti (sync bytes + header + body)
- Lettura da offset `0x80 + TX_COUNT` funziona perfettamente

### Conclusione diagnostica FIFO
Il controller AMD FCH scrive correttamente i dati TX sul bus MOSI.
Il device touchscreen **ignora selettivamente** le transazioni con opcode 0x02:
- MISO resta a zero (undriven/floating) per tutta la durata della write
- Nessun ACK, nessun errore, nessuna risposta di alcun tipo
- Le read (0x0B) funzionano immediatamente prima e dopo la write fallita

---

## 5. Comparazione MMIO Windows vs Linux

Dump ottenuti con RWEverything su Windows (4 dump: IDLE, TOUCH1, TOUCH2, TOUCH3)
e con modulo diagnostico su Linux.

| Registro | Offset | BIOS/Linux | Windows | Match | Note |
|----------|--------|-----------|---------|-------|------|
| CTRL0 | 0x00 | 0x6f8ca90b | +=0x60040000 | ✅ | bits[15:8] differ: 0xA9 (Linux) vs 0x0E (Windows) — **hardwired, immutabile** |
| CTRL1 | 0x0C | 0x02000000 | 0x020006B5 | ⚠️ | Windows ha 0x06B5 nei bit bassi, ma CTRL1 e' **read-only** da software |
| ALT_CS | 0x1D | 0xB1 | (r&0xFC)\|0x01=0xB1 | ✅ | Identico |
| ENA | 0x20 | 0x11110713 | low nibble=3 | ✅ | Identico |
| 0x44 | 0x44 | 0x0200 | →0x3300 dance | ✅ | Identico |
| 0x45 | 0x45 | 0x0B/0x02 | opcode ×2 | ✅ | Identico |
| 0x22 | 0x22 | 0x1111 | save/restore | ✅ | Identico |
| Speed | — | 33.33 MHz | da ACPI _CRS | ✅ | Identico |

### CTRL0 bits[15:8] — Il sospetto principale

- **Windows**: 0x0E
- **Linux**: 0xA9
- **Comportamento**: ogni writel a CTRL0 con bits[15:8] modificati viene **ignorato** dal controller hardware. Il read immediato dopo la write mostra sempre 0xA9.
- **Impatto**: questi bit controllano parametri di chip select timing. Un valore errato potrebbe invalidare il framing delle write a livello elettrico.
- **Causa**: il valore e' impostato dal BIOS al boot e il controller FCH Cezanne non permette di modificarlo via MMIO. Non e' chiaro come Windows ottenga 0x0E — possibile che il driver Windows riconfiguri il controller via un meccanismo non-MMIO (PCI config space, SMBus, o altro).

### CTRL1 — Read-only

Il valore Windows 0x020006B5 (vs Linux 0x02000000) non e' riproducibile:
la write a CTRL1 viene ignorata dal controller. Il registro e' read-only da software.

---

## 6. FIFO dump Windows vs Linux

### Windows (da RWEverything dump)
Il FIFO su Windows contiene ancora dati residui del vendor init command
(`02 00 00 04 82 00 00 04 00 01 01 0C EE 5B`) dopo il boot completo.
Questo conferma che:
- Il controller Windows lascia i dati TX nel FIFO
- Il device non modifica il FIFO durante le write
- Il comportamento del FIFO e' identico a Linux

---

## 7. CSV Windows completamente analizzati

### surface_boot_auto.csv (warm boot, 2384 transazioni)
```
RESET_RSP @0x00 → drain ACK → DESCREQ @0x01 → IRQ → DEVICE_DESC type=7
→ DESCREQ2 @0x02 → IRQ → RPT_DESC type=8 → ~962ms gap
→ attivazione companion device (NON touchscreen)
→ DATA runtime (type=1)
```

### surface_init.csv (cold boot, 513 transazioni)
```
Vendor init WRITE @0x04 FIRST → IRQ 73us → READ 0x04 → DATA type=1 immediato
→ NESSUN DESCREQ nel cold boot!
→ Il device entra in modalita' DATA direttamente dopo vendor init
```

### surface_touch.csv (runtime touch)
- 4304 byte full frame reports
- 216 byte heatmap frame reports
- Alternanza ogni ~10 frame
- **Approval bytes**: a8=0x00 (init), a8=0x03 (post DESCREQ), a8=0x04 (runtime)
- I "byte di approval" sono in realta' eco dell'ultima write, non costanti di protocollo
- **Device descriptor**: Vendor=0x045E (MS), Product=0x0C19, Version=0x0100

---

## 8. State machine V0 (62 stati)

Flusso enumerazione standard:
```
resettingSync → _RST (GPIO reset 300ms)
  → readResetResponse → header+body RESET_RSP (type=3)
  → writeDescriptorRequest → DESCREQ @0x01 (10 byte, opcode 0x02)
  → readDescriptorResponse → DEVICE_DESC (type=7)
  → verifyDescriptor → DESCREQ2 @0x02
  → readDescriptorResponse → RPT_DESC (type=8)
  → ready → DATA (type=1)
```

Su RESET_RSP ripetuta: il device segnala errore → host RIPARTE DAL DESCREQ.

---

## 9. Codifica header V0 (4 byte)

```
byte[0] = (type << 4) | 2      type: 1=DATA, 3=RESET_RSP, 7=DEVICE_DESC, 8=RPT_DESC
byte[1-2] = body_len * 4 (LE)  es: 0x0010/4=4 (RESET_RSP), 0x0EB0/4=940 (RPT_DESC)
byte[3] = 0x5A (V2 sync)
```

### Header osservati:
| Header | Type | Body len | Contesto |
|--------|------|----------|----------|
| `12 60 03 5A` | 1 (DATA) | 216 | Cold boot iniziale |
| `12 40 43 5A` | 1 (DATA) | 4304 | Touch frame pieno |
| `32 10 00 5A` | 3 (RESET_RSP) | 4 | Reset response |
| `72 80 00 5A` | 7 (DEVICE_DESC) | 32 | Device descriptor |
| `82 B0 0E 5A` | 8 (RPT_DESC) | 940 | Report descriptor |

---

## 10. GPIO (da SSDT5 M009/M010)

- **M084** (GPIO base) = puntatore in CPNV OperationRegion @0x7C7A3018+8
- **M009(0x0103)**: Bank 1, Pin 3, legge `M084 + 0x1202 + pin*4`
- **M010(0x0103, v)**: Bank 1, Pin 3, scrive 2-bit field a offset 6 con `(2|v)`
- **M009(0x5B)**: Bank 0, Pin 91, legge `M084 + 0x1502 + pin*4`
- **M010(0x5B, v)**: Bank 0, Pin 91, scrive output value
- **0x0103**: RESET line (assertito LOW, rilasciato HIGH)
- **0x5B**: POWER ENABLE (HIGH = acceso, LOW = spento)

### Sequenze ACPI:
- `_INI`: se spento → accende power (0x5B=1), sleep 300ms, rilascia reset (0x0103=1)
- `_RST`: reset pulse (0x0103=0, sleep 300ms, 0x0103=1)
- `_PS0`: se era D3 → accende power, rilascia reset
- `_PS3`: se non D3 → assert reset, spegne power
- WRST/NRST: stub vuoti (NOP)

**M009/M010 verificati funzionanti da Linux** — power cycle HW non sblocca le write.

---

## 11. Strumenti diagnostici creati

| Tool | Path | Descrizione |
|------|------|-------------|
| `parse_spi.py` | `~/Scrivania/tools/parse_spi.py` | Parser completo CSV ETW: transazioni SPI byte-by-byte, timing, IRQ, sequenze init |
| `mmio_write.c` | `~/spi-amd-v2-multi/mmio_write.c` | Modulo kernel raw MMIO: write Windows-exact bypassando framework SPI Linux, con dump FIFO |
| `parse_spb_csv.py` | `~/Scrivania/tools/parse_spb_csv.py` | Parser alternativo per CSV SPB |
| `decode_hidspi.py` | `~/Scrivania/tools/decode_hidspi.py` | Decoder header HID-over-SPI |
| `gpio_test.ko` | modulo kernel | Test M009/M010 GPIO via ACPI evaluate |

---

## 12. Verifica assenza interferenze Linux

- **Nessun driver Linux interferisce** (no upstream spi-amd, no pinctrl, no IRQ conflict)
- **Nessuna init pre-driver Windows** (SpbCx non tocca registri, _INI solo GPIO)
- **Nessun firmware upload** al touchscreen (solo companion device su connessione SPB diversa)
- **DKMS pulito** (moduli freschi, no conflitti)

---

## 13. Conclusione Finale

Dopo ore di analisi esaustiva di ogni componente software — decompilazione driver Windows,
tracce CSV ETW, dump MMIO e PCI, test GPIO/ACPI, comparazione registro-per-registro — **tutte
le opzioni software sono state esaurite**.

Il device **ignora qualsiasi write (opcode 0x02) su qualsiasi registro**, mentre tutte
le read (opcode 0x0B) funzionano. I registri SPI sono bit-identici tra Windows e Linux
(tranne CTRL0 bits[15:8] che e' hardwired). La sequenza MMIO e' identica.

### PCI 0xB8 bit7 (16-bit FIFO mode): progresso PARZIALE

Il dump PCI del bridge FCH LPC (1022:790e, device 00:14.3) ha rivelato che il bit7
dell'offset 0xB8 controlla la modalita' di accesso al FIFO:
- **Windows** (0xB8=0x33ED0084, bit7=1): 16-bit FIFO mode → dati letti come word
- **Linux default** (0xB8=0x33ED0004, bit7=0): 8-bit FIFO mode → dati interlacciati

Impostando 0xB8 al valore Windows con setpci:
```
setpci -s 00:14.3 B8.L=0x0084
```
...si ottiene il corretto layout dei dati in lettura. Tuttavia, le **write (opcode 0x02)
continuano a non funzionare**. Il 16-bit FIFO mode risolve il problema del layout dati
ma NON il problema delle write.

### PCI 0xB4

Il registro PCI 0xB4 (Windows: 0x7DFFE000 — **CORRETTO 06/07 sera**: il valore
riportato in precedenza (0x007DFFE0) era un errore di trascrizione byte-order,
verificato leggendo i byte raw del dump con xxd: `b4-b7 = 00 e0 ff 7d` LE =
0x7DFFE000; Linux: 0x00000000) controlla il layout dei dati FIFO e il conteggio
dei sync byte. E' scrivibile via setpci. **Retest 06/07 sera con il valore
CORRETTO** (`setpci -s 00:14.3 B4.L=0x7DFFE000`, insieme a B8 col valore pieno
`0x33ED0084`): nessun effetto sulle write — il test precedente (fatto con
`B4.L=0xFFE0`, che scriveva 0x0000FFE0, ne' il valore giusto ne' quello
erroneamente documentato) non era valido; questo retest lo e' e conferma la
chiusura della pista.

### Causa piu' probabile

Il controller AMD FCH Cezanne sotto Linux non produce un segnale write valido
che il touchscreen MSHW0231 riconosce. Possibili cause:
1. **CTRL0 bits[15:8] = 0xA9 vs 0x0E** — differenza di chip select timing
2. **CTRL1 bits[15:0] = 0x0000 vs 0x06B5** — differenza in configurazione controller (read-only)
3. **Timing CS/clock** — differenze a livello di segnale fisico tra i due OS
4. **Inizializzazione controller** — Windows potrebbe fare init PCI/SMBus non visibile via MMIO
5. **PCI config space** — altri offset non ancora scoperti che influenzano il comportamento write

### Next step OBBLIGATORIO

**Logic analyzer** per confrontare i segnali SCK/MOSI/MISO/CS tra Windows e Linux.
Senza questa misurazione fisica, non e' possibile determinare la differenza di segnale
e nessun fix software puo' risolvere il problema.

---

## 15. Sessione 06/07 sera/notte — chiusura definitiva delle piste software

Sessione senza logic analyzer ne' secondo PC (niente WinDbg a breakpoint), con
l'obiettivo esplicito di esaurire ogni pista "solo software" rimasta.

### 15.1 Correzione bug documentazione: PCI 0xB4

Il valore Windows di 0xB4 riportato ovunque (`0x007DFFE0`) era un **errore di
trascrizione**: il valore reale, letto byte-per-byte dal dump raw, e'
`0x7DFFE000`. Di conseguenza il test precedente "0xB4 fallito" non era mai stato
eseguito col valore giusto (il comando usato scriveva `0x0000FFE0`). Rifatto con
`setpci -s 00:14.3 B4.L=0x7DFFE000` + `B8.L=0x33ED0084` (valori pieni): **nessun
effetto** — CTRL0/CTRL1 pre-trigger identici, DESCREQ ancora ignorato. Stavolta
il test e' valido, la pista e' chiusa per davvero.

### 15.2 WPP tracing di hidspi.sys (via tracepdb+traceview+tracefmt, WDK)

Estratti i file TMF dal PDB (`hidspi.pdb` contiene info WPP complete: 7 GUID di
messaggio, ~185 stringhe di formato, classi `HidSpiDeviceV0`, `SpbBusWrapper`,
`Fdo`, `CxClient`). Catturate due sessioni reali (ciclo disabilita/abilita e
disinstalla/rileva hardware in Device Manager) e decodificate con `tracefmt.exe`.

**Risultato**: entrambe le catture mostrano solo un resume D2→D0 (mai un vero
cold reset con RESET_RSP→DESCREQ — nemmeno "Disinstalla dispositivo" forza lo
spegnimento elettrico del device). Sequenza osservata: `EvtD0Entry` → vendor-init
14B (registro 0x000004) → `HidGetFeature Report 6` (10B+129B, risposta valida) →
**`HidSetFeature Report 5` (14B) completa con STATUS_SUCCESS** — una vera write
che riesce, confermata dall'interno del driver stesso (state machine SmFx pulita,
zero retry). Riproducibile in entrambe le catture.

**Perche' conta**: e' la prima conferma **indipendente dal punto di vista del
driver stesso** (non solo dump MMIO esterni RWEverything) che l'opcode 0x02
funziona davvero su Windows su questo stesso hardware. Incrociato con la
matrice test (che include "Vendor init @0x04, 14 byte" tra i FALLITI su Linux,
stesso registro/pacchetto): il blocco NON e' specifico al registro 0x000001/
DESCREQ, e' uniforme su Linux (fallisce ovunque) e uniforme su Windows
(funziona ovunque osservato) — asimmetria Windows-vs-Linux, non registro-per-
registro.

### 15.3 Confronto PCI config space esteso: 11 device totali

Oltre al bridge LPC (00:14.3, gia' fatto), confrontati byte-per-byte: Root
Complex (00:00.0), IOMMU (00:00.2), SMBus (00:14.0), tutte le 8 funzioni Data
Fabric (00:18.0-7). Comandi RWEverything via console Command (`SAVE <file>.BIN
PCI <Bus#dec> <Dev#dec> <Fun#dec>` — Dev# in decimale, 0x14→20, 0x18→24).

| Device | Esito |
|--------|-------|
| SMBus (00:14.0) | Identico |
| Data Fabric Fn 0,1,2,3,5,6,7 | Identici |
| Data Fabric **Fn4** (00:18.4, PCI ID 1022:144c) | 3 offset diversi: 0x5C, 0x98, 0x9C |
| Root Complex (00:00.0) | 31 byte diversi ma pattern da stato/contatore volatile (stesso valore ripetuto a piu' offset), scartato |
| IOMMU (00:00.2) | Windows legge tutto `0xFFFFFFFF` (device non risponde a quella BDF sotto Windows), irrilevante per SPI |

### 15.4 Data Fabric Fn4 — testato su HW, nessun effetto

Cercata documentazione pubblica AMD (OSRR) e kernel Linux: Fn4 e' una funzione
reale (usata da Linux per maschere partizionamento cache L3, ma a offset 0x1D4,
fuori dai 256B confrontati) — nessuna doc per 0x5C/0x98/0x9C specificamente.
Notare che a 0x98/0x9C e' **Linux** ad avere il bit alto settato (`0x80000001`),
Windows ha `0x00000000` — verso opposto rispetto alla narrativa "Windows abilita
qualcosa che manca a Linux".

Testato su HW (laptop di test, un registro alla volta con verifica stabilita'):
- 0x98, 0x9C: **read-only**, scrittura ignorata dall'hardware (come CTRL0[15:8]/CTRL1)
- 0x5C: scrittura **accettata** (portato al valore Windows `0x000a0085`), sistema stabile
- Ricaricato spi-amd+spi-hid con 0x5C modificato: **nessun effetto**, DESCREQ
  ancora ignorato, CTRL0 pre-trigger invariato. Valore ripristinato a fine test.

### 15.6 Ricerca esterna: community linux-surface e meccanismo "AMD SPI Lock"

Verificato lo stato della community `linux-surface`: il touchscreen SPI su
Surface Laptop 3/4 AMD e' un problema **noto e mai risolto neanche da loro**
(repo `linux-surface/spi-hid`, adattamento del driver Surface Duo 2, mai
completato per device ACPI-based per mancanza di hardware — "We very briefly
tried to make that work on the AMD/ACPI-based devices, but it didn't work out").
Il nostro progetto e' piu' avanti del loro tentativo ufficiale. Nessun fix
nascosto da riusare. Unico suggerimento utile dalle loro discussioni: provare
`iommu=off`/`amd_iommu=off` (coincide con variabile gia' isolata ma non ancora
testata in questa sessione).

Trovato e verificato il meccanismo **"AMD SPI Lock"** (documentato da AMD,
usato da fwupd per rilevare BIOS write-protection): tre componenti —
`RomProtect` (bridge LPC PCI offset **0x50**), `SpiProtectEn0`/`SpiProtectLock`
(il nostro **ALT_CS, MMIO 0x1D**), `SPIRestrictedCmd`/`SPIRestrictedCmd2` (MMIO
**0x04/0x08**, lista di opcode SPI bloccati). Sembrava un candidato forte
(blocco scrittura hardware documentato, indipendente dal registro target).
**Verificato con dati reali** (dump gia' presenti nel progetto + letture fresh):

| Registro | Windows | Linux | Esito |
|----------|---------|-------|-------|
| RomProtect (LPC 0x50) | `05 05 fa ff 00 00 00 00` | `05 05 fa ff 00 00 00 00` | Identico |
| ALT_CS / SpiProtectEn0-Lock (0x1D) | `0xB1` | `0xB1` | Identico |
| SPIRestrictedCmd (MMIO 0x04) | `0x00000006` | `0x00000006` | Identico |
| SPIRestrictedCmd2 (MMIO 0x08) | `0x00000000` | `0x00000000` | Identico |

Tutti e quattro i componenti del meccanismo sono byte-identici tra Windows e
Linux — reale e ben documentato, ma su questa macchina non e' la causa. Pista
chiusa con dati verificati, non solo teoria.

### 15.8 Protocollo: conferma esterna "0x0100 legacy" + opcode 0x07 (SET_POWER) ritestato

Il PDF della spec pubblica v1.0 (pag.16, ASL di esempio) dice esplicitamente
"Revision ID – Will be 3 for HidSpi V1"; il nostro `_DSM` risponde revision 1,
che per la spec stessa e' "unsupported revision" — conferma che MSHW0231 non
implementa la v1.0 pubblica ma qualcosa di precedente/non documentato.

Trovato un thread LKML 2022 (patchew.org, proposta upstream spi-hid di un
ingegnere Microsoft) in cui **Maximilian Luz (maintainer linux-surface)
conferma esplicitamente che i device AMD/x86 usano
`SPI_HID_SUPPORTED_VERSION 0x0100`**, non 0x0300 (la versione del driver
proposto/spec pubblica) — conferma esterna indipendente, non solo nostra
deduzione dal _DSM. Citato un repository Microsoft reale (Surface Duo,
Qualcomm) con codice per 0x0100: la write li' e' un singolo transfer TX-only
half-duplex (framing gia' testato da noi, fallito), header 6 byte identico
al nostro modello — validazione strutturale, nessuna pista nuova. Nessun
quirk AMD presente (codice specifico Qualcomm/pinctrl).

Trovato inoltre, in un branch ABBANDONATO del nostro progetto (pacchetto DKMS
`/usr/src/spi-hid-0.1`, mai incorporato nel driver attivo), un opcode mai
testato in questa sessione: **0x07 (SET_POWER stile HidSpiCx)**, con un
commento che ne rivendicava un funzionamento empirico — ma per un problema
diverso dal nostro (device completamente addormentato, tutte le read a 0xFF;
nel nostro caso le read funzionano gia' bene dall'inizio). Ritestato su HW
oggi: risposta MISO `0xFF` (diverso dagli `0x00` delle write 0x02) ma
**nessun effetto reale** sul seguito — DESCREQ continua a fallire identico.
0xFF quasi certamente bus floating, non risposta device. Pista chiusa.

### 15.9 Ipotesi "modulo fantasma pre-boot" indagata e chiusa

Sospetto valido sollevato dall'utente: un modulo si autocaricava ad ogni boot (visto per
tutta la sessione) — possibile stesse corrompendo l'hardware prima dei nostri test puliti
(precedente reale del 04/07: poking CTRL0 mise il controller in uno stato corrotto
sopravvissuto a rmmod, servi' un reboot). Indagine: quel modulo NON era il DKMS abbandonato
(installato solo per il kernel LTS) ma una **vecchia build del nostro stesso progetto,
congelata dentro l'INITRAMFS ATTIVO**, caricata a 1.24s di boot (prima del filesystem
reale), invisibile a rmmod/ricerche su disco. Rigenerato l'initramfs pulito e sincronizzato
l'hash in `/boot/limine.conf` (via `limine-entry-tool`, necessario perche' Limine verifica
l'integrita' via hash). **Test decisivo su boot genuinamente vergine** (confermato via
`/sys/bus/acpi/devices/AMDI0060:00/` — nessun driver, nessun modulo, nessun device SPI,
dmesg silenzioso): risultato IDENTICO a sempre, DESCREQ ignorato, MISO zero. L'ipotesi e'
esclusa con la prova piu' pulita possibile — il fix all'initramfs resta comunque valido
(era un vero problema di igiene di sistema) ma non cambia il verdetto di fondo.

### 15.5/15.7 Conclusione definitiva

Con 15.1-15.4 e 15.6 si chiude, in modo verificato e pulito (nessun test invalido come
in passato), **ogni pista software rimasta**: PCI config space (bridge LPC E
Data Fabric), WPP/tracing interno al driver, _OSI/ACPI, WREN/SPI-NOR heritage,
kernel lockdown/SME/IOMMU (testato dal vivo con boot dedicato `iommu=off
amd_iommu=off` — AMD-Vi confermato disattivo, nessun effetto), meccanismo AMD
SPI Lock (RomProtect/SpiProtectEn0/SPIRestrictedCmd). Nessuna di queste ha prodotto un effetto sul write
path. Il verdetto resta quello del punto 13: **serve la misura fisica (logic
analyzer su MOSI/CS/SCK)** o una cattura WinDbg a breakpoint reale (richiede un
secondo PC, non disponibile). Non risultano ulteriori idee software plausibili
da testare.

---

## 14. Riferimenti

| Risorsa | Path |
|----------|------|
| Driver Linux SPI | `~/spi-amd-v2-multi/spi-amd.c` |
| Driver Linux HID | `~/spi-hid/driver/spi-hid-core.c` |
| CSV ETW Windows | `~/Scrivania/traces/surface_*.csv` |
| Decomp hidspi.sys | `~/Scrivania/decomp/clean/` |
| Decomp amdspi.sys | `~/Scrivania/decomp/amdspi/` |
| MMIO dump Windows | `/mnt/win/Users/simon/Desktop/spi_dump/` |
| PCI dump Windows | `/mnt/win/Users/simon/Desktop/spi_dump/LPC_00_14_3.BIN` |
| ACPI Linux | `~/Scrivania/acpi_linux/` |
| ACPI Windows | `~/Scrivania/acpi_windows/` |
| Parser CSV | `~/Scrivania/tools/parse_spi.py` |
| MMIO test module | `~/spi-amd-v2-multi/mmio_write.c` |
| WPP TMF + PDB + log decodificati | `/mnt/win/Users/simon/Desktop/windrivers/` (hidspi.pdb/.sys, *.tmf, logtouch*.etl/_decoded*.txt) |
| Dump PCI 11 device (Windows .BIN + riferimento Linux) | `/mnt/win/Users/simon/Desktop/windrivers/` e `linux_pci_reference/` |
| Repository GitHub | https://github.com/Syax89/SL4A_TouchScreen |
