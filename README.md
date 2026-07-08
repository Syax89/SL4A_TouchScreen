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
│  · IRQ-driven seq_thread (states 0-4)│
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
State 4 (DONE)        → forward input reports via hid_input_report()
```

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

Debug: `sudo dmesg -w | grep "SEQ\|spi-hid"`

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

`driver/sl4a-touch.service` (a systemd unit that `insmod`s straight from this checkout,
absolute path, no DKMS) plus `tools/rebuild_and_install.sh` (rebuild + reload in one step,
see [Tools](#tools)) automate this loop:

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
| `reset_touch.sh` | `tools/reset_touch.sh` | Power-cycle just the touchscreen via ACPI `\M010`, no reboot needed |
| `parse_spi.py` | `tools/parse_spi.py` | Full ETW CSV parser (2384 transactions, timing, GPIO) |
| `parse_spb_csv.py` | `tools/parse_spb_csv.py` | SPB payload extraction |
| `decode_hidspi.py` | `tools/decode_hidspi.py` | HID-over-SPI protocol decoder |
| `extract_companion.py` | `tools/extract_companion.py` | Companion device sequence extractor |
| `test_coldboot.sh` | `tools/test_coldboot.sh` | Virgin-boot test harness |
| `ghidra/` | `tools/ghidra/` | Headless decompilation scripts |

The early bring-up phase (proving DESCREQ/writes reach the device at all) used dozens of
one-off raw-MMIO kernel modules and exploratory shell scripts; now that the driver works
end-to-end, those are removed (still recoverable from git history) — only the tools still
useful going forward are listed above.

---

## Documentation

| File | Contents |
|------|----------|
| [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) | **Canonical knowledge base** — cross-verified model (CSV × decomp × ACPI × PCI × tests), the full investigation journal |
| [`docs/NEXT_STEPS.md`](docs/NEXT_STEPS.md) | Roadmap: multi-touch handshake reliability + heatmap blob detection |
| [`docs/SPI_REGISTERS.md`](docs/SPI_REGISTERS.md) | AMD FCH SPI + PCI config register map |
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 protocol decoded |
| [`docs/CSV_SEQUENCE.md`](docs/CSV_SEQUENCE.md) | Annotated Windows boot SPI trace |
| [`docs/AMDSPI_DECOMP.md`](docs/AMDSPI_DECOMP.md) | `amdspi.sys` decompilation index |
| [`docs/decomp/uefi/`](docs/decomp/uefi/) | Decompiled UEFI DXE drivers (SurfaceTouchHidDxe, AmdSpiHc, MsTouchUnlock) |
| [`docs/decomp/clean/`](docs/decomp/clean/) | Clean `hidspi.sys` and `HidSpiCx.sys` decompilations |

Earlier bring-up-phase docs (independent SPI-controller bug audit from before the driver
worked, verification methodology, a stale mid-investigation session summary, an early
hardware-identification analysis whose conclusions were later superseded by the working
driver itself) have been removed now that the driver is functional — recoverable from git
history if needed.

### Verification reports

| File | Verdict |
|------|---------|
| [`docs/verification/amdspi-decomp-report.md`](docs/verification/amdspi-decomp-report.md) | SPI controller register-level verification |
| [`docs/verification/protocol-verification-report.md`](docs/verification/protocol-verification-report.md) | HID protocol verification (spec + decomp + CSV) |
| [`docs/verification/csv-verification-report.md`](docs/verification/csv-verification-report.md) | CSV trace ground-truth verification |

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

- **Multi-touch handshake reliability**: `raw_mode=1` (experimental, see `docs/NEXT_STEPS.md` §D) enables `SET_FEATURE`-based raw heatmap streaming with blob detection, but the device only accepts `SET_FEATURE` intermittently for reasons not yet understood at the hardware/firmware level (a software watchdog retries automatically, matching Windows's own behavior, but doesn't fix the root cause)
- **Multi-touch coordinate mapping**: once connected, reverse-engineer the DFT antenna layout in `TouchPenProcessor0C19.dll` to correctly map blob centroids to screen coordinates
- **Upstreaming**: split into proper Linux kernel patches (SPI controller + HID transport)

---

## License

Dual **GPL-2.0** / **BSD-3-Clause**. See source file headers.
