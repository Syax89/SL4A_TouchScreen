# Windows Boot CSV — Full SPI Transaction Sequence

Extracted from `surface_boot_auto.csv` — ETW trace of a Windows boot on the Surface Laptop 4 AMD.

> **UPDATE 2026-07-06**: Corrected based on `verification/csv-verification-report.md`.
> The "ACK" concept, the device's own auto state-change, treating "approval bytes" as
> protocol fields, and the wrong vendor ID have all been fixed.

## Summary Statistics

| Metric | Value |
|---------|--------|
| Total SPI transactions | 2384 |
| Total transfer buffers | 3779 |
| Total GPIO interrupts | 676 |
| Total boot duration | ~88 seconds |
| Source file | `surface_boot_auto.csv` |
| Timestamp | 100ns units (ETW Clock-Time) |

### Two SPB devices exist in the trace

| | Handle | Connection | Opcodes | Transactions |
|---|---|---|---|---|
| **Device A** — touchscreen | `0x7F74AA5D37F8` | 0x0B | 0x0B, 0x02 | 1,357 |
| **Device B** — companion | `0x7F74AA5D7B88` | 0x18, 0x19, 0x1A | 0x00, 0x70, 0xB0, 0xB1, 0x28, 0x22... | 1,027 |

The activation commands (0x00/0x70) and firmware uploads (0xB0) belong to **Device B**, NOT the touchscreen.

### Firmware operation detail
- **120 FW blocks (0xB0)** for the companion device (Device B), NOT for the touchscreen
- **Second binary upload (0x22)** for the companion device
- The touchscreen does NOT receive a firmware upload
- The touchscreen does NOT receive activation commands (0x00/0x70)

### Device descriptor (Device A — touchscreen)
- Vendor=0x045E (Microsoft), Product=0x0C19, Version=0x0100
- wInputRegister=0x0000, wOutputRegister=0x0003, wReportDescRegister=0x0002

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
- **0xFF in MISO**: the host transmits 0xFF as placeholder during the read approval (MOSI byte 5). These are NOT "sync bytes from the device" — they are transmitted by the HOST.
- **HID header** (V0 format): 4 bytes `[TYPE:4|VERSION:4][LEN_HIGH][LEN_LOW][0x5A]`
- **Descriptor data prefix** (body): 3 bytes `[len16 LE = content+3][ContentID]` before actual content
- **Report type**: `(TYPE >> 4) & 0xF`, version: `TYPE & 0x0F` == 2
- **"Approval bytes" in TX**: bytes 6-8 of the 9-byte 0x0B read approval are **residual buffer data** from a previous write, not intentional protocol fields. The device does not sample MOSI after byte 4 of the read approval.

---

## PHASE 1: Reset Response (Device A — touchscreen)

### TXN #0 — First 0x0B Read (Reset Response Header)
```
PayloadStart: total=18 bytes, transfers=2
┌─ TX ToDevice:  9 bytes ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 00 00                         │
│ │  └──────────────┘ └┘ └──── dummy ────┘           │
│ opcode  addr=0x0000  FF pad (placeholder for device) │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 32 10 00 5A                          │
│ └─host TX──┘ └─header──┘                          │
│ TYPE=0x32 → (3<<4)|2 = RESET_RSP type=3            │
│ body_length = 0x1000>>4 = 0x100 → *4 = 4 bytes      │
└─────────────────────────────────────────────────────┘
⏱ Total duration: ~6.3 µs (ETW event span, not bus time)
```

### TXN #1 — Second 0x0B Read (Reset Response Body)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice:  9 bytes ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 00 00   (same read approval)   │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 03 00 00 00                          │
│ └─host TX──┘ └─reset response body──┘             │
│ Body: 03 00 00 00 = len16=3 (content_len 0 + 3)     │
│       ContentID=0, 1 pad byte                       │
│ This is the BODY of the RESET_RSP, NOT a separate    │
│ "ACK" report. The two reads are header+body of the   │
│ SAME Reset Response input report.                    │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #0: ~168 µs (body read follows header)
⏱ Duration: ~5.5 µs

