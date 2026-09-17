#!/bin/bash
# hunt, end to end, without hardware.
#
# This is the test that would have caught the bug that cost a user an evening:
# a two-line comment whose second line had lost its leading '#', which bash
# then ran as a command ('yields: command not found'), killing hunt with rc=127
# after it had already unloaded the driver. bash -n cannot see that, the host
# suite could not see that, and the artifact looked complete because stderr
# from inside the sweep is redirected into the file.
#
# So: build a fake machine (stub modprobe/dkms/dmesg/sleep, a fake panel in a
# fake sysfs, a profile file), point the tool at it, run `hunt`, and demand the
# things a working sweep must produce. Only the root check and the absolute
# paths are patched out of the copy under test; everything else is the shipped
# script, verbatim.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SB="$(mktemp -d "${TMPDIR:-/tmp}/sl4a-hunt-sandbox.XXXXXX")"
trap 'rm -rf "$SB"' EXIT

fail() { echo "hunt sandbox contract: FAIL — $*"; exit 1; }

mkdir -p "$SB/bin" "$SB/etc" "$SB/var" "$SB/src" \
         "$SB/sys/bus/spi/devices/spi-MSHW0231:00"
D="$SB/sys/bus/spi/devices/spi-MSHW0231:00"
printf 'reset_rsp=0\ndevice_desc=0\nbus_error_count=0\n' > "$D/protocol_stats"
for f in ready seq_state lifecycle_status bus_error_count device_initiated_reset_count; do
	printf 'stub\n' > "$D/$f"
done
printf '# SL4A_TouchScreen\noptions sl4a_spi_hid raw_mode=Y raw_input_beta=Y acpi_probe_power_cycle=1 skip_vendor_stop=1\n' \
	> "$SB/etc/sl4a-spi-hid.conf"
printf '# SL4A_TouchScreen\n' > "$SB/etc/sl4a-touch-activate.service"
: > "$SB/dmesg.txt"
for i in $(seq 1 200); do
	printf '[%d.0] fake line %d\n[%d.1] sl4a_spi_hid: stub %d\n' "$i" "$i" "$i" "$i" >> "$SB/dmesg.txt"
done

# `sleep` is stubbed so the sweep takes a second instead of 33; `make` so
# run_host_self_tests cannot re-enter this very suite.
for c in systemctl depmod mokutil make modinfo sleep; do
	printf '#!/bin/bash\nexit 0\n' > "$SB/bin/$c"
	chmod +x "$SB/bin/$c"
done
# dkms records what it was asked to do: the rebuild path (restage_and_rebuild)
# must actually invoke it — a stale stamp that prints "rebuilding first" and
# then rebuilds nothing swept the old modules silently with the sandbox green
# (P15 wave, M9).
cat > "$SB/bin/dkms" <<EOS
#!/bin/bash
echo "dkms \$*" >> "$SB/dkms.log"
exit 0
EOS
chmod +x "$SB/bin/dkms"
# modprobe records its arguments: the sweep must hand the controller's
# debug_trace to sl4a_spi_amd, or the peek line — the one that answers the
# RX-region question — can never appear in the artifact. A load of the driver
# also emits a realistic level-3 burst: the FIRST control write the sweep's
# evidence line looks for, then a hundred-plus per-frame lines. The write is
# deliberately far from the tail — a window that greps only the last 60 lines
# must lose it (that was the P3 wave's finding).
cat > "$SB/bin/modprobe" <<'EOS'
#!/bin/bash
echo "$*" >> "__SB__/modprobe.log"
if [ "${1:-}" != "-r" ] && [[ " $* " == *" sl4a_spi_hid "* ]] && [ ! -f "__SB__/quiet" ]; then
	{
		printf '[999.0] sl4a_spi_hid: SEQ: write op=0x02 reg=1 raw=[02 00 00 01 42 00 00 03 00 00]\n'
		i=0
		while [ "$i" -lt 120 ]; do
			printf '[999.1] sl4a_spi_hid: read begin op=0x0b tx=8 rx=16\n'
			i=$((i + 1))
		done
	} >> "__SB__/dmesg.txt"
fi
exit 0
EOS
sed -i "s#__SB__#$SB#g" "$SB/bin/modprobe"
chmod +x "$SB/bin/modprobe"
printf '#!/bin/bash\ncat "%s/dmesg.txt"\n' "$SB" > "$SB/bin/dmesg"
chmod +x "$SB/bin/dmesg"

