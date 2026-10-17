#!/usr/bin/env python3
"""One bounded, read-only touchscreen qualification bundle.

Records the evidence that the driver this kernel is running matches this checkout:
the installer's installed-head stamp, module vermagic and srcversion, compared for
consistency only. That is not attestation: stamp+srcversion agreement does not prove
the exact source of the running module. The run snapshots host state, drives one
bounded gesture capture through the existing hardware-evidence helpers, then asks the
operator for ONE outcome for the whole gesture set. Never installs, loads/unloads
modules, reboots, suspends, touches MOK, escalates privileges or uploads anything.
SL4A_QUALIFY_HOST_ROOT relocates the host tree and is recorded in status.json: a
value other than "/" is a sandbox run.

--guided is the one opt-in exception to "never loads/unloads": after ONE explicit
typed consent it does a normal `modprobe -r` + `modprobe name=value ...` to put the
standard-transport bridge in the running kernel for the length of the gesture set,
then restores the exact original parameters and binding (success, exception,
SIGINT/SIGTERM). The persistent modprobe.d config is never written, no unload is
forced, and there is no install, MOK, reboot or suspend. It then evaluates the
evdev stream of the ONE selected direct touch node per gesture, and reports
narrowly scoped PASS/FAIL/NOT_TESTED: evdev observations are not desktop or
application gesture acceptance, and cold boot, resume and Secure Boot enrollment
stay NOT_TESTED without their own evidence.
"""
import argparse, hashlib, json, math, os, signal, struct, subprocess, sys, time, traceback
from datetime import datetime, timezone
from pathlib import Path

sys.dont_write_bytecode = True  # a __pycache__ in tools/ would make this tree dirty
sys.path.insert(0, str(Path(__file__).resolve().parent))
import touch_evdev as evdev  # focused stdlib decoder/scorer, no framework

HOST = Path(os.environ.get("SL4A_QUALIFY_HOST_ROOT", "/"))
REPO = Path(__file__).resolve().parents[1]
EVIDENCE = REPO / "tools" / "hardware_evidence"
STAMP = HOST / "var/lib/sl4a-touch/installed-head"
CONF = HOST / "etc/modprobe.d/sl4a-spi-hid.conf"
UNIT = HOST / "etc/systemd/system/sl4a-touch-activate.service"
SYS, PROC = HOST / "sys", HOST / "proc"
PROC_TABLE = Path("/proc")  # the live process table is never relocated
PKG, HID = "sl4a-touch", "sl4a-spi-hid"
MODULES = ("sl4a-spi-amd", "sl4a-spi-hid")  # modinfo takes the hyphenated name
TOUCH_IDS = ("MSHW0162", "MSHW0230", "MSHW0231")  # the driver's match table, exactly
PARAMS = ("raw_mode", "std_raw_transition", "raw_input_beta")
GRACE = 60       # seconds a capture may overrun its own bound before it is killed
TERM_GRACE = 10  # seconds the capture's process group gets between TERM and KILL
KILL_GRACE = 5
EDGE = "single-finger drag to each edge and corner, then hold there"
ONE_FINGER = ("single-finger tap on an icon",
              "one slow diagonal line across the screen, not jagged",
              "one fast drag across the screen, watching for dropped or stuck contacts",
              EDGE)
GESTURES = {
    "standard": ONE_FINGER + ("two quick taps, no double-tap misfire",),
    "bridge": ("five-finger hold, then lift all five at the same moment: all five must release",
               "two-finger pinch to zoom in and out (scrolling is the app's decision, not"
               " a device promise)",
               "lift one finger and put it back down in the same place immediately (recontact)",
               "two-finger tap, no phantom single tap") + ONE_FINGER,
}


class Stop(Exception):
    """Ends the run with a status/exit code; artefacts are still written."""

    def __init__(self, code, status, detail):
        super().__init__(detail)
        self.code, self.status, self.detail = code, status, detail


def utc():
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def run(cmd, timeout=120):
    """Run one read-only command; never raises, always returns (rc, output)."""
    try:
        done = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        return done.returncode, (done.stdout + done.stderr).strip()
    except FileNotFoundError:
        return 127, f"{cmd[0]}: unavailable"
    except subprocess.TimeoutExpired:
        return 124, f"{cmd[0]}: timed out"


def text_of(path):
    try:
        return path.read_text(errors="replace").strip()
    except OSError:
        return ""


def sysfs_root(module):
    """sysfs spells module names with underscores: sl4a-spi-hid is sl4a_spi_hid."""
    return SYS / "module" / module.replace("-", "_")


def param(name):
    return text_of(sysfs_root(HID) / "parameters" / name)


def acpi_nodes():
    """Exact touchscreen ids present, plus every other MSHW node, for the record.

    A `MSHW02*` glob silently misses MSHW0162, so a platform exposing both MSHW0162
    and MSHW0231 would look like a pure MSHW0231 machine and pass the bridge gate on
    ambiguous hardware.
    """
    seen = sorted({node.name.split(":")[0] for node in (SYS / "bus/acpi/devices").glob("MSHW*")})
    return [uid for uid in TOUCH_IDS if uid in seen], seen


def dkms_installed(kernel, rc, status):
    """The one version installed for PKG on exactly `kernel`, or "" and the reason."""
    if rc != 0:
        return "", f"dkms status rc={rc}"
    versions = set()
    for line in status.splitlines():
        package, sep, rest = line.partition("/")
        if not sep or package.strip() != PKG:
            continue  # some other package's row
        fields = [field.strip() for field in rest.split(",")]
        if len(fields) < 3 or fields[1] != kernel or not fields[-1].endswith(": installed"):
            continue  # another kernel, or built/added/broken instead of installed
        versions.add(fields[0])
    if len(versions) > 1:
        return "", f"conflicting installed rows: {sorted(versions)}"
    return (versions.pop() if versions else ""), ""


def facts():
    """Identity of the checkout and of the driver this kernel is running."""
    kernel = run(["uname", "-r"])[1] or "unknown"
    head_rc, head = run(["git", "-C", str(REPO), "rev-parse", "HEAD"])
    status_rc, status = run(["git", "-C", str(REPO), "status", "--porcelain"])
    dkms_rc, dkms = run(["dkms", "status", "-m", PKG, "-k", kernel])
    installed, dkms_note = dkms_installed(kernel, dkms_rc, dkms)
    return {"kernel": kernel, "head": head.strip() if head_rc == 0 else "", "head_rc": head_rc,
            "status_rc": status_rc, "dirty": status, "dkms_rc": dkms_rc,
            "dkms_note": dkms_note, "installed": installed, "stamp": text_of(STAMP),
            "version": text_of(REPO / "VERSION")}