KEY CORRECTION: The device does NOT "change state on its own."
TXN #0 reads the header (type=3, len=4), TXN #1 reads the body.
No command was sent because none is needed — both reads belong
to the same Reset Response input report per the HID-over-SPI protocol.
```

---

## PHASE 2: Device Discovery (Device A — touchscreen)

### TXN #2 — DESCREQ (Write)
```
PayloadStart: total=20, transfers=2 (full-duplex)
┌─ TX ToDevice: 10 bytes ─────────────────────────────┐
│ 02 00 00 01 42 00 00 03 00 00                       │
│ │  └──addr──┘ └─len?──┘ └pad?┘                     │
│ opcode 0x02 = output report write                    │
│ register address 0x000001 (descriptor request)       │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 10 bytes ───────────────────────────┐
│ FF FF FF FF FF 03 00 00 00 00                       │
│ └─host TX──┘ └──MISO during write──┘               │
│ MISO shows residual data — the driver does NOT read  │
│ the FIFO after a write (RX_COUNT=0).                 │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #1: ~147 µs
⏱ Duration: ~5.6 µs (ETW span)
```

### GPIO IRQ — After DESCREQ
```
GPIO-ClassExtension: InterruptInvokeDeviceIsrStart
GPIO-ClassExtension: InterruptInvokeDeviceIsrComplete
⏱ After DESCREQ IoComplete: ~58 µs
⏱ ISR duration: ~1 µs
Pin: 0x15 (ETW), 0x55 in DSDT
```

### TXN #3 — 0x0B Read (Device Descriptor Header)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice: 9 bytes ──────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00                          │
│                         └─┘                          │
│              byte7=0x03 (buffer residual, NOT a      │
│              protocol field — see note above)        │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 72 80 00 5A                          │
│ └─host TX──┘ └─header──┘                           │
│ TYPE=0x72 → (7<<4)|2 = DEVICE_DESC type=7           │
│ body_length = 0x8000>>4 = 0x800 → *4 = 32 bytes     │
└─────────────────────────────────────────────────────┘
⏱ Gap from GPIO IRQ: ~112 µs (actually ~10 µs — see csv-verification-report)
⏱ Duration: ~5.4 µs
```

### TXN #4 — 0x0B Read, 37 bytes (Device Descriptor Body)
```
PayloadStart: total=74, transfers=2
┌─ TX ToDevice: 37 bytes ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00 00 00 00 00...          │
│ Padded to 37 bytes (zero-filled)                    │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 37 bytes ───────────────────────────┐
│ FF FF FF FF FF 1F 00 00 1C 00 00 01 A8 03 02 00... │
│ └─host TX──┘ └─body prefix─┘ └──descriptor data───┘ │
│ Body prefix (3 bytes): len16=0x001F, ContentID=0x00  │
│ Descriptor (28 bytes at body+3):                     │
│   wDeviceDescLength = 0x001C (28)                    │
│   bcdVersion = 0x0100                                │
│   wReportDescLength = 0x03A8 (936)  ← NOT vendor ID! │
│   wReportDescRegister = 0x0002                       │
│   wInputRegister = 0x0000                            │
│   wMaxInputLength = 0x0020 (32)                      │
│   wOutputRegister = 0x0003                           │
│   wMaxOutputLength = 0x0002                          │
│   wCommandRegister = 0x0004 (SET_POWER)              │
│   wVendorID = 0x045E (Microsoft)                      │
│   wProductID = 0x0C19                                 │
│   wVersionID = 0x0100                                 │
│   dword flags = 0x00000081                           │
│                                                       │
│ CORRECTED: 0x03A8 at offset 4 is the report descriptor│
│ LENGTH (936 bytes), NOT the vendor ID. Previous docs  │
│ mis-parsed this as vendor=0x03A8 product=0x0002.     │
└─────────────────────────────────────────────────────┘
⏱ Duration: ~5.6 µs (ETW span)
```

### TXN #5 — DESCREQ2 (Write)
```
PayloadStart: total=20, transfers=2
┌─ TX ToDevice: 10 bytes ─────────────────────────────┐
│ 02 00 00 02 42 00 00 03 00 00                       │
│    └──addr──┘                                       │
│    register=0x0002 (report descriptor request)       │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 10 bytes ───────────────────────────┐
│ 00 00 00 00 00 00 00 00 00 00                       │
│ ALL ZEROS! Device busy processing DESCREQ            │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #4: ~140 µs
⏱ Duration: ~3.0 µs
```

### GPIO IRQ — After DESCREQ2
```
⏱ After DESCREQ2 IoComplete: ~727 µs
⏱ ISR duration: ~5 µs
Pin: 0x15
```

