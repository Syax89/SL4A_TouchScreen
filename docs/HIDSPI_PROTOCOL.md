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

### Report Types (header byte 0, upper nibble)

These correspond to the `SPI_HID_REPORT_TYPE_*` constants defined
in `driver/spi-hid-core.h`.

| Value | Define | Description |
|-------|--------|-------------|
| 0x01 | `SPI_HID_REPORT_TYPE_DATA` | Unsolicited input data |
| 0x03 | `SPI_HID_REPORT_TYPE_RESET_RESP` | Reset acknowledgement |
| 0x04 | `SPI_HID_REPORT_TYPE_COMMAND_RESP` | Command/request response |
| 0x05 | `SPI_HID_REPORT_TYPE_GET_FEATURE_RESP` | Get-feature response |
| 0x07 | `SPI_HID_REPORT_TYPE_DEVICE_DESC` | Device descriptor data |
| 0x08 | `SPI_HID_REPORT_TYPE_REPORT_DESC` | HID report descriptor data |

### V0 Body Format

The V0 parser (`spi_hid_protocol_parse_content` in
`driver/spi-hid-protocol.h`) uses **variable-length** bodies
prefixed by a 3-byte content header:

```
[0..1]       total_length   — u16 LE, total body length (≥ 3, ≤ 8192)
[2]          content_id     — identifies the payload type
[3..N-1]     data           — payload (N = total_length, data_length = N - 3)
```

The `report_length` field in the input header covers the full
message including the body.  There is no fixed 64-byte slot size
or transport trailer; `total_length` determines the semantic
data boundary.

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

This command is part of the observed raw-mode sequence. It is not yet proven
that ID5 alone establishes a reliable stream, and the frame layout is still
under reconciliation. The current parser accepts byte-indexed CapImg bodies of
roughly 4304 bytes; older documentation described a 16-bit 6912-byte raster.
See `docs/EVIDENCE.md` before using either interpretation as a protocol change.

### 5. Input Report Processing

After an observed raw-mode sequence, the device can assert a GPIO interrupt
when data is available. The driver reads and validates the input before routing
it to the raw pipeline or HID subsystem. Release behavior requires an E1 result
for the exact firmware and profile.

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

After a cold boot, the first DESCREQ attempt may fail. Recovery timing and
power sequencing are experimental and require target-machine evidence.

### GET_FEATURE Delay

Windows traces include a ~5900ms gap between RPT_DESC and GET_FEATURE. The
Linux delay is configuration-dependent; with `skip_getfeat=1`, the experimental
vendor-init path (0xC2 opcode) bypasses GET_FEATURE. Neither path is a
release-qualified activation contract.

## References

- `docs/ACTIVATION.md` — Raw mode activation protocol
- `docs/decomp/` — Decompilation notes (local only, excluded from repo)
