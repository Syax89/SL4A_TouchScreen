#!/bin/bash
# ============================================================================
# sl4a-touch.sh — unified install / uninstall / activate / status / logs /
# rebuild tool for the SL4A_TouchScreen driver (Surface Laptop 3/4 AMD
# touchscreen, MSHW0231 / MSHW0162 over AMDI0060).
#
# Replaces the previously separate tools/install.sh, tools/uninstall.sh,
# tools/activate-fch.sh, and tools/rebuild_and_install.sh with one entry
# point. Tested on: Arch, CachyOS, Ubuntu/Debian, Fedora, openSUSE.
#
# Usage:
#   ./tools/sl4a-touch.sh                              interactive arrow-key menu
#   sudo ./tools/sl4a-touch.sh install [--standard|--raw] [--check|--dry-run] [--force]
#   sudo ./tools/sl4a-touch.sh uninstall
#   sudo ./tools/sl4a-touch.sh activate
#   ./tools/sl4a-touch.sh status
#   sudo ./tools/sl4a-touch.sh logs [-o FILE]
#   ./tools/sl4a-touch.sh rebuild            (developer use only, see --help)
#
# install builds, installs via DKMS, enables a systemd unit that
# auto-activates on every future boot (after multi-user.target — i.e.
# after the base system, not during early kernel boot), and activates
# immediately. Nothing here requires typing a subcommand: run the script
# with no arguments for an arrow-key menu. Run with -h/--help for the
# full command list.
# ============================================================================
set -e -o pipefail
shopt -s nullglob

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DRIVER_DIR="$REPO_DIR/driver"
PKG_NAME="sl4a-touch"
PKG_VERSION="$(cat "$REPO_DIR/VERSION" 2>/dev/null || echo "1.0.0~beta1")"
SRC_DEST="/usr/src/${PKG_NAME}-${PKG_VERSION}"
MODPROBE_CONF="/etc/modprobe.d/sl4a-spi-hid.conf"
SYSTEMD_UNIT="/etc/systemd/system/sl4a-touch-activate.service"
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

# Echo the touchscreen ACPI ID (MSHW0231 on SL4, MSHW0162 on SL3 AMD),
# or nothing (and non-zero) if neither is present.
touchscreen_acpi_id() {
	local id
	for id in MSHW0231 MSHW0162; do
		if acpi_device_present "$id"; then
			echo "$id"
			return 0
		fi
	done
	return 1
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

# Remove every DKMS registration of this package except $1 (empty = all of them),
# together with its staged source tree. An upgrade used to leave the previous
# version registered: both then built the same sl4a-spi-amd.ko/sl4a-spi-hid.ko
# names, and `dkms autoinstall` installed whichever ran last on the next kernel
# update, so an older revision could silently become the one that loads.
dkms_remove_other_versions() {
	local keep="$1" line ver src
	dkms status -m "$PKG_NAME" 2>/dev/null | while IFS= read -r line; do
		# Both real shapes parse here: "sl4a-touch/1.6.1, 6.12, x86_64:
		# installed" and the source-only "sl4a-touch/1.6.1: added" (no comma —
		# the old "%%," kept "1.6.1: added", so every later `dkms remove -v`
		# failed on it). A legacy DKMS 2.x line ("sl4a-touch, 1.6.1, ...:
		# installed") matches nothing and is skipped rather than acted on.
		ver="$(printf '%s\n' "$line" | sed -n "s|^${PKG_NAME}/\([^,: ]*\)[,: ].*|\1|p")"
		case "$ver" in
			''|*[!A-Za-z0-9.+~_-]*) continue ;;
		esac
		[ "$ver" = "$keep" ] && continue
		src="/usr/src/${PKG_NAME}-${ver}"
		info "Removing stale DKMS registration $PKG_NAME/$ver..."
		if dkms remove -m "$PKG_NAME" -v "$ver" --all >/dev/null 2>&1; then
			# Drop the tree only when DKMS let go of it and the tree is
			# this package's (same ownership marker the uninstall path uses).
			if [ -f "$src/dkms.conf" ] && grep -qE '^PACKAGE_NAME="sl4a-touch"[[:space:]]*$' "$src/dkms.conf"; then
				rm -rf "$src"
			else
				info "Leaving unowned $src untouched"
			fi
		else
			warn "DKMS removal of $PKG_NAME/$ver failed; $PKG_NAME/$ver stays registered and can win the next kernel update (remove it by hand: sudo dkms remove -m $PKG_NAME -v $ver --all)"
		fi
	done || true
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
                    Build, install via DKMS, enable automatic activation on
                    every future boot (a systemd unit gated on
                    multi-user.target — after the base system is up, not
                    during early kernel boot), and activate immediately.
                    Nothing further to run. Prompts interactively for a
                    profile if none is given on a terminal; defaults to
                    --standard otherwise.
                      --standard  Single-touch + pen. Stable, supported. (default)
                      --raw       EXPERIMENTAL heatmap multitouch. May be
                                  unstable; no hardware-qualified result yet.
                      --check     Validate prerequisites only, write nothing.
                      --dry-run   Validate and print the selected profile.
                      --force     Continue even if expected hardware/DMI is
                                  not detected.

  uninstall         Remove the installed driver, its DKMS registration, and
                    the boot-activation service. Loaded modules are left
                    running until reboot.

  activate          Load and bind the modules right now. install already
                    sets this up to happen automatically on every future
                    boot; use this command directly only to redo it
                    immediately (e.g. after 'rebuild', or to retry after
                    fixing a Secure Boot key enrollment). Refuses to
                    displace a device already bound to another driver.

  status            Show the installed DKMS version (if any), whether it
                    matches this checkout, the active profile, and whether
                    the driver is currently loaded and bound.

  logs [-o FILE]    Collect a diagnostic bundle (versions, dkms/modprobe
                    state, driver sysfs stats, the last captured frame as raw
                    bytes, filtered dmesg) into a single text file for bug
                    reports. Default output path is printed at the end.

  rebuild           Developer use only: rebuild the .ko files against the
                    running kernel and drop them directly into
                    /lib/modules/$(uname -r)/updates/dkms/, bypassing DKMS.
                    Use this while iterating on driver source; use "install"
                    for anything you want to survive a kernel update.

  -h, --help        Show this help.
EOF
}

