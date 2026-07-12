# HID-over-SPI V0 Protocol — Complete Reference

> The MSHW0231 touchscreen on Surface Laptop 4 (AMD) uses the **pre-release V0 path**,
> not the public v1.0 spec. This document is the definitive technical reference,
> verified against both live SPI traces and decompilation of the Windows driver stack
> (`hidspi.sys`, `HidSpiCx.sys`, `amdspi.sys`).

## Discovery — How the Driver Chooses V0

The Windows driver probes ACPI `_DSM` in a two-pass approach (from
`AcpiHelper::GetHidSpiAcpiDefinition` at `hidspi.sys+0x11060`):

```
Pass 1: _DSM(rev=3, func=0) → returns 0x7F → take v1.0 (HidSpiCx) path
Pass 2: _DSM(rev=1, func=0) → returns != 0  → take V0 (HidSpiDeviceV0) path
```

The MSHW0231 _DSM only responds to `Arg1 == 1`. `func=0` returns `0x03` (three functions),
which is `!= 0`, so the driver selects **HidSpiDeviceV0**. This is correct — AMD/x86
platform use `SPI_HID_SUPPORTED_VERSION = 0x0100`, and Maximilian Luz of linux-surface
independently confirmed this on LKML for AMD-based devices.

---

## Input Header (4 bytes, "version 2" wire)

```
Byte 0:  [report_type:4 | version:4]
           version = 2 (SPI_HID_INPUT_HEADER_VERSION)

Byte 1-2: u16, little-endian, with two distinct subfields:
           bits[3:0]  = length_reserved (MUST be 0 — no fragment support)
           bits[15:4] = length_in_units_of_4

Byte 3:   0x5A (mandatory sync byte)
```

Body length on wire: `body_len = ((u16 >> 4) * 4)`

**Report type table** (validated by `ValidateResponse` bitmask `0x1B2`):

| Type | Name | Wire | Description |
|------|------|------|-------------|
| 1 | DATA | | Runtime input reports (pre-computed touch coordinates or raw heatmap) |
| 3 | RESET_RSP | `32 10 00 5A` | Device reset acknowledgment (4-byte body) |
| 4 | COMMAND_RESP | | Response to SET_POWER or other commands |
| 5 | GET_FEATURE_RESP | | Response to GET_FEATURE |
| 7 | DEVICE_DESC | `72 80 00 5A` | 28-byte I2C-like device descriptor |
| 8 | RPT_DESC | `82 B0 0E 5A` | HID report descriptor |

Type 0 does **not** exist. Type 2, 6, and values >8 are invalid on V0.

**Concrete headers from live captures**:

| Header (hex) | Decoded | Body Length |
|-------------|---------|-------------|
| `12 60 03 5A` | DATA type=1 | ((0x360>>4))*4 = 216 bytes |
| `12 40 43 5A` | DATA type=1 | ((0x4340>>4))*4 = 4304 bytes (raw heatmap) |
| `32 10 00 5A` | RESET_RSP type=3 | ((0x0100>>4))*4 = 4 bytes |
| `72 80 00 5A` | DEVICE_DESC type=7 | ((0x8000>>4))*4 = 32 bytes |
| `82 B0 0E 5A` | RPT_DESC type=8 | ((0xEB00>>4))*4 = 940 bytes |

---

## Input Body (after header, 3-byte prefix)

```
Byte 0-1: total_len_16, little-endian
          = content_length + sizeof(total_len_16) + sizeof(content_id)
          = content_length + 3
Byte 2:   content_id
```

**Examples**:

```
RESET_RSP body:    03 00 00 00   (total=3, content_id=0, 0 data bytes, 1 pad)
DEVICE_DESC body:   1F 00 00 1C... (total=31=28+3, content_id=0, 28 descriptor bytes)
RPT_DESC body:      AB 03 00 ...  (total=0x3AB=939=936+3, content_id=0, 936 descriptor bytes)
```

The real payload starts at byte 3 of the body and is `total_len_16 - 3` bytes long.

---

## Output Header (6 bytes, for writes)

```
Byte 0:   0x02  (SPI_HID_PROTOCOL_WRITE_OPCODE — the SPI opcode for write)
Byte 1-3: output_register address, 24-bit big-endian
Byte 4:   version=0x02 | (output_length & 0x0f) << 4
Byte 5:   output_length >> 4
```

