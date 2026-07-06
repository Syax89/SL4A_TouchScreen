# HID-over-SPI Analysis — Surface touchscreen (ACPI\MSHW0231)

## Hardware Identification
- Bus: HID-over-SPI (not I2C, not classic USB)
- ACPI HID: `MSHW0231`, Windows driver: `hidspi.sys` ("SPI HID Device")
- Vendor/Product/Version ID: 0x045E / 0xC19 / 0x4
- Public protocol: "HID Over SPI Protocol Specification v1.0" (Microsoft, Community Promise) — attachment `HidSpiProtocolSpec.pdf`

## Existing Linux Driver
`linux-surface/spi-hid` (adapted from the Surface Duo 2 Android sources).
Current ACPI match: `MSHW0134`, `MSHW0162`, `MSHW0235`, `PNP0C51` (generic).
`MSHW0231` isn't explicitly listed but can attach via the generic `_CID` `PNP0C51` if the DSDT exposes it.

## Protocol Verification via ETW Capture (SPB-ClassExtension + GPIO-ClassExtension)
Bytes observed on the SPI bus, compared against the constants in `spi-hid-core.h`:

| Field | Observed Value | Linux Driver Constant | Match |
|---|---|---|---|
| Read approval opcode | 0x0B | `SPI_HID_READ_APPROVAL_OPCODE_READ` | OK |
| Read approval constant | 0xFF | `SPI_HID_READ_APPROVAL_CONSTANT` | OK |
| Output write opcode | 0x02 | `SPI_HID_OUTPUT_HEADER_OPCODE_WRITE` | OK |
| Input header sync byte | 0x5A | `SPI_HID_INPUT_HEADER_SYNC_BYTE` | OK |
| Input header version | 0x02 | `SPI_HID_INPUT_HEADER_VERSION` | OK |

**The software protocol is therefore confirmed to conform to the public spec.**

## Full Report Descriptor (extracted from an ETW HIDCLASS event, no WinDbg needed)
Length: 936 bytes. Attached files: `mshw0231_report_descriptor.bin`, `mshw0231_report_descriptor.txt` (decoded with `hiddump.py`).

Relevant content:
- **Standard Pen Collection** (Report ID 1): InRange/TipSwitch/BarrelSwitch/Invert/Eraser + X/Y + TipPressure — pure Digitizer HID.
- **Standard TouchScreen Collection** (Report ID 0x40 / 64): TipSwitch + X/Y — pure HID, but **never observed on the bus** in any of the captures.
- **~10 "raw" Report IDs** under the Digitizer/vendor page (6, 7, 8, 10, 11, 12, 13, 26, 28...): a 16-bit field + a huge block marked "constant" (from 11 to 7485 bytes). Report ID 6 is a **"DeviceMode" Feature** (119 bytes).

## What the Device Actually Sends (confirmed across 3 captures: warm touch, disable/enable, cold boot)
- **~90% of DATA reports → Report ID 12**, a raw blob of **4299 bytes** (2-byte "SurfaceSwitch" + 4297 bytes marked constant)
- A minority → Report ID 8, a raw blob of 211 bytes
- **0% → Report ID 0x40** (the standard TouchScreen report with X/Y is never sent)

**Conclusion**: the digitizer transmits **raw Intel-IPTS-style frames**, not processed coordinates. A userspace daemon (like `iptsd`) is needed in addition to the transport driver.

## SPI Register Map (observed across 3 independent captures)

| Register | Observed Use |
|---|---|
| `0x0` | **Read-approval only** (opcode `0x0B`). Never a write. Dedicated input address — this is the "ctrl0". |
| `0x1` | Write COMMAND, content_id=0 — part of the handshake right after reset |
| `0x2` | Write COMMAND, content_id=0 — same |
| `0x3` | Feature channel: `GET_FEATURE` on Report ID 6 (DeviceMode), `SET_FEATURE` on Report ID 5 and Report ID 86 (0x56) |
| `0x4` | Device Descriptor request |

The Linux driver used the hardcoded default `SPI_HID_DEFAULT_INPUT_REGISTER = 0x1000` before reading the descriptor — on this hardware the real register is `0x0` (0x000000), so the pre-descriptor reads must use address 0x0000. **FIXED**: the driver now uses 0x0000 in state 0.

## No Mode-Change Command
Throughout the entire boot sequence, Windows only ever issues a **`GET_FEATURE` on Report ID 6 (DeviceMode)**, never a `SET_FEATURE` to change it. So there is no host-side command that forces "processed contacts" mode: the device starts up already configured (presumably by firmware) to stream raw frames.

## Next Step
1. Direct verification of `_DSM` for MSHW0231 via WinDbg (`!amli de \_SB.<path>.MSHW0231._DSM` with a local kernel debug session active) to confirm *why* the register map is this way (the `_DSM` functions 1/2/3 should return these same addresses).
2. Full decoding of the Report ID 12 payload to understand the raw frame format (grid dimensions, bits per cell, any calibration header) — needed to write the equivalent of `iptsd` for this hardware.
