# MSHW0231 HID-over-SPI Driver — Documentazione Completa

> **Repository**: https://github.com/Syax89/SL4A_TouchScreen
> **⚠️ 2026-07-05 — verifica incrociata CSV × decomp amdspi × decomp hidspi V0 × spec:
> molte affermazioni di questo file e di HIDSPI_PROTOCOL/CSV_SEQUENCE/VERIFICATION_PLAN
> sono SMENTITE. La fonte di verità per protocollo e controller è ora
> [GROUND_TRUTH.md](GROUND_TRUTH.md)** (report completi in [verification/](verification/)).
> In particolare: la teoria "DESCREQ TX+RX inline stesso opcode" è FALSA (write TX-only +
> IRQ + read separata); gli "approval bytes" non esistono (artefatto buffer riusato);
> VENDOR_INIT/fw upload appartengono a un SECONDO device SPB, non al touchscreen.
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

### ❌ DA RISOLVERE (ordinati per priorità, verificati 2026-07-04)

#### CRITICAL — HID Protocol
1. **[C4]** **Bug `memcpy` nel parse descrittore (state 1)**: `memcpy(&raw, hdr+off, s)` copia i 4 byte dell'header HID invece del body (`spi-hid-core.c:1237`). Dovrebbe essere `hdr+off+4`. Tutti i campi del descrittore (vendor, product, registri) sono shiftati di 4 byte.
2. **[C5]** **input_register iniziale = 0x1000**: Dovrebbe essere `0x0000` per le letture pre-descriptor (Windows approva con approval7=0x00 su register=0x0000). `spi-hid-core.h:76`, `spi-hid-core.c:1814`.
3. **[C6/C7]** **Approval bytes errati a runtime**: approval7 deve essere `0x0A` (non 0x03) dopo state 4; approval8 deve essere sempre `0x00` (non 0x0A a state 4). `spi-hid-core.c:1129-1131`. Il device rifiuta report con approval errati.
4. **DESCREQ non produce type=7**: TX opcode 0x02 + RX separata opcode 0x0B — La 0x0B read separata è un nuovo comando. Windows usa TX+RX **inline con stesso opcode** (0x02 per entrambi) in una singola transazione SPI.

#### CRITICAL — SPI Controller
5. **[C1]** **ALT_CS encoding errato**: `amd_spi_select_chip` usa `cs & 0x03`, Windows forza `AND 0xFC, OR 0x01`. Con CS=0 il nostro driver scrive 0x00 (strobe disabilitato). `spi-amd.c:106-108`.
6. **[C2]** **`amd_spi_setup_v2_regs` chiamata in `host_transfer` ma NON in `exec_segment`**: i secret bits (30+29+18) vanno applicati per ogni segmento come fa Windows nella fcn.0x2be4. `spi-amd.c:426-427`.
7. **[C3]** **Strobe 0x49/0x4A non in Windows**: write non necessarie, potenzialmente dannose. `spi-amd.c:269-270`.

#### ACPI
8. **[C8]** **GPIO pin documentato errato**: DSDT dice pin `0x55` (85), trigger `Edge/ActiveLow`. Documentato come `0x15` `Level/ActiveHigh`. Il driver legge da ACPI a runtime, ma la documentazione è fuorviante.

### Piano Fix (2026-07-05 — sostituisce il precedente; vedi [GROUND_TRUTH.md](GROUND_TRUTH.md) §7)

| Ordine | Fix | Fonte |
|--------|-----|-------|
| **G0** | Baseline: build repo attuale + reload + dmesg | — |
| **G1** | Read Windows-shape: approval 5B `[0B][addr24 BE][FF]`, header read 9B, body read 5+blen, validazioni `hdr[3]==0x5A`/`(hdr[1]&0xF)==0`; **rimuovere** approval_byte7/8 (artefatto) | GT §3-4 |
| **G2** | Write path exec_segment: dance 0x44 (opcode→word 0x44→opcode), FIFO clear single-set | GT §4 |
| **G3** | Sequencer: header+body per IRQ; NIENTE read subito dopo la write (aspettare IRQ, timeout 2 s); su RESET_RESP ripetuta ri-inviare DESCREQ | GT §1-2 |
| **G4** | Parse descriptor a **body+3** (28 byte, validare wDeviceDescLength==0x001C); DESCREQ2 a wReportDescRegister | GT §5 |
| **G5** | Rimuovere VENDOR_INIT/fw_work dal path HID (device SPB sbagliato); runtime su type=1 | GT §6 |

NB: i vecchi fix C6/C7 (approval bytes) erano basati su un artefatto del CSV; C4 era
incompleto (+4 salta solo l'header: il descriptor inizia a body+3). La teoria "DESCREQ
TX+RX inline" (vecchia Opzione A/D) è SMENTITA da spec+decomp hidspi+decomp amdspi.

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

## 4. BUGS NOTI (aggiornato 2026-07-04 — verifica sub-agenti)

| # | Bug | File:Line | Priorità |
|---|-----|-----------|----------|
| C4 | **Descriptor parse offset +4**: `memcpy(&raw, hdr+off, s)` anziché `+4` | `spi-hid-core.c:1237` | **CRITICAL** |
| C5 | **input_register = 0x1000**: deve essere 0x0000 pre-descriptor | `spi-hid-core.h:76`, `spi-hid-core.c:1814` | **CRITICAL** |
| C6 | **approval7 = 0x03 a runtime**: deve essere 0x0A dopo state 4 | `spi-hid-core.c:1129` | **CRITICAL** |
| C7 | **approval8 = 0x0A a runtime**: deve essere sempre 0x00 | `spi-hid-core.c:1131` | **CRITICAL** |
| C1 | **ALT_CS encoding**: `cs & 0x03` anziché `OR 0x01` stile Windows | `spi-amd.c:106-108` | **CRITICAL** |
| C2 | **Secret bits non in exec_segment**: setup_v2_regs chiamata solo in host_transfer | `spi-amd.c:234,426-427` | HIGH |
| C3 | **Strobe 0x49/0x4A non in Windows**: write extra, verificare se necessarie | `spi-amd.c:269-270` | MEDIUM |
| 8 | **ACPI table manca MSHW0231**: matcha solo via `PNP0C51` fallback | `spi-hid-core.c:1604-1610` | LOW |
| 9 | **GPIO pin errato in docs**: DSDT dice `0x55` (85), documentato come `0x15` | DSDT/DRIVER_STATE.md | MEDIUM |

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
