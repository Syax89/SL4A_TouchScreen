# Quickstart

Get SL4A TouchScreen running on your Surface Laptop 3/4 (AMD) in 5 steps.

## Prerequisites

- Linux kernel **6.x+**
- Kernel headers installed (`linux-headers` or `kernel-devel` matching `uname -r`)
- **DKMS** (`dkms` package)
- **Git**

## 1. Install

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/sl4a-touch.sh install
```

Prompts for a profile (standard HID, the supported default, or the
experimental raw multitouch profile) unless `--standard`/`--raw` is given.

## 2. Secure Boot

The installer handles everything automatically — if Secure Boot is enabled,
it generates the DKMS signing key and walks you through enrolling it. You just
need a temporary password (used once at the next boot).

When you enroll by hand, use the certificate the installer actually resolved —
the path it prints as `DKMS MOK certificate path:`. That is **not** always
`/var/lib/dkms/mok.pub`: Ubuntu-family DKMS signs with the shim-signed
certificate, and a `mok_certificate` set in `/etc/dkms/framework.conf` (or in a
`framework.conf.d/*.conf` drop-in) moves it again.

If you skipped the enrollment prompt during installation:
```bash
# Replace this with the absolute path install printed after
# "DKMS MOK certificate path:" — do not substitute a guessed path.
MOK_CERT=/absolute/path/printed/by/installer
sudo mokutil --import "$MOK_CERT"
sudo reboot
```

Enrolling a certificate that is not the one DKMS signs with leaves the kernel
rejecting the modules, so never copy `/var/lib/dkms/mok.pub` out of habit.
If `install` instead refused with `DKMS configuration sets only one of
mok_signing_key/mok_certificate`, fix the DKMS configuration itself: set **both**
options in `/etc/dkms/framework.conf` (or a drop-in), or remove both to keep the
distro default pair, then re-run `install`. `--rotate-mok` does not help there —
it only replaces the material at the resolved paths.

At the blue MOK Manager screen: **Enroll MOK → Continue → Yes → enter password → Reboot**.
After login the driver activates automatically.

## 3. Activate

Only after login, with a local console or remote shell available:

```bash
sudo ./tools/sl4a-touch.sh activate
```

The script binds the SL4A controller and HID transport. Keep shell access
available in case recovery is needed.

## 4. Verify

```bash
./tools/sl4a-touch.sh status
```

Shows the installed version, active profile, and whether the driver is
currently loaded and bound — no root required. Or check manually:

```bash
ls /sys/bus/spi/devices/          # the controller enumerates as spi-MSHW0231:00 (or spi-MSHW0162:00)
cat /sys/class/input/input*/name | sort -u    # the touch input node is named "spi 045E:0C19"
```

Look for the input node named `spi 045E:0C19` (raw mode names it
`MSHW0231 Touchscreen`). If something looks wrong,
`sudo ./tools/sl4a-touch.sh logs` collects a diagnostic bundle to attach to a
bug report.

## 5. Recovery

If the touchscreen becomes unresponsive after activation:

```bash
sudo modprobe -r sl4a-spi-hid sl4a-spi-amd
sudo reboot
```

The system boots with the in-tree `spi-amd` driver and no SL4A module loaded.
Force-reboot if the unload fails.

## Qualifying your machine (Italian guide)

To produce one coherent, repeatable hardware-qualification bundle instead of
ad-hoc captures, use the guided runner `tools/qualify-touch.py`. It is
read-only, needs no new install step, and writes its artifacts outside the
repository:

```bash
./tools/qualify-touch.py --phase initial \
  --output ~/sl4a-qual/$(date -u +%Y%m%dT%H%M%SZ)-initial
```

It re-checks the recorded build evidence against this exact clean checkout
before it lets a run be called qualified: the installer's stamp, module
`vermagic` and `srcversion`. Those are consistency checks — they show the
stamp, the module on disk and the loaded module agree, not which source the
running code was compiled from. Then it snapshots config/service/kernel state
and drives one bounded, guided gesture capture through the existing evidence
helpers. Run it as the owner of the checkout: under `sudo` git refuses the
user-owned tree ("dubious ownership") and the `repo-clean` gate fails on
purpose. The step-by-step guide, including the opt-in `--bridge` variant, the
`initial|coldboot|resume` declarations, the exact-clone `safe.directory`
exception, the rollback order and the exit-code table, is in Italian:
[QUALIFICAZIONE-TOUCH.md](QUALIFICAZIONE-TOUCH.md).

## What to Expect

- **Single-touch only** in standard HID mode (the default). Multi-touch
  requires the experimental raw mode (`raw_mode=1`).
- The touch input node is named **`spi 045E:0C19`** (raw mode: `MSHW0231 Touchscreen`);
  the controller itself is the SPI device `spi-MSHW0231:00` (SL3: `spi-MSHW0162:00`).
- A **stylus/pen input node is published** but remains untested — pen
  input behavior has not been qualified.
- **No multi-touch, no palm rejection, no pen qualification** in the
  standard profile. Raw mode is experimental and not validated.
