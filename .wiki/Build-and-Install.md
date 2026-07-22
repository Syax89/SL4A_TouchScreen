# Build & Install

## Prerequisites

- Linux kernel 6.x+ with headers installed
- Microsoft Surface Laptop 4 AMD with `AMDI0060` and `MSHW0231`
- `make`, `clang` or `gcc`, DKMS

## DKMS Installation (Recommended)

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
sudo reboot
```

The installer:
1. Copies the driver source to `/usr/src/sl4a-touch-<version>/`
2. Registers with DKMS
3. Builds the two kernel modules
4. Creates `/etc/modprobe.d/spi-hid.conf`

Module signing is delegated to the distribution DKMS configuration. The
installer does not install a udev rule; the kernel binds modules through ACPI
aliases.

After reboot, the driver loads automatically when the MSHW0231 ACPI
device is enumerated.

### Uninstall

```bash
sudo ./tools/uninstall.sh
```

## Manual Build

```bash
cd driver
make -C /lib/modules/$(uname -r)/build M=$PWD modules

# Manual install
sudo cp spi-amd.ko spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo modprobe spi-amd
sudo modprobe spi-hid
```

## Module Parameters

Load-time parameters in `/etc/modprobe.d/spi-hid.conf`:

```
options spi_hid raw_mode=N
```

`sudo ./tools/install.sh --raw` writes the experimental raw profile:
`options spi_hid raw_mode=Y skip_getfeat=Y`.

| Parameter | Module default | Description |
|-----------|---------|-------------|
| `raw_mode` | N | Enable experimental raw heatmap + multi-touch mode with `install.sh --raw` |
| `skip_getfeat` | Y | Skip GET_FEATURE handshake |
| `ema_alpha` | 7 | EMA smoothing coefficient (1-10) |
| `blob_max_distance` | 3 | Hungarian association base radius (cells) |
| `blob_min_weight` | 1000 | Minimum blob signal weight |
| `blob_debounce` | 3 | New-touch debounce frames |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace period (0 = disabled) |
| `ghost_dist` | 6 | Pre-merge radius in cells |
| `pre_assoc_ratio` | 0 | Pre-association weight filter (0 = disabled) |
| `grid_cols` | 0 | Current fallback is 72 columns |
| `grid_rows` | 0 | Current fallback is 48 rows |
| `calib_scale_x` | 0 | X scale ×1000 (0 = auto from descriptor) |
| `calib_scale_y` | 0 | Y scale ×1000 (0 = auto) |
| `calib_offset_x` | 0 | X offset in screen pixels |
| `calib_offset_y` | 0 | Y offset in screen pixels |
| `invert_x` | 0 | Invert X axis |
| `invert_y` | 0 | Invert Y axis |
| `swap_xy` | 0 | Swap X and Y axes |

## Verification

```bash
# Check driver loaded
lsmod | grep spi_hid

# Observe HID reports if the device emits them
ls /sys/class/hidraw/
sudo evtest  # select the device and record emitted events

# Inspect protocol and raw-frame state
find /sys/bus/spi/devices -name protocol_stats -o -name baseline_status

# Monitor traces
sudo cat /sys/kernel/debug/tracing/trace
```

## Troubleshooting

### Cold boot: no touch after boot

The device may need a full power cycle after extended rmmod cycles:
1. Power off completely
2. Unplug AC adapter
3. Wait 30 seconds
4. Power on

Recovery timing is experimental; record the observed sequence in the hardware
test matrix rather than relying on a fixed retry claim.

### No multi-touch, no contact

Raw mode is experimental. Confirm that the `--raw` profile was selected and
inspect `protocol_stats` and `baseline_status`; ID5 activation alone is not a
reliable-stream guarantee.

### Slow touch or stuttering

- Reduce `ema_alpha` for more responsive movement (trade-off: more jitter)
- Increase `blob_lift_frames` if fingers are lost prematurely
- Enable `debug_trace` only when collecting controller diagnostics; it does not
  report or set SPI speed.

### Touch not using full screen

Adjust `calib_scale_x/y` and `calib_offset_x/y`. The driver extracts
these from the HID report descriptor's logical/physical mapping by default.
Override if your display resolution or DPI scaling requires it.

## Known Issues

1. **Cold boot failure**: After 2+ consecutive rmmod cycles, the device
   stops responding to DESCREQ. Requires physical power cycle.

2. **No _RST support**: The ACPI `_RST` method calls `M010` which destroys
   the device. The driver never invokes it.

3. **4+ finger instability**: Without the Mahalanobis contact classifier
   and per-cycle gain adaptation (both unavailable without device firmware
   access), tracking 4+ simultaneous fingers has partial contact loss.
