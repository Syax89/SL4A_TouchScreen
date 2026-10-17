#!/usr/bin/env python3
"""Sandbox contract for tools/qualify-touch.py: fake repo, fake host, fake helpers.

Never touches the real host: the tool is copied into a throwaway repository whose
hardware_evidence helpers are fakes, and every host path goes through
SL4A_QUALIFY_HOST_ROOT. git is real (the clean-tree gate must be real); the rest is a
stub. Fixtures mirror the layout a real kernel exposes, underscores included, and
every hidden path a regression could read by mistake exists as a decoy with wrong
contents.
"""
import hashlib, json, os, shutil, signal, subprocess, sys, tempfile, time
from pathlib import Path

TOOL = Path(__file__).resolve().parents[1] / "tools/qualify-touch.py"
VERSION, PARAMS = "1.7.1", ("raw_mode", "std_raw_transition", "raw_input_beta")
KERNEL = "6.12.0-sandbox"
TOP_LEVEL = ["capture", "capture.log", "checksums.txt", "collection", "collection.log",
             "observations.txt", "preflight.txt", "snapshot", "status.json"]
COMMON = '''#!/bin/bash
out=""
while [ $# -gt 0 ]; do case "$1" in --output) out="$2"; shift 2;; *) shift;; esac; done
'''
COLLECT_OK = COMMON + 'mkdir -p "$out/kernel"; printf \'fake dmesg tail\\n\' > "$out/kernel/dmesg.txt"\n'
COLLECT_FAIL = COMMON + 'echo "collect stdout note"; echo "collect stderr note" >&2; exit 5\n'
# "orphan" is the interesting one: the helper exits 0 at once while leaving a
# grandchild that ignores TERM, so only a group-wide TERM-then-KILL reaches it.
CAPTURE = COMMON + '''case "${FAKE_CAPTURE_MODE:-ok}" in
  fail) echo "fake capture exploded" >&2; exit 7;;
  spawn) mkdir -p "$out"; sleep 300 & child=$!; printf '%s %s\\n' "$$" "$child" > "$out/pids"; wait;;
  orphan) mkdir -p "$out"; ( trap '' TERM; sleep 300 ) & child=$!
          printf '%s %s\\n' "$$" "$child" > "$out/pids"; exit 0;;
  *) mkdir -p "$out"; printf 'fake trace\\n' > "$out/trace.txt";;
esac
'''
# The dkms stub prints the rows it is given: real dkms filters by -m/-k itself, but a
# parser that trusts that filter cannot notice a wrong kernel or a foreign package.
DKMS = '''#!/bin/bash
if [ "${FAKE_DKMS_RC:-0}" != 0 ]; then echo "dkms: cannot read status" >&2; exit "$FAKE_DKMS_RC"; fi
if [ -n "${FAKE_DKMS_ROWS:-}" ]; then cat "$FAKE_DKMS_ROWS"; exit 0; fi
k=sandbox
while [ $# -gt 0 ]; do case "$1" in -k) k="$2";; esac; shift; done
printf 'sl4a-touch/%s, %s, x86_64: installed\\n' \\
  "$(cat "$SL4A_QUALIFY_HOST_ROOT/var/lib/sl4a-touch/dkms-version")" "$k"
'''
GIT_STUB = '''#!/bin/bash
if [ "$1" = "-C" ]; then shift 2; fi
if [ "$1" = "status" ]; then
  [ -n "${FAKE_GIT_STATUS_STDOUT:-}" ] && printf '%s\\n' "$FAKE_GIT_STATUS_STDOUT"
  printf '%s\\n' "${FAKE_GIT_STATUS_ERR-fatal: unknown git failure}" >&2
  exit 128
fi
exec "{git}" "$@"
'''
STUBS = {
    "uname": '#!/bin/bash\n[ "$1" = -r ] && echo ' + KERNEL + ' || echo "Linux sandbox ' + KERNEL + ' #1"\n',
    "dkms": DKMS,
    "modinfo": '#!/bin/bash\nf=""; [ "$1" = -F ] && { f="$2"; shift 2; }\ncase "$f" in vermagic)'
               ' echo "$(uname -r) SMP preempt mod_unload";; srcversion) echo fresh1234;; *) echo "$1";; esac\n',
    "mokutil": '#!/bin/bash\necho "SecureBoot enabled"\n',
    "systemctl": '#!/bin/bash\n[ "$1" = is-active ] && echo active || echo enabled\n',
}
GATES = [("build-identity", {"stamp": None}, []), ("build-identity", {"stamp": "0" * 40}, []),
         ("repo-clean", {"dirty": True}, []), ("dkms-version", {"dkms_version": "1.7.0"}, []),
         ("srcversion-sl4a-spi-hid", {"srcversion": "stale9999"}, []),
         ("params-standard", {"params": ("N", "1", "Y")}, []),
         ("params-bridge", {"params": ("N", "0", "Y")}, ["--bridge"]),
         # both touchscreen nodes on one board: a MSHW02* glob would have missed 0162
         ("acpi-mshw0231", {"params": ("N", "1", "Y"), "acpi": ("MSHW0162", "MSHW0231")},
          ["--bridge"])]


