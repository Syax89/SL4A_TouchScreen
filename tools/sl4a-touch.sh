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
# Where the input layer exposes its event nodes (discovery) and where the
# corresponding evdev char devices live (the bounded read during the touch
# window). Split in two on purpose: the diagnostic can find the panel's node
# by name under the first even when the second is not readable (no root, or
# the node belongs to another device).
INPUT_SYSFS="${SL4A_INPUT_SYSFS:-$SYSFS_ROOT/class/input}"
INPUT_DEV_ROOT="${SL4A_INPUT_DEV_ROOT:-/dev/input}"
# Seconds the touch window stays open (countdown and the bounded evdev read),
# and the most bytes one evdev read will take before giving up. 24 B is one
# struct input_event on 64-bit, so bytes/24 is the event count reported.
HUNT_TOUCH_SECS="${SL4A_HUNT_TOUCH_SECS:-6}"
HUNT_EVDEV_MAX="${SL4A_HUNT_EVDEV_MAX:-4096}"

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
# Strict charset (review R26-9): VERSION is interpolated by sed into the staged
# dkms.conf and passed to DKMS as the version of its build command, so anything
# outside this set corrupts one of them silently — '1.0.&' staged
# PACKAGE_VERSION="1.0.#VERSION#" (sed treats & as the match), and a value with
# a space split the DKMS build line into two words.
if [[ ! "$PKG_VERSION" =~ ^[0-9A-Za-z][0-9A-Za-z.+~_-]*$ ]]; then
	fail "VERSION ('$PKG_VERSION') is not a valid version string. Allowed: letters, digits and . + ~ _ -, starting with a letter or digit — never whitespace, '&', '#', '/' or another shell metacharacter. Fix the VERSION file and re-run."
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

# The revision the installed modules were built from. DKMS tracks a version
# (1.7.0) that does not change during a campaign, and the srcversion only says
# "same as the file on disk" — so both call a module installed when it was
# built before the last pull. That is how a sweep reloaded a driver three
# commits old and its field bundle described a revision that no longer existed.
# This stamp is the missing fact: install/rebuild write it, hunt compares it,
# and a mismatch rebuilds before sweeping.
INSTALLED_HEAD_STAMP="/var/lib/sl4a-touch/installed-head"

stamp_installed_head() {
	# Atomic, and only on success: the redirect used to truncate first, so a git
	# that refused (dubious ownership under sudo is the documented path) left a
	# zero-byte stamp, installed_head returned an empty string, and every bundle
	# then printed a false MISMATCH while hunt rebuilt on every single run.
	local head tmp="$INSTALLED_HEAD_STAMP.$$" pre="${1:-}"
	mkdir -p "$(dirname "$INSTALLED_HEAD_STAMP")" 2>/dev/null || true
	head="$(git -C "$REPO_DIR" rev-parse HEAD 2>/dev/null)" || return 0
	[ -n "$head" ] || return 0
	$pre sh -c 'cat >"$1"' _ "$tmp" <<< "$head" 2>/dev/null || return 0
	$pre mv -f "$tmp" "$INSTALLED_HEAD_STAMP" 2>/dev/null || rm -f "$tmp" 2>/dev/null || true
}

installed_head() {
	local head
	head="$(cat "$INSTALLED_HEAD_STAMP" 2>/dev/null)" || head=""
	[ -n "$head" ] || head="unknown"
	echo "$head"
}

