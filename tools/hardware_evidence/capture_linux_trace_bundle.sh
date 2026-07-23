#!/usr/bin/env bash
# Collect a bounded, read-only Linux session for SL4A hardware validation.

set -u
set -o pipefail

usage() {
	cat <<'EOF'
Usage: tools/hardware_evidence/capture_linux_trace_bundle.sh [OPTIONS]

Create a new, timestamped Linux trace bundle. All host and sysfs discovery is
read-only. This helper never invokes sudo or loads, unloads, binds, or configures
drivers.

Options:
  --output DIRECTORY       New bundle directory (default: sl4a-linux-trace-<UTC>)
  --duration SECONDS       Session duration, from 1 through 3600 (default: 20)
   --capture-direct-touch   Capture an optional bounded direct-touch evtest trace
   --capture-stylus         Capture an optional bounded stylus evtest trace
  --help                   Show this help
EOF
}

output=""
duration=20
capture_direct_touch=0
capture_stylus=0
while [ "$#" -gt 0 ]; do
	case "$1" in
		--output)
			[ "$#" -ge 2 ] || { usage >&2; exit 2; }
			output="$2"
			shift 2
			;;
		--duration)
			[ "$#" -ge 2 ] || { usage >&2; exit 2; }
			duration="$2"
			shift 2
			;;
		--capture-direct-touch)
			capture_direct_touch=1
			shift
			;;
		--capture-stylus)
			capture_stylus=1
			shift
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

if ! [[ "$duration" =~ ^[1-9][0-9]*$ ]] || [ "$duration" -gt 3600 ]; then
	printf 'Duration must be an integer from 1 through 3600 seconds.\n' >&2
	exit 2
fi

start_utc="$(date -u +%Y-%m-%dT%H:%M:%SZ)"
if [ -z "$output" ]; then
	output="sl4a-linux-trace-$(date -u +%Y%m%dT%H%M%SZ)"
fi
if [ -e "$output" ]; then
	printf 'Refusing to overwrite existing path: %s\n' "$output" >&2
	exit 1
fi

umask 077
mkdir "$output" || exit 1
mkdir "$output/provenance" "$output/sysfs" "$output/input" "$output/kernel"
manifest="$output/manifest.txt"
tmp_journal="$(mktemp "${TMPDIR:-/tmp}/sl4a-journal.XXXXXX")" || exit 1
trap 'rm -f "$tmp_journal"' EXIT

capture_command() {
	local destination="$1"
	shift
	{
		printf '$'
		printf ' %q' "$@"
		printf '\n\n'
		"$@"
		printf '\n[exit status: %s]\n' "$?"
	} > "$destination" 2>&1
}

capture_file() {
	local source="$1" destination="$2"
	{
		printf '# source: %s\n' "$source"
		if [ -r "$source" ]; then
			cat "$source"
		else
			printf '[unreadable or absent]\n'
		fi
	} > "$destination"
}

has_touch_identity() {
	local node="$1" line
	while [ "$node" != /sys ] && [ "$node" != / ]; do
		if [ -r "$node/uevent" ]; then
			while IFS= read -r line; do
				[[ "$line" =~ ^HID_ID=.*:0000045[Ee]:00000[Cc]19$ ]] && return 0
			done < "$node/uevent"
		fi
		node="$(dirname "$node")"
	done
	return 1
}

printf 'format_version=1\n' > "$manifest"
printf 'bundle_type=sl4a-linux-trace\n' >> "$manifest"
printf 'session_start_utc=%s\n' "$start_utc" >> "$manifest"
printf 'requested_duration_seconds=%s\n' "$duration" >> "$manifest"
printf 'direct_touch_capture_requested=%s\n' "$capture_direct_touch" >> "$manifest"
printf 'stylus_capture_requested=%s\n' "$capture_stylus" >> "$manifest"
printf 'collector=%s\n' "${BASH_SOURCE[0]}" >> "$manifest"
printf 'collector_uid=%s\n' "$(id -u)" >> "$manifest"
printf 'safety=read-only; no sudo; no driver load/unload/bind/configure\n' >> "$manifest"

capture_command "$output/provenance/uname.txt" uname -a
capture_command "$output/provenance/os-release.txt" cat /etc/os-release
capture_command "$output/provenance/kernel-command-line.txt" cat /proc/cmdline
capture_command "$output/provenance/loaded-modules.txt" sh -c 'grep -Ei "sl4a|spi_hid|hid_spi" /proc/modules || true'
for dmi_file in product_name product_version sys_vendor bios_version bios_date; do
	capture_file "/sys/class/dmi/id/$dmi_file" "$output/provenance/dmi-$dmi_file.txt"
done

{
	printf '# Dynamically discovered ACPI nodes for the touchscreen controller and child\n'
	for id in AMDI0060 MSHW0231; do
		printf '\n[%s]\n' "$id"
		matches=0
		for node in /sys/bus/acpi/devices/"$id":*; do
			[ -d "$node" ] || continue
			matches=1
			printf 'path=%s\n' "$node"
			for attribute in hid modalias status path uevent; do
				[ -e "$node/$attribute" ] || continue
				printf '%s:\n' "$attribute"
				sed 's/^/  /' "$node/$attribute" 2>/dev/null || printf '  [unreadable]\n'
			done
		done
		[ "$matches" -eq 1 ] || printf '[not found]\n'
	done
} > "$output/sysfs/acpi-devices.txt"