def write(path, text, mode=0o644):
    path.parent.mkdir(parents=True, exist_ok=True); path.write_text(text); path.chmod(mode)


def git(repo, *args):
    done = subprocess.run(["git", "-c", "user.email=t@e", "-c", "user.name=t", *args], cwd=repo,
                          capture_output=True, text=True)
    assert done.returncode == 0, done.stderr


def build(root, stamp="HEAD", acpi=("MSHW0231",), params=("N", "0", "Y"), srcversion="fresh1234",
          dkms_version=VERSION, dirty=False, decoys=True, dkms_rows=None, dkms_rc=None,
          git_status=None, collect_fail=False):
    """A fake repo (real git) plus a fake host tree and stub PATH helpers."""
    repo, host, stubs = root / "repo", root / "host", root / "bin"
    helpers = repo / "tools/hardware_evidence"
    helpers.mkdir(parents=True)
    shutil.copy2(TOOL, repo / "tools/qualify-touch.py")
    write(repo / "VERSION", VERSION + "\n")
    write(helpers / "collect.sh", COLLECT_FAIL if collect_fail else COLLECT_OK)
    write(helpers / "capture_linux_trace_bundle.sh", CAPTURE)
    git(repo, "init", "-q"); git(repo, "add", "-A"); git(repo, "commit", "-qm", "sandbox")
    head = subprocess.run(["git", "-C", str(repo), "rev-parse", "HEAD"], capture_output=True,
                          text=True).stdout.strip()
    world = {"root": root, "repo": repo, "host": host, "env": {}}
    for name, stub in STUBS.items(): write(stubs / name, stub, 0o755)
    if git_status is not None:  # a git that fails `status` while printing nothing
        real = subprocess.run(["command", "-v", "git"], shell=True, capture_output=True,
                              text=True).stdout.strip()
        write(stubs / "git", GIT_STUB.replace("{git}", real), 0o755)
        world["env"]["FAKE_GIT_STATUS_ERR"] = git_status
    if dkms_rc is not None:
        world["env"]["FAKE_DKMS_RC"] = str(dkms_rc)
    if dkms_rows is not None:
        rows = root / "dkms-rows.txt"
        write(rows, "".join(f"{row}\n" for row in dkms_rows))
        world["env"]["FAKE_DKMS_ROWS"] = str(rows)
    for module in ("sl4a-spi-amd", "sl4a-spi-hid"):  # sysfs spells modules with underscores
        write(host / "sys/module" / module.replace("-", "_") / "srcversion", srcversion + "\n")
    for name, value in zip(PARAMS, params):
        write(host / "sys/module/sl4a_spi_hid/parameters" / name, value + "\n")
    if decoys:  # no real kernel creates these: reading them is the regression
        for module in ("sl4a-spi-amd", "sl4a-spi-hid"):
            write(host / "sys/module" / module / "srcversion", "decoy9999\n")
        write(host / "sys/module/sl4a-spi-hid/parameters/raw_mode", "Y\n")
        write(host / "sys/module/sl4a-spi-hid/parameters/std_raw_transition", "1\n")
    for uid in acpi:
        (host / "sys/bus/acpi/devices" / f"{uid}:00").mkdir(parents=True)
    fixed = {"proc/sys/kernel/random/boot_id": "sandbox-boot-id\n", "proc/uptime": "12.5 9.0\n",
             "etc/modprobe.d/sl4a-spi-hid.conf": "options sl4a-spi-hid raw_mode=0\n",
             "etc/systemd/system/sl4a-touch-activate.service": "[Service]\n",
             "var/lib/sl4a-touch/dkms-version": dkms_version + "\n"}
    for name, text in fixed.items(): write(host / name, text)
    if stamp is not None:
        write(host / "var/lib/sl4a-touch/installed-head", (head if stamp == "HEAD" else stamp) + "\n")
    if dirty:
        write(repo / "UNCOMMITTED", "x\n")
    return world


