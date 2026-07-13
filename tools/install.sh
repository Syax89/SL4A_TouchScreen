#!/bin/bash
# ============================================================================
# install.sh — Multi-distro installer for the SL4A_TouchScreen driver
# (Surface Laptop 4 AMD touchscreen, MSHW0231).
#
# Builds spi-amd.ko + spi-hid.ko via DKMS (auto-rebuilt on kernel updates),
# installs a systemd unit that loads them at every boot in standard HID mode
# (raw_mode=N — single-touch + pen; see README.md).
#
# Tested on: Arch, CachyOS, Ubuntu/Debian, Fedora, openSUSE.
#
# Usage:
#   sudo ./tools/install.sh
#
# Safe to re-run (rebuilds/reinstalls cleanly). See tools/uninstall.sh.
# ============================================================================
set -e

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
SRC_DEST="/usr/src/${PKG_NAME}-${PKG_VERSION}"
SERVICE_PATH="/etc/systemd/system/sl4a-touch.service"
MODPROBE_CONF="/etc/modprobe.d/spi-hid.conf"

if [[ "$PKG_VERSION" == *-* ]]; then
	echo "FAIL: VERSION ('$PKG_VERSION') contains '-', which breaks Arch/CachyOS's"
	echo "      dkms hook on kernel update (see driver/dkms.conf). Use '~' or no"
	echo "      separator instead (e.g. 1.0.0~beta1) and re-run."
	exit 1
fi

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; CYAN='\033[0;36m'; NC='\033[0m'
info()  { echo -e "${YELLOW}$1${NC}"; }
pass()  { echo -e "${GREEN}OK: $1${NC}"; }
fail()  { echo -e "${RED}FAIL: $1${NC}"; exit 1; }

# ---- distro detection -------------------------------------------------------
ID="unknown"; ID_LIKE=""
if [ -f /etc/os-release ]; then
	. /etc/os-release
fi
# Normalise: map sub-variants to their parent package manager
case "$ID" in
	cachyos|endeavouros|manjaro|arcolinux|garuda|archbang) ID_LIKE="arch" ;;
	rhel|centos|almalinux|rocky|ol)                        ID_LIKE="fedora" ;;
	linuxmint|pop|elementary|zorin|neon)                   ID_LIKE="debian" ;;
esac

kernel_headers_pkg=""; build_deps_pkg=""
case "$ID_LIKE" in
	arch)    kernel_headers_pkg="linux-headers" ;;     # actually a meta-pkg on Arch
	debian)  kernel_headers_pkg="linux-headers-$(uname -r)" ;;
	fedora)  kernel_headers_pkg="kernel-devel" ;;
	suse*|opensuse*) kernel_headers_pkg="kernel-devel" ;;
	*)       kernel_headers_pkg="your kernel headers package" ;; # fallback
esac
case "$ID_LIKE" in
	arch)    build_deps_pkg="dkms make" ;;
	debian)  build_deps_pkg="dkms make linux-headers-$(uname -r)" ;;
	fedora)  build_deps_pkg="dkms make kernel-devel" ;;
	suse*|opensuse*) build_deps_pkg="dkms make kernel-devel" ;;
	*)       build_deps_pkg="dkms make $kernel_headers_pkg" ;;
esac

pkg_manager="your package manager"
case "$ID_LIKE" in
	arch)    pkg_manager="pacman -S" ;;
	debian)  pkg_manager="apt install" ;;
	fedora)  pkg_manager="dnf install" ;;
	suse*|opensuse*) pkg_manager="zypper install" ;;
esac

echo "============================================"
echo " SL4A_TouchScreen driver installer (${PKG_VERSION})"
echo " Surface Laptop 4 (AMD) touchscreen — MSHW0231"
echo " Distro detected: $ID (${ID_LIKE:-unknown})"
echo "============================================"
echo ""
echo "This installs an EXPERIMENTAL out-of-tree kernel driver."
echo "Single-touch + pen are stable; multi-touch is experimental."
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
	if command -v "$cmd" >/dev/null 2>&1; then
		:
	else
		echo "  missing: $cmd"
		MISSING=1
	fi
done
KVER_CONFIG="/lib/modules/$(uname -r)/build/.config"
if [ ! -d "/lib/modules/$(uname -r)/build" ]; then
	echo "  missing: kernel headers/build tree for $(uname -r)"
	echo "           package: $kernel_headers_pkg"
	MISSING=1
elif grep -q '^CONFIG_CC_IS_CLANG=y' "$KVER_CONFIG" 2>/dev/null; then
	if ! command -v clang >/dev/null 2>&1; then
		echo "  missing: clang (kernel $(uname -r) was built with clang)"
		MISSING=1
	fi
