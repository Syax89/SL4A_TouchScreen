# MSHW0231 HID-over-SPI Driver — Documentazione Completa

> **Repository**: https://github.com/Syax89/SL4A_TouchScreen
> **Ultimo commit**: v5 (587d0a1) - SPI working, DESCREQ needs TX+RX opcode fix
> **File correlati**: [CSV_SEQUENCE.md](CSV_SEQUENCE.md) | [SPI_REGISTERS.md](SPI_REGISTERS.md) | [HIDSPI_PROTOCOL.md](HIDSPI_PROTOCOL.md)

---

## 1. Hardware

| Caratteristica | Valore |
|---------------|--------|
| Dispositivo | Surface Laptop 4 AMD touchscreen |
| ACPI ID (HID) | `MSHW0231` |
| ACPI ID (SPI) | `AMDI0060` |
| Controller SPI | AMD FCH V2 |
| MMIO Base | `0xFEC10000` |
| FIFO | 70 byte @ `0x80` |
| GPIO Interrupt | Pin 0x15, level-triggered active-high (flags 0x409) |
| SPI Speed | 33.33 MHz |
| Vendor/Product | 0x045E / 0x0C19 |

---

## 2. Stato Attuale

### ✅ FUNZIONANTE
- **SPI comunica**: fix FIFO_CLEAR toggle, V2 trigger 0x47, strobe 0x49/0x4A
- **Device risponde**: ACK `03 00 00 00` ricevuto correttamente
- **Header detection**: ACK pattern riconosciuto come type=0
- **State machine avanza**: state 0 → DESCREQ → state 1
- **ACPI table**: MSHW0231 matches via PNP0C51 fallback
- **No black screen**: `gpio_vendor_init` rimosso (ioremap 0xFED80000)

### ❌ DA RISOLVERE
1. **DESCREQ non produce type=7**: TX opcode 0x02 + RX separata opcode 0x0B
   - Windows usa TX+RX **inline con stesso opcode** (0x02 per entrambi)
   - La 0x0B read separata è un nuovo comando, non la completion di 0x02
   - Il device risponde con ACK ma non avanza a DEVICE_DESC
