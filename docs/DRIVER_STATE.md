# DRIVER STATE — 2026-07-06 (FINALE + chiusura piste software)

> **Repository**: https://github.com/Syax89/SL4A_TouchScreen
> **Fonte di verita'**: [GROUND_TRUTH.md](GROUND_TRUTH.md)

---

## Stato complessivo: SOFTWARE ESAURITO

Dopo analisi esaustiva di **ogni** componente software (decompilazione driver Windows,
tracce CSV ETW, dump MMIO e PCI, test GPIO/ACPI, comparazione registro-per-registro),
**tutte le opzioni software sono state esaurite**.

---

## Read path

**FUNZIONANTE** — RESET_RSP identico a Windows (`32 10 00 5A` + `03 00 00 00`), bit-identical.
Tutte le read (opcode 0x0B) funzionano correttamente. Il device risponde con dati HID validi.

---

## Write path

**ESAURITO** — Il device ignora **qualsiasi write** (opcode 0x02) su qualsiasi registro.
Il controller AMD FCH Cezanne sotto Linux non produce un segnale write che il touchscreen
MSHW0231 riconosce.

### Test falliti (matrice completa)
- SPI modes 0, 1, 2, 3
- Velocita' 800 KHz - 33 MHz
- Trigger V1 (CTRL0 bit16)
- Trigger V2 (0x47 bit7)
- Trigger V2 hardcoded 0x80
- CTRL1 write Windows value 0x020006B5 (read-only register, ignorato)
- 0x44 dance con mask 0xF0FF, 0x0FFF
- TXMODE CTRL0 bit23
- CS1 hardcoded
- Opcode prepended al FIFO
- Full power cycle: _PS3→_PS0→_RST via ACPI
- GPIO power cycle via M010
- Vendor init @0x04 (14 byte cold boot)
- DESCREQ sincrono IRQ e workqueue
- PCI 0xB8 bit7 (16-bit FIFO mode — fixa layout dati read, NON write)
- **Tutte le combinazioni delle categorie sopra**

---

## PCI Configuration Discovery (CRITICAL)

Dump del bridge FCH LPC (1022:790e, device 00:14.3) da Windows:

### PCI 0xB4 — FIFO Data Layout
| | Value | Descrizione |
|--|-------|-------------|
| Windows | 0x7DFFE000 | FIFO data layout / sync byte count (**corretto 06/07 sera**, era trascritto male come 0x007DFFE0) |
| Linux | 0x00000000 | Default BIOS |
| Scrivibile | SI | Via setpci — retestato col valore corretto: nessun effetto sulle write |

### PCI 0xB8 — 16-bit FIFO Access Mode
| | Value | Descrizione |
|--|-------|-------------|
| Windows | 0x33ED0084 | bit7=1: 16-bit FIFO access |
| Linux | 0x33ED0004 | bit7=0: 8-bit FIFO access (default) |
| Scrivibile | SI | Via `setpci -s 00:14.3 B8.L=0x0084` |

Impostando 0xB8 al valore Windows si abilita il 16-bit FIFO mode. I dati letti
sono corretti con la formula word extraction (readw + estrazione byte pari/dispari).
**Tuttavia, le write (opcode 0x02) continuano a fallire.**

---

## CTRL0 bits[15:8] — Sospetto principale

- **Windows** = 0x0E
- **Linux** = 0xA9 (hardwired, **immutabile da software** — ogni writel ignorato dal controller)

Questi bit controllano parametri di chip select timing. Un valore errato potrebbe
invalidare il framing delle write a livello di segnale elettrico.

---

## CTRL1 — Read-only

- **Windows** = 0x020006B5
- **Linux** = 0x02000000
- La differenza nei bit bassi (0x06B5 vs 0x0000) non e' riproducibile: CTRL1 e' read-only.

---

## FIFO diagnostics

Il modulo `mmio_write.c` conferma:
- Write: i dati TX sono corretti nel FIFO, ma MISO resta **tutti zeri** (device non pilota la linea)
- Read: MISO contiene dati HID validi (sync bytes + header + body) — con PCI 0xB8 bit7=1, il layout e' corretto
- Il device **ignora selettivamente** le write, non le read

