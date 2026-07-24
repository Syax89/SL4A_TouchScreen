#!/bin/bash
# ============================================================================
# sl4a-touch.sh — unified install / uninstall / activate / status / logs /
# rebuild tool for the SL4A_TouchScreen driver (Surface Laptop 4 AMD
# touchscreen, MSHW0231 over AMDI0060).
#
# Replaces the previously separate tools/install.sh, tools/uninstall.sh,
# tools/activate-fch.sh, and tools/rebuild_and_install.sh with one entry
# point. Tested on: Arch, CachyOS, Ubuntu/Debian, Fedora, openSUSE.
#
# Usage:
#   ./tools/sl4a-touch.sh install [--standard|--raw] [--check|--dry-run] [--force]
#   sudo ./tools/sl4a-touch.sh uninstall
#   sudo ./tools/sl4a-touch.sh activate
#   ./tools/sl4a-touch.sh status
#   sudo ./tools/sl4a-touch.sh logs [-o FILE]
#   ./tools/sl4a-touch.sh rebuild            (developer use only, see --help)
#
# Run with no arguments, or -h/--help, for the full command list.
# ============================================================================
set -e -o pipefail
shopt -s nullglob

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DRIVER_DIR="$REPO_DIR/driver"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
SRC_DEST="/usr/src/${PKG_NAME}-${PKG_VERSION}"
MODPROBE_CONF="/etc/modprobe.d/sl4a-spi-hid.conf"
SYSFS_ROOT="${SL4A_SYSFS_ROOT:-/sys}"
DMI_ROOT="${SL4A_DMI_ROOT:-/sys/class/dmi/id}"

CONTROLLER_MODULE="sl4a-spi-amd"
CONTROLLER_DRIVER="sl4a_spi_amd_v2_multi"
HID_MODULE="sl4a-spi-hid"
HID_DRIVER="sl4a_spi_hid"

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; CYAN='\033[0;36m'; BOLD='\033[1m'; NC='\033[0m'
info()  { echo -e "${CYAN}→${NC} $1"; }
pass()  { echo -e "${GREEN}✓${NC} $1"; }
warn()  { echo -e "${YELLOW}⚠${NC}  $1"; }
fail()  { echo -e "${RED}✗ $1${NC}" >&2; exit 1; }

# Section header with a title-length underline (no fixed-width box to keep
# alignment right regardless of terminal width or title length).
header() {
	local title="$1" rule
	rule="$(printf -- '─%.0s' $(seq 1 ${#title}))"
	echo ""
	echo -e "${BOLD}${title}${NC}"
	echo -e "${CYAN}${rule}${NC}"
}

# Re-exec under sudo with a clear, action-specific reason, preserving the
# given args exactly (callers resolve any interactive prompts BEFORE
# calling this, then pass the resolved flags through).
elevate() {
	local reason="$1"; shift
	if [ "$EUID" -ne 0 ]; then
		info "Root is required to $reason. Re-running with sudo..."
		exec sudo "$0" "$@"
	fi
}

# Thin divider for closing a section, with no title to match (see
# header() for the title-length-matched version used at section starts).
rule() { echo -e "${CYAN}$(printf -- '─%.0s' $(seq 1 44))${NC}"; }

if [[ "$PKG_VERSION" == *-* ]]; then
	fail "VERSION ('$PKG_VERSION') contains '-', which breaks Arch/CachyOS's dkms hook on kernel update (see driver/dkms.conf). Use '~' or no separator instead (e.g. 1.0.0~beta1) and re-run."
fi
if [[ "$PKG_VERSION" == *[\|/\\]* ]]; then
	fail "VERSION ('$PKG_VERSION') contains a shell metacharacter (|/\\)."
fi

# ── Shared helpers ──────────────────────────────────────────────────────────

acpi_device_present() {
	compgen -G "$SYSFS_ROOT/bus/acpi/devices/$1:*" >/dev/null
}