def run_tool(world, *cli, stdin="\np\n", mode="ok"):
    env = dict(os.environ, PATH=f"{world['root'] / 'bin'}:{os.environ['PATH']}",
               SL4A_QUALIFY_HOST_ROOT=str(world["host"]), FAKE_CAPTURE_MODE=mode,
               **world.get("env", {}))
    return subprocess.run([sys.executable, str(world["repo"] / "tools/qualify-touch.py"), *cli],
                          input=stdin, text=True, capture_output=True, env=env, timeout=180)


def status(out):
    return json.loads((out / "status.json").read_text())


def preflight(out):
    return (out / "preflight.txt").read_text()


def assert_checksums(out):
    for line in (out / "checksums.txt").read_text().splitlines():
        digest, name = line.split("  ", 1)
        assert hashlib.sha256((out / name).read_bytes()).hexdigest() == digest, f"bad {name}"


def wait_gone(pids, timeout=30):
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if not [pid for pid in pids if os.path.exists(f"/proc/{pid}")]:
            return True
        time.sleep(0.1)
    return False


def test_success(tmp):
    world, out = build(tmp), tmp / "bundle"
    done = run_tool(world, "--output", str(out), "--phase", "resume", "--duration", "5")
    assert done.returncode == 0, done.stderr
    assert status(out)["status"] == "qualified", status(out)
    assert status(out)["host_root"] == str(world["host"]), "sandbox root not recorded"
    assert sorted(p.name for p in out.iterdir()) == TOP_LEVEL, "unexpected files in the bundle"
    assert oct(out.stat().st_mode & 0o777) == "0o700", "bundle is not 0700"
    gates = preflight(out)
    assert "result=fail" not in gates
    assert "loaded=fresh1234" in gates, "srcversion not read from /sys/module/sl4a_spi_*"
    assert "decoy9999" not in gates, "read the hyphenated sysfs path that does not exist"
    assert "raw_mode=N" in (out / "snapshot/host.txt").read_text(), "params not recorded"
    assert "sl4a_spi_hid" in (out / "snapshot/host.txt").read_text(), "sysfs path not recorded"
    assert "[absent:" not in (out / "snapshot/modprobe.conf").read_text()
    assert (out / "collection/kernel/dmesg.txt").is_file(), "collect.sh not run"
    assert_checksums(out)
    bridge = tmp / "bridge"  # opt-in multitouch profile: N/1/Y params, exact MSHW0231 node
    assert run_tool(build(tmp / "b", params=("N", "1", "Y")), "--output", str(bridge),
                    "--bridge", "--duration", "5").returncode == 0
    gates = preflight(bridge)
    assert "params-bridge: result=pass" in gates and "['MSHW0231']" in gates
    gestures = (bridge / "observations.txt").read_text()
    assert "five-finger hold" in gestures and "pinch" in gestures, "bridge gestures incomplete"
    assert "recontact" in gestures and "edge and corner" in gestures, "edge/recontact missing"


