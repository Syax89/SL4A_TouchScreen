# Quickstart

Get SL4A TouchScreen running on your Surface Laptop 4 (AMD) in 5 steps.

## Prerequisites

- Linux kernel **6.x+**
- Kernel headers installed (`linux-headers` or `kernel-devel` matching `uname -r`)
- **DKMS** (`dkms` package)
- **Git**

## 1. Install

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/install.sh
```

## 2. Secure Boot

If Secure Boot is enabled, enroll the DKMS MOK key before the module will
load:

```bash
sudo mokutil --import /var/lib/dkms/mok.pub
```

Set a one-time password, reboot, and select **Enroll MOK** in the MOK
Manager interface. See [`docs/ROLLBACK.md`](ROLLBACK.md) for the full procedure.

## 3. Activate

Only after login, with a local console or remote shell available:

```bash
sudo ./tools/activate-fch.sh
```

The script binds the SL4A controller and HID transport. Keep shell access
available in case recovery is needed.

## 4. Verify

```bash
ls /sys/bus/spi/devices/spi*
```

Look for a device node containing `spi 045E:0C19`. Confirm the driver is
loaded:

```bash
cat /sys/bus/spi/devices/spi*/modalias | grep 0C19
```

## 5. Recovery

If the touchscreen becomes unresponsive after activation:

```bash
sudo modprobe -r sl4a-spi-hid sl4a-spi-amd
sudo reboot
```

The system boots with the in-tree `spi-amd` driver and no SL4A module loaded.
Force-reboot if the unload fails.

## What to Expect

- **Single-touch only** in standard HID mode (the default). Multi-touch
  requires the experimental raw mode (`raw_mode=1`).
- Device node appears as a **SPI device with ID `045E:0C19`** under
  `/sys/bus/spi/devices/`.
- A **stylus/pen input node is published** but remains untested — pen
  input behavior has not been qualified.
- **No multi-touch, no palm rejection, no pen qualification** in the
  standard profile. Raw mode is experimental and not validated.
