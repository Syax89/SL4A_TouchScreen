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
# fake sysfs, a fake input event node + evdev char device, a profile file),
# point the tool at it, run `hunt`, and demand the things a working battery
# must produce. Only the root check and the absolute paths are patched out of
# the copy under test; everything else is the shipped script, verbatim.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SB="$(mktemp -d "${TMPDIR:-/tmp}/sl4a-hunt-sandbox.XXXXXX")"
trap 'rm -rf "$SB"' EXIT

fail() { echo "hunt sandbox contract: FAIL — $*"; exit 1; }

mkdir -p "$SB/bin" "$SB/etc" "$SB/var" "$SB/src" \
         "$SB/sys/bus/spi/devices/spi-MSHW0231:00/input/input10" \
         "$SB/sys/class/input/event10" "$SB/dev/input"
D="$SB/sys/bus/spi/devices/spi-MSHW0231:00"
printf 'reset_rsp=0\ndevice_desc=0\nrpt_desc=0\ndata=0\nirq_count=0\n' > "$D/protocol_stats"
for f in ready seq_state lifecycle_status bus_error_count device_initiated_reset_count; do
	printf 'stub\n' > "$D/$f"
done
# The panel's input event node and its evdev char device. The node's `device`
# path resolves UNDER the panel's controller ($D/input/input10) — that is how
# the battery finds the standard profile's node, whose name ("spi 045E:0C19")
# matches none of the raw-name patterns the discovery used to rely on. The
# name is (re)written per profile by the modprobe stub below, exactly as the
# driver names its node in each mode. The raw device is a regular file inside
# the sandbox: the bounded read that a real run performs against
# /dev/input/eventN finishes instantly here (EOF), so the flow stays covered
# without a touch surface and without the fallback prompt.
printf 'MSHW0231 Touchscreen\n' > "$D/input/input10/name"
ln -s "$D/input/input10" "$SB/sys/class/input/event10/device"
head -c 48 /dev/zero > "$SB/dev/input/event10"
printf '# SL4A_TouchScreen\noptions sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y\n' \
	> "$SB/etc/sl4a-spi-hid.conf"
printf '# SL4A_TouchScreen\n' > "$SB/etc/sl4a-touch-activate.service"
: > "$SB/dmesg.txt"
for i in $(seq 1 200); do
	printf '[%d.0] fake line %d\n[%d.1] sl4a_spi_hid: stub %d\n' "$i" "$i" "$i" "$i" >> "$SB/dmesg.txt"
done

# `make` so run_host_self_tests cannot re-enter this very suite. `sleep` is not
# a no-op: it advances the panel's counters, so the before/after deltas the
# summary reports are non-zero and exercise the verdict arithmetic (a real run
# spends its whole touch window in `sleep`). systemctl/depmod/mokutil/modinfo
# are inert.
for c in systemctl depmod mokutil make modinfo; do
	printf '#!/bin/bash\nexit 0\n' > "$SB/bin/$c"
	chmod +x "$SB/bin/$c"
done
cat > "$SB/bin/sleep" <<EOS
#!/bin/bash
f="$D/protocol_stats"
if [ -f "\$f" ]; then
	# Which load is CURRENT: the last sl4a_spi_hid load line in the modprobe
	# log. Only the raw_b1f8109_preset variant advances the descriptor-reply
	# counters (b1f8109 is the dialect that answered), so its summary note's
	# device_desc/rpt_desc deltas are non-zero and that note's wiring is
	# actually tested; every other load stays at zero, exactly as before.
	last="\$(grep '^sl4a_spi_hid ' "$SB/modprobe.log" 2>/dev/null | tail -1)"
	desc=0
	case " \$last " in
		*" raw_b1f8109_preset=1 "*) desc=1 ;;
	esac
	{
		while IFS= read -r line; do
			case "\$line" in
				reset_rsp=*) n="\${line#reset_rsp=}"; echo "reset_rsp=\$((n + 1))" ;;
				device_desc=*)
					n="\${line#device_desc=}"
					if [ "\$desc" = 1 ]; then echo "device_desc=\$((n + 1))"; else echo "\$line"; fi ;;
				rpt_desc=*)
					n="\${line#rpt_desc=}"
					if [ "\$desc" = 1 ]; then echo "rpt_desc=\$((n + 1))"; else echo "\$line"; fi ;;
				*) echo "\$line" ;;
			esac
		done < "\$f"
	} > "\$f.tmp" && mv "\$f.tmp" "\$f"
