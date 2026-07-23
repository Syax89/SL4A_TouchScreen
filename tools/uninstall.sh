#!/bin/bash
# ============================================================================
# uninstall.sh — Removes everything tools/install.sh installed:
# the package-owned modprobe.d config and matching DKMS source tree.
# Loaded modules are left intact until reboot.
#
# Usage: sudo ./tools/uninstall.sh
# ============================================================================
set -e

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
MODPROBE_CONF="/etc/modprobe.d/sl4a-spi-hid.conf"

GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'
info() { echo -e "${YELLOW}$1${NC}"; }
pass() { echo -e "${GREEN}OK: $1${NC}"; }

if [ "$EUID" -ne 0 ]; then
	info "Re-running with sudo..."
	exec sudo "$0" "$@"
fi

echo "============================================"
echo " SL4A_TouchScreen driver uninstaller"
echo "============================================"

if [ -f "$MODPROBE_CONF" ]; then
	if grep -q '^# SL4A_TouchScreen' "$MODPROBE_CONF"; then
		info "Removing package-owned modprobe config..."
		rm -f "$MODPROBE_CONF"
		pass "Modprobe config removed"
	else
		info "Leaving unowned $MODPROBE_CONF untouched"
	fi
fi

# Legacy service files are not removed automatically because the current
# installer does not own them. Remove them manually after verifying ownership.

info "Leaving active modules untouched..."
pass "Reboot is required to stop the active driver safely"

SRC_DEST="/usr/src/${PKG_NAME}-${PKG_VERSION}"
info "Removing package-owned DKMS registration $PKG_NAME/$PKG_VERSION..."
if [ -f "$SRC_DEST/dkms.conf" ] && grep -q '^PACKAGE_NAME="sl4a-touch"' "$SRC_DEST/dkms.conf"; then
	if dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all; then
		rm -rf "$SRC_DEST"
		pass "Removed package-owned DKMS version $PKG_VERSION"
	else
		info "DKMS removal failed; leaving $SRC_DEST for recovery"
	fi
elif [ -e "$SRC_DEST" ]; then
	info "Leaving unowned $SRC_DEST untouched"
fi

depmod -a
pass "DKMS removal completed"

echo ""
echo "============================================"
echo -e "${GREEN}Uninstall complete.${NC} Reboot to unload the active driver."
echo "The git repo itself was not touched."
echo "============================================"
