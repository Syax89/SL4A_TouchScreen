#!/bin/bash
# ============================================================================
# uninstall.sh — Removes everything tools/install.sh installed:
# the modprobe.d config, the DKMS-registered source tree (for every kernel it
# was built against), and any leftover systemd service from older versions.
# Loaded modules are left intact until reboot.
#
# Usage: sudo ./tools/uninstall.sh
# ============================================================================
set -e

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
SERVICE_PATH="/etc/systemd/system/sl4a-touch.service"
LOAD_SCRIPT_PATH="/usr/lib/sl4a-touch/load-touch.sh"
MODPROBE_CONF="/etc/modprobe.d/spi-hid.conf"

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

info "Removing modprobe config..."
rm -f "$MODPROBE_CONF"
pass "Modprobe config removed"

# Clean up legacy systemd service from older versions (no longer installed).
if [ -f "$SERVICE_PATH" ]; then
	info "Removing legacy systemd service..."
	systemctl disable --now sl4a-touch.service 2>/dev/null || true
	rm -f "$SERVICE_PATH"
	rm -rf "$(dirname "$LOAD_SCRIPT_PATH")"
	systemctl daemon-reload
	pass "Legacy service removed"
fi

info "Leaving active modules untouched..."
pass "Reboot is required to stop the active driver safely"

info "Removing DKMS registration ($PKG_NAME/$PKG_VERSION, all kernels)..."
dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all 2>/dev/null || true
rm -rf "/usr/src/${PKG_NAME}-${PKG_VERSION}"

for stale in /usr/src/${PKG_NAME}-*; do
	[ -d "$stale" ] || continue
	stale_nv="$(grep -oP '(?<=PACKAGE_VERSION=").*(?=")' "$stale/dkms.conf" 2>/dev/null || true)"
	if [ -n "$stale_nv" ]; then
		dkms remove -m "$PKG_NAME" -v "$stale_nv" --all 2>/dev/null || true
	fi
	rm -rf "$stale"
done

depmod -a
pass "DKMS source tree removed"

echo ""
echo "============================================"
echo -e "${GREEN}Uninstall complete.${NC} Reboot to unload the active driver."
echo "The git repo itself was not touched."
echo "============================================"