### TXN #6 — 0x0B Read (Report Descriptor Header)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice: 9 bytes ──────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00                          │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 bytes ────────────────────────────┐
│ FF FF FF FF FF 82 B0 0E 5A                          │
│ └─host TX──┘ └─header──┘                           │
│ TYPE=0x82 → (8<<4)|2 = RPT_DESC type=8              │
│ body_length = 0xEB00>>4 = 0xEB0 → *4 = 940 bytes    │
└─────────────────────────────────────────────────────┘
⏱ Gap from GPIO IRQ: ~185 µs
⏱ Duration: ~6.7 µs
```

### TXN #7 — 0x0B Read, 945 bytes (Report Descriptor Body)
```
PayloadStart: total=1890, transfers=2
┌─ TX ToDevice: 945 bytes ────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00... (945 bytes total)     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 945 bytes ──────────────────────────┐
│ FF FF FF FF FF AB 03 00 75 08 15 00 26 FF 00 06... │
│ └─host TX──┘ └─body prefix─┘ └──936B report desc──┘ │
│ Body prefix (3 bytes): len16=0x03AB, ContentID=0x00  │
│ HID Report Descriptor: 936 bytes, starts with 0x75   │
│ (UsagePage byte). Contains Usage Pages, Logical      │
│ Min/Max, Report IDs...                                │
└─────────────────────────────────────────────────────┘
⏱ Gap from TXN #6: ~560 µs
⏱ Duration: ~5.4 µs
```

### ~962ms GAP
```
The Windows driver processes the descriptors, sets up the HID framework,
initializes internal structures. NO SPI activity on the TOUCHSCREEN during
this gap. (Companion device B becomes active during this period — see below.)
```

---

## Device B Activity During the Gap (Companion Chip — NOT the touchscreen)

The following transactions happen on Device B (`0x7F74AA5D7B88`), on connections
0x18/0x19/0x1A. These are **NOT** touchscreen operations.

```
TXN B1 (conn 0x18): Write 5B — 00 0E 00 00 00     (cmd1)
TXN B2 (conn 0x19): Write 1B — 00                   (cmd2)
TXN B3 (conn 0x19): Read 3B  — 84 26 AA             (checksum)
TXN B4 (conn 0x1A): Seq W1+R14 — 70 → 0E 00 42...  (cmd3, FW status)
TXN B5+ (conn 0x1A): Write 241B — B0 + 240B data    (FW blocks ×120)
TXN post-FW: B1 execute, 0x70 status, 0x28 command, 0x22 second binary
```

---

## PHASE 3: Runtime Operation (Device A — touchscreen)

Resumes after ~5902ms from boot start:

```
├─ Write 0x02 (SET_FEATURE/GET_FEATURE to register 0x0003)
├─ 0x0B read 9B → header type=1 (DATA report, NOT type=7)
├─ 0x0B read N bytes → HID input report body
├─ Periodic resync reads every ~110ms
└─ Continuous loop
```

### Runtime read example (around 42s):
```
MOSI: 0B 00 00 00 FF 00 03 0A 00...
MISO: FF FF FF FF FF 12 60 03 5A    (header type=1, len=0x6000>>4=0x600*4=6144B)
```

---

## TIMING SUMMARY

| Event | Duration/Gap |
|--------|-----------|
| 0x0B read (9+9 bytes) ETW span | ~6 µs |
| DESCREQ (10+10 bytes) ETW span | ~6 µs |
| DESCREQ IoComplete → GPIO IRQ | ~58 µs |
| GPIO IRQ → 0x0B read | ~10 µs (typical) |
| GPIO ISR duration | ~1-5 µs |
| DESCREQ2 (10+10 bytes) ETW span | ~3 µs |
| DESCREQ2 IoComplete → GPIO IRQ | ~727 µs |
| GPIO IRQ → 0x0B read | ~185 µs (varies) |
| Report descriptor read (1890 bytes) ETW span | ~5.4 µs |
| Descriptor → next activity gap | ~962 ms |
| Resync read frequency (runtime) | every ~110 ms |

**Note**: ETW "duration" times (~6 µs) are logging event spans, not actual SPI bus times.
Actual SPI transfer time at 33.33 MHz for 9 bytes = ~2.2 µs.

---

## RECURRING PATTERNS (Device A — touchscreen)

### Opcodes
| Opcode | Name | TX | Notes |
|--------|------|----|------|
| 0x0B | Read Approval | 9+ bytes | 5 protocol bytes + dummy padding. RX returns HID input report. |
| 0x02 | Output Report Write | 10 bytes | TX-only write. Response arrives via separate 0x0B + GPIO IRQ. |

### Body Length Formula (V0)
```
body_length = ((header_u16 >> 4) * 4)
```
Examples:
- RESET_RSP: header_u16=0x0100 → (0x100)*4 = 4
- DEVICE_DESC: header_u16=0x8000 → (0x800)*4 = 32
- RPT_DESC: header_u16=0xEB00 → (0xEB0)*4 = 940

### HID Header (V0 format)
```
Format: [TYPE 7:4 | VERSION 3:0] [u16 LE, bits 3:0=0, bits 15:4=len] [0x5A]

TYPE byte:
  bits 3-0: protocol version (= 2 for HID-over-SPI V0)
  bits 7-4: report type

Report types observed on touchscreen:
  1 = DATA (runtime input reports)
  3 = RESET_RSP
  7 = DEVICE_DESC
  8 = RPT_DESC

Type 0 does NOT exist in the V0 decomp (ValidateResponse bitmask 0x1B2).
03 00 00 00 is the RESET_RSP body (len16=3), not an "ACK" type.
```

### Note on TX "Approval Bytes"
The bytes at positions 6-8 of the 9-byte 0x0B TX payload (previously called
"approval7/approval8") are **residual buffer data** from the Windows driver's
buffer reuse. After a DESCREQ write (10 bytes: `02 00 00 01 42 00 00 03 00 00`),
the next read only writes bytes 0-4 (0x0B + addr + 0xFF). Bytes 5-8 retain
values from the previous write — hence byte7=0x03 after DESCREQ.

The observed values (0x00 before write, 0x03 after, 0x0A at runtime) are
explained by buffer reuse, NOT by an intentional "approval" protocol mechanism.

