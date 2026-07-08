# Next Steps — Post-Standard-HID-Mode Roadmap (2026-07-08)

> Standard HID mode (Report ID 0x40) is functional: single-touch + pen work.
> This doc tracks remaining work.

---

## DONE

- [x] Device init: DESCREQ → DEVICE_DESC → DESCREQ2 → RPT_DESC
- [x] HID report descriptor: 936B, 98.5% wire + 14B targeted patch
- [x] Standard HID mode: Report ID 0x40 (TouchScreen) + 0x01 (Pen)
- [x] Small report buffer overflow fix (rl - 2)
- [x] BTN_TOUCH tap/lift detection
- [x] Stylus/Pen input (Report ID 0x01, event21 + mouse2)

---

## §D — Multi-touch via raw heatmap blob detection

The device supports multi-touch, but only in **raw heatmap mode** (enabled by
SET_FEATURE(id=4, val=1)). In this mode the device sends content_id=0x0C
frames (~4302 bytes) containing DFT antenna data.

### What we know

- The raw payload is 4297 bytes of "constant" data per the HID descriptor
- This is NOT a simple capacitance grid — it's dual-frequency DFT antenna data
  with real and imaginary components (9 Short + 9 Long antennas in each direction)
- 4297 is a prime number → cannot be a rectangular grid
- Windows processes this via `TouchPenProcessor0C19.dll` (9.7 MB):
  - DFT processing (PenMagToBits, ShortDftRefAntAllTouchedReal/Image)
  - Connected Component Labeling (TouchDetectionCclLogic)
  - Kalman tracking (TrackLibRunTrackingLogicEntry)
  - 3 gain sets, multi-level thresholds (15%-37.5%)

### What we tried

- Blob detection on assumed 86x50 grid: produces stable single blob but coordinates
  don't match physical position
- Attempts at 87x49, 64x67 grids: similar results
- Dilation + EMA smoothing: improves stability but doesn't fix grid alignment
- Axis inversion (invert_x, invert_y, swap_xy): no combination matches correctly

### What's needed

The fundamental challenge is understanding the **DFT antenna layout** — how the
4297 frequency-domain samples map to physical positions on the screen.
This requires reverse-engineering the `TouchPenProcessor0C19.dll` to extract:

1. The antenna-to-position mapping matrix
2. The DFT window parameters (Short vs Long window sizes)
3. The coordinate calculation formulas (PenPosition, PenCenterOfMass)
4. The noise rejection and gain control logic

### Files to investigate

| File | Size | Contents |
|------|------|----------|
| `windrivers/TouchPenProcessor0C19.dll` | 9.8 MB | Main touch/pen processing DLL |
| `docs/decomp/uefi/SurfaceTouchHidDxe.c` | 1944 lines | UEFI touch driver (>>5 coordinate transform) |
| `docs/decomp/uefi/MsTouchUnlockDxe.c` | 2220 lines | Touch unlock/calibration sequences |
| `docs/analisi_MSHW0231.md` | 59 lines | Touchscreen register analysis |

---

## §E — Cleanup and polish

- [ ] Reduce dev_info logging noise in production
- [ ] Remove unused heatmap blob detection code (or guard behind module param)
- [ ] Proper sysfs interface for diagnostics
- [ ] Systemd service for auto-load at boot
- [ ] Kernel module signing for Secure Boot

---

## §F — Upstreaming

Split into Linux kernel patches:
1. `spi-amd.c` — AMD FCH Cezanne SPI controller V2 driver
2. `spi-hid-core.c` — HID-over-SPI transport driver (V0 protocol)
3. Device tree / ACPI binding for MSHW0231
4. Documentation