restage_and_rebuild() {
	echo "→ Staging the checkout into $SRC_DEST and rebuilding..."
	mkdir -p "$SRC_DEST" || fail "cannot create $SRC_DEST"
	cp -a "$DRIVER_DIR"/. "$SRC_DEST"/ || fail "cannot stage the driver sources"
	rm -f "$SRC_DEST"/*.o "$SRC_DEST"/*.ko "$SRC_DEST"/*.mod "$SRC_DEST"/*.mod.c 2>/dev/null || true
	dkms build -m "$PKG_NAME" -v "$PKG_VERSION" --force || fail "DKMS build failed"
	dkms install -m "$PKG_NAME" -v "$PKG_VERSION" --force || fail "DKMS install failed"
	depmod -a 2>/dev/null || true
	stamp_installed_head
}

dkms_installed_version() {
	# Real dkms status output looks like:
	#   sl4a-touch/1.2.0, 7.1.3-2-cachyos, x86_64: installed
	# -k pins this to the RUNNING kernel (review R26-3): without it a leftover
	# entry for another kernel answered "matches this checkout" while the
	# kernel that would actually load the module had nothing installed, and
	# the kernelless "sl4a-touch/1.2.0: added" shape is not an install either.
	# "|| true" throughout: an empty/no-match result is a normal "not
	# installed" outcome here, not a script-ending error under set -e.
	dkms status -m "$PKG_NAME" -k "$(uname -r)" 2>/dev/null | grep ': installed$' | head -1 | \
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
	# Spellings: the tool writes Y/N, but the README/QUICKSTART (and the
	# driver's own parm desc) use 0/1, and modprobe/kstrtobool accept both —
	# accepting only Y/N made status call a working profile "unrecognized"
	# and install warn about a change that never happened (P12 wave, F3).
	if grep -qE '^[[:space:]]*options[[:space:]]+sl4a_spi_hid[[:space:]].*raw_mode=([Yy]|1)' "$MODPROBE_CONF" 2>/dev/null; then
		echo "raw"
	elif grep -qE '^[[:space:]]*options[[:space:]]+sl4a_spi_hid[[:space:]].*raw_mode=([Nn]|0)' "$MODPROBE_CONF" 2>/dev/null; then
		echo "standard"
	else
		echo "unknown"
	fi
}

# Echo the raw_mode the LOADED module was started with ("Y"/"N"), or return 1
# when no module is loaded. A load-time parameter cannot change on a running
# module, so "what the config file says" and "what is loaded right now" are two
# different answers and every caller asks this one helper for the live value
# (review R26-8).
loaded_raw_mode() {
	local param="/sys/module/${HID_MODULE//-/_}/parameters/raw_mode"
	[ -r "$param" ] || return 1
	cat "$param"
}

# True only when the boot-activation unit is enabled AND systemd can load it.
# `systemctl is-enabled` is file state only: a checkout moved or deleted since
# the install leaves ExecStart pointing at nothing, and every boot then fails
# the unit with 203/EXEC — exactly the "activates automatically" promise
# breaking silently (review R26-4). systemd-analyze resolves ExecStart without
# starting anything; the fallback keeps the check useful where it is absent.
boot_unit_loadable() {
	systemctl is-enabled sl4a-touch-activate.service >/dev/null 2>&1 || return 1
	if command -v systemd-analyze >/dev/null 2>&1; then
		systemd-analyze verify "$SYSTEMD_UNIT" >/dev/null 2>&1 || return 1
	else
		[ -x "$REPO_DIR/tools/sl4a-touch.sh" ] || return 1
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
                      --raw       Beta heatmap multitouch. May be
                                  unstable; no hardware-qualified result yet.
                      --check     Validate prerequisites only, write nothing.
                      --dry-run   Validate and print the selected profile.
                      --force     Continue even if expected hardware/DMI is
                                  not detected.
                      --repair    If a file at one of the tool's own paths
                                  (the modprobe conf, the boot unit) is not
                                  ours, move it aside to <path>.unowned-<time>
                                  and continue. Nothing is deleted; without
                                  this, install and uninstall both refuse and
                                  the machine is stuck until you move the file
                                  yourself.

  uninstall [--repair]
                    Remove the installed driver, its DKMS registration, and
                    the boot-activation service. Loaded modules are left
                    running until reboot.
                      --repair    Move aside (never delete) a file at one of the
                                  tool's own paths that is not ours, instead of
                                  leaving it in place to block install.

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
  hunt [-o PATH]   One command for a display problem: runs the full battery —
                    every field variant the campaign designed (11 raw + 4
                    standard), one per driver reload, on the command line so
                    /etc/modprobe.d is never edited. For each it unloads and
                    reloads the driver at debug level 3, waits while you touch
                    the panel (reading the touch device's evdev node, or a
                    y/n fallback), snapshots the counters before and after,
                    and records the driver log. Writes ONE file with every
                    variant's profile, counters, deltas, log, a verdict line
                    and a final summary table. Send that file.
                    Default output: next to the driver, like the
                    diagnostics bundle (sl4a-hunt-<timestamp>.txt).
   soak --minutes N [-o FILE] [--profile raw|standard|current] [--interactive]
                     Observe the RUNNING driver for N minutes WITHOUT
                     unloading, reloading, or asking anything: every 30 s it
                     snapshots ready, seq_state and protocol_stats, slices
                     the kernel log for the interval, and watches the touch
                     evdev node in the background (no prompts — 'evdev
                     unavailable' is recorded when there is no node). The
                     pinch phase is skipped unless --interactive is given on
                     a terminal. Writes ONE file with per-interval deltas, a
                     VERDICT line (PASS only when ready and seq_state 4 hold
                     steady and either touch data flows with zero drops or
                     the IRQ line stays flat with no storm) and a summary
                     table. Run with sudo so the dmesg slices are populated;
                     without root the counters still work.
                     Default output: next to the driver, like the
                     hunt battery (sl4a-soak-<timestamp>.txt).


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
	local labels=("Install" "Uninstall" "Activate" "Status" "Collect diagnostics (logs)" "Frame hunt battery (touch the panel)" "Quit")
	local cmds=("install" "uninstall" "activate" "status" "logs" "hunt" "")
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

quarantine_unowned() {
	local f="$1" dest
	dest="$f.unowned-$(date +%Y%m%d-%H%M%S)"
	if mv "$f" "$dest" 2>/dev/null; then
		info "Moved the unowned $f aside to $dest (--repair); nothing was deleted"
	else
		fail "could not move unowned $f aside — rename or remove it by hand"
	fi
}

cmd_install() {
	local MODE="install" PROFILE="" FORCE=0 REPAIR=0
	for arg in "$@"; do
		case "$arg" in
			--check) MODE="check" ;;
			--dry-run) MODE="dry-run" ;;
			--raw) PROFILE="raw" ;;
			--standard) PROFILE="standard" ;;
			--force) FORCE=1 ;;
			--repair)
				# A file at one of our paths that carries no ownership marker
				# blocks install at the guard below AND uninstall at its own
				# guard, so the only way out was one hand-typed rm on a machine
				# that may have no driver at all. This is the documented escape:
				# move it aside, never delete it (review round 9, T59).
				REPAIR=1 ;;
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
			echo -e "  ${YELLOW}2) Raw multitouch${NC} — Beta. Heatmap-based multi-finger"
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
		install "--$PROFILE" $([ "$FORCE" -eq 1 ] && echo --force) $([ "$REPAIR" -eq 1 ] && echo --repair)

	if [ -e "$MODPROBE_CONF" ] && ! grep -q '^# SL4A_TouchScreen' "$MODPROBE_CONF"; then
		[ "$REPAIR" -eq 1 ] || fail "refusing to replace unowned $MODPROBE_CONF — it is not ours. Move it aside yourself, or re-run with '--repair' to have it moved aside for you (nothing is deleted)"
		quarantine_unowned "$MODPROBE_CONF"
	fi
	# Same guard, same reason, deliberately in the same place: it used to run
	# at Step 6, AFTER the DKMS build/install, the version sweep and the
	# profile write — a refusal there left a half install with no boot unit
	# and no rollback (P12 wave, F1). Nothing below depends on the old spot.
	if [ -e "$SYSTEMD_UNIT" ] && ! grep -q '^# SL4A_TouchScreen' "$SYSTEMD_UNIT"; then
		[ "$REPAIR" -eq 1 ] || fail "refusing to replace unowned $SYSTEMD_UNIT — it is not ours. Move it aside yourself, or re-run with '--repair' to have it moved aside for you (nothing is deleted)"
		quarantine_unowned "$SYSTEMD_UNIT"
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
		# An unowned tree is refused before staging (see the guard above), so
		# by the time this runs $SRC_DEST is ours or a partial copy of our
		# own staging — both safe to drop.
		rm -rf "$SRC_DEST"
	}

	# Undo a failed attempt only when THIS run created the DKMS entry. VERSION
	# does not move between commits, so the usual "upgrade" reuses the same
	# version: a plain cleanup then removes the registration AND uninstalls the
	# module that works right now (dkms remove deletes the /updates/dkms
	# object), leaving no driver for the next boot — the one outcome an upgrade
	# must never produce (review R26-2). When the entry was already registered
	# and installed, its registration and installed module are what stays
	# behind, so "existing driver state was left unchanged" is literally true.
	stage_failed() {
		[ "$already_added" -eq 1 ] || cleanup_staged_install
		if [ "$legacy_removed" -eq 1 ]; then
			# The legacy artifact is already uninstalled at this point, so
			# "existing driver state was left unchanged" would be false.
			fail "${1%%;*} — the previous driver artifact was already uninstalled, so there is no driver installed right now; re-run 'install' once the cause above is fixed"
		fi
		if [ "$already_added" -eq 1 ]; then
			# The DKMS entry and the installed module are untouched, but the
			# staged sources were updated from this checkout before the build
			# ran — and DKMS rebuilds from them on every kernel update, so a
			# broken tree keeps failing there too (P12 wave, P12-5).
			fail "${1%%;*} — the DKMS entry and the installed driver are unchanged, but the staged sources under $SRC_DEST were updated from this checkout; fix the build so the next kernel update can rebuild from them"
		fi
		fail "$1"
	}

	local already_added=0
	local legacy_removed=0
	# The staged tree gets the same pre-write ownership guard as the modprobe
	# config and the boot unit: staging below (cp -a) replaces every file in
	# it, so an unowned tree would be clobbered file by file while the old
	# code printed "Leaving unowned ... untouched" (P12 wave, P12-4). Refuse
	# first; --repair moves it aside.
	if [ -e "$SRC_DEST" ] && [ -f "$SRC_DEST/dkms.conf" ] && \
	   ! grep -qE '^PACKAGE_NAME="sl4a-touch"[[:space:]]*$' "$SRC_DEST/dkms.conf"; then
		[ "$REPAIR" -eq 1 ] || fail "refusing to replace unowned $SRC_DEST — it is not ours. Move it aside yourself, or re-run with '--repair' to have it moved aside for you (nothing is deleted)"
		quarantine_unowned "$SRC_DEST"
	fi
	if [ -e "$SRC_DEST" ]; then
		if dkms status -m "$PKG_NAME" -v "$PKG_VERSION" 2>/dev/null | grep -q "installed"; then
			if grep -q '^obj-m += sl4a-spi-amd.o$' "$SRC_DEST/Kbuild" && \
			   grep -q '^obj-m += sl4a-spi-hid.o$' "$SRC_DEST/Kbuild"; then
				# Same version already registered: re-stage and rebuild
				# anyway. VERSION does not change between commits, so
				# "already installed" says nothing about whether the module
				# on the machine matches this checkout — trusting it is how
				# a pull used to leave a stale module running. `dkms add`
				# would refuse an entry that already exists, so it is
				# skipped below instead of failing the install.
				info "DKMS version $PKG_NAME/$PKG_VERSION is already registered; rebuilding from this checkout"
				already_added=1
			else
				info "Replacing the package's legacy spi-amd artifact with the opt-in controller module..."
				dkms remove -m "$PKG_NAME" -v "$PKG_VERSION" --all || fail "could not remove the package's legacy DKMS artifact"
				legacy_removed=1
				# The tree is ours (the ownership guard ran before the case
				# analysis), so it goes with the legacy artifact it holds.
				rm -rf "$SRC_DEST"
			fi
		else
			# Left behind by an interrupted run: recoverable, not a dead end.
			info "Cleaning up an incomplete staging of $PKG_NAME/$PKG_VERSION..."
			cleanup_staged_install
		fi
	fi

	mkdir -p "$SRC_DEST"
	cp -a "$DRIVER_DIR"/. "$SRC_DEST"/
	rm -f "$SRC_DEST"/*.o "$SRC_DEST"/*.ko "$SRC_DEST"/*.mod "$SRC_DEST"/*.mod.c \
	      "$SRC_DEST"/*.mod.o "$SRC_DEST"/Module.symvers "$SRC_DEST"/modules.order \
	      "$SRC_DEST"/.*.cmd 2>/dev/null || true
	rm -f "$SRC_DEST/test_harness.c" "$SRC_DEST/sl4a-touch.service" "$SRC_DEST/sl4a-touch-load.sh" 2>/dev/null || true
	sed -i "s|#VERSION#|${PKG_VERSION}|" "$SRC_DEST/dkms.conf"

	if [ "$already_added" -eq 0 ]; then
		dkms add -m "$PKG_NAME" -v "$PKG_VERSION" || stage_failed "DKMS add failed; existing driver state was left unchanged"
	else
		pass "Reusing the DKMS entry already registered for $PKG_NAME/$PKG_VERSION"
	fi
	# --force on both: DKMS caches the built module per (module, version,
	# kernel), and VERSION does not change between commits, so a plain
	# `dkms build` after a pull answers "already built" and installs the
	# STALE object. The module on the machine then silently stops matching
	# the checkout (field: a 1.7.0+main install whose loaded module had
	# none of the new attributes).
	dkms build -m "$PKG_NAME" -v "$PKG_VERSION" --force || stage_failed "DKMS build failed; existing driver state was left unchanged"
	dkms install -m "$PKG_NAME" -v "$PKG_VERSION" --force || stage_failed "DKMS install failed; existing driver state was left unchanged"
	pass "sl4a-spi-amd.ko + sl4a-spi-hid.ko built and installed via DKMS for kernel $(uname -r)"
	stamp_installed_head

	# Only now, with the new version built AND installed, drop any other
	# registration of this package: an older version left registered keeps
	# building the same sl4a-spi-amd.ko/sl4a-spi-hid.ko names on every kernel
	# update, and `dkms autoinstall` then installs whichever ran last, so an
	# older revision can silently become the one that loads. This used to run
	# BEFORE the build above, which meant a failed build had already removed
	# the working version — leaving no registered driver at all, and a reboot
	# away from a dead touchscreen (review R26-2).
	dkms_remove_other_versions "$PKG_VERSION"
	# Removing a version also deletes the module objects it recorded, and both
	# versions record the same /updates/dkms destination — so the new .ko files
	# can go with the old registration. Put them back; the command is
	# idempotent and this keeps the on-disk driver matching the checkout.
	dkms install -m "$PKG_NAME" -v "$PKG_VERSION" --force || \
		fail "DKMS install failed after the DKMS version cleanup; the driver running right now keeps working until you reboot, but re-run 'install' once the cause above is fixed"

	info "Step 4: Updating module dependencies..."
	depmod -a
	pass "Module dependencies updated"

	info "Step 5: Writing the $PROFILE profile to $MODPROBE_CONF..."
	local tmp_config
	tmp_config="$(mktemp "${MODPROBE_CONF}.XXXXXX")"
	if [ "$PROFILE" = "raw" ]; then
		cat > "$tmp_config" <<'EOF'
# SL4A_TouchScreen experimental raw heatmap profile
options sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y wire_double_opcode=1
EOF
	else
		cat > "$tmp_config" <<'EOF'
# SL4A_TouchScreen standard HID profile
options sl4a_spi_hid raw_mode=N wire_double_opcode=1
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
	# The ownership check for this path already ran next to the modprobe
	# config's (before any write this run makes), so reaching this point
	# means the unit file is ours or absent.
	install -m 0644 "$tmp_config" "$SYSTEMD_UNIT"
	rm -f "$tmp_config"
	systemctl daemon-reload
	systemctl enable sl4a-touch-activate.service >/dev/null 2>&1 || \
		fail "could not enable sl4a-touch-activate.service — the driver would not come back after a reboot (run 'sudo systemctl enable sl4a-touch-activate.service' to see why)"
	# The pass below is a promise about every future boot, so it is only made
	# after the unit is verified to be enabled AND loadable (review R26-4).
	boot_unit_loadable || \
		fail "$SYSTEMD_UNIT is enabled but systemd cannot load it (ExecStart points at $REPO_DIR/tools/sl4a-touch.sh — was the checkout moved or deleted?). Run 'systemd-analyze verify $SYSTEMD_UNIT' for the reason"
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
		local requested_raw_mode="N" live_raw
		[ "$PROFILE" = "raw" ] && requested_raw_mode="Y"
		if live_raw="$(loaded_raw_mode)" && [ "$live_raw" != "$requested_raw_mode" ]; then
			warn "The selected profile changes a load-time-only module parameter."
			echo "  The modules keep the previous profile until the next boot;"
			echo "  the boot unit then activates the new one automatically."
			echo "  Nothing else to do (to activate by hand now:  sudo ./tools/sl4a-touch.sh activate)"
		else
			cmd_activate
			# cmd_activate cannot tell a no-op from a fresh load: `modprobe`
			# does nothing when the module is already loaded, and
			# wait_for_driver then sees the OLD object. srcversion moves with
			# every source edit, so compare what is running against what was
			# just installed and say it — never let "Install complete" imply
			# the new build is the one answering (review R26-5).
			local mod loaded_src installed_src stale=""
			for mod in sl4a_spi_amd sl4a_spi_hid; do
				loaded_src="$(cat "/sys/module/$mod/srcversion" 2>/dev/null || true)"
				installed_src="$(modinfo -F srcversion "$mod" 2>/dev/null || true)"
				if [ -n "$loaded_src" ] && [ -n "$installed_src" ] && \
				   [ "$loaded_src" != "$installed_src" ]; then
					stale="${stale:+$stale, }${mod//_/-}"
				fi
			done
			# The parameters count too: a module already in memory keeps the
			# ones it was loaded with, so installing the other profile changes
			# the file and not the driver — a test then exercises the previous
			# profile while everything looks installed. (This is exactly what
			# made a standard-mode test run in raw mode.)
			profile_mismatch=""
			while read -r k v; do
				[ -n "$k" ] || continue
				# sysfs prints booleans as Y/N; a hand-edited profile may
				# spell them 0/1 or y/n (all equivalent to the kernel). Say
				# nothing about a spelling difference (P12 wave, F3).
				case "$v" in 1|[Yy]) v=Y ;; 0|[Nn]) v=N ;; esac
				running="$(cat "/sys/module/sl4a_spi_hid/parameters/$k" 2>/dev/null || true)"
				case "$running" in 1|[Yy]) running=Y ;; 0|[Nn]) running=N ;; esac
				if [ -n "$running" ] && [ "$running" != "$v" ]; then
					profile_mismatch="${profile_mismatch:+$profile_mismatch, }$k=$running running, $v configured"
				fi
			done < <(awk '/^[ \t]*options[ \t]+sl4a_spi_hid/ { sub(/#.*/, ""); for (i = 3; i <= NF; i++) { split($i, kv, "="); if (kv[1] != "") print kv[1], kv[2] } }' "$MODPROBE_CONF" 2>/dev/null)
			if [ -n "$profile_mismatch" ]; then
				stale="${stale:+$stale, }profile"
				warn "The running module has a different profile than the one installed:"
				echo "  $profile_mismatch"
			fi

			if [ -n "$stale" ]; then
				warn "The driver running right now is NOT the build just installed ($stale)."
				echo "  A loaded module is not replaced by modprobe: the previous build"
				echo "  stays in memory, bound to the touchscreen, until it is unloaded"
				echo "  or the machine reboots."
				# Act on it instead of describing it: the panel is dead while
				# the old build is bound to it, and unloading is the step the
				# user would otherwise do by hand. An unload that fails is not
				# fatal — a client can hold the device — so the reboot remains
				# the advertised way out.
				info "Unloading the previous build and loading the new one..."
				if modprobe -r sl4a_spi_hid sl4a_spi_amd 2>/dev/null; then
					cmd_activate
					stale=""
					for mod in sl4a_spi_amd sl4a_spi_hid; do
						loaded_src="$(cat "/sys/module/$mod/srcversion" 2>/dev/null || true)"
						installed_src="$(modinfo -F srcversion "$mod" 2>/dev/null || true)"
						if [ -n "$loaded_src" ] && [ -n "$installed_src" ] && \
						   [ "$loaded_src" != "$installed_src" ]; then
							stale="${stale:+$stale, }${mod//_/-}"
						fi
					done
				fi
				if [ -n "$stale" ]; then
					warn "Still running the previous build ($stale)."
					echo "  To load the new build by hand:"
					echo "    sudo modprobe -r sl4a-spi-hid sl4a-spi-amd && sudo ./tools/sl4a-touch.sh activate"
					echo "  or simply reboot — the boot unit loads the new build automatically."
				else
					pass "The new build is the one running now"
				fi
			fi
		fi
	fi

	# Only now: everything that can fail has run. The banner used to print
	# before Step 7, so a failed activation was announced by "Install complete"
	# (review R16).
	echo ""
	rule
	if [ "$PROFILE" = "raw" ]; then
		echo -e "${YELLOW}${BOLD}Install complete${NC} ${YELLOW}— Beta raw multitouch profile selected.${NC}"
	else
		echo -e "${GREEN}${BOLD}Install complete${NC} ${GREEN}— standard HID profile selected.${NC}"
	fi
	echo "  To remove:  sudo ./tools/sl4a-touch.sh uninstall"
	rule
}

