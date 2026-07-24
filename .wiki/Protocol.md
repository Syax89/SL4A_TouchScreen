# HID over SPI V0 Protocol

The MSHW0231 touch controller uses the HID-over-SPI Version 0 protocol,
defined in the Microsoft HIDSPI specification. This page documents the
protocol as implemented by the Linux driver, cross-referenced against
decompiled Windows `hidspi.sys` and `HidSpiCx.sys`.

## Protocol Discovery

The HID-over-SPI protocol is discovered via ACPI. The runtime device identifies
as `MSHW0231` and uses the `SPI1` resource descriptor. The driver reads
`hid_desc_addr` from the ACPI device properties to locate the HID descriptor
register.

### ACPI Resource

```
Runtime resource: SPI1, logical chip-select 0, 33.33 MHz, mode 0.
```

## V0 Message Format

Each SPI exchange consists of a **request** (from host) and a **response**
(from device), with an optional **input report** frame.

### Input Header (6 bytes)

The device responds with a 6-byte header followed by the message body:

| Offset | Size | Name | Description |
|--------|------|------|-------------|
| 0 | 2 | content_id | Content identifier (opcode) |
| 2 | 2 | length | Length of message body + content |
| 4 | 1 | content_length | Reserved byte (ignored) |
| 5 | 1 | report_type | Report type flags |

The content_id determines how the remainder of the frame is interpreted:

| content_id | Opcode | Description |
|------------|--------|-------------|
| 0x07 | DESCREQ | Device descriptor request response |
| 0x08 | DEVICE_DESC | Hardware descriptor register |
| 0x0B | RPT_DESC | Report descriptor |
| 0x0F | COMMAND | Feature / SET_REPORT command |
| 0x11 | INPUT_REPORT | Input data (touch/pen) |

### Input Body Layout (V0)

The V0 protocol field in the device descriptor controls body format.
Version 0 bodies are 64-byte aligned:

```
[0..63]   64-byte report body (padded)
[64..67]  4-byte transport trailer
```

### Descriptor Register Structure

The device descriptor register (read at `hid_desc_addr`) contains:

| Offset | Size | Field |
|--------|------|-------|
| 0 | 2 | Length of register |
| 2 | 2 | BCD version |
| 4 | 2 | Report descriptor length |
| 6 | 2 | Report descriptor register V1 |
| 8 | 2 | Input register V1 |
| 10 | 2 | Max input length |
| 12 | 2 | Output register V1 |
| 14 | 2 | Max output length |
| 16 | 2 | Command register V1 |
| 18 | 2 | Data register V1 |
| 20 | 2 | Vendor ID |
| 22 | 2 | Device ID |
| 24 | 2 | Version (0x0101 = V0 HIDSPI) |
| 26 | 2 | Reserved |

## Communication Sequence

### DESCREQ → DEVICE_DESC (Discovery)

```
Host:  write DESCREQ frame (10 bytes)
       [header: content_id=0x01, length=10]
       [body: known DESCREQ payload]

Device: responds with DEVICE_DESC (0x08)
        [header: content_id=0x08, length=30]
        [body: 20-byte hardware descriptor + 10-byte DESCREQ response]
```

### RPT_DESC → Report Descriptor

```
Host:  read at rpt_desc_addr (937 bytes)
Device: responds with RPT_DESC (0x0B), body=936-byte HID report descriptor
```

### SET_FEATURE ID5 → Activation

Observed raw-mode experiments include a SET_FEATURE command sending value
`0x01` on Report ID 5:

```
Host:  write SET_FEATURE frame
       [header: content_id=0x0F, length]
       [body: ID5, value=0x01]
```

This is not yet sufficient evidence that ID5 alone creates a reliable stream.
Windows traces include a ~5900ms gap between RPT_DESC and GET_FEATURE; Linux
delay behavior is configuration-dependent and remains experimental.

### INPUT_REPORT → Touch Data

Older trace notes describe a 3528-byte frame containing:

```
[0..5]      Input header (content_id=0x11)
[6]         Frame type (0x00 = standard touch)
[7..11]     Frame metadata
[12..11+3456]  Heatmap data (older 16-bit interpretation)
```

The current parser instead handles byte-indexed CapImg bodies around 4304
bytes. Neither layout is a release contract until a labelled replay fixture
resolves the discrepancy; see `docs/EVIDENCE.md`.

## Power Management

The driver follows ACPI power states:
- **D0 (Active)**: device operational, SPI bus active
- **D3 (Off)**: device powered down, SPI bus suspended writes

Power transitions are managed via ACPI `_PS0` / `_PS3` methods.
The driver avoids `_RST` (M010): it destroys the device on this
hardware and requires a physical power cycle.

## Differences from Windows Canonical Protocol

| Item | Windows | Linux |
|------|---------|-------|
| DESCREQ size | 10 bytes (canonical) | 8 bytes (V0 spec) |
| SET_FEATURE opcode | Double opcode | Doubled to match trace |
| TX_COUNT for PIO reads | 3 | 3 (matches Windows 0x4bac) |
| D2→D0 transition | Active | Active |
| `_RST` support | Error path only | NEVER called |

## References

- `docs/HIDSPI_PROTOCOL.md` — Full protocol specification
- Decompilation notes retained locally (excluded from repository)