sed -e "s#^REPO_DIR=.*#REPO_DIR=\"$ROOT\"#" \
    -e "s#^SRC_DEST=.*#SRC_DEST=\"$SB/src\"#" \
    -e "s#^MODPROBE_CONF=.*#MODPROBE_CONF=\"$SB/etc/sl4a-spi-hid.conf\"#" \
    -e "s#^SYSTEMD_UNIT=.*#SYSTEMD_UNIT=\"$SB/etc/sl4a-touch-activate.service\"#" \
    -e "s#^INSTALLED_HEAD_STAMP=.*#INSTALLED_HEAD_STAMP=\"$SB/var/installed-head\"#" \
    -e "s#/sys/bus/spi/devices/\*MSHW\*#$SB/sys/bus/spi/devices/*MSHW*#" \
    -e 's#^\([[:space:]]*\)\[ "\$(id -u)" = 0 \] || fail "hunt needs root.*#\1: #' \
    "$ROOT/tools/sl4a-touch.sh" > "$SB/tool.sh" || fail "could not stage the tool"
chmod +x "$SB/tool.sh"
git -C "$ROOT" rev-parse HEAD > "$SB/var/installed-head"

PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out.txt" > "$SB/run.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run.txt"; fail "hunt exited $rc (a silent death is exactly the bug this test exists for)"; }

[ -s "$SB/out.txt" ] || fail "no artifact was written"
n="$(grep -c '^VERDICT' "$SB/out.txt" || true)"
[ "$n" -eq 4 ] || fail "expected 4 verdicts in the artifact, found ${n:-0}"

# The peek line that settles the RX-region question only prints at the
# controller's debug_trace=3; the sweep must pass it to sl4a_spi_amd. Without
# this check the sweep loads the controller bare and the one artifact the
# user sends can never carry the peek.
grep -q '^sl4a_spi_amd debug_trace=3$' "$SB/modprobe.log" \
	|| fail "hunt loaded sl4a_spi_amd without debug_trace=3 — the RX-region peek cannot reach the artifact"

# The probe arms are the axis the artifact has to explain: if the sweep does
# not actually load all four, the bundle cannot say which one answered. Order
# matters too — a swapped case arm loads every pair while mislabelling which
# one ran (the P3 wave's stays-green mutation), so the pairs must appear as
# 0/0, 1/0, 0/1, 1/1 repeating, once per sweep.
grep -q 'acpi_probe_power_cycle=0 skip_vendor_stop=0' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the control arm"
grep -q 'acpi_probe_power_cycle=1 skip_vendor_stop=0' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the power-cycle arm"
grep -q 'acpi_probe_power_cycle=0 skip_vendor_stop=1' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the skip-preamble arm"
grep -q 'acpi_probe_power_cycle=1 skip_vendor_stop=1' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the combined arm"
grep -q '^VERDICT (acpi_probe_power_cycle=1 skip_vendor_stop=1)' "$SB/out.txt" \
	|| fail "the VERDICT line itself must carry the probe pair (the echo/header lines are not the verdict)"

grep -o 'acpi_probe_power_cycle=[01] skip_vendor_stop=[01]' "$SB/modprobe.log" > "$SB/pairs.txt"
k=0
while IFS= read -r p; do
	case "$k" in
		0) want="acpi_probe_power_cycle=0 skip_vendor_stop=0" ;;
		1) want="acpi_probe_power_cycle=1 skip_vendor_stop=0" ;;
		2) want="acpi_probe_power_cycle=0 skip_vendor_stop=1" ;;
		3) want="acpi_probe_power_cycle=1 skip_vendor_stop=1" ;;
		*) break ;;
	esac
	[ "$p" = "$want" ] \
		|| fail "load $((k + 1)) carried '$p', expected '$want' — variant order or pair swap"
	k=$((k + 1))
done < "$SB/pairs.txt"
[ "$k" -eq 4 ] || fail "expected one full sweep of 4 driver loads so far, saw $k"

# The artifact must carry the first control write of each load's OWN slice:
# with the realistic burst above, the write sits far above the 60-line tail,
# and a tail-only window dropped it exactly when the load was productive (P3).
grep -q 'first write on the wire: \[999.0\] sl4a_spi_hid: SEQ: write op=0x02' "$SB/out.txt" \
	|| fail "the artifact lost the first control write (the 60-line window again?)"

# The "running variant" line echoes what was REQUESTED; the live readback
# must accompany it so a failed load cannot masquerade as a productive one.
# On this sandbox host the module is never loaded, and the artifact must say
# exactly that (P3 wave).
grep -q 'loaded params (read back): MODULE NOT LOADED' "$SB/out.txt" \
	|| fail "no live module readback line: a failed load would still read as loaded"
grep -q -- '-- OS binding (before the sweep) --' "$SB/out.txt" \
	|| fail "the OS-binding block is not labelled as the pre-sweep snapshot it is"

