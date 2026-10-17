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
"""
import argparse, hashlib, json, os, signal, subprocess, sys, time
from datetime import datetime, timezone
from pathlib import Path

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
    args = parser.parse_args(argv)
    if not 5 <= args.duration <= 600:
        parser.error("--duration must be between 5 and 600 seconds")
    return args


def main(argv=None):
    args = parse(argv)
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