### The "V2 body" magic: `0x42`

For DESCREQ-pattern writes (content_type=COMMAND with no payload beyond content_id),
byte 4 is the constant `0x42`. This encodes:

```
0x42 = (4 << 4) | 2
       └─ protocol_type=4 ─┘ └─ version=2
```

This is a V0-specific constant not present in v1.0. It appears in every DESCREQ, DESCREQ2,
and `SET_POWER` command. The Windows decomp confirms it at two independent call sites
(`ConfigureTransfer` and `ConfiguringDescriptorTransferEntry`):

```asm
mov word [r8+4], 0x42    ; V2 body: type=4<<4 | version=2
```

## Output Body (after header)

The output body immediately follows the 6-byte header and uses the same 3-byte
content prefix as the input body, but with a **content_type** field in place of content_id:

```
Byte 0-1: total_len_16, LE
          = content_length + sizeof(total_len_16) + sizeof(content_type)
          = content_length + 3
Byte 2:   content_type  (not content_id — different semantics on output)
Bytes 3+: content payload
```

### Content type codes (output direction)

| Constant | Value | Used in |
|----------|-------|---------|
| `CONTENT_TYPE_COMMAND` | `0x00` | DESCREQ, DESCREQ2, SET_POWER |
| `CONTENT_TYPE_SET_FEATURE` | `0x03` | SET_FEATURE writes |
| `CONTENT_TYPE_GET_FEATURE` | `0x04` | GET_FEATURE writes |
| `CONTENT_TYPE_OUTPUT_REPORT` | `0x05` | HID output reports |

---

## Complete Wire Frames — Canonical Examples

### DESCREQ (10 bytes on wire, 11 in driver internal buffer)

```
Canonical (wire):        02 00 00 01 42 00 00 03 00 00
Driver internal:         02 02 00 00 01 42 00 03 00 00 00  (11 bytes, doubled opcode)

│  │       │  │  │  │  │_______│
│  │       │  │  │  │     │
│  │       │  │  │  │     └─ content_id=0 (COMMAND), pad
│  │       │  │  │  └─ total_len_16 LE = 0x0003 = 3
│  │       │  │  └─ 0x00 (pad/alignment)
│  │       │  └─ 0x42 = V2 body magic
│  │       └─ output_register = 0x000001 (big-endian)
│  └─ 0x02 = write opcode (Doubled in driver: first 0x02 consumed by amd_spi_exec_segment)
└─ SPI opcode
```

Register addresses:
- **DESCREQ** → register `0x000001` (from ACPI `_DSM func=1`)
- **DESCREQ2** → register `0x000002` (from device descriptor `wReportDescRegister`)

**The doubled-opcode quirk**: The driver's DESCREQ byte sequence has an extra leading `0x02`.
This byte is consumed/stripped by `amd_spi_exec_segment()` before the wire frame is assembled.
The actual SPI transaction is always 10 bytes. Raw MMIO tools that bypass the segment executor
must NOT include the doubled byte.

### Read Approval (9 bytes)

```
TX: 0B 00 00 00 FF 00 00 00 00
RX: [4-byte HID input header][variable body]
    │  │       │  │
    │  │       │  └─ dummy/placeholder (ignored by device)
    │  │       └─ 0xFF = SPI_HID_READ_APPROVAL_CONSTANT
    │  └─ register address = 0x000000 (input register)
    └─ 0x0B = read-approval opcode
```

The input register is always `0x000000` for this device. Unlike v1.0, it does not change
after descriptor parsing. The `wInputRegister` field in the device descriptor is `0x0000`.

### GET_FEATURE (requesting id=4 from Windows trace)

```
02 00 00 03 42 00 04 03 00 06
│           │  │  │  │  │     │
│           │  │  │  │  │     └─ total_len_16=6, content_type=GET_FEATURE
│           │  │  │  └─ pad
│           │  └─ V2 body magic
│           └─ output_register = 0x0003
└─ write opcode
```

### SET_FEATURE (id=4, value=1, from Windows trace)

