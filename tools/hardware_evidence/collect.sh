#!/usr/bin/env bash
# Collect read-only host evidence for the Surface Laptop 4 AMD touchscreen.

set -u
set -o pipefail

usage() {
	cat <<'EOF'
Usage: tools/hardware_evidence/collect.sh [--output DIRECTORY]

Creates a new evidence directory. The harness only reads host state; it never
installs, loads, unloads, or configures drivers. It does not invoke sudo.
EOF
}

output=""
while [ "$#" -gt 0 ]; do
	case "$1" in
		--output)
			[ "$#" -ge 2 ] || { usage >&2; exit 2; }
			output="$2"
			shift 2
			;;
		--help|-h)
			usage
			exit 0
			;;
		*)
			usage >&2
			exit 2
			;;
	esac
done

timestamp="$(date -u +%Y%m%dT%H%M%SZ)"
if [ -z "$output" ]; then
	output="sl4a-hardware-evidence-$timestamp"
fi

if [ -e "$output" ]; then
	printf 'Refusing to overwrite existing path: %s\n' "$output" >&2
	exit 1
fi

umask 077
mkdir -p "$output" || exit 1
mkdir -p "$output/provenance" "$output/modules" "$output/sysfs" "$output/input" "$output/kernel"

manifest="$output/manifest.txt"
printf 'collected_at_utc=%s\n' "$timestamp" > "$manifest"
printf 'collector=%s\n' "${BASH_SOURCE[0]}" >> "$manifest"
printf 'uid=%s\n' "$(id -u)" >> "$manifest"

capture_command() {
	local destination="$1"
	shift
	{
		printf '$'
		printf ' %q' "$@"
		printf '\n\n'
		"$@"
		local status=$?
		printf '\n[exit status: %s]\n' "$status"
		return 0
	} > "$destination" 2>&1
}

capture_file() {
	local source="$1"
	local destination="$2"
	{
		printf '# source: %s\n' "$source"
		if [ -r "$source" ]; then
			cat "$source"
		else
			printf '[unreadable or absent]\n'
		fi
	} > "$destination"
}

capture_command "$output/provenance/uname.txt" uname -a
capture_command "$output/provenance/os-release.txt" cat /etc/os-release
capture_command "$output/provenance/kernel-command-line.txt" cat /proc/cmdline
capture_command "$output/provenance/kernel-build.txt" cat /proc/version
capture_command "$output/provenance/secure-boot.txt" sh -c 'if command -v mokutil >/dev/null 2>&1; then mokutil --sb-state; else printf "mokutil unavailable\n"; fi'

for dmi_file in product_name product_version product_sku sys_vendor bios_version bios_date board_name; do
	capture_file "/sys/class/dmi/id/$dmi_file" "$output/provenance/dmi-$dmi_file.txt"
done

capture_command "$output/modules/dkms-status.txt" sh -c 'if command -v dkms >/dev/null 2>&1; then dkms status; else printf "dkms unavailable\n"; fi'
capture_command "$output/modules/loaded-modules.txt" cat /proc/modules

for module in sl4a_spi_amd sl4a_spi_hid; do
	capture_command "$output/modules/$module-modinfo.txt" sh -c 'if command -v modinfo >/dev/null 2>&1; then modinfo "$1"; else printf "modinfo unavailable\n"; fi' sh "$module"
	dir="$output/modules/$module-parameters"
	mkdir -p "$dir"
	if [ -d "/sys/module/$module/parameters" ]; then
		for parameter in /sys/module/"$module"/parameters/*; do
			[ -e "$parameter" ] || continue
			capture_file "$parameter" "$dir/$(basename "$parameter").txt"
		done
	else
		printf '[module parameters directory absent]\n' > "$dir/status.txt"
	fi
done

acpi_inventory="$output/sysfs/acpi-devices.txt"
{
	printf '# Discovered ACPI nodes matching the driver hardware IDs\n'
	for id in AMDI0060 MSHW0231; do
		printf '\n[%s]\n' "$id"
		matches=0
		for node in /sys/bus/acpi/devices/"$id":*; do
			[ -d "$node" ] || continue
			matches=1
			printf '%s\n' "$node"
			for attribute in hid modalias status path; do
				[ -e "$node/$attribute" ] || continue
				printf '  %s=' "$attribute"
				tr '\n' ' ' < "$node/$attribute" 2>/dev/null || printf '[unreadable]'
				printf '\n'
			done
		done
		[ "$matches" -eq 1 ] || printf '[not found]\n'
	done
} > "$acpi_inventory"

spi_inventory="$output/sysfs/spi-devices.txt"
{
	printf '# SPI devices and selected driver state attributes\n'
	for node in /sys/bus/spi/devices/*; do
		[ -d "$node" ] || continue
		printf '\n[%s]\n' "$node"
		for attribute in modalias uevent ready protocol_stats baseline_status lifecycle_status; do
			[ -e "$node/$attribute" ] || continue
			printf '  %s:\n' "$attribute"
			if [ -r "$node/$attribute" ]; then
				sed 's/^/    /' "$node/$attribute" 2>/dev/null || printf '    [unreadable]\n'
			else
				printf '    [unreadable]\n'
			fi
		done
	done
} > "$spi_inventory"

input_inventory="$output/input/devices.txt"
{
	printf '# Input nodes discovered from sysfs; /dev paths are derived, not assumed\n'
	for node in /sys/class/input/input*; do
		[ -d "$node" ] || continue
		name="$(cat "$node/name" 2>/dev/null || printf '[unreadable]')"
		printf '\n[%s]\nname=%s\n' "$node" "$name"
		for attribute in uevent properties modalias id/vendor id/product id/version; do
			[ -e "$node/$attribute" ] || continue
			printf '%s:\n' "$attribute"
			sed 's/^/  /' "$node/$attribute" 2>/dev/null || printf '  [unreadable]\n'
		done
		for event in "$node"/event*; do
			[ -e "$event" ] || continue
			printf 'event_sysfs=%s\ndevnode=/dev/input/%s\n' "$event" "$(basename "$event")"
		done
	done
} > "$input_inventory"

capture_command "$output/input/tool-availability.txt" sh -c '
for tool in libinput evtest; do
    if command -v "$tool" >/dev/null 2>&1; then
        printf "%s=%s\n" "$tool" "$(command -v "$tool")"
        "$tool" --version 2>&1 || true
    else
        printf "%s=unavailable\n" "$tool"
    fi
done'

# A bounded recent slice is useful even when dmesg access is restricted; any
# permission error is retained in the artifact instead of being hidden.
capture_command "$output/kernel/dmesg-last-300.txt" sh -c 'dmesg --color=never 2>&1 | tail -n 300'

{
	printf 'artifact_root=%s\n' "$output"
	printf 'files=\n'
	find "$output" -type f -printf '%P\n' | sort
} >> "$manifest"

printf 'Evidence collected in %s\n' "$output"
