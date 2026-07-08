#!/bin/bash
# ============================================================================
# rebuild_and_install.sh — Rebuild spi-amd.ko/spi-hid.ko against the running
# kernel and reload them via the sl4a-touch systemd service.
#
# Use this after pulling driver changes, or after a kernel update (out-of-tree
# modules are built against a specific kernel version and won't load anymore
# once the kernel is upgraded until rebuilt).
#
# Usage: ./tools/rebuild_and_install.sh [raw_mode=1 debug_coords=1 ...]
#   Any extra arguments are passed straight through as insmod parameters for
#   spi-hid.ko (e.g. to test raw_mode=1) — the systemd service itself always
#   loads with no parameters (raw_mode=0, the stable default), so passing
#   parameters here only affects this one manual run, not future boots.
# ============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_DIR="$(dirname "$SCRIPT_DIR")"
DRIVER_DIR="$REPO_DIR/driver"
SERVICE_SRC="$DRIVER_DIR/sl4a-touch.service"
SERVICE_DST="/etc/systemd/system/sl4a-touch.service"

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; NC='\033[0m'
info() { echo -e "${YELLOW}$1${NC}"; }
pass() { echo -e "${GREEN}OK: $1${NC}"; }
fail() { echo -e "${RED}FAIL: $1${NC}"; exit 1; }

EXTRA_PARAMS="$*"

echo "============================================"
echo " SL4A Touchscreen — rebuild and install"
echo " $(date)"
echo "============================================"

info "Step 1: Building spi-amd.ko + spi-hid.ko for kernel $(uname -r)..."
make LLVM=1 -C "/lib/modules/$(uname -r)/build" M="$DRIVER_DIR" modules \
	2>&1 | grep -E "error:|Error" && fail "Build failed" || true
[ -f "$DRIVER_DIR/spi-amd.ko" ] && [ -f "$DRIVER_DIR/spi-hid.ko" ] || fail "Build did not produce spi-amd.ko/spi-hid.ko"
pass "Build succeeded"

info "Step 2: Syncing sl4a-touch.service (in case it changed)..."
if ! sudo cmp -s "$SERVICE_SRC" "$SERVICE_DST" 2>/dev/null; then
	sudo cp "$SERVICE_SRC" "$SERVICE_DST"
	sudo systemctl daemon-reload
	pass "Service file updated"
else
	pass "Service file unchanged"
fi

info "Step 3: Reloading the driver via systemd (stop -> start)..."
sudo systemctl stop sl4a-touch.service 2>/dev/null || true
sleep 1

if [ -n "$EXTRA_PARAMS" ]; then
	# Manual one-off load with extra module params, bypassing the service
	# (the service itself never takes parameters, so this only affects
	# this run — reboot or a plain `systemctl start` reverts to raw_mode=0).
	info "Loading manually with extra params: $EXTRA_PARAMS"
	sudo insmod "$DRIVER_DIR/spi-amd.ko"
	sleep 1
	sudo insmod "$DRIVER_DIR/spi-hid.ko" $EXTRA_PARAMS
else
	sudo systemctl start sl4a-touch.service
fi
sleep 2

info "Step 4: Verifying..."
if lsmod | grep -q '^spi_hid ' && lsmod | grep -q '^spi_amd '; then
	pass "Both modules loaded"
else
	fail "Modules did not load — check: sudo dmesg | tail -40"
fi

if sudo dmesg | tail -30 | grep -qE "hid-generic.*SPI HID"; then
	pass "hid-generic bound — touch should be working"
else
	info "hid-generic bind not seen in the last 30 dmesg lines yet — check manually: sudo dmesg | tail -40"
fi

echo ""
echo "============================================"
echo " Done. Recent kernel log:"
echo "============================================"
sudo dmesg | tail -15