bound_driver() {
	[ -L "$1/driver" ] || return 1
	basename "$(readlink -f "$1/driver")"
}

dkms_installed_version() {
	# Real dkms status output looks like:
	#   sl4a-touch/1.2.0, 7.1.3-2-cachyos, x86_64: installed
	# "|| true" throughout: an empty/no-match result is a normal "not
	# installed" outcome here, not a script-ending error under set -e.
	dkms status -m "$PKG_NAME" 2>/dev/null | grep ': installed$' | head -1 | \
		sed -n 's/^sl4a-touch\/\([^,]*\),.*/\1/p' || true
}

modprobe_profile() {
	[ -f "$MODPROBE_CONF" ] || { echo "none"; return; }
	if grep -q 'raw_mode=Y' "$MODPROBE_CONF" 2>/dev/null; then
		echo "raw"
	elif grep -q 'raw_mode=N' "$MODPROBE_CONF" 2>/dev/null; then
		echo "standard"
	else
		echo "unknown"
	fi
}

detect_distro() {
	ID="unknown"; ID_LIKE=""
	if [ -f /etc/os-release ]; then . /etc/os-release; fi
	case "$ID" in
		arch|cachyos|endeavouros|manjaro|arcolinux|garuda|archbang) ID_LIKE="arch" ;;
		debian|ubuntu|linuxmint|pop|elementary|zorin|neon)          ID_LIKE="debian" ;;
		fedora|rhel|centos|almalinux|rocky|ol)                      ID_LIKE="fedora" ;;
		opensuse*|sles)                                              ID_LIKE="suse" ;;
	esac
}

# ── Top-level usage ──────────────────────────────────────────────────────────

usage() {
	cat <<'EOF'
sl4a-touch.sh — SL4A_TouchScreen driver management tool

Usage: tools/sl4a-touch.sh <command> [options]

Commands:
  install [--standard|--raw] [--check|--dry-run] [--force]
                    Build and install the driver via DKMS. Prompts
                    interactively for a profile if none is given on a
                    terminal; defaults to --standard otherwise.
                      --standard  Single-touch + pen. Stable, supported. (default)
                      --raw       EXPERIMENTAL heatmap multitouch. May be
                                  unstable; no hardware-qualified result yet.
                      --check     Validate prerequisites only, write nothing.
                      --dry-run   Validate and print the selected profile.
                      --force     Continue even if expected hardware/DMI is
                                  not detected.

  uninstall         Remove the installed driver and its DKMS registration.
                    Loaded modules are left running until reboot.

  activate          Load and bind the modules for this boot session. Never
                    automatic (by design) — run this after every login/boot
                    you want the touchscreen driver active for. Refuses to
                    displace a device already bound to another driver.

  status            Show the installed DKMS version (if any), whether it
                    matches this checkout, the active profile, and whether
                    the driver is currently loaded and bound.

  logs [-o FILE]    Collect a diagnostic bundle (versions, dkms/modprobe
                    state, driver sysfs stats, filtered dmesg) into a single
                    text file for bug reports. Default output path is
                    printed at the end.

  rebuild           Developer use only: rebuild the .ko files against the
                    running kernel and drop them directly into
                    /lib/modules/$(uname -r)/updates/dkms/, bypassing DKMS.
                    Use this while iterating on driver source; use "install"
                    for anything you want to survive a kernel update.

  -h, --help        Show this help.
EOF
}