def preflight(args, known):
    """Identity and hardware gates. Any fail means: do not capture, do not qualify."""
    results = []

    def gate(name, ok, detail):
        results.append({"name": name, "result": "pass" if ok else "fail", "detail": detail})

    head, kernel, status = known["head"], known["kernel"], known["dirty"]
    ownership = "dubious ownership" in status or "unsafe repository" in status
    hint = (f" -- git refuses this checkout for uid {os.getuid()}: run the tool as the"
            f" checkout owner, or allow ONLY this exact clone with: sudo git config"
            f" --global --add safe.directory {REPO} (never '*')") if ownership else ""
    absent = " [clean]" if known["status_rc"] == 0 else " [empty output from a FAILED git status]"
    gate("repo-clean", known["status_rc"] == 0 and not status,
         f"git status --porcelain rc={known['status_rc']}:{status or absent}{hint}")
    gate("build-identity", known["head_rc"] == 0 and bool(head) and known["stamp"] == head,
         f"git rev-parse HEAD rc={known['head_rc']} checkout_head={head or 'unknown'}"
         f" installed_head_stamp={known['stamp'] or '[missing]'}"
         " (installer-written local file compared for equality: consistency, not attestation)")
    gate("dkms-version",
         known["dkms_rc"] == 0 and bool(known["installed"]) and known["installed"] == known["version"],
         f"VERSION={known['version'] or '[missing]'} dkms_installed_for_{kernel}="
         f"{known['installed'] or '[none]'} dkms_status_rc={known['dkms_rc']}"
         + (f" ({known['dkms_note']})" if known["dkms_note"] else ""))
    for module in MODULES:
        root = sysfs_root(module)
        gate(f"loaded-{module}", root.is_dir(), f"loaded module dir={root}")
        vermagic = run(["modinfo", "-F", "vermagic", module])[1]
        gate(f"vermagic-{module}", vermagic.split()[:1] == [kernel],
             f"vermagic={vermagic or '[none]'} running_kernel={kernel}")
        loaded, on_disk = text_of(root / "srcversion"), run(["modinfo", "-F", "srcversion", module])[1]
        gate(f"srcversion-{module}", bool(loaded) and loaded == on_disk,
             f"loaded={loaded or '[missing]'} on_disk={on_disk or '[missing]'}"
             ": equal srcversion is consistency, not proof of the exact running source")
    raw_mode, beta = param("raw_mode").upper(), param("raw_input_beta").upper()
    transition = param("std_raw_transition")
    if args.bridge:
        gate("params-bridge", raw_mode == "N" and transition == "1" and beta == "Y",
             f"raw_mode={raw_mode or '?'} std_raw_transition={transition or '?'} "
             f"raw_input_beta={beta or '?'} (bridge needs N/1/Y)")
        touch, seen = acpi_nodes()
        gate("acpi-mshw0231", touch == ["MSHW0231"],
             f"touchscreen ids={touch} all MSHW nodes={seen} (bridge is exact MSHW0231:"
             " MSHW0162/MSHW0230 here, or a mixed set, is ambiguous hardware)")
    else:
        gate("params-standard", raw_mode == "N" and transition == "0",
             f"raw_mode={raw_mode or '?'} std_raw_transition={transition or '?'} "
             "(standard control needs N/0, not the bridge profile)")
    return results


def snapshot(out, args, known):
    """Copy-only snapshot: package config and service unit, never keys or MOK material."""
    (snap := out / "snapshot").mkdir()
    for name, source in (("modprobe.conf", CONF), ("sl4a-touch-activate.service", UNIT)):
        body = text_of(source) if source.is_file() else f"[absent: {source}]"
        (snap / name).write_text(body + "\n")
    uptime = text_of(PROC / "uptime").split()
    lines = [f"systemctl {q}: rc={rc} {answer}" for q in ("is-enabled", "is-active")
             for rc, answer in [run(["systemctl", q, PKG + "-activate.service"])]]
    lines += [
        f"uname_r={known['kernel']} uname_a={run(['uname', '-a'])[1]}",
        f"boot_id={text_of(PROC / 'sys/kernel/random/boot_id') or 'unknown'}"
        f" uptime_seconds={uptime[0] if uptime else 'unknown'}",
        f"phase_declared={args.phase} (operator declaration: compare boot_id/uptime across bundles)",
        f"dkms_status={run(['dkms', 'status'])[1]} secure_boot={run(['mokutil', '--sb-state'])[1]}",
        f"checkout_head={known['head'] or 'unknown'} installed_head_stamp={known['stamp'] or '[missing]'}"
        f" version_file={known['version'] or '[missing]'} dkms_installed={known['installed'] or '[none]'}",
        "module_params=" + " ".join(f"{name}={param(name) or '?'}" for name in PARAMS),
        "sysfs_module_dirs=" + " ".join(str(sysfs_root(m)) for m in MODULES),
        "dmesg comes from tools/hardware_evidence/collect.sh, not duplicated here",
    ]
    (snap / "host.txt").write_text("\n".join(lines) + "\n")
    (snap / "modinfo.txt").write_text("\n".join(f"[{m}]\n{run(['modinfo', m])[1]}" for m in MODULES) + "\n")


def resolve_output(raw, phase):
    """Absolute path resolved before any mkdir; never an existing or in-repo path."""
    stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    raw = raw or str(Path.home() / "sl4a-qualification" / f"{stamp}-{phase}")
    given = Path(os.path.abspath(Path(raw).expanduser()))  # absolute, symlinks kept
    for candidate in (given, given.resolve()):
        if os.path.lexists(candidate):
            sys.exit(f"refusing to overwrite an existing path: {candidate}")
    out = given.resolve()
    if out == REPO or out.is_relative_to(REPO) or REPO.is_relative_to(out):
        sys.exit(f"refusing an output inside the repository: {out} (repo {REPO})")
    return out


def ask(prompt):
    print(prompt, end="", flush=True)
    line = sys.stdin.readline()
    if not line:
        raise Stop(5, "incomplete", "end of input at an operator prompt")
    return line.strip().lower()


def declare_readiness(bridge):
    profile = "bridge" if bridge else "standard"
    print(f"\nGesture list ({profile} profile, ONE verdict for the whole set at the end):")
    print("\n".join(f"  {n}. {g}" for n, g in enumerate(GESTURES[profile], 1)))
    ask("\nPress Enter when you are ready to start the capture (Ctrl-C aborts): ")


def ask_outcome():
    answer = ask("\nOutcome for the whole gesture set - [p]ass / [f]ail / [n]ot observed: ")
    while answer not in ("p", "f", "n"):
        answer = ask("Answer p, f or n: ")
    return {"p": ("pass", 0, "qualified"), "f": ("fail", 4, "operator-fail"),
            "n": ("not-observed", 5, "incomplete")}[answer]


def session_members(pgid):
    """Live (non-zombie) pids in session AND group pgid, from the real /proc.

    start_new_session=True made the capture the leader of its own session, so
    sid == pgid identifies exactly the processes it forked: grandchildren that were
    orphaned when the direct child exited are still found, and a pid recycled into
    another session is never mistaken for ours.
    """
    members = []
    try:
        entries = os.listdir(PROC_TABLE)
    except OSError:
        return members
    for entry in entries:
        if not entry.isdigit():
            continue
        try:
            stat = (PROC_TABLE / entry / "stat").read_text(errors="replace")
        except OSError:
            continue
        rest = stat.rsplit(")", 1)[-1].split()  # comm may contain ")", the terminator is last
        # after "(comm)" the fields are state, ppid, pgrp, session, tty_nr, tpgid, ...
        if len(rest) >= 4 and rest[0] != "Z" and rest[2] == rest[3] == str(pgid):
            members.append(int(entry))
    return members


def kill_group(pgid):
    """TERM, a bounded grace, then KILL — on every process of the capture's group.

    * The group id is the capture's pid (start_new_session=True), stored at spawn.
      os.getpgid(pid) is not usable here: it raises ESRCH as soon as the direct child
      has been reaped, which is exactly the moment the rest of the group still needs
      killing.
    * process.wait() returning does NOT mean the group is empty. A grandchild that
      ignores TERM (a shell with trap '', a helper blocked in read()) outlives a parent
      that exited on TERM, so KILL is sent after the grace no matter what wait() said.
    * Membership is re-enumerated inside the grace window, so a helper that forks while
      it is being killed cannot leave a straggler behind.
    * Only pids whose session *and* group are this pgid are signalled, so an unrelated
      process — including one that reused a freed pid in another session — is never hit.
    """
    for sig, grace in ((signal.SIGTERM, TERM_GRACE), (signal.SIGKILL, KILL_GRACE)):
        deadline = time.monotonic() + grace
        while True:
            members = session_members(pgid)
            if not members:
                return
            for pid in members:
                try:
                    os.kill(pid, sig)
                except (ProcessLookupError, PermissionError):
                    pass
            if time.monotonic() >= deadline:
                break
            time.sleep(0.1)
    # KILL cannot be ignored: anything still listed has escaped the session (setsid)
    # and is intentionally left alone.


def write_preflight(out, results):
    gates = [f"{i['name']}: result={i['result']} {i['detail']}" for i in results]
    (out / "preflight.txt").write_text("\n".join([f"# preflight {utc()}"] + gates) + "\n")


def write_checksums(out):
    files = sorted(p for p in out.rglob("*") if p.is_file() and p.name != "checksums.txt")
    (out / "checksums.txt").write_text("".join(
        f"{hashlib.sha256(p.read_bytes()).hexdigest()}  {p.relative_to(out)}\n" for p in files))