# Arrow-key menu shown when the script is run with no arguments on a real
# terminal — no subcommand to remember, just run the script. Prints the
# chosen command name on stdout; caller captures it. Falls back to plain
# usage text when stdin isn't a tty (piped/scripted invocation).
menu_pick_command() {
	local labels=("Install" "Uninstall" "Activate" "Status" "Collect diagnostics (logs)" "Quit")
	local cmds=("install" "uninstall" "activate" "status" "logs" "")
	local selected=0 n=${#labels[@]} key rest
	# draw() below prints exactly this many lines every time: title +
	# subtitle + one blank line + one line per option. Cursor-up must
	# move by exactly this count each redraw, or successive frames drift
	# and the terminal scrolls instead of redrawing in place.
	local menu_lines=$((n + 3))

	tput civis >&2 2>/dev/null || true
	trap 'tput cnorm >&2 2>/dev/null || true' RETURN

	draw() {
		echo "SL4A_TouchScreen driver management" >&2
		echo "(up/down arrows, Enter to select)" >&2
		echo "" >&2
		for i in "${!labels[@]}"; do
			if [ "$i" -eq "$selected" ]; then
				echo -e "  ${CYAN}❯ ${BOLD}${labels[$i]}${NC}" >&2
			else
				echo "    ${labels[$i]}" >&2
			fi
		done
	}

	draw
	while true; do
		IFS= read -rsn1 key
		if [ "$key" = $'\x1b' ]; then
			IFS= read -rsn2 -t 0.01 rest || true
			case "$rest" in
				'[A') selected=$(( (selected - 1 + n) % n )) ;;
				'[B') selected=$(( (selected + 1) % n )) ;;
			esac
		elif [ -z "$key" ]; then
			break
		fi
		tput cuu "$menu_lines" >&2 2>/dev/null || true
		tput ed >&2 2>/dev/null || true
		draw
	done
	tput cnorm >&2 2>/dev/null || true
	echo "" >&2
	echo "${cmds[$selected]}"
}

