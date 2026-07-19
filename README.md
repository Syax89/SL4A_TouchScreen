# SL4A TouchScreen

Linux kernel driver for the Microsoft Surface Laptop 4 (AMD) touchscreen,
providing standard HID single-touch, pen, and raw multi-touch via a
V0 HID-over-SPI transport on the AMD Cezanne FCH SPI controller.

[![Status](https://img.shields.io/badge/status-beta-orange)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.1.0-brightgreen)](VERSION)
[![License](https://img.shields.io/badge/license-GPL--2.0-blue)](LICENSE)

> [!WARNING]
> **Beta software.** This is an experimental, reverse-engineered kernel driver.
> Use at your own risk. No warranty; provided "as is".

## Device

| Component | Detail |
|-----------|--------|
| Model | Surface Laptop 4 (AMD Cezanne) |
| Touch ACPI ID | `MSHW0231` (HID VID/PID: 0x045E/0x0C19) |
| SPI Controller | `AMDI0060` (AMD FCH SPI V2 at MMIO 0xFEC10000) |
| Protocol | HID-over-SPI Version 0 |
| Touch grid | 72×48 mutual-capacitance (3456 cells) |
| Report rate | ~100 Hz (raw mode) |

## Feature Status

| Feature | Status |
|---------|--------|
| HID report descriptor (936 bytes, read live from device) | Complete |
| Single-touch + pen (Report ID 0x40 and 0x01) | Stable |
| Multi-touch CCL pipeline (peak gate → flood-fill → Hungarian) | Working |
| Touch ellipse (per-blob eigenvalues, major/minor/orientation) | Working |
| Stationary lock (eliminates pinch-to-zoom jitter) | Working |
| Velocity rejection (Windows dist² ≤ 36.0) | Working |
| Edge penalty (Windows config+0x8D0/0x8D4) | Working |
| Multi-finger association radii (Windows +0x8DC-0x8EC) | Working |
| Cold boot auto-retry handshake | Working |
| 2-finger tracking | Excellent |
| 3-finger tracking | Good |
| 4-finger tracking | Partial |
| Candidate classification (Mahalanobis) | Not implemented |
| Per-cycle gain adaptation | Not implemented |

## Architecture

```
┌──────────────────────────────────────────────────────────────┐
│ Userspace: libinput / evdev ← hid-multitouch                 │
├──────────────────────────────────────────────────────────────┤
│ spi-hid.ko (spi-hid-core.c)                                  │
│   ├─ HID LL driver (DESCREQ → DEVICE_DESC → RPT_DESC)        │
│   ├─ IRQ-driven input (IRQ → SPI read → input_report)        │
│   └─ Raw heatmap pipeline:                                   │
│        baseline → peak gate → CCL → velocity → edge →        │
│        split → centroid → pre-merge → Hungarian →            │
│        EMA + deadband + stationary lock → MT emission         │
├──────────────────────────────────────────────────────────────┤
│ spi-amd.ko (spi-amd.c)                                       │
│   AMD FCH SPI controller V2 PIO driver                       │
│   TX/RX FIFO, chunked reads, opcode model                    │
├──────────────────────────────────────────────────────────────┤
│ Hardware: AMD FCH SPI @ 0xFEC10000 → MSHW0231                │
└──────────────────────────────────────────────────────────────┘
```

### Raw Touch Pipeline

The raw multi-touch pipeline processes the 72×48 heatmap into HID contacts,
matching the Windows `TouchPenProcessor0C19.dll` processing chain at ~85%
fidelity. Config values are extracted from the DLL data segment at
`0x1808E0460`.

| Stage | Function |
|-------|----------|
| **c590 LUT** | Raw 16-bit → fixed-point: `max(0, 10000 - (i·22 + 6000))` |
| **Baseline** | 30-frame asymmetric EMA per cell |
| **Noise floor** | c590 < 400 → suppressed (Windows DAT_1806c08c8 = 0.04) |
| **Peak gate** | Cross-shaped ±5 cells, min_rise=200, max 16 peaks |
| **CCL flood-fill** | 4-connected BFS, filters: n≥2, max_rise≥200, weight≥1000 |
| **Velocity rejection** | Blob must be within 6 cells of a detected peak |
| **Edge penalty** | Bottom edge ×0.23, other edges ×0.97 |
| **Blob splitting** | Multi-peak blobs (≥4 cells apart) split into sub-blobs |
| **Centroid** | Signal-weighted ×100 fixed-point on full blob extent |
| **Eigenvalues** | Second moments → touch major/minor/orientation |
| **Pre-merge** | Merge blobs within ghost_dist=6 (distance² < 36) |
| **Hungarian** | Assignment with multi-finger radii (1×2.2, 2×1.0, 3×2.8, 4×3.4, 5+×4.0) |
| **EMA + deadband** | Alpha=7 smoothing, ±0.8 cell deadband, 6-frame stationary lock |
| **Lift lookback** | Emit lift at position from 2 frames ago |

## Install

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
sudo reboot
```

The DKMS installer supports Arch/CachyOS, Ubuntu/Debian, Fedora, openSUSE.
After reboot the driver auto-loads via udev and `/etc/modprobe.d/spi-hid.conf`.

## Module Parameters

```
/etc/modprobe.d/spi-hid.conf:
  options spi_hid raw_mode=Y skip_getfeat=Y
```

| Parameter | Default | Description |
|-----------|---------|-------------|
| `raw_mode` | 1 | Raw heatmap + multi-touch mode |
| `skip_getfeat` | 1 | Internal: skip GET_FEATURE handshake, use vendor-init path |
| `ema_alpha` | 7 | EMA smoothing (1-10) |
| `blob_max_distance` | 3 | Hungarian base radius in cells |
| `blob_min_weight` | 1000 | Minimum blob signal weight |
| `blob_debounce` | 3 | New-touch debounce frames |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace period (0 = disabled) |
| `ghost_dist` | 6 | Pre-merge radius in cells |
| `pre_assoc_ratio` | 0 | Pre-association filter (0 = disabled) |
| `grid_cols/rows` | 72/48 | Heatmap grid dimensions |
| `calib_scale_x/y` | 0 | Scale ×1000 (0 = auto from descriptor) |
| `calib_offset_x/y` | 0 | Screen offset |
| `invert_x/y` | 0 | Invert axis |
| `swap_xy` | 0 | Swap axes |

## Troubleshooting

| Issue | Fix |
|-------|-----|
| No touch after cold boot | Power off → unplug AC → wait 30s → reboot |
| No multi-touch (only single-touch) | Check `raw_mode=Y` in modprobe config |
| Fingers lost during fast movement | Increase `blob_lift_frames` |
| Jitter during pinch-to-zoom | Verify `ema_alpha=7`, stationary lock active |
| Module rejected (Secure Boot) | Enroll DKMS signing key via distribution MOK |

## Build from Source

```bash
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules
sudo cp driver/spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo reboot
```

## Documentation

| Document | Content |
|----------|---------|
| [Wiki](https://github.com/Syax89/SL4A_TouchScreen/wiki) | Project wiki: protocol, pipeline, config, hardware |
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 wire protocol |
| [`docs/PIPELINE.md`](docs/PIPELINE.md) | Touch pipeline specification |
| [`docs/SPI_REGISTERS.md`](docs/SPI_REGISTERS.md) | AMD FCH SPI controller registers |
| [`docs/CONFIG_TABLE.md`](docs/CONFIG_TABLE.md) | DLL config table values |
| [`docs/ACTIVATION.md`](docs/ACTIVATION.md) | Raw mode activation (SET_FEATURE ID5) |
| [`docs/CONTACT_ABI.md`](docs/CONTACT_ABI.md) | Contact struct ABI |
| [`docs/ETW_CSV_FORMAT.md`](docs/ETW_CSV_FORMAT.md) | Windows ETW trace format |
| [`docs/decomp/`](docs/decomp/) | Windows driver decompilation reference |

## License

**GPL-2.0**. See [LICENSE](LICENSE).