def spawn_capture(out, args):
    """Bounded capture: its own session, so cleanup can signal the whole group."""
    cmd = ["bash", str(EVIDENCE / "capture_linux_trace_bundle.sh"),
           "--output", str(out / "capture"), "--duration", str(args.duration),
           "--capture-beta-multitouch" if args.bridge else "--capture-direct-touch"]
    log = (out / "capture.log").open("wb")
    process = subprocess.Popen(cmd, stdin=subprocess.DEVNULL, stdout=log,
                               stderr=subprocess.STDOUT, start_new_session=True)
    return process, log


def parse(argv):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--output", metavar="DIR", help="new directory outside the repository")
    parser.add_argument("--phase", default="initial", choices=("initial", "coldboot", "resume"),
                        help="declared moment of the run")
    parser.add_argument("--bridge", action="store_true", help="opt-in MT bridge, MSHW0231 only")
    parser.add_argument("--duration", type=int, default=20, help="capture seconds, 5..600")
    parser.add_argument("--guided", action="store_true",
                        help="automatic evdev evaluation; one typed consent for a temporary"
                             " standard->bridge reload, then touch only the screen")
    parser.add_argument("--gesture-timeout", type=float, default=12.0, metavar="SECONDS",
                        help="bound for each guided gesture window, 3..60 (default 12)")
    parser.add_argument("--idle-gap", type=float, default=2.0, metavar="SECONDS",
                        help="guided: close a window after this much silence once every"
                             " contact is released (0 = always wait the full timeout)")
    parser.add_argument("--knob", action="append", metavar="NAME=VALUE",
                        help="guided numeric threshold override, repeatable"
                             f" (known: {', '.join(sorted(evdev.KNOBS))})")
    args = parser.parse_args(argv)
    if not 5 <= args.duration <= 600:
        parser.error("--duration must be between 5 and 600 seconds")
    if args.guided and args.bridge:
        parser.error("--guided runs the standard profile first and switches to the bridge"
                     " itself: drop --bridge")
    if args.guided and not 3 <= args.gesture_timeout <= 60:
        parser.error("--gesture-timeout must be between 3 and 60 seconds")
    if not 0 <= args.idle_gap < args.gesture_timeout:
        parser.error("--idle-gap must be >= 0 and smaller than --gesture-timeout")
    args.knobs = knobs(args.knob, parser)
    return args


def knobs(pairs, parser):
    """Numeric thresholds: the shipped defaults, minus any --knob override."""
    picked = dict(evdev.KNOBS)
    for pair in pairs or []:
        name, sep, value = pair.partition("=")
        if not sep or name.strip() not in picked:
            parser.error(f"--knob {pair!r}: expected NAME=VALUE with NAME one of"
                         f" {', '.join(sorted(picked))}")
        try:
            picked[name.strip()] = float(value)
        except ValueError:
            parser.error(f"--knob {pair!r}: {value!r} is not a number")
        low, high = evdev.KNOB_BOUNDS[name.strip()]
        if not math.isfinite(picked[name.strip()]) or not low <= picked[name.strip()] <= high:
            parser.error(f"--knob {pair!r}: must be a finite number in [{low:g}, {high:g}]")
    return picked


# ── guided: automatic, bounded evdev qualification ──────────────────────────
#
# What this path is allowed to touch on the host, and what it is not:
#   * runtime module parameters (load-time, 0444 in sysfs) via a normal
#     `modprobe -r` + `modprobe name=value ...` — never `--force`, never
#     rmmod -f, never a persistent config edit;
#   * nothing else: no install, no MOK, no reboot, no suspend, no GUI as root.
# The installed profile in /etc/modprobe.d is read and hashed, so a reboot puts
# the machine back on it by itself and the bundle can prove the file never
# changed.
GUIDED_CONSENT = "BRIDGE"
BRIDGE_PARAMS = {"raw_mode": "0", "std_raw_transition": "1", "raw_input_beta": "1"}
BRIDGE_EXPECT = {"raw_mode": "N", "std_raw_transition": "1", "raw_input_beta": "Y"}
DRIVER, CONTROLLER_DRIVER = "sl4a_spi_hid", "sl4a_spi_amd_v2_multi"
DEV_ROOT = Path(os.environ.get("SL4A_QUALIFY_DEV_ROOT", "/dev/input"))
SETTLE = 0.6   # seconds drained before/after a window (persistence + stale check)
GUARD_BIND = {"wait_bound": 15.0, "wait_unloaded": 10.0}
WINDOWS = {
    "standard": (("tap", "one quick single-finger tap anywhere on the screen"),
                 ("motion", "drag ONE finger slowly along a long diagonal"),
                 ("edges", "drag ONE finger to each of the four edges and corners")),
    # The bridge keeps the tap/pinch/hold/recontact windows AND the two plain
    # one-finger windows: the standard-transport bridge forwards a single
    # contact through the same path every multi-finger report uses, so a drag
    # and an edge sweep are the windows that show that continuity.
    "bridge": (("tap", "one quick single-finger tap anywhere on the screen"),
               ("motion", "drag ONE finger slowly along a long diagonal"),
               ("edges", "drag ONE finger to each of the four edges and corners"),
               ("five_hold", "put FIVE fingers down, hold, then lift all five together"),
               ("pinch", "two fingers close together, spread them, then pinch back in"),
               ("recontact", "tap, lift, then immediately tap the SAME spot again")),
}
NOT_TOUCH = ("keyboard", "power button", "lid switch", "video bus", "pcspkr",
             "touchpad", "mouse")
# A stylus/pen node is INPUT_PROP_DIRECT too: scoring a gesture list on it would
# qualify the wrong transducer, so those names are excluded outright.
NOT_TOUCH += ("stylus", "pen", "eraser", "wacom")


def hex_bits(path):
    try:
        words = (text_of(path) or "0").split()
        return sum(int(word, 16) << (64 * i) for i, word in enumerate(reversed(words)))
    except ValueError:
        return 0


def still_down(data, kind=None):
    """True when the last report of a drained stream still holds a contact.

    This only sees REPORTS: a held finger emits nothing, so silence is not
    proof of release. `evdev.down_state` is the authoritative check.
    """
    parsed = evdev.frames(evdev.decode(data)[0], kind)
    return bool(parsed) and bool(parsed[-1]["contacts"])


def bound_driver(device):
    link = device / "driver"
    return Path(os.path.realpath(link)).name if link.exists() else ""


def guided_panel():
    """The one exact MSHW0231 SPI device and its controller, or a loud STOP.

    Bridge hardware is exactly MSHW0231 — a board exposing MSHW0162/MSHW0230 as
    well is ambiguous and stays out, because the standard-transport bridge is
    qualified on that one ID only.
    """
    touch, seen = acpi_nodes()
    if touch != ["MSHW0231"]:
        raise Stop(2, "preflight-failed",
                   f"touchscreen ids={touch} all MSHW nodes={seen}: the guided bridge run is"
                   " exact MSHW0231 only")
    acpi = sorted((SYS / "bus/acpi/devices").glob("MSHW0231:*"))
    if len(acpi) != 1:
        raise Stop(2, "preflight-failed", f"expected one MSHW0231 ACPI node, found {acpi}")
    physical = acpi[0] / "physical_node"
    if not physical.exists():
        raise Stop(2, "preflight-failed", f"{acpi[0]} exposes no physical_node")
    controllers = sorted((SYS / "bus/acpi/devices").glob("AMDI0060:*"))
    if len(controllers) != 1:
        raise Stop(2, "preflight-failed",
                   f"expected one AMDI0060 controller, found {controllers}")
    return (Path(os.path.realpath(physical)),
            SYS / "bus/platform/devices" / controllers[0].name)


def evdev_nodes(panel):
    """Every input event node under the panel, with the facts a gate needs."""
    nodes = []
    for entry in sorted((SYS / "class/input").glob("event*")):
        device = entry / "device"
        real = Path(os.path.realpath(device))
        if real != panel and panel not in real.parents:
            continue  # another device's node: never a candidate, not even read
        nodes.append({"node": entry.name, "path": DEV_ROOT / entry.name,
                      "name": text_of(device / "name"),
                      "props": hex_bits(device / "properties"),
                      "abs": hex_bits(device / "capabilities/abs"),
                      "keys": hex_bits(device / "capabilities/key"),
                      "dev": text_of(entry / "dev"),
                      "sysfs": str(real)})
    return nodes