if [ $# -eq 0 ]; then
	if [ -t 0 ]; then
		CMD="$(menu_pick_command)"
		[ -z "$CMD" ] && exit 0
	else
		usage
		exit 1
	fi
elif [ "$1" = "-h" ] || [ "$1" = "--help" ] || [ "$1" = "help" ]; then
	usage
	exit 0
else
	CMD="$1"; shift
fi

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
	echo "Surface Laptop 3/4 (AMD) touchscreen — MSHW0231 / MSHW0162"
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
	# SL4 uses MSHW0231, SL3 (AMD) uses MSHW0162 — either is fine, AMDI0060 is required.
	if ! acpi_device_present "MSHW0231" && ! acpi_device_present "MSHW0162"; then
		missing_hardware="MSHW0231/MSHW0162"
	fi
	if ! acpi_device_present "AMDI0060"; then missing_hardware="${missing_hardware:+$missing_hardware, }AMDI0060"; fi
	if [ -n "$missing_hardware" ]; then
		if [ "$FORCE" -eq 1 ]; then
			warn "expected ACPI device(s) not found: $missing_hardware. Continuing only because --force was supplied."
		else
			fail "expected Surface Laptop 3/4 AMD hardware not found: $missing_hardware (use --force to override)"
		fi
	else
		pass "MSHW0231/MSHW0162 and AMDI0060 found"
	fi

	if [ -r "$DMI_ROOT/product_name" ]; then
		local product_name
		product_name="$(tr -d '\n' < "$DMI_ROOT/product_name")"
		if [ "$product_name" != "Surface Laptop 4" ] && [ "$product_name" != "Surface Laptop 3" ]; then
			if [ "$FORCE" -eq 1 ]; then
				warn "expected DMI product Surface Laptop 3/4, found: $product_name"
			else
				fail "expected DMI product Surface Laptop 3/4, found: $product_name (use --force to override)"
			fi
		else
			pass "Surface Laptop 3/4 DMI product found"
		fi
	else
		if [ "$FORCE" -eq 1 ]; then
			warn "DMI product name unavailable; continuing only because --force was supplied"
		else
			fail "DMI product name unavailable; use --force to bypass the Surface Laptop 3/4 check"
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
	if command -v mokutil >/dev/null 2>&1 && mokutil --sb-state 2>/dev/null | grep -qi 'SecureBoot enabled'; then
		command -v openssl >/dev/null 2>&1 || { echo "  missing: openssl (needed to generate the DKMS signing key for Secure Boot)"; MISSING=1; }
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

	local skip_activate=0

	info "Step 2.5: Checking Secure Boot signing key..."
	if command -v mokutil >/dev/null 2>&1 && mokutil --sb-state 2>/dev/null | grep -qi 'SecureBoot enabled'; then
		if [ ! -f /var/lib/dkms/mok.pub ]; then
			warn "Secure Boot is enabled but the DKMS signing key is missing."
			info "Generating the signing key..."
			# mokutil requires a DER-encoded certificate.
			if dkms generate_mok 2>/dev/null && [ -f /var/lib/dkms/mok.pub ]; then
				pass "DKMS signing key generated at /var/lib/dkms/mok.pub"
			else
				info "'dkms generate_mok' did not produce the key. Generating it manually with openssl..."
				openssl req -new -x509 -nodes -days 36500 -subj "/CN=SL4A_TouchScreen DKMS MOK/" \
					-newkey rsa:2048 -keyout /var/lib/dkms/mok.key -outform DER -out /var/lib/dkms/mok.pub 2>/dev/null || \
					fail "Could not generate the DKMS signing key. Install 'openssl' and retry."
				pass "DKMS signing key generated at /var/lib/dkms/mok.pub"
			fi
		elif ! openssl x509 -in /var/lib/dkms/mok.pub -inform DER -noout 2>/dev/null; then
			warn "Existing DKMS signing key at /var/lib/dkms/mok.pub is not DER-encoded; mokutil cannot import it."
			info "Re-encoding the existing certificate as DER (no new key pair is generated)..."
			command -v openssl >/dev/null 2>&1 || \
				fail "openssl is required to re-encode the DKMS signing key as DER (mokutil only accepts DER). Install 'openssl' and retry."
			openssl x509 -in /var/lib/dkms/mok.pub -out /var/lib/dkms/mok.pub.der -outform DER 2>/dev/null \
				&& mv /var/lib/dkms/mok.pub.der /var/lib/dkms/mok.pub \
				|| fail "Could not re-encode the existing signing key as DER."
			pass "DKMS signing key re-encoded as DER at /var/lib/dkms/mok.pub"
		else
			pass "DKMS signing key found at /var/lib/dkms/mok.pub"
		fi

		info "Step 2.6: Checking MOK enrollment status..."
		local mok_test_output
		mok_test_output="$(mokutil --test-key /var/lib/dkms/mok.pub 2>&1 || true)"
		if echo "$mok_test_output" | grep -qi "already enrolled"; then
			pass "The MOK key is already enrolled — modules will load immediately"
		else
			warn "The MOK key is NOT yet enrolled. The kernel will refuse to load the signed modules until it is."
			skip_activate=1
			echo ""

			if [ -t 0 ]; then
				echo "╔══════════════════════════════════════════════════════════════╗"
				echo "║  SECURE BOOT KEY ENROLLMENT REQUIRED                        ║"
				echo "║──────────────────────────────────────────────────────────────║"
				echo "║  The driver is installed but the kernel needs to trust the   ║"
				echo "║  signing key before the modules can load. This takes two     ║"
				echo "║  simple steps (do step 1 now, step 2 at the next boot).      ║"
				echo "║                                                              ║"
				echo "║  Step 1 of 2 — do this NOW:                                  ║"
				echo "║    You will set a temporary password.                        ║"
				echo "║    You need it only ONCE, at the next boot.                  ║"
				echo "╚══════════════════════════════════════════════════════════════╝"
				echo ""
				read -r -p "Enroll the key now? [Y/n]: " enroll_choice
				echo ""
				case "$enroll_choice" in
					[nN]*)
						echo "╔══════════════════════════════════════════════════════════════╗"
						echo "║  SKIPPED — the driver will NOT load until you enroll the    ║"
						echo "║  key. When you are ready:                                    ║"
						echo "║                                                              ║"
						echo "║    sudo mokutil --import /var/lib/dkms/mok.pub              ║"
						echo "║    sudo reboot                                               ║"
						echo "║                                                              ║"
						echo "║  At the blue MOK Manager screen after reboot:               ║"
						echo "║    Enroll MOK → Continue → Yes → enter password → Reboot     ║"
						echo "║                                                              ║"
						echo "║  After login, the driver activates automatically.            ║"
						echo "╚══════════════════════════════════════════════════════════════╝"
						;;
					*)
						if mokutil --import /var/lib/dkms/mok.pub; then
							pass "Key staged for enrollment."
							echo ""
							echo "╔══════════════════════════════════════════════════════════════╗"
							echo "║  Step 2 of 2 — do this RIGHT NOW:                           ║"
							echo "║──────────────────────────────────────────────────────────────║"
							echo "║                                                              ║"
							echo "║  1. REBOOT                                                     ║"
							echo "║      sudo reboot                                              ║"
							echo "║                                                              ║"
							echo "║  2. BLUE SCREEN (MOK Manager)                                  ║"
							echo "║      A blue screen appears BEFORE your operating system       ║"
							echo "║      loads. THIS IS NORMAL. DO NOT PANIC. DO NOT SKIP IT.     ║"
							echo "║      (If you miss it, it reappears at the next boot.)         ║"
							echo "║                                                              ║"
							echo "║  3. Enroll MOK                                                ║"
							echo "║      Use the KEYBOARD (touch and mouse may not work here).    ║"
							echo "║      Select: 'Enroll MOK'                                     ║"
							echo "║      Then:    'Continue'                                      ║"
							echo "║      Then:    'Yes'                                           ║"
							echo "║                                                              ║"
							echo "║  4. PASSWORD                                                   ║"
							echo "║      Enter the password you set in step 1.                    ║"
							echo "║                                                              ║"
							echo "║  5. REBOOT                                                     ║"
							echo "║      Select: 'Reboot'                                         ║"
							echo "║                                                              ║"
							echo "║  6. DONE                                                       ║"
							echo "║      After login, the driver activates automatically.         ║"
							echo "║      Nothing else to do. Verify with:                         ║"
							echo "║        ./tools/sl4a-touch.sh status                           ║"
							echo "║                                                              ║"
							echo "║  ──────────────────────────────────────────────────────       ║"
							echo "║  IN SHORT: reboot → blue screen → Enroll MOK → password →    ║"
							echo "║  reboot → done.                                              ║"
							echo "╚══════════════════════════════════════════════════════════════╝"
						else
							fail "Key enrollment was not completed (mokutil exited non-zero). No changes were made."
						fi
						;;
				esac
			else
				echo "╔══════════════════════════════════════════════════════════════╗"
				echo "║  SECURE BOOT KEY ENROLLMENT REQUIRED                        ║"
				echo "║──────────────────────────────────────────────────────────────║"
				echo "║  The driver is installed but the signing key must be         ║"
				echo "║  enrolled before it can load. Run these commands:            ║"
				echo "║                                                              ║"
				echo "║    sudo mokutil --import /var/lib/dkms/mok.pub              ║"
				echo "║    sudo reboot                                               ║"
				echo "║                                                              ║"
				echo "║  At the blue MOK Manager screen after reboot:               ║"
				echo "║    Enroll MOK → Continue → Yes → enter password → Reboot     ║"
				echo "║                                                              ║"
				echo "║  After login, the driver activates automatically.            ║"
				echo "║  Full guide: docs/ROLLBACK.md                                ║"
				echo "╚══════════════════════════════════════════════════════════════╝"
			fi
		fi
	else
		pass "Secure Boot is disabled or mokutil is not available — no key enrollment needed"
	fi

	info "Step 3: Staging driver sources via DKMS ($SRC_DEST)..."

	cleanup_staged_install() {
		dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all >/dev/null 2>&1 || true
		# Same ownership rule as the two removal paths: never delete a tree
		# that carries someone else's dkms.conf. A partial copy of our own
		# staging has no dkms.conf yet and stays safe to drop.
		if [ -f "$SRC_DEST/dkms.conf" ] && \
		   ! grep -qE '^PACKAGE_NAME="sl4a-touch"[[:space:]]*$' "$SRC_DEST/dkms.conf"; then
			info "Leaving unowned $SRC_DEST untouched"
			return 0
		fi
		rm -rf "$SRC_DEST"
	}

	# Upgrades: drop any other version of this package first, or the old
	# registration keeps building the same module names on every kernel update.
	dkms_remove_other_versions "$PKG_VERSION"

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
			# Left behind by an interrupted run: recoverable, not a dead end.
			info "Cleaning up an incomplete staging of $PKG_NAME/$PKG_VERSION..."
			cleanup_staged_install
		fi
	fi

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

		info "Step 4: Updating module dependencies..."
		depmod -a
		pass "Module dependencies updated"
	fi

	info "Step 5: Writing the $PROFILE profile to $MODPROBE_CONF..."
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

	# Auto-activate on every future boot via a systemd unit that runs AFTER
	# multi-user.target — i.e. after the base system is already up, not
	# during early kernel/initrd boot. This is the fix for the exact
	# incident CHANGELOG.md's "Boot safety (black-screen fix)" describes:
	# that incident came from kernel-level auto-binding via ACPI/SPI module
	# aliases, which runs WHILE the kernel is still bringing the system up,
	# with no shell and no recovery if the driver hangs. A systemd unit
	# gated on multi-user.target runs well after that point — if activation
	# ever hangs or fails here, you already have a working login and a
	# shell to fix it with, which is the actual safety property that
	# matters, not "never load automatically at all."
	info "Step 6: Enabling automatic activation on every future boot..."
	tmp_config="$(mktemp)"
	cat > "$tmp_config" <<EOF