{
	printf '# Dynamically discovered SPI nodes relevant to SL4A/HID touch\n'
	matches=0
	for node in /sys/bus/spi/devices/*; do
		[ -d "$node" ] || continue
		identity="$(cat "$node/modalias" "$node/uevent" 2>/dev/null || true)"
		driver="$(readlink -f "$node/driver" 2>/dev/null || true)"
		if ! [[ "$identity $driver" =~ [Ss][Ll]4[Aa]|[Mm][Ss][Hh][Ww]0231|spi-hid|spi_hid ]]; then
			continue
		fi
		matches=1
		printf '\npath=%s\ndriver=%s\n' "$node" "${driver:-[unbound]}"
		for attribute in modalias uevent ready protocol_stats baseline_status lifecycle_status; do
			[ -e "$node/$attribute" ] || continue
			printf '%s:\n' "$attribute"
			sed 's/^/  /' "$node/$attribute" 2>/dev/null || printf '  [unreadable]\n'
		done
	done
	[ "$matches" -eq 1 ] || printf '[no matching SPI node found]\n'
} > "$output/sysfs/spi-devices.txt"

{
	printf '# Dynamically discovered SL4A HID input candidates\n'
	matches=0
	for input in /sys/class/input/input*; do
		[ -d "$input" ] || continue
		name="$(cat "$input/name" 2>/dev/null || true)"
		device="$(readlink -f "$input/device" 2>/dev/null || true)"
		if ! has_touch_identity "$device" && ! [[ "$name" =~ MSHW0231|045[Ee]:0[Cc]19 ]]; then
			continue
		fi
		matches=1
		printf '\ninput_sysfs=%s\ninput_name=%s\ninput_device_sysfs=%s\n' "$input" "$name" "$device"
		for attribute in uevent properties capabilities/ev capabilities/key; do
			[ -e "$input/$attribute" ] || continue
			printf '%s:\n' "$attribute"
			sed 's/^/  /' "$input/$attribute" 2>/dev/null || printf '  [unreadable]\n'
		done
		for event in "$input"/event*; do
			[ -e "$event" ] || continue
			printf 'event_sysfs=%s\ndevnode=/dev/input/%s\n' "$event" "$(basename "$event")"
		done
	done
	[ "$matches" -eq 1 ] || printf '[no matching input node found]\n'
} > "$output/input/devices.txt"

capture_command "$output/input/tool-availability.txt" sh -c '
for tool in evtest timeout; do
    command -v "$tool" >/dev/null 2>&1 && printf "%s=%s\n" "$tool" "$(command -v "$tool")" || printf "%s=unavailable\n" "$tool"
done'

if [ "$capture_direct_touch" -eq 1 ]; then
	"$(dirname "${BASH_SOURCE[0]}")/capture_direct_touch.sh" --duration "$duration" \
		--output "$output/input/direct-touch.evtest.txt" &
	evtest_pid=$!
	evtest_status="running"
else
	evtest_pid=""
	evtest_status="not-requested"
fi
if [ "$capture_stylus" -eq 1 ]; then
	"$(dirname "${BASH_SOURCE[0]}")/capture_stylus.sh" --duration "$duration" \
		--output "$output/input/stylus.evtest.txt" &
	stylus_pid=$!
	stylus_status="running"
else
	stylus_pid=""
	stylus_status="not-requested"
fi

# Preserve a defined session even when no input trace was requested.
sleep "$duration"
if [ -n "$evtest_pid" ]; then
	wait "$evtest_pid"
	evtest_status=$?
fi
if [ -n "$stylus_pid" ]; then
	wait "$stylus_pid"
	stylus_status=$?
fi

end_utc="$(date -u +%Y-%m-%dT%H:%M:%SZ)"
if command -v journalctl >/dev/null 2>&1; then
	journalctl --no-pager --output=short-iso-precise --since "$start_utc" --until "$end_utc" \
		--grep='[Ss][Ll]4[Aa]|[Aa][Mm][Dd][Ii]0060|[Mm][Ss][Hh][Ww]0231|[Ss][Pp][Ii][-_ ]?[Hh][Ii][Dd]|[Hh][Ii][Dd][-_ ]?[Ss][Pp][Ii]|[Ss]urface.*([Tt]ouch|[Dd]igitizer)|[Tt]ouch' \
		--lines=2000 > "$tmp_journal" 2>&1
	journal_status=$?
else
	printf 'journalctl unavailable\n' > "$tmp_journal"
	journal_status=127
fi
{
	printf '# journal window: %s through %s\n' "$start_utc" "$end_utc"
	printf '# filter: SL4A, AMDI0060, MSHW0231, SPI/HID, Surface touch/digitizer\n'
	if [ "$journal_status" -eq 0 ]; then
		cat "$tmp_journal"
	else
		printf '# journalctl exit status: %s\n' "$journal_status"
		cat "$tmp_journal"
	fi
} > "$output/kernel/journal-sl4a-session.txt"

printf 'session_end_utc=%s\n' "$end_utc" >> "$manifest"
printf 'journal_exit_status=%s\n' "$journal_status" >> "$manifest"
printf 'direct_touch_capture_exit_status=%s\n' "$evtest_status" >> "$manifest"
printf 'stylus_capture_exit_status=%s\n' "$stylus_status" >> "$manifest"
printf 'artifacts_sha256=\n' >> "$manifest"
while IFS= read -r artifact; do
	[ "$artifact" = "manifest.txt" ] && continue
	(cd "$output" && sha256sum "$artifact")
done < <(cd "$output" && find . -type f -printf '%P\n' | sort) >> "$manifest"

# A caller using sudo still needs to inspect and archive the complete bundle.
if [ -n "${SUDO_USER:-}" ]; then
	chown -R "$SUDO_USER" "$output"
fi

printf 'Linux trace bundle collected in %s\n' "$output"