def pick_touch(nodes):
    """Exactly one INPUT_PROP_DIRECT node under the panel, or a loud STOP.

    INPUT_PROP_DIRECT is what separates a touch surface from a keyboard, a
    touchpad or a button node; ancestry under the panel is what separates it
    from every other direct device in the machine. When both the standard HID
    node and the driver's own MT device hang off the same SPI device, MT slots
    rank first (the multi-finger windows are unreadable without them) and the
    exact driver name breaks a remaining tie — all candidates are recorded.
    """
    direct = [n for n in nodes
              if n["props"] & (1 << evdev.INPUT_PROP_DIRECT)
              and n["abs"] & evdev.EV_ABS_BIT["x"] and n["abs"] & evdev.EV_ABS_BIT["y"]
              and n["keys"] & (1 << evdev.BTN_TOUCH)
              and evdev.protocol_kind(n["abs"]) in ("B", "single")
              and not any(word in n["name"].lower() for word in NOT_TOUCH)]
    if not direct:
        raise Stop(2, "preflight-failed",
                   "no INPUT_PROP_DIRECT touch node under the panel"
                   f" (candidates: {[(n['node'], n['name'], hex(n['props'])) for n in nodes]})")
    if len(direct) == 1:
        return direct[0], direct
    slots = [n for n in direct if n["abs"] & evdev.EV_ABS_BIT["slot"]]
    for pool in (slots, direct):
        named = [n for n in pool if "MSHW0231 Touchscreen" in n["name"]]
        if len(named) == 1:
            return named[0], direct
        if len(pool) == 1:
            return pool[0], direct
    raise Stop(2, "preflight-failed", "ambiguous touch node: " +
               ", ".join(f"{n['node']}={n['name']}" for n in direct))


def abs_range(meta):
    """ABS min/max asked of the device itself. NEVER guessed on a live host.

    Every spatial rule (edges, travel, recontact drift) is a fraction of this
    range, so an invented range mis-scores the panel in both directions. On the
    live host a failed or degenerate EVIOCGABS is therefore a hard stop; the
    0..32767 the driver itself declares is allowed only inside a relocated
    sandbox tree, where it is recorded as a fixture and never as a device fact.
    """
    live = HOST == Path("/")
    forced = os.environ.get("SL4A_QUALIFY_ABS")
    if forced:
        if live:
            raise Stop(2, "preflight-failed",
                       "SL4A_QUALIFY_ABS is a sandbox fixture: refusing to score a live host"
                       " against a range it was handed")
        try:
            xmin, xmax, ymin, ymax = (int(v) for v in forced.split(","))
        except ValueError:
            raise Stop(2, "preflight-failed", "SL4A_QUALIFY_ABS must be xmin,xmax,ymin,ymax")
        return {"x": (xmin, xmax), "y": (ymin, ymax)}, "fixture-env-sandbox"
    problem, found, slot_high = "", {}, None
    fd = None
    try:
        import fcntl
        fd = os.open(str(meta["path"]), os.O_RDONLY | os.O_NONBLOCK | os.O_NOCTTY)
        for axis, code in (("x", evdev.ABS_X), ("y", evdev.ABS_Y)):
            buf = fcntl.ioctl(fd, evdev.ev_iocgabs(code), b"\0" * 24)
            _, low, high = struct.unpack("@6i", buf)[:3]
            found[axis] = (low, high)
        if meta.get("kind") == "B":
            buf = fcntl.ioctl(fd, evdev.ev_iocgabs(evdev.ABS_MT_SLOT), b"\0" * 24)
            slot_high = struct.unpack("@6i", buf)[2]
        if not (found["x"][1] > found["x"][0] and found["y"][1] > found["y"][0]):
            problem = f"degenerate range {found} (max must exceed min)"
    except (OSError, struct.error) as error:
        problem = f"EVIOCGABS failed: {error}"
    finally:
        if fd is not None:
            try:
                os.close(fd)
            except OSError:
                pass
    if not problem and slot_high is not None:
        meta["slots"] = max(0, int(slot_high)) + 1
    if not problem:
        return found, "ioctl"
    if live:
        raise Stop(2, "preflight-failed",
                   f"cannot read the ABS range of {meta['path']} from the device ({problem}):"
                   " a guessed range would misnormalize every spatial check")
    return {"x": (0, 32767), "y": (0, 32767)}, f"declared-sandbox ({problem})"


def guided_device(panel, profile):
    """The gate + the record: one node, its protocol, its range."""
    nodes = evdev_nodes(panel)
    chosen, direct = pick_touch(nodes)
    kind = evdev.protocol_kind(chosen["abs"])
    if profile == "bridge" and kind != "B":
        raise Stop(2, "preflight-failed",
                   f"the bridge windows need a Type B (ABS_MT_SLOT) node, and"
                   f" {chosen['node']} ({chosen['name']}) advertises {kind}: multi-finger"
                   " contacts would be unobservable")
    meta = {"profile": profile, "node": chosen["node"], "path": str(chosen["path"]),
            "name": chosen["name"], "kind": kind, "slots": 0, "dev": chosen["dev"],
            "props": hex(chosen["props"]), "abs_bits": hex(chosen["abs"]),
            "key_bits": hex(chosen["keys"]), "sysfs": chosen["sysfs"],
            "candidates": [{k: n[k] for k in ("node", "name", "props", "abs", "keys")}
                           for n in nodes]}
    rng, source = abs_range(meta)
    meta["range"] = {axis: list(value) for axis, value in rng.items()}
    meta["range_source"] = source
    return meta, rng


def hid_params():
    """Every parameter of the loaded module, exactly as the kernel prints it."""
    root = sysfs_root(HID) / "parameters"
    if not root.is_dir():
        return {}
    return {p.name: text_of(p) for p in sorted(root.glob("*"))}


def usable(value):
    """A value modprobe can be handed as one argument, or "" when it cannot."""
    return bool(value) and len(value.split()) == 1 and value != "(null)"


def param_args(params, override=None):
    """modprobe arguments reproducing `params`, with `override` on top.

    Values go one per argument, never through a shell. Anything unprintable is
    skipped rather than guessed — and named in `skipped_params` so the bundle
    says which value it could not hand back instead of leaving that invisible.
    """
    chosen = {name: (override or {}).get(name, value) for name, value in params.items()}
    return [f"{name}={value}" for name, value in sorted(chosen.items()) if usable(value)]


def skipped_params(params, override=None):
    chosen = {name: (override or {}).get(name, value) for name, value in params.items()}
    return [name for name, value in sorted(chosen.items()) if not usable(value)
            and not (name == "win_host_addr" and value == "(null)")]  # source default: omit to recreate NULL


def normalize(value):
    low = (value or "").strip().lower()
    if low in ("1", "y", "yes", "on", "true"):
        return "1"
    if low in ("0", "n", "no", "off", "false"):
        return "0"
    return low


def verify_params(expected):
    """Read the LIVE module back: what was requested is not evidence."""
    live = hid_params()
    bad = [f"{name}={live.get(name) or '[missing]'} (wanted {want})"
           for name, want in expected.items() if normalize(live.get(name)) != normalize(want)]
    if bad:
        raise Stop(3, "transition-failed",
                   "loaded parameters disagree with the requested profile: " + ", ".join(bad))
    return live


def modprobe(arguments, log):
    rc, out = run(["modprobe", *arguments], 120)
    log.append(f"modprobe {' '.join(arguments)} -> rc={rc}" + (f": {out}" if out else ""))
    return rc


def wait_unloaded(seconds=None):
    deadline = time.monotonic() + (seconds or GUARD_BIND["wait_unloaded"])
    while time.monotonic() < deadline:
        if not sysfs_root(HID).is_dir():
            return True
        time.sleep(0.05)
    return False


