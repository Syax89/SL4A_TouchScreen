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
printf '# SL4A_TouchScreen\noptions sl4a_spi_hid raw_mode=Y raw_input_beta=Y\n' \
	> "$SB/etc/sl4a-spi-hid.conf"
printf '# SL4A_TouchScreen\n' > "$SB/etc/sl4a-touch-activate.service"
: > "$SB/dmesg.txt"
for i in $(seq 1 200); do
	printf '[%d.0] fake line %d\n[%d.1] sl4a_spi_hid: stub %d\n' "$i" "$i" "$i" "$i" >> "$SB/dmesg.txt"
done

# `sleep` is stubbed so the sweep takes a second instead of 33; `make` so
# run_host_self_tests cannot re-enter this very suite.
for c in dkms systemctl depmod mokutil make modinfo sleep; do
	printf '#!/bin/bash\nexit 0\n' > "$SB/bin/$c"
	chmod +x "$SB/bin/$c"
done
# modprobe records its arguments: the sweep must hand the controller's
# debug_trace to sl4a_spi_amd, or the peek line — the one that answers the
# RX-region question — can never appear in the artifact.
printf '#!/bin/bash\necho "$*" >> "%s/modprobe.log"\nexit 0\n' "$SB" > "$SB/bin/modprobe"
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

# The wire-form axis is the one the artifact has to explain: if the sweep does
# not actually load all three shapes, the bundle cannot say which one answered.
grep -q 'acpi_probe_power_cycle=1 skip_vendor_stop=0' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the power-cycle arm"
grep -q 'acpi_probe_power_cycle=0 skip_vendor_stop=1' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the skip-preamble arm"
grep -q 'acpi_probe_power_cycle=1 skip_vendor_stop=1' "$SB/modprobe.log" \
	|| fail "the sweep never loaded the combined arm"
grep -q 'acpi_probe_power_cycle=1 skip_vendor_stop=1' "$SB/out.txt" \
	|| fail "the artifact never names the probe variant it ran"

# The progress the user asked for has to be on the terminal too, not only in
# the file — that is the whole point of it.
grep -q '\[1/4\] variant 0' "$SB/run.txt" || fail "no per-variant progress on the terminal"
grep -q 'TOUCH THE PANEL NOW' "$SB/run.txt" || fail "no touch prompt on the terminal"

# A stale stamp must take the rebuild path (it is the path that once died with
# 'command not found'), and the sweep must survive it.
echo "0000000000000000000000000000000000000000" > "$SB/var/installed-head"
PATH="$SB/bin:$PATH" bash "$SB/tool.sh" hunt -o "$SB/out2.txt" > "$SB/run2.txt" 2>&1
rc=$?
[ "$rc" -eq 0 ] || { sed -n '1,40p' "$SB/run2.txt"; fail "hunt exited $rc with a stale stamp (the rebuild path)"; }
grep -q 'rebuilding first' "$SB/run2.txt" || fail "a stale stamp did not trigger a rebuild"
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
[ "$(grep -c 'NO COUNTERS READ' "$SB/out3.txt" || true)" -eq 4 ] \
	|| fail "the no-panel artifact does not degrade honestly to NO COUNTERS READ"

echo "hunt sandbox contract: PASS (sweep completes, rebuild path survives, 4 verdicts, progress on the terminal, controller debug_trace passed, no-panel run warns)"
