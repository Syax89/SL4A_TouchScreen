# HID-over-SPI Version 0 Protocol

The MSHW0231 touch controller communicates via the HID-over-SPI protocol
Version 0 (V0), a pre-release variant that differs from the public v1.0
specification. This document describes the wire protocol as implemented
by the Linux driver, validated against decompiled Windows `hidspi.sys`
and `HidSpiCx.sys` drivers. The Surface Laptop 3 AMD controller
(`MSHW0162`) uses the same V0 transport; every frame-level detail in this
document applies to both devices.

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

**One exception, and it is the one that matters at startup.** The device's
`32 10 00 5A` frame **is** the reset response: by the sync-based rule (high
nibble is the type, the version nibble is 2, sync `5A` present) it types as 3,
and the reference's own boot trace labels exactly that frame its RESET_RSP. A
sync-less `03 00 00 00` is **not** a reset — it is the drain that answers the
read after one. Two earlier readings inverted the pair: a narrowing taken from
a Cx-layer function (`VerifyResetResponse` compares `msg[0]` against 3, the
whole byte) rejected the real reset and typed the drain in its place. The
trace bytes, read with `tools/parse_spi.py`, settle it; the rule lives in
`spi_hid_protocol_frame_type()` (`driver/spi-hid-protocol.h`).

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

The driver sends a 10-byte DESCREQ frame to inquire about device capabilities
(wire bytes; the doubled-opcode legacy form is behind `wire_double_opcode=1`):

```
Host sends: 02 00 00 01 42 00 00 03 00 00   (register 0x0001)
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
Host sends: 02 00 00 03 82 00 03 04 00 05 01 0C EE 5B   (14 bytes)
```

`0F` is the content-layer id, not a wire opcode. The frame is built by
`spi_hid_wire_set_feature5()` (`driver/spi-hid-wire-frames.h`) and pinned byte
for byte by `tests/wire_frames_test.c`.

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

A single write opcode (`0x02`) starts each command frame. Windows sends it once
and pads the short command bodies with the constant `0C EE 5B` trailer; the
Linux driver sent it twice (`02 02 ..`) with a zeroed trailer until the frames
were reconciled against `captures/wintrace/surface_init.csv`. The Windows form is
now the default (`wire_double_opcode=0`) and the doubled form is behind
`wire_double_opcode=1`. The frames themselves are defined in
`driver/spi-hid-wire-frames.h` and asserted byte for byte by
`tests/wire_frames_test.c`.

### TX_COUNT Quirk

For AMD SPI V2 PIO reads, TX_COUNT must be 3 (not 0) to correctly
trigger the read phase. This matches Windows `amdspi.sys` decompilation.

Capture lengths are not wire lengths: the SPB capture records SpbCx
transfer-descriptor **buffer** sizes (TX equal to RX on every read) and no
TX_COUNT/RX_COUNT, so it cannot show what reaches the wire — do not port a
length from a capture row into a frame builder. The driver sizes each request
as the frame itself (`driver/spi-hid-core.c`, the read-approval caller; see
also `docs/FRAME-MATRIX.md`).

### Cold Boot Handshake

After a cold boot, the first DESCREQ attempt may fail. Recovery timing and
power sequencing are experimental and require target-machine evidence.

### GET_FEATURE Delay

Windows traces measure a ~3.6 s gap between RPT_DESC and GET_FEATURE
(`surface_init.csv` rows 6195→6431: 3.623 s); the original protocol
documentation cited ~5.9 s, which is not reproducible from the trace rows.
The Linux delay is configuration-dependent; with `skip_getfeat=1`, the
experimental vendor-init path (0xC2 opcode) does not park in `WAIT_FEATURE`
waiting for the GET_FEATURE reply, and no `skip_getfeat` value suppresses the
raw-mode Report ID 6 configuration read that Windows performs between RPT_DESC
and SET_FEATURE ID5. The
driver's `sync_timeout_ms` (default 6000) bounds synchronous requests so a
feature query issued during this settle window no longer tears the transport
down. Neither path is a release-qualified activation contract.

## References

- `docs/ACTIVATION.md` — Raw mode activation protocol
- `docs/decomp/` — Decompilation notes (local only, excluded from repo)