def wait_bound(device, driver, seconds=None):
    deadline = time.monotonic() + (seconds or GUARD_BIND["wait_bound"])
    while time.monotonic() < deadline:
        if bound_driver(device) == driver:
            return True
        time.sleep(0.1)
    return False


def module_clients(driver):
    """Every spi device currently bound to `driver`.

    `modprobe -r` is global: if a second panel (or any other spi device) is
    bound to the same module, unloading it to change this panel's parameters
    would take that device down too, so anything but the one panel is a stop.
    """
    devices = SYS / "bus/spi/devices"
    if not devices.is_dir():
        return []
    return sorted(str(device) for device in devices.iterdir()
                  if bound_driver(device) == driver)


def write_origin_backup(out, origin):
    """Persist the exact original state BEFORE the first mutation.

    The in-memory dict dies with the process; a bundle written early is what a
    human needs if this run is killed while the touchscreen is dead.
    """
    (out / "origin-backup.txt").write_text("\n".join([
        "# original runtime state, written BEFORE the first reload (no mutation yet)",
        f"params={' '.join(param_args(origin.get('params', {})))}",
        f"skipped_params={','.join(skipped_params(origin.get('params', {}))) or '[none]'}",
        f"panel_driver={origin.get('driver') or '[unbound]'}",
        f"controller_driver={origin.get('platform_driver') or '[unbound]'}",
        f"config={CONF} sha256={origin.get('config_sha') or '[absent]'}",
        f"recovery: sudo modprobe -r {HID} && sudo modprobe {HID} "
        + " ".join(param_args(origin.get("params", {}))),
    ]) + "\n")


def ask_consent(panel, origin):
    """One explicit typed consent, before anything on the host is reloaded.

    It comes before the FIRST gesture window too, so the promise the operator
    reads ("after this prompt you only touch the screen") is literally true:
    no gesture happens before the consent, and nothing but touches after it.
    """
    print("\nThe guided run needs the standard-transport bridge for its second half.")
    print("It will, on this machine, right now:")
    print(f"  1. modprobe -r {HID}                     (normal unload, never --force)")
    print(f"  2. modprobe {HID} " + " ".join(param_args(origin["params"], BRIDGE_PARAMS)))
    print(f"  3. wait until {panel} binds again to {DRIVER}, then verify the live values")
    print("  The touchscreen will be DEAD for a few seconds in between, and it stays")
    print("  dead until the module loads if that reload fails.")
    print(f"  Never touched: {CONF} (hashed before and after), the systemd unit,")
    print("  DKMS, MOK/keys, the boot configuration. No install, no reboot, no suspend.")
    print("  After this prompt you only touch the screen: every gesture is timed out")
    print("  and evaluated automatically, no keyboard answer is needed.")
    print("  Original params: " + " ".join(param_args(origin["params"])))
    print("  That exact state is already written to origin-backup.txt in the bundle.")
    print(f"  Manual rollback any time: sudo modprobe -r {HID} && sudo modprobe {HID} "
          + " ".join(param_args(origin["params"])))
    answer = ask(f"\nType {GUIDED_CONSENT} to allow the temporary reload"
                 " (anything else aborts, nothing is changed): ")
    if answer != GUIDED_CONSENT.lower():
        raise Stop(5, "consent-declined",
                   f"consent not given ({answer!r}): the driver was not reloaded,"
                   " parameters and binding are as they were")


def open_node(device, live):
    """Open the ONE selected node, prove which char device it is, and grab it.

    `eventN` can be renumbered between the sysfs read and this open, and a
    wrong node would be scored as the panel: the fd's own rdev is compared with
    sysfs, and EVIOCGNAME with the sysfs name. On the live host a mismatch is a
    hard stop; a relocated sandbox tree has no char device (or a FIFO), so the
    check is recorded as not applicable instead of pretending to pass.

    The grab is what keeps the test gestures off the desktop: while EVIOCGRAB is
    held only this process sees the events. On a live host a refused grab is a
    stop too — without it every gesture would also click whatever is on screen.
    """
    try:
        fd = os.open(str(device["path"]), os.O_RDONLY | os.O_NONBLOCK | os.O_NOCTTY)
    except OSError as error:
        raise Stop(2, "preflight-failed",
                   f"cannot open {device['path']}: {error} (run as root, or join group input)")
    problem = ""
    try:
        rdev = os.fstat(fd).st_rdev
    except OSError as error:
        rdev, problem = 0, f"fstat failed: {error}"
    want = device.get("dev") or ""
    if want and rdev:
        try:
            major, minor = (int(part) for part in want.split(":", 1))
        except ValueError:
            major = minor = None
        if major is not None and os.makedev(major, minor) != rdev:
            problem = (f"the opened fd is char device {os.major(rdev)}:{os.minor(rdev)}"
                       f" but sysfs says {want}")
    if live and (not want or not rdev):
        problem = "missing sysfs dev identity or non-character fd"
    name = evdev.device_name(fd)
    if live and not name:
        problem = "cannot read EVIOCGNAME"
    if name and device.get("name") and name != device["name"]:
        problem = f"EVIOCGNAME={name!r} but the sysfs name is {device['name']!r}"
    if problem and live:
        os.close(fd)
        raise Stop(2, "preflight-failed",
                   f"{device['path']} is not the node sysfs describes ({problem}): refusing to"
                   " score a device that may have been renumbered")
    note = problem
    try:
        evdev.grab(fd, True)
        return fd, True, note
    except OSError as error:
        if live:
            os.close(fd)
            raise Stop(2, "preflight-failed",
                       f"EVIOCGRAB on {device['path']} failed ({error}): without the grab the"
                       " desktop would react to every test gesture")
        return fd, False, (note + f" EVIOCGRAB unavailable: {error}").strip()


def final_persistence(fd, device):
    """Is anything still held down? Asked of the kernel, on the same fd.

    A fresh file descriptor cannot answer this: while a contact is held the
    kernel emits nothing, so a new fd reads silence whether the panel is idle or
    stuck. EVIOCGKEY/EVIOCGMTSLOTS on the fd the windows were read from can.
    """
    state = evdev.down_state(fd, device["kind"], device.get("slots") or 0)
    tail, eof = evdev.read_window(fd, SETTLE, idle_gap=0, kind=device["kind"])
    drained = still_down(tail, device["kind"])
    if HOST == Path("/") and not state["supported"]:
        raise Stop(3, "capture-failed", "cannot query final kernel contacts: " + state["error"])
    if state["supported"]:
        return {"stale_contacts": bool(eof or drained or state["btn_touch"] or state["tracking_ids"]),
                "source": "kernel", "btn_touch": state["btn_touch"],
                "tracking_ids": state["tracking_ids"], "eof": eof}
    return {"stale_contacts": bool(drained), "source": "drained-same-fd",
            "btn_touch": None, "tracking_ids": [], "eof": eof}


