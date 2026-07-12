# SL4A_TouchScreen

> Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen

[![Status](https://img.shields.io/badge/status-functional-brightgreen)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.0.0--beta2-yellow)](VERSION)
[![Hardware](https://img.shields.io/badge/device-surface%20laptop%204%20amd-blue)](#hardware)
[![License](https://img.shields.io/badge/license-GPL--2.0%20%7C%20BSD--3-orange)](LICENSE)

---

## Status: FUNCTIONAL — Single-touch + Pen working

The driver successfully initializes the MSHW0231 touchscreen on the Surface Laptop 4 AMD.
**Single-touch and pen work** via standard HID mode (Report IDs 0x40 and 0x01).
KDE/Wayland recognizes touches correctly — tap, drag, and single-finger gestures all work.

| Feature | Status |
|---------|--------|
| Device initialization (DESCREQ, DEVICE_DESC, RPT_DESC) | Complete |
| HID report descriptor (936 bytes, 98.5% wire-read + 14-byte targeted patch) | Complete |
| Single-touch X/Y coordinates (Report ID 0x40) | **Working** |
| BTN_TOUCH (tap/lift detection) | **Working** |
| Stylus/Pen (Report ID 0x01) | **Working** |
| Multi-touch (raw heatmap pipeline) | Experimental |
| Grid-to-screen calibration | In progress |

Multi-touch requires switching the device into raw heatmap mode (`SET_FEATURE(id=4)`)
and processing the raw capacitive frames (~4302 bytes, 288×14 antenna grid) through
connected-component labeling (CCL) to extract blob centroids. The Windows
`TouchPenProcessor0C19.dll` (9.7 MB) handles this with dual-frequency DFT processing,
8-connectivity CCL, eigenvalue decomposition, and Kalman tracking — all now fully
mapped via static analysis.

**The CCL pipeline is functional end-to-end**: the driver streams raw heatmap frames,
computes EMA-smoothed baselines (eliminating thermal drift false positives),
extracts blobs via 8-connectivity union-find, debounces, assigns slots, computes
centroids with eigenvalue orientation (±89°), and emits `ABS_MT_POSITION_X/Y` events.
Verified live with `evtest` — up to 6 concurrent touchpoints detected.

**Current limitations**:
- The raw-mode handshake (`SET_FEATURE`) succeeds only on a fresh cold boot —
  repeated power-cycles degrade the device state. A software watchdog matches
  Windows's own 2000ms-timeout/3-retry behavior but cannot fix the underlying cause.
- Grid-to-screen coordinate mapping is being calibrated. The sensor has 288×14
  antenna cells covering ~100% screen width × ~86% screen height. Two-point
  linear calibration is implemented (`calib_scale_*`, `calib_offset_*` params).
- Standard mode (`raw_mode=N`, the default) is what's stable for daily use.

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
┌──────────────────────────────────────┐
│  spi-hid.ko (driver/spi-hid-core.c)  │
│  HID-over-SPI protocol state machine │
│  · DESCREQ → DEVICE_DESC → RPT_DESC  │
│  · IRQ-driven seq_thread (states 0-5)│
│  · Standard HID mode (Report 0x40)   │
│  · Optional raw heatmap interception │
└──────────────┬───────────────────────┘
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
State 5 (WAIT_FEATURE)→ optional raw-mode GET_FEATURE response → State 4
State 4 (DONE)        → forward input reports via hid_input_report()
State 3 (VENDOR_INIT) → vendor-init fallback path (hardcoded descriptors)
```

State 3 is a recovery path reached when vendor-init mode does not produce
a valid DATA report; it hardcodes the known descriptors and transitions
to state 4. In practice the driver proceeds directly from state 2 to
state 4 in standard mode.

The driver also runs an **active polling loop** (`poll_interval` module
param, default 10 ms) that reads data directly from the SPI controller
alongside the IRQ thread. This recovers from intermittent IRQ silence,
with a stream watchdog providing automatic re-initialisation when the
data stream stops.

**No GET_FEATURE/SET_FEATURE is sent** — the device stays in standard HID mode and sends proper Report ID 0x40 (TouchScreen) and Report ID 1 (Pen) reports with pre-computed X/Y coordinates.

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

## Installing (beta testers)

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
```

This builds the driver via **DKMS**, so it's automatically rebuilt for every kernel you
install afterward, and installs a systemd service that loads the driver in standard HID
mode on every boot via `/etc/modprobe.d/spi-hid.conf` (`options spi_hid raw_mode=N`).

**Multi-distro support** (auto-detected via `/etc/os-release`): Arch/CachyOS (pacman),
Ubuntu/Debian (apt), Fedora (dnf), openSUSE (zypper). Missing dependencies are reported
with distro-specific package names. Interactive on first run; safe to re-run.

To remove everything it installed:

```bash
sudo ./tools/uninstall.sh
```

This is beta software: single-touch + pen are stable, multi-touch is experimental (see
above). It taints the kernel (unsigned out-of-tree module) — if Secure Boot is enforced in a
way that rejects unsigned/DKMS-signed modules, loading will fail; see your distro's DKMS
Secure Boot / MOK enrollment documentation.

### After installing

The touchscreen appears as:
- `/dev/input/eventN` — `spi 045E:0C19` (ABS_X, ABS_Y, BTN_TOUCH) for touch
- `/dev/input/eventN` — `spi 045E:0C19 Stylus` for pen

Status is exposed through the device sysfs directory (`seq_state`,
`protocol_stats`, and `lifecycle_status`). For verbose sequence diagnostics,
set `debug_level=3` and enable the standard kernel dynamic-debug facility
before reproducing an issue:

```bash
sudo sh -c "echo 'module spi_hid +p' > /sys/kernel/debug/dynamic_debug/control"
sudo sh -c "echo 3 > /sys/module/spi_hid/parameters/debug_level"
sudo dmesg -w
```

For sparse lifecycle diagnostics, use tracefs instead of per-packet logging:

```bash
sudo sh -c "echo 1 > /sys/kernel/tracing/events/spi_hid/spi_hid_seq_state/enable"
sudo sh -c "echo 1 > /sys/kernel/tracing/events/spi_hid/spi_hid_lifecycle/enable"
sudo cat /sys/kernel/tracing/trace_pipe
```

---

## Building from source (developers)

If you're working on the driver itself rather than just using it, building/reloading
directly from the working tree (no DKMS involved) is faster to iterate on:

```bash
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules
sudo rmmod spi_hid spi_amd 2>/dev/null
sudo insmod driver/spi-amd.ko
sudo insmod driver/spi-hid.ko
```

### Offline Protocol Tests

The byte-level protocol codec can be tested without root, hardware, or a loaded
module:

```bash
make -C tests
./tests/protocol_test
```

`driver/sl4a-touch.service` is a systemd unit that loads the driver via
`modprobe` at boot or after a DKMS install. For development, use
`tools/rebuild_and_install.sh` (rebuild + reload in one step,
see [Tools](#tools)):

```bash
sudo cp driver/sl4a-touch.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable --now sl4a-touch.service
# after editing driver/ code:
./tools/rebuild_and_install.sh
```

Note this dev-loop service and the beta installer's service share the same unit name
(`sl4a-touch.service`) but are **not** the same file — installing one overwrites the other.
Pick one workflow per machine (DKMS for daily use, direct-checkout for active development).

---

## Key Technical Decisions

### HID Report Descriptor (98.5% wire-read + 14-byte patch)

The 936-byte report descriptor is read live from the device. 14 specific byte positions (at offsets n·64+55 from descriptor start) are corrupted to `0xFF` by a characterized hardware defect in the device's 64-byte page structure. These bytes are patched from a hardcoded ground-truth copy. The remaining 922 bytes (98.5%) come from the live wire read every boot — meaning firmware updates or different SKUs would be picked up.

### No vendor init (Himax, 0x04 register)

Windows cold-boot traces show a 14-byte vendor init write to register 0x04. Testing proved this is optional — the device initializes without it. Sending it may corrupt state.

### Standard HID mode (no raw heatmap)

By NOT sending `SET_FEATURE(id=4)`, the device stays in standard HID mode and sends Report ID 0x40 with pre-computed touch coordinates. This avoids the complex blob detection on raw heatmap data that Windows handles via `TouchPenProcessor0C19.dll` (9.7 MB, DFT processing, CCL, Kalman tracking).

### Companion chip (0x18/0x19/0x1A) not needed

The touchscreen has no companion chip dependency. Probed all CS lines 0-3, chip is absent. The touchscreen works standalone.

---

## Tools

| Tool | Path | Description |
|------|------|-------------|
| `install.sh` | `tools/install.sh` | **Multi-distro installer** (Arch/Debian/Fedora/openSUSE) — DKMS build + systemd service |
| `uninstall.sh` | `tools/uninstall.sh` | Removes everything `install.sh` installed |
| `rebuild_and_install.sh` | `tools/rebuild_and_install.sh` | Developer loop: rebuild in-place and reload (no DKMS) |
| `calibrate.c` | `tools/calibrate.c` | 4-corner evdev calibration capture — builds binary `tools/calibrate` |
| `cli_probe.py` | `tools/cli_probe.py` | Calibration CLI protocol probe (Report ID 0x1f) |
| `parse_spi.py` | `tools/parse_spi.py` | Full ETW CSV parser (transactions, timing, GPIO) |
| `parse_spb_csv.py` | `tools/parse_spb_csv.py` | SPB payload extraction |
| `ghidra/` | `tools/ghidra/` | Headless decompilation scripts for `TouchPenProcessor0C19.dll` |
| `protocol_test` | `tests/protocol_test.c` | Portable V0 protocol codec test (9 suites, 75k assertions) |

Windows-side capture utilities are in `tools/windows_capture/`.

---

## Documentation

| File | Contents |
|------|----------|
| [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) | **Canonical knowledge base** — cross-verified model (CSV × decomp × ACPI × PCI × tests) |
| [`docs/NEXT_STEPS.md`](docs/NEXT_STEPS.md) | Roadmap: handshake reliability, heatmap blob detection, calibration |
| [`docs/AMD_CONTROLLER_VALIDATION.md`](docs/AMD_CONTROLLER_VALIDATION.md) | AMD FCH SPI controller: verified boundaries and pending hardware validation |
| [`docs/SPI_REGISTERS.md`](docs/SPI_REGISTERS.md) | AMD FCH SPI + PCI config register map |
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 protocol decoded |
| [`docs/CSV_SEQUENCE.md`](docs/CSV_SEQUENCE.md) | Annotated Windows boot SPI trace |
| [`docs/AMDSPI_DECOMP.md`](docs/AMDSPI_DECOMP.md) | `amdspi.sys` decompilation index |
| [`docs/decomp/`](docs/decomp/) | `hidspi.sys` / `HidSpiCx.sys` / UEFI DXE decompilations |

---

## Traces

| File | Size | Description |
|------|------|-------------|
| `traces/surface_boot_auto.csv` | 29.3 MB | Warm boot, 2384 SPI transactions |
| `traces/surface_init.csv` | 9.1 MB | Cold boot vendor init sequence |
| `traces/surface_touch.csv` | 19.5 MB | Runtime touch data capture |

---

## UEFI / Windows Binaries

| File | Size | Description |
|------|------|-------------|
| `windrivers/hidspi.sys` | 143 KB | Windows HID-over-SPI transport driver |
| `windrivers/HidSpiCx.sys` | 135 KB | Windows HidSpiCx class extension |
| `windrivers/amdspi.sys` | 233 KB | Windows AMD FCH SPI controller driver |
| `windrivers/TouchPenProcessor0C19.dll` | 9.8 MB | Windows touch/pen processing DLL |
| `windrivers/AmdSpiHcProtocolDxe.efi` | 11 KB | UEFI SPI host controller protocol |
| `windrivers/SurfaceTouchHidDxe.efi` | 15 KB | UEFI touch HID driver |
| `windrivers/MsTouchUnlockDxe.efi` | 15 KB | UEFI touch unlock/calibration |
| `windrivers/HidSpiProtocolSpec.pdf` | 1.3 MB | Microsoft HID-over-SPI v1.0 spec |

---

## Next Steps

- **Multi-touch handshake reliability**: `raw_mode=Y` (see `probe_raw_id`) sends `SET_FEATURE` to switch into raw heatmap streaming. The feature selector is device-state-specific (Windows uses `content_id=4`; confirmed working on first cold boot). The handshake degrades after repeated power-cycles; a watchdog retries automatically (2000ms timeout, 3 retries — matching Windows's own `CheckingResetRetryCountEntry` in `HidSpiCx.sys`) but cannot fix the underlying hardware state issue. **Workaround**: cold boot before raw-mode testing.
- **Grid-to-screen calibration**: the driver computes blob centroids from the 288×14 heatmap grid and scales them to screen coordinates (0..32767). Calibration parameters (`calib_scale_x`, `calib_scale_y`, `calib_offset_x`, `calib_offset_y`) are extracted from `TouchPenProcessor0C19.dll` (X scale = `32767/287`, Y scale = 2730.78, derived from DLL's per-device context tables). The `tools/calibrate` utility supports empirical 4-corner capture. Resolution values match the HID descriptor (X=112 ppi, Y=198 ppi).
- **Windows ETW trace**: capturing a fresh `TouchAndPen.Prod` trace on Windows would empirically confirm the `TouchBlobCoMX`/`TouchBlobCoMY` coordinate mapping, providing ground-truth calibration data without needing to fully reverse-engineer the DLL's DFT math.
- **Upstreaming**: split into proper Linux kernel patches (SPI controller + HID transport).

---

## License

Dual **GPL-2.0** / **BSD-3-Clause**. See source file headers.