fi
exit 0
EOS
chmod +x "$SB/bin/sleep"
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
# modprobe records its arguments: the battery must hand the controller's
# debug_trace to sl4a_spi_amd, or the peek line — the one that answers the
# RX-region question — can never appear in the artifact. A load of the driver
# also emits a realistic level-3 burst: the FIRST control write the sweep's
# evidence line looks for, then a hundred-plus per-frame lines. The write is
# deliberately far from the tail — a window that greps only the last 60 lines
# must lose it (that was the P3 wave's finding).
cat > "$SB/bin/modprobe" <<'EOS'
#!/bin/bash
echo "$*" >> "__SB__/modprobe.log"
if [ "${1:-}" != "-r" ] && [[ " $* " == *" sl4a_spi_hid "* ]]; then
	# Name the panel's input node the way the driver does for this profile:
	# the standard HID path names it after the hid device ("spi 045E:0C19"),
	# raw mode names it "MSHW0231 Touchscreen". The battery must find the node
	# in BOTH cases, so the standard name matches none of the raw ones.
	if [[ " $* " == *" raw_mode=Y "* ]]; then
		printf 'MSHW0231 Touchscreen\n' > "__SB__/sys/bus/spi/devices/spi-MSHW0231:00/input/input10/name"
	else
		printf 'spi 045E:0C19\n' > "__SB__/sys/bus/spi/devices/spi-MSHW0231:00/input/input10/name"
	fi
fi
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
    -e "s#^INPUT_SYSFS=.*#INPUT_SYSFS=\"$SB/sys/class/input\"#" \
    -e "s#^INPUT_DEV_ROOT=.*#INPUT_DEV_ROOT=\"$SB/dev/input\"#" \
    -e "s#/sys/bus/spi/devices/\*MSHW\*#$SB/sys/bus/spi/devices/*MSHW*#" \
    -e "s#/sys/module/#$SB/sys/module/#g" \
    -e 's#^\([[:space:]]*\)\[ "\$(id -u)" = 0 \] || fail "hunt needs root.*#\1: #' \
    "$ROOT/tools/sl4a-touch.sh" > "$SB/tool.sh" || fail "could not stage the tool"
chmod +x "$SB/tool.sh"
git -C "$ROOT" rev-parse HEAD > "$SB/var/installed-head"

# ── the variant plan, as the battery loads it (one line per variant) ───────
# The battery is data-driven (HUNT_VARIANTS): every variant is one driver load,
# in this order, with exactly these parameters. Order matters — a swapped label
# or a dropped arm would otherwise stay green.
cat > "$SB/expected-loads.txt" <<'EOS'
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y raw_pre_desc_reg0=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y raw_fallback_on_reset=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y raw_pre_desc_reg0=1 raw_fallback_on_reset=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y read_frame_variant=2 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y wire_double_opcode=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y raw_b1f8109_preset=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y wire_double_opcode=1 read_frame_variant=2 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y wire_double_opcode=1 raw_pre_desc_reg0=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y wire_double_opcode=1 read_frame_variant=2 raw_pre_desc_reg0=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y wire_double_opcode=1 skip_vendor_stop=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=N sl4a_debug_level=3
sl4a_spi_hid raw_mode=N wire_double_opcode=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=N skip_std_getfeat=1 sl4a_debug_level=3
sl4a_spi_hid raw_mode=N wire_double_opcode=1 skip_std_getfeat=1 sl4a_debug_level=3
EOS
NVARIANTS=15
# Sweeps this script runs (main, stale-stamp rebuild, no-panel, quiet, wrapped
# ring, evdev fallback, stubbed module).
NSWEEPS=7

PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out.txt" > "$SB/run.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run.txt"; fail "hunt exited $rc (a silent death is exactly the bug this test exists for)"; }