def capture_windows(out, args, device, rng, profile, results):
    """One bounded window per gesture; the operator only touches the screen.

    Returns the final persistence reading, taken from the SAME fd (see
    `final_persistence`).
    """
    raw = out / "raw"
    raw.mkdir(exist_ok=True)
    live = HOST == Path("/")
    fd, grabbed, note = open_node(device, live)
    device["grabbed"], device["identity"] = grabbed, note or "char device matches sysfs"
    try:
        initial = evdev.down_state(fd, device["kind"], device.get("slots") or 0)
        device["initial_state"] = initial
        for name, prompt in WINDOWS[profile]:
            # Read the pre-window state BEFORE the marker: anything still down
            # when the window opens is a persistence failure, and the marker
            # must not race with this drain.
            pre, pre_eof = evdev.read_window(fd, SETTLE, idle_gap=0, kind=device["kind"])
            state = evdev.down_state(fd, device["kind"], device.get("slots") or 0)
            if live and not state["supported"]:
                raise Stop(3, "capture-failed", "cannot query kernel contacts: " + state["error"])
            kernel_down = bool(state["btn_touch"] or state["tracking_ids"])
            context = {"stale_contacts": bool(kernel_down or still_down(pre, device["kind"])),
                       "pre_eof": pre_eof, "profile": profile, "protocol": device["kind"],
                       "state_source": "kernel" if state["supported"] else "drained"}
            print(f"\n[{profile}] window={name} device={device['node']} name={device['name']}",
                  flush=True)
            print(f"  TOUCH NOW: {prompt}.", flush=True)
            print(f"  capturing {args.gesture_timeout:.0f}s, evaluated automatically"
                  " — do not touch the keyboard", flush=True)
            data, eof = evdev.read_window(fd, args.gesture_timeout, args.idle_gap,
                                          kind=device["kind"])
            records, leftover = evdev.decode(data)
            parsed = evdev.frames(records, device["kind"])
            context.update(eof=eof, partial=bool(leftover))
            target = raw / f"{profile}-{name}.evdev"
            target.write_bytes(data)
            result = evdev.evaluate(name, parsed, rng, args.knobs, context)
            result.update(profile=profile, device=device["node"], raw=target.name,
                          events=len(records), reports=len(parsed),
                          window_seconds=args.gesture_timeout, captured_utc=utc())
            results.append(result)
            print("  -> " + result["status"].upper()
                  + (f": {result['reason']}" if result["reason"] else ""), flush=True)
        return final_persistence(fd, device)
    finally:
        if grabbed:
            try:
                evdev.grab(fd, False)
            except OSError:
                pass
        os.close(fd)


def transition_to_bridge(panel, log, state):
    """Standard -> bridge at runtime, without editing one persistent byte.

    All three parameters are load-time (0444 in sysfs), so a normal unload plus
    a load with the values on the command line is the only runtime way to change
    them. The unload is plain `modprobe -r`: if the module is busy it fails and
    nothing is changed.
    """
    origin = state["origin"]
    dropped = skipped_params(origin["params"], BRIDGE_PARAMS)
    if dropped:
        # A value that cannot be handed back would silently change a parameter
        # this run never meant to touch: refuse before the module is touched.
        raise Stop(2, "preflight-failed",
                   "cannot hand these parameters back to modprobe: " + ", ".join(dropped)
                   + " (empty/(null)/space values): the module was not reloaded")
    origin["changed"] = True  # also restore if interrupted inside the unload command
    if modprobe(["-r", HID], log) != 0:
        origin["changed"] = not (sysfs_root(HID).is_dir() and hid_params() == origin["params"]
                                 and bound_driver(panel) == origin["driver"])
        raise Stop(3, "transition-failed",
                   f"modprobe -r {HID} failed (module busy): nothing was reloaded and"
                   " no parameter changed")
    if not wait_unloaded():
        raise Stop(3, "transition-failed", f"{HID} is still present after modprobe -r")
    # From here on the module is GONE and the run owes the machine a restore; a
    # refused unload above deliberately leaves this False so nothing is unloaded
    # a second time for nothing.
    origin["changed"] = True
    if modprobe([HID, *param_args(origin["params"], BRIDGE_PARAMS)], log) != 0:
        raise Stop(3, "transition-failed",
                   f"modprobe {HID} with the bridge parameters failed: touch stays dead"
                   " until the restore step reloads the original profile")
    if not wait_bound(panel, DRIVER):
        raise Stop(3, "transition-failed",
                   f"{panel} did not bind to {DRIVER} after the bridge load")
    state["bridge_params"] = verify_params(BRIDGE_EXPECT)


def restore(out, state, log):
    """Put back the runtime parameters and both bindings this run found.

    Called from `finally` on success, on any exception and on SIGINT/SIGTERM,
    and idempotent: if the live module already carries the original parameters
    and binding (a refused unload, a run that never reached the reload) it does
    NOT tear the driver down again — a needless unload is a real risk taken for
    nothing. It never reboots, never forces an unload, never writes a
    persistent file.
    """
    origin, lines = state["origin"], []
    if not origin.get("changed"):
        lines.append("no runtime change was attempted: parameters and binding untouched")
        return True, lines
    panel = Path(state["panel"]) if state.get("panel") else None
    platform = Path(state["platform"]) if state.get("platform") else None
    want = origin.get("params", {})
    live = hid_params()
    if (sysfs_root(HID).is_dir() and want and panel is not None
            and bound_driver(panel) == (origin.get("driver") or DRIVER)
            and all(normalize(live.get(name, "")) == normalize(value)
                    for name, value in want.items())):
        lines.append("live module already matches the original parameters and binding:"
                     " nothing to reload")
        return _verify_restore(origin, lines, panel, platform)
    if sysfs_root(HID).is_dir():
        rc = modprobe(["-r", HID], log)
        lines.append(f"unload before restore: rc={rc}")
        if rc != 0 or not wait_unloaded():
            lines.append("could not unload the module: the original profile cannot be loaded")
            return False, lines
    dropped = skipped_params(want)
    if dropped:
        lines.append("CANNOT be handed back to modprobe (empty/(null)/space value): "
                     + ", ".join(dropped))
    rc = modprobe([HID, *param_args(want)], log)
    lines.append(f"reload original profile: rc={rc}")
    if rc != 0:
        return False, lines
    if panel is not None:
        driver = origin.get("driver") or DRIVER
        if not wait_bound(panel, driver):
            lines.append(f"{panel} did not bind to {driver} on its own: writing the sysfs"
                         " bind attribute")
            try:
                (SYS / "bus/spi/drivers" / driver / "bind").write_text(panel.name + "\n")
                wait_bound(panel, driver)
            except OSError as error:
                lines.append(f"bind write failed: {error}")
    ok, lines = _verify_restore(origin, lines, panel, platform)
    if dropped:
        ok = False
    return ok, lines


def _verify_restore(origin, lines, panel, platform):
    """What must hold after ANY restore path: live parameters, both bindings and
    a byte-identical persistent config. Every parameter read from the original
    load is compared, not just the three profile ones, so a value that could not
    be handed back cannot pass as "restored"."""
    ok = True
    want = origin.get("params", {})
    live = hid_params()
    if want:
        drift = [f"{name}: {value!r} -> {live.get(name)!r}"
                 for name, value in sorted(want.items())
                 if normalize(live.get(name, "")) != normalize(value)]
        if drift:
            ok = False
            lines.append("parameters differ after restore: " + "; ".join(drift))
        else:
            lines.append(f"all {len(want)} parameters match the original load")
    for device, driver, label in ((panel, origin.get("driver") or DRIVER, "panel"),
                                  (platform, origin.get("platform_driver"), "controller")):
        if device is None or not driver:
            continue
        actual = bound_driver(device) or "[unbound]"
        lines.append(f"{label} binding: {actual} (wanted {driver})")
        ok = ok and actual == driver
    if origin.get("config_sha"):
        same = (hashlib.sha256(CONF.read_bytes()).hexdigest() == origin["config_sha"]
                if CONF.is_file() else origin["config_sha"] == "[absent]")
        ok = ok and same
        lines.append(f"persistent config unchanged: {same}")
    return ok, lines


def write_recovery(out, state):
    """Loud, copy-pasteable recovery when the restore did not succeed."""
    origin = state.get("origin", {})
    lines = [
        "# RECOVERY REQUIRED: the guided run could not restore the driver",
        "",
        "Use a console or SSH session: the touchscreen may be dead.",
        "",
        f"1. sudo modprobe -r {HID}",
        "2. sudo modprobe sl4a-spi-amd",
        f"3. sudo modprobe {HID} " + " ".join(param_args(origin.get("params", {}))),
        f"4. check both bindings: readlink -f {state.get('panel', '<panel>')}/driver"
        f"   (expected basename {origin.get('driver') or DRIVER}) and"
        f" {state.get('platform', '<controller>')}/driver"
        f"   (expected {origin.get('platform_driver') or CONTROLLER_DRIVER})",
        "5. sudo ./tools/sl4a-touch.sh status",
        "",
        f"{CONF} was never written by this run"
        + (f" (sha256 {origin.get('config_sha')})" if origin.get("config_sha") else "")
        + ".",
        "A reboot also returns the machine to the installed profile: the boot unit",
        "activates it after multi-user.target. NO reboot was performed by this tool.",
    ]
    text = "\n".join(lines) + "\n"
    (out / "recovery.txt").write_text(text)
    print("\n" + text, file=sys.stderr, flush=True)


