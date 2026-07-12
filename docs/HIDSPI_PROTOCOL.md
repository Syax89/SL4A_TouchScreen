# HID-over-SPI Protocol — MSHW0231 Touchscreen

> **WARNING**: Corrected 2026-07-06 based on independent verification reports.
> Several earlier claims (ACK, approval bytes, vendor init for the touchscreen, combined
> TX+RX) have been **disproven** by the Windows decompilation and ETW analysis. See
> `docs/verification/`.

## Overview

The Surface Laptop 4 AMD touchscreen uses the V0 (pre-release) HID-over-SPI path,
NOT the spec v1.0 HidSpiCx path. The device is on SPB connection 0x0B, handle `0x7F74AA5D37F8`.

Two SPB devices exist in the system:
- **Device A** (touchscreen): conn 0x0B, opcodes 0x0B (read) and 0x02 (write) only
- **Device B** (companion chip): conn 0x18/0x19/0x1A, opcodes 0x00/0x70/0xB0/B1/0x28/0x22/0x24/0x25/0x26

The touchscreen does NOT receive firmware uploads, activation commands (0x00/0x70),
or any vendor-specific opcodes beyond 0x0B and 0x02.

## HID Header Format (V0 path, "version 2" wire format)

```
Byte 0: [TYPE 7:4 | VERSION 3:0]
        version = 2 (HID-over-SPI V0)
        report type:
           1 = DATA (runtime input reports)
           3 = RESET_RSP
           4 = COMMAND_RESP (e.g. SET_POWER ON response)
           5 = GET_FEATURE_RESP
           7 = DEVICE_DESC
           8 = RPT_DESC
        Type 0 does NOT exist (ValidateResponse bitmask 0x1B2 = {1,3,4,5,7,8})

Byte 1-2: u16 LE, bits 3:0 reserved (must be 0), bits 15:4 = body length in units of 4
Byte 3: 0x5A (marker, mandatory)

Body length = ((u16 >> 4) * 4)
Body starts with 3-byte prefix: [len16 = content+3 LE][ContentID]
```

### Examples

```
RESET_RSP:  32 10 00 5A    type=3, body_len = ((0x0100>>4))*4 = 4
  Body: 03 00 00 00        (len16=3, ContentID=0, 1 pad byte)

DEVICE_DESC: 72 80 00 5A    type=7, body_len = ((0x8000>>4))*4 = 32
RPT_DESC:   82 B0 0E 5A    type=8, body_len = ((0xEB00>>4))*4 = 940
```

**IMPORTANT**: `03 00 00 00` is the **body of the Reset Response** (len16=3 = 0 content + 3 overhead),
NOT a separate "ACK" report type. The two initial 0x0B reads are header+body of a single RESET_RSP
input report.

---

## SPI Commands (Device A — touchscreen only)

### 0x0B — Read Approval

```
TX: [0x0B] [ADDR_H] [ADDR_M] [ADDR_L] [0xFF] [dummy] [dummy] [dummy] [dummy]
      opcode  └──── register address (24bit big-endian) ──┘

9 bytes total. The device responds with the HID input report on MISO.
Bytes 5-8 of TX are dummy/placeholder — the device does not read them.
What appears at "byte7" in CSV traces is residual buffer data from a previous write, NOT a protocol field.
```

Register addresses (for touchscreen):
  0x000000: input register (used before descriptor parse, and stays 0x000000 on this device)
  After descriptor parse: `wInputRegister` comes from device descriptor offset 8 (word)

### 0x02 — Output Report Write (DESCREQ, SET_POWER, etc.)

```
TX: [0x02] [addr_H] [addr_M] [addr_L] [len16 LE] [ContentID] [pad...]
      opcode  └── register address (24bit BE) ──┘

10 bytes total (for DESCREQ).
This is a TX-only write — RX_COUNT=0 in the write handler (0x54d0).
The response arrives via a SEPARATE 0x0B read, triggered by a GPIO interrupt.
```

DESCREQ (Device Descriptor Request): register 0x000001
DESCREQ2 (Report Descriptor Request): register 0x000002

---

## Complete Touchscreen Boot Sequence

### Phase 1: Reset Response (2 reads, ~200 µs)

```
GPIO IRQ arrives (pin 0x15 in ETW, 0x55 in DSDT)
├─ 0x0B read 9B → header: 32 10 00 5A (RESET_RSP type=3, body_len=4)
├─ 0x0B read 9B → body:  03 00 00 00 (len16=3, ContentID=0)
│  These are header+body of the SAME input report — NOT "RESET_RSP then ACK."
│  No command is sent between them; the second read is the body drain.
└─ The device does NOT change state on its own.
```

### Phase 2: Device Discovery (~200-2000 µs)

