# SL4A TouchScreen — Linux Kernel Driver

> Historical research summary. `docs/SUPPORT.md`, `docs/COMPATIBILITY.md`, and
> `docs/EVIDENCE.md` define the current release contract and take precedence.

Linux kernel driver for the Surface Laptop 4 (AMD) touchscreen, providing
standard HID single-touch, pen, and raw multi-touch via a V0 HID-over-SPI
transport on the AMD FCH SPI controller.

**Device**: MSHW0231 (045E:0C19), experimental 72×48 fallback grid
**Transport**: HID-over-SPI V0 protocol (AMD Cezanne FCH SPI controller V2)  
**Kernel**: historical research environment; see `docs/COMPATIBILITY.md`
**Alignment**: experimental pipeline comparison, not a release claim

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
│         │       SPI bus (ACPI: 33.33 MHz)          │             │
│  MSHW0231 Touch MCU ◄──────────────────────────────             │
│  045E:0C19                                                      │
└─────────────────────────────────────────────────────────────────┘
```

## Feature Status

| Feature | Status |
|---------|--------|
| HID report descriptor | Implemented; hardware qualification pending |
| Standard HID report forwarding | Implemented; contact qualification pending |
| Multi-touch CCL pipeline | Experimental |
| Touch ellipse and stationary lock | Experimental |
| Cold boot auto-retry handshake | Implemented; hardware qualification pending |

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
sudo ./tools/sl4a-touch.sh install
# → After login: sudo ./tools/sl4a-touch.sh activate
#   (never automatic by design — no module aliases are exported)
```
