# HID Report Descriptor

The touch controller returns a **936-byte HID report descriptor** in the
`RPT_DESC` (type 8) response. It declares the pen stylus, the touchscreen, and
a vendor/raw collection used in raw mode.

### Four lengths, four layers

The same descriptor is described by four numbers, and each is correct for what
it counts:

| Number | Layer | What it counts |
|---|---|---|
| **936** | content | the descriptor bytes themselves, as `wReportDescLength` declares |
| **939** | V0 body | 936 plus a 3-byte prefix (u16 total length, u8 content id) |
| **940** | frame | 939 plus one pad byte, which aligns the body to a 4-byte word |
| **945** | wire | 940 plus the 5-byte `FF FF FF FF FF` preamble before the body |

The device descriptor follows the same pattern one size down: 28 bytes of
structure, 31 with its prefix, 32 with the pad. The gap between the content and
the frame number is not a constant — it is three bytes of prefix plus whatever
pads the body to the next 4-byte word, which is 4 for this descriptor but 2 for
a raw sensor frame (4302 bytes of body, 4304 on the frame).

A read is sized from the frame layer — the length its own header announces —
while parsing and buffer sizing use the content layer. The prefix and the pad
are transport, not descriptor.

## Collections at a glance

| Usage | Report ID | Purpose | Details |
|---|---|---|---|
| Digitizer — Pen | `0x01` | Stylus: InRange, TipSwitch, BarrelSwitch, Invert, Eraser, X, Y, TipPressure | [Standard Touch Mode](Standard-Touch-Mode) |
| Digitizer — TouchScreen | `0x40` | Single-touch: TipSwitch, X (0–32767), Y (0–32767), 6 bytes total | [Standard Touch Mode](Standard-Touch-Mode) |
| Vendor (raw) | `0x0C` | CapImg sensor frames (~4302 bytes) consumed by the raw pipeline | [Multi-touch (Beta)](Multi-touch-Experimental) |

## How the driver uses it

During enumeration the driver:

1. reads the descriptor over 64-byte PIO continuations (invariant below),
2. stores the exact bytes in `shid->wire_report_descriptor`,
3. passes those bytes unchanged to `hid_parse_report()`,
4. registers the HID device with `hid_add_device()`.

There is **no embedded descriptor, byte repair, VID/PID substitution, or
fabricated descriptor** in the standard path. A descriptor that cannot be
read or parsed is an **initialization failure** — the driver refuses to
register a guessed HID device.

A hardcoded copy exists only for the fallback path (when the device will not
answer DESCREQ at all); it is never used when the wire descriptor is readable.

## PIO continuation invariant

The AMD controller's 70-byte FIFO forces large reads into 64-byte chunks:

| Field | Value |
|---|---:|
| Address bytes transmitted | 3 |
| `TX_COUNT` | 3 |
| `RX_COUNT` | 65 |
| First RX FIFO byte | `0x84` |

The three address bytes are resent for every continuation; no trailing dummy
byte is transmitted. A dummy byte switches the controller to `TX_COUNT=4` /
FIFO `0x85` — an unwritten slot — and yields stale `0xFF` data. See
[Wire Protocol](Wire-Protocol) for the full register context.

## Relationship to the raw mode

In raw mode the same descriptor is read and registered first, then the
driver performs the vendor feature exchange and the device begins streaming
`0x0C` frames, which bypass the HID stack and feed
`mshw0231_raw_consume_v0()` directly (see [Touch Pipeline](Pipeline)).
