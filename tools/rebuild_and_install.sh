#!/bin/bash
# ============================================================================
# rebuild_and_install.sh — Rebuild sl4a-spi-amd.ko/sl4a-spi-hid.ko against the running
# kernel. The controller module is never reloaded while the system is running.
#
# Use this after pulling driver changes, or after a kernel update (out-of-tree
# modules are built against a specific kernel version and won't load anymore
# once the kernel is upgraded until rebuilt).
#
# Usage: ./tools/rebuild_and_install.sh
#   The modprobe.d config controls boot-time parameters.
# ============================================================================
set -e -o pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
REPO_DIR="$(dirname "$SCRIPT_DIR")"
DRIVER_DIR="$REPO_DIR/driver"

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; NC='\033[0m'
info() { echo -e "${YELLOW}$1${NC}"; }
pass() { echo -e "${GREEN}OK: $1${NC}"; }
fail() { echo -e "${RED}FAIL: $1${NC}"; exit 1; }

echo "============================================"
echo " SL4A Touchscreen — rebuild"
echo " $(date)"
echo "============================================"

info "Step 1: Building sl4a-spi-amd.ko + sl4a-spi-hid.ko for kernel $(uname -r)..."
MAKE_LLVM=""
grep -q '^CONFIG_CC_IS_CLANG=y' "/lib/modules/$(uname -r)/build/.config" 2>/dev/null && MAKE_LLVM="LLVM=1"
make ${MAKE_LLVM} -C "/lib/modules/$(uname -r)/build" M="$DRIVER_DIR" modules
[ -f "$DRIVER_DIR/sl4a-spi-amd.ko" ] && [ -f "$DRIVER_DIR/sl4a-spi-hid.ko" ] || fail "Build did not produce sl4a-spi-amd.ko/sl4a-spi-hid.ko"
pass "Build succeeded"

info "Step 2: Copying modules to updates/dkms..."
sudo cp -f "$DRIVER_DIR/sl4a-spi-amd.ko" "$DRIVER_DIR/sl4a-spi-hid.ko" "/lib/modules/$(uname -r)/updates/dkms/"
sudo depmod -a
pass "Modules installed — reboot to load"

info "Step 3: Leaving the active driver untouched..."
pass "Build complete; reboot to load the new modules"

echo ""
echo "============================================"
echo " Reboot required before the rebuilt modules are loaded."
echo "============================================"
