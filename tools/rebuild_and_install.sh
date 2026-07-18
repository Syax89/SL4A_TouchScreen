#!/bin/bash
# ============================================================================
# rebuild_and_install.sh — Rebuild spi-amd.ko/spi-hid.ko against the running
# kernel. The controller module is never reloaded while the system is running.
#
# Use this after pulling driver changes, or after a kernel update (out-of-tree
# modules are built against a specific kernel version and won't load anymore
# once the kernel is upgraded until rebuilt).
#
# Usage: ./tools/rebuild_and_install.sh [raw_mode=1 debug_coords=1 ...]
#   Any extra arguments are passed straight through as insmod parameters for
#   spi-hid.ko.  The modprobe.d config controls boot-time parameters;
#   passing parameters here only affects this one manual run, not future boots.
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

if [ -n "$EXTRA_PARAMS" ]; then
	fail "Extra module parameters require a cold reboot; configure them in /etc/modprobe.d first"
fi

info "Step 3: Leaving the active driver untouched..."
pass "Build complete; reboot to load the new controller module"

echo ""
echo "============================================"
echo " Reboot required before the rebuilt modules are loaded."
echo "============================================"