# ── uninstall ────────────────────────────────────────────────────────────

cmd_uninstall() {
	# Nothing is installed afterwards, so the stamp must not outlive it.
	rm -f "$INSTALLED_HEAD_STAMP" 2>/dev/null || true
	elevate "remove the DKMS registration and /etc/modprobe.d config" uninstall "$@"

	header "SL4A_TouchScreen driver uninstaller"

	# Same escape as install: without --repair this command leaves a
	# marker-less file at one of our paths in place and still reports success,
	# so the state that blocks install is exactly the state uninstall cannot
	# clear. (review round 9, T59)
	local REPAIR=0
	for arg in "$@"; do
		case "$arg" in
			--repair) REPAIR=1 ;;
			*) fail "unknown uninstall option: $arg (see --help)" ;;
		esac
	done
	# Anything this run could not remove (unowned files it refuses to touch,
	# a DKMS registration that survived) changes the closing verdict: they
	# block the next install or keep DKMS rebuilding on kernel updates
	# (P12 wave, P12-8/F6).
	local leftovers=0

	if [ -f "$SYSTEMD_UNIT" ]; then
		if grep -q '^# SL4A_TouchScreen' "$SYSTEMD_UNIT"; then
			info "Disabling and removing the boot-activation service..."
			systemctl disable sl4a-touch-activate.service >/dev/null 2>&1 || true
			rm -f "$SYSTEMD_UNIT"
			systemctl daemon-reload
			pass "Boot-activation service removed"
		else
			if [ "$REPAIR" -eq 1 ]; then
				quarantine_unowned "$SYSTEMD_UNIT"
			else
				info "Leaving unowned $SYSTEMD_UNIT untouched (re-run with '--repair' to move it aside so install can proceed)"
			leftovers=1
			fi
		fi
	fi

	if [ -f "$MODPROBE_CONF" ]; then
		if grep -q '^# SL4A_TouchScreen' "$MODPROBE_CONF"; then
			info "Removing package-owned modprobe config..."
			rm -f "$MODPROBE_CONF"
			pass "Modprobe config removed"
		else
			if [ "$REPAIR" -eq 1 ]; then
				quarantine_unowned "$MODPROBE_CONF"
			else
				info "Leaving unowned $MODPROBE_CONF untouched (re-run with '--repair' to move it aside so install can proceed)"
			leftovers=1
			fi
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
			leftovers=1
		fi
	elif [ -e "$SRC_DEST" ]; then
		info "Leaving unowned $SRC_DEST untouched"
		leftovers=1
	fi

	# Any other version of this package (a leftover from an earlier upgrade)
	# would survive this uninstall and keep being rebuilt on kernel updates,
	# so a version mismatch no longer turns "Uninstall complete" into a lie.
	dkms_remove_other_versions ""

	# Whatever is still registered (the version above if its removal failed,
	# or another one) keeps DKMS rebuilding it on kernel updates.
	if [ -n "$(dkms status -m "$PKG_NAME" 2>/dev/null || true)" ]; then
		leftovers=1
	fi

	depmod -a
	pass "DKMS removal completed"

	echo ""
	rule
	if [ "$leftovers" -eq 1 ]; then
		echo -e "${YELLOW}${BOLD}Uninstall finished with items left behind.${NC} See the notes above."
		echo "To clear them: re-run with '--repair' (unowned files), or remove the DKMS registration by hand."
	else
		echo -e "${GREEN}${BOLD}Uninstall complete.${NC} Reboot to unload the active driver."
	fi
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

	local installed kernel
	kernel="$(uname -r)"
	# Kernel-aware (review R26-3): a leftover entry for another kernel is not
	# "installed" for the kernel that would load it, and the line says which
	# kernel it is talking about.
	installed="$(dkms_installed_version)"
	if [ -n "$installed" ]; then
		if [ "$installed" = "$PKG_VERSION" ]; then
			pass "DKMS installed version: $installed (installed for kernel $kernel)"
		else
			warn "DKMS installed version: $installed for kernel $kernel (this checkout is $PKG_VERSION — run 'install' to upgrade)"
		fi
	# The version string never moves between commits, so "matches this checkout"
	# read off it alone is a claim this tool documents as meaningless: both the
	# loaded module and the installed file can be three commits old together.
	local head_now head_built
	head_now="$(git -C "$REPO_DIR" rev-parse HEAD 2>/dev/null || echo unknown)"
	head_built="$(installed_head)"
	if [ "$head_built" = "$head_now" ] && [ "$head_built" != "unknown" ]; then
		pass "Installed modules were built from $head_built (this checkout)"
	elif [ "$head_built" = "unknown" ]; then
		warn "No installed-revision stamp — run 'install' so a bundle can say which build it describes"
	else
		warn "Installed modules were built from $head_built, this checkout is $head_now — run 'install' (or 'hunt')"
	fi
	else
		info "Not installed via DKMS for the running kernel ($kernel) — run 'install' to install"
	fi

	# Two different answers on purpose (review R26-8): the config file decides
	# the next boot, while the modules keep the parameters they were loaded
	# with until then — printing only the file's value claimed a profile the
	# running driver does not use.
	local profile live_raw
	profile="$(modprobe_profile)"
	case "$profile" in
		raw)      warn "Modprobe profile for the next boot: raw (Beta multitouch)" ;;
		standard) pass "Modprobe profile for the next boot: standard HID" ;;
		none)     info "No modprobe profile configured (nothing installed)" ;;
		*)        warn "Unrecognized contents in $MODPROBE_CONF — no profile will be applied" ;;
	esac
	if live_raw="$(loaded_raw_mode)"; then
		if [ "$live_raw" = "Y" ]; then
			warn "Profile running right now: raw (Beta multitouch) — it stays until the next boot"
		else
			pass "Profile running right now: standard HID"
		fi
	fi

	if boot_unit_loadable; then
		pass "Auto-activates on every boot (sl4a-touch-activate.service enabled and loadable)"
	elif systemctl is-enabled sl4a-touch-activate.service >/dev/null 2>&1; then
		warn "sl4a-touch-activate.service is enabled but systemd cannot load it (check ExecStart — is $REPO_DIR still there?)"
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