# The progress the user asked for has to be on the terminal too, not only in
# the file — that is the whole point of it.
grep -q '\[1/4\] variant 0' "$SB/run.txt" || fail "no per-variant progress on the terminal"
grep -q 'TOUCH THE PANEL NOW' "$SB/run.txt" || fail "no touch prompt on the terminal"
grep -q 'four probe variants' "$SB/run.txt" \
	|| fail "the terminal intro still describes the retired wire axis (it must name the probe sweep)"

# A stale stamp must take the rebuild path (it is the path that once died with
# 'command not found'), and the sweep must survive it.
echo "0000000000000000000000000000000000000000" > "$SB/var/installed-head"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out2.txt" > "$SB/run2.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run2.txt"; fail "hunt exited $rc with a stale stamp (the rebuild path)"; }
grep -q 'rebuilding first' "$SB/run2.txt" || fail "a stale stamp did not trigger a rebuild"
grep -q 'dkms build -m sl4a-touch -v ' "$SB/dkms.log" \
	|| fail "a stale stamp printed the rebuild message but dkms was never asked to build (P15 wave, M9)"
[ "$(grep -c '^VERDICT' "$SB/out2.txt" || true)" -eq 4 ] || fail "the artifact after a rebuild is incomplete"

# No panel at all: the sysfs glob matches nothing and the sweep must say so.
# `ls -d` on a vanished (nullglob) pattern lists the CURRENT DIRECTORY, so the
# old one-liner set SYSFS_DIR="." — never empty — and the intended warning was
# dead code. Move the fake panel away and demand the warning plus the honest
# no-counters verdicts.
mv "$D" "$SB/panel-away"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out3.txt" > "$SB/run3.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run3.txt"; fail "hunt exited $rc with no panel present"; }
grep -q 'sysfs directory for the device not found' "$SB/run3.txt" \
	|| fail "no-panel run: the missing-sysfs warning never fired (the glob still resolves to '.')"
grep -q 'bound driver: (sysfs dir not found' "$SB/out3.txt" \
	|| fail "no-panel run: the OS-binding block printed a bare 'none' as if it had probed (a reader would blame the OS)"
[ "$(grep -c 'NO COUNTERS READ' "$SB/out3.txt" || true)" -eq 4 ] \
	|| fail "the no-panel artifact does not degrade honestly to NO COUNTERS READ"

# A load that logs nothing must not abort the sweep, and its absence must be
# STATED: under `set -e -o pipefail` an unguarded `dmesg | grep` that matched
# nothing killed hunt after it had unloaded the driver, and a silenced
# fallback read as "searched and found nothing" without evidence (P14 wave:
# both mutations stayed green before these two sweeps).
mv "$SB/panel-away" "$D"
touch "$SB/quiet"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out4.txt" > "$SB/run4.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run4.txt"; fail "hunt exited $rc with a silent driver (an unguarded dmesg|grep again?)"; }
grep -q 'no lines after the mark' "$SB/out4.txt" \
	|| fail "quiet run: the ring-wrap fallback message never appeared"
