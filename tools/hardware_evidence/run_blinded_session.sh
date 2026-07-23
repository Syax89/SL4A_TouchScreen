#!/usr/bin/env bash
# Orchestrate a bounded, read-only evidence session without profile knowledge.

set -u
set -o pipefail

usage() {
	cat <<'EOF'
Usage: tools/hardware_evidence/run_blinded_session.sh --period p1|p2|p3 --case CASE \
        --duration SECONDS --output NEW_DIRECTORY [--capture-direct-touch] [--capture-stylus]

Create one blinded hardware-session evidence directory. PERIOD is an opaque
label; this runner never accepts, selects, installs, loads, unloads, or records
a profile or profile mapping. CASE is an opaque identifier using letters,
numbers, dots, underscores, or hyphens. DURATION must be 1 through 3600 seconds.

The runner invokes only the read-only collection and Linux trace helpers. It
never invokes sudo. A caller may invoke this script with the access they need.
EOF
}

period=""
case_name=""
duration=""
output=""
capture_direct_touch=0
capture_stylus=0
while [ "$#" -gt 0 ]; do
	case "$1" in
		--period)
			[ "$#" -ge 2 ] || { usage >&2; exit 2; }
			period="$2"
			shift 2
			;;
		--case)
			[ "$#" -ge 2 ] || { usage >&2; exit 2; }
			case_name="$2"
			shift 2
			;;
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

if [ "$period" != "p1" ] && [ "$period" != "p2" ] && [ "$period" != "p3" ]; then
	printf 'Period must be one of: p1, p2, p3.\n' >&2
	exit 2
fi
if ! [[ "$case_name" =~ ^[A-Za-z0-9][A-Za-z0-9._-]*$ ]]; then
	printf 'Case must be a non-empty opaque identifier using letters, numbers, dots, underscores, or hyphens.\n' >&2
	exit 2
fi
if ! [[ "$duration" =~ ^[1-9][0-9]*$ ]] || [ "$duration" -gt 3600 ]; then
	printf 'Duration must be an integer from 1 through 3600 seconds.\n' >&2
	exit 2
fi
if [ -z "$output" ]; then
	usage >&2
	exit 2
fi
if [ -e "$output" ]; then
	printf 'Refusing to overwrite existing path: %s\n' "$output" >&2
	exit 1
fi

script_dir="$(dirname "${BASH_SOURCE[0]}")"
umask 077
mkdir "$output" || exit 1
mkdir "$output/operator" || exit 1

status="completed"
trace_args=(--duration "$duration" --output "$output/trace")
if [ "$capture_direct_touch" -eq 1 ]; then
	trace_args+=(--capture-direct-touch)
fi
if [ "$capture_stylus" -eq 1 ]; then
	trace_args+=(--capture-stylus)
fi

if ! "$script_dir/collect.sh" --output "$output/collection"; then
	status="collection-failed"
fi
if ! "$script_dir/capture_linux_trace_bundle.sh" "${trace_args[@]}"; then
	status="trace-failed"
fi

created_utc="$(date -u +%Y-%m-%dT%H:%M:%SZ)"
cat > "$output/session-status.txt" <<EOF
format_version=1
session_type=blinded-hardware-evidence
created_at_utc=$created_utc
period_label=$period
case_name=$case_name
requested_duration_seconds=$duration
direct_touch_capture_requested=$capture_direct_touch
stylus_capture_requested=$capture_stylus
runner=${BASH_SOURCE[0]}
runner_uid=$(id -u)
status=$status
safety=read-only; no sudo; no profile selection/install/load/unload; no profile mapping
EOF

cat > "$output/operator/worksheet.txt" <<EOF
Blinded Hardware Session Operator Worksheet

Period label: $period
Case name: $case_name
Requested duration (seconds): $duration
Direct-touch capture requested: $capture_direct_touch
Stylus capture requested: $capture_stylus
Session start (UTC): ____________________
Session end (UTC): ____________________
Operator name/signature: ____________________

Safety acknowledgement: independent input is available; stop and preserve
evidence for a warning, crash, unexpected input, thermal issue, or input loss.

Outcome (pass/fail/not observed/not applicable): ____________________
Observed input behavior and deviations (verbatim):


Kernel warnings/errors observed:


Direct-touch capture result (if requested):


Stylus capture result (if requested):


Artifact review: collection/ and trace/ retained; verify SHA-256 checksums.txt.
EOF

cat > "$output/ROLE_BOUNDARIES.md" <<'EOF'
# Blinded Session Role Boundaries

The coordinator assigns only the opaque period label and case name to the
operator. The coordinator keeps any assignment key and all profile information
outside this evidence directory.

The operator runs the command below exactly with the assigned opaque values,
records observations in `operator/worksheet.txt`, and does not inspect or
request profile selection, installation, module changes, or an assignment key.

```sh
./tools/hardware_evidence/run_blinded_session.sh --period p1 --case CASE --duration 20 --output evidence/RUN/p1-CASE [--capture-direct-touch] [--capture-stylus]
```

The runner and its helpers are read-only. They do not invoke `sudo`, choose,
install, load, unload, bind, configure, or identify a profile. If access to
logs or `/dev/input` is needed, the caller may use their approved access method.

The assessor receives the completed evidence directory and worksheet, verifies
`checksums.txt`, and evaluates artifacts using only opaque labels. The assessor
does not obtain assignment information until the predeclared assessment is
frozen.
EOF

{
	printf '# SHA-256 checksums for blinded session artifacts\n'
	printf '# Excludes checksums.txt because it cannot checksum itself.\n'
	while IFS= read -r artifact; do
		[ "$artifact" = "checksums.txt" ] && continue
		(cd "$output" && sha256sum "$artifact")
	done < <(cd "$output" && find . -type f -printf '%P\n' | sort)
} > "$output/checksums.txt"

# Keep the complete blinded evidence directory reviewable by the sudo caller.
if [ -n "${SUDO_USER:-}" ]; then
	chown -R "$SUDO_USER" "$output"
fi

printf 'Blinded session evidence written to %s (status: %s)\n' "$output" "$status"
[ "$status" = "completed" ] || exit 1