if [ $# -eq 0 ] || [ "$1" = "-h" ] || [ "$1" = "--help" ] || [ "$1" = "help" ]; then
	usage
	[ $# -eq 0 ] && exit 1
	exit 0
fi
CMD="$1"; shift

# ── install ──────────────────────────────────────────────────────────────

cmd_install() {
	local MODE="install" PROFILE="" FORCE=0
	for arg in "$@"; do
		case "$arg" in
			--check) MODE="check" ;;
			--dry-run) MODE="dry-run" ;;
			--raw) PROFILE="raw" ;;
			--standard) PROFILE="standard" ;;
			--force) FORCE=1 ;;
			*) fail "unknown install option: $arg (see --help)" ;;
		esac
	done

	detect_distro
	local kernel_headers_pkg="" build_deps_pkg="" pkg_manager="your package manager"
	case "$ID_LIKE" in
		arch)    kernel_headers_pkg="linux-headers"; build_deps_pkg="dkms make"; pkg_manager="pacman -S" ;;
		debian)  kernel_headers_pkg="linux-headers-$(uname -r)"; build_deps_pkg="dkms make linux-headers-$(uname -r)"; pkg_manager="apt install" ;;
		fedora)  kernel_headers_pkg="kernel-devel"; build_deps_pkg="dkms make kernel-devel"; pkg_manager="dnf install" ;;
		suse*|opensuse*) kernel_headers_pkg="kernel-devel"; build_deps_pkg="dkms make kernel-devel"; pkg_manager="zypper install" ;;
		*)       kernel_headers_pkg="your kernel headers package"; build_deps_pkg="dkms make $kernel_headers_pkg" ;;
	esac

	header "SL4A_TouchScreen driver installer — v${PKG_VERSION}"
	echo "Surface Laptop 4 (AMD) touchscreen — MSHW0231"
	echo "Distro detected: $ID (${ID_LIKE:-unknown})"
	echo ""

	# Profile selection: explicit flag wins; otherwise prompt interactively
	# on a terminal, or default to standard (the supported profile) when
	# not interactive, e.g. run from a script or CI.
	if [ -z "$PROFILE" ]; then
		if [ -t 0 ] && [ "$MODE" = "install" ]; then
			echo "Which profile do you want to install?"
			echo ""
			echo -e "  ${GREEN}1) Standard HID${NC}   — single-touch + pen. Stable; this is the"
			echo    "                       supported default. [recommended]"
			echo -e "  ${YELLOW}2) Raw multitouch${NC} — EXPERIMENTAL. Heatmap-based multi-finger"
			echo    "                       tracking. May fail to activate after a cold"
			echo    "                       boot, may be unstable with 3+ fingers, and has"
			echo    "                       no hardware-qualified compatibility result yet."
			echo    "                       Choose this only if you understand it may not"
			echo    "                       work reliably."
			echo ""
			read -r -p "Select [1]: " choice
			case "$choice" in
				2) PROFILE="raw" ;;
				""|1) PROFILE="standard" ;;
				*) fail "unrecognized selection: $choice" ;;
			esac
		else
			PROFILE="standard"
		fi
	fi
	echo -e "Selected profile: ${BOLD}${PROFILE}${NC}"
	echo ""

	info "Step 1: Checking hardware..."
	local missing_hardware=""
	if ! acpi_device_present "MSHW0231"; then missing_hardware="MSHW0231"; fi
	if ! acpi_device_present "AMDI0060"; then missing_hardware="${missing_hardware:+$missing_hardware, }AMDI0060"; fi
	if [ -n "$missing_hardware" ]; then
		if [ "$FORCE" -eq 1 ]; then
			warn "expected ACPI device(s) not found: $missing_hardware. Continuing only because --force was supplied."
		else
			fail "expected Surface Laptop 4 AMD hardware not found: $missing_hardware (use --force to override)"
		fi
	else
		pass "MSHW0231 and AMDI0060 found"
	fi

	if [ -r "$DMI_ROOT/product_name" ]; then
		local product_name
		product_name="$(tr -d '\n' < "$DMI_ROOT/product_name")"
		if [ "$product_name" != "Surface Laptop 4" ]; then
			if [ "$FORCE" -eq 1 ]; then
				warn "expected DMI product Surface Laptop 4, found: $product_name"
			else
				fail "expected DMI product Surface Laptop 4, found: $product_name (use --force to override)"
			fi
		else
			pass "Surface Laptop 4 DMI product found"
		fi
	else
		if [ "$FORCE" -eq 1 ]; then
			warn "DMI product name unavailable; continuing only because --force was supplied"
		else
			fail "DMI product name unavailable; use --force to bypass the Surface Laptop 4 check"
		fi
	fi

	info "Step 2: Checking build dependencies..."
	local MISSING=0
	for cmd in dkms make; do
		command -v "$cmd" >/dev/null 2>&1 || { echo "  missing: $cmd"; MISSING=1; }
	done
	local KVER_CONFIG="/lib/modules/$(uname -r)/build/.config"
	if [ ! -d "/lib/modules/$(uname -r)/build" ]; then
		echo "  missing: kernel headers/build tree for $(uname -r)"
		echo "           package: $kernel_headers_pkg"
		MISSING=1
	elif grep -q '^CONFIG_CC_IS_CLANG=y' "$KVER_CONFIG" 2>/dev/null; then
		command -v clang >/dev/null 2>&1 || { echo "  missing: clang (kernel $(uname -r) was built with clang)"; MISSING=1; }
	else
		command -v gcc >/dev/null 2>&1 || { echo "  missing: gcc"; MISSING=1; }
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
		return 0
	fi
	if [ "$MODE" = "dry-run" ]; then
		pass "Dry run passed; would install the $PROFILE profile"
		return 0
	fi

	# Re-exec with the already-resolved profile (not the original "$@"),
	# so a profile picked at the interactive prompt above survives the
	# switch to sudo instead of prompting a second time under the child
	# process.
	elevate "build and install kernel modules, and write /etc/modprobe.d config" \
		install "--$PROFILE" $([ "$FORCE" -eq 1 ] && echo --force)

	if [ -e "$MODPROBE_CONF" ] && ! grep -q '^# SL4A_TouchScreen' "$MODPROBE_CONF"; then
		fail "refusing to replace unowned $MODPROBE_CONF"
	fi

	info "Step 3: Leaving active modules untouched..."
	pass "Active modules left untouched; nothing is loaded or bound until 'activate' is run"

	info "Step 4: Staging driver sources via DKMS ($SRC_DEST)..."
	local profile_only=0
	if [ -e "$SRC_DEST" ]; then
		if dkms status -m "$PKG_NAME" -v "$PKG_VERSION" 2>/dev/null | grep -q "installed"; then
			if grep -q '^obj-m += sl4a-spi-amd.o$' "$SRC_DEST/Kbuild" && \
			   grep -q '^obj-m += sl4a-spi-hid.o$' "$SRC_DEST/Kbuild"; then
				info "DKMS version $PKG_NAME/$PKG_VERSION is already installed; updating only the $PROFILE profile"
				profile_only=1
			else
				info "Replacing the package's legacy spi-amd artifact with the opt-in controller module..."
				dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all || fail "could not remove the package's legacy DKMS artifact"
				rm -rf "$SRC_DEST"
			fi
		else
			fail "DKMS source for $PKG_NAME/$PKG_VERSION already exists from an incomplete install; remove that exact version before retrying"
		fi
	fi

	cleanup_staged_install() {
		dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all >/dev/null 2>&1 || true
		rm -rf "$SRC_DEST"
	}

	if [ "$profile_only" -eq 0 ]; then
		mkdir -p "$SRC_DEST"
		cp -a "$DRIVER_DIR"/. "$SRC_DEST"/
		rm -f "$SRC_DEST"/*.o "$SRC_DEST"/*.ko "$SRC_DEST"/*.mod "$SRC_DEST"/*.mod.c \
		      "$SRC_DEST"/*.mod.o "$SRC_DEST"/Module.symvers "$SRC_DEST"/modules.order \
		      "$SRC_DEST"/.*.cmd 2>/dev/null || true
		rm -f "$SRC_DEST/test_harness.c" "$SRC_DEST/sl4a-touch.service" "$SRC_DEST/sl4a-touch-load.sh" 2>/dev/null || true
		sed -i "s|#VERSION#|${PKG_VERSION}|" "$SRC_DEST/dkms.conf"

		dkms add -m "$PKG_NAME" -v "$PKG_VERSION" || { cleanup_staged_install; fail "DKMS add failed; existing driver state was left unchanged"; }
		dkms build -m "$PKG_NAME" -v "$PKG_VERSION" || { cleanup_staged_install; fail "DKMS build failed; existing driver state was left unchanged"; }
		dkms install -m "$PKG_NAME" -v "$PKG_VERSION" || { cleanup_staged_install; fail "DKMS install failed; existing driver state was left unchanged"; }
		pass "sl4a-spi-amd.ko + sl4a-spi-hid.ko built and installed via DKMS for kernel $(uname -r)"

		info "Step 5: Updating module dependencies..."
		depmod -a
		pass "Module dependencies updated"
	fi

	info "Step 6: Activating $PROFILE profile..."
	local tmp_config
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

	if [ -d /sys/firmware/efi ] && mokutil --sb-state 2>/dev/null | grep -qi "SecureBoot enabled"; then
		echo ""
		warn "Secure Boot is enabled."
		echo "      If the modules fail to load, you may need to enroll a MOK key:"
		echo "      sudo mokutil --import /var/lib/dkms/mok.pub  # (if exists)"
		echo "      Or use your distro's DKMS signing configuration."
	fi

	echo ""
	rule
	if [ "$PROFILE" = "raw" ]; then
		echo -e "${YELLOW}${BOLD}Install complete${NC} ${YELLOW}— EXPERIMENTAL raw multitouch profile selected.${NC}"
	else
		echo -e "${GREEN}${BOLD}Install complete${NC} ${GREEN}— standard HID profile selected.${NC}"
	fi
	echo "Nothing is loaded or bound yet."
	echo "  After login, run: sudo ./tools/sl4a-touch.sh activate"
	echo "  To remove:         sudo ./tools/sl4a-touch.sh uninstall"
	rule
}

# ── uninstall ────────────────────────────────────────────────────────────

cmd_uninstall() {
	elevate "remove the DKMS registration and /etc/modprobe.d config" uninstall "$@"

	header "SL4A_TouchScreen driver uninstaller"

	if [ -f "$MODPROBE_CONF" ]; then
		if grep -q '^# SL4A_TouchScreen' "$MODPROBE_CONF"; then
			info "Removing package-owned modprobe config..."
			rm -f "$MODPROBE_CONF"
			pass "Modprobe config removed"
		else
			info "Leaving unowned $MODPROBE_CONF untouched"
		fi
	fi

	info "Leaving active modules untouched..."
	pass "Reboot is required to stop the active driver safely"

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
	rule
	echo -e "${GREEN}${BOLD}Uninstall complete.${NC} Reboot to unload the active driver."
	echo "The git repo itself was not touched."
	rule
}

# ── activate ─────────────────────────────────────────────────────────────

cmd_activate() {
	elevate "load kernel modules and bind them to the touch hardware" activate "$@"

	header "SL4A_TouchScreen driver activation"
	local controller_loaded=0 hid_loaded=0

	wait_for_driver() {
		local device="$1" expected="$2" attempt
		for attempt in {1..150}; do
			if [ "$(bound_driver "$device" 2>/dev/null)" = "$expected" ]; then
				return 0
			fi
			sleep 0.1
		done
		return 1
	}
	rollback() {
		[ "$hid_loaded" -eq 1 ] && modprobe -r "$HID_MODULE" 2>/dev/null || true
		[ "$controller_loaded" -eq 1 ] && modprobe -r "$CONTROLLER_MODULE" 2>/dev/null || true
	}
	fail_rollback() { rollback; fail "$1; modules loaded by this command were rolled back"; }

	if command -v mokutil >/dev/null 2>&1 && mokutil --sb-state 2>/dev/null | grep -qi 'SecureBoot enabled'; then
		if ! mokutil --test-key /var/lib/dkms/mok.pub >/dev/null 2>&1; then
			cat >&2 <<'EOF'
WARNING: Secure Boot is enabled but the DKMS MOK key is not enrolled.
Signed modules will be rejected by the kernel.
Enroll the key once:  sudo mokutil --import /var/lib/dkms/mok.pub
See docs/ROLLBACK.md for the complete MOK enrollment procedure.
After enrollment, reboot before running this command again.
EOF
			exit 1
		fi
	fi

	local controllers=(/sys/bus/acpi/devices/AMDI0060:*)
	[ "${#controllers[@]}" -eq 1 ] || fail "expected exactly one AMDI0060 ACPI device"
	local controller="${controllers[0]}"
	local controller_platform="/sys/bus/platform/devices/$(basename "$controller")"
	[ -d "$controller_platform" ] || fail "AMDI0060 platform device is absent"
	local touches=(/sys/bus/acpi/devices/MSHW0231:*)
	[ "${#touches[@]}" -eq 1 ] || fail "expected exactly one MSHW0231 ACPI device"
	local extra_mshw=(/sys/bus/acpi/devices/MSHW*:*)
	if [ "${#extra_mshw[@]}" -gt 1 ]; then
		local extra_list
		extra_list=$(printf '%s\n' "${extra_mshw[@]}" | grep -v "MSHW0231" | xargs -r -n1 basename | tr '\n' ' ')
		[ -n "$extra_list" ] && fail "extra MSHW ACPI devices found beyond MSHW0231: ${extra_list}activation refused"
	fi
	local touch="${touches[0]}"

	if [ -L "$controller_platform/driver" ]; then
		fail "AMDI0060 is already bound to $(bound_driver "$controller_platform"); refusing to displace it"
	fi
	if [ -L "$touch/physical_node/driver" ]; then
		fail "MSHW0231 is already bound to $(bound_driver "$touch/physical_node"); refusing to displace it"
	fi

	[ ! -d "/sys/module/${CONTROLLER_MODULE//-/_}" ] && controller_loaded=1
	modprobe "$CONTROLLER_MODULE" || fail_rollback "could not load experimental controller"
	wait_for_driver "$controller_platform" "$CONTROLLER_DRIVER" || fail_rollback "experimental controller did not bind"

	[ ! -d "/sys/module/${HID_MODULE//-/_}" ] && hid_loaded=1
	modprobe "$HID_MODULE" || fail_rollback "could not load HID transport"
	wait_for_driver "$touch/physical_node" "$HID_DRIVER" || fail_rollback "MSHW0231 did not bind to the HID transport"

	pass "AMDI0060 and MSHW0231 are bound"
	echo "Recovery: sudo modprobe -r sl4a-spi-hid sl4a-spi-amd; reboot."
}

# ── status ───────────────────────────────────────────────────────────────

cmd_status() {
	header "SL4A_TouchScreen driver status"
	echo "(read-only — no root required)"
	echo ""
	echo "Repository checkout version: $PKG_VERSION"

	local installed
	installed="$(dkms_installed_version)"
	if [ -n "$installed" ]; then
		if [ "$installed" = "$PKG_VERSION" ]; then
			pass "DKMS installed version: $installed (matches this checkout)"
		else
			warn "DKMS installed version: $installed (this checkout is $PKG_VERSION — run 'install' to upgrade)"
		fi
	else
		info "Not installed via DKMS (run 'install' to install)"
	fi

	local profile
	profile="$(modprobe_profile)"
	case "$profile" in
		raw)      warn "Active modprobe profile: raw (EXPERIMENTAL multitouch)" ;;
		standard) pass "Active modprobe profile: standard HID" ;;
		none)     info "No modprobe profile configured (nothing installed)" ;;
		*)        warn "Active modprobe profile: unrecognized contents in $MODPROBE_CONF" ;;
	esac

	echo ""
	echo "Hardware:"
	if acpi_device_present "MSHW0231" && acpi_device_present "AMDI0060"; then
		pass "  MSHW0231 and AMDI0060 present"
	else
		warn "  Expected ACPI devices not found — this may not be a Surface Laptop 4 AMD"
	fi

	echo ""
	echo "Runtime state:"
	if [ -d "/sys/module/${HID_MODULE//-/_}" ]; then
		pass "  $HID_MODULE is loaded"
		local touches=(/sys/bus/acpi/devices/MSHW0231:*)
		if [ "${#touches[@]}" -eq 1 ]; then
			local bound
			bound="$(bound_driver "${touches[0]}/physical_node" 2>/dev/null || true)"
			if [ "$bound" = "$HID_DRIVER" ]; then
				pass "  MSHW0231 is bound to $HID_DRIVER"
			else
				info "  MSHW0231 is not bound to $HID_DRIVER yet — run 'activate'"
			fi
		fi
	else
		info "  $HID_MODULE is not loaded — run 'activate' after login to load it"
	fi
	if [ -d "/sys/module/${CONTROLLER_MODULE//-/_}" ]; then
		pass "  $CONTROLLER_MODULE is loaded"
	else
		info "  $CONTROLLER_MODULE is not loaded"
	fi

	echo ""
	echo "Kernel: $(uname -r)"
}

# ── logs ─────────────────────────────────────────────────────────────────

cmd_logs() {
	local OUT=""
	while [ $# -gt 0 ]; do
		case "$1" in
			-o|--output) OUT="$2"; shift 2 ;;
			*) fail "unknown logs option: $1 (see --help)" ;;
		esac
	done
	elevate "read the kernel log (dmesg)" logs ${OUT:+-o "$OUT"}
	[ -n "$OUT" ] || OUT="$REPO_DIR/sl4a-touch-diagnostics-$(date +%Y%m%d-%H%M%S).txt"

	header "SL4A_TouchScreen diagnostic collection"
	info "Writing to $OUT..."

	{
		echo "=== SL4A_TouchScreen diagnostic bundle ==="
		echo "Generated: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
		echo ""

		echo "--- System ---"
		uname -a
		[ -f /etc/os-release ] && cat /etc/os-release
		echo ""

		echo "--- Repository ---"
		echo "Checkout version: $PKG_VERSION"
		if [ -d "$REPO_DIR/.git" ]; then
			git -C "$REPO_DIR" rev-parse HEAD 2>/dev/null | sed 's/^/git HEAD: /'
			git -C "$REPO_DIR" status --short 2>/dev/null | sed 's/^/git status: /'
		fi
		echo ""

		echo "--- Hardware ---"
		acpi_device_present "MSHW0231" && echo "MSHW0231: present" || echo "MSHW0231: NOT FOUND"
		acpi_device_present "AMDI0060" && echo "AMDI0060: present" || echo "AMDI0060: NOT FOUND"
		[ -r "$DMI_ROOT/product_name" ] && echo "DMI product: $(tr -d '\n' < "$DMI_ROOT/product_name")"
		echo ""

		echo "--- DKMS ---"
		dkms status 2>&1 | grep -i sl4a || echo "(no sl4a-touch DKMS registration found)"
		echo ""

		echo "--- modprobe config ($MODPROBE_CONF) ---"
		[ -f "$MODPROBE_CONF" ] && cat "$MODPROBE_CONF" || echo "(not present)"
		echo ""

		echo "--- Loaded modules ---"
		lsmod | grep -i sl4a || echo "(not loaded)"
		echo ""

		echo "--- Module parameters ---"
		for p in /sys/module/sl4a_spi_hid/parameters/*; do
			[ -f "$p" ] && echo "$(basename "$p") = $(cat "$p" 2>/dev/null)"
		done 2>/dev/null

		echo ""
		echo "--- Driver sysfs stats (if bound) ---"
		local touches=(/sys/bus/acpi/devices/MSHW0231:*)
		if [ "${#touches[@]}" -eq 1 ]; then
			local dev="/sys/bus/platform/devices/$(basename "$(dirname "${touches[0]}")" 2>/dev/null)"
			local spidev
			spidev=$(find /sys/devices -maxdepth 6 -path '*spi-MSHW0231:00' -type d 2>/dev/null | head -1)
			if [ -n "$spidev" ]; then
				for f in lifecycle_status seq_state protocol_stats baseline_status; do
					if [ -r "$spidev/$f" ]; then
						echo "-- $f --"
						cat "$spidev/$f"
					fi
				done
			else
				echo "(spi-MSHW0231:00 sysfs node not found — driver not bound)"
			fi
		fi

		echo ""
		echo "--- Secure Boot ---"
		if command -v mokutil >/dev/null 2>&1; then
			mokutil --sb-state 2>&1
		else
			echo "(mokutil not available)"
		fi

		echo ""
		echo "--- dmesg (driver-related lines) ---"
		dmesg | grep -iE "sl4a|MSHW0231|AMDI0060" | tail -300
	} > "$OUT"

	chmod 644 "$OUT" 2>/dev/null || true
	pass "Diagnostic bundle written to: $OUT"
	echo "Attach this file when reporting an issue."
}

# ── rebuild (developer use only) ────────────────────────────────────────

cmd_rebuild() {
	header "SL4A_TouchScreen — developer rebuild ($(date '+%Y-%m-%d %H:%M'))"
	info "This bypasses DKMS and installs directly into /lib/modules/$(uname -r)/updates/dkms/."
	info "For anything meant to survive a kernel update, use 'install' instead."
	echo ""

	info "Step 1/2: Building sl4a-spi-amd.ko + sl4a-spi-hid.ko for kernel $(uname -r)..."
	local MAKE_LLVM=""
	grep -q '^CONFIG_CC_IS_CLANG=y' "/lib/modules/$(uname -r)/build/.config" 2>/dev/null && MAKE_LLVM="LLVM=1"
	make ${MAKE_LLVM} -C "/lib/modules/$(uname -r)/build" M="$DRIVER_DIR" modules
	[ -f "$DRIVER_DIR/sl4a-spi-amd.ko" ] && [ -f "$DRIVER_DIR/sl4a-spi-hid.ko" ] || fail "Build did not produce sl4a-spi-amd.ko/sl4a-spi-hid.ko"
	pass "Build succeeded (no root needed for this step)"

	info "Step 2/2: Copying modules into /lib/modules/$(uname -r)/updates/dkms/..."
	local SUDO=""
	if [ "$EUID" -ne 0 ]; then
		info "Root is required to write under /lib/modules. Using sudo for this step only..."
		SUDO="sudo"
	fi
	$SUDO mkdir -p "/lib/modules/$(uname -r)/updates/dkms"
	$SUDO cp -f "$DRIVER_DIR/sl4a-spi-amd.ko" "$DRIVER_DIR/sl4a-spi-hid.ko" "/lib/modules/$(uname -r)/updates/dkms/"
	$SUDO depmod -a
	pass "Modules installed"

	echo ""
	rule
	echo "Reload with 'sudo modprobe -r sl4a-spi-hid sl4a-spi-amd' then"
	echo "'./tools/sl4a-touch.sh activate' to pick up the rebuilt modules"
	echo "(or reboot)."
	rule
}

case "$CMD" in
	install)   cmd_install "$@" ;;
	uninstall) cmd_uninstall "$@" ;;
	activate)  cmd_activate "$@" ;;
	status)    cmd_status "$@" ;;
	logs)      cmd_logs "$@" ;;
	rebuild)   cmd_rebuild "$@" ;;
	*) echo "Unknown command: $CMD"; echo ""; usage; exit 1 ;;
esac
