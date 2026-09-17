# Protocol — HID-over-SPI V0

Both supported panels — `MSHW0231` (Surface Laptop 4 AMD) and `MSHW0162`
(Surface Laptop 3 AMD) — speak the same **HID-over-SPI Version 0 (V0)** protocol,
Microsoft's pre-release HidSpiDeviceV0 variant. It differs from the public
HID-over-SPI v1.0 spec in framing, descriptor layout and enumeration order.
This page documents the protocol as implemented by the Linux driver,
cross-validated against decompiled Windows `hidspi.sys` / `HidSpiCx.sys`.

## Discovery via ACPI

The device is declared in the DSDT under `\_SB.SPI1` with a `PNP0C50`
(compatible) HID-over-SPI ID. The driver reads the **`hid_desc_addr`** ACPI
device property to locate the HID descriptor register.

```
Device (TPD0) {
    Name (_HID, "MSHW0231")        // or "MSHW0162" on SL3 AMD
    Name (_CID, "PNP0C50")         // HID-over-SPI compatible
    Method (_CRS) { ... }          // SPI1: chip-select 0, 12 MHz, mode 0
}
```

The SPI controller is `AMDI0060` (AMD FCH SPI V2). The driver's ACPI match
table selects per-device tuning from the ACPI ID (see [Architecture](Architecture)).

## Transport model

Every exchange is host-initiated over one SPI transaction:

1. Host writes a **request frame** (TX-only) — opcode `0x02` (a single one; a driver option emits a doubled variant), a 4-byte
   register address, a content ID and a length (see [Wire Protocol](Wire-Protocol)).
2. The device asserts the data-ready GPIO **IRQ** when a response is available.
3. Host sends a **read approval** (`0x0B` frame), then reads the response from
   the FIFO. The driver's default is the field-settled **5-byte legacy shape**
   (`0B <reg3> FF`, register in the address field) — the only shape this panel
   answers; the 9-byte reference shape (register at offset 7) is
   `read_frame_variant=0` and silent on it.

All input is IRQ-driven; there is no polling in normal operation.

## Response types

Response frames carry a type nibble in the header. The driver handles:

| Type | Name | Meaning |
|---:|---|---|
| 1 | `DATA` | Runtime input report (touch `0x40`, pen `0x01`, raw `0x0C`) |
| 3 | `RESET_RSP` | Power-on reset notification — restarts descriptor discovery |
| 5 | `GET_FEAT_RESP` | Feature query response |
| 7 | `DEVICE_DESC` | Device descriptor (V0) |
| 8 | `RPT_DESC` | HID report descriptor (936 bytes) |

## Descriptor register

The HID descriptor register (at `hid_desc_addr`) describes the transport
geometry:

| Offset | Size | Field |
|---:|---:|---|
| 0 | 2 | Register length |
| 2 | 2 | BCD version (0x0100 = V0) |
| 4 | 2 | Report descriptor length (936) |
| 6 | 2 | Report descriptor register |
| 8 | 2 | Input register |
| 10 | 2 | Max input length |
| 12 | 2 | Output register |
| 14 | 2 | Max output length |
| 16 | 2 | Command register |
| 18 | 2 | Data register |
| 20 | 2 | Vendor ID (0x045E) |
| 22 | 2 | Product ID (0x0C19) |
| 24 | 2 | Version ID (0x0101) |
| 26 | 2 | Reserved |

The driver parses these with `spi_hid_parse_dev_desc()` and stores them in
`shid->desc`; nothing is hardcoded in the standard path (a descriptor that
cannot be read is an initialization failure, see
[Report Descriptor](Report-Descriptor)).

## Standard enumeration sequence

```
power-on ──► RESET_RSP ──► DESCREQ ──► DEVICE_DESC ──► RPT_DESC ──► DONE
             (type 3)      (write)     (type 7)        (type 8)     (input)
```

1. **`RESET_RSP`** (type 3): the device's power-on reset notification; the
   sequencer drains the body and sends `DESCREQ`.
2. **`DESCREQ`**: canonical frame, `content_id=0`, `len=3`, register `0x0001`:

   ```
   02 00 00 01 42 00 00 03 00 00
   ```
3. **`DEVICE_DESC`** (type 7): 28-byte descriptor; the driver reads it, then
   issues the second descriptor request for the report descriptor.
4. **`RPT_DESC`** (type 8): 936-byte HID report descriptor, read in 64-byte PIO
   continuations (see [Wire Protocol](Wire-Protocol) for the invariant).
5. **`DONE`**: input reports flow via `hid_input_report()`.

In standard mode (`raw_mode=0`) the driver **never** sends GET_FEATURE or
SET_FEATURE — the device streams pre-computed coordinates on its own (see
[Standard Touch Mode](Standard-Touch-Mode)).

## Raw-mode feature exchange

Raw mode adds three commands before discovery completes:

| Step | Frame | Register | Content ID | Payload |
|---|---|---|---|---|
| Vendor init | `vendor_init_cmd` | `0x0003` | `0xC2` | `56 BD 0C EE 5B 44 4C`, len 0x0A |
| GET_FEATURE | `gf_cmd` | `0x0042` | `0x04` | len 3 |
| SET_FEATURE | `sf_cmd` | `0x0003` | `0x04` | report ID `0x05`, value `0x01` |

```c
/* vendor init (18 bytes): opcode 0x02, reg 0x0003, content_id 0xC2 */
02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00

/* SET_FEATURE ID5=1 (14 bytes): report 0x05, value 0x01 */
02 00 00 03 82 00 03 04 00 05 01 0C EE 5B

/* GET_FEATURE (10 bytes) */
02 00 00 03 42 00 04 03 00 06
```

These are the wire frames the driver sends by default: one `0x02` opcode, and
the `0C EE 5B` device-key trailer on the short commands. The doubled-opcode
legacy form (`02 02 …`, zeroed trailer) is emitted only with
`wire_double_opcode=1`; every frame is byte-pinned in `tests/wire_frames_test.c`.

After SET_FEATURE the device starts streaming raw **CapImg frames**
(`content_id=0x0C`, ~4302–4304 bytes) — see
[Multi-touch (Experimental)](Multi-touch-Experimental).

## Timing and timeouts

| Parameter | Default | Role |
|---|---|---|
| `sync_timeout_ms` | 6000 | Bounds every synchronous request this driver issues (feature queries; clamped to [100, 60000] at probe) |
| `getfeat_delay_ms` | 0 | Optional delay between RPT_DESC and GET_FEATURE (Windows traces show ~3.6 s device settle; the original doc cited ~5.9 s) |
| descreq work delay | 100 ms | Deferred `DESCREQ` re-send on `WAIT_DESC` entry |

A connect-time feature GET_REPORT (e.g. hidraw `HIDIOCGFEATURE`) can take up
to ~3.6 s while the device settles. Feature-query timeouts are **non-fatal**
(`SPI_HID_SYNC_FEATURE` class) — the only synchronous-request class this
driver issues, since descriptor geometry is learned through the DESCREQ
sequencer path. The protocol's descriptor class is classified fatal, but the
driver has no caller for it (see `driver/spi-hid-protocol.h`).

## Raw CapImg frame

Raw frames are validated per device by `spi_hid_capimg_decode_v0()` against
the probe-selected sample count:

| Device | Raster samples | Frame body |
|---|---|---|
| SL4 `MSHW0231` | 3456 (72×48) | 16-byte heatmap section header + 3456 samples + vendor section |
| SL3 `MSHW0162` | 4056 (78×52) | same layout, larger raster |

Frames carrying a different count are rejected. See
[Wire Protocol](Wire-Protocol) for the byte-level layout.