# SL4A_TouchScreen — installed by tools/sl4a-touch.sh, removed by 'uninstall'.
[Unit]
Description=SL4A_TouchScreen driver activation (Surface Laptop 3/4 AMD touchscreen)
After=multi-user.target
Wants=multi-user.target

[Service]
Type=oneshot
ExecStart="$REPO_DIR/tools/sl4a-touch.sh" activate
RemainAfterExit=yes
# A failed activation (e.g. Secure Boot key not yet enrolled, hardware
# absent) must never fail the boot or retry-loop — see journalctl -u
# sl4a-touch-activate for why, then run 'activate' manually once fixed.
SuccessExitStatus=0 1

[Install]
WantedBy=multi-user.target
EOF
	# Same ownership guard the modprobe config gets: never clobber a file at
	# this path that this tool did not write.
	if [ -e "$SYSTEMD_UNIT" ] && ! grep -q '^# SL4A_TouchScreen' "$SYSTEMD_UNIT"; then
		fail "refusing to replace unowned $SYSTEMD_UNIT"
	fi
	install -m 0644 "$tmp_config" "$SYSTEMD_UNIT"
	rm -f "$tmp_config"
	systemctl daemon-reload
	systemctl enable sl4a-touch-activate.service >/dev/null 2>&1
	pass "Created $SYSTEMD_UNIT (enabled — activates automatically after every boot)"

	info "Step 7: Activating..."
	if [ "$skip_activate" -eq 1 ]; then
		warn "Activation is skipped — the MOK key must be enrolled first."
		echo "  After you reboot and complete the key enrollment (see the"
		echo "  instructions printed above), the driver will activate"
		echo "  automatically on every boot. No further action needed."
		echo ""
		echo "  To verify after the reboot:  ./tools/sl4a-touch.sh status"
	else
		local requested_raw_mode="N"
		[ "$PROFILE" = "raw" ] && requested_raw_mode="Y"
		if [ -r /sys/module/sl4a_spi_hid/parameters/raw_mode ] && \
		   [ "$(cat /sys/module/sl4a_spi_hid/parameters/raw_mode)" != "$requested_raw_mode" ]; then
			warn "The selected profile changes a load-time-only module parameter."
			echo "  The modules keep the previous profile until the next boot;"
			echo "  the boot unit then activates the new one automatically."
			echo "  Nothing else to do (to activate by hand now:  sudo ./tools/sl4a-touch.sh activate)"
		else
			cmd_activate
		fi
	fi

	# Only now: everything that can fail has run. The banner used to print
	# before Step 7, so a failed activation was announced by "Install complete"
	# (review R16).
	echo ""
	rule
	if [ "$PROFILE" = "raw" ]; then
		echo -e "${YELLOW}${BOLD}Install complete${NC} ${YELLOW}— EXPERIMENTAL raw multitouch profile selected.${NC}"
	else
		echo -e "${GREEN}${BOLD}Install complete${NC} ${GREEN}— standard HID profile selected.${NC}"
	fi
	echo "  To remove:  sudo ./tools/sl4a-touch.sh uninstall"
	rule
}

