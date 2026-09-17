#!/bin/bash
# One-off bisect driver for the 2026-09-17 raw regression: on this panel
# v1.5.0 answers (the raw handshake completes) and v1.6.3 reset-loops.
# This walks v1.5.0..v1.6.3, building and probing each step on THIS machine
# and prints the first bad commit.
#
#   sudo bash tools/bisect-raw-regression.sh
#
# It works in a scratch clone (the checkout is untouched), takes ~7 builds
# (~20-30 min), and reinstalls the current driver from the checkout at the
# end. Send back the log file it prints.
set -u
SRC="$(cd "$(dirname "$0")/.." && pwd)"
WORK="${SL4A_BISECT_DIR:-/tmp/sl4a-bisect}"
CHECK="$WORK/check.sh"
LOG="$WORK/bisect.log"

[ "$(id -u)" = 0 ] || { echo "run me with sudo: the check builds and loads kernel modules"; exit 1; }
command -v dkms >/dev/null || { echo "dkms is missing"; exit 1; }

# git as root over the user's trees
git config --global --add safe.directory "$SRC" 2>/dev/null || true
git config --global --add safe.directory "$WORK/repo" 2>/dev/null || true

rm -rf "$WORK"; mkdir -p "$WORK"
git clone -q "$SRC" "$WORK/repo" || { echo "clone failed"; exit 1; }

cat > "$CHECK" <<'EOS'
#!/bin/bash
# bisect step: 0 = the raw handshake works, 1 = broken, 125 = cannot build.
set -u
cd "$(dirname "$0")/repo" || exit 125
# Revisions before 6e094d6 carry `bool current;`, which collides with the
# kernel's get_current() macro on 7.2.x and fails the build: apply that
# commit's rename so every step can compile. No-op where it is already in.
sed -i \
  -e 's/\tbool current;/\tbool owned;/' \
  -e 's/\tcurrent = shid->output_pending/\towned = shid->output_pending/' \
  -e 's/\tif (!current) {/\tif (!owned) {/' \
  driver/spi-hid-core.c 2>/dev/null || true
./tools/sl4a-touch.sh install --raw >/dev/null 2>&1 || exit 125
sleep 12
D="$(ls -d /sys/bus/spi/devices/*MSHW* 2>/dev/null | head -1)"
dd="$(sed -n 's/^device_desc=//p' "$D/protocol_stats" 2>/dev/null | head -1)"
da="$(sed -n 's/^data=//p'        "$D/protocol_stats" 2>/dev/null | head -1)"
rr="$(sed -n 's/^reset_rsp=//p'   "$D/protocol_stats" 2>/dev/null | head -1)"
echo "probe $(git rev-parse --short HEAD): device_desc=${dd:-?} data=${da:-?} reset_rsp=${rr:-?}" | tee -a /tmp/sl4a-bisect-probes.log
[ "${dd:-0}" -gt 0 ] 2>/dev/null && exit 0
[ "${da:-0}" -gt 0 ] 2>/dev/null && exit 0
exit 1
EOS
chmod +x "$CHECK"
: > /tmp/sl4a-bisect-probes.log

cd "$WORK/repo" || exit 1
echo "bisect: v1.5.0 (good) .. v1.6.3 (bad), in $WORK/repo"
git bisect start v1.6.3 v1.5.0
git bisect run bash "$CHECK" 2>&1 | tee "$LOG"
git bisect log >> "$LOG" 2>&1 || true
git bisect reset >/dev/null 2>&1 || true

echo
echo "=== RESULT ==="
grep -E "device_desc=" /tmp/sl4a-bisect-probes.log || true
grep -E "^# first bad commit" "$LOG" || true
echo
echo "Restoring the current driver from $SRC ..."
( cd "$SRC" && ./tools/sl4a-touch.sh install --raw >/dev/null 2>&1 ) && echo "restored" || echo "RESTORE FAILED - reinstall by hand"
echo "Send $LOG (it carries the per-step probes and the first bad commit)."
