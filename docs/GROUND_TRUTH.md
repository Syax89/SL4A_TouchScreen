# GROUND TRUTH — Modello verificato incrociando CSV ETW × decomp amdspi × decomp hidspi V0 × spec PDF

> **Data ultima verifica: 2026-07-06.** 
> Fonti: decomp hidspi.sys (PDB), amdspi.sys (no PDB, objdump), CSV ETW,
> DSDT/SSDT ACPI, test su Linux con Surface Laptop 4 AMD (Cezanne), kernel 7.1.2-3-cachyos.

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
Il decomp che abbiamo è dal driver giusto. amdspi.sys ha codice init WDF 
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

## 3. Stato driver Linux — 2026-07-06

### Read path — FUNZIONANTE ✓
- `readb` (non `readw & 0xFF`), TX_COUNT=3, offset 0x83
- 6 sync byte (non 5), header read 10B, parser a `&hdr[6]`
- Body read: `blen + 6` byte, validazione header `0x5A`
- CTRL0 = 0x60040000, no TXMODE bit23, ALT_CS CS1 (0x01)

### Write path — BLOCCATO ✗
TUTTE le differenze con Windows sono state corrette — i registri sono bit-identici:
- 0x44 dance con mask `0x00FF` (match Windows), opcode re-write dopo 0x44
- FIFO clear single-set, trigger write8(0x47, 0x80)
- 0x22 save/restore, opcode restore 0x0B post-transfer
- RX_COUNT=0 write, full-duplex test, retry loop 0x80→0x4B+0x02→0x45
- Busy poll su CTRL0 (più affidabile di Windows che non aspetta davvero)
- **Il device ignora OGNI write (opcode 0x02) — solo read (0x0B) funzionano**

### Tentativi falliti
- _RST, _PS3→_PS0→_RST, zero-touch BIOS state, delay 5s
- Vendor init @0x04, read 0x04, input register 0x04
- DESCREQ sincrono IRQ (~70µs), workqueue, 800KHz, 33MHz, CS0/CS1

### Valori registri confermati (BIOS → Linux → Windows)
| Registro | BIOS/Linux | Windows | Note |
|----------|-----------|---------|------|
| CTRL0 (0x00) | 0x6f8ca90b | +=0x60040000 | Identico |
| CTRL1 (0x0C) | 0x02000000 | MAI toccato | BIOS-only |
| ALT_CS (0x1D) | 0xB1 | (r&0xFC)\|0x01=0xB1 | Identico |
| ENA (0x20) | 0x11110713 | low nibble=3 | Identico |
| 0x44 | 0x0200 | →0x3300 dance | Identico |
| 0x45 | 0x0B/0x02 | opcode ×2 | Identico |
| 0x22 | 0x1111 | save/restore | Identico |
| Speed | 33.33 MHz | da ACPI _CRS | Identico |

---

## 4. State machine V0 (62 stati)

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

## 5. Surface_init.csv (cold boot) vs surface_boot_auto.csv (warm boot)

### Cold boot (init trace, 513 transazioni):
1. Vendor init WRITE @0x04: `02 00 00 04 82 00 00 04 00 01 01 0C EE 5B`
2. IRQ 73µs dopo → READ 0x04 → header `12 60 03 5A` (type=1 DATA!)
3. Dopo 2 cicli: header → `12 40 43 5A`, body 4281B (touch frame)
4. **NON c'è DESCREQ!** Il device va direttamente in modalità DATA
5. Più tardi: re-enumeration con comandi vendor (0xC2, 0x42, 0x82)
6. Poi DESCREQ @0x01 → DEVICE_DESC → funziona

### Warm boot (boot_auto trace):
1. RESET_RSP @0x00 → DESCREQ @0x01 → DEVICE_DESC → funziona
2. Il device è già stato inizializzato dal cold boot precedente

### Companion device (conn 0x18/0x19/0x1a):
- Inizializzato DOPO il touchscreen (gap ~962ms)
- Riceve 121 firmware block (opcode 0xB0) — NON il touchscreen
- Il touchscreen NON riceve firmware upload

---

## 6. Codifica header V0 (4 byte)

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

## 7. GPIO (da SSDT5 M009/M010)

- **M084** (GPIO base) = puntatore in CPNV OperationRegion @0x7C7A3018+8
- **M009(0x0103)**: Bank 1, Pin 3, legge `M084 + 0x1202 + pin*4`
- **M010(0x0103, v)**: Bank 1, Pin 3, scrive 2-bit field a offset 6 con `(2|v)`
- **M009(0x5B)**: Bank 0, Pin 91, legge `M084 + 0x1502 + pin*4`
- **M010(0x5B, v)**: Bank 0, Pin 91, scrive output value
- **0x0103**: RESET line (assetito LOW, rilasciato HIGH)
- **0x5B**: POWER ENABLE (HIGH = acceso, LOW = spento)

### Sequenze ACPI:
- `_INI`: se spento → accende power (0x5B=1), sleep 300ms, rilascia reset (0x0103=1)
- `_RST`: reset pulse (0x0103=0, sleep 300ms, 0x0103=1)
- `_PS0`: se era D3 → accende power, rilascia reset
- `_PS3`: se non D3 → assert reset, spegne power
- WRST/NRST: stub vuoti (NOP)

---

## 8. Verifica assenza interferenze Linux

- **Nessun driver Linux interferisce** (no upstream spi-amd, no pinctrl, no IRQ conflict)
- **Nessuna init pre-driver Windows** (SpbCx non tocca registri, _INI solo GPIO)
- **Nessun firmware upload** al touchscreen (solo companion)
- **DKMS pulito** (moduli freschi, no conflitti)

---

## 9. Conclusione

Il device **ignora qualsiasi write (opcode 0x02) su qualsiasi registro**,
mentre tutte le read (opcode 0x0B) funzionano. I registri SPI sono
bit-identici tra Windows e Linux. La sequenza MMIO è identica.

**Causa più probabile**: differenza a livello fisico/elettrico —
l'opcode 0x02 non viene riconosciuto dal device quando inviato dal
controller AMD SPI sotto Linux. Possibili cause:
1. Differenza di timing CS/clock tra Windows e Linux
2. Controller errata su questa generazione FCH (Cezanne)
3. Signal integrity a 33 MHz

**Next step**: logic analyzer per confrontare i segnali SPI tra Windows e Linux.
