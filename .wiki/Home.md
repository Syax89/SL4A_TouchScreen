# SL4A TouchScreen — Linux Kernel Driver

Linux kernel driver for the Surface Laptop 4 (AMD) touchscreen, providing
standard HID single-touch, pen, and raw multi-touch via a V0 HID-over-SPI
transport on the AMD FCH SPI controller.

**Device**: MSHW0231 (045E:0C19), 72×48 mutual-capacitance grid  
**Transport**: HID-over-SPI V0 protocol (AMD Cezanne FCH SPI controller V2)  
**Kernel**: 7.1.3-2-cachyos (DKMS `sl4a-touch/1.1.0`)  
**Alignment**: ~85% with Windows `TouchPenProcessor0C19.dll` pipeline

## Architecture

```
┌──────────────────────────────────────────────────────────────┐
│  Userspace                                                     │
│  libinput / X11 / Wayland ← evdev ← hid-multitouch            │
├──────────────────────────────────────────────────────────────┤
│  Kernel                                                        │
│                                                                 │
│  ┌─────────────┐   ┌──────────────┐   ┌───────────────────┐  │
│  │ spi-hid-core │   │  spi-hid-core │   │    spi-amd         │  │
│  │  HID LL      │ ← │  Heatmap + MT │ ← │  AMD FCH SPI V2   │  │
│  │  transport   │   │  Pipeline     │   │  PIO controller    │  │
│  └──────┬───────┘   └──────────────┘   └────────┬──────────┘  │
│         │                                         │             │
├─────────┼─────────────────────────────────────────┼─────────────┤
│ Hardware│                                         │             │
│         │            SPI bus (12 MHz)             │             │
│  MSHW0231 Touch MCU ◄──────────────────────────────             │
│  045E:0C19                                                      │
└─────────────────────────────────────────────────────────────────┘
```

## Feature Status

| Feature | Status |
|---------|--------|
| HID report descriptor (936 bytes, 100% wire-read) | Complete |
| Single-touch + pen (Report ID 0x40 and 0x01) | Stable |
| Multi-touch CCL pipeline (peak gate + flood-fill + Hungarian) | Functional |
| Touch ellipse (per-blob eigenvalues) | Working |
| Stationary lock (eliminates pinch-to-zoom jitter) | Working |
| Cold boot auto-retry handshake | Working |
| 2-finger tracking | Excellent |
| 3-finger tracking | Good |
| 4-finger tracking | Partial |

## Navigation

- [Protocol: HID over SPI V0](Protocol) — Wire format, DESCREQ, SET_FEATURE activation
- [Touch Pipeline](Pipeline) — Peak gate, CCL, centroid, Hungarian, EMA, emission
- [Config Table](Config-Table) — Values extracted from `TouchPenProcessor0C19.dll`
- [Hardware Reference](Hardware) — MSHW0231, AMD FCH SPI controller, GPIO
- [Build & Install](Build-and-Install) — DKMS, module parameters, troubleshooting
- [Reverse Engineering](Reverse-Engineering) — Methodology, decompilation, known gaps

## Quick Install

```bash
git clone git@github.com:Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
# → Reboot, driver auto-loads via udev + /etc/modprobe.d/spi-hid.conf
```