# Everything that can be checked on this machine without hardware, in one
# place: the bundle carries the result instead of a reviewer asking for a
# command the user has no reason to know to run.
run_host_self_tests() {
	echo "--- Self-tests (feasible on this machine) ---"
	if ! command -v make >/dev/null 2>&1 || ! command -v cc >/dev/null 2>&1; then
		echo "skipped: make/cc not installed"
		return 0
	fi
	[ -d "$REPO_DIR/tests" ] || { echo "skipped: no tests directory"; return 0; }
	echo "$ make -C tests test"
	local out rc
	# This runs under `set -e -o pipefail` from a diagnostic path: a failing
	# suite used to abort hunt mid-variant, before the driver was put back.
	local had_errexit=0
	case "$-" in *e*) had_errexit=1 ;; esac
	set +e +o pipefail
	out="$(make -C "$REPO_DIR/tests" test 2>&1)"
	rc=$?
	[ "$had_errexit" -eq 1 ] && set -e -o pipefail || true
	echo "$out" | grep -E 'PASS|FAIL|assertions' | tail -n 12 || true
	if [ "$rc" = 0 ]; then
		echo "suite result: PASS"
	else
		echo "suite result: FAIL (exit $rc) — last 40 lines:"
		echo "$out" | tail -n 40
	fi
	# Leave no build output behind, least of all root-owned files in the user's
	# checkout when this ran under sudo.
	make -C "$REPO_DIR/tests" clean >/dev/null 2>&1 || true
}

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

	# A path starting with '-' is a file name, not an option, but $OUT is fed to
	# head/grep/chmod as well as to the redirect below: `head -n 1 "-x"` is an
	# invalid option and `grep -q '^--- dmesg' "-x"` reads stdin, so the bundle
	# was written and then the completion check failed on the path (review
	# R26-6). Normalise once here — every consumer gets the ./-prefixed form —
	# and keep the guards below on the same value they always checked.
	case "$OUT" in
		-*) OUT="./$OUT" ;;
	esac

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
			# Three states, never conflated (review R26-10): git ran and FAILED
			# (its stderr is shown — as root against a user-owned checkout git
			# refuses with "detected dubious ownership"), git ran on a clean
			# tree, git ran on a modified one. Swallowing stderr made a refusal
			# print exactly what a clean checkout prints.
			local git_out git_rc
			git_out="$(git -C "$REPO_DIR" rev-parse HEAD 2>&1)"; git_rc=$?
			if [ "$git_rc" -ne 0 ]; then
				echo "git: FAILED (exit $git_rc): $git_out"
				echo "     (as root against a user-owned checkout this is usually 'detected dubious ownership'; allow it with: git config --global --add safe.directory $REPO_DIR)"
			else
				echo "git HEAD: $git_out"
				git_out="$(git -C "$REPO_DIR" status --short 2>&1)"; git_rc=$?
				if [ "$git_rc" -ne 0 ]; then
					echo "git status: FAILED (exit $git_rc): $git_out"
				elif [ -n "$git_out" ]; then
					printf '%s\n' "$git_out" | sed 's/^/git status (modified): /'
				else
					echo "git status: clean (no local modifications)"
				fi
			fi
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

		# Read this first: the modinfo below describes the module ON DISK and
		# lsmod only knows names — neither says which build is loaded. srcversion
		# moves with every source edit, so the running module's own srcversion
		# against the installed one is the staleness answer (review R26-7).
		echo ""
		echo "--- Installed revision vs this checkout (what srcversion cannot see) ---"
		# srcversion compares the loaded module with the file on disk; both can be
		# three commits old together. Only the build-time stamp knows which
		# revision the modules came from, and a bundle that does not say it gets
		# read as if it came from the checkout beside it — which cost this
		# campaign four rounds of field testing against a module that no longer
		# existed in git.
		local head_now head_built
		head_now="$(git -C "$REPO_DIR" rev-parse HEAD 2>/dev/null || echo unknown)"
		head_built="$(installed_head)"
		echo "checkout:                $head_now"
		echo "installed modules built: $head_built"
		if [ "$head_built" = "$head_now" ] && [ "$head_built" != "unknown" ]; then
			echo "the installed modules were built from this exact checkout"
		else
			echo "MISMATCH — the installed modules predate this checkout, so every figure"
			echo "below describes ${head_built:0:8}, not ${head_now:0:8}. Run 'install', or 'hunt'"
			echo "which rebuilds by itself, before reading anything into these counters."
		fi
		echo ""
		echo "--- Loaded vs installed module (srcversion — read first) ---"
		for mod in sl4a_spi_amd sl4a_spi_hid; do
			local loaded_src disk_src
			loaded_src="$(cat "/sys/module/$mod/srcversion" 2>/dev/null || true)"
			disk_src="$(modinfo -F srcversion "$mod" 2>/dev/null || true)"
			if [ -z "$loaded_src" ]; then
				echo "$mod: not loaded (nothing running to compare)"
			elif [ -z "$disk_src" ]; then
				echo "$mod: loaded, srcversion $loaded_src — no installed object found to compare against"
			elif [ "$loaded_src" = "$disk_src" ]; then
				echo "$mod: MATCHES the installed module (srcversion $loaded_src)"
			else
				echo "$mod: STALE — running build $loaded_src, installed module $disk_src (the previous build is still loaded)"
			fi
		done

		# What the kernel would load, so a module older than the checkout is
		# visible here: DKMS reuses a cached build when VERSION has not moved,
		# and srcversion changes with every source edit. This is the ON-DISK
		# identity — the section above is what is actually running.
		echo ""
		echo "--- Module objects ---"
		for mod in sl4a_spi_amd sl4a_spi_hid; do
			modinfo "$mod" 2>/dev/null | grep -E "^(filename|version|srcversion|vermagic):" \
				|| echo "$mod: modinfo failed"
		done
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
							# build_info is the compile-time string of the SOURCE this
							# module was built from, not the identity of the loaded
							# module — label it so it is not read as one (review
							# R26-7); the srcversion section above answers that.
							if [ "$f" = "build_info" ]; then
								echo "-- build_info (checkout/toolchain string, not the loaded module's identity) --"
							else
								echo "-- $f --"
							fi
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
		run_host_self_tests
		echo ""
		echo "--- dmesg (driver-related lines, last 1000) ---"
		dmesg | grep -iE "sl4a|spi-amd|MSHW0231|MSHW0162|AMDI0060" | tail -1000
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
	stamp_installed_head "$SUDO"
	pass "Modules installed"

	echo ""
	rule
	echo "Reload with 'sudo modprobe -r sl4a-spi-hid sl4a-spi-amd' then"
	echo "'./tools/sl4a-touch.sh activate' to pick up the rebuilt modules"
	echo "(or reboot)."
	rule
}

# ── frame-hunt battery plan ─────────────────────────────────────────────
#
# One line per variant: `<profile>|<label>|<params>`. The profile selects the
# base parameter set (hunt_profile_params); the params are the extra module
# parameters appended to that variant's load line. The whole flow — unload,
# load, settle, snapshot, touch (evdev read + verdict), counter deltas, dmesg
# slice, artifact block, summary row — is driven from this array, so a new
# variant is a one-line addition and nothing else changes.
#
# RAW is today's raw profile (raw_mode=Y raw_input_beta=Y skip_getfeat=Y);
# STANDARD is the single-touch path (raw_mode=N). The retired pc/svs arms
# (acpi_probe_power_cycle × skip_vendor_stop, the P13-P16 waves) are NOT
# re-run: that question is answered (all four arms negative), and running it
# twice wastes a field trip. Revive it by adding a line here.
#
# The three `wire_double_opcode=1` combinations after its first row are the
# 2026-09-17 battery's one positive: raw wire_double_opcode=1 was the only
# variant that delivered a descriptor, so its read-side partners
# (read_frame_variant=2, raw_pre_desc_reg0=1, both) are the shapes that have
# never been run. Kept next to the row they extend, not appended at the end.
#
# raw raw_b1f8109_preset=1 sits right after the doubled-write row it gener-
# alises: the preset is the whole b1f8109 raw dialect behind one switch
# (doubled writes PLUS the pre-DONE reg0 reads, the poller give-up and the
# D2/D0-without-STOP teardown). It is the "restart from where it worked"
# candidate, so the operator reads it directly under the doubled-write row it
# extends rather than appended at the end.
HUNT_VARIANTS=(
	"raw|raw control|"
	"raw|raw raw_pre_desc_reg0=1|raw_pre_desc_reg0=1"
	"raw|raw raw_fallback_on_reset=1|raw_fallback_on_reset=1"
	"raw|raw raw_pre_desc_reg0=1+raw_fallback_on_reset=1|raw_pre_desc_reg0=1 raw_fallback_on_reset=1"
	"raw|raw read_frame_variant=2|read_frame_variant=2"
	"raw|raw wire_double_opcode=1|wire_double_opcode=1"
	"raw|raw raw_b1f8109_preset=1|raw_b1f8109_preset=1"
	"raw|raw wire_double_opcode=1+read_frame_variant=2|wire_double_opcode=1 read_frame_variant=2"
	"raw|raw wire_double_opcode=1+raw_pre_desc_reg0=1|wire_double_opcode=1 raw_pre_desc_reg0=1"
	"raw|raw wire_double_opcode=1+read_frame_variant=2+raw_pre_desc_reg0=1|wire_double_opcode=1 read_frame_variant=2 raw_pre_desc_reg0=1"
	"raw|raw wire_double_opcode=1+skip_vendor_stop=1|wire_double_opcode=1 skip_vendor_stop=1"
	"standard|std control|"
	"standard|std wire_double_opcode=1|wire_double_opcode=1"
	"standard|std skip_std_getfeat=1|skip_std_getfeat=1"
	"standard|std wire_double_opcode=1+skip_std_getfeat=1|wire_double_opcode=1 skip_std_getfeat=1"
)

hunt_variant_count() { printf '%s\n' "${#HUNT_VARIANTS[@]}"; }

# Width of the summary table's label column: the longest planned label, with a
# floor of 48. Computing it from the plan keeps the whole table aligned when a
# longer combination is added — the data-driven promise of "a new variant is a
# one-line addition and nothing else changes" would otherwise need a second
# edit here to stop the numeric columns drifting.
hunt_summary_label_width() {
	local w=48 v label
	for v in "${HUNT_VARIANTS[@]}"; do
		label="${v#*|}"; label="${label%%|*}"
		[ "${#label}" -gt "$w" ] && w="${#label}"
	done
	printf '%s' "$w"
}

# Base module parameters for a plan profile.
hunt_profile_params() {
	case "$1" in
		raw)      printf 'raw_mode=Y raw_input_beta=Y skip_getfeat=Y' ;;
		standard) printf 'raw_mode=N' ;;
		*)        return 1 ;;
	esac
}

# Every input event node visible here, by basename.
hunt_input_events() {
	local e
	for e in "$INPUT_SYSFS"/event*; do
		[ -e "$e" ] || continue
		basename "$e"
	done
}