[ -s "$SB/out.txt" ] || fail "no artifact was written"
n="$(grep -c '^VERDICT' "$SB/out.txt" || true)"
[ "$n" -eq "$NVARIANTS" ] || fail "expected $NVARIANTS verdicts in the artifact, found ${n:-0}"

# The peek line that settles the RX-region question only prints at the
# controller's debug_trace=3; the battery must pass it to sl4a_spi_amd. Without
# this check the sweep loads the controller bare and the one artifact the
# user sends can never carry the peek.
grep -q '^sl4a_spi_amd debug_trace=3$' "$SB/modprobe.log" \
	|| fail "hunt loaded sl4a_spi_amd without debug_trace=3 — the RX-region peek cannot reach the artifact"

# Every variant is one driver load, in order, with exactly its parameters —
# the battery is the campaign's whole plan, so a dropped arm or a swapped pair
# has to bite. (`-r` unloads and the controller load do not match this prefix.)
grep '^sl4a_spi_hid ' "$SB/modprobe.log" | head -n "$NVARIANTS" > "$SB/got-loads.txt"
if ! diff -u "$SB/expected-loads.txt" "$SB/got-loads.txt" > "$SB/loads.diff"; then
	sed -n '1,40p' "$SB/loads.diff"
	fail "the battery did not load the planned variants in order (see diff above)"
fi

# The summary table is the "where are we" the user asked for: one row per
# variant, right before the self-tests, with the evdev touch verdict.
grep -q '=== SUMMARY (15 variants) ===' "$SB/out.txt" \
	|| fail "the artifact has no summary table"
grep -q 'variant .*| device_desc .*| data .*| reset_rsp .*| touch(evdev events) .*| note' "$SB/out.txt" \
	|| fail "the summary table header is missing a column"
n_rows="$(grep -cE '^raw |^std ' "$SB/out.txt" || true)"
[ "$n_rows" -eq "$NVARIANTS" ] \
	|| fail "the summary table has $n_rows rows, expected $NVARIANTS"
awk '/^=== SUMMARY/{found=1; next} found && /^raw |^std /{n++} END{exit (n == 15) ? 0 : 1}' "$SB/out.txt" \
	|| fail "the summary rows are not all after the SUMMARY header"
# The summary must precede the self-tests section.
grep -n '=== SUMMARY' "$SB/out.txt" | head -1 | cut -d: -f1 > "$SB/summary.line"
grep -n -- '--- Self-tests' "$SB/out.txt" | head -1 | cut -d: -f1 > "$SB/selftest.line"
[ -s "$SB/summary.line" ] && [ -s "$SB/selftest.line" ] \
	|| fail "the summary table is not before the self-tests section"
[ "$(cat "$SB/summary.line")" -lt "$(cat "$SB/selftest.line")" ] \
	|| fail "the summary table is not before the self-tests section"

# The touch verdict is measured, not asked: the panel's evdev node is read
# during the window and the byte/event count reaches the artifact and the table.
grep -q 'evdev read on event10' "$SB/out.txt" \
	|| fail "the artifact does not record the evdev read"
grep -q 'evdev event10: 48 bytes, 2 events' "$SB/out.txt" \
	|| fail "the evdev read did not report bytes/events from the node"
grep -qE '^raw control +\| \+0 +\| \+0 +\| \+6 +\| 2 events +\| resets \+6' "$SB/out.txt" \
	|| fail "the summary row does not carry the deltas and the evdev verdict"

# …and the same for the STANDARD profile. The raw node is named "MSHW0231
# Touchscreen" and was always found; the standard node is named "spi 045E:0C19"
# after the hid device and matches none of the raw-name patterns, so before the
# fix every standard row fell through to "human:no" while its node sat right
# there. Discovery must now reach it by its ancestry under the panel's
# controller, and the standard row must carry an event count too.
grep -q 'spi 045E:0C19.*<-- touch device' "$SB/out.txt" \
	|| fail "the standard profile's HID node was not recognised as the touch device"