def guided_secure_boot(out):
    """Read-only Secure Boot facts: state, signer, enrolled-cert count.

    No key material, no enrollment, no reboot: enrolling is the operator's
    decision and cannot be observed from inside a running session.
    """
    enrolled = run(["mokutil", "--list-enrolled"])[1]
    lines = [
        f"secure_boot_state={run(['mokutil', '--sb-state'])[1] or '[mokutil unavailable]'}",
        f"module_signer={run(['modinfo', '-F', 'signer', HID])[1] or '[unsigned or unknown]'}",
        "enrolled_certificates=" + str(sum(1 for line in enrolled.splitlines()
                                           if line.lstrip().startswith("Subject:"))),
        "enrollment_status=NOT_TESTED (enrollment and its boot are the operator's decision;"
        " this bundle cannot observe them)",
    ]
    (out / "secureboot.txt").write_text("\n".join(lines) + "\n")


def guided_verdict(results, persistence, phase, collection_rc=None):
    """Narrowly scoped verdicts. Never a full PR PASS.

    Every PR in the branch list gets its OWN entry, each one either a measured
    narrow subtest of exactly the thing that PR changes or an honest NOT_TESTED
    with the reason. One gesture set does not become another PR's evidence, and
    cold boot, resume and Secure Boot enrollment need sessions this run cannot
    contain.
    """
    def roll(profile):
        """PASS only for the COMPLETE window set: two of four passing is not a
        bridge gesture result, and an interrupted run must not read as one."""
        rows = [r for r in results if r["profile"] == profile]
        expected = {name for name, _ in WINDOWS[profile]}
        if {r["name"] for r in rows} != expected:
            return "NOT_TESTED"
        if any(r["status"] == "fail" for r in rows):
            return "FAIL"
        return "NOT_TESTED" if any(r["status"] != "pass" for r in rows) else "PASS"

    persisted = persistence.get("stale_contacts")
    persistence_result = ("NOT_TESTED" if persisted is None
                          else ("FAIL" if persisted else "PASS"))
    standard, bridge = roll("standard"), roll("bridge")
    evidence = "PASS" if collection_rc == 0 else "NOT_TESTED"
    items = [
        {"id": "standard_evdev_gestures", "result": standard,
         "scope": "evdev frames of the selected touch node in the installed standard profile"},
        {"id": "bridge_evdev_gestures", "result": bridge,
         "scope": "evdev frames of the selected touch node after the temporary"
                  " standard->bridge reload (raw_mode=0 std_raw_transition=1 raw_input_beta=1)"},
        {"id": "pr87_bridge_standard_transport", "result": bridge,
         "scope": "narrow subtest: the bridge profile's own evdev gesture set, run after the"
                  " temporary reload on the MSHW0231 node - one measured subresult, NOT a PR"
                  " acceptance (cold boot, resume and enrollment are NOT_TESTED)"},
        {"id": "pr88_close_contact_coalescing", "result": "NOT_TESTED",
         "scope": "NOT_TESTED: no window here exercises post-association close-contact"
                  " coalescing; the bridge recontact window only proves a lift and a NEW"
                  " contact lifecycle, which is a different code path"},
        {"id": "pr89_dkms_signing_identity", "result": "NOT_TESTED",
         "scope": "NOT_TESTED: only modinfo -F signer and mokutil --sb-state were read; no"
                  " signing identity was verified and no enrollment happened"},
        {"id": "pr90_evidence_workflow", "result": evidence,
         "scope": "narrow subtest: tools/hardware_evidence/collect.sh produced a bundle in"
                  " this run (its rc is recorded); the full evidence workflow is not tested"},
        {"id": "pr91_fixed_header_bounds", "result": "NOT_TESTED",
         "scope": "NOT_TESTED: the edges window normalizes ABS min/max of the input node, not"
                  " the fixed-header bounds of the transport payload"},
        {"id": "persistence_after_release", "result": persistence_result,
         "scope": "nothing held down after the last window (EVIOCGKEY BTN_TOUCH plus"
                  " EVIOCGMTSLOTS tracking ids on the window's own fd)"},
        {"id": f"coldboot_{phase}", "result": "NOT_TESTED",
         "scope": "needs a real power-on session; this bundle holds one declared phase"},
        {"id": f"resume_{phase}", "result": "NOT_TESTED",
         "scope": "needs an actual suspend/resume, which this tool never performs"},
        {"id": "secureboot_enrollment", "result": "NOT_TESTED",
         "scope": "mokutil and modinfo were only read; no enrollment, no reboot"},
        {"id": "full_pr_pass", "result": "NOT_APPLICABLE",
         "scope": "a gesture subtest cannot accept a PR: cold boot, resume and"
                  " enrollment are NOT_TESTED here"},
    ]
    if any(item["result"] == "FAIL" for item in items):
        return {"status": "gesture-fail", "exit_code": 4, "items": items}
    if any(item["result"] == "NOT_TESTED" for item in items
           if item["id"] in ("bridge_evdev_gestures", "standard_evdev_gestures")):
        return {"status": "incomplete", "exit_code": 5, "items": items}
    return {"status": "guided-qualified", "exit_code": 0, "items": items}


def render_verdicts(verdict):
    head = ["# scoped verdicts (NOT a full PR pass)", ""]
    rows = [f"{item['id']}: {item['result']} — {item['scope']}" for item in verdict["items"]]
    return "\n".join(head + rows) + "\n"


