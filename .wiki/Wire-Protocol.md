# Wire Protocol — Byte-Level Framing

This page is the byte-level companion to [Protocol](Protocol): exact frame
shapes, header decoding, and the AMD FCH PIO read invariant.

## Frame anatomy

### Request frame (host → device)

```
[opcode:u8 = 0x02] [register:u32 LE] [content_id:u8] [len:u16 LE] [payload...]
```

The opcode is a single `0x02` in the frames this device is driven with; a
driver option (`wire_double_opcode=1`) emits a doubled-opcode variant instead.
The register is the descriptor-relative address (e.g. `0x0001` = DESCREQ
register, `0x0003` = command register). `content_id` selects the operation
(`0x00` DESCREQ, `0xC2` vendor init), `len` is the payload length.

Example — canonical DESCREQ (10 bytes):

```
02 00 00 01 42 00 00 03 00 00
   └─reg 0x0001─┘ └id=0x00┘ └len=3┘
```

### Response prefix (device → host)

The driver locates the header by scanning for the **sync byte `0x5A`**
(`spi_hid_protocol_find_header()`). The canonical response prefix is:

```
[TYPE:4 | VERSION:4] [report_length:u16 LE] [0x5A sync] [content_length:u16 LE] [content_id:u8] [payload...]
```

| Field | Size | Meaning |
|---|---|---|
| `TYPE` (high nibble, byte 0) | 4 bits | Response type (see table below) |
| `VERSION` (low nibble, byte 0) | 4 bits | Protocol version; the frames this device sends carry `2` |
| `report_length` | 12 bits | Body length in **4-byte words**: the low four bits are the high nibble of byte 1, the high eight bits are byte 2, and the byte count is that value × 4 |
| `sync` | 1 byte | Always `0x5A` |
| `content_length` | u16 LE | Unpadded semantic length of the body |
| `content_id` | u8 | Content identifier (`0x0C` raw, `0x40` touch, `0x01` pen…) |
| payload | — | Body bytes |

The header sits at **offset 5** of the read buffer for a plain reference frame,
and at **offset 8** for this panel, which prefixes its raw-mode answers with
three bytes:

```
[0x01] [status] [0xEE]   then the frame above
```

The driver accepts both positions — `5` and `8` — and drops anything else as a
malformed header.

### Read approval (host → device)

Before reading a response, the host sends a `0x0B` frame. Two shapes exist:

```
reference: 0B 00 00 00 FF 00 00 <reg> 00   (9 bytes; register at offset 7)
legacy:    0B <reg3> FF                     (5 bytes; register in the address field)
```

The driver's default is the **legacy** shape (`read_frame_variant=1`), the only
shape this panel answers; the reference shape is `read_frame_variant=0`, both is
`2`. (`SPI_HID_WIRE_OPCODE_READ = 0x0B`; the write opcode is `0x02`.)

## Response types

| Type | Name | See |
|---:|---|---|
| 1 | `DATA` — runtime input report | [Standard Touch Mode](Standard-Touch-Mode), [Multi-touch (Beta)](Multi-touch-Experimental) |
| 3 | `RESET_RSP` — power-on reset | [Protocol](Protocol) |
| 5 | `GET_FEAT_RESP` — feature response | [Protocol](Protocol) |
| 7 | `DEVICE_DESC` — V0 device descriptor (28 bytes) | [Protocol](Protocol) |
| 8 | `RPT_DESC` — HID report descriptor (936 bytes) | [Report Descriptor](Report-Descriptor) |

## PIO read invariant (64-byte continuations)

The AMD FCH SPI V2 controller has a **70-byte FIFO** (71 slots, 32-bit each).
Large reads (the 936-byte report descriptor) are split into 64-byte PIO
continuations:

| Field | Value |
|---|---:|
| Address bytes transmitted | 3 |
| `TX_COUNT` | 3 |
| `RX_COUNT` | 65 |
| First RX FIFO byte | `0x84` |

Each continuation **re-sends the three address bytes**; no trailing dummy byte
is transmitted. A dummy changes the controller framing to `TX_COUNT=4` /
FIFO `0x85`, selecting an unwritten slot that returns stale `0xFF` data.

## AMD FCH V2 controller registers

| Offset | Register | Purpose |
|---:|---|---|
| `0x1D` | `ALT_CS` | Alternate chip select (physical ALT_CS 1 for this board) |
| `0x22` | `SPI100_SPEED_CONFIG` | SPI speed tiers (index 0 = 12 MHz default) |
| `0x45` | `OPCODE_REG` | V2 transaction opcode |
| `0x47` | `CMD_TRIGGER` | Bit 7 starts a transaction |
| `0x48` | `TX_COUNT` | Transmitted byte count (≤ FIFO depth − 4) |
| `0x4B` | `RX_COUNT` | Received byte count |
| `0x80` | FIFO | 70-byte controller FIFO |

`TX_COUNT` must always leave at least 4 bytes of FIFO space for the
controller's internal status word; larger reads are split into 64-byte chunks.
The `TX_COUNT=3` quirk (instead of 0) correctly triggers the full-duplex read
phase — without it the first 3 bytes of received data are corrupted.

## Example: standard enumeration on the wire

```
Host  → [02 00 00 01 42 00 00 03 00 00]           DESCREQ (register 0x0001)
Device→ [IRQ]
Host  → [0B 00 00 03 FF]                          read approval (legacy 5-byte, register 3)
Device→ [70 1C 00 5A ... 28-byte DEVICE_DESC]     type=7 ver=0, len 0x001C
Host  → [02 00 00 02 42 00 00 03 00 00]           second descriptor request
Device→ [80 A9 03 5A ... 936-byte RPT_DESC]       type=8 ver=0, len 0x03A9
```

`0x70` = type 7 (`DEVICE_DESC`) + version 0; `0x80` = type 8 (`RPT_DESC`) +
version 0. Full sequence context and timing: [Protocol](Protocol).
