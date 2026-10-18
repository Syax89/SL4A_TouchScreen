#!/usr/bin/env bash
# Record a bounded, read-only evtest trace from the SL4A heatmap MT bridge.

set -u
set -o pipefail

usage() {
	cat <<'EOF'
Usage: tools/hardware_evidence/capture_beta_multitouch.sh --duration SECONDS --output FILE

Discovers the SL4A heatmap-backed multitouch bridge by its driver-owned input
identity ("MSHW0231 Touchscreen") and multitouch capabilities, then records a
bounded evtest trace. FILE must not already exist. The helper never escalates
privileges or installs, loads, unloads, or configures drivers.
EOF
}

duration=""
output=""
while [ "$#" -gt 0 ]; do
	case "$1" in
		--duration)
			[ "$#" -ge 2 ] || { usage >&2; exit 2; }
			duration="$2"
			shift 2
			;;
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

if ! [[ "$duration" =~ ^[1-9][0-9]*$ ]] || [ -z "$output" ]; then
	usage >&2
	exit 2
fi
if [ -e "$output" ]; then
	printf 'Refusing to overwrite existing path: %s\n' "$output" >&2
	exit 1
fi
output_dir="$(dirname "$output")"
if [ ! -d "$output_dir" ]; then
	printf 'Output directory does not exist: %s\n' "$output_dir" >&2
	exit 1
fi
if ! command -v evtest >/dev/null 2>&1; then
	printf 'evtest is unavailable; no capture was created.\n' >&2
	exit 1
fi
if ! command -v timeout >/dev/null 2>&1; then
	printf 'timeout is unavailable; no capture was created.\n' >&2
	exit 1
fi

capability_bit_set() {
	local path="$1" bit="$2" index word value
	local -a words
	[ -r "$path" ] || return 1
	read -r -a words < "$path" || return 1
	index=$((bit / 64))
	[ "${#words[@]}" -gt "$index" ] || return 1
	word="${words[${#words[@]} - 1 - index]}"
	value=$((16#$word))
	(( value & (1 << (bit % 64)) ))
}

matches=()
for input in /sys/class/input/input*; do
	[ -d "$input" ] || continue
	[ -r "$input/name" ] || continue
	IFS= read -r name < "$input/name" || continue
	[ "$name" = "MSHW0231 Touchscreen" ] || continue
	capability_bit_set "$input/capabilities/ev" 3 || continue       # EV_ABS
	capability_bit_set "$input/capabilities/key" 330 || continue    # BTN_TOUCH
	capability_bit_set "$input/capabilities/abs" 57 || continue     # ABS_MT_TRACKING_ID
	for event in "$input"/event*; do
		[ -e "$event" ] || continue
		matches+=("$event")
	done
done

if [ "${#matches[@]}" -ne 1 ]; then
	printf 'Expected one SL4A beta multitouch bridge event node; found %s.\n' "${#matches[@]}" >&2
	printf 'No capture was created. Inspect /sys/class/input and the evidence collector output.\n' >&2
	exit 1
fi

event_sysfs="${matches[0]}"
device="/dev/input/$(basename "$event_sysfs")"
input="$(dirname "$event_sysfs")"
device_path="$(readlink -f "$input/device")"
timestamp="$(date -u +%Y-%m-%dT%H:%M:%SZ)"

umask 077
{
	printf '# SL4A beta heatmap multitouch evtest capture\n'
	printf 'captured_at_utc=%s\n' "$timestamp"
	printf 'requested_duration_seconds=%s\n' "$duration"
	printf 'event_sysfs=%s\n' "$event_sysfs"
	printf 'device=%s\n' "$device"
	printf 'input_name='
	cat "$input/name"
	printf 'input_device_sysfs=%s\n' "$device_path"
	printf 'input_uevent:\n'
	cat "$input/uevent" 2>&1 || true
	printf 'capabilities_ev='
	cat "$input/capabilities/ev"
	printf 'capabilities_key='
	cat "$input/capabilities/key"
	printf 'capabilities_abs='
	cat "$input/capabilities/abs"
	printf 'properties='
	cat "$input/properties"
	printf '\n# Raw evtest output follows\n'
	timeout --signal=INT "$duration" evtest "$device"
	status=$?
	printf '\n# evtest_exit_status=%s\n' "$status"
} > "$output" 2>&1

if [ "$status" -ne 0 ] && [ "$status" -ne 124 ]; then
	printf 'evtest failed (status %s); provenance and output were retained in %s\n' "$status" "$output" >&2
	exit "$status"
fi

if [ -n "${SUDO_USER:-}" ]; then
	chown "$SUDO_USER" "$output"
fi

printf 'Capture written to %s\n' "$output"
