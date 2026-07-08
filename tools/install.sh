#!/bin/bash
# ============================================================================
# install.sh — Beta installer for the SL4A_TouchScreen driver
# (Surface Laptop 4 AMD touchscreen, MSHW0231).
#
# Builds spi-amd.ko + spi-hid.ko via DKMS (so they're automatically rebuilt on
# every kernel update, not just the one you have right now), then installs a
# systemd unit that loads them at every boot in the stable, default
# configuration (raw_mode=0 — single-touch + pen; see README.md for what's
# experimental vs. stable).
#
# Usage:
#   sudo ./tools/install.sh
#
# Safe to re-run (rebuilds/reinstalls cleanly). See tools/uninstall.sh to
# remove everything this script installs.
# ============================================================================
set -e

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0-beta1")"
SRC_DEST="/usr/src/${PKG_NAME}-${PKG_VERSION}"
SERVICE_PATH="/etc/systemd/system/sl4a-touch.service"

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; NC='\033[0m'
info() { echo -e "${YELLOW}$1${NC}"; }
pass() { echo -e "${GREEN}OK: $1${NC}"; }
fail() { echo -e "${RED}FAIL: $1${NC}"; exit 1; }

echo "============================================"
echo " SL4A_TouchScreen driver installer (${PKG_VERSION})"
echo " Surface Laptop 4 (AMD) touchscreen — MSHW0231"
echo "============================================"
echo ""
echo "This installs an EXPERIMENTAL/BETA out-of-tree kernel driver."
echo "Single-touch + pen are stable; multi-touch is not (see README.md)."
echo ""

if [ "$EUID" -ne 0 ]; then
	info "Re-running with sudo..."
	exec sudo "$0" "$@"
fi

info "Step 1: Checking hardware..."
if ! ls /sys/bus/acpi/devices/ 2>/dev/null | grep -qi "MSHW0231"; then
	echo -e "${RED}WARNING: no MSHW0231 ACPI device found on this system.${NC}"
	echo "This driver is specifically for the Surface Laptop 4 (AMD) touchscreen."
	echo "Installing it on other hardware will do nothing useful."
	read -rp "Continue anyway? [y/N] " ans
	[[ "$ans" =~ ^[Yy]$ ]] || exit 1
else
	pass "MSHW0231 found"
fi

info "Step 2: Checking build dependencies..."
MISSING=0
for cmd in dkms make; do
	command -v "$cmd" >/dev/null 2>&1 || { echo "  missing: $cmd"; MISSING=1; }
done
if [ ! -d "/lib/modules/$(uname -r)/build" ]; then
	echo "  missing: kernel headers/build tree for $(uname -r)"
	echo "           (package name varies by distro: linux-headers, linux-<flavor>-headers, ...)"
	MISSING=1
fi
if ! command -v clang >/dev/null 2>&1; then
	echo "  missing: clang (this driver requires LLVM=1 — a plain gcc build will fail)"
	MISSING=1
fi
if [ "$MISSING" -ne 0 ]; then
	fail "Install the missing dependencies above, then re-run this script."
fi
pass "All build dependencies present"

info "Step 3: Installing driver sources via DKMS ($SRC_DEST)..."
dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all >/dev/null 2>&1 || true
rm -rf "$SRC_DEST"
mkdir -p "$SRC_DEST"
cp -a "$REPO_DIR"/driver/. "$SRC_DEST"/
# Drop any local build artifacts that shouldn't ship with the DKMS source tree.
rm -f "$SRC_DEST"/*.o "$SRC_DEST"/*.ko "$SRC_DEST"/*.mod "$SRC_DEST"/*.mod.c "$SRC_DEST"/*.mod.o \
      "$SRC_DEST"/Module.symvers "$SRC_DEST"/modules.order "$SRC_DEST"/.*.cmd 2>/dev/null || true
sed -i "s/#VERSION#/${PKG_VERSION}/" "$SRC_DEST/dkms.conf"

dkms add -m "$PKG_NAME" -v "$PKG_VERSION"
dkms build -m "$PKG_NAME" -v "$PKG_VERSION"
dkms install -m "$PKG_NAME" -v "$PKG_VERSION"
pass "spi-amd.ko + spi-hid.ko built and installed via DKMS for kernel $(uname -r)"

info "Step 4: Installing systemd service (auto-load at boot, standard mode)..."
cat > "$SERVICE_PATH" <<'EOF'
[Unit]
Description=Surface Laptop 4 Touchscreen Driver (spi-amd + spi-hid, standard HID mode)
After=local-fs.target

[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/sbin/modprobe spi-amd
ExecStart=/sbin/modprobe spi-hid
ExecStop=/sbin/modprobe -r spi-hid
ExecStop=/sbin/modprobe -r spi-amd

[Install]
WantedBy=multi-user.target
EOF
systemctl daemon-reload
pass "Service installed at $SERVICE_PATH"

info "Step 5: Loading now..."
systemctl stop sl4a-touch.service 2>/dev/null || true
depmod -a
systemctl enable --now sl4a-touch.service
sleep 2

if lsmod | grep -q '^spi_hid ' && lsmod | grep -q '^spi_amd '; then
	pass "Both modules loaded"
else
	fail "Modules did not load — check: journalctl -u sl4a-touch -b ; dmesg | tail -40"
fi

echo ""
echo "============================================"
if dmesg | tail -40 | grep -qE "hid-generic.*SPI HID"; then
	echo -e "${GREEN}Install complete — touch and pen should be working now.${NC}"
else
	echo -e "${YELLOW}Install complete, but hid-generic hasn't bound yet.${NC}"
	echo "Give it a few seconds, then check: sudo dmesg | tail -40"
fi
echo " The driver will now load automatically on every boot."
echo " To remove everything this script installed: sudo ./tools/uninstall.sh"
echo "============================================"
