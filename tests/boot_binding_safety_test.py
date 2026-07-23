#!/usr/bin/env python3
"""Ensure neither experimental module can auto-load or bind at boot."""

from pathlib import Path


root = Path(__file__).parents[1]
kbuild = (root / "driver" / "Kbuild").read_text()
controller = (root / "driver" / "spi-amd.c").read_text()
hid = (root / "driver" / "spi-hid-core.c").read_text()
dkms = (root / "driver" / "dkms.conf").read_text()
activate = (root / "tools" / "activate-fch.sh").read_text()
install = (root / "tools" / "install.sh").read_text()
uninstall = (root / "tools" / "uninstall.sh").read_text()

assert "obj-m += sl4a-spi-amd.o" in kbuild
assert "obj-m += sl4a-spi-hid.o" in kbuild
assert 'BUILT_MODULE_NAME[0]="sl4a-spi-amd"' in dkms
assert 'BUILT_MODULE_NAME[1]="sl4a-spi-hid"' in dkms
assert "MODULE_DEVICE_TABLE(" not in controller
assert "MODULE_DEVICE_TABLE(" not in hid
assert '.name = "sl4a_spi_amd_v2_multi"' in controller
assert '.name\t= "sl4a_spi_hid"' in hid
assert 'modprobe "$CONTROLLER_MODULE"' in activate
assert 'modprobe "$HID_MODULE"' in activate
assert "wait_for_driver" in activate
assert "controller_platform" in activate
assert 'refusing to displace it' in activate
assert 'modprobe -r "$HID_MODULE" || true' in activate
assert 'modprobe -r "$CONTROLLER_MODULE" || true' in activate
assert 'MSHW0231 did not bind to the HID transport' in activate
assert "grep -q '^obj-m += sl4a-spi-amd.o$'" in install
assert "grep -q '^obj-m += sl4a-spi-hid.o$'" in install
assert "grep -q '^# SL4A_TouchScreen'" in uninstall
assert "grep -q '^PACKAGE_NAME=\"sl4a-touch\"'" in uninstall

for module in ("sl4a-spi-amd.ko", "sl4a-spi-hid.ko"):
    path = root / "driver" / module
    if path.exists():
        assert b"alias=" not in path.read_bytes(), f"{module} exports a module alias"

print("boot binding safety and no-alias contract: PASS")
