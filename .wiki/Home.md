# SL4A TouchScreen — Linux Kernel Driver

> A reverse-engineered Linux kernel driver for the **Microsoft Surface Laptop 3/4 (AMD)** touchscreen: standard HID single-touch and pen out of the box, plus an experimental raw-heatmap multi-touch pipeline.

The driver speaks the pre-release **HID-over-SPI Version 0 (V0)** protocol that Microsoft's `HidSpiDeviceV0` uses on these panels, over the AMD FCH SPI controller (`AMDI0060`). It is a from-scratch implementation validated against decompiled Windows drivers and real hardware traces — not a fork of an in-tree driver.

## Supported hardware

| Component | Identity | Notes |
|---|---|---|
| Surface Laptop 4 (AMD) | touch controller ACPI `MSHW0231` (HID 045E:0C19) | 72×48 grid, 3456 CapImg cells |
| Surface Laptop 3 (AMD) | touch controller ACPI `MSHW0162` | 78×52 grid, 4056 CapImg cells |
| SPI controller (both) | `AMDI0060` (AMD Cezanne FCH SPI V2, MMIO 0xFEC10000) | PIO mode, 12 MHz, mode 0 |

The driver selects device-specific tuning (grid geometry, CapImg sample count, baseline length) from the ACPI ID at probe time — see [Architecture](Architecture).

## Two operating modes

| Mode | `raw_mode` | What you get | Status |
|---|---|---|---|
| **Standard HID** | `0` (default) | Single-touch + pen, firmware-computed coordinates, ~10 ms reports | **Stable, recommended** |
| **Raw heatmap** | `1` | Sensor-grid data processed host-side into 2–4 finger multi-touch | **Experimental** |

[Standard Touch Mode](Standard-Touch-Mode) is what you should run day to day. [Multi-touch (Experimental)](Multi-touch-Experimental) documents the raw path and its caveats.

## Quick start

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/sl4a-touch.sh install     # hardware check, DKMS build, boot service
sudo reboot
```

After reboot the driver binds automatically. Verify with:

```bash
sudo ./tools/sl4a-touch.sh status      # hardware + runtime state
sudo evtest                            # pick "spi 045E:0C19", touch the screen
```

Full instructions: [Build & Install](Build-and-Install) · [Usage & Troubleshooting](Building-Usage-and-Troubleshooting).

## How it works, in one picture

```
Linux input subsystem (evdev)
        ▲
   hid-generic / hid-input         ┌───────────────────────────┐
        ▲                          │  raw_mode=1               │
   hid_input_report()              │  heatmap → baseline →     │
        │                          │  peaks → CCL → Hungarian  │
  ┌─────┴──────────┐               │  → MT slots → input_mt    │
  │ spi-hid-core   │◄──────────────┤  (mshw0231-raw.c)         │
  │ V0 protocol +  │               └───────────────────────────┘
  │ IRQ sequencer  │
  └─────┬──────────┘
        │ SPI transfers
  ┌─────┴──────────┐
  │ spi-amd        │   AMD FCH SPI V2 PIO controller
  └─────┬──────────┘
        │ SPI bus (12 MHz, mode 0)
   MSHW0231 / MSHW0162 touch controller
```

The two kernel modules, the sequencer state machine and the IRQ model are described in [Architecture](Architecture); the wire format in [Protocol](Protocol) and [Wire Protocol](Wire-Protocol).

## Wiki map

| Page | What it covers |
|---|---|
| [Architecture](Architecture) | Module split, sequencer state machine, IRQ model, sysfs interface, recovery |
| [Protocol](Protocol) | HID-over-SPI V0: discovery, message types, feature exchange, timing |
| [Wire Protocol](Wire-Protocol) | Byte-level framing: headers, bodies, opcodes, PIO continuations |
| [Report Descriptor](Report-Descriptor) | The 936-byte descriptor: collections, reports, PIO read invariant |
| [Touch Pipeline](Pipeline) | Raw multi-touch chain: baseline, peaks, CCL, Hungarian, slots |
| [Config Table](Config-Table) | Windows DLL configuration values and their Linux mapping |
| [Hardware](Hardware) | Both panels, AMD FCH SPI registers, wiring, ACPI power |
| [Standard Touch Mode](Standard-Touch-Mode) | Default mode: report formats 0x40/0x01, why it is stable |
| [Multi-touch (Experimental)](Multi-touch-Experimental) | Raw mode: activation, per-device geometry, operational safety |
| [Build & Install](Build-and-Install) | Installer, DKMS, Secure Boot/MOK, module parameters, build from source |
| [Usage & Troubleshooting](Building-Usage-and-Troubleshooting) | Day-to-day usage, debugging, recovery, common issues |
| [Reverse Engineering](Reverse-Engineering) | Methodology, evidence, decompiled sources, known gaps |
| [Further Reading](Further-Reading) | Repository docs: evidence files, testing procedure, register maps |

## Repository documentation

The repo's `docs/` directory holds the evidence-grade material this wiki is built on: protocol traces, ACPI/decompilation dumps, hardware validation procedures and the chronological investigation record. See [Further Reading](Further-Reading) for the map.

## License and status

GPL-2.0, **beta software**. This is an experimental, reverse-engineered driver — use at your own risk. The standard mode is the qualified profile; everything else is best-effort.