def test_identity_gates(tmp):
    for index, (gate, options, cli) in enumerate(GATES):
        world = build(tmp / f"case{index}", **options)
        out = tmp / f"case{index}" / "bundle"
        done = run_tool(world, "--output", str(out), *cli)
        assert done.returncode == 2, f"{gate}: exit {done.returncode} {done.stderr}"
        assert status(out)["status"] == "preflight-failed", gate
        assert f"{gate}: result=fail" in preflight(out), f"{gate} not reported"
        assert not (out / "capture").exists(), f"{gate}: captured anyway"
        assert (out / "collection/kernel/dmesg.txt").is_file(), f"{gate}: evidence lost"
        assert_checksums(out)


def test_dkms_identity(tmp):
    """dkms rc, the wrong kernel, a foreign package and contradictory rows all fail."""
    cases = [("rc", {"dkms_rc": 3}),
             ("kernel", {"dkms_rows": [f"sl4a-touch/1.7.1, 6.1.0-other, x86_64: installed"]}),
             ("package", {"dkms_rows": [f"nvidia/535.104, {KERNEL}, x86_64: installed"]}),
             ("conflict", {"dkms_rows": [f"sl4a-touch/1.7.1, {KERNEL}, x86_64: installed",
                                         f"sl4a-touch/1.7.0, {KERNEL}, x86_64: installed"]})]
    for index, (label, options) in enumerate(cases):
        world = build(tmp / f"bad{index}", **options)
        out = tmp / f"bad{index}" / "bundle"
        done = run_tool(world, "--output", str(out), "--duration", "5")
        assert done.returncode == 2, f"{label}: exit {done.returncode}"
        detail = [ln for ln in preflight(out).splitlines() if ln.startswith("dkms-version:")]
        assert detail and "result=fail" in detail[0], f"{label}: not reported ({detail})"
    # positive control: unrelated and other-kernel rows next to the right one still pass
    world = build(tmp / "good", dkms_rows=[f"nvidia/535.104, {KERNEL}, x86_64: installed",
                                           f"sl4a-touch/1.7.1, 6.1.0-other, x86_64: installed",
                                           f"sl4a-touch/{VERSION}, {KERNEL}, x86_64: installed"])
    assert run_tool(world, "--output", str(tmp / "good" / "bundle"),
                    "--duration", "5").returncode == 0, "a correct row was rejected"


def test_git_status_failure(tmp):
    """A failing/unknown `git status` fails the gate, message or no message at all."""
    cases = [("dubious-ownership", "fatal: detected dubious ownership in repository at '/x'",
              ("result=fail", "safe.directory", "never")),
             ("silent-failure", "", ("result=fail", "empty output"))]  # blank stdout, rc 128
    for index, (label, err, expected) in enumerate(cases):
        world = build(tmp / f"git{index}", git_status=err)
        out = tmp / f"git{index}" / "bundle"
        done = run_tool(world, "--output", str(out), "--duration", "5")
        assert done.returncode == 2, f"{label}: exit {done.returncode}"
        detail = [ln for ln in preflight(out).splitlines() if ln.startswith("repo-clean:")]
        assert detail, f"{label}: gate missing"
        for fragment in expected:
            assert fragment in detail[0], f"{label}: {fragment!r} missing from {detail[0]!r}"
        if label == "dubious-ownership":
            assert str(world["repo"]) in detail[0], "ownership hint lacks the exact clone"
        assert not (out / "capture").exists(), f"{label}: captured without a known-clean tree"


def test_collection_failure(tmp):
    world, out = build(tmp, collect_fail=True), tmp / "bundle"
    done = run_tool(world, "--output", str(out), "--duration", "5")
    assert done.returncode == 6, f"collection failure ignored (exit {done.returncode})"
    assert status(out)["status"] == "collection-failed", status(out)
    assert status(out)["collection_rc"] == 5, status(out)
    log = (out / "collection.log").read_text()
    assert "collect stdout note" in log and "collect stderr note" in log, "streams lost"
    assert not (out / "capture").exists(), "captured without host evidence"
    assert_checksums(out)


def test_outcomes(tmp):
    for answer, expect in (("f", (4, "operator-fail")), ("n", (5, "incomplete")), ("", (5, "incomplete"))):
        world = build(tmp / f"case{answer or 'eof'}")
        out = world["root"] / "bundle"
        done = run_tool(world, "--output", str(out), stdin=f"\n{answer}\n", mode="ok")
        assert (done.returncode, status(out)["status"]) == expect, f"{answer!r}: {done.returncode}"
        assert_checksums(out)