---

## ACPI verification

- DSDT md5sum identico Windows/Linux: `78046fa74c0282ee59db8b04a5204d88`
- Le tabelle ACPI sono **bit-identiche** — nessuna differenza di configurazione hardware
- SPI mode: MODE 0 (ClockPolarityLow, ClockPhaseFirst) a 33.33 MHz

---

## Sessione 06/07 sera/notte — chiusura piste software rimanenti

Senza logic analyzer ne' secondo PC (niente WinDbg a breakpoint), testate e chiuse
tutte le idee software plausibili rimaste:

- **WPP tracing di hidspi.sys** (tracepdb+traceview+tracefmt): catturata una write
  reale (`HidSetFeature`, 14B) che completa con STATUS_SUCCESS, confermata
  dall'interno del driver — conferma indipendente che opcode 0x02 funziona su
  Windows. Incrociato con "Vendor init @0x04" (gia' nella matrice test FALLITI su
  Linux, stesso registro): il blocco resta uniforme Windows-vs-Linux, non
  specifico a un registro.
- **PCI config space esteso**: confrontati Root Complex, IOMMU, SMBus, tutte le 8
  funzioni Data Fabric oltre al bridge LPC. Solo Data Fabric Fn4 (00:18.4) mostrava
  differenze reali (0x5C, 0x98, 0x9C) — testate su HW: 0x98/0x9C read-only, 0x5C
  scrivibile ma **nessun effetto** sul write path una volta applicato.
- **PCI 0xB4**: corretto un bug di trascrizione (valore vero 0x7DFFE000, non
  0x007DFFE0) e retestato — nessun effetto, pista chiusa per davvero stavolta.
- Gia' escluse in precedenza: SMN/PCI-config nel driver Windows (decomp), _OSI
  ACPI gating, WREN/SPI-NOR heritage, kernel lockdown, SME, IOMMU (probabile
  irrilevanza).

**Nessuna pista software plausibile rimane da testare.**

---

## Next step OBBLIGATORIO

**Logic analyzer** su bus SPI — confronto segnali SCK/MOSI/MISO/CS tra Windows e Linux.
Senza questa misurazione fisica, **nessun fix software e' possibile**.

---

## Strumenti diagnostici

| Tool | Path | Descrizione |
|------|------|-------------|
| `parse_spi.py` | `~/Scrivania/tools/parse_spi.py` | Parser completo CSV ETW (transazioni, timing, IRQ, init) |
| `mmio_write.c` | `~/spi-amd-v2-multi/mmio_write.c` | Raw MMIO test: write Windows-exact + FIFO dump |
| `gpio_test.ko` | modulo kernel | Test GPIO M009/M010 via ACPI evaluate |
| PCI dump | `/mnt/win/Users/simon/Desktop/spi_dump/LPC_00_14_3.BIN` | PCI config space (bridge 1022:790e) |

---

## Moduli

Compilati in `~/spi-hid/driver/` e `~/spi-amd-v2-multi/`

---

## File chiave

| File | Contenuto |
|------|-----------|
| `~/spi-hid/driver/spi-hid-core.c` | WINSEQ + GPIO in probe |
| `~/spi-amd-v2-multi/spi-amd.c` | CTRL0 fix pre-trigger |
| `~/spi-amd-v2-multi/mmio_write.c` | Raw MMIO test module |
| `~/Scrivania/tools/parse_spi.py` | CSV ETW parser |
| `~/Scrivania/traces/surface_*.csv` | Windows ETW traces |
| `~/Scrivania/decomp/` | Decomp driver Windows |
| `/mnt/win/Users/simon/Desktop/spi_dump/` | MMIO dump Windows (RWEverything) |
| `/mnt/win/Users/simon/Desktop/spi_dump/LPC_00_14_3.BIN` | PCI config space dump (bridge LPC) |
| `/mnt/win/Users/simon/Desktop/windrivers/` | hidspi.pdb/.sys, TMF WPP, log decodificati, dump PCI 11 device |
