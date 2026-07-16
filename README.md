# SL4A_TouchScreen

> Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen

[![Status](https://img.shields.io/badge/status-functional-brightgreen)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.0.0-brightgreen)](VERSION)
[![Hardware](https://img.shields.io/badge/device-surface%20laptop%204%20amd-blue)](#hardware)

> [!WARNING]
> **This is beta software.** It is an experimental, reverse-engineered kernel driver and may cause touchscreen failures, system instability, data loss, or hardware behavior not yet understood. Use it entirely at your own risk. It is provided "as is", without warranty of any kind; the authors and contributors accept no responsibility or liability for damages or problems resulting from its use.

---

## Status: beta — standard HID works

The driver successfully initializes the MSHW0231 touchscreen on the Surface Laptop 4 AMD.
**Single-touch and pen work** via standard HID mode (Report IDs 0x40 and 0x01).
KDE/Wayland recognizes touches correctly — tap, drag, and single-finger gestures all work.

| Feature | Status |
|---------|--------|
| Device initialization (DESCREQ, DEVICE_DESC, RPT_DESC) | Complete |
| HID report descriptor (936 bytes, read from device) | Complete |
| Single-touch X/Y coordinates (Report ID 0x40) | **Working** |
| BTN_TOUCH (tap/lift detection) | **Working** |
| Stylus/Pen (Report ID 0x01) | **Working** |
| Multi-touch (raw heatmap pipeline) | Experimental |
| Grid-to-screen calibration | In progress |

Multi-touch requires experimental `raw_mode=1`. Its frames are DFT antenna data,
not a rectangular capacitive grid, and the raw-mode handshake and coordinate
calibration are not reliable. Keep the default `raw_mode=0` for daily use.

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
│  · Windows-shaped chunked PIO reads  │
│  · 64-byte descriptor continuations  │
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
State 3 (VENDOR_INIT) → restart descriptor discovery on RESET_RSP
```

State 3 is a recovery path. A RESET_RSP there restarts standard descriptor
discovery; it never fabricates a device descriptor or substitutes a report
descriptor. In practice the driver proceeds directly from state 2 to state 4
in standard mode.

Input is IRQ-driven. `poll_interval` is deprecated and ignored; the runtime
stream watchdog is disabled by default.

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

## Installing

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
sudo reboot
```

This builds the driver via **DKMS** for every installed kernel with an available build
tree, removes obsolete `sl4a-touch` DKMS versions, and installs a systemd service that
loads the driver in standard HID mode on every boot via `/etc/modprobe.d/spi-hid.conf`
(`options spi_hid raw_mode=N`). The installer deliberately does not replace loaded
modules: reboot is required after an install or update to load a new `spi-amd`
controller module.

**Multi-distro support** (auto-detected via `/etc/os-release`): Arch/CachyOS (pacman),
Ubuntu/Debian (apt), Fedora (dnf), openSUSE (zypper). Missing dependencies are reported
with distro-specific package names. Interactive on first run; safe to re-run.

To remove everything it installed:

```bash
sudo ./tools/uninstall.sh
sudo reboot
```

The uninstaller removes the service and DKMS installation but leaves active
modules untouched; reboot to stop the driver safely.

This is beta software: single-touch + pen are stable, multi-touch is experimental (see
above). The out-of-tree module taints the kernel. Secure Boot support depends on your
distribution's DKMS signing setup; enroll its signing key through your distribution's
documented MOK process if the module is rejected after reboot.

### After installing

The touchscreen appears as:
- `/dev/input/eventN` — `spi 045E:0C19` (ABS_X, ABS_Y, BTN_TOUCH) for touch
- `/dev/input/eventN` — `spi 045E:0C19 Stylus` for pen

For a boot freeze or initialization failure, use the opt-in trace procedure in
[docs/DEBUGGING.md](docs/DEBUGGING.md). Do not reload `spi_amd` on a live system.

---

## Building from source (developers)

If you're working on the driver itself, build locally for a quick compile check.
To test modified code, install the working tree through DKMS and then reboot:

```bash
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules
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

`driver/sl4a-touch.service` loads the driver via `modprobe` at boot.
`tools/rebuild_and_install.sh` is a compile check that synchronizes the unit
file if needed; it does not install or reload a module:

```bash
./tools/rebuild_and_install.sh
```

The installer and developer workflow use the same versioned
`driver/sl4a-touch.service` file.

---

## Key Technical Decisions

### HID Report Descriptor (device-read, no repair or fallback)

The 936-byte report descriptor is read live from the device and parsed directly. The
earlier apparent `0xFF` corruption was caused by the host controller driver adding a
trailing dummy byte to each continuation: that changed the AMD FCH transaction from
`TX_COUNT=3`/FIFO `0x84` to `TX_COUNT=4`/FIFO `0x85`. The Windows-shaped continuation
keeps the three address bytes, `TX_COUNT=3`, `RX_COUNT=65`, and FIFO `0x84`. No bytes are
patched and no captured descriptor is embedded as a fallback; a parse failure is reported
instead of registering a guessed device.

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
| `rebuild_and_install.sh` | `tools/rebuild_and_install.sh` | Developer compile check and service sync; does not install or reload a module |
| `calibrate_axes.py` | `tools/calibrate_axes.py` | Experimental raw-mode, four-corner axis calibration helper |
| `touchtest` / `touchviz` | `tools/touchtest*`, `tools/touchviz*` | evdev input inspection utilities |
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
| [`docs/DEBUGGING.md`](docs/DEBUGGING.md) | Boot-freeze trace collection |
| [`docs/TESTING.md`](docs/TESTING.md) | Portable protocol tests and kernel-test scope |
| [`docs/NEXT_STEPS.md`](docs/NEXT_STEPS.md) | Current raw-mode and calibration roadmap |
| [`docs/AMD_CONTROLLER_VALIDATION.md`](docs/AMD_CONTROLLER_VALIDATION.md) | AMD FCH SPI controller validation boundary |
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 protocol reference |
| [`docs/STANDARD_BOOT_EVIDENCE.md`](docs/STANDARD_BOOT_EVIDENCE.md) | Evidence boundary for supported standard boot behavior |
| [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) | Dated research journal; later sections supersede earlier entries |
| [`docs/CSV_SEQUENCE.md`](docs/CSV_SEQUENCE.md), [`docs/AMDSPI_DECOMP.md`](docs/AMDSPI_DECOMP.md), [`docs/decomp/`](docs/decomp/) | Historical Windows-trace and decompilation research |

---

## Research Inputs

| File | Size | Description |
|------|------|-------------|
| `traces/*.csv` | Local, untracked | Windows SPI and GPIO captures referenced by research documents |

---

## UEFI / Windows Binaries

These are local, untracked research inputs. They are not distributed by this repository.

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

See [docs/NEXT_STEPS.md](docs/NEXT_STEPS.md). The open work is raw-mode handshake
reliability, raw-data calibration, and upstream-quality patch preparation.

---

## Licensing

Licensing is specified in each source-file header. A repository-wide license file is
not currently supplied.
