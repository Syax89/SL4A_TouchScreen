# SL4A_TouchScreen

Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen
(MSHW0231, VID/PID 045E:0C19).

> [!WARNING]
> **Beta software.** Reverse-engineered kernel driver. May cause touchscreen
> failures or system instability. Use at your own risk. No warranty.

---

## Status

| Feature | Status |
|---------|--------|
| Device initialization (DESCREQ, DEVICE_DESC, RPT_DESC) | Complete |
| HID report descriptor (936 bytes, 100% wire-read) | Complete |
| Single-touch + pen (Report IDs 0x40 and 0x01) | Working |
| Multi-touch raw heatmap (peak, centroid, Hungarian) | Working |
| 2-finger tracking | Very good |
| 3-finger tracking | Good (95%) |
| 4-finger tracking | Unstable |
| Touch ellipse (per-blob eigenvalues) | Working |
| Auto-retry on cold boot handshake failure | Working |
| Stationary lock (no pinch-to-zoom jitter) | Working |

---

## Hardware

| Component | Detail |
|-----------|--------|
| Device | Surface Laptop 4 (AMD) |
| Touchscreen ACPI HID | `MSHW0231` |
| SPI Controller | `AMDI0060` (AMD FCH Cezanne) |
| SPI Mode | MODE 0, 33.33 MHz |
| Protocol | HID-over-SPI **V0** (pre-release) |
| Report descriptor | 936 bytes, 8 collections |

---

## Architecture

```
spi-hid.ko  ─── HID-over-SPI protocol  ─── Linux SPI  ─── spi-amd.ko  ─── MMIO
(state machine)                                │           (AMD FCH V2)    @ 0xFEC10000
   │                                            │
   ├─ IRQ-driven discover (states 0→2)          │
   ├─ Standard HID (Report 0x40 pen/touch)      │
   └─ Raw multi-touch pipeline:                 │
      c590 LUT → baseline → peak detect →       │
      5×5 centroid → Hungarian → EMA/dedband    │
      → lift lookback → eigenvalues → MT emit   │
```

### State machine

```
State 0 (WAIT_RESET)   → DESCREQ → State 1
State 1 (WAIT_DESC)    → DEVICE_DESC → DESCREQ2 → State 2
State 2 (WAIT_RPT)     → RPT_DESC (100% wire-read) → HID device → State 4
State 3 (VENDOR_INIT)  → vendor command → handshake → State 4
State 4 (DATA)         → standard: hid_input_report() | raw: heatmap pipeline
State 5 (WAIT_FEATURE) → raw-mode GET/SET_FEATURE handshake
```

Input is IRQ-driven. No polling, no systemd service — udev auto-loads via
`/etc/modprobe.d/spi-hid.conf` (`raw_mode=1 skip_getfeat=1`).

---

## Protocol

The device uses **HidSpiDeviceV0** (pre-release). See
[docs/HIDSPI_PROTOCOL.md](docs/HIDSPI_PROTOCOL.md).

Key differences from v1.0:

| Aspect | V0 | v1.0 |
|--------|----|------|
| Discovery | `_DSM rev=1 func=0` → `0x03` | `_DSM rev=3 func=0` → `0x7F` |
| Device descriptor | 28 bytes | 24 bytes |
| Header length | `((u16 >> 4) * 4)` units | bits[13:0] bytes |
| Input register | Descriptor offset 8 | ACPI _DSM |
| Write/read | TX-only + separate 0x0B read | Same, via SpbCx |

---

