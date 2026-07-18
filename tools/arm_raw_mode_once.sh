#!/bin/sh
# Arm one instrumented raw-mode boot. The boot loader clears this marker before
# loading the modules, so the following boot always falls back to standard HID.
set -eu

marker=/var/lib/sl4a-touch/raw-mode-once

if [ "$(id -u)" -ne 0 ]; then
	exec sudo "$0" "$@"
fi

install -d -m 0755 /var/lib/sl4a-touch
touch "$marker"
echo "Raw mode is armed for one boot. Reboot now; do not reload modules live."
