# DRIVER STATE — 2026-07-08

> **Repository**: https://github.com/Syax89/SL4A_TouchScreen
> **Source of truth**: [GROUND_TRUTH.md](GROUND_TRUTH.md) §19

---

## Overall status: FUNCTIONAL — Single-touch + Pen working

The driver successfully initializes and streams touch and pen data. Single-touch is recognized
by KDE/Wayland. Multi-touch requires raw heatmap blob detection (future work).

---

## Read path

**WORKING** — All reads (opcode 0x0B) work correctly. The device responds with valid HID data
in standard mode (Report ID 0x40, 0x01) and raw heatmap mode (content_id 0x0C). The 936-byte
report descriptor is read live from the device with a 14-byte targeted patch for a
characterized hardware defect at page-relative offsets (n*64+55).

## Write path

**WORKING** — Writes (opcode 0x02) reach the device. The DESCREQ/DESCREQ2 commands
successfully trigger DEVICE_DESC and RPT_DESC responses. No other writes are needed
in standard HID mode. GET_FEATURE/SET_FEATURE are intentionally NOT sent — they would
switch the device to raw heatmap mode (not needed for single-touch).

## Standard HID mode (2026-07-08 breakthrough)

By NOT sending GET_FEATURE/SET_FEATURE, the device stays in standard HID mode:
- Report ID 0x40: TouchScreen (ABS_X, ABS_Y, BTN_TOUCH)
- Report ID 0x01: Pen (InRange, TipSwitch, X, Y, TipPressure)
- Reports arrive at ~10ms intervals
- Coordinates are pre-computed by the controller firmware
- No blob detection or calibration needed

## Raw heatmap mode (multi-touch investigation)

With SET_FEATURE(id=4, val=1), the device switches to raw mode:
- content_id 0x0C frames (4302 bytes)
- Data is DFT antenna output (not a simple capacitance grid)
- 4297 data bytes (prime number — not a rectangular grid)
- Blob detection on this data produces unstable coordinates
- Multi-touch requires reverse-engineering the DFT antenna layout
  in TouchPenProcessor0C19.dll (see [NEXT_STEPS.md](NEXT_STEPS.md) §D)

## HID Report Descriptor

- 936 bytes, read live from device
- 14 bytes at positions n*64+55 are corrupted to 0xFF by a hardware defect
  (characterized: clock-independent, timing-independent, tied to device's
  internal 64-byte page structure)
- Targeted patch restores correct values from hardcoded ground truth
- 922/936 bytes (98.5%) come from live wire read every boot
- 8 top-level collections: TouchScreen, Pen, Digitizer raw data, vendor features

## Key Files

| File | Contents |
|------|----------|
| `driver/spi-hid-core.c` | HID-over-SPI protocol driver + seq_thread state machine |
| `driver/spi-amd.c` | AMD FCH SPI V2 controller driver |
| `driver/spi-hid-core.h` | Shared definitions, struct spi_hid |
| `driver/hardcoded_rd.h` | 936-byte hardcoded report descriptor (ground truth) |
| `tools/parse_spi.py` | ETW CSV parser (2384 transactions) |
| `traces/surface_*.csv` | Windows ETW boot/touch traces |
| `docs/decomp/clean/` | hidspi.sys + HidSpiCx.sys decompilations |
| `docs/decomp/uefi/` | UEFI DXE driver decompilations |
| `windrivers/` | Windows binaries and spec PDF |