def guided(args):
    out = resolve_output(args.output, args.phase)
    state = {"status": "guided-failed", "exit_code": 3, "phase": args.phase,
             "guided": True, "repo": str(REPO), "host_root": str(HOST),
             "dev_root": str(DEV_ROOT), "output": str(out), "started_utc": utc(),
             "detail": "", "gesture_timeout": args.gesture_timeout,
             "idle_gap": args.idle_gap, "knobs": args.knobs, "origin": {},
             "collection_rc": None, "persistence": {"stale_contacts": None},
             "notes": ["evdev observations are not desktop/application gesture acceptance",
                       "no install, no MOK change, no reboot, no suspend, no force unload",
                       "the persistent modprobe.d config is never written"]}
    log, results = [], []

    def _interrupt(signum, _frame):
        if state.get("restoring"):
            print(f"\nsignal {signum} while restoring the driver: ignored until the"
                  " restore finishes", file=sys.stderr, flush=True)
            return
        raise KeyboardInterrupt

    signal.signal(signal.SIGTERM, _interrupt)
    signal.signal(signal.SIGINT, _interrupt)
    os.umask(0o077)
    out.mkdir(parents=True)
    try:
        if HOST == Path("/") and os.geteuid() != 0:
            raise Stop(2, "preflight-failed",
                       "the guided run needs root for /dev/input and modprobe:"
                       " sudo python3 tools/qualify-touch.py --guided")
        collect_rc, collected = run(["bash", str(EVIDENCE / "collect.sh"),
                                    "--output", str(out / "collection")], 300)
        state["collection_rc"] = collect_rc
        (out / "collection.log").write_text(collected + "\n")
        known = facts()
        state.update(kernel=known["kernel"], checkout_head=known["head"],
                     version_file=known["version"])
        snapshot(out, args, known)
        guided_secure_boot(out)
        panel, platform = guided_panel()
        state.update(panel=str(panel), platform=str(platform))
        clients = module_clients(DRIVER)
        gates = preflight(args, known) + [
            {"name": "single-bound-device",
             "result": "pass" if clients == [str(panel)] else "fail",
             "detail": f"devices bound to {DRIVER}: {clients or '[none]'} (a global unload may"
                       f" only serve {panel}; a second client is never taken down for it)"},
            {"name": "bound-panel", "result": "pass" if bound_driver(panel) == DRIVER else "fail",
             "detail": f"{panel} driver={bound_driver(panel) or '[unbound]'} (needs {DRIVER};"
                       " this tool never displaces another driver)"},
            {"name": "bound-controller",
             "result": "pass" if bound_driver(platform) == CONTROLLER_DRIVER else "fail",
             "detail": f"{platform} driver={bound_driver(platform) or '[unbound]'}"
                       f" (needs {CONTROLLER_DRIVER})"}]
        write_preflight(out, gates)
        if collect_rc != 0:
            raise Stop(6, "collection-failed", f"collect.sh exit {collect_rc}: no host"
                                               " evidence, so no verdict (see collection.log)")
        failed = [item["name"] for item in gates if item["result"] != "pass"]
        if failed:
            raise Stop(2, "preflight-failed", "failed gates: " + ", ".join(failed))

        # Snapshot the origin BEFORE anything can change, on disk as well as in
        # memory, then take consent, and only then touch the screen.
        origin = {"params": hid_params(), "driver": bound_driver(panel),
                  "platform_driver": bound_driver(platform),
                  "config_sha": hashlib.sha256(CONF.read_bytes()).hexdigest()
                  if CONF.is_file() else "[absent]", "changed": False}
        origin["expect"] = {name: origin["params"].get(name, "")
                            for name in ("raw_mode", "std_raw_transition", "raw_input_beta")}
        state["origin"] = origin
        write_origin_backup(out, origin)
        ask_consent(panel, origin)

        device, rng = guided_device(panel, "standard")
        state["devices"] = {"standard": device}
        state["persistence"] = capture_windows(out, args, device, rng, "standard", results)

        # Do not put the machine through a reload for a profile that already
        # failed: no bridge gesture set is worth a dead touchscreen.
        standard_failed = [r for r in results
                           if r["profile"] == "standard" and r["status"] != "pass"]
        if standard_failed:
            raise Stop(4, "gesture-fail",
                       "the standard profile did not pass (" + ", ".join(
                           f"{r['name']}: {r['reason'] or r['status']}" for r in standard_failed)
                       + "): the bridge reload was NOT attempted, the driver was never changed")

        transition_to_bridge(panel, log, state)

        device, rng = guided_device(panel, "bridge")
        state["devices"]["bridge"] = device
        state["persistence"] = capture_windows(out, args, device, rng, "bridge", results)
        state.update(status="guided-evaluated", exit_code=0)
    except Stop as stop:
        state.update(status=stop.status, exit_code=stop.code, detail=stop.detail)
    except KeyboardInterrupt:
        state.update(status="interrupted", exit_code=130,
                     detail="interrupted (Ctrl-C/SIGTERM): the capture was bounded and the"
                            " driver is being restored")
    except Exception as error:  # a bug here must not cost the operator the restore
        state.update(status="internal-error", exit_code=13,
                     detail=f"{type(error).__name__}: {error}"
                            " (artefacts kept, the driver is being restored)")
        traceback.print_exc()
    finally:
        state["restoring"] = True
        try:
            ok, lines = restore(out, state, log)
        except Exception as error:  # never lose the recovery path to a bug here
            ok, lines = False, [f"restore raised {error!r}"]
        state["restore"] = {"ok": ok, "lines": lines}
        (out / "transition.txt").write_text("\n".join(
            ["# runtime transition (no persistent file was written)", *log, "", "# restore",
             *lines]) + "\n")
        verdict = guided_verdict(results, state["persistence"], state["phase"],
                                 state.get("collection_rc"))
        state["verdicts"] = verdict["items"]
        (out / "verdicts.txt").write_text(render_verdicts(verdict))
        payload = {"tool": "tools/qualify-touch.py --guided", "phase": args.phase,
                   "started_utc": state["started_utc"], "ended_utc": utc(),
                   "kernel": state.get("kernel"), "checkout_head": state.get("checkout_head"),
                   "gesture_timeout_seconds": args.gesture_timeout,
                   "idle_gap_seconds": args.idle_gap, "knobs": args.knobs,
                   "devices": state.get("devices", {}), "gestures": results,
                   "persistence": state["persistence"], "transition_log": log,
                   "restore": state["restore"], "verdicts": verdict["items"],
                   "observations_not_proof": "raw evdev records of ONE selected direct touch"
                                             " node, scored against ABS min/max: this is not"
                                             " desktop/application gesture acceptance, not a"
                                             " cold-boot or resume result, and not a PR verdict"}
        (out / "machine-results.json").write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n")
        if not ok:
            state.update(status="restore-failed", exit_code=7,
                         detail="could not restore the original runtime parameters/binding:"
                                " see recovery.txt")
            write_recovery(out, state)
        elif state["status"] == "guided-evaluated":
            state.update(status=verdict["status"], exit_code=verdict["exit_code"])
        state["ended_utc"] = utc()
        (out / "status.json").write_text(json.dumps(state, indent=2, sort_keys=True) + "\n")
        write_checksums(out)
    print(f"{state['status']} (exit {state['exit_code']}) -> {out}")
    return state["exit_code"]


def main(argv=None):
    args = parse(argv)
    if args.guided:
        return guided(args)
    out = resolve_output(args.output, args.phase)
    state = {"status": "capture-failed", "exit_code": 3, "phase": args.phase,
             "bridge": args.bridge, "duration_seconds": args.duration, "repo": str(REPO),
             "host_root": str(HOST), "output": str(out), "started_utc": utc(), "outcome": None,
             "collection_rc": None, "capture_pgid": None, "detail": "",
             "notes": ["the installed-head stamp is a local file written by the installer:"
                       " stamp and srcversion agreement are consistency checks, not"
                       " attestation of the exact running source"]}

    def _interrupt(_signum, _frame):
        raise KeyboardInterrupt

    signal.signal(signal.SIGTERM, _interrupt)
    os.umask(0o077)
    out.mkdir(parents=True)
    process = log = None
    try:
        collect_rc, collected = run(["bash", str(EVIDENCE / "collect.sh"),
                                    "--output", str(out / "collection")], 300)
        state["collection_rc"] = collect_rc
        (out / "collection.log").write_text(collected + "\n")
        known = facts()
        state.update(kernel=known["kernel"], checkout_head=known["head"], version_file=known["version"])
        snapshot(out, args, known)
        results = preflight(args, known)
        write_preflight(out, results)
        if collect_rc != 0:
            raise Stop(6, "collection-failed", f"collect.sh exit {collect_rc}: no host evidence,"
                                               " so no verdict (see collection.log)")
        failed = [item["name"] for item in results if item["result"] != "pass"]
        if failed:
            raise Stop(2, "preflight-failed", "failed gates: " + ", ".join(failed))
        declare_readiness(args.bridge)
        process, log = spawn_capture(out, args)
        state["capture_pgid"] = process.pid
        try:
            capture_rc = process.wait(timeout=args.duration + GRACE)
        except subprocess.TimeoutExpired:
            capture_rc = 124
        log.close()
        if capture_rc != 0:
            raise Stop(3, "capture-failed",
                       f"capture helper exit {capture_rc} (see capture.log, artefacts kept)")
        outcome, state["exit_code"], state["status"] = ask_outcome()
        state["outcome"] = outcome
        gestures = GESTURES["bridge" if args.bridge else "standard"]
        lines = [f"phase_declared={args.phase}", "gestures:"] + [f"- {g}" for g in gestures]
        lines.append(f"outcome={outcome} (operator declaration, recorded {utc()})")
        (out / "observations.txt").write_text("\n".join(lines) + "\n")
    except Stop as stop:
        state.update(status=stop.status, exit_code=stop.code, detail=stop.detail)
    except KeyboardInterrupt:
        state.update(status="interrupted", exit_code=130,
                     detail="interrupted (Ctrl-C/SIGTERM): capture group terminated")
    finally:
        if process is not None:
            kill_group(process.pid)  # the pgid survives a reaped direct child
            try:
                process.wait(timeout=KILL_GRACE)  # reap only after the group is gone
            except subprocess.TimeoutExpired:
                pass
        if log:
            log.close()
        state["ended_utc"] = utc()
        (out / "status.json").write_text(json.dumps(state, indent=2, sort_keys=True) + "\n")
        write_checksums(out)
    print(f"{state['status']} (exit {state['exit_code']}) -> {out}")
    return state["exit_code"]


if __name__ == "__main__":
    sys.exit(main())