grep -qE '^std control +\| \+0 +\| \+0 +\| \+6 +\| 2 events +\| resets \+6' "$SB/out.txt" \
	|| fail "the standard profile fell back to the human y/n instead of reading its evdev node"

# The raw_b1f8109_preset row is the "restart from where it worked" candidate, so
# its note must surface BOTH descriptor-reply counters (device_desc, rpt_desc)
# explicitly — those two say whether b1f8109's dialect lands. The sleep stub
# advances them only for this load, so the note carries the real deltas and a
# hard-coded or generic note cannot satisfy the pin.
grep -qE '^raw raw_b1f8109_preset=1 +\| \+6 +\| \+0 +\| \+6 +\| 2 events +\| device_desc \+6 rpt_desc \+6' "$SB/out.txt" \
	|| fail "the b1f8109-preset row does not surface its device_desc/rpt_desc deltas in the note"

# The deltas are measured from the counters around the touch window. The sleep
# stub advances reset_rsp once per countdown second (6), so a run that dropped
# the before/after snapshots would report +0.
grep -q 'reset_rsp=6 device_desc=0 data=0 irq_count=0' "$SB/out.txt" \
	|| fail "the counter deltas were not computed around the touch window"

# The artifact must carry the first control write of each load's OWN slice:
# with the realistic burst above, the write sits far above the 60-line tail,
# and a tail-only window dropped it exactly when the load was productive (P3).
n_wr="$(grep -c 'first write on the wire: \[999.0\] sl4a_spi_hid: SEQ: write op=0x02' "$SB/out.txt" || true)"
[ "$n_wr" -eq "$NVARIANTS" ] \
	|| fail "the artifact lost the first control write for some variant (the 60-line window again? saw $n_wr)"

# The "running variant" line echoes what was REQUESTED; the live readback must
# accompany it so a failed load cannot masquerade as a productive one. All
# module reads go through the scoped stub sysfs (the staging sed rewrites
# /sys/module/), so with no stub module present the artifact must say exactly
# that (P3 wave) — and must say it identically on a host that happens to have
# the real driver loaded.
grep -q 'loaded params (read back): MODULE NOT LOADED' "$SB/out.txt" \
	|| fail "no live module readback line: a failed load would still read as loaded"
grep -q -- '-- OS binding (before the sweep) --' "$SB/out.txt" \
	|| fail "the OS-binding block is not labelled as the pre-sweep snapshot it is"

# The progress the user asked for has to be on the terminal too, not only in
# the file — that is the whole point of it.
grep -q '\[1/15\] raw control' "$SB/run.txt" || fail "no per-variant progress on the terminal"
grep -q 'TOUCH THE PANEL NOW' "$SB/run.txt" || fail "no touch prompt on the terminal"
grep -q 'raw AND standard variants' "$SB/run.txt" \
	|| fail "the terminal intro still describes the retired probe sweep (it must name the raw+standard battery)"

# A stale stamp must take the rebuild path (it is the path that once died with
# 'command not found'), and the battery must survive it.
echo "0000000000000000000000000000000000000000" > "$SB/var/installed-head"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out2.txt" > "$SB/run2.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run2.txt"; fail "hunt exited $rc with a stale stamp (the rebuild path)"; }
grep -q 'rebuilding first' "$SB/run2.txt" || fail "a stale stamp did not trigger a rebuild"
grep -q 'dkms build -m sl4a-touch -v ' "$SB/dkms.log" \
	|| fail "a stale stamp printed the rebuild message but dkms was never asked to build (P15 wave, M9)"
# ...and only THERE: a `dkms build` from any other path (say, unconditionally
# at the top of cmd_hunt) satisfied the existence-only check above while the
# stale-stamp path rebuilt nothing (P16 wave, B:C6). Run 1 used a fresh stamp,
# so by now exactly one build has a reason to exist.
[ "$(grep -c 'dkms build -m sl4a-touch -v ' "$SB/dkms.log" || true)" -eq 1 ] \
	|| fail "expected exactly one dkms build (the stale-stamp rebuild), saw $(grep -c 'dkms build -m sl4a-touch -v ' "$SB/dkms.log" || true) — a build from another path satisfies the old check (P16 wave, B:C6)"
