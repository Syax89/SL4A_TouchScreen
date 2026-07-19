# Build & Install

## Prerequisites

- Linux kernel 6.x+ with headers installed
- AMD Cezanne (Ryzen 4000/5000 Mobile) platform
- `make`, `clang` or `gcc`, DKMS

## DKMS Installation (Recommended)

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
sudo reboot
```

The installer:
1. Copies the driver source to `/usr/src/sl4a-touch-1.1.0/`
2. Registers with DKMS
3. Builds and signs the kernel module
4. Creates `/etc/modprobe.d/spi-hid.conf`
5. Installs a udev rule for auto-loading

After reboot, the driver loads automatically when the MSHW0231 ACPI
device is enumerated.

### Uninstall

```bash
sudo ./tools/uninstall.sh
```

## Manual Build

```bash
cd driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD modules

# Manual install
sudo cp spi-hid.ko /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo modprobe spi-hid
```

## Module Parameters

Load-time parameters in `/etc/modprobe.d/spi-hid.conf`:

```
options spi_hid raw_mode=Y skip_getfeat=Y
```

| Parameter | Default | Description |
|-----------|---------|-------------|
| `raw_mode` | Y | Enable raw heatmap + multi-touch mode |
| `skip_getfeat` | Y | Internal: skip redundant GET_FEATURE handshake |
| `ema_alpha` | 7 | EMA smoothing coefficient (1-10) |
| `blob_max_distance` | 3 | Hungarian association base radius (cells) |
| `blob_min_weight` | 1000 | Minimum blob signal weight |
| `blob_debounce` | 3 | New-touch debounce frames |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace period (0 = disabled) |
| `ghost_dist` | 6 | Pre-merge radius in cells |
| `pre_assoc_ratio` | 0 | Pre-association weight filter (0 = disabled) |
| `grid_cols` | 72 | Columns in heatmap grid |
| `grid_rows` | 48 | Rows in heatmap grid |
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

# Check touch device created
ls /sys/class/hidraw/
sudo evtest  # select touch device, verify single-touch events

# Check raw multi-touch active
cat /sys/module/spi_hid/parameters/raw_enabled  # should be 1

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

The driver retries the DESCREQ handshake 3 times with 1-second delays.

### No multi-touch, no contact

Check that `skip_getfeat=Y` is set. The device needs vendor-init
(register 0xC2) and SET_FEATURE ID5=01 to activate raw mode.

### Slow touch or stuttering

- Reduce `ema_alpha` for more responsive movement (trade-off: more jitter)
- Increase `blob_lift_frames` if fingers are lost prematurely
- Check SPI bus speed with `cat /sys/module/spi_amd/parameters/debug_trace`

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
