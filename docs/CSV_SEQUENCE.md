# Windows Boot CSV — Full SPI Transaction Sequence

Extracted from `surface_boot_auto.csv` — ETW trace of a Windows boot on the Surface Laptop 4 AMD.

## Summary Statistics

| Metric | Value |
|---------|--------|
| Total SPI transactions | 2384 |
| Total transfer buffers | 3779 |
| Total GPIO interrupts | 676 |
| Total boot duration | ~88 seconds |
| Source file | `surface_boot_auto.csv` |
| Timestamp | 100ns units (ETW Clock-Time) |

### Protocol status byte (approval byte a8)
| Value | Phase |
|--------|------|
| 0x00 | Init / pre-DESCREQ |
| 0x03 | Post-DESCREQ (descriptors received) |
| 0x04 | Runtime (post init) |

### Firmware operation detail
- **120 FW blocks (0xB0)** for the companion device, NOT for the touchscreen
- **Second binary upload (0x22)** for the companion device
- The touchscreen does NOT receive a firmware upload

### Device descriptor
- Vendor=0x045E (Microsoft), Product=0x0C19, Version=0x0100

---

## CSV Format

Each row contains comma-separated fields. Relevant fields:
- **Column 1**: Event Name (SPB-ClassExtension, GPIO-ClassExtension)
- **Column 2**: Event Type (IoSpbPayloadTdBuffer, InterruptInvokeDeviceIsr, etc.)
- **Column 28**: Clock-Time (timestamp in 100ns units)
- **Column 33**: Payload total size
- **Column 34**: Transfer count (1 = TX only, 2 = TX+RX)
- **Column 37**: Payload hex data

## Conventions

