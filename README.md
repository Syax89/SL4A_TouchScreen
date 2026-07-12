# SL4A_TouchScreen

> Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen

[![Status](https://img.shields.io/badge/status-functional-brightgreen)](https://github.com/Syax89/SL4A_TouchScreen)
[![Release](https://img.shields.io/badge/release-1.0.0--beta1-yellow)](VERSION)
[![Hardware](https://img.shields.io/badge/device-surface%20laptop%204%20amd-blue)](#hardware)
[![License](https://img.shields.io/badge/license-GPL--2.0%20%7C%20BSD--3-orange)](LICENSE)

---

## Status: FUNCTIONAL — Single-touch working

The driver successfully initializes the MSHW0231 touchscreen on the Surface Laptop 4 AMD. **Single-touch works** via standard HID mode (Report ID 0x40). The KDE/Wayland desktop recognizes touches correctly — tap, drag, and single-finger gestures all work.

| Feature | Status |
|---------|--------|
| Device initialization (DESCREQ, DEVICE_DESC, RPT_DESC) | Complete |
| HID report descriptor (936 bytes, 98.5% wire-read + 14-byte targeted patch) | Complete |
| Single-touch X/Y coordinates (Report ID 0x40) | **Working** |
| BTN_TOUCH (tap/lift detection) | **Working** |
| Stylus/Pen (Report ID 1) | **Working** |
| Multi-touch (2+ fingers) | Experimental — see below |

Multi-touch requires processing the raw capacitive heatmap frames (content_id=0x0C, ~4302 bytes) through blob detection. The Windows `TouchPenProcessor0C19.dll` handles this with DFT processing on dual-frequency antenna data. This is a complex computer-vision task (see [docs/NEXT_STEPS.md](docs/NEXT_STEPS.md) §D).

An experimental `raw_mode=1` module param exists (default `0`, off) that sends `GET_FEATURE`/`SET_FEATURE` to switch the device into raw heatmap streaming and runs a connected-component blob detector to emit real multitouch events. The blob detection and slot-tracking logic is confirmed correct (verified live with `evtest`), but the `SET_FEATURE` handshake itself only succeeds intermittently for reasons not yet root-caused (a software watchdog retries automatically — same 2000ms-timeout/3-retry pattern Windows's own driver uses, discovered by decompiling it — but doesn't fix the underlying cause). Not recommended for daily use; standard mode (`raw_mode=0`, the default) is what's stable.

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

### Wire format (V0 "version 2")

```
Header: [TYPE:4|VERSION:4] [u16 LE report_length] [0x5A]
        version = 2
Body:   [content_length LE] [content_id] [payload...]
```

### Report types

| Type | Name | Description |
|------|------|-------------|
| `1` | DATA | Runtime input reports |
| `3` | RESET_RSP | Device reset signal |
| `5` | GET_FEAT_RESP | Feature report response |
| `7` | DEVICE_DESC | 28-byte device descriptor |
| `8` | RPT_DESC | 936-byte HID report descriptor |

### Active report IDs (standard HID mode)

| Report ID | Usage | Fields |
|-----------|-------|--------|
| `0x40` (64) | TouchScreen | TipSwitch (1 bit), X (16-bit), Y (16-bit) |
| `0x01` (1) | Pen/Stylus | InRange, TipSwitch, BarrelSwitch, Invert, Eraser, X, Y, TipPressure |

---

## Installing (beta testers)

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
```

This builds the driver via **DKMS**, so it's automatically rebuilt for every kernel you
install afterward (no manual rebuild needed after a kernel update — a real concern for an
out-of-tree module like this one), and installs a systemd service that loads it in the
stable configuration (`raw_mode=0`, standard mode) on every boot. It's interactive the first
time (asks for confirmation if it can't find the `MSHW0231` ACPI device) and safe to re-run.

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
| `install.sh` | `tools/install.sh` | **Beta installer** — builds via DKMS, installs the systemd auto-load service |
| `uninstall.sh` | `tools/uninstall.sh` | Removes everything `install.sh` installed |
| `rebuild_and_install.sh` | `tools/rebuild_and_install.sh` | Developer loop: rebuild in-place and reload via `driver/sl4a-touch.service` (no DKMS) |
| `cli_probe.py` | `tools/cli_probe.py` | Calibration CLI protocol probe against the touch controller |
| `parse_spi.py` | `tools/parse_spi.py` | Full ETW CSV parser (transactions, timing, GPIO) |
| `parse_spb_csv.py` | `tools/parse_spb_csv.py` | SPB payload extraction |
| `ghidra/` | `tools/ghidra/` | Headless decompilation scripts for TouchPenProcessor0C19.dll |

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

- **Multi-touch handshake reliability**: `raw_mode=1` (experimental, see `docs/NEXT_STEPS.md` §D) enables `SET_FEATURE`-based raw heatmap streaming with blob detection. The feature selector is firmware/device-state-specific: Windows traces use selector 4 while this Linux device has empirically streamed with selector 5. Do not scan selectors on a live device; the handshake can silence the controller and the underlying cause remains under investigation.
- **Multi-touch coordinate mapping**: once connected, reverse-engineer the DFT antenna layout in `TouchPenProcessor0C19.dll` to correctly map blob centroids to screen coordinates
- **Upstreaming**: split into proper Linux kernel patches (SPI controller + HID transport)

---

## License

Dual **GPL-2.0** / **BSD-3-Clause**. See source file headers.