# ── uninstall ────────────────────────────────────────────────────────────

cmd_uninstall() {
	elevate "remove the DKMS registration and /etc/modprobe.d config" uninstall "$@"

	header "SL4A_TouchScreen driver uninstaller"

	if [ -f "$SYSTEMD_UNIT" ]; then
		if grep -q '^# SL4A_TouchScreen' "$SYSTEMD_UNIT"; then
			info "Disabling and removing the boot-activation service..."
			systemctl disable sl4a-touch-activate.service >/dev/null 2>&1 || true
			rm -f "$SYSTEMD_UNIT"
			systemctl daemon-reload
			pass "Boot-activation service removed"
		else
			info "Leaving unowned $SYSTEMD_UNIT untouched"
		fi
	fi

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
	if [ -f "$SRC_DEST/dkms.conf" ] && grep -qE '^PACKAGE_NAME="sl4a-touch"[[:space:]]*$' "$SRC_DEST/dkms.conf"; then
		if dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all; then
			rm -rf "$SRC_DEST"
			pass "Removed package-owned DKMS version $PKG_VERSION"
		else
			info "DKMS removal failed; leaving $SRC_DEST for recovery"
		fi
	elif [ -e "$SRC_DEST" ]; then
		info "Leaving unowned $SRC_DEST untouched"
	fi

	# Any other version of this package (a leftover from an earlier upgrade)
	# would survive this uninstall and keep being rebuilt on kernel updates,
	# so a version mismatch no longer turns "Uninstall complete" into a lie.
	dkms_remove_other_versions ""

	depmod -a
	pass "DKMS removal completed"

	echo ""
	rule
	echo -e "${GREEN}${BOLD}Uninstall complete.${NC} Reboot to unload the active driver."
	echo "To reinstall later: sudo ./tools/sl4a-touch.sh install"
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
		# Re-encode a legacy PEM certificate for mokutil.
		if [ -r /var/lib/dkms/mok.pub ] && ! openssl x509 -in /var/lib/dkms/mok.pub -inform DER -noout 2>/dev/null; then
			warn "Existing DKMS signing key at /var/lib/dkms/mok.pub is not DER-encoded; re-encoding..."
			command -v openssl >/dev/null 2>&1 || \
				fail "openssl is required to re-encode the DKMS signing key as DER (mokutil only accepts DER). Install 'openssl' and retry."
			openssl x509 -in /var/lib/dkms/mok.pub -out /var/lib/dkms/mok.pub.der -outform DER 2>/dev/null \
				&& mv /var/lib/dkms/mok.pub.der /var/lib/dkms/mok.pub \
				&& pass "DKMS signing key re-encoded as DER at /var/lib/dkms/mok.pub" \
				|| fail "Could not re-encode the existing signing key as DER (mokutil only accepts DER)."
		fi
		if [ ! -r /var/lib/dkms/mok.pub ]; then
			echo ""
			echo "╔══════════════════════════════════════════════════════════════╗"
			echo "║  MISSING SIGNING KEY                                        ║"
			echo "║──────────────────────────────────────────────────────────────║"
			echo "║  Secure Boot is ON but the DKMS signing key does not exist.  ║"
			echo "║                                                              ║"
			echo "║  QUICK FIX (3 commands):                                    ║"
			echo "║    1. sudo dkms generate_mok                                ║"
			echo "║    2. sudo mokutil --import /var/lib/dkms/mok.pub          ║"
			echo "║    3. sudo reboot                                           ║"
			echo "║                                                              ║"
			echo "║  At the blue MOK Manager screen after reboot:               ║"
			echo "║    Enroll MOK → Continue → Yes → enter password → Reboot     ║"
			echo "║                                                              ║"
			echo "║  After login, the driver activates automatically.            ║"
			echo "║                                                              ║"
			echo "║  ALSO: running 'install' instead handles all of this         ║"
			echo "║  for you automatically.                                      ║"
			echo "╚══════════════════════════════════════════════════════════════╝"
			exit 1
		fi
		# mokutil --test-key's exit code alone is unreliable: on this
		# system it returns 1 even when the key IS in the enrolled MOK
		# database (it also checks the *running* kernel's live trusted
		# keyring, which only picks up a change after the next reboot).
		# Its own output still says "already enrolled" in that case, so
		# check that instead of trusting $? — capture output separately
		# first ("|| true"), since under `set -o pipefail` a direct
		# `mokutil | grep` pipeline would still report failure overall
		# from mokutil's own exit code even when grep finds the match.
		local mok_test_output
		mok_test_output="$(mokutil --test-key /var/lib/dkms/mok.pub 2>&1 || true)"
		if ! echo "$mok_test_output" | grep -qi "already enrolled"; then
			echo ""
			warn "Secure Boot is enabled but the DKMS signing key is not enrolled yet."
			echo "The kernel will refuse to load the signed modules until it is."
			echo ""
			if [ -t 0 ]; then
				echo "Step 1 of 2: enroll the key now (sets a one-time password you"
				echo "re-enter once at the next boot):"
				echo ""
				if mokutil --import /var/lib/dkms/mok.pub; then
					echo ""
					pass "Key staged for enrollment."
					echo ""
					echo -e "${BOLD}Step 2 of 2 — do this now:${NC}"
					echo "  1. Reboot: sudo reboot"
					echo "  2. A blue 'MOK Manager' screen appears before your OS loads."
					echo "     (If you miss it, it reappears on the next boot attempt.)"
					echo "  3. Select 'Enroll MOK' -> 'Continue' -> 'Yes'."
					echo "  4. Enter the password you just set above."
					echo "  5. Select 'Reboot'."
				echo "  6. After login, the driver activates automatically"
				echo "     (if installed via 'install'). Verify with:"
				echo "       ./tools/sl4a-touch.sh status"
				else
					fail "Key enrollment was not completed (mokutil exited non-zero). Nothing was activated."
				fi
			else
				echo "Run this command yourself in a real terminal (it needs an"
				echo "interactive password prompt), then follow the on-screen steps:"
				echo ""
				echo "  sudo mokutil --import /var/lib/dkms/mok.pub"
				echo ""
				echo "Full step-by-step MOK enrollment procedure: docs/ROLLBACK.md"
			fi
			exit 1
		fi
	fi

	local controllers=("$SYSFS_ROOT"/bus/acpi/devices/AMDI0060:*)
	[ "${#controllers[@]}" -eq 1 ] || fail "expected exactly one AMDI0060 ACPI device"
	local controller="${controllers[0]}"
	local controller_platform="$SYSFS_ROOT/bus/platform/devices/$(basename "$controller")"
	[ -d "$controller_platform" ] || fail "AMDI0060 platform device is absent"
	# Touchscreen node: MSHW0231 (SL4) or MSHW0162 (SL3 AMD) — exactly one of the two.
	local touches=()
	local mshw
	for mshw in MSHW0231 MSHW0162; do
		local matches=("$SYSFS_ROOT"/bus/acpi/devices/${mshw}:*)
		[ -e "${matches[0]}" ] && touches+=("${matches[@]}")
	done
	[ "${#touches[@]}" -eq 1 ] || fail "expected exactly one MSHW0231/MSHW0162 ACPI device"
	local touch="${touches[0]}"
	# Note: earlier versions of this check also refused to proceed if any
	# OTHER "MSHW*" ACPI device existed at all. That's not a meaningful
	# safety signal on real Surface hardware, which always exposes several
	# unrelated MSHW* nodes (keyboard, sensors, battery, ...) with their own
	# drivers already bound — it made activation impossible on every real
	# Surface Laptop 3/4. The check above (exactly one MSHW0231/MSHW0162) is what
	# actually identifies the touchscreen; that's sufficient.

	if [ -L "$controller_platform/driver" ] && \
	   [ "$(bound_driver "$controller_platform")" != "$CONTROLLER_DRIVER" ]; then
		fail "AMDI0060 is already bound to $(bound_driver "$controller_platform"); refusing to displace it"
	fi
	if [ -L "$touch/physical_node/driver" ] && \
	   [ "$(bound_driver "$touch/physical_node")" != "$HID_DRIVER" ]; then
		fail "touchscreen is already bound to $(bound_driver "$touch/physical_node"); refusing to displace it"
	fi

	[ ! -d "$SYSFS_ROOT/module/${CONTROLLER_MODULE//-/_}" ] && controller_loaded=1
	modprobe "$CONTROLLER_MODULE" || fail_rollback "could not load experimental controller"
	wait_for_driver "$controller_platform" "$CONTROLLER_DRIVER" || fail_rollback "experimental controller did not bind"

	[ ! -d "$SYSFS_ROOT/module/${HID_MODULE//-/_}" ] && hid_loaded=1
	modprobe "$HID_MODULE" || fail_rollback "could not load HID transport"
	wait_for_driver "$touch/physical_node" "$HID_DRIVER" || fail_rollback "touchscreen did not bind to the HID transport"

	pass "AMDI0060 and touchscreen are bound"
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

	if systemctl is-enabled sl4a-touch-activate.service >/dev/null 2>&1; then
		pass "Auto-activates on every boot (sl4a-touch-activate.service enabled)"
	else
		info "Does not auto-activate on boot — run 'install' to enable it"
	fi

	echo ""
	echo "Hardware:"
	if [ -n "$(touchscreen_acpi_id)" ] && acpi_device_present "AMDI0060"; then
		pass "  touchscreen ($(touchscreen_acpi_id)) and AMDI0060 present"
	else
		warn "  Expected ACPI devices not found — this may not be a Surface Laptop 3/4 AMD"
	fi

	echo ""
	echo "Runtime state:"
	if [ -d "/sys/module/${HID_MODULE//-/_}" ]; then
		pass "  $HID_MODULE is loaded"
		local ts_id
		ts_id="$(touchscreen_acpi_id)" || true
		if [ -n "$ts_id" ]; then
			local touches=(/sys/bus/acpi/devices/${ts_id}:*)
			if [ "${#touches[@]}" -eq 1 ]; then
				local bound
				bound="$(bound_driver "${touches[0]}/physical_node" 2>/dev/null || true)"
				if [ "$bound" = "$HID_DRIVER" ]; then
					pass "  $ts_id is bound to $HID_DRIVER"
				else
					info "  $ts_id is not bound to $HID_DRIVER yet — run 'activate'"
				fi
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
			-o|--output)
				[ $# -ge 2 ] || fail "-o requires a path"
				[ -n "$2" ] || fail "-o requires a non-empty path"
				OUT="$2"
				shift 2 ;;
			*) fail "unknown logs option: $1 (see --help)" ;;
		esac
	done

	# $OUT ends up in a root redirect plus a chmod that follows symlinks: keep it
	# away from device nodes, symlinks, and files that are not one of our bundles
	# (this is what keeps `logs -o /etc/shadow` from truncating the file). The
	# check-to-use window stays open — bash cannot open with O_NOFOLLOW — so the
	# guard is against mistakes, not against a hostile local user.
	if [ -n "$OUT" ]; then
		[ -L "$OUT" ] && fail "refusing to write the bundle through the symlink $OUT"
		if [ -e "$OUT" ]; then
			[ -f "$OUT" ] || fail "refusing to overwrite $OUT: not a regular file"
			# First line only: a file that merely quotes the header somewhere
			# is not one of our bundles and must not be truncated.
			[ "$(head -n 1 "$OUT" 2>/dev/null)" = "=== SL4A_TouchScreen diagnostic bundle ===" ] || \
				fail "refusing to overwrite $OUT: it is not a diagnostic bundle (choose another -o path)"
		fi
	fi

	# Quote the -o path explicitly: `${OUT:+-o "$OUT"}` unquoted is word-split by
	# bash, so a filename with a space reaches the elevated child as several
	# words and is rejected there as an unknown option (review R3-F4).
	if [ -n "$OUT" ]; then
		elevate "read the kernel log (dmesg)" logs -o "$OUT"
	else
		elevate "read the kernel log (dmesg)" logs
	fi
	[ -n "$OUT" ] || OUT="$REPO_DIR/sl4a-touch-diagnostics-$(date +%Y%m%d-%H%M%S).txt"

	header "SL4A_TouchScreen diagnostic collection"
	info "Writing to $OUT..."

	# Nothing in the bundle may abort collection: git refuses to read a
	# user-owned checkout as root, `systemctl status` exits non-zero for an
	# inactive unit, and a dmesg|grep with no match is exit 1 — under
	# `set -e -o pipefail` each of those used to truncate the file that a bug
	# report needs, in exactly the broken states worth diagnosing.
	# `+o pipefail` as well: `set +e` alone leaves pipefail on, so a dmesg|grep
	# with no match would make the status check below reject a bundle that was
	# written perfectly (review R19).
	set +e +o pipefail
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
		local ts_id
		ts_id="$(touchscreen_acpi_id)" || true
		if [ -n "$ts_id" ]; then
			echo "$ts_id: present"
		else
			echo "touchscreen ACPI (MSHW0231/MSHW0162): NOT FOUND"
		fi
		acpi_device_present "AMDI0060" && echo "AMDI0060: present" || echo "AMDI0060: NOT FOUND"
		[ -r "$DMI_ROOT/product_name" ] && echo "DMI product: $(tr -d '\n' < "$DMI_ROOT/product_name")"
		echo ""

		echo "--- DKMS ---"
		dkms status 2>&1 | grep -i sl4a || echo "(no sl4a-touch DKMS registration found)"
		echo ""

		echo "--- modprobe config ($MODPROBE_CONF) ---"
		[ -f "$MODPROBE_CONF" ] && cat "$MODPROBE_CONF" || echo "(not present)"
		echo ""

		echo "--- Boot activation service ---"
		systemctl status sl4a-touch-activate.service --no-pager 2>&1 | head -10
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
		local ts_id
		ts_id="$(touchscreen_acpi_id)" || true
		if [ -n "$ts_id" ]; then
			local touches=(/sys/bus/acpi/devices/${ts_id}:*)
			if [ "${#touches[@]}" -eq 1 ]; then
				local dev="/sys/bus/platform/devices/$(basename "$(dirname "${touches[0]}" 2>/dev/null)")"
				local spidev
				spidev=$(find /sys/devices -maxdepth 6 -path "*spi-${ts_id}:00" -type d 2>/dev/null | head -1)
				if [ -n "$spidev" ]; then
					for f in build_info ready lifecycle_status seq_state protocol_stats baseline_status \
						 bus_error_count device_initiated_reset_count; do
						if [ -r "$spidev/$f" ]; then
							echo "-- $f --"
							cat "$spidev/$f"
						fi
					done

					# Frame data, so a report can be analysed without asking for
					# anything else. heatmap_raw is the binary attribute and
					# carries the whole cell field (one byte per cell);
					# heatmap_debug is the one-page hex view kept for older
					# modules, and cuts the tail.
					echo ""
					echo "--- Last captured frame ---"
					if [ -r "$spidev/heatmap_raw" ]; then
						local frame_tmp frame_bytes
						frame_tmp=$(mktemp 2>/dev/null)
						if [ -n "$frame_tmp" ]; then
							# One read only: the attribute is rewritten at frame
							# rate, so the count has to describe the bytes below it.
							cat "$spidev/heatmap_raw" > "$frame_tmp" 2>/dev/null
							frame_bytes=$(wc -c < "$frame_tmp")
							if [ "${frame_bytes:-0}" -gt 0 ]; then
								echo "-- heatmap_raw ($frame_bytes bytes, cell field complete) --"
								od -An -v -tx1 -w32 "$frame_tmp"
							else
								echo "(no frame data: nothing captured yet, or the driver is being unbound)"
							fi
							rm -f "$frame_tmp"
						else
							echo "(could not create a temporary file to read the frame into)"
						fi
					elif [ -r "$spidev/heatmap_debug" ]; then
						echo "-- heatmap_debug (hex, truncated to one page: this module has no heatmap_raw) --"
						cat "$spidev/heatmap_debug"
					else
						echo "(no frame attribute: driver not bound, or no frame captured yet)"
					fi
				else
					echo "(spi-${ts_id}:00 sysfs node not found — driver not bound)"
				fi
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
		dmesg | grep -iE "sl4a|MSHW0231|MSHW0162|AMDI0060" | tail -300
	} > "$OUT"
	bundle_status=$?
	set -e -o pipefail

	# With `set +e` a failed redirect is silent: without this check the script
	# would report a bundle it never wrote. The size alone is not enough — a
	# redirect that cannot be opened keeps the previous (non-empty) file, and a
	# write that stops on a full disk is non-empty too — so the redirect's own
	# status and the bundle's last section are checked as well.
	if [ "$bundle_status" -ne 0 ] || [ ! -s "$OUT" ] || \
	   ! grep -q '^--- dmesg' "$OUT" 2>/dev/null; then
		fail "the diagnostic bundle could not be written to $OUT"
	fi

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