- **Opcode**: the first byte of the TX payload is the SPI opcode
- **Sync bytes**: 5 bytes of `0xFF` before the HID header
- **HID header** (RESET_RSP/DEVICE_DESC/RPT_DESC): 4 bytes `[TYPE][LEN_LOW][LEN_HIGH][0x5A]`
- **Descriptor data header** (TXN#8): 3 bytes `[TYPE][LEN_LOW][LEN_HIGH]` — 0x75 is the first data byte, NOT 0x5A
- **Report type**: `(TYPE >> 4) & 0xF`, version: `TYPE & 0x0F` == 2
- **Approval bytes**: bytes 7 and 8 of the 0x0B TX payload

---

## PHASE 1: Reset and Discovery (~0-2000 µs, 8 transactions)

### TXN #1 — First 0x0B Read (lines 63-73)
```
PayloadStart: total=18 bytes, transfers=2
┌─ TX ToDevice:  9 bytes ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 00 00                         │
│ │  └──────────────┘ └┘ └─┘ └─┘                    │
│ opcode  addr=0x0000  FF pad  approval7=0x00        │
│           (register 0)          approval8=0x00      │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 32 10 00 5A                          │
│ └─sync (5)──┘ └─header──┘                          │
│ TYPE=0x32 → (3<<4)|2 = RESET_RSP type=3            │
│ body_length = (0x10>>4)|(0x00<<4) = 1 → *4 = 4      │
└─────────────────────────────────────────────────────┘
⏱ Total duration: ~6.3 µs (63 ticks of 100ns)
```

### TXN #2 — Second 0x0B Read (lines 80-90)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice:  9 bytes ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 00 00   (IDENTICAL to the first)│
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 03 00 00 00                          │
│ └─sync──┘ └─ACK──┘                                 │
│ ACK pattern: 03 00 00 00 (device READY!)           │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #1: ~168 µs
⏱ Duration: ~5.5 µs

🔑 KEY INSIGHT: the device changes state between TXN #1 and TXN #2
   with no command at all! Elapsed time alone (~168 µs)
   makes the device go from RESET_RSP to ACK.
```

### TXN #3 — DESCREQ (lines 97-108)
```
PayloadStart: total=20, transfers=2
┌─ TX ToDevice: 10 bytes ─────────────────────────────┐
│ 02 00 00 01 42 00 00 03 00 00                       │
│ │  └──???──┘ └??┘ └??┘ └??┘ └??┘                   │
│ opcode 0x02 = DESCREQ                               │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 10 bytes ───────────────────────────┐
│ FF FF FF FF FF 03 00 00 00 00                       │
│ └─sync──┘ └───ACK────┘                             │
│ DESCREQ acknowledged!                                │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #2: ~147 µs
⏱ Duration: ~5.6 µs
```

### 🔴 GPIO IRQ (lines 120-121) — After DESCREQ
```
GPIO-ClassExtension: InterruptInvokeDeviceIsrStart
GPIO-ClassExtension: InterruptInvokeDeviceIsrComplete
⏱ After DESCREQ: ~58 µs
⏱ ISR duration: ~1 µs
Pin: 0x15, Flags: 0x409 (level-triggered active-high)
```

### TXN #4 — 0x0B Read (post-DESCREQ, lines 128-139)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice: 9 bytes ──────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00                          │
│                         └─┘                          │
│              approval7=0x03 (CHANGED from 0x00!)    │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 72 80 00 5A                          │
│ └─sync──┘ └─header──┘                              │
│ TYPE=0x72 → (7<<4)|2 = DEVICE_DESC type=7 !!!      │
│ body_length = (0x80>>4)|(0x00<<4) = 8 → *4 = 32     │
└─────────────────────────────────────────────────────┘
⏱ Gap from GPIO IRQ: ~112 µs
⏱ Duration: ~5.4 µs
```

### TXN #5 — 0x0B Read, 37 bytes (lines 145-155)
```
PayloadStart: total=74, transfers=2
┌─ TX ToDevice: 37 bytes ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00 00 00 00 00...          │
│                         └─┘                          │
│              approval7=0x03                          │
│ Padded to 37 bytes (zero-filled)                    │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 37 bytes ───────────────────────────┐
│ FF FF FF FF FF 1F 00 00 1C 00 00 01 A8 03 02 00... │
│ └─sync──┘ └───device descriptor data─────────────┘ │
│ vendor=0x03A8, product=0x0002, version=0x0320       │
└─────────────────────────────────────────────────────┘
⏱ Duration: ~5.6 µs
```

### TXN #6 — DESCREQ2 (lines 162-172)
```
PayloadStart: total=20, transfers=2
┌─ TX ToDevice: 10 bytes ─────────────────────────────┐
│ 02 00 00 02 42 00 00 03 00 00                       │
│    └──???──┘                                        │
│    register=0x0002 (different from TXN #3: 0x0001) │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 10 bytes ───────────────────────────┐
│ 00 00 00 00 00 00 00 00 00 00                       │
│ ALL ZEROS! Device busy/not responding                │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #5: ~140 µs
⏱ Duration: ~3.0 µs
```

### 🔴 GPIO IRQ (lines 185-186) — After DESCREQ2
```
⏱ After DESCREQ2: ~727 µs
⏱ ISR duration: ~5 µs
Pin: 0x15
```

### TXN #7 — 0x0B Read (post-DESCREQ2, lines 193-203)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice: 9 bytes ──────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00                          │
│              approval7=0x03                          │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 82 B0 0E 5A                          │
│ └─sync──┘ └─header──┘                              │
│ TYPE=0x82 → (8<<4)|2 = RPT_DESC type=8 !!!         │
│ body_length = (0xB0>>4)|(0x0E<<4) = 0xEB → *4 = 940 │
└─────────────────────────────────────────────────────┘
⏱ Gap from GPIO IRQ: ~185 µs
⏱ Duration: ~6.7 µs
```

### TXN #8 — 0x0B Read, 945 bytes (lines 210-220)
```
PayloadStart: total=1890, transfers=2
┌─ TX ToDevice: 945 bytes ────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00... (945 bytes total)     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 945 bytes ──────────────────────────┐
│ FF FF FF FF FF AB 03 00 75 08 15 00 26 FF 00 06... │
│ └─sync──┘ └header┘ └──descriptor data─────────────┘ │
│            (3 bytes, NO 0x5A! 0x75 is the 1st data byte)│
│ Contains Usage Pages, Logical Min/Max, Report IDs...│
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #7: ~560 µs
⏱ Duration: ~5.4 µs
```

---

## PHASE 2: Activation (~962ms after TXN #8)

### ⏸ ~962 millisecond GAP
```
The Windows driver processes the descriptors, sets up the HID framework,
prepares the buffers, initializes internal structures.
NO SPI activity during this gap.
```

### TXN #9 — cmd1 (lines 233-235)
```
PayloadStart: total=5, transfers=1 (TX only!)
┌─ TX ToDevice: 5 bytes ──────────────────────────────┐
│ 00 0E 00 00 00                                      │
│ │  └─payload──┘                                     │
│ opcode 0x00 = activation command                    │
│ payload: 0x0E 0x00 0x00 0x00                        │
└─────────────────────────────────────────────────────┘
```

### TXN #10 — cmd2
```
PayloadStart: total=1, transfers=1 (TX only!)
┌─ TX ToDevice: 1 byte ──────────────────────────────┐
│ 00                                                  │
│ opcode 0x00 = NOP/padding                           │
└─────────────────────────────────────────────────────┘
```

### TXN #11 — Checksum Response (RX-only, no 0x0B)
```
┌─ RX FromDevice: checksum data                        │
│ 84 26 AA...                                         │
└─────────────────────────────────────────────────────┘
```

### TXN #12 — cmd3 (0x70 TX+RX)
```
┌─ TX ToDevice: 1 byte ──────────────────────────────┐
│ 70                                                  │
│ opcode 0x70 = FW status request                     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 14 bytes ───────────────────────────┐
│ 0E 00 42 01 02 03 FF...                             │
│ └───firmware status────────────────────────────────┘ │
⏱ Duration: ~23.5 ms (!! much slower than the others)  │
```

---

## PHASE 3: Firmware Upload (from ~989ms onward)

### TXN #13+ — B0 Firmware Blocks
```
PayloadStart: total=241, transfers=1 (TX only)
┌─ TX ToDevice: 241 bytes ────────────────────────────┐
│ B0 [240 bytes of firmware data]                     │
│ │                                                    │
│ opcode 0xB0 = firmware block write                   │
└─────────────────────────────────────────────────────┘
⏱ Each block: ~3.4 ms
⏱ Inter-block gap: ~100-170 µs
⏱ Total blocks: ~120
⏱ Total FW time: ~410 ms
```

---

## PHASE 4: Post-FW — Second Binary and Runtime (~410ms+)

### TXN #134 — 0xB1 Post-FW Execute (5-byte TX)
```
┌─ TX ToDevice: 5 bytes ──────────────────────────────┐
│ B1 [4-byte payload]                                  │
│ │                                                    │
│ opcode 0xB1 = post-firmware execute command          │
└─────────────────────────────────────────────────────┘
```

### TXN #135-136 — 0x70 Read Status (×2)
```
┌─ TX ToDevice: 1 byte ──────────────────────────────┐
│ 70                                                  │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: N bytes ─────────────────────────────┐
│ post-upload firmware status                          │
└─────────────────────────────────────────────────────┘
```

### TXN #137 — 0x28 Unknown Command (7-byte TX+RX)
```
┌─ TX ToDevice: 7 bytes ──────────────────────────────┐
│ 28 [6-byte payload]                                  │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: N bytes ─────────────────────────────┐
│ 0x28 command response                                │
└─────────────────────────────────────────────────────┘
```

### TXN #138+ — 0x22 Second Binary Upload
```
┌─ TX ToDevice: N bytes ──────────────────────────────┐
│ 22 [payload]                                         │
│ opcode 0x22 = second binary upload                   │
└─────────────────────────────────────────────────────┘
⏱ Multiple blocks, similar to PHASE 3
```

### Runtime — 0x0B Read with approval7=0x0A
```
┌─ TX ToDevice: 9+ bytes ─────────────────────────────┐
│ 0B ... 0A 00                                        │
│        └─┘                                          │
│   approval7=0x0A (runtime)                           │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: N bytes ─────────────────────────────┐
│ sensor data / HID report                            │
└─────────────────────────────────────────────────────┘
⏱ Interval between reads: ~110 ms (periodic resync)
```

---

## TIMING SUMMARY

| Event | Duration/Gap |
|--------|-----------|
| 0x0B read (9+9 bytes) | ~6 µs |
| DESCREQ (10+10 bytes) | ~6 µs |
| DESCREQ → GPIO IRQ | ~58 µs |
| GPIO IRQ → 0x0B read | ~112 µs |
| GPIO ISR duration | ~1-5 µs |
| DESCREQ2 (10+10 bytes) | ~3 µs |
| DESCREQ2 → GPIO IRQ | ~727 µs |
| GPIO IRQ → 0x0B read | ~185 µs |
| Report descriptor read (1890 bytes) | ~5.4 µs |
| Descriptor → cmd1 gap | ~962 ms (!) |
| FW block write (241 bytes) | ~3.4 ms |
| FW inter-block gap | ~100-170 µs |
| Resync byte frequency (runtime) | every ~110 ms |

---

## RECURRING PATTERNS

### Opcodes and Meaning
| Opcode | Name | TX | RX | Notes |
|--------|------|----|----|------|
| 0x0B | Read Register | 9+ bytes | n bytes | Approval at byte7/8, address at byte1-3 |
| 0x02 | DESCREQ | 10 bytes | 10 bytes | Requests a device descriptor |
| 0x00 | Activation | 1-5 bytes | — | cmd1/cmd2 |
| 0x70 | Read Status | 1 byte | 14 bytes | Firmware status |
| 0xB0 | FW Block | 241 bytes | — | Firmware write |
| 0xB1 | Post-FW Execute | 5 bytes | — | Post-firmware execution |
| 0x28 | Unknown | 7 bytes | N bytes | Unknown post-FW command |
| 0x22 | Second Binary | N bytes | — | Second binary upload |

### Approval Bytes
```
Byte7 (approval7):
  - 0x00: initial state (before DESCREQ)
  - 0x03: after receiving at least one descriptor
  - 0x0A: runtime (~52s after boot)

Byte8 (approval8):
  - 0x00: ALWAYS (never changes)
```

### Body Length Formula
```
body_length = ((len_low >> 4) | (len_high << 4)) * 4
```
Examples:
- RESET_RSP: len_low=0x10, len_high=0x00 → (1|0)*4 = 4
- DEVICE_DESC: len_low=0x80, len_high=0x00 → (8|0)*4 = 32
- RPT_DESC: len_low=0xB0, len_high=0x0E → (0x0B|0xE0)*4 = 0xEB*4 = 940

### HID Header
```
Format: [TYPE][LEN_LOW][LEN_HIGH][0x5A]

TYPE byte:
  bits 3-0: protocol version (= 2 for HID-over-SPI)
  bits 7-4: report type

Report types:
  0 = ACK/Ready (pattern: 03 00 00 00, no 0x5A!)
  3 = RESET_RSP   (e.g.: 32 10 00 5A)
  7 = DEVICE_DESC  (e.g.: 72 80 00 5A)
  8 = RPT_DESC     (e.g.: 82 B0 0E 5A)
```
