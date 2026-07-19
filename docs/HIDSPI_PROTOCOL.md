# HID-over-SPI Version 0 Protocol

The MSHW0231 touch controller communicates via the HID-over-SPI protocol
Version 0 (V0), a pre-release variant that differs from the public v1.0
specification. This document describes the wire protocol as implemented
by the Linux driver, validated against decompiled Windows `hidspi.sys`
and `HidSpiCx.sys` drivers.

## Discovery

The device is declared in ACPI with `_HID = "MSHW0231"` and a `SPI1`
resource descriptor. The HID descriptor register address is obtained
from ACPI device properties.

## Message Format

Each SPI exchange has a request (host→device) and response (device→host).

### Input Header (6 bytes)

| Offset | Size | Name | Description |
|--------|------|------|-------------|
| 0 | 2 | content_id | Opcode identifying the message type |
| 2 | 2 | length | Total length of content + body (big-endian) |
| 4 | 1 | content_length | Reserved |
| 5 | 1 | report_type | Report type flags |

### Opcodes (content_id)

| Value | Name | Description |
|-------|------|-------------|
| 0x07 | DESCREQ_RESPONSE | Response to device descriptor request |
| 0x08 | DEVICE_DESC | Hardware descriptor register |
| 0x0B | RPT_DESC | HID report descriptor data |
| 0x0F | COMMAND | Feature/command report |
| 0x11 | INPUT_REPORT | Input data (touch coordinates) |
| 0x13 | OUTPUT_REPORT | Output data from host |
| 0x1F | VENDOR_DEFINED | Vendor-specific commands |

### V0 Body Format

Version 0 uses fixed-length 64-byte body slots:

```
[0..body_len]    Input report body (padded to 64 bytes)
[64]             Transport status byte
[65..67]         Reserved
```

The `length` field in the header covers all content including the
4-byte transport trailer.

## Communication Sequence

### 1. Device Descriptor Request (DESCREQ)

The driver sends a 10-byte DESCREQ frame to inquire about device capabilities:

```
Host sends: [02 00] [0A 00] [00 00 01 00] [00 00 00 00]
            content_id  length   DESCREQ magic   padding
```

The device responds with a 30-byte DEVICE_DESC (0x08) frame containing
the hardware descriptor and the DESCREQ response fields.

### 2. Hardware Descriptor

Read from the input register's `device_descriptor_register` field:

| Offset | Size | Field |
|--------|------|-------|
| 0 | 2 | Descriptor length (usually 28) |
| 2 | 2 | BCD version (0x0100) |
| 4 | 2 | Report descriptor length |
| 6 | 2 | Report descriptor register address |
| 8 | 2 | Input register address |
| 10 | 2 | Max input length |
| 12 | 2 | Output register address |
| 14 | 2 | Max output length |
| 16 | 2 | Command register address |
| 18 | 2 | Data register address |
| 20 | 2 | Vendor ID (0x045E) |
| 22 | 2 | Device ID (0x0C19) |
| 24 | 2 | Protocol version (0x0101 = V0) |
| 26 | 2 | Reserved |

### 3. Report Descriptor

Read from the `report_descriptor_register` at the address reported
in the hardware descriptor. The device returns a RPT_DESC (0x0B) frame
containing the 936-byte HID report descriptor.

### 4. SET_FEATURE ID5 (Raw Mode Activation)

```
Host sends: [0F 00] [length] [type=03, id=05, value=01]
            content_id  COMND  SET_FEATURE frame
```

This activates the raw heatmap mode. Without it, the device produces
only pre-processed single-touch coordinates (Report ID 0x40).

In raw mode, input reports contain the full 72×48 mutual-capacitance
heatmap (3456 cells × 16 bits = 6912 bytes of raw data per frame).

### 5. Input Report Processing

After activation, the device asserts a GPIO interrupt when data is available.
The driver reads the frame header (content_id=0x11), validates the
heatmap header, and forwards the raw data to the processing pipeline
or the HID subsystem depending on mode.

## V0 vs V1.0 Differences

| Aspect | V0 (MSHW0231) | V1.0 (HidSpiCx) |
|--------|---------------|------------------|
| Discovery | `_DSM rev=1 func=0` → 0x03 | `_DSM rev=3 func=0` → 0x7F |
| Device descriptor | 28 bytes (I2C-like layout) | 24 bytes |
| Input body | 64-byte aligned (v4 header) | Variable-length (length from v3 header) |
| Fragment support | None | LFF bit in header |
| Input register | From device descriptor offset 8 | From ACPI _DSM |
| State machine | 62-state V0 FSM (direct MMIO) | SmFx FSM (SpbCx abstraction) |

## Implementation Notes

### Opcode Doubling

The Windows driver doubles the opcode byte in certain commands. The
Linux driver matches this behavior for compatibility.

### TX_COUNT Quirk

For AMD SPI V2 PIO reads, TX_COUNT must be 3 (not 0) to correctly
trigger the read phase. This matches Windows `amdspi.sys` decompilation.

### Cold Boot Handshake

After a cold boot, the first DESCREQ attempt may fail. The driver
retries up to 3 times with 1-second delays and D2→D0 power cycles.

### GET_FEATURE Delay

Windows inserts a ~5900ms gap between RPT_DESC and GET_FEATURE. The
Linux driver replicates this via a deferred work item when
`skip_getfeat=0`. With `skip_getfeat=1`, the vendor-init path
(0xC2 opcode) is used instead, avoiding the delay.

## References

- `docs/ACTIVATION.md` — Raw mode activation protocol
- `docs/decomp/` — Decompilation notes (local only, excluded from repo)
