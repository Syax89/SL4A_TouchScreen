#!/bin/sh
# WARNING: This script may cause system freeze. Use only with cold reboot.
# See SESSION_HANDOFF.md safety rules.
# Raw mode is armed for one boot only. Remove its marker before loading either
# module so a frozen raw boot falls back to standard HID after a forced reboot.
set -eu

marker=/var/lib/sl4a-touch/raw-mode-once
raw_mode=0

if [ -e "$marker" ]; then
	if ! rm -f "$marker"; then
		echo "sl4a-touch: cannot clear raw-mode marker" >&2
		exit 1
	fi
	raw_mode=1
	logger -t sl4a-touch "raw mode armed for this boot only"
fi

/sbin/modprobe spi-amd
if [ "$raw_mode" -eq 1 ]; then
	exec /sbin/modprobe spi-hid raw_mode=1 debug_level=1
fi
exec /sbin/modprobe spi-hid
