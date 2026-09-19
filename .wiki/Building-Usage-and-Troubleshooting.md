# Building, Usage and Troubleshooting

> **Beta software:** this is a beta, reverse-engineered kernel driver.
> Use it at your own risk. It is provided "as is", without warranty; the authors
> and contributors accept no responsibility for problems or damage from its use.

## Install or update

```bash
git clone https://github.com/Syax89/SL4A_TouchScreen.git
cd SL4A_TouchScreen
sudo ./tools/sl4a-touch.sh install
sudo reboot
```

The unified installer checks for `MSHW0231`/`MSHW0162` +
`AMDI0060`, builds `sl4a-spi-amd` and `sl4a-spi-hid` with DKMS for
every installed kernel with an available build tree, removes obsolete
`sl4a-touch` DKMS versions, and installs `sl4a-touch-activate.service`
for boot activation. It deliberately leaves active modules untouched:
replacing the AMD SPI controller while running can freeze the system.
Reboot is required after every install or update.

The installer chooses the compiler required by the running kernel. With
Secure Boot enabled it generates/re-encodes the DKMS MOK signing key
(DER-encoded, required by `mokutil`); enroll it once with
`sudo mokutil --import /var/lib/dkms/mok.pub` and reboot.

## Uninstall

```bash
sudo ./tools/sl4a-touch.sh uninstall
sudo reboot
```

The uninstaller removes the service and DKMS installation, but intentionally
leaves loaded modules active until reboot.

## Input devices

After a successful boot, the touchscreen normally appears as `spi 045E:0C19`
and the pen as `spi 045E:0C19 Stylus` under `/dev/input/eventN`. In raw mode the
driver publishes its own node named `MSHW0231 Touchscreen`.

## Development

Use a local build as a compile check:

```bash
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules
```

Use `LLVM=1` only for a clang-built kernel. To test modified driver code, run
`sudo ./tools/sl4a-touch.sh rebuild` (compile check) or
`sudo ./tools/sl4a-touch.sh install` from the checkout and reboot. Do not use
`rmmod`, `insmod`, or a systemd restart to replace `spi-amd` on a live system.

## Debugging and recovery

For a boot freeze or initialization failure, run
`sudo ./tools/sl4a-touch.sh logs` — it collects a full bundle
(hardware, DKMS, modprobe config, service state, module parameters, sysfs
stats, Secure Boot state, and driver-related dmesg lines).

Do not use direct ACPI GPIO calls as a recovery method. A full reboot is the
only supported recovery after a controller freeze or a raw-mode test.

## Common issues

- **No touchscreen after install/update:** reboot, then inspect `systemctl status sl4a-touch-activate`
  and `journalctl -b -k`.
- **Touchscreen dies after a cold boot:** a connect-time feature GET_REPORT
  can time out while the device settles (~3.6 s). Feature-query timeouts are
  non-fatal and `sync_timeout_ms` (default 6000) bounds every synchronous
  request; recovery uses an ACPI `_PS3`→`_PS0` power cycle.
- **Secure Boot rejects modules:** the installer generates a DER MOK key —
  enroll it with `sudo mokutil --import /var/lib/dkms/mok.pub`, reboot, then
  run `sudo ./tools/sl4a-touch.sh activate`.
- **`raw_mode=1` does not stream or leaves touch unusable:** this is the
  expected failure shape of the beta raw path; reboot and return to the
  default `raw_mode=0`.
- **Service appears failed immediately after a kernel update:** this can occur
  when headers for the running kernel were removed. The loaded driver can keep
  running; reboot into the updated kernel and verify `dkms status`.
