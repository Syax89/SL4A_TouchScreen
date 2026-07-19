# SL4A_TouchScreen

> Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen

[![Status](https://img.shields.io/badge/status-functional-brightgreen)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.0.0-brightgreen)](VERSION)
[![Hardware](https://img.shields.io/badge/device-surface%20laptop%204%20amd-blue)](#hardware)

> [!WARNING]
> **This is beta software.** It is an experimental, reverse-engineered kernel driver and may cause touchscreen failures, system instability, data loss, or hardware behavior not yet understood. Use it entirely at your own risk. It is provided "as is", without warranty of any kind; the authors and contributors accept no responsibility or liability for damages or problems resulting from its use.

---

## Status: beta — raw multi-touch ~85% Windows alignment

Standard HID (single-touch + pen) is stable. Raw multi-touch mode (`raw_mode=1`)
is the default and is functional: 2-finger very good, 3-finger good, 4-finger
improved. The pipeline matches Windows `TouchPenProcessor0C19.dll` at ~85%
fidelity with config values extracted from the DLL data segment.

| Feature | Status |
|---------|--------|
| Device initialization (DESCREQ, DEVICE_DESC, RPT_DESC) | Complete |
| HID report descriptor (936 bytes, 100% wire-read) | Complete |
| Single-touch + pen (Report ID 0x40 and 0x01) | **Working** |
| Multi-touch CCL pipeline (peak gate + flood-fill + Hungarian) | **Working** |
| Auto-retry on cold boot handshake failure | **Working** |
| Touch ellipse (per-blob eigenvalues) | **Working** |
| Stationary lock (no pinch-to-zoom jitter) | **Working** |
| Velocity rejection (Windows dist² ≤ 36.0) | **Working** |
| Edge penalty (Windows config+0x8D0/0x8D4) | **Working** |
| Multi-finger association radii (Windows +0x8DC-0x8EC) | **Working** |
| 3+ finger tracking | Improved (min_rise=200) |
| Candidate classification | Not implemented |
| 4+ finger tracking | Partial contact loss |

---

## Hardware

| Component | Detail |
|-----------|--------|
| Device | Surface Laptop 4 (AMD) |
| Touchscreen ACPI HID | `MSHW0231` (falls back to `PNP0C51`) |
| SPI Controller | `AMDI0060` (AMD FCH Cezanne) |
| MMIO Base | `0xFEC10000` |
| FIFO | 70 bytes at `0x80` |
| GPIO IRQ | Pin `0x55` (DSDT), Power=`0x5B`, Reset=`0x0103` |
| SPI Mode | MODE 0 (CPOL=0, CPHA=0) at 33.33 MHz |
| Protocol | HID-over-SPI **V0** (pre-release, "version 2" wire format) |
| VID/PID | 0x045E / 0x0C19 |
| Report descriptor | 936 bytes (HID v1.00, 8 top-level collections) |

---

## Architecture

```
┌─────────────────────────────────────────────────┐
│  spi-hid.ko (driver/spi-hid-core.c)             │
│  HID-over-SPI protocol state machine            │
│  · DESCREQ → DEVICE_DESC → RPT_DESC             │
│  · IRQ-driven seq_thread (states 0-5)           │
│  · Standard HID mode (Report 0x40)              │
│  · Raw heatmap pipeline:                        │
│    baseline → peak detect → 5×5 centroid →      │
│    Hungarian → EMA+deadband+stationary lock →   │
│    eigenvalues → MT emission                    │
│  · Auto-retry: 3 probe attempts on handshake    │
│    failure (5s delay, D2→D0+DESCREQ restart)    │
└──────────────┬──────────────────────────────────┘
               │ Linux SPI framework
┌──────────────┴───────────────────────┐
│  spi-amd.ko (driver/spi-amd.c)       │
│  AMD FCH SPI controller driver (V2)  │
│  · TX/RX FIFO management             │
│  · Chunked reads (>70B = DMA quanta) │
│  · PIO remainder path                │
└──────────────┬───────────────────────┘
               │ MMIO
┌──────────────┴───────────────────────┐
│  AMD FCH SPI Controller @ 0xFEC10000 │
└──────────────────────────────────────┘
```

The driver uses an **IRQ-driven sequencer** (`spi_hid_seq_thread`) that mirrors the Windows HidSpiCx automaton:

```
State 0 (WAIT_RESET)  → drain RESET_RSP → DESCREQ → State 1
State 1 (WAIT_DESC)   → read DEVICE_DESC → DESCREQ2 → State 2
State 2 (WAIT_RPT)    → read RPT_DESC → create HID device → State 4
State 3 (VENDOR_INIT) → vendor-init fallback path (hardcoded descriptors)
State 4 (DONE)        → forward input reports (standard: hid_input_report, raw: heatmap pipeline)
State 5 (WAIT_FEATURE)→ raw-mode GET_FEATURE/SET_FEATURE handshake → State 4
```

In raw mode the driver sends `vendor_init` (0xC2 opcode) followed by
`SET_FEATURE` (type 3, ID 5, value 1). In standard mode (`raw_mode=0`) no
feature exchange is sent — the device stays in HID mode and delivers Report
ID 0x40 pre-computed coordinates.

Input is IRQ-driven. `poll_interval` is deprecated and ignored; the runtime
stream watchdog is disabled by default.

---

## Protocol

The device uses the **HidSpiDeviceV0** path (pre-release protocol), not the public v1.0 spec.
For the complete reference with wire frames, state machine, and decomp-confirmed details,
see **[docs/HIDSPI_PROTOCOL.md](docs/HIDSPI_PROTOCOL.md)**.

Key differences from spec v1.0:

| Aspect | V0 (MSHW0231) | v1.0 (HidSpiCx) |
|--------|---------------|------------------|
| Discovery | `_DSM rev=1 func=0` → `0x03` | `_DSM rev=3 func=0` → `0x7F` |
| Device descriptor | 28 bytes (I2C-like layout) | 24 bytes |
| Header v4 length | `((u16 >> 4) * 4)` units | bits[13:0] = length in bytes |
| Fragment support | None (low nibble reserved = 0) | LFF bit in header |
| Input register | From device descriptor offset 8 | From ACPI _DSM |
| Write mechanism | TX-only (separate 0x0B read for response) | TX-only + separate read |
| State machine | 62-state V0 FSM (direct MMIO) | SmFx FSM (SpbCx abstraction) |

---

## Installing

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
sudo reboot
```

This builds the driver via **DKMS**, so it's automatically rebuilt for every kernel you
install afterward. Udev auto-loads the driver when `AMDI0060` (AMD FCH SPI controller)
is probed. `/etc/modprobe.d/spi-hid.conf` is configured with `raw_mode=Y skip_getfeat=Y`
for multi-touch raw mode. The installer does not replace loaded modules: reboot is
required after an install or update.

**Multi-distro support** (auto-detected via `/etc/os-release`): Arch/CachyOS (pacman),
Ubuntu/Debian (apt), Fedora (dnf), openSUSE (zypper). Missing dependencies are reported
with distro-specific package names. Interactive on first run; safe to re-run.

To remove everything it installed:

```bash
sudo ./tools/uninstall.sh
sudo reboot
```

The uninstaller removes the DKMS installation but leaves active
modules untouched; reboot to stop the driver safely.

This is beta software: single-touch + pen are stable, multi-touch is working (2-finger
very good, 3+ finger gradual contact loss). The out-of-tree module taints the kernel.
Secure Boot support depends on your distribution's DKMS signing setup; enroll its
signing key through your distribution's documented MOK process if the module is
rejected after reboot.

### After installing

The touchscreen appears as:
- `/dev/input/eventN` — `MSHW0231 Touchscreen` (multi-touch) for all touch contacts
- `/dev/input/eventN` — `MSHW0231 Stylus` for pen

For a boot freeze or initialization failure, use the opt-in trace procedure in
[docs/DEBUGGING.md](docs/DEBUGGING.md). Do not reload `spi_amd` on a live system.

---

## Building from source (developers)

If you're working on the driver itself, build locally for a compile check.
To test modified code, install the working tree through DKMS and then reboot:

```bash
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules
sudo cp -f driver/spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo ./tools/install.sh
sudo reboot
```

### Offline Protocol Tests

The byte-level protocol codec can be tested without root, hardware, or a loaded
module:

```bash
make -C tests
./tests/protocol_test
```

---

## Key Technical Decisions

### HID Report Descriptor (100% wire-read)

The 936-byte report descriptor is read live from the device every boot.
A historical PIO TX_COUNT bug caused corruption in the continuation path
(byte `n*64+55` shifting), fixed by matching the Windows `0x4bac` TX_COUNT=3
convention. The hardcoded descriptor remains as an emergency fallback in case
`hid_parse_report()` rejects the wire bytes.

### Raw Mode Activation

The device enters raw (heatmap) mode via `SET_FEATURE` (type 0x03, feature ID 5,
value 0x01). The `skip_getfeat=1` path first sends a vendor-init command
(0xC2 opcode) to prepare the device, then writes the SET_FEATURE frame.
After cold boot the first handshake attempt may fail intermittently; the driver
auto-retries up to 2 additional times (3 total attempts) with a 5-second delay
and a full D2→D0 power cycle between each, then falls back to standard HID.

### Raw Heatmap Pipeline

The pipeline matches the Windows `TouchPenProcessor0C19.dll` processing chain
(config values extracted from DLL data segment at `0x1808E0460`):

1. **c590 LUT**: maps raw 16-bit to fixed-point `c590[i] = max(0, 10000 - (i*22 + 6000))`
2. **Baseline** (30 frames): asymmetric EMA, noise floor c590 < 400 (Windows 0.04)
3. **Peak detection gate** (Windows FUN_1805fba00): cross-shaped ±5 cells, min_rise=200
4. **CCL flood-fill** (Windows FUN_180600c40): 4-connected BFS, pixel_count≥2, max_rise≥200
5. **Velocity rejection**: blob within 6 cells of peak (Windows dist² ≤ 36.0)
6. **Edge penalty**: bottom edge ×23%, other edges ×97% (Windows config+0x8D0/0x8D4)
7. **Blob splitting**: multi-peak blobs split by internal peak positions
8. **5×5 / full-blob centroid** ×100 fixed-point, eigenvalues on bounding box
9. **Pre-merge**: ghost_dist=6, distance²<36 (Windows config+0xC98=36.0)
10. **Hungarian assignment** with multi-finger radii (1×2.2, 2×1.0, 3×2.8, 4×3.4, 5+×4.0)
11. **EMA + deadband + stationary lock**: alpha=7, ±80 deadband, 6-frame lock
12. **Lift lookback**: 2-frame history position on lift

Module parameters for tuning: `blob_max_distance`, `blob_min_weight`, `blob_debounce`,
`blob_lift_frames`, `ema_alpha`, `grid_cols`, `grid_rows`, `calib_scale_x/y`,
`calib_offset_x/y`, `invert_x`, `invert_y`, `swap_xy`.

### Companion chip (0x18/0x19/0x1A) not needed

The touchscreen has no companion chip dependency. Probed all CS lines 0-3, chip
is absent. The touchscreen works standalone.

---

## Tools

| Tool | Path | Description |
|------|------|-------------|
| `install.sh` | `tools/install.sh` | Multi-distro DKMS installer |
| `uninstall.sh` | `tools/uninstall.sh` | Removes DKMS installation |
| `rebuild_and_install.sh` | `tools/rebuild_and_install.sh` | Developer compile check + DKMS sync |
| `surface_tracker.py` | `tools/surface_tracker.py` | Python oracle: Windows pipeline reference |
| `parse_spi.py` | `tools/parse_spi.py` | ETW CSV parser (SPI traces) |
| `cli_probe.py` | `tools/cli_probe.py` | Calibration CLI protocol probe (Report ID 0x1f) |
| `ghidra/` | `tools/ghidra/` | Decompilation scripts for `TouchPenProcessor0C19.dll` |
| `protocol_test` | `tests/protocol_test.c` | Portable V0 protocol codec test |

---

## Documentation

| File | Contents |
|------|----------|
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 protocol reference |
| [`docs/PIPELINE.md`](docs/PIPELINE.md) | Kernel pipeline detailed specification |
| [`docs/NEXT_STEPS.md`](docs/NEXT_STEPS.md) | Roadmap and open issues |
| [`docs/DEBUGGING.md`](docs/DEBUGGING.md) | Trace collection |
| [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) | Research journal |
| [`docs/SPI_REGISTERS.md`](docs/SPI_REGISTERS.md) | AMD FCH SPI register map |
| [`docs/AMDSPI_DECOMP.md`](docs/AMDSPI_DECOMP.md), [`docs/CSV_SEQUENCE.md`](docs/CSV_SEQUENCE.md), [`docs/decomp/`](docs/decomp/) | Historical Windows decomp research |

---

## Next Steps

See [docs/NEXT_STEPS.md](docs/NEXT_STEPS.md). The open work is 3+ finger tracking
reliability, coordinate calibration, and upstream-quality patch preparation.

---

## Licensing

SPDX-License-Identifier: GPL-2.0. See [LICENSE](LICENSE) for the full text.