2. **Firmware non testato** con le fix SPI attuali
3. **Nessun DESCREQ2** (il codice c'è ma state 1 non raggiunge mai type=7)
4. **input_register per letture iniziali è 0x1000**: Windows usa 0x0000 prima del descriptor parse (approval byte7=0x00, register=0x0000). Il driver usa `SPI_HID_DEFAULT_INPUT_REGISTER 0x1000` (spi-hid-core.h:76) e non cambia mai a 0x0000 durante lo stato 0.
5. **Bug `memcpy` nel parse descrittore (state 1)**: `memcpy(&raw, hdr+off, s)` copia i 4 byte dell'header HID invece del body (spi-hid-core.c:1237). Dovrebbe essere `hdr+off+4`. Il parse risulta corrotto — `wVendorID`, `wProductID` e tutti i campi del descrittore vengono letti 4 byte sfalsati.

### Teoria di Fix
- TX con TXMODE=1 per inviare DESCREQ, poi RX con TXMODE=0
- Oppure attendere GPIO IRQ prima di fare 0x0B read
- Oppure path V1 (CTRL0 bit16) che gestisce TX+RX nativamente
- **D**: Refactor `spi_hid_seq_write_then_read` to use a single `spi_transfer` with both `tx_buf` and `rx_buf` (same len). Then in `host_transfer`, detect this case and call `exec_segment(opcode, tx, tx_len, rx, rx_len)` as one operation.

### Fix Proposti
- **Opzione A (TX+RX inline stesso opcode)** — VIABLE. Richiede di modificare `spi_hid_seq_write_then_read` per creare un singolo `spi_transfer` con sia `tx_buf` che `rx_buf`, E modificare `host_transfer` per eseguire entrambi in una singola chiamata `exec_segment`. Questo emula il comportamento Windows osservato nel CSV (TXN #3: opcode 0x02 TX+RX come singola operazione).

---

## 3. Fix SPI Applicati (v4)

Vedi [SPI_REGISTERS.md](SPI_REGISTERS.md) per dettagli completi.

| # | Bug | Fix | Effetto |
|---|-----|-----|---------|
| 1 | FIFO_CLEAR non toggle | clear→set (2 write) | FIFO reset corretto |
| 2 | V2 trigger = CTRL0 bit16 | 0x47 bit7 toggle | Trigger V2 corretto |
| 3 | Busy poll su STATUS | unificato CTRL0 bit31 | Wait affidabile |
| 4 | RX offset `+tx_len` | `+4` fisso | RX data letto correttamente |
| 5 | RX_COUNT spurio `+1` | `rx_len` esatto | Byte count corretto |
| 6 | Strobe V2 assenti | `writeb(0, 0x49)` e `writeb(0, 0x4A)` | Inizializzazione V2 |
| 7 | EXEC_CMD non toggle | clear→set per V1 | Trigger V1 corretto |

---

## 4. BUGS NOTI

| # | Bug | File:Line |
|---|-----|-----------|
| 1 | **Descriptor parse in state 1**: `memcpy(&raw, hdr+off, s)` copia l'header invece del body | `spi-hid-core.c:1237` |
| 2 | **ACPI table manca MSHW0231**: il device matcha solo via `PNP0C51` fallback | `spi-hid-core.c:1604-1610` |
| 3 | **input_register = 0x1000 per letture pre-descriptor**: Windows usa `0x0000` con approval byte7=0x00 prima del parse | `spi-hid-core.h:76`, `spi-hid-core.c:1814` |

---

## 5. Codice — Organizzazione

```
File                           Contenuto
─────────────────────────────────────────────────────────
spi-amd.c                      Controller SPI (V2, register setup, esecuzione)
spi-hid-core.c                 State machine HID-over-SPI, attivazione, FW
spi-hid-core.h                 Struct spi_hid (descrittori, stato, buffer)

Backup:
  ~/backup_20260704_v2/        Versione originale (con gpio_vendor_init)
  ~/spi-amd-v2-multi/spi-amd.c.bak  Versione intermedia con fix trigger

Diagnostico:
  /usr/src/spi-amd-amdi0060-1.0/  Modulo test register (amd_exec_v2_pure)

Decomp:
  ~/Scrivania/decomp/amdspi/     amdspi.sys (SPI controller Windows)
  ~/Scrivania/decomp/hidspicx_*.txt  hidspi.sys (HID protocol Windows)

CSV:
  ~/Scrivania/wintrace/surface_boot_auto.csv  Trace ETW boot Windows
```

---

## 6. Sequenza Completa (da CSV Windows)

Vedi [CSV_SEQUENCE.md](CSV_SEQUENCE.md) per analisi completa di ogni transazione.

```
FASE 1: Reset (~200 µs)
  TXN#1  0x0B read → RESET_RSP (32 10 00 5A)     type=3
  TXN#2  0x0B read → ACK (03 00 00 00)            device PRONTO!

FASE 2: Discovery (~200-2000 µs)  
  TXN#3  DESCREQ 0x02 TX+RX → ACK (03 00 00 00 00)
  🔴 GPIO IRQ (58 µs dopo)
  TXN#4  0x0B read → DEVICE_DESC (72 80 00 5A)    type=7 !!!
  TXN#5  0x0B read 37B → desc data
  TXN#6  DESCREQ2 0x02 → all zeros
  🔴 GPIO IRQ (727 µs dopo)
  TXN#7  0x0B read → RPT_DESC (82 B0 0E 5A)       type=8 !!!
  TXN#8  0x0B read 945B → HID report desc

FASE 3: Attivazione (~962ms dopo)
  TXN#9  cmd1 0x00 (5B) → TX only
  TXN#10 cmd2 0x00 (1B) → TX only
  TXN#11 0x0B read → checksum
  TXN#12 cmd3 0x70 TX+RX → FW status

FASE 4: Firmware (~410ms)
  TXN#13+ 0xB0 blocks (241B × 120 + 125B tail)
```

---

## 7. State Machine Attuale

Vedi [HIDSPI_PROTOCOL.md](HIDSPI_PROTOCOL.md) per dettagli.

```
State 0 (WAIT_RESET):
  ├── type==3: drain #1 + drain #2
  └── SEMPRE: DESCREQ → state=1
      └── spec_read: 64 byte dopo DESCREQ (spi-hid-core.c:1228)

State 1 (WAIT_DESC):
  ├── type==7: parse desc → DESCREQ2 → state=2
  │   └── BUG: memcpy(&raw, hdr+off, s) → hdr+off+4
  ├── type==3: "still RESET_RSP"
  └── type==0: "ACK in state 1"

State 2 (WAIT_RPT):
  ├── type==8: drain → goto VENDOR_INIT
  └── type==3/7: log and retry

State 3 (VENDOR_INIT):
  cmd1(0x00,5B TX) → cmd2(0x00,1B TX) → ACK read(0x0B,16B) → cmd3(0x70 TX+RX 14B)
  → fw_work + create_device → state=4
  approval byte7=0x03, byte8=0x00 fino a state=4

State 4 (DONE):
  ├── type==7: hid_input_report(hdr+off+4, rl-2)  ← CORRECT offset
  └── approval byte8 passa a 0x0A (spi-hid-core.c:1130-1131)
```

---

## 8. Comandi Rapidi

```bash
# Build
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$HOME/spi-amd-v2-multi modules
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$HOME/spi-hid/module modules

# Deploy
sudo cp ~/spi-amd-v2-multi/spi-amd.ko ~/spi-hid/module/spi-hid.ko \
        /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a

# Reload completo
sudo rmmod spi_hid; sudo rmmod spi_amd_amdi0060; sudo rmmod spi_amd
sudo insmod ~/spi-amd-v2-multi/spi-amd.ko
sudo insmod ~/spi-hid/module/spi-hid.ko
# oppure: sudo modprobe spi_hid

# Monitor
sudo dmesg -w | grep -E "SEQ|spi-amd"

# Push GitHub
cd /tmp/SL4A_TouchScreen
cp ~/spi-hid/module/spi-hid-core.c ~/spi-amd-v2-multi/spi-amd.c driver/
git add -A && git commit -m "..." && git push
```
