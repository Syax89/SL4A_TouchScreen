# SL4A TouchScreen

Linux kernel driver for the Microsoft Surface Laptop 4 (AMD) touchscreen,
implementing the MSHW0231 V0 HID-over-SPI transport and an experimental raw
multitouch pipeline on the AMD Cezanne FCH SPI controller.

[![Status](https://img.shields.io/badge/status-beta-orange)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.2.0-brightgreen)](VERSION)
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
| Touch grid | Current experimental raw-pipeline fallback: 72×48 cells |
| Report rate | ~100 Hz (raw mode) |

## Feature Status

| Feature | Implementation status | Release qualification |
|---------|-----------------------|-----------------------|
| HID descriptor discovery | Implemented, with a hardcoded fallback | Hardware matrix required |
| Standard HID report forwarding | Implemented | Contact behavior requires hardware evidence |
| Raw CCL and multitouch pipeline | Implemented | Experimental |
| Cold-boot retry and recovery | Implemented | Hardware matrix required |
| Candidate classification and per-cycle gain | Not implemented | Not planned for v1.2.0 |

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

The raw multitouch pipeline is an experimental implementation that processes
the current 72×48 fallback grid into HID contacts. Its comparison targets and
unresolved frame-layout assumptions are recorded in `docs/EVIDENCE.md`.

| Stage | Function |
|-------|----------|
| **c590 LUT** | Byte-indexed CapImg sample → fixed-point: `max(0, 10000 - (i·22 + 6000))` |
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

Read [`docs/SUPPORT.md`](docs/SUPPORT.md) before installing. This repository is
only for the Surface Laptop 4 AMD `AMDI0060` + `MSHW0231` hardware contract.
The installer now uses the standard profile by default; raw mode requires an
explicit `--raw` option. See [`docs/COMPATIBILITY.md`](docs/COMPATIBILITY.md)
before treating a setup as supported.

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
./tools/install.sh --check
sudo ./tools/install.sh
sudo reboot
```

Use `sudo ./tools/install.sh --raw` only for the experimental raw heatmap
profile. `--check` performs a read-only ACPI and build-prerequisite preflight;
use `--force` only to investigate unsupported hardware.

The DKMS installer contains dependency guidance for Arch/CachyOS,
Ubuntu/Debian, Fedora, and openSUSE. It installs a modprobe configuration;
the kernel binds the modules through their ACPI aliases. Secure Boot remains
unqualified until recorded in the compatibility matrix.

## Module Parameters

```
/etc/modprobe.d/spi-hid.conf:
  options spi_hid raw_mode=N
```

The explicit raw profile written by `install.sh --raw` uses
`raw_mode=Y skip_getfeat=Y`.

| Parameter | Module default | Description |
|-----------|---------|-------------|
| `raw_mode` | 0 | Raw heatmap + multi-touch mode; enabled only by `install.sh --raw` |
| `skip_getfeat` | 1 | Skip GET_FEATURE handshake; raw profile sets `Y` explicitly |
| `ema_alpha` | 7 | EMA smoothing (1-10) |
| `blob_max_distance` | 3 | Hungarian base radius in cells |
| `blob_min_weight` | 1000 | Minimum blob signal weight |
| `blob_debounce` | 3 | New-touch debounce frames |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace period (0 = disabled) |
| `ghost_dist` | 6 | Pre-merge radius in cells |
| `pre_assoc_ratio` | 0 | Pre-association filter (0 = disabled) |
| `grid_cols/rows` | 0/0 | Uses the current 72×48 fallback grid when unset |
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
make -C /lib/modules/$(uname -r)/build M=$PWD/driver modules
sudo cp driver/spi-amd.ko driver/spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
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
| [`docs/SUPPORT.md`](docs/SUPPORT.md) | Supported hardware and release profiles |
| [`docs/COMPATIBILITY.md`](docs/COMPATIBILITY.md) | Hardware validation matrix |
| [`docs/TESTING.md`](docs/TESTING.md) | Reproducible validation procedure |
| [`docs/EVIDENCE.md`](docs/EVIDENCE.md) | Evidence ledger and open discrepancies |

## License

**GPL-2.0**. See [LICENSE](LICENSE).