## Installing

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
sudo reboot
```

- **DKMS**: auto-rebuilt for every kernel update.
- **Multi-distro**: Arch/CachyOS, Ubuntu/Debian, Fedora, openSUSE.
- **Auto-load**: udev probes `AMDI0060` → `spi_amd` → `MSHW0231` → `spi_hid`.
- **Secure Boot**: DKMS signs with `/var/lib/dkms/mok.key`; enroll via MOK if needed.

To remove:

```bash
sudo ./tools/uninstall.sh
sudo reboot
```

---

## Pipeline

The raw heatmap pipeline processes 72×48-cell frames matching the Windows
`TouchPenProcessor0C19.dll` chain at ~85% fidelity.

### c590 signal LUT

Maps raw 16-bit cell values to fixed-point ×10000: `c590[i] = max(0, 10000 - (i*0.0222 + 6000))`

### Baseline

30-frame warm-up, asymmetric per-cell EMA (7/8 upward), no update on cells under
a finger.

### Peak detection

Cross-shaped check at ±5 cells (N/S/E/W). Min rise 300 c590 units, max 16
candidates, sorted by strength. Matches Windows `FUN_1805fba00`.

### 5×5 local centroid

Signal-weighted average in ±2 window around each peak, fixed-point ×100
sub-cell precision. Eliminates visible grid stepping (~461 px → ~4.6 px
on a 2256px panel).

### Pre-merge

Coalesce blobs within distance² < 36 cells, keep the strongest.

### Hungarian assignment

Bipartite minimum-cost matching (blobs → slots) with costs matching the
Windows oracle: `sqrt(dx²+dy²)` in-range, 1000000 out-of-range, 1000 dummy.
Jump rejection: if a blob is > 5 cells from its claimed slot, the assignment
is refused — the slot enters lift countdown instead.

### EMA + deadband + stationary lock

- EMA alpha=7 (weight 1/8) for smooth tracking
- Deadband ±0.8 cells: suppresses antenna-noise jitter during holds
- Stationary lock: after 6 consecutive frames within deadband, position frozen
  until real movement — eliminates pinch-to-zoom trembling

### Lift lookback

On finger lift, emit lift at history position from 2 frames ago (matching
Windows `FUN_180601dd0`).

### Per-blob eigenvalues

Second-moment decomposition on the 5×5 window → touch major/minor and
orientation emitted as `ABS_MT_TOUCH_MAJOR/MINOR/ORIENTATION`.

### Module parameters

| Param | Default | Description |
|-------|---------|-------------|
| `raw_mode` | 1 | Enable raw heatmap + multitouch |
| `skip_getfeat` | 1 | Vendor-init + SET_FEATURE activation |
| `ema_alpha` | 7 | EMA smoothing weight |
| `blob_max_distance` | 3 | Hungarian association radius (cells) |
| `blob_min_weight` | 1000 | Minimum blob weight for emission |
| `blob_debounce` | 3 | Frames before claiming new contact |
| `blob_lift_frames` | 2 | Missed frames before lift |
| `grid_cols/rows` | 72/48 | Grid override |
| `calib_scale_x/y` | 0 | Scale ×1000 (0=auto) |
| `calib_offset_x/y` | 0 | Screen offset |
| `invert_x/y` | 0 | Invert axis |
| `swap_xy` | 0 | Swap axes |

---

## Diagnostics

```bash
# Raw mode stream stats
cat /sys/devices/platform/AMDI0060:00/spi_master/spi0/spi-MSHW0231:00/protocol_stats
# → reset_rsp, device_desc, rpt_desc, data, frames_dropped, irq_count, wire_patches

# Baseline convergence
cat /sys/devices/platform/AMDI0060:00/spi_master/spi0/spi-MSHW0231:00/baseline_status
# → frame_count, rows, cols

# Module parameters
cat /sys/module/spi_hid/parameters/*
```

### Key indicators

- `data > 0`: raw mode active and streaming
- `wire_patches = 0`: 100% wire-read descriptor
- `baseline_status frame_count = 30`: baseline converged
- `frames_dropped > 0`: pipeline or queue pressure

---

## Key design decisions

**100% wire-read descriptor.** A PIO TX_COUNT bug (1 extra byte per 64-byte
continuation) corrupted bytes at offsets n·64+55. Fixed by matching the Windows
TX_COUNT=3 convention. The hardcoded descriptor is an emergency fallback only.

**Raw mode via skip_getfeat.** The reliable activation path sends a vendor-init
command (0xC2 opcode) followed by SET_FEATURE (type 3, ID 5, value 1). The
traditional GET_FEATURE → SET_FEATURE path is unreliable.

**Auto-retry on cold boot.** First discovery attempt fails ~30% of the time
after cold boot. The driver retries up to 2 additional times (5s delay,
D2→D0 cycle, fresh DESCREQ), then falls back to standard HID.

**No companion chip.** The touchscreen has no CS0-3 dependency. Probed and
confirmed absent.

**Python oracle validation.** `tools/surface_tracker.py` is a ground-truth
Python reimplementation of the Windows pipeline, used to validate the kernel
pipeline output frame-by-frame.

---

## Building from source

```bash
# Compile check
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules

# Install for current kernel only
sudo cp -f driver/spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a

# Full DKMS install (survives kernel updates)
sudo ./tools/install.sh
sudo reboot

# Offline protocol tests
make -C tests
./tests/protocol_test
```

---

## Documentation

| File | Contents |
|------|----------|
| [docs/HIDSPI_PROTOCOL.md](docs/HIDSPI_PROTOCOL.md) | V0 protocol reference |
| [docs/PIPELINE.md](docs/PIPELINE.md) | Kernel pipeline detailed spec |
| [docs/NEXT_STEPS.md](docs/NEXT_STEPS.md) | Roadmap and open issues |
| [docs/DEBUGGING.md](docs/DEBUGGING.md) | Trace collection |
| [docs/GROUND_TRUTH.md](docs/GROUND_TRUTH.md) | Research journal |
| [docs/SPI_REGISTERS.md](docs/SPI_REGISTERS.md) | AMD FCH SPI register map |
| [docs/AMDSPI_DECOMP.md](docs/AMDSPI_DECOMP.md) | Windows amdspi.sys decomp |

---

## Reference binaries (local, untracked)

| File | Description |
|------|-------------|
| `windrivers/TouchPenProcessor0C19.dll` | Windows touch/pen processing |
| `windrivers/hidspi.sys` | Windows HID-over-SPI transport |
| `windrivers/HidSpiCx.sys` | Windows HidSpiCx class driver |
| `windrivers/amdspi.sys` | Windows AMD SPI controller |
| `traces/*.csv` | ETW wire captures |

---

Licensing is specified in each source-file header.
