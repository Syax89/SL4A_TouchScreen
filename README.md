# SL4A TouchScreen

Linux kernel driver for the Microsoft Surface Laptop 4 (AMD) touchscreen,
implementing the MSHW0231 V0 HID-over-SPI transport and an experimental raw
multitouch pipeline on the AMD Cezanne FCH SPI controller.

[![Status](https://img.shields.io/badge/status-beta-orange)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.2.0-brightgreen)](VERSION)
[![CI](https://github.com/Syax89/SL4A_TouchScreen/actions/workflows/ci.yml/badge.svg)](https://github.com/Syax89/SL4A_TouchScreen/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-GPL--2.0-blue)](LICENSE)

> [!WARNING]
> **Beta software.** This is an experimental, reverse-engineered kernel driver.
> Use at your own risk. No warranty; provided "as is".

## What to Expect

- **Standard HID mode** (default) provides **single-touch only** — basic
  tap, drag, and single-finger interaction. This is the qualified profile.
- A **stylus/pen input node** is published by the HID descriptor but is
  **untested** — pen behavior has not been observed or validated.
- **No multi-touch** in standard mode. Multi-touch requires the
  experimental raw pipeline (`raw_mode=1`).
- **Raw mode is experimental** and not release-qualified. Finger tracking
  degrades at 4 contacts; pipeline tuning is ongoing.

See [`docs/QUICKSTART.md`](docs/QUICKSTART.md) for a 5-step install and
activation guide.

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
│ sl4a-spi-hid.ko (spi-hid-core.c, explicit opt-in only)        │
│   ├─ HID LL driver (DESCREQ → DEVICE_DESC → RPT_DESC)        │
│   ├─ IRQ-driven input (IRQ → SPI read → input_report)        │
│   └─ Raw heatmap pipeline:                                   │
│        baseline → peak gate → CCL → velocity → edge →        │
│        split → centroid → pre-merge → Hungarian →            │
│        EMA + deadband + stationary lock → MT emission         │
├──────────────────────────────────────────────────────────────┤
│ sl4a-spi-amd.ko (spi-amd.c, explicit opt-in only)            │
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
The installer does not load or bind either experimental module. It uses distinct
`sl4a-spi-amd` and `sl4a-spi-hid` module names and does not replace in-tree drivers.
See [`docs/COMPATIBILITY.md`](docs/COMPATIBILITY.md) before treating a setup as
supported.

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
./tools/sl4a-touch.sh install --check
sudo ./tools/sl4a-touch.sh install
```

`install` prompts interactively for a profile (standard HID, the supported
default, or the experimental raw multitouch profile) unless `--standard` or
`--raw` is given explicitly. `--check` performs a read-only ACPI and
build-prerequisite preflight and needs no root; `--force` only to investigate
unsupported hardware.

Only after login, with local/remote recovery access available, activate the
experimental controller with:

```bash
sudo ./tools/sl4a-touch.sh activate
```

The command refuses to displace existing AMDI0060 or MSHW0231 drivers, then
verifies both bindings. To recover after a failed experiment, run
`sudo modprobe -r sl4a-spi-hid sl4a-spi-amd` and reboot. Use
`sudo ./tools/sl4a-touch.sh install --raw` only for the experimental raw
heatmap profile. `./tools/sl4a-touch.sh status` shows the installed version,
active profile, and whether the driver is currently loaded and bound;
`sudo ./tools/sl4a-touch.sh logs` collects a diagnostic bundle for bug reports.

The DKMS installer contains dependency guidance for Arch/CachyOS,
Ubuntu/Debian, Fedora, and openSUSE. Neither module exports aliases or has a
boot-time binding rule. Secure Boot remains unqualified until recorded in the
compatibility matrix. See [`docs/ROLLBACK.md`](docs/ROLLBACK.md) for the
complete rollback and upgrade procedure.

## Module Parameters

```
/etc/modprobe.d/sl4a-spi-hid.conf:
  options sl4a_spi_hid raw_mode=N
```

The explicit raw profile written by `sl4a-touch.sh install --raw` uses
`raw_mode=Y raw_input_beta=Y skip_getfeat=Y`. Every raw control is experimental
and load-time-only. The complete release, diagnostic, and experimental contract
is in [`docs/PARAMETERS.md`](docs/PARAMETERS.md).

## What Will Not Work

- **Multi-touch** in standard mode (`raw_mode=0`)
- **Pen input** — published node, untested and unqualified
- **4-finger tracking** — unstable even in raw mode
- **Palm rejection** — not implemented
- **Other Surface models** — this driver targets only Surface Laptop 4
  (AMD) with `AMDI0060` + `MSHW0231`

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
sudo cp driver/sl4a-spi-amd.ko driver/sl4a-spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo reboot
```

## Documentation

| Document | Content |
|----------|---------|
| [Wiki](https://github.com/Syax89/SL4A_TouchScreen/wiki) | Project wiki: protocol, pipeline, config, hardware |
| [`docs/QUICKSTART.md`](docs/QUICKSTART.md) | 5-step install and activation guide |
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 wire protocol |
| [`docs/PIPELINE.md`](docs/PIPELINE.md) | Touch pipeline specification |
| [`docs/SPI_REGISTERS.md`](docs/SPI_REGISTERS.md) | AMD FCH SPI controller registers |
| [`docs/AMDI0060_CONTRACT.md`](docs/AMDI0060_CONTRACT.md) | AMDI0060 controller boundary and safety contract |
| [`docs/CONFIG_TABLE.md`](docs/CONFIG_TABLE.md) | DLL config table values |
| [`docs/ACTIVATION.md`](docs/ACTIVATION.md) | Raw mode activation (SET_FEATURE ID5) |
| [`docs/CONTACT_ABI.md`](docs/CONTACT_ABI.md) | Contact struct ABI |
| [`docs/ETW_CSV_FORMAT.md`](docs/ETW_CSV_FORMAT.md) | Windows ETW trace format |
| [`docs/decomp/`](docs/decomp/) | Windows driver decompilation reference |
| [`docs/SUPPORT.md`](docs/SUPPORT.md) | Supported hardware and release profiles |
| [`docs/COMPATIBILITY.md`](docs/COMPATIBILITY.md) | Hardware validation matrix |
| [`docs/TESTING.md`](docs/TESTING.md) | Reproducible validation procedure |
| [`docs/EVIDENCE.md`](docs/EVIDENCE.md) | Evidence ledger and open discrepancies |
| [`docs/HARDWARE_VALIDATION.md`](docs/HARDWARE_VALIDATION.md) | Blinded hardware-validation protocol and bounded input captures |

## License

**GPL-2.0**. See [LICENSE](LICENSE).