else
	if ! command -v gcc >/dev/null 2>&1; then
		echo "  missing: gcc"
		MISSING=1
	fi
fi
if [ "$MISSING" -ne 0 ]; then
	echo ""
	echo -e "${CYAN}Install missing packages with:${NC}"
	echo "  $pkg_manager $build_deps_pkg"
	echo ""
	echo "(Consult your distro's docs if the package names differ.)"
	exit 1
fi
pass "All build dependencies present"

info "Step 3: Stopping and unloading any existing driver..."
systemctl stop sl4a-touch.service 2>/dev/null || true
modprobe -r spi-hid 2>/dev/null || true
modprobe -r spi-amd 2>/dev/null || true
if lsmod | grep -qE '^spi_(hid|amd) '; then
	fail "Existing spi-hid/spi-amd modules are still active; unload their users before reinstalling"
fi
pass "No active driver modules remain"

info "Step 4: Creating modprobe.d config (standard HID mode)..."
cat > "$MODPROBE_CONF" <<'EOF'
# SL4A_TouchScreen — standard HID mode (single-touch + pen, stable)
options spi_hid raw_mode=N
EOF
pass "Created $MODPROBE_CONF"

info "Step 5: Installing driver sources via DKMS ($SRC_DEST)..."
dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all >/dev/null 2>&1 || true
rm -rf "$SRC_DEST"
for stale in /usr/src/${PKG_NAME}-*; do
	[ -d "$stale" ] || continue
	[ "$stale" = "$SRC_DEST" ] && continue
	stale_nv="$(grep -oP '(?<=PACKAGE_VERSION=").*(?=")' "$stale/dkms.conf" 2>/dev/null || true)"
	if [ -n "$stale_nv" ]; then
		dkms remove -m "$PKG_NAME" -v "$stale_nv" --all >/dev/null 2>&1 || true
	fi
	rm -rf "$stale"
done
mkdir -p "$SRC_DEST"
cp -a "$REPO_DIR"/driver/. "$SRC_DEST"/
rm -f "$SRC_DEST"/*.o "$SRC_DEST"/*.ko "$SRC_DEST"/*.mod "$SRC_DEST"/*.mod.c \
      "$SRC_DEST"/*.mod.o "$SRC_DEST"/Module.symvers "$SRC_DEST"/modules.order \
      "$SRC_DEST"/.*.cmd 2>/dev/null || true
sed -i "s/#VERSION#/${PKG_VERSION}/" "$SRC_DEST/dkms.conf"

dkms add -m "$PKG_NAME" -v "$PKG_VERSION"
dkms build -m "$PKG_NAME" -v "$PKG_VERSION"
dkms install -m "$PKG_NAME" -v "$PKG_VERSION"
pass "spi-amd.ko + spi-hid.ko built and installed via DKMS for kernel $(uname -r)"

info "Step 6: Installing systemd service (auto-load at boot)..."
cat > "$SERVICE_PATH" <<'EOF'
[Unit]
Description=Surface Laptop 4 Touchscreen Driver (spi-amd + spi-hid, standard HID mode)
After=local-fs.target

[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/sbin/modprobe spi-hid
ExecStart=/sbin/modprobe spi-amd
ExecStop=/sbin/modprobe -r spi-hid
ExecStop=/sbin/modprobe -r spi-amd

[Install]
WantedBy=multi-user.target
EOF
systemctl daemon-reload
pass "Service installed at $SERVICE_PATH"

info "Step 7: Loading now..."
depmod -a
systemctl enable --now sl4a-touch.service
sleep 2

if lsmod | grep -q '^spi_hid ' && lsmod | grep -q '^spi_amd '; then
	pass "Both modules loaded"
else
	fail "Modules did not load — check: journalctl -u sl4a-touch -b ; dmesg | tail -40"
fi

# Warn about Secure Boot if enforced
if [ -d /sys/firmware/efi ] && mokutil --sb-state 2>/dev/null | grep -qi "SecureBoot enabled"; then
	echo ""
	echo -e "${YELLOW}NOTE: Secure Boot is enabled.${NC}"
	echo "      If the modules fail to load, you may need to enroll a MOK key:"
	echo "      sudo mokutil --import /var/lib/dkms/mok.pub  # (if exists)"
	echo "      Or use your distro's DKMS signing configuration."
fi

echo ""
echo "============================================"
if dmesg | tail -40 | grep -qE "spi_hid.*input.*Touch" 2>/dev/null; then
	echo -e "${GREEN}Install complete — touch and pen should be working now.${NC}"
else
	echo -e "${YELLOW}Install complete. Check touch functionality with evtest:${NC}"
	echo "  sudo evtest /dev/input/event16"
fi
echo " The driver auto-loads on every boot via systemd."
echo " To remove: sudo ./tools/uninstall.sh"
echo "============================================"