# The input-event node that belongs to OUR panel, or nothing. Two shapes
# exist for the same hardware and only one of them carries the panel's name:
#   raw mode      -> the driver's own device, named "MSHW0231 Touchscreen";
#   standard HID  -> the hid-core device, named after the hid device
#                    ("spi 045E:0C19", hid->name in spi-hid-core.c), which
#                    matches none of MSHW/sl4a/Touchscreen.
# Matching on the node NAME alone (as this did) therefore found only the raw
# node and dropped every standard variant into the human y/n fallback while
# its node sat right there — the 2026-09-17 battery's "every standard row is
# human:no". So the name test is joined by an ancestry test: both nodes hang
# off the panel's controller (the raw device's parent IS the SPI device; the
# standard hid device's parent is too), so the resolved event-node device
# path is under $SYSFS_DIR. Name first (a raw run then prefers its own
# stream device over the HID one), then ancestry (finds the standard node).
# Matched by name, not by "new since boot": the battery reloads the driver
# between variants, so the node is present before and after each load and a
# set-difference would call the panel "old".
hunt_touch_event() {
	local e name dev panel
	for e in "$INPUT_SYSFS"/event*; do
		[ -e "$e" ] || continue
		name="$(cat "$e/device/name" 2>/dev/null || true)"
		case "$name" in
			*MSHW*|*sl4a*|*SL4A*|*[Tt]ouchscreen*)
				basename "$e"; return 0 ;;
		esac
	done
	panel="$(readlink -f "$SYSFS_DIR" 2>/dev/null || true)"
	[ -n "$panel" ] || return 1
	for e in "$INPUT_SYSFS"/event*; do
		[ -e "$e" ] || continue
		dev="$(readlink -f "$e/device" 2>/dev/null || true)"
		case "$dev" in
			"$panel"|"$panel"/*)
				basename "$e"; return 0 ;;
		esac
	done
	return 1
}

# Human-readable name of one input event node (for the artifact).
hunt_input_name() {
	cat "$INPUT_SYSFS/$1/device/name" 2>/dev/null || echo "?"
}

# Bounded read of one raw evdev node DURING the touch window. Writes
# "<bytes> <events>" to $2 — a file, so the read can run in the background
# underneath the countdown (an evdev client must be open while the finger is
# on the panel: events delivered with no reader are dropped). Reads at most
# $HUNT_EVDEV_MAX bytes or for $HUNT_TOUCH_SECS seconds, whichever comes
# first; the char device blocks until events arrive.
hunt_evdev_read() {
	local node="$1" out="$2" dev="$INPUT_DEV_ROOT/$1" tmp bytes
	printf '0 0\n' > "$out" 2>/dev/null || return 0
	[ -r "$dev" ] || return 0
	tmp="$(mktemp 2>/dev/null)" || return 0
	timeout "$HUNT_TOUCH_SECS" head -c "$HUNT_EVDEV_MAX" "$dev" > "$tmp" 2>/dev/null || true
	bytes="$(wc -c < "$tmp" 2>/dev/null || echo 0)"
	rm -f "$tmp" 2>/dev/null || true
	case "$bytes" in ''|*[!0-9]*) bytes=0 ;; esac
	printf '%s %s\n' "$bytes" "$((bytes / 24))" > "$out"
}

# The interesting module parameters, read back from the LIVE module so the
# artifact states what is loaded rather than what was requested. Every read
# goes through /sys/module (scoped to the sandbox's stub sysfs under test).
hunt_param_readback() {
	local p out=""
	for p in raw_mode raw_input_beta skip_getfeat read_frame_variant wire_double_opcode \
	         raw_pre_desc_reg0 raw_fallback_on_reset skip_vendor_stop \
	         raw_b1f8109_preset; do
		out="$out$p=$(cat "/sys/module/sl4a_spi_hid/parameters/$p" 2>/dev/null || echo '?') "
	done
	printf '%s' "$out"
}

# One field out of a protocol_stats blob ("key=value" lines).
hunt_stat_field() {
	printf '%s\n' "$1" | awk -F= -v k="$2" '$1==k {gsub(/ /,"",$2); print $2; exit}'
}

# after - before for one protocol_stats field. A non-numeric or unreadable
# field is read as 0, so a missing snapshot never prints a bogus delta; a
# counter that went backwards (impossible inside one load, but cheap to allow)
# would print a negative number rather than a fabricated 0.
hunt_delta() {
	local fa fb a b
	fa="$(hunt_stat_field "$1" "$3")"
	fb="$(hunt_stat_field "$2" "$3")"
	a="${fa:-0}"; b="${fb:-0}"
	case "$a" in ''|*[!0-9]*) a=0 ;; esac
	case "$b" in ''|*[!0-9]*) b=0 ;; esac
	printf '%d' "$((b - a))"
}

# Short verdict cell for the summary table. `rpt` is the rpt_desc delta.
# `descfocus` is set for the raw_b1f8109_preset row: whether b1f8109's dialect
# has landed is decided by the two descriptor-reply counters (device_desc,
# rpt_desc), so that one row surfaces BOTH explicitly — even at zero, where a
# generic "silent" would hide the very pair the row exists to read.
hunt_note() {
	local have="$1" dd="$2" rr="$3" rpt="${4:-0}" descfocus="${5:-}"
	if [ "$have" != 1 ]; then echo "no counters"; return; fi
	if [ "$descfocus" = 1 ]; then echo "device_desc +$dd rpt_desc +$rpt"; return; fi
	if [ "${dd:-0}" -gt 0 ]; then echo "descriptor +$dd"; return; fi
	if [ "${rr:-0}" -gt 0 ]; then echo "resets +$rr"; return; fi
	echo "silent"
}

cmd_hunt() {
	local OUT="" variant
	while [ $# -gt 0 ]; do
		case "$1" in
			-o|--output)
				[ $# -ge 2 ] || fail "-o requires a path"
				OUT="$2"
				shift 2 ;;
			-h|--help)
				usage
				exit 0 ;;
			*) fail "unknown hunt option: $1" ;;
		esac
	done
	[ "$(id -u)" = 0 ] || fail "hunt needs root (it unloads and loads the driver): run it with sudo"

	# -o feeds a root redirect. logs -o guards its path; this command took the
	# user's word for it, so `sudo hunt -o /etc/shadow` would truncate it.
	if [ -n "$OUT" ]; then
		case "$OUT" in -*) OUT="./$OUT" ;; esac
		# Never over one of the driver's own files: the modprobe conf and the
		# boot unit both START with our marker line, so the 'looks like ours'
		# test below would happily truncate them (double-blind leg, C4).
		local _real
		_real="$(readlink -f -- "$OUT" 2>/dev/null || echo "$OUT")"
		case "$_real" in
			"$MODPROBE_CONF"|"$SYSTEMD_UNIT"|"$INSTALLED_HEAD_STAMP")
				fail "refusing to write the hunt file over $OUT — that is one of the driver's own files, not a diagnostic" ;;
		esac
		[ -L "$OUT" ] && fail "refusing to write the hunt file through the symlink $OUT"
		if [ -e "$OUT" ]; then
			[ -f "$OUT" ] && [ ! -L "$OUT" ] || fail "$OUT is not a regular file; refusing to write it"
			if [ -s "$OUT" ]; then
				head -n 1 "$OUT" | grep -q SL4A_TouchScreen || \
					fail "$OUT does not look like a diagnostic file of ours; refusing to overwrite it"
			fi
		fi
	fi

	# Same place as the diagnostics bundle: next to the driver, not in /tmp
	# where it can be cleaned up before the file is even sent.
	[ -n "$OUT" ] || OUT="$REPO_DIR/sl4a-hunt-$(date +%Y%m%d-%H%M%S).txt"

	local SYSFS_DIR d
	# Any supported Surface panel, not just the SL4 one: a glob matching nothing
	# turned every counter unreadable and the verdict then blamed the device.
	# The glob is resolved without `ls`: with nullglob on a no-match pattern
	# vanishes, and `ls -d` then lists the current directory (SYSFS_DIR="."),
	# which is never empty and made the warning below dead code.
	for d in /sys/bus/spi/devices/*MSHW*; do
		if [ -d "$d" ]; then SYSFS_DIR="$d"; break; fi
	done

	info "Frame hunt battery: raw AND standard variants, one file, no commands for you. Leave the panel alone until asked."
	[ -n "$SYSFS_DIR" ] || warn "sysfs directory for the device not found — statistics will be missing"

	# The sweep is worthless against a stale module, and reloading does not
	# rebuild anything: DKMS built these files at install time. Compare the
	# stamp with the checkout and rebuild when they differ — without touching
	# the profile, which belongs to the user.
	local head_now head_built
	head_now="$(git -C "$REPO_DIR" rev-parse HEAD 2>/dev/null || echo unknown)"
	head_built="$(installed_head)"
	if [ "$head_built" != "$head_now" ]; then
		info "Installed modules came from ${head_built:0:8}, this checkout is ${head_now:0:8} — rebuilding first (30-60 s)."
		restage_and_rebuild
		pass "Rebuilt from $head_now"
	fi

	trap 'rc=$?; printf "\n\033[0;31m\xe2\x9c\x97 hunt stopped at line $LINENO (rc=$rc)\033[0m\n" >&3; printf "  artifact so far: %s\n" "$OUT" >&3; printf "  send that file: it ends where the error is\n" >&3; exit $rc' ERR
	# fd 3 is the terminal itself, duplicated before the sweep redirects
	# both streams into the artifact. Progress and the error trap write
	# there: the file must stay complete, but a person watching the screen
	# has to see movement — and see where it stopped if it stops.
	exec 3>&2
	info "Full sweep goes to: $OUT"
	info "$(hunt_variant_count) variants, ~20 s each (reload + settle + touch window); the verdicts and the summary are printed here at the end."

	{
		echo "=== SL4A_TouchScreen frame hunt ==="
		echo "Generated: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
		echo "Battery plan: $(hunt_variant_count) variants (raw + standard), one touch verdict each"
		echo "Modules built from revision: $(installed_head)  (checkout: $head_now)"
		# What the OS itself sees right now, before anything is unloaded: a
		# panel that never binds, or an input device that never registers, is
		# a different problem from one that binds and stays quiet.
		local acpi_id drv input_state
		acpi_id="$(touchscreen_acpi_id 2>/dev/null)" || acpi_id="none"
		if [ -n "$SYSFS_DIR" ]; then
			drv="$(bound_driver "$SYSFS_DIR" 2>/dev/null)" || drv="none"
		else
			# "none" here reads as "no driver is bound" while the panel's
			# sysfs directory was never found: nothing was probed, and a
			# reader must not blame the OS binding for a wire problem
			# (P3 wave).
			drv="(sysfs dir not found, not probed)"
		fi
		if grep -qi MSHW /proc/bus/input/devices 2>/dev/null; then
			input_state="registered"
		else
			input_state="not registered"
		fi
		echo "-- OS binding (before the sweep) --"
		echo "ACPI device: $acpi_id"
		echo "bound driver: $drv"
		echo "input device (MSHW in /proc/bus/input/devices): $input_state"
		echo ""

		# The full battery, driven from HUNT_VARIANTS: every field test the
		# campaign designed, raw and standard, one touch verdict each. The
		# retired pc/svs arms are not re-run (see the plan above).
		local vi=0 total rows=() row
		total="$(hunt_variant_count)"
		for variant in "${HUNT_VARIANTS[@]}"; do
			vi=$((vi + 1))
			local profile label vparams rest base
			profile="${variant%%|*}"
			rest="${variant#*|}"
			label="${rest%%|*}"
			vparams="${rest#*|}"
			base="$(hunt_profile_params "$profile")" || base=""

			echo "--- variant $vi/$total: $label (profile=$profile params='${vparams:-(none)}') ---"
			printf '\n[%d/%d] %s: reloading the driver, debug level 3\n' "$vi" "$total" "$label" >&3

			local dmesg_mark
			dmesg_mark="$(dmesg 2>/dev/null | wc -l)"
			modprobe -r sl4a_spi_hid sl4a_spi_amd 2>/dev/null || true
			sleep 1
			# The input nodes with the driver UNLOADED: whatever this load
			# registers afterwards is "new since before this load".
			local ev_before
			ev_before="$(hunt_input_events)"
			# The RX-region peek logs at the CONTROLLER's debug_trace=3
			# (spi-amd.c) — its own module param, distinct from the core's
			# sl4a_debug_level. Loading the controller bare kept the one
			# line this sweep exists to capture out of every artifact.
			modprobe sl4a_spi_amd debug_trace=3 2>/dev/null || true
			# The variant's own parameter set on the command line: the profile
			# base plus the plan's extra params. /etc/modprobe.d is never
			# edited — the sweep leaves the installed profile as it found it.
			# shellcheck disable=SC2086
			modprobe sl4a_spi_hid $base $vparams sl4a_debug_level=3 2>/dev/null || true
			sleep 10
			echo "running variant: profile=$profile params='${vparams:-(none)}' base='$base' debug level $(cat /sys/module/sl4a_spi_hid/parameters/sl4a_debug_level 2>/dev/null) controller trace $(cat /sys/module/sl4a_spi_amd/parameters/debug_trace 2>/dev/null || echo '?')"
			# The line above echoes what was REQUESTED — a load that failed
			# leaves it looking the same. Read the live parameters back so
			# the artifact states what is actually loaded; the module being
			# absent is the explicit failure marker (P3 wave: the intent
			# echo alone let a failed load masquerade as a productive one).
			if [ -d /sys/module/sl4a_spi_hid ]; then
				echo "loaded params (read back): $(hunt_param_readback)(sysfs spells booleans Y/N)"
			else
				echo "loaded params (read back): MODULE NOT LOADED — nothing was measured for this variant"
			fi

			# Snapshot at settle: counters, sequence state, and the input
			# devices this load registered (which touch node is new).
			local ev_after touch_node touch_new
			ev_after="$(hunt_input_events)"
			touch_node="$(hunt_touch_event)" || touch_node=""
			touch_new="?"
			if [ -n "$touch_node" ]; then
				case " $ev_before " in
					*" $touch_node "*) touch_new="no (present before this load)" ;;
					*)                 touch_new="yes" ;;
				esac
			fi
			echo "-- settle snapshot --"
			echo "ready: $(cat "$SYSFS_DIR/ready" 2>/dev/null || echo '(unavailable)')"
			echo "seq_state: $(cat "$SYSFS_DIR/seq_state" 2>/dev/null || echo '(unavailable)')"
			echo "-- input devices (eventN + name) --"
			local e
			for e in $ev_after; do
				if [ "$e" = "$touch_node" ]; then
					echo "$e $(hunt_input_name "$e")   <-- touch device (new since before this load: $touch_new)"
				else
					echo "$e $(hunt_input_name "$e")"
				fi
			done
			[ -n "$ev_after" ] || echo "(no input event nodes)"
			local stats_before
			stats_before="$(cat "$SYSFS_DIR/protocol_stats" 2>/dev/null || true)"
			echo "-- protocol_stats (after load, before touch) --"
			if [ -n "$stats_before" ]; then printf '%s\n' "$stats_before"; else echo "(unavailable)"; fi

			# Touch window. An evdev client must be open WHILE the finger is
			# down (events delivered with no reader are dropped), so the
			# bounded read runs in the background under the countdown. The
			# human y/n answer is the fallback ONLY when there is no node to
			# read — and never silently: the artifact states which fallback
			# reason applied (no node registered by this load, or a node whose
			# char device is not readable). A bare "human:no" hid the standard
			# profile's node-naming bug for a whole battery (2026-09-17).
			local ev_tmp ev_bytes=0 ev_events=0 touch_cell ev_chardev
			ev_tmp="$(mktemp 2>/dev/null)" || ev_tmp="/tmp/sl4a-hunt-ev.$$"
			ev_chardev="$INPUT_DEV_ROOT/${touch_node:-none}"
			if [ -n "$touch_node" ] && [ -r "$ev_chardev" ]; then
				hunt_evdev_read "$touch_node" "$ev_tmp" &
				local ev_pid=$!
				for _s in 6 5 4 3 2 1; do
					printf '\r     >>> TOUCH THE PANEL NOW (tocca il pannello) — %d <<<   ' "$_s" >&3
					sleep 1
				done
				wait "$ev_pid" 2>/dev/null || true
				printf '\r%80s\r' '' >&3
				read -r ev_bytes ev_events < "$ev_tmp" 2>/dev/null || true
				case "$ev_bytes" in ''|*[!0-9]*) ev_bytes=0 ;; esac
				case "$ev_events" in ''|*[!0-9]*) ev_events=0 ;; esac
				touch_cell="$ev_events events"
				echo "-- touch (evdev read on $touch_node) --"
				echo "evdev $touch_node: $ev_bytes bytes, $ev_events events (${HUNT_TOUCH_SECS}s window)"
			else
				for _s in 6 5 4 3 2 1; do
					printf '\r     >>> TOUCH THE PANEL NOW (tocca il pannello) — %d <<<   ' "$_s" >&3
					sleep 1
				done
				printf '\r%80s\r' '' >&3
				printf '     evdev node unavailable — did you touch the panel? [y/N] ' >&3
				local ans=""
				read -rt "$HUNT_TOUCH_SECS" -n 1 ans 2>/dev/null || ans=""
				echo "" >&3
				case "$ans" in
					y|Y) touch_cell="human:yes" ;;
					*)   touch_cell="human:no" ;;
				esac
				if [ -n "$touch_node" ]; then
					# A node exists but its char device is not readable: name
					# the path, do not blame the panel.
					echo "-- touch (evdev node $touch_node present but not readable) --"
					echo "no readable char device at $ev_chardev for $touch_node"
					echo "human fallback (evdev node '$touch_node' not readable): $touch_cell"
				else
					# No node registered by THIS load. In standard mode that is
					# the honest answer whenever the handshake never delivered
					# a descriptor (no HID device -> no input node); state it
					# rather than turning it silently into a y/n.
					echo "-- touch (no evdev node for this panel) --"
					echo "no input event node under the panel's controller (${SYSFS_DIR:-sysfs not found}) — this load registered none"
					echo "human fallback (evdev node '${touch_node:-not found}'): $touch_cell"
				fi
			fi
			rm -f "$ev_tmp" 2>/dev/null || true

			# Snapshot again and compute the deltas the summary reports.
			local stats_after have d_rr d_dd d_rpt d_data d_irq descfocus note
			stats_after="$(cat "$SYSFS_DIR/protocol_stats" 2>/dev/null || true)"
			echo "-- protocol_stats (after touch) --"
			if [ -n "$stats_after" ]; then printf '%s\n' "$stats_after"; else echo "(unavailable)"; fi
			have=0
			if [ -n "$stats_after" ]; then have=1; fi
			d_rr=0; d_dd=0; d_rpt=0; d_data=0; d_irq=0
			if [ "$have" = 1 ]; then
				d_rr="$(hunt_delta "$stats_before" "$stats_after" reset_rsp)"
				d_dd="$(hunt_delta "$stats_before" "$stats_after" device_desc)"
				d_rpt="$(hunt_delta "$stats_before" "$stats_after" rpt_desc)"
				d_data="$(hunt_delta "$stats_before" "$stats_after" data)"
				d_irq="$(hunt_delta "$stats_before" "$stats_after" irq_count)"
			fi
			# The raw_b1f8109_preset row is singled out in the summary note: the
			# two descriptor-reply counters (device_desc, rpt_desc) say whether
			# b1f8109's dialect lands, so that row surfaces both explicitly.
			descfocus=""
			case " $vparams " in
				*" raw_b1f8109_preset=1 "*) descfocus=1 ;;
			esac
			echo "-- deltas (after - before) --"
			if [ "$have" = 1 ]; then
				echo "reset_rsp=$d_rr device_desc=$d_dd data=$d_data irq_count=$d_irq"
			else
				echo "(no counters — deltas not measured)"
			fi

			echo "-- dmesg, this load only (the read bytes are here)"
			# `|| true` is load-bearing: under `set -e -o pipefail` a grep that
			# matches nothing (every modprobe in this variant failed, say) aborted
			# hunt after it had unloaded the driver and before putting it back.
			local all win fb wr_fb
			fb=""
			wr_fb=""
			# The spi-amd prefix matters: the controller layer logs the read
			# regions (peek) under its own name, and filtering it out threw
			# away exactly the line the RX-region question needs answered.
			all="$(dmesg 2>/dev/null | tail -n +"$((dmesg_mark + 1))" | grep -iE "sl4a_spi_hid|spi-amd")" || true
			win="$(printf '%s\n' "$all" | tail -n 60)" || true
			if [ -n "$win" ]; then
				echo "$win"
			else
				# Ring buffer wrapped between the mark and now: the arithmetic
				# yields nothing while the lines still exist (shifted out of
				# the slice, not out of the buffer). Say so, then show the
				# newest driver lines — which can span loads, so the write
				# search below labels where it read.
				echo "(no lines after the mark — the ring may have wrapped; last 60 driver lines, possibly from an earlier load)"
				fb="$(dmesg 2>/dev/null | grep -iE "sl4a_spi_hid|spi-amd" | tail -n 60)" || true
				if [ -n "$fb" ]; then printf '%s\n' "$fb"; fi
			fi
			echo ""
			# The proof of what actually went on the wire for this load: the first
			# control write, hex and all - doubled vs single is its second byte.
			local wr
			# Searched over the WHOLE slice for this load, not the 60-line
			# tail kept for the reader: at debug level 3 a productive load
			# logs hundreds of frame lines after the first control write,
			# and the tail-only grep lost this line exactly when the load
			# worked (P3 wave). Absence is stated, not silent.
			wr="$(printf '%s\n' "$all" | grep -m1 'write op=0x02')" || wr=""
			if [ -z "$wr" ] && [ -n "$fb" ]; then
				# The slice was empty (ring wrapped): the write may sit in the
				# fallback lines shown above. Report it from there — labelled,
				# because those lines can span loads (P14 wave, F7).
				wr_fb="$(printf '%s\n' "$fb" | grep -m1 'write op=0x02')" || wr_fb=""
			fi
			if [ -n "$wr" ]; then
				echo "first write on the wire: $wr"
			elif [ -n "$wr_fb" ]; then
				echo "first write on the wire (from the wrapped ring — may belong to an earlier load): $wr_fb"
			else
				echo "first write on the wire: (none in this load's log)"
			fi
			echo "VERDICT ($label): $(hunt_verdict "$label" "$have" "$d_dd" "$d_rr")"
			echo ""
			note="$(hunt_note "$have" "$d_dd" "$d_rr" "$d_rpt" "$descfocus")"
			rows+=("$label|+$d_dd|+$d_data|+$d_rr|$touch_cell|$note")
			printf '     [%d/%d] %s done\n' "$vi" "$total" "$label" >&3
		done

		# The "where are we" the whole battery exists for: one row per variant,
		# before the self-tests, so a reader sees the shape at a glance.
		echo ""
		echo "=== SUMMARY ($total variants) ==="
		local lw
		lw="$(hunt_summary_label_width)"
		printf "%-${lw}s | %-11s | %-4s | %-9s | %-15s | %s\n" \
			"variant" "device_desc" "data" "reset_rsp" "touch(evdev events)" "note"
		local r_label r_dd r_data r_rr r_touch r_note
		for row in "${rows[@]}"; do
			IFS='|' read -r r_label r_dd r_data r_rr r_touch r_note <<< "$row"
			printf "%-${lw}s | %-11s | %-4s | %-9s | %-15s | %s\n" \
				"$r_label" "$r_dd" "$r_data" "$r_rr" "$r_touch" "$r_note"
		done
		echo ""

		run_host_self_tests
		echo ""
		echo "--- module objects (which build ran) ---"
		modinfo sl4a_spi_hid 2>/dev/null | head -4 || true
		echo ""
	} >"$OUT" 2>&1
	chmod 644 "$OUT" 2>/dev/null || true

	# Leave the machine exactly as it was: the installed profile, no debug level.
	modprobe -r sl4a_spi_hid sl4a_spi_amd 2>/dev/null || true
	# In a subshell on purpose: cmd_activate's own fail() would 'exit 1' the
	# whole sweep after the artifact was complete (the restore must never be
	# able to kill the run it is restoring from).
	# In a subshell, with '|| true' OUTSIDE it: cmd_activate's own fail() calls
	# exit 1, which leaves the subshell immediately and can only be caught from
	# the outside. Inside, the || true never ran at all.
	( cmd_activate >/dev/null 2>&1 ) || true

	pass "Wrote $OUT (one file, all $total variants — raw + standard)"
	grep -h '^VERDICT' "$OUT" 2>/dev/null || true
	echo ""
	echo "Send that file: it already contains every variant with its verdict and the summary table."
}

# Turns one variant's counter deltas into the verdict line the file carries.
# "No counters" is not "counters at zero": when the panel's sysfs is missing
# every counter is unreadable, and a diagnostic that wrote "silent" anyway
# would claim a measurement that was never taken.
hunt_verdict() {
	local label="$1" have="$2" dd="$3" rr="$4"
	if [ "$have" != 1 ]; then
		echo "variant $label: NO COUNTERS READ (sysfs not found for this panel) — nothing was measured"
		return 0
	fi
	if [ "${dd:-0}" -gt 0 ]; then
		echo "variant $label DELIVERED A DESCRIPTOR (device_desc +$dd) — this is the shape"
	elif [ "${rr:-0}" -gt 0 ]; then
		echo "variant $label gets answers from the device (reset_rsp +$rr)"
	else
		echo "variant $label is silent (no new RESET_RSP, no descriptor)"
	fi
}

# ── soak: non-interactive stability observation ──────────────────────────
#
# Unlike hunt this command NEVER unloads or reloads the driver: the panel is
# assumed to be streaming live, and every sample is a read (cat) of the
# running module's sysfs attributes plus a dmesg slice. No touch prompts, no
# questions, nothing loaded or unloaded — safe to leave running next to a
# live session. The verdict is PASS only when ready and seq_state 4 (DONE)
# hold steady for the whole run AND either touch data flows with zero drops
# or the IRQ line stays flat with no storm (no drops, no device resets).
cmd_soak() {
	local OUT="" MINUTES="" PROFILE="current" INTERACTIVE=0
	while [ $# -gt 0 ]; do
		case "$1" in
			--minutes)
				[ $# -ge 2 ] || fail "--minutes requires a number of minutes"
				[ -n "$2" ] || fail "--minutes requires a non-empty number of minutes"
				MINUTES="$2"
				shift 2 ;;
			--minutes=*)
				MINUTES="${1#--minutes=}"
				shift ;;
			-o|--output)
				[ $# -ge 2 ] || fail "-o requires a path"
				[ -n "$2" ] || fail "-o requires a non-empty path"
				OUT="$2"
				shift 2 ;;
			--profile)
				[ $# -ge 2 ] || fail "--profile requires one of raw, standard or current"
				[ -n "$2" ] || fail "--profile requires a non-empty profile"
				PROFILE="$2"
				shift 2 ;;
			--profile=*)
				PROFILE="${1#--profile=}"
				shift ;;
			--interactive)
				INTERACTIVE=1
				shift ;;
			-h|--help)
				usage
				exit 0 ;;
			*) fail "unknown soak option: $1 (see --help)" ;;
		esac
	done
	case "$MINUTES" in ''|*[!0-9]*|0) fail "soak needs --minutes N with N a positive whole number (got '${MINUTES:-none}')" ;; esac
	case "$PROFILE" in raw|standard|current) ;; *) fail "unknown soak profile: $PROFILE (raw, standard or current)" ;; esac

	# -o feeds a root redirect: the same guards hunt carries (symlink,
	# non-regular file, foreign file, dash-leading path, and never one of
	# the driver's own files).
	if [ -n "$OUT" ]; then
		case "$OUT" in -*) OUT="./$OUT" ;; esac
		local _real
		_real="$(readlink -f -- "$OUT" 2>/dev/null || echo "$OUT")"
		case "$_real" in
			"$MODPROBE_CONF"|"$SYSTEMD_UNIT"|"$INSTALLED_HEAD_STAMP")
				fail "refusing to write the soak file over $OUT — that is one of the driver's own files, not a diagnostic" ;;
		esac
		[ -L "$OUT" ] && fail "refusing to write the soak file through the symlink $OUT"
		if [ -e "$OUT" ]; then
			[ -f "$OUT" ] && [ ! -L "$OUT" ] || fail "$OUT is not a regular file; refusing to write it"
			if [ -s "$OUT" ]; then
				head -n 1 "$OUT" | grep -q SL4A_TouchScreen || \
					fail "$OUT does not look like a diagnostic file of ours; refusing to overwrite it"
			fi
		fi
	fi

	# Same place as the hunt battery: next to the driver, not in /tmp
	# where it can be cleaned up before the file is even sent.
	[ -n "$OUT" ] || OUT="$REPO_DIR/sl4a-soak-$(date +%Y%m%d-%H%M%S).txt"

	# Read-only by construction: no elevation (a sudo password prompt
	# would itself be an interactive question), no module loads, no
	# reloads. Under sudo the dmesg slices are populated; without root
	# they come out empty while the counters still work.
	# Seconds between samples: 30 unless the sandbox says otherwise
	# (the same SL4A_-override pattern HUNT_TOUCH_SECS uses for tests).
	local SOAK_INTERVAL="${SL4A_SOAK_INTERVAL_SECS:-30}"
	case "$SOAK_INTERVAL" in ''|*[!0-9]*) fail "SL4A_SOAK_INTERVAL_SECS must be a non-negative whole number of seconds (got '$SOAK_INTERVAL')" ;; esac
	local NSAMPLES
	if [ "$SOAK_INTERVAL" -eq 0 ]; then
		NSAMPLES="$MINUTES"
	else
		NSAMPLES=$(( 10#$MINUTES * 60 / SOAK_INTERVAL ))
	fi
	[ "$NSAMPLES" -ge 1 ] || NSAMPLES=1

	local SYSFS_DIR d
	# Same discovery as hunt (kept literal so the sandbox staging
	# rewrites it the same way): any supported Surface panel.
	for d in /sys/bus/spi/devices/*MSHW*; do
		if [ -d "$d" ]; then SYSFS_DIR="$d"; break; fi
	done

	info "Soak: observing the RUNNING driver for ${MINUTES} min ($NSAMPLES samples every ${SOAK_INTERVAL}s). Nothing is unloaded, reloaded or asked."
	[ -n "$SYSFS_DIR" ] || warn "sysfs directory for the device not found — statistics will be missing"

	trap 'rc=$?; printf "\n\033[0;31m\xe2\x9c\x97 soak stopped at line $LINENO (rc=$rc)\033[0m\n" >&3; printf "  artifact so far: %s\n" "$OUT" >&3; printf "  send that file: it ends where the error is\n" >&3; exit $rc' ERR
	# fd 3 is the terminal itself, duplicated before the run redirects
	# both streams into the artifact (the same arrangement hunt uses).
	exec 3>&2
	info "Soak goes to: $OUT"
	printf '%s samples, one every %ss; the verdict and the summary are printed here at the end.\n' "$NSAMPLES" "$SOAK_INTERVAL" >&3

	{
		echo "=== SL4A_TouchScreen soak ==="
		echo "Generated: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
		echo "Soak plan: ${MINUTES} min, $NSAMPLES samples every ${SOAK_INTERVAL}s (observe only — no unload, no reload, no prompts)"
		echo "Modules built from revision: $(installed_head)  (checkout: $(git -C "$REPO_DIR" rev-parse HEAD 2>/dev/null || echo unknown))"
		# What the OS itself sees right now (observe only): a panel that
		# never binds, or an input device that never registers, is a
		# different problem from one that binds and goes quiet.
		local acpi_id drv input_state
		acpi_id="$(touchscreen_acpi_id 2>/dev/null)" || acpi_id="none"
		if [ -n "$SYSFS_DIR" ]; then
			drv="$(bound_driver "$SYSFS_DIR" 2>/dev/null)" || drv="none"
		else
			drv="(sysfs dir not found, not probed)"
		fi
		if grep -qi MSHW /proc/bus/input/devices 2>/dev/null; then
			input_state="registered"
		else
			input_state="not registered"
		fi
		echo "-- OS binding (at soak start, observe only) --"
		echo "ACPI device: $acpi_id"
		echo "bound driver: $drv"
		echo "input device (MSHW in /proc/bus/input/devices): $input_state"
		echo ""

		# The requested profile only selects what the run is COMPARED
		# against: soak never reloads, so a mismatch is reported and the
		# run observes whatever is actually loaded.
		local live_raw cfg
		live_raw="$(loaded_raw_mode 2>/dev/null)" || live_raw="(module not loaded)"
		cfg="$(modprobe_profile)"
		echo "-- profile (observe only — soak never reloads) --"
		echo "requested: $PROFILE"
		echo "running now (raw_mode): $live_raw"
		echo "configured for next boot: $cfg"
		if [ -d /sys/module/sl4a_spi_hid ]; then
			echo "loaded params (read back): $(hunt_param_readback)(sysfs spells booleans Y/N)"
		else
			echo "loaded params (read back): MODULE NOT LOADED — nothing was measured for this run"
		fi
		case "$PROFILE/$live_raw" in
			raw/Y|standard/N|current/*) ;;
			*) echo "note: requested $PROFILE but the running module reports raw_mode=$live_raw — observing it anyway (no reload)" ;;
		esac
		echo ""

		# The pinch phase runs only as a bounded, question-free evdev
		# window on a real terminal. Anything else — the default, or
		# --interactive with stdin redirected — skips it and says so.
		if [ "$INTERACTIVE" -eq 1 ] && [ -t 0 ]; then
			echo "-- pinch phase (interactive terminal: one bounded evdev window, nothing to answer) --"
			local pinch_node pinch_tmp pinch_bytes pinch_events
			pinch_node="$(hunt_touch_event)" || pinch_node=""
			if [ -n "$pinch_node" ] && [ -r "$INPUT_DEV_ROOT/$pinch_node" ]; then
				printf 'pinch observation: use two fingers on the panel (%ss window, nothing to answer)\n' "$HUNT_TOUCH_SECS" >&3
				pinch_tmp="$(mktemp 2>/dev/null)" || pinch_tmp="/tmp/sl4a-soak-pinch.$$"
				hunt_evdev_read "$pinch_node" "$pinch_tmp"
				pinch_bytes="$(awk '{print $1}' "$pinch_tmp" 2>/dev/null || echo 0)"
				pinch_events="$(awk '{print $2}' "$pinch_tmp" 2>/dev/null || echo 0)"
				case "$pinch_bytes" in ''|*[!0-9]*) pinch_bytes=0 ;; esac
				case "$pinch_events" in ''|*[!0-9]*) pinch_events=0 ;; esac
				echo "pinch window on $pinch_node: $pinch_bytes bytes, $pinch_events events (${HUNT_TOUCH_SECS}s window)"
				rm -f "$pinch_tmp" 2>/dev/null || true
			else
				echo "pinch phase: evdev unavailable (no readable node for this panel) — skipped"
			fi
		elif [ "$INTERACTIVE" -eq 1 ]; then
			echo "-- pinch phase: SKIP (--interactive needs a terminal; stdin is not a tty) --"
		else
			echo "-- pinch phase: SKIP (non-interactive soak; re-run with --interactive on a tty to observe it) --"
		fi
		echo ""

		# Baseline before the first interval: whole-run deltas are
		# measured from here, per-interval deltas from sample to sample.
		local base_stats base_ready base_seq
		base_ready="$(cat "$SYSFS_DIR/ready" 2>/dev/null || echo '(unavailable)')"
		base_seq="$(cat "$SYSFS_DIR/seq_state" 2>/dev/null || echo '(unavailable)')"
		base_stats="$(cat "$SYSFS_DIR/protocol_stats" 2>/dev/null || true)"
		echo "-- baseline (before interval 1) --"
		echo "ready: $base_ready"
		echo "seq_state: $base_seq"
		if [ -n "$base_stats" ]; then printf '%s\n' "$base_stats"; else echo "(protocol_stats unavailable)"; fi
		echo ""

		local i ready_now seq_now cur_stats prev_stats dmesg_mark all win
		local have=0 ready_bad=0 ready_bad_at="" seq_bad=0 seq_bad_at=""
		local ev_total_bytes=0 ev_total_events=0 ev_unavail=0
		local s_n=() s_ready=() s_seq=() s_data=() s_irq=() s_drops=() s_ev=()
		[ -n "$base_stats" ] && have=1
		prev_stats="$base_stats"
		for i in $(seq 1 "$NSAMPLES"); do
			printf '\n[%d/%d] sample %d: reading counters (no reload)\n' "$i" "$NSAMPLES" "$i" >&3
			dmesg_mark="$(dmesg 2>/dev/null | wc -l | tr -d ' ' || echo 0)"
			case "$dmesg_mark" in ''|*[!0-9]*) dmesg_mark=0 ;; esac

			# The evdev client must be open WHILE the interval elapses
			# (events delivered with no reader are dropped), so the
			# bounded read runs in the background under the sleep —
			# with no countdown and no questions. When there is no
			# node to read, that is recorded, not asked about.
			local touch_node ev_tmp ev_pid ev_bytes ev_events ev_cell
			touch_node="$(hunt_touch_event)" || touch_node=""
			ev_tmp="$(mktemp 2>/dev/null)" || ev_tmp="/tmp/sl4a-soak-ev.$$"
			if [ -n "$touch_node" ] && [ -r "$INPUT_DEV_ROOT/$touch_node" ]; then
				hunt_evdev_read "$touch_node" "$ev_tmp" &
				ev_pid=$!
			else
				ev_pid=""
			fi
			if [ "$SOAK_INTERVAL" -gt 0 ]; then sleep "$SOAK_INTERVAL"; fi
			if [ -n "$ev_pid" ]; then
				wait "$ev_pid" 2>/dev/null || true
				ev_bytes="$(awk '{print $1}' "$ev_tmp" 2>/dev/null || echo 0)"
				ev_events="$(awk '{print $2}' "$ev_tmp" 2>/dev/null || echo 0)"
				case "$ev_bytes" in ''|*[!0-9]*) ev_bytes=0 ;; esac
				case "$ev_events" in ''|*[!0-9]*) ev_events=0 ;; esac
				ev_total_bytes=$((ev_total_bytes + ev_bytes))
				ev_total_events=$((ev_total_events + ev_events))
				ev_cell="$ev_events events"
			else
				ev_bytes=0; ev_events=0
				ev_cell="evdev unavailable"
				ev_unavail=$((ev_unavail + 1))
			fi
			rm -f "$ev_tmp" 2>/dev/null || true

			ready_now="$(cat "$SYSFS_DIR/ready" 2>/dev/null || echo '(unavailable)')"
			seq_now="$(cat "$SYSFS_DIR/seq_state" 2>/dev/null || echo '(unavailable)')"
			cur_stats="$(cat "$SYSFS_DIR/protocol_stats" 2>/dev/null || true)"
			[ -n "$cur_stats" ] && have=1
			case "$ready_now" in
				ready) ;;
				*) ready_bad=$((ready_bad + 1)); [ -n "$ready_bad_at" ] || ready_bad_at="$i:$ready_now" ;;
			esac
			case "$seq_now" in
				4) ;;
				*) seq_bad=$((seq_bad + 1)); [ -n "$seq_bad_at" ] || seq_bad_at="$i:$seq_now" ;;
			esac

			local d_data d_irq d_drops
			d_data="$(hunt_delta "$prev_stats" "$cur_stats" data)"
			d_irq="$(hunt_delta "$prev_stats" "$cur_stats" irq_count)"
			d_drops="$(hunt_delta "$prev_stats" "$cur_stats" frames_dropped)"
			prev_stats="$cur_stats"

			echo "--- interval $i/$NSAMPLES (${SOAK_INTERVAL}s, observe only) ---"
			echo "ready: $ready_now"
			echo "seq_state: $seq_now"
			if [ -n "$cur_stats" ]; then printf '%s\n' "$cur_stats"; else echo "(protocol_stats unavailable)"; fi
			echo "deltas (this interval): data=+$d_data irq_count=+$d_irq frames_dropped=+$d_drops"
			if [ "$ev_cell" != "evdev unavailable" ]; then
				echo "evdev $touch_node: $ev_bytes bytes, $ev_events events (${SOAK_INTERVAL}s background window)"
			else
				echo "evdev: evdev unavailable (no readable node for this panel)"
			fi
			echo "-- dmesg, this interval only --"
			# `|| true` is load-bearing (see hunt): under
			# `set -e -o pipefail` a grep that matches nothing would
			# abort the run mid-soak.
			all="$(dmesg 2>/dev/null | tail -n +"$((dmesg_mark + 1))" | grep -iE "sl4a_spi_hid|spi-amd")" || true
			win="$(printf '%s\n' "$all" | tail -n 60)" || true
			if [ -n "$win" ]; then
				echo "$win"
			else
				echo "(no driver lines in this interval)"
			fi
			echo ""
			s_n+=("$i"); s_ready+=("$ready_now"); s_seq+=("$seq_now")
			s_data+=("$d_data"); s_irq+=("$d_irq"); s_drops+=("$d_drops"); s_ev+=("$ev_cell")
			printf '     [%d/%d] sample %d done\n' "$i" "$NSAMPLES" "$i" >&3
		done

		# Whole-run deltas decide the verdict: ready and seq_state 4
		# (DONE) must hold every sample, AND either touch data flows
		# with zero drops or the IRQ line stays flat with no storm (no
		# drops, no device resets — an untouched but healthy panel
		# idles instead of streaming).
		local t_data t_drops t_irq t_reset t_ddesc t_rdesc
		t_data="$(hunt_delta "$base_stats" "$prev_stats" data)"
		t_drops="$(hunt_delta "$base_stats" "$prev_stats" frames_dropped)"
		t_irq="$(hunt_delta "$base_stats" "$prev_stats" irq_count)"
		t_reset="$(hunt_delta "$base_stats" "$prev_stats" reset_rsp)"
		t_ddesc="$(hunt_delta "$base_stats" "$prev_stats" device_desc)"
		t_rdesc="$(hunt_delta "$base_stats" "$prev_stats" rpt_desc)"
		local verdict verdict_reason
		if [ "$have" != 1 ]; then
			verdict="FAIL"
			verdict_reason="no counters read (sysfs not found for this panel) — nothing was measured"
		elif [ "$ready_bad" -gt 0 ]; then
			verdict="FAIL"
			verdict_reason="ready unstable ($ready_bad/$NSAMPLES samples not 'ready', first at sample $ready_bad_at)"
		elif [ "$seq_bad" -gt 0 ]; then
			verdict="FAIL"
			verdict_reason="seq_state left 4 (DONE) ($seq_bad/$NSAMPLES samples, first at sample $seq_bad_at)"
		elif [ "$t_data" -gt 0 ] && [ "$t_drops" -eq 0 ]; then
			verdict="PASS"
			verdict_reason="streaming: data +$t_data with zero drops (irq +$t_irq, resets +$t_reset, evdev $ev_total_events events)"
		elif [ "$t_irq" -eq 0 ] && [ "$t_drops" -eq 0 ] && [ "$t_reset" -eq 0 ]; then
			verdict="PASS"
			verdict_reason="idle-stable: irq flat (+0), zero drops, zero resets over ${MINUTES} min"
		else
			verdict="FAIL"
			if [ "$t_drops" -gt 0 ]; then
				verdict_reason="frames dropped +$t_drops over the run (data +$t_data, irq +$t_irq)"
			elif [ "$t_reset" -gt 0 ]; then
				verdict_reason="device resets +$t_reset over the run (data +$t_data, irq +$t_irq)"
			else
				verdict_reason="no touch data delivered (data +$t_data) while irqs moved (irq +$t_irq)"
			fi
		fi
		echo "VERDICT (soak ${MINUTES}min profile=$PROFILE): $verdict — $verdict_reason"
		echo ""
		echo "whole-run deltas: data=+$t_data irq_count=+$t_irq frames_dropped=+$t_drops reset_rsp=+$t_reset device_desc=+$t_ddesc rpt_desc=+$t_rdesc"
		echo "evdev totals: $ev_total_bytes bytes, $ev_total_events events ($ev_unavail/$NSAMPLES intervals evdev unavailable)"
		echo ""
		echo "=== SUMMARY ($NSAMPLES samples over ${MINUTES} min) ==="
		printf "%-8s | %-12s | %-9s | %-9s | %-9s | %-9s | %s\n" \
			"sample" "ready" "seq_state" "data+" "irq+" "drops+" "evdev"
		local idx
		for idx in "${!s_n[@]}"; do
			printf "%-8s | %-12s | %-9s | +%-8s | +%-8s | +%-8s | %s\n" \
				"${s_n[$idx]}" "${s_ready[$idx]}" "${s_seq[$idx]}" \
				"${s_data[$idx]}" "${s_irq[$idx]}" "${s_drops[$idx]}" "${s_ev[$idx]}"
		done
		echo ""

		run_host_self_tests
		echo ""
		echo "--- module objects (which build ran) ---"
		modinfo sl4a_spi_hid 2>/dev/null | head -4 || true
		echo ""
	} >"$OUT" 2>&1
	chmod 644 "$OUT" 2>/dev/null || true

	pass "Wrote $OUT (soak: $NSAMPLES samples over ${MINUTES} min, observe-only — nothing was unloaded or reloaded)"
	grep -h '^VERDICT' "$OUT" 2>/dev/null || true
	echo ""
	echo "Send that file: it already contains every sample with its deltas, the verdict and the summary table."
}

case "$CMD" in
	install)   cmd_install "$@" ;;
	uninstall) cmd_uninstall "$@" ;;
	activate)  cmd_activate "$@" ;;
	status)    cmd_status "$@" ;;
	logs)      cmd_logs "$@" ;;
	hunt)      cmd_hunt "$@" ;;
	soak)      cmd_soak "$@" ;;
	rebuild)   cmd_rebuild "$@" ;;
	*) echo "Unknown command: $CMD"; echo ""; usage; exit 1 ;;
esac