[ "$(grep -c '^VERDICT' "$SB/out2.txt" || true)" -eq "$NVARIANTS" ] || fail "the artifact after a rebuild is incomplete"

# No panel at all: the sysfs glob matches nothing and the battery must say so.
# `ls -d` on a vanished (nullglob) pattern lists the CURRENT DIRECTORY, so the
# old one-liner set SYSFS_DIR="." — never empty — and the intended warning was
# dead code. Move the fake panel away and demand the warning plus the honest
# no-counters verdicts. The panel's input node hangs off the controller, so it
# goes with it (its `device` link dangles) and the touch verdict is the honest
# fallback — keep stdin at /dev/null so that fallback read returns at once.
mv "$D" "$SB/panel-away"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out3.txt" > "$SB/run3.txt" 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run3.txt"; fail "hunt exited $rc with no panel present"; }
grep -q 'sysfs directory for the device not found' "$SB/run3.txt" \
	|| fail "no-panel run: the missing-sysfs warning never fired (the glob still resolves to '.')"
grep -q 'bound driver: (sysfs dir not found' "$SB/out3.txt" \
	|| fail "no-panel run: the OS-binding block printed a bare 'none' as if it had probed (a reader would blame the OS)"
[ "$(grep -c 'NO COUNTERS READ' "$SB/out3.txt" || true)" -eq "$NVARIANTS" ] \
	|| fail "the no-panel artifact does not degrade honestly to NO COUNTERS READ"
grep -q 'no input event node under the panel' "$SB/out3.txt" \
	|| fail "no-panel run: the artifact did not state that no input node was registered"

# The descfocus branch must not fire without counters: with the panel gone every
# row degrades to "no counters" (including the preset row), never a fabricated
# "device_desc +0 rpt_desc +0" that would read as a measured dialect.
grep -qE '^raw raw_b1f8109_preset=1 .*\| no counters$' "$SB/out3.txt" \
	|| fail "no-panel run: the preset row's note did not degrade to 'no counters'"

# A load that logs nothing must not abort the battery, and its absence must be
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
[ "$n_none" -eq "$NVARIANTS" ] \
	|| fail "quiet run: the artifact does not state the write's absence for every variant (saw $n_none)"

# Ring wrapped between the mark and the slice: the write still exists in the
# buffer, outside the slice, and the search must find it in the fallback too —
# labelled, because those lines can span loads (P14 wave, F7). `quiet` is
# deliberately left in place: the run emits no burst of its own, so the only
# write in the buffer is the pre-wrap marker appended here, exactly the shape
# the fallback search exists for.
printf '[1000.0] sl4a_spi_hid: SEQ: write op=0x02 reg=1 raw=[02 00 00 01 42 00 00 03 00 00] (pre-wrap marker)\n' >> "$SB/dmesg.txt"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out5.txt" > "$SB/run5.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run5.txt"; fail "hunt exited $rc on the wrapped-ring run"; }
n_wr="$(grep -c 'first write on the wire (from the wrapped ring — may belong to an earlier load)' "$SB/out5.txt" || true)"
[ "$n_wr" -eq "$NVARIANTS" ] \
	|| fail "wrapped-ring run: the first-write line did not search the fallback window (saw $n_wr) — or its caveat was dropped (P15 wave: the pin must cover the full label, not its prefix)"
rm -f "$SB/quiet"

# No evdev node (an unusual panel, or the node not registered): the tool must
# fall back to the human y/n and still produce the full artifact. Run it with
# stdin at /dev/null so the fallback read returns immediately (a real run waits
# the countdown). The touch column then says "human:no", not an event count.
mv "$SB/sys/class/input" "$SB/class-input-away"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out6.txt" > "$SB/run6.txt" 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run6.txt"; fail "hunt exited $rc with no evdev node (the fallback path)"; }
[ "$(grep -c '^VERDICT' "$SB/out6.txt" || true)" -eq "$NVARIANTS" ] \
	|| fail "the no-evdev artifact is incomplete"
