#!/bin/bash
# ============================================================================
# install.sh — Multi-distro installer for the SL4A_TouchScreen driver
# (Surface Laptop 4 AMD touchscreen, MSHW0231).
#
# Builds sl4a-spi-amd.ko + sl4a-spi-hid.ko via DKMS (auto-rebuilt on kernel updates),
# and configures a standard HID profile by default. Raw multi-touch requires an
# explicit --raw opt-in until its hardware validation matrix is complete.
# The experimental FCH controller is never auto-loaded or bound at boot.
#
# Tested on: Arch, CachyOS, Ubuntu/Debian, Fedora, openSUSE.
#
# Usage:
#   ./tools/install.sh --check
#   sudo ./tools/install.sh [--raw] [--force]
#
# Safe to re-run for profile changes. Rebuilding an installed version requires
# a new VERSION or removal of that exact version first. See tools/uninstall.sh.
# ============================================================================
set -e -o pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
SRC_DEST="/usr/src/${PKG_NAME}-${PKG_VERSION}"
MODPROBE_CONF="/etc/modprobe.d/sl4a-spi-hid.conf"
SYSFS_ROOT="${SL4A_SYSFS_ROOT:-/sys}"
DMI_ROOT="${SL4A_DMI_ROOT:-/sys/class/dmi/id}"
MODE="install"
PROFILE="standard"
FORCE=0

if [[ "$PKG_VERSION" == *-* ]]; then
	echo "FAIL: VERSION ('$PKG_VERSION') contains '-', which breaks Arch/CachyOS's"
	echo "      dkms hook on kernel update (see driver/dkms.conf). Use '~' or no"
	echo "      separator instead (e.g. 1.0.0~beta1) and re-run."
	exit 1
fi
if [[ "$PKG_VERSION" == *[|/\\]* ]]; then
	echo "FAIL: VERSION ('$PKG_VERSION') contains a shell metacharacter (|/\\)."
	exit 1
fi

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; CYAN='\033[0;36m'; NC='\033[0m'
info()  { echo -e "${YELLOW}$1${NC}"; }
pass()  { echo -e "${GREEN}OK: $1${NC}"; }
fail()  { echo -e "${RED}FAIL: $1${NC}"; exit 1; }

usage() {
	cat <<'EOF'
Usage: tools/install.sh [--check|--dry-run] [--raw] [--force]

  --check    Validate hardware and build prerequisites without writing files.
  --dry-run  Validate prerequisites and print the selected install profile.
  --raw      Install the experimental raw heatmap profile.
  --force    Continue when the expected ACPI hardware is not detected.
EOF
}

for arg in "$@"; do
	case "$arg" in
		--check) MODE="check" ;;
		--dry-run) MODE="dry-run" ;;
		--raw) PROFILE="raw" ;;
		--force) FORCE=1 ;;
		--help|-h) usage; exit 0 ;;
		*) fail "unknown option: $arg" ;;
	esac
done

# ---- distro detection -------------------------------------------------------
ID="unknown"; ID_LIKE=""
if [ -f /etc/os-release ]; then
	. /etc/os-release
fi
case "$ID" in
	arch|cachyos|endeavouros|manjaro|arcolinux|garuda|archbang) ID_LIKE="arch" ;;
	debian|ubuntu|linuxmint|pop|elementary|zorin|neon)          ID_LIKE="debian" ;;
	fedora|rhel|centos|almalinux|rocky|ol)                      ID_LIKE="fedora" ;;
	opensuse*|sles)                                              ID_LIKE="suse" ;;
esac

kernel_headers_pkg=""; build_deps_pkg=""
case "$ID_LIKE" in
	arch)    kernel_headers_pkg="linux-headers" ;;
	debian)  kernel_headers_pkg="linux-headers-$(uname -r)" ;;
	fedora)  kernel_headers_pkg="kernel-devel" ;;
	suse*|opensuse*) kernel_headers_pkg="kernel-devel" ;;
	*)       kernel_headers_pkg="your kernel headers package" ;;
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
echo "The selected profile is: $PROFILE"
echo ""

info "Step 1: Checking hardware..."
acpi_device_present() {
	compgen -G "$SYSFS_ROOT/bus/acpi/devices/$1:*" >/dev/null
}

missing_hardware=""
if ! acpi_device_present "MSHW0231"; then
	missing_hardware="MSHW0231"
fi
if ! acpi_device_present "AMDI0060"; then
	missing_hardware="${missing_hardware:+$missing_hardware, }AMDI0060"
fi
if [ -n "$missing_hardware" ]; then
	if [ "$FORCE" -eq 1 ]; then
		info "WARNING: expected ACPI device(s) not found: $missing_hardware"
		info "Continuing only because --force was supplied."
	else
		fail "expected Surface Laptop 4 AMD hardware not found: $missing_hardware (use --force to override)"
	fi
else
	pass "MSHW0231 and AMDI0060 found"
fi