```
├─ DESCREQ 0x02 TX 10B (register 0x000001) → write-only, RX_COUNT=0
│  MISO during write shows FF×5 + 03 00 00 00 00 (not captured by driver)
├─ GPIO IRQ (~58 µs after DESCREQ IoComplete)
├─ 0x0B read 9B → header: 72 80 00 5A (DEVICE_DESC type=7)
├─ 0x0B read 37B → device descriptor body (28 bytes of descriptor at body+3)
│  vendor=0x045E (Microsoft), product=0x0C19, version=0x0100
│  wInputRegister=0x0000, wOutputRegister=0x0003, wReportDescRegister=0x0002
├─ DESCREQ2 0x02 TX 10B (register 0x000002) → MISO all zeros (device busy)
├─ GPIO IRQ (~727 µs after DESCREQ2 IoComplete)
├─ 0x0B read 9B → header: 82 B0 0E 5A (RPT_DESC type=8)
├─ 0x0B read 945B → 936-byte HID report descriptor (at body+3)
└─ ~962ms gap (driver internal processing)
```

### Phase 3: Runtime Operation

```
├─ Periodic GPIO IRQ (every ~110ms for resync)
├─ 0x0B read 9B → header: type=1 (DATA, not type=7!)
├─ 0x0B read N bytes → HID input report data
├─ SET_FEATURE/GET_FEATURE writes (opcode 0x02, register 0x0003)
└─ Continuous loop with type=1 data reports
```

---

## Windows Driver Architecture (HidSpiDeviceV0)

The touchscreen uses the **V0 path** (HidSpiDeviceV0 class), NOT the Cx (HidSpiCx) path.
Key functions from decomp:

- `ValidateResponseHeader` @0x140008900: validates header byte3=0x5A, low nibble of u16 must be 0
- `ValidateResponse` @0x140008454: accepts types {1,3,4,5,7,8} (bitmask 0x1B2)
- `VerifyAndCompleteTransfer` @0x140008ba8: runtime handler, main case type==1 (data)
- `SendingWriteToBusEntry` @0x140008180: TX-only write (single transfer, length=10)
- `ReadingResponseHeaderEntry` @0x140007da0: sends read approval (5 bytes), 9-byte total transaction

### Key differences from spec v1.0 (HidSpiCx)

| Aspect | V0 (this device) | v1.0 (HidSpiCx) |
|--------|------------------|------------------|
| Device descriptor | 28 bytes (I2C-like layout) | 24 bytes |
| Header byte 0 | type:4|version:4 | type:4|version:4 |
| Header byte 1-2 | u16 (low nibble must be 0) | u16 bits 13:0 = length, bit14 = LFF |
| Input register | From descriptor offset 8 | From ACPI _DSM |
| Output register | From descriptor offset 12 | From ACPI _DSM |
| Descriptor body prefix | 3 bytes [len16|ContentID] | 2 bytes [len16|ContentID] |
| Write is | TX-only (RX_COUNT=0) | TX-only |
| Read-after-write | Separate 0x0B, GPIO-gated | Separate read approval, GPIO-gated |

---

## What is CONFIRMED CORRECT

| Aspect | Status |
|--------|--------|
| Header body_len formula: `(u16>>4)*4` | CONFIRMED (decomp validates this) |
| Header sync byte 0x5A at position 3 | CONFIRMED |
| Read approval format: 0x0B + 24-bit BE addr + 0xFF | CONFIRMED |
| DESCREQ 10-byte format: 0x02 + addr + len16 + ContentID + pad | CONFIRMED |
| DESCREQ register 0x000001, DESCREQ2 register 0x000002 | CONFIRMED |
| GPIO interrupt before every read | CONFIRMED |
| GPIO interrupt after write before reading response | CONFIRMED |
| ~962ms gap between descriptors and next activity | CONFIRMED |

---

## What has been FALSIFIED

| Previous Claim | Reality |
|---------------|---------|
| "type 0 = ACK/Ready" | Does not exist. 03 00 00 00 = RESET_RSP body |
| "Device changes state on its own" | Header+body of same report |
| "Approval bytes (APPR7/APPR8) as protocol fields" | Buffer echo artifact |
| "input_register = 0x001000 after descriptor" | Stays 0x000000 on this device |
| "TX+RX same opcode 0x02 combined" | TX-only write + separate 0x0B read |
| "Runtime data type=7" | type=1 (DATA) |
| "Vendor=0x03A8, Product=0x0002" | 0x045E, 0x0C19 (0x03A8 = report desc length 936) |
| "Activation/FW phases for touchscreen" | Belong to companion device B |
| "Sync bytes (0xFF) from device" | Host TX placeholder on MOSI during read approval |
| "Byte1 [LEN_LOW][FRAGMENT_ID]" | Low nibble reserved, must be 0 |
| "Approval bytes set by ConfigureTransfer" | Not protocol; buffer reuse artifact |