grep -q 'no evdev node for this panel' "$SB/out6.txt" \
	|| fail "no-evdev run: the artifact does not record the fallback"
grep -q 'human fallback (evdev node' "$SB/out6.txt" \
	|| fail "no-evdev run: the human y/n fallback did not run"
grep -qE '^raw control +\| \+0 +\| \+0 +\| \+6 +\| human:no +\| resets \+6' "$SB/out6.txt" \
	|| fail "no-evdev run: the summary does not carry the human fallback verdict"
mv "$SB/class-input-away" "$SB/sys/class/input"

# A stubbed module must be seen through the scoped sysfs too: every module
# read (the readback line, the arm echo) goes through /sys/module under $SB.
# If the staging sed ever drops the rewrite, this run reads the HOST's
# /sys/module instead — on a module-less CI host it would print MODULE NOT
# LOADED and still pass, so only the stub values pin it.
mkdir -p "$SB/sys/module/sl4a_spi_hid/parameters" "$SB/sys/module/sl4a_spi_amd/parameters"
for p in raw_mode raw_input_beta skip_getfeat read_frame_variant wire_double_opcode; do
	printf 'Y\n' > "$SB/sys/module/sl4a_spi_hid/parameters/$p"
done
for p in raw_pre_desc_reg0 raw_fallback_on_reset skip_vendor_stop raw_b1f8109_preset; do
	printf 'N\n' > "$SB/sys/module/sl4a_spi_hid/parameters/$p"
done
printf '3\n' > "$SB/sys/module/sl4a_spi_hid/parameters/sl4a_debug_level"
printf '0\n' > "$SB/sys/module/sl4a_spi_amd/parameters/debug_trace"
git -C "$ROOT" rev-parse HEAD > "$SB/var/installed-head"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out7.txt" > "$SB/run7.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run7.txt"; fail "hunt exited $rc with a stubbed module present"; }
grep -q 'loaded params (read back): raw_mode=Y raw_input_beta=Y skip_getfeat=Y read_frame_variant=Y wire_double_opcode=Y raw_pre_desc_reg0=N raw_fallback_on_reset=N skip_vendor_stop=N raw_b1f8109_preset=N' "$SB/out7.txt" \
	|| fail "the readback did not quote the scoped sysfs — the staging sed lost the /sys/module/ rewrite and the host is being read"

# ── the whole battery, every sweep, in aggregate ───────────────────────────
# Seven sweeps x the plan: the counts land exactly, so a sweep that silently
# skipped a variant (or an extra load from another path) bites here. A
# `dkms`/controller line cannot satisfy the `sl4a_spi_hid ` prefix, and the
# `-r` unloads do not start with it either.
n_hid="$(grep -c '^sl4a_spi_hid ' "$SB/modprobe.log" || true)"
[ "$n_hid" -eq "$((NSWEEPS * NVARIANTS))" ] \
	|| fail "expected $((NSWEEPS * NVARIANTS)) driver loads after $NSWEEPS sweeps, saw $n_hid"
n_rawy="$(grep -c 'raw_mode=Y' "$SB/modprobe.log" || true)"
n_rawn="$(grep -c 'raw_mode=N' "$SB/modprobe.log" || true)"
[ "$n_rawy" -eq "$((NSWEEPS * 11))" ] \
	|| fail "raw_mode=Y reached $n_rawy driver loads, expected $((NSWEEPS * 11)) (11 raw variants per sweep)"
[ "$n_rawn" -eq "$((NSWEEPS * 4))" ] \
	|| fail "raw_mode=N reached $n_rawn driver loads, expected $((NSWEEPS * 4)) (4 standard variants per sweep)"

echo "hunt sandbox contract: PASS (battery completes, $NVARIANTS variants loaded in plan order with the right raw/standard mode, rebuild path survives and really rebuilds, summary table with $NVARIANTS rows before the self-tests, evdev touch verdict measured, counter deltas computed, first write survives the window, live readback present, no-panel run warns and degrades honestly, quiet load states its absence and survives, wrapped ring labels the fallback read, no-evdev run falls back to y/n, module reads scoped to the stub sysfs)"
