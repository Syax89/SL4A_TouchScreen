# Build & Install

## Prerequisites

- Linux kernel 6.x+ with headers installed
- Surface Laptop 3 (AMD) or Surface Laptop 4 (AMD) — `AMDI0060` SPI
  controller with `MSHW0162` (SL3) or `MSHW0231` (SL4) touch controller
- `make`, `clang` or `gcc`, DKMS

## Unified Installer

All operations live in one tool: `tools/sl4a-touch.sh`
(subcommands: `install`, `uninstall`, `activate`, `status`, `logs`,
`rebuild`).

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/sl4a-touch.sh install
sudo reboot
```

The installer:
1. Checks hardware (ACPI `MSHW0231`/`MSHW0162` + `AMDI0060`, DMI product)
2. Stages the driver source via DKMS (`/usr/src/sl4a-touch-<version>/`)
3. Builds and signs `sl4a-spi-amd.ko` + `sl4a-spi-hid.ko`
4. Writes the profile to `/etc/modprobe.d/sl4a-spi-hid.conf`
5. Installs `sl4a-touch-activate.service` (auto-activates after every boot)

### Uninstall

```bash
sudo ./tools/sl4a-touch.sh uninstall
sudo reboot
```

### Activate / status / logs

```bash
sudo ./tools/sl4a-touch.sh activate   # bind modules now (no reboot)
sudo ./tools/sl4a-touch.sh status     # hardware + runtime state
sudo ./tools/sl4a-touch.sh logs       # full diagnostic bundle, one text file
```

The bundle is written to
`sl4a-touch-diagnostics-<date>-<time>.txt` (or the path given with `-o`) and
holds everything a problem report needs on its own: OS and checkout revision,
ACPI/DMI identity, DKMS and modprobe state, the activation service, loaded
modules, every module parameter, the driver's sysfs stats (`build_info`,
`ready`, `lifecycle_status`, `seq_state`, `protocol_stats`, `baseline_status`,
error counters), the **last captured frame's cell field** (one byte per cell,
3456 bytes on MSHW0231 / 4056 on MSHW0162) as hex, and the
last 300 driver dmesg lines — which include the per-blob lines when
`sl4a_debug_level=2` is set.

## Secure Boot

If Secure Boot is enabled the installer generates the DKMS MOK signing
key automatically (`dkms generate_mok`, or an openssl fallback that
writes a **DER**-encoded certificate — `mokutil` requires DER). An
existing PEM key is re-encoded in place. Enroll it with
`sudo mokutil --import /var/lib/dkms/mok.pub` and reboot once.

## Manual Build

```bash
cd driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD modules
```

## Module Parameters

Profile parameters live in `/etc/modprobe.d/sl4a-spi-hid.conf`
(standard profile: `raw_mode=N`).

| Parameter | Default | Description |
|-----------|---------|-------------|
| `raw_mode` | N | Enable raw heatmap + multi-touch mode |
| `sync_timeout_ms` | 6000 | Bounds every synchronous request (covers the ~3.6 s device settle) |
| `stream_watchdog_ms` | 0 | Runtime streaming watchdog (disabled by default; Windows uses 2000) |
| `stream_watchdog_max_retries` | 3 | Re-init retries before giving up |
| `skip_getfeat` | 0 | Skip the connect-time GET_FEATURE exchange |
| `getfeat_delay_ms` | 0 | Delay between RPT_DESC and GET_FEATURE |
| `ema_alpha` | 2 | Position-smoothing EMA coefficient (position only) |
| `blob_max_distance` | 3 | Hungarian association base radius (cells) |
| `blob_min_weight` | 1000 | Minimum blob signal weight |
| `blob_debounce` | 3 | New-touch debounce frames |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace period (0 = disabled) |
| `ghost_dist` | 6 | Pre-merge radius in cells |
| `pre_assoc_ratio` | 0 | Pre-association weight filter (0 = disabled) |
| `grid_cols` | 72 (SL4) / 78 (SL3) | Heatmap grid columns — per-device default by ACPI ID |
| `grid_rows` | 48 (SL4) / 52 (SL3) | Heatmap grid rows — per-device default by ACPI ID |
| `calib_scale_x` | 0 | X scale ×1000 (0 = auto from descriptor) |
| `calib_scale_y` | 0 | Y scale ×1000 (0 = auto) |
| `calib_offset_x` | 0 | X offset in screen pixels |
| `calib_offset_y` | 0 | Y offset in screen pixels |
| `invert_x` | 0 | Invert X axis |
| `invert_y` | 0 | Invert Y axis |
| `swap_xy` | 0 | Swap X and Y axes |

Device-specific defaults (set from the probe-selected config when the
parameter is left at 0):

| Device | ACPI ID | Grid | CapImg samples | Baseline frames | Baseline EMA alpha |
| --- | --- | --- | --- | --- | --- |
| Surface Laptop 4 AMD | `MSHW0231` | 72×48 | 3456 | 30 | 7 |
| Surface Laptop 3 AMD | `MSHW0162` | 78×52 | 4056 | 33 | 7 |

The baseline recovery alpha (7) is the Windows-documented 12.5% recovery
rate; it is separate from the `ema_alpha` position-smoothing parameter.

## Verification

```bash
# Check driver loaded and bound
lsmod | grep sl4a
sudo ./tools/sl4a-touch.sh status

# Check touch device created
ls /sys/class/hidraw/
sudo evtest  # select touch device, verify single-touch events

# Watch the probe log for the per-device config line
sudo dmesg | grep -i "device config"
```

## Troubleshooting

### No touch after cold boot

A HID client's connect-time feature GET_REPORT can take up to ~3.6 s
while the device settles. Feature-query timeouts are **non-fatal**:
`sync_timeout_ms` (default 6000) bounds every synchronous request and a
timeout leaves the input stream untouched. If recovery is triggered,
dmesg shows an ACPI `_PS3`→`_PS0` cycle followed by RESET_RSP, DESCREQ,
discovery and state `DONE`.

### Secure Boot rejects the modules

The installer auto-generates and re-encodes the MOK key as DER. Enroll
it with `sudo mokutil --import /var/lib/dkms/mok.pub`, reboot, confirm
the enrollment, then `sudo ./tools/sl4a-touch.sh activate`.

### No multi-touch, no contact

Check that `raw_mode=Y` is set for the raw profile. The device needs
SET_FEATURE ID5=01 to activate raw mode.

### Slow touch or stuttering

- Reduce `ema_alpha` for more responsive movement (trade-off: more jitter)
- Increase `blob_lift_frames` if fingers are lost prematurely
- Check SPI bus speed with `cat /sys/module/spi_amd/parameters/debug_trace`

### Touch not using full screen

Adjust `calib_scale_x/y` and `calib_offset_x/y`. The driver extracts
these from the HID report descriptor's logical/physical mapping by default.
Override if your display resolution or DPI scaling requires it.

## Known Issues

1. **No `_RST` support**: The ACPI `_RST` method calls `M010` which destroys
   the device. The driver never invokes it (recovery uses `_PS3`→`_PS0`).
2. **4+ finger instability**: Without the Mahalanobis contact classifier
   and per-cycle gain adaptation (both unavailable without device firmware
   access), tracking 4+ simultaneous fingers has partial contact loss.
3. **Raw mode is experimental**: may fail to activate after a cold boot;
   reboot and return to `raw_mode=0` after raw experiments.
