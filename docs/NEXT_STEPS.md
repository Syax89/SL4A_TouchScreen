# Next Steps — Post-Raw-Mode Roadmap (2026-07-18)

> Standard HID mode (Report ID 0x40) is stable: single-touch + pen work.
> **Raw multi-touch mode** is functional with peak detection, 5×5 local centroid,
> fixed-point ×100, Hungarian assignment, EMA, lift lookback, and per-blob eigenvalues.
> 2-finger tracking is very good; 3-finger gradually loses contacts.

---

## What's still open

1. **3+ finger tracking reliability** — 3-finger contact gradually loses one finger over
   several seconds. Likely causes: peak detection at ±5 cell spacing may conflate nearby
   antennas at triple-touch density, or the Hungarian cost matrix needs tuning for 3-way
   assignments when fingers are close.

2. **Coordinate calibration** — grid-to-screen mapping uses auto-calculated scale factors
   (2256/72 cols, 1504/48 rows), but raw frame coordinates are centroid positions on the
   antenna grid, not direct pixel mapping. The Windows per-device context tables
   (`GROUD_TRUTH.md §22.8`) contain calibrated scaling factors and offsets. Without those,
   touch positions are approximate.

3. **Per-finger ellipse validation** — eigenvalues produce touch major/minor and orientation
   values but their physical accuracy hasn't been validated against a ground-truth capture.

4. **Cold boot reliability** — first probe after cold boot fails ~30% of the time; the new
   auto-retry mechanism (3 attempts, 5s delay, D2→D0 cycle) handles it gracefully, but the
   root cause is unknown (device power sequencing timing).

5. **Upstreaming** — split into Linux kernel patches:
   - `spi-amd.c` — AMD FCH Cezanne SPI controller V2 driver
   - `spi-hid-core.c` — HID-over-SPI transport driver (V0 protocol)
   - Device tree / ACPI binding for MSHW0231
   - Documentation

---

## DONE

- [x] Device init: DESCREQ → DEVICE_DESC → DESCREQ2 → RPT_DESC
- [x] HID report descriptor: 936B, 98.5% wire + 14B targeted patch
- [x] Standard HID mode: Report ID 0x40 (TouchScreen) + 0x01 (Pen)
- [x] BTN_TOUCH tap/lift detection
- [x] Stylus/Pen input (Report ID 0x01)
- [x] Raw mode handshake (SET_FEATURE ID5=01 via skip_getfeat=1 path)
- [x] Raw mode auto-retry on cold boot failure (3 probe attempts)
- [x] Crash fixes: SPI frequency, readb→readl, LPC bridge, HOST_PREF, post-trigger wait
- [x] c590 signal LUT matching Python oracle
- [x] Peak detection (cross-shaped, min_rise=350, max 16)
- [x] 5×5 local centroid with fixed-point ×100 sub-cell precision
- [x] Hungarian assignment with Windows-matching cost matrix
- [x] Light EMA (alpha=3) for position smoothness
- [x] Lift history lookback (2 frames)
- [x] Per-blob eigenvalues (touch ellipse)
- [x] HEATMAP_MAX_BLOBS 10→20 (buffer overflow fix for 3+ fingers)
- [x] Stack warning fix (cost matrix moved to spi_hid struct)
- [x] Dead code removal (7 unused arrays, blob splitting, ghost rejection, systemd service)
- [x] DKMS installer updated for raw_mode=1
- [x] Windows decomp: full CCL pipeline mapped (GROUND_TRUTH.md §22)
- [x] Python oracle (tools/surface_tracker.py) as ground-truth reference
- [x] Systemd service removed, udev auto-loads via modprobe.d

---

## Files to investigate

| File | Size | Contents |
|------|------|----------|
| `windrivers/TouchPenProcessor0C19.dll` | 9.8 MB | Main touch/pen processing DLL — decompiled, see `GROUND_TRUTH.md` §21-22 |
| `docs/decomp/uefi/SurfaceTouchHidDxe.c` | 1944 lines | UEFI touch driver |
| `docs/decomp/uefi/MsTouchUnlockDxe.c` | 2220 lines | Touch unlock/calibration sequences |
| `tools/surface_tracker.py` | Python oracle | Reference pipeline implementation validated against Windows |