def test_capture_failure(tmp):
    world, out = build(tmp), tmp / "bundle"
    assert run_tool(world, "--output", str(out), mode="fail").returncode == 3  # helper exits 7
    assert status(out)["status"] == "capture-failed"
    assert "exploded" in (out / "capture.log").read_text(), "helper output not preserved"
    assert_checksums(out)


def test_interrupt_cleanup(tmp):
    world = build(tmp)
    out = tmp / "bundle"
    env = dict(os.environ, PATH=f"{world['root'] / 'bin'}:{os.environ['PATH']}",
               SL4A_QUALIFY_HOST_ROOT=str(world["host"]), FAKE_CAPTURE_MODE="spawn")
    process = subprocess.Popen([sys.executable, str(world["repo"] / "tools/qualify-touch.py"),
                                "--output", str(out), "--duration", "5"], env=env, text=True,
                               stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    try:
        process.stdin.write("\n\n")
        process.stdin.flush()
        for _ in range(600):  # the fake capture announces itself, then waits with a child
            if (out / "capture/pids").exists():
                break
            time.sleep(0.1)
        pids = [int(pid) for pid in (out / "capture/pids").read_text().split()]
        process.send_signal(signal.SIGTERM)
        assert process.wait(timeout=60) == 130, "interrupt did not report 130"
        assert wait_gone(pids), f"descendants survived: {pids}"
        assert status(out)["status"] == "interrupted", status(out)
        assert not (out / "observations.txt").exists(), "an outcome was recorded anyway"
        assert_checksums(out)
    finally:
        if process.poll() is None:
            process.kill()


def test_orphan_cleanup(tmp):
    """The helper exits 0 and leaves a TERM-ignoring grandchild: KILL must still reach it.

    The grace is the tool's real one (10s), so this takes about that long on purpose:
    shortening it for the test would stop testing the shipped constant.
    """
    world, out = build(tmp), tmp / "bundle"
    unrelated = subprocess.Popen(["sleep", "300"])  # another group: must survive cleanup
    try:
        done = run_tool(world, "--output", str(out), "--duration", "5", mode="orphan")
        pids = [int(pid) for pid in (out / "capture/pids").read_text().split()]
        assert done.returncode == 0, done.stderr
        assert status(out)["status"] == "qualified", status(out)
        assert len(pids) == 2, pids
        assert wait_gone(pids), f"TERM-resistant orphan survived the group KILL: {pids}"
        assert unrelated.poll() is None, "cleanup killed a process outside the capture group"
        assert_checksums(out)
    finally:
        unrelated.kill(); unrelated.wait()


def test_output_rejection(tmp):
    world = build(tmp); (tmp / "existing").mkdir(); (tmp / "link").symlink_to(tmp / "nowhere")
    for target in (tmp / "existing", tmp / "link", world["repo"] / "bundle", world["repo"].parent):
        done = run_tool(world, "--output", str(target))
        assert done.returncode != 0, f"{target}: accepted"
        assert "refusing" in done.stderr, f"{target}: {done.stderr!r}"
        assert "Traceback" not in done.stderr, f"{target}: crash instead of refusal"
    assert not (world["repo"] / "bundle").exists()


def main():
    tests = (test_success, test_identity_gates, test_dkms_identity, test_git_status_failure,
             test_collection_failure, test_outcomes, test_capture_failure,
             test_interrupt_cleanup, test_orphan_cleanup, test_output_rejection)
    failures = 0
    for test in tests:
        tmp = Path(tempfile.mkdtemp(prefix="sl4a-qualify-"))
        try:
            test(tmp); print(f"ok   {test.__name__}")
        except AssertionError as failure:
            failures += 1; print(f"FAIL {test.__name__}: {failure}")
        finally:
            shutil.rmtree(tmp, ignore_errors=True)
    print(f"{len(tests) - failures}/{len(tests)} qualifier sandbox checks passed")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
