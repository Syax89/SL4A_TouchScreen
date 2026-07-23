#!/bin/bash
# Load the experimental FCH controller and HID transport only after login.
set -e -o pipefail

CONTROLLER_MODULE="sl4a-spi-amd"
CONTROLLER_DRIVER="sl4a_spi_amd_v2_multi"
HID_MODULE="sl4a-spi-hid"
HID_DRIVER="sl4a_spi_hid"
controller_loaded=0
hid_loaded=0

fail() { echo "FAIL: $1" >&2; exit 1; }

bound_driver() {
	basename "$(readlink -f "$1/driver")"
}

wait_for_driver() {
	local device="$1"
	local expected="$2"
	local attempt

	for attempt in {1..150}; do
		if [ -L "$device/driver" ] && [ "$(bound_driver "$device")" = "$expected" ]; then
			return 0
		fi
		sleep 0.1
	done

	return 1
}

rollback() {
	if [ "$hid_loaded" -eq 1 ]; then
		modprobe -r "$HID_MODULE" || true
	fi
	if [ "$controller_loaded" -eq 1 ]; then
		modprobe -r "$CONTROLLER_MODULE" || true
	fi
}

fail_rollback() {
	rollback
	fail "$1; modules loaded by this command were rolled back"
}

if [ "$EUID" -ne 0 ]; then
	exec sudo "$0" "$@"
fi

if command -v mokutil >/dev/null 2>&1; then
	if mokutil --sb-state 2>/dev/null | grep -qi 'SecureBoot enabled'; then
		if ! mokutil --test-key /var/lib/dkms/mok.pub >/dev/null 2>&1; then
			cat >&2 <<'EOF'
WARNING: Secure Boot is enabled but the DKMS MOK key is not enrolled.
Signed modules will be rejected by the kernel.
Enroll the key once:  sudo mokutil --import /var/lib/dkms/mok.pub
See docs/ROLLBACK.md for the complete MOK enrollment procedure.
After enrollment, reboot before running this script again.
EOF
			exit 1
		fi
	fi
fi

shopt -s nullglob
controllers=(/sys/bus/acpi/devices/AMDI0060:*)
[ "${#controllers[@]}" -eq 1 ] || fail "expected exactly one AMDI0060 ACPI device"
controller="${controllers[0]}"
controller_platform="/sys/bus/platform/devices/$(basename "$controller")"
[ -d "$controller_platform" ] || fail "AMDI0060 platform device is absent"
touches=(/sys/bus/acpi/devices/MSHW0231:*)
[ "${#touches[@]}" -eq 1 ] || fail "expected exactly one MSHW0231 ACPI device"
extra_mshw=(/sys/bus/acpi/devices/MSHW*:*)  # nullglob is on, matches only existing devices
extra_count=${#extra_mshw[@]}
if [ "$extra_count" -gt 1 ]; then
	extra_list=$(printf '%s\n' "${extra_mshw[@]}" | grep -v "MSHW0231" | xargs -r -n1 basename | tr '\n' ' ')
	if [ -n "$extra_list" ]; then
		fail "extra MSHW ACPI devices found beyond MSHW0231:${extra_list}activation refused"
	fi
fi
touch="${touches[0]}"

if [ -L "$controller_platform/driver" ]; then
	fail "AMDI0060 is already bound to $(bound_driver "$controller_platform"); refusing to displace it"
fi
if [ -L "$touch/physical_node/driver" ]; then
	fail "MSHW0231 is already bound to $(bound_driver "$touch/physical_node"); refusing to displace it"
fi

if [ ! -d "/sys/module/${CONTROLLER_MODULE//-/_}" ]; then
	controller_loaded=1
fi
if ! modprobe "$CONTROLLER_MODULE"; then
	fail_rollback "could not load experimental controller"
fi
if ! wait_for_driver "$controller_platform" "$CONTROLLER_DRIVER"; then
	fail_rollback "experimental controller did not bind"
fi

if [ ! -d "/sys/module/${HID_MODULE//-/_}" ]; then
	hid_loaded=1
fi
if ! modprobe "$HID_MODULE"; then
	fail_rollback "could not load HID transport"
fi
if ! wait_for_driver "$touch/physical_node" "$HID_DRIVER"; then
	fail_rollback "MSHW0231 did not bind to the HID transport"
fi

echo "AMDI0060 and MSHW0231 are bound. Recovery: sudo modprobe -r sl4a-spi-hid sl4a-spi-amd; reboot."