n_none="$(grep -cF "first write on the wire: (none in this load's log)" "$SB/out4.txt" || true)"
[ "$n_none" -eq 4 ] \
	|| fail "quiet run: the artifact does not state the write's absence for every variant (saw $n_none)"

# Ring wrapped between the mark and the slice: the write still exists in the
# buffer, outside the slice, and the search must find it in the fallback too —
# labelled, because those lines can span loads (P14 wave, F7).
printf '[1000.0] sl4a_spi_hid: SEQ: write op=0x02 reg=1 raw=[02 00 00 01 42 00 00 03 00 00] (pre-wrap marker)\n' >> "$SB/dmesg.txt"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out5.txt" > "$SB/run5.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run5.txt"; fail "hunt exited $rc on the wrapped-ring run"; }
n_wr="$(grep -c 'first write on the wire (from the wrapped ring — may belong to an earlier load)' "$SB/out5.txt" || true)"
[ "$n_wr" -eq 4 ] \
	|| fail "wrapped-ring run: the first-write line did not search the fallback window (saw $n_wr) — or its caveat was dropped (P15 wave: the pin must cover the full label, not its prefix)"

# No profile at all: the sweep still runs, but raw_mode=N is STANDARD mode
# where the probe arms are inert (spi_hid_vendor_init is raw-gated) — the
# fallback must be labelled in the artifact AND warned on the terminal, not
# printed as if the file had said so (P14 wave, A:C6/F5).
mv "$SB/etc/sl4a-spi-hid.conf" "$SB/etc/sl4a-spi-hid.conf.bak"
git -C "$ROOT" rev-parse HEAD > "$SB/var/installed-head"
rm -f "$SB/quiet"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out6.txt" > "$SB/run6.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run6.txt"; fail "hunt exited $rc with a missing profile"; }
grep -q 'STANDARD mode, probe arms 2/3 are inert here' "$SB/out6.txt" \
	|| fail "the missing-profile artifact does not label its raw_mode=N fallback (full note, P15 wave: a shortened ' (FALLBACK)' satisfied the old prefix-only pin)"
grep -q 'fallback standard mode' "$SB/run6.txt" \
	|| fail "the missing-profile run printed no warning about the fallback"
[ "$(grep -c 'first write on the wire: \[999.0\]' "$SB/out6.txt" || true)" -eq 4 ] \
	|| fail "the missing-profile sweep did not carry the first write (dmesg emission should have resumed)"
mv "$SB/etc/sl4a-spi-hid.conf.bak" "$SB/etc/sl4a-spi-hid.conf"

# A profile whose EVERY parameter is one the sweep controls empties the
# carried-options set without the file being missing: the artifact must not
# claim a missing 'options' line about a line that is right there (P15 wave,
# B:C1), and the sweep must still run and label honestly.
printf '# SL4A_TouchScreen\noptions sl4a_spi_hid acpi_probe_power_cycle=1 skip_vendor_stop=1\n' \
	> "$SB/etc/sl4a-spi-hid.conf"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out7.txt" > "$SB/run7.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run7.txt"; fail "hunt exited $rc with an all-filtered profile"; }
[ "$(grep -c '^VERDICT' "$SB/out7.txt" || true)" -eq 4 ] \
	|| fail "the all-filtered-profile artifact is incomplete"
grep -q 'all of its parameters are sweep-controlled' "$SB/out7.txt" \
	|| fail "an all-filtered profile is not labelled as such (P15 wave, B:C1)"
grep -q "no 'options sl4a_spi_hid' line" "$SB/run7.txt" \
	&& fail "the all-filtered profile took the missing-line warning about a line that exists (P15 wave, B:C1)"
grep -q 'FALLBACK' "$SB/out7.txt" \
	&& fail "the all-filtered profile is labelled FALLBACK though nothing is missing (P15 wave, B:C1)"

# Every sweep so far (control, rebuild, no-panel, quiet, wrapped ring,
# missing profile, all-filtered) loaded the same four arms in the same order:
# 28 loads, the pattern repeating. The profile now also carries the two knobs,
# and the $opts filter must keep them OFF the load lines — a leaked pair ahead
# of the arm's own could read as the arm's value (P14 wave: that mutation
# stayed green too).
n_loaded="$(grep -c 'acpi_probe_power_cycle=[01] skip_vendor_stop=[01]' "$SB/modprobe.log" || true)"
[ "$n_loaded" -eq 28 ] \
	|| fail "expected 28 driver loads after seven sweeps, saw $n_loaded"
awk '
	/sl4a_spi_hid/ && /acpi_probe_power_cycle=/ {
		if (gsub(/acpi_probe_power_cycle=/, "&") != 1 ||
		    gsub(/skip_vendor_stop=/, "&") != 1) {
			printf "leaky load line: %s\n", $0
			bad = 1
		}
	}
	END { exit bad }
' "$SB/modprobe.log" || fail "the profile's knobs leaked onto a load line (the \$opts filter regressed)"

# The profile's raw_mode must REACH the load lines: it is the mode the whole
# probe sweep is about, and the $opts filter carries it (raw_mode is not one
# of the six the sweep controls). Adding it to the filter dropped the field's
# raw mode on every arm with the sandbox still green — every verdict read
# productive while the sweep measured standard mode (P15 wave, M6).
n_rawy="$(grep -c 'raw_mode=Y' "$SB/modprobe.log" || true)"
[ "$n_rawy" -eq 20 ] \
	|| fail "the profile's raw_mode=Y reached $n_rawy driver loads, expected 20 (five profile sweeps) — the \$opts filter dropped it (P15 wave, M6)"
n_rawn="$(grep -c 'raw_mode=N' "$SB/modprobe.log" || true)"
[ "$n_rawn" -eq 4 ] \
	|| fail "the missing-profile fallback loads should carry raw_mode=N on the 4 loads, saw $n_rawn (P15 wave, M6)"

echo "hunt sandbox contract: PASS (sweep completes, rebuild path survives and really rebuilds, 4 verdicts, progress on the terminal, controller debug_trace passed, probe arms loaded in order, raw_mode carried, first write survives the window, live readback present, no-panel run warns and degrades honestly, quiet load states its absence and survives, wrapped ring labels the fallback read, missing profile labels the fallback mode, all-filtered profile labels itself)"