if [ -r "$DMI_ROOT/product_name" ]; then
	product_name="$(tr -d '\n' < "$DMI_ROOT/product_name")"
	if [ "$product_name" != "Surface Laptop 4" ]; then
		if [ "$FORCE" -eq 1 ]; then
			info "WARNING: expected DMI product Surface Laptop 4, found: $product_name"
		else
			fail "expected DMI product Surface Laptop 4, found: $product_name (use --force to override)"
		fi
	else
		pass "Surface Laptop 4 DMI product found"
	fi
else
	if [ "$FORCE" -eq 1 ]; then
		info "WARNING: DMI product name unavailable; continuing only because --force was supplied"
	else
		fail "DMI product name unavailable; use --force to bypass the Surface Laptop 4 check"
	fi
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

if [ "$MODE" = "check" ]; then
	pass "Preflight passed; no files were modified"
	exit 0
fi

if [ "$MODE" = "dry-run" ]; then
	pass "Dry run passed; would install the $PROFILE profile"
	exit 0
fi

if [ "$EUID" -ne 0 ]; then
	info "Re-running with sudo..."
	exec sudo "$0" "$@"
fi

if [ -e "$MODPROBE_CONF" ] && ! grep -q '^# SL4A_TouchScreen' "$MODPROBE_CONF"; then
	fail "refusing to replace unowned $MODPROBE_CONF"
fi

info "Step 3: Leaving active modules untouched..."
pass "Active modules left untouched; the FCH controller stays inactive until explicit activation"

info "Step 4: Staging driver sources via DKMS ($SRC_DEST)..."
if [ -e "$SRC_DEST" ]; then
	if dkms status -m "$PKG_NAME" -v "$PKG_VERSION" 2>/dev/null | grep -q "installed"; then
		if grep -q '^obj-m += sl4a-spi-amd.o$' "$SRC_DEST/Kbuild" && \
		   grep -q '^obj-m += sl4a-spi-hid.o$' "$SRC_DEST/Kbuild"; then
			info "DKMS version $PKG_NAME/$PKG_VERSION is already installed; updating only the $PROFILE profile"
			profile_only=1
		else
			info "Replacing the package's legacy spi-amd artifact with the opt-in controller module..."
			if ! dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all; then
				fail "could not remove the package's legacy DKMS artifact"
			fi
			rm -rf "$SRC_DEST"
			profile_only=0
		fi
	else
		fail "DKMS source for $PKG_NAME/$PKG_VERSION already exists from an incomplete install; remove that exact version before retrying"
	fi
else
	profile_only=0
fi

cleanup_staged_install() {
	dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all >/dev/null 2>&1 || true
	rm -rf "$SRC_DEST"
}

if [ "$profile_only" -eq 0 ]; then
mkdir -p "$SRC_DEST"
cp -a "$REPO_DIR"/driver/. "$SRC_DEST"/
rm -f "$SRC_DEST"/*.o "$SRC_DEST"/*.ko "$SRC_DEST"/*.mod "$SRC_DEST"/*.mod.c \
      "$SRC_DEST"/*.mod.o "$SRC_DEST"/Module.symvers "$SRC_DEST"/modules.order \
      "$SRC_DEST"/.*.cmd 2>/dev/null || true

# Remove test harness and service files from the DKMS source—they're not
# kernel modules and would trigger build warnings.
rm -f "$SRC_DEST/test_harness.c" "$SRC_DEST/sl4a-touch.service" "$SRC_DEST/sl4a-touch-load.sh" 2>/dev/null || true
sed -i "s|#VERSION#|${PKG_VERSION}|" "$SRC_DEST/dkms.conf"

if ! dkms add -m "$PKG_NAME" -v "$PKG_VERSION"; then
	cleanup_staged_install
	fail "DKMS add failed; existing driver state was left unchanged"
fi
if ! dkms build -m "$PKG_NAME" -v "$PKG_VERSION"; then
	cleanup_staged_install
	fail "DKMS build failed; existing driver state was left unchanged"
fi
if ! dkms install -m "$PKG_NAME" -v "$PKG_VERSION"; then
	cleanup_staged_install
	fail "DKMS install failed; existing driver state was left unchanged"
fi
pass "sl4a-spi-amd.ko + sl4a-spi-hid.ko built and installed via DKMS for kernel $(uname -r)"

info "Step 5: Updating module dependencies..."
depmod -a
pass "Module dependencies updated"
fi

info "Step 6: Activating $PROFILE profile..."
tmp_config="$(mktemp "${MODPROBE_CONF}.XXXXXX")"
if [ "$PROFILE" = "raw" ]; then
	cat > "$tmp_config" <<'EOF'
# SL4A_TouchScreen experimental raw heatmap profile
options sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y
EOF
else
	cat > "$tmp_config" <<'EOF'
# SL4A_TouchScreen standard HID profile
options sl4a_spi_hid raw_mode=N
EOF
fi
install -m 0644 "$tmp_config" "$MODPROBE_CONF"
rm -f "$tmp_config"
pass "Created $MODPROBE_CONF"

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
echo -e "${GREEN}Install complete. The experimental FCH controller remains inactive.${NC}"
echo " After login, use: sudo ./tools/activate-fch.sh"
echo " To remove: sudo ./tools/uninstall.sh"
echo "============================================"