```
02 00 00 03 82 00 03 04 00 05 01 00 00 00
│           │  │  │  │  │  │     │        │
│           │  │  │  │  │  │     │        └─ pad
│           │  │  │  │  │  │     └─ value = 0x01 (enable raw mode)
│           │  │  │  │  │  └─ content_id = 4
│           │  │  │  │  └─ total_len_16 = 5
│           │  │  │  └─ content_type = SET_FEATURE (0x03)
│           │  │  └─ 0x00
│           │  └─ 0x82 = V2 body with length encoded: (8<<4)|2 = 0x82
│           └─ output_register = 0x0003
└─ write opcode
```

### SET_POWER (V0-specific, D0 = Active)

```
02 00 00 04 82 00 00 04 00 01 02 00 00 00
│           │  │  │  │  │  │  │  │
│           │  │  │  │  │  │  │  └─ pad
│           │  │  │  │  │  │  └─ param = 0x02
│           │  │  │  │  │  └─ SET_POWER = 0x01
│           │  │  │  │  └─ content_id = 0
│           │  │  │  └─ total_len_16 = 4
│           │  │  └─ content_type = COMMAND
│           │  └─ 0x82
│           └─ command_register = 0x0004
└─ write opcode
```

Power states: D0=0x01 (Active), D2=0x02 (Sleep), D3=0x03 (Off).

---

## Device Descriptor (28 bytes, I2C-like)

The V0 descriptor layout is closer to I2C-HID than the v1.0 spec:

| Offset | Size | Field | Value (MSHW0231) |
|--------|------|-------|------------------|
| 0 | u16 | Device Desc Length | 0x001F (31) |
| 2 | u16 | HID Version (BCD) | 0x0100 |
| 4 | u16 | Report Desc Length | 0x03A8 (936) |
| 6 | u16 | Report Desc Register | 0x0002 |
| 8 | u16 | Input Register | 0x0000 |
| 10 | u16 | Max Input Length | 0x10C9 (4297) |
| 12 | u16 | Output Register | 0x0003 |
| 14 | u16 | Max Output Length | 0x0000 |
| 16 | u16 | Command Register | 0x0004 |
| 18 | u16 | Vendor ID | 0x045E (Microsoft) |
| 20 | u16 | Product ID | 0x0C19 |
| 22 | u16 | Version ID | 0x0100 |
| 24 | u16 | Flags | (device-specific) |
| 26 | 4B | Reserved | 0x00000000 |

---

## V0 State Machine (62 states, Windows `hidspi.sys`)

The Windows V0 driver implements a 62-state SM with these key transitions:

```
STATE_resettingSync        → GPIO reset (300ms), then STATE_resetting
STATE_resetting             → drains RESET_RSP (header+body via 0x0B read)
STATE_resettingComplete     → returns success to caller
STATE_writingDescriptor1    → sends DESCREQ @0x01 (10B write)
STATE_readingDescriptor1Hdr → 0x0B read → header parse (expects type=7)
STATE_readingDescriptor1Bdy → 0x0B read → body (28B device descriptor)
STATE_descriptor1Complete   → validates descriptor, transitions to DESCREQ2
STATE_writingDescriptor2    → sends DESCREQ2 @0x02
STATE_readingDescriptor2Hdr → 0x0B read → header parse (expects type=8)
STATE_readingDescriptor2Bdy → 0x0B read → body (936B report descriptor)
STATE_descriptor2Complete   → creates HID device, enables runtime
STATE_ready                 → forwarding input reports, monitoring GPIO
STATE_middleOfReceive*      → handling re-synchronisation on RESET_RSP
```

On RESET_RSP during runtime: device signals error → driver **restarts from DESCREQ**,
NOT from a full GPIO reset.

### Comparison with v1.0 HidSpiCx SmFx

| Aspect | V0 (62 states) | v1.0 (SmFx) |
|--------|----------------|-------------|
| Control | Direct MMIO to SPI controller | WDF/SpbCx abstraction |
| GPIO | Raw GPIO pin toggle (300ms pulse) | Managed by ACPI power resource |
| Timer | Per-state WDFTIMER | WDFREQUEST-level timeout |
| Reset on error | Restart from DESCREQ | CheckingResetRetryCountEntry (3 retries) |
| Feature timeout | None explicit in SM | 2000ms response timer per feature op |
| Companion device | Ignored (conn 0x0B only) | Handled via SpbBusWrapper |

---

## GPIO Timing and IRQ Flow

