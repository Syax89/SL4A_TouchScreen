#!/bin/bash
# ============================================================================
# uninstall.sh — Removes everything tools/install.sh installed:
# the systemd service, the loaded modules, and the DKMS-registered source
# tree (for every kernel it was built against).
#
# Usage: sudo ./tools/uninstall.sh
# ============================================================================
set -e

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
SERVICE_PATH="/etc/systemd/system/sl4a-touch.service"

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

info "Stopping and disabling the systemd service..."
systemctl disable --now sl4a-touch.service 2>/dev/null || true
rm -f "$SERVICE_PATH"
systemctl daemon-reload
pass "Service removed"

info "Unloading modules (if still loaded)..."
modprobe -r spi_hid 2>/dev/null || true
modprobe -r spi_amd 2>/dev/null || true
pass "Modules unloaded"

info "Removing DKMS registration ($PKG_NAME/$PKG_VERSION, all kernels)..."
dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all 2>/dev/null || true
rm -rf "/usr/src/${PKG_NAME}-${PKG_VERSION}"

# Also sweep any leftovers from a *previous* version's source tree (e.g. from a mid-beta
# VERSION bump) -- found live 2026-07-08: a stray /usr/src/sl4a-touch-1.0.0-beta1 directory
# from before a hotfix kept getting rediscovered and rebuilt (under the wrong name) by
# Arch/CachyOS's pacman dkms hook on every later kernel update, since that hook scans
# /usr/src/*-*/dkms.conf directly instead of only trusting `dkms status`.
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
echo -e "${GREEN}Uninstall complete.${NC} The git repo itself was not touched."
echo "============================================"