The device uses **pin 0x55** on the AMD GPIO controller (AMDI0031 in DSDT, labelled 0x15
in ETW traces due to remapping). Every read is IRQ-gated:

```
1. Host sends DESCREQ/SET_FEATURE/write via TX-only SPI transaction (RX_COUNT=0)
2. Device processes the write (58-727µs)
3. Device asserts GPIO interrupt
4. GPIO controller delivers IRQ to driver
5. Driver sends 0x0B read approval (9 bytes TX)
6. Device responds with HID input packet (4-byte header on MISO)
7. Driver loops reading body if needed
8. Normal IRQ path: header+body in same 0x0B read if body fits in transfer
   (RESET_RSP with 4-byte body → single 9-byte read approval)
```

**Cold boot special case**: On very first power-on, a 14-byte vendor init command
is sent to register `0x000004` before any DESCREQ:

```
02 00 00 04 82 00 00 04 00 01 02 0C EE 5B
```

Testing proved this is optional — the device initializes without it on warm boot.

---

## What Does NOT Apply (v1.0 spec features absent from V0)

| v1.0 Feature | V0 Status |
|-------------|-----------|
| `_DSM rev=3 func=0` returning `0x7F` | Does not happen (returns `0x03` via rev=1) |
| Fragment ID in header byte 1 | Not supported (low nibble reserved = 0) |
| LFF (Last Fragment Flag) bit 14 | Not present |
| Content ID in header | Not in header (in body prefix) |
| Combined TX+RX transfers | TX-only writes (separate 0x0B for response) |
| SpbCx framework (SpbBusWrapper) | Not used (direct MMIO) |
| 24-byte device descriptor | 28-byte V0 descriptor |

---

## Implementation Reference

The portable protocol codec is in `driver/spi-hid-protocol.h`:

```c
// Decode an input header (4 raw bytes → struct)
spi_hid_protocol_decode_header(raw, &header);

// Encode an output header (register + length → 6 raw bytes)
spi_hid_protocol_encode_output_header(raw, output_register, output_length);

// Find a valid input header in an RX buffer (scans for sync byte 0x5A)
ssize_t pos = spi_hid_protocol_find_header(buf, len, protocol_version);

// Validate an input header (returns 0 on success)
spi_hid_protocol_validate_header(header, protocol_version);
```

Key constants:
```c
#define SPI_HID_PROTOCOL_VERSION          2     // "version 2" wire format
#define SPI_HID_PROTOCOL_SYNC_BYTE        0x5A  // mandatory at header[3]
#define SPI_HID_PROTOCOL_READ_OPCODE      0x0B  // GPIO-gated read approval
#define SPI_HID_PROTOCOL_WRITE_OPCODE     0x02  // TX-only write
#define SPI_HID_PROTOCOL_READ_APPROVAL    0xFF  // dummy byte in read approval
#define SPI_HID_PROTOCOL_DEFAULT_INPUT    0x000000  // input register
#define SPI_HID_PROTOCOL_INPUT_HEADER_LEN 4     // header size (always 4)
#define SPI_HID_PROTOCOL_INPUT_BODY_PFX   3     // body prefix (len16+content_id)
#define SPI_HID_PROTOCOL_OUTPUT_HEADER_LEN 6    // output header size
#define SPI_HID_PROTOCOL_DESCREQ_LEN      10    // DESCREQ wire frame size
```

---

## Sources

- `windrivers/hidspi.sys` — V0 state machine, decompiled (62-state FSM, direct MMIO)
- `windrivers/HidSpiCx.sys` — v1.0 reference (SmFx FSM, SpbCx bus wrapper)
- `windrivers/amdspi.sys` — AMD FCH SPI controller driver, V2 hardware
- `traces/surface_boot_auto.csv` — 2384-transaction ETW capture (warm boot)
- `traces/surface_init.csv` — 513-transaction capture (cold boot + vendor init)
- `traces/surface_touch.csv` — runtime touch data with raw heatmap frames
- `driver/spi-hid-protocol.h` — portable codec (header decode/encode, validation, search)
- `tests/protocol_test.c` — 9 suites / 75067 assertions verifying the codec
- `windrivers/HidSpiProtocolSpec.pdf` — Microsoft HID-over-SPI v1.0 public spec
