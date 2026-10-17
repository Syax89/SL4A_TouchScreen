#!/usr/bin/env python3
"""Contracts for the guided (automatic) touchscreen qualification.

Two halves, both runnable without hardware and without root:

  1. `touch_evdev` — the stdlib decoder/scorer — against synthetic evdev
     streams: Type B slot/tracking-id lifecycle and slot reuse, Type A and
     plain single-touch, SYN_DROPPED, a trailing partial record, a silent
     window (timeout), EOF, contacts still down at the open or at the end of a
     window, edges normalized to the device's ABS min/max, and knob overrides.

  2. `qualify-touch.py --guided` end to end in a throwaway repository with a
     fake host tree (real sysfs layout, underscores included), stub PATH
     helpers, a modprobe stub that records every argument and rewrites the fake
     module parameters, and FIFOs standing in for the evdev char devices. The
     scripted streams are fed per gesture window by a feeder thread.

The safety assertions are the point of half 2: after success, after a failed
gesture set, after a failed transition and after SIGTERM, the fake machine must
be back on its original parameters and binding, the persistent config must be
byte-identical, and no `--force`/rmmod -f/reboot/suspend/systemctl call may
appear anywhere.

    python3 tests/touch_guided_test.py            # or: python3 -m unittest ...
"""
import json
import os
import queue
import re
import shutil
import signal
import struct
import subprocess
import sys
import tempfile
import threading
import time
import unittest
from pathlib import Path
from unittest import mock
import importlib.util

ROOT = Path(__file__).resolve().parents[1]
TOOL = ROOT / "tools/qualify-touch.py"
sys.path.insert(0, str(ROOT / "tools"))

import touch_evdev as evdev  # noqa: E402

KERNEL = "6.12.0-sandbox"
RANGE = {"x": (0, 32767), "y": (0, 32767)}
PARAMS = {"raw_mode": "N", "std_raw_transition": "0", "raw_input_beta": "N",
          "skip_getfeat": "N", "sl4a_debug_level": "0", "win_host_addr": "(null)"}
# ABS bitmask of the driver's own MT device: X, Y, MT slot, MT X, MT Y, tracking id
MT_ABS = hex(sum(1 << code for code in (evdev.ABS_X, evdev.ABS_Y, evdev.ABS_MT_SLOT,
                                        evdev.ABS_MT_POSITION_X, evdev.ABS_MT_POSITION_Y,
                                        evdev.ABS_MT_TRACKING_ID)))


# ── synthetic evdev streams ────────────────────────────────────────────────

def rec(t, typ, code, val):
    sec = int(t)
    return struct.pack("@llHHi", sec, int(round((t - sec) * 1e6)), typ, code, val)


def syn(t):
    return rec(t, evdev.EV_SYN, evdev.SYN_REPORT, 0)


def dropped(t):
    return rec(t, evdev.EV_SYN, evdev.SYN_DROPPED, 0)


def slot(t, value):
    return rec(t, evdev.EV_ABS, evdev.ABS_MT_SLOT, value)


def tid(t, value):
    return rec(t, evdev.EV_ABS, evdev.ABS_MT_TRACKING_ID, value)


def px(t, value):
    return rec(t, evdev.EV_ABS, evdev.ABS_MT_POSITION_X, value)


def py(t, value):
    return rec(t, evdev.EV_ABS, evdev.ABS_MT_POSITION_Y, value)


def btn(t, value):
    return rec(t, evdev.EV_KEY, evdev.BTN_TOUCH, value)


def stream(*parts):
    return b"".join(parts)


def frames_of(data):
    return evdev.frames(evdev.decode(data)[0])


def point(t, x, y, ident=1):
    """One Type B contact report for slot 0 (a single finger on the panel)."""
    return stream(slot(t, 0), tid(t, ident), px(t, x), py(t, y), syn(t))


def lift(t, ident):
    return stream(slot(t, 0), tid(t, -1), syn(t))


def single(t, x, y, down=True):
    """One plain single-touch report (BTN_TOUCH + ABS_X/ABS_Y)."""
    return stream(btn(t, 1 if down else 0), rec(t, evdev.EV_ABS, evdev.ABS_X, x),
                  rec(t, evdev.EV_ABS, evdev.ABS_Y, y), syn(t))


def tap_stream():
    return single(0.0, 16000, 16000) + single(0.06, 16010, 15990) + single(0.14, 16000, 16000, False)


def motion_stream():
    return stream(*[single(i * 0.03, 3000 + i * 2200, 3000 + i * 2100) for i in range(12)],
                  single(0.36, 27000, 26000, False))


def edges_stream(margin=0):
    corners = [(margin, margin), (32767 - margin, margin),
               (32767 - margin, 32767 - margin), (margin, 32767 - margin)]
    return stream(*[single(i * 0.05, x, y) for i, (x, y) in enumerate(corners)],
                  single(0.25, 0, 0, False))


def b_motion_stream():
    """A Type B (slot + tracking id) diagonal drag, 12 reports, then one lift."""
    return stream(*[point(i * 0.03, 3000 + i * 2200, 3000 + i * 2100, ident=3)
                    for i in range(12)], lift(0.36, 3))


def b_edges_stream(margin=0):
    corners = [(margin, margin), (32767 - margin, margin),
               (32767 - margin, 32767 - margin), (margin, 32767 - margin)]
    return stream(*[point(i * 0.05, x, y, ident=4) for i, (x, y) in enumerate(corners)],
                  lift(0.25, 4))


def five_stream(fingers=5, staggered=False, steps=20, release=True):
    parts = []
    for step in range(steps):  # a SUSTAINED hold: an instant report is not one
        t = step * 0.02
        for f in range(fingers):
            parts.append(stream(slot(t, f), tid(t, 100 + f), px(t, 3000 + f * 3000),
                                py(t, 6000 + f * 2000)))
        parts.append(syn(t))
    release_at = steps * 0.02 + 0.02
    if not release:  # the fingers never come up
        return stream(*parts)
    if staggered:  # one finger per report: a real "staggered" lift
        for index, f in enumerate(range(fingers)):
            t = release_at + 0.02 * index
            parts.append(stream(slot(t, f), tid(t, -1), syn(t)))
    else:  # every finger in the same report: a simultaneous lift
        parts.append(stream(*[stream(slot(release_at, f), tid(release_at, -1))
                              for f in range(fingers)], syn(release_at)))
    return stream(*parts)


def pinch_stream(halves=(6000, 9000, 12000, 15000, 12000, 9000, 6000, 3000, 2000)):
    parts, t = [], 0.0
    for half in halves:
        t += 0.03
        parts.append(stream(slot(t, 0), tid(t, 1), px(t, 16000 - half), py(t, 16000),
                            slot(t, 1), tid(t, 2), px(t, 16000 + half), py(t, 16000), syn(t)))
    t += 0.03
    parts.append(stream(slot(t, 0), tid(t, -1), slot(t, 1), tid(t, -1), syn(t)))
    return stream(*parts)


def recontact_stream(new_tid=8, drift=60):
    return stream(point(0.0, 16000, 16000, ident=7), lift(0.10, 7),
                  point(0.24, 16000 + drift, 16000 + drift, ident=new_tid), lift(0.40, new_tid))


# ── half 1: decoder and scorer ─────────────────────────────────────────────

class DecoderTest(unittest.TestCase):
    """One assertion per rule the guided verdict depends on."""

    def test_type_b_tracking_id_lifecycle_and_slot_reuse(self):
        parsed = frames_of(recontact_stream())
        self.assertEqual([f["kind"] for f in parsed], ["B"] * 4)
        self.assertEqual(parsed[1]["released"], [(0, 7)], "the lift was not recorded")
        self.assertEqual(parsed[2]["contacts"][0]["slot"], 0, "slot reuse not visible")
        self.assertEqual(parsed[2]["contacts"][0]["tid"], 8)
        verdict = evdev.evaluate("recontact", parsed, RANGE)
        self.assertEqual(verdict["status"], "pass", verdict)
        self.assertTrue(verdict["metrics"]["slot_reused"])

    def test_recontact_rejects_a_reused_tracking_id(self):
        # same id after a lift: userspace would see one endless finger
        parsed = frames_of(recontact_stream(new_tid=7))
        verdict = evdev.evaluate("recontact", parsed, RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("tracking id 7 was reused", verdict["reason"])

    def test_recontact_rejects_a_far_recontact(self):
        parsed = frames_of(recontact_stream(drift=4000))
        verdict = evdev.evaluate("recontact", parsed, RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("farther than", verdict["reason"])

    def test_syn_dropped_fails_closed(self):
        parsed = frames_of(tap_stream() + dropped(0.2) + single(0.25, 16000, 16000))
        verdict = evdev.evaluate("tap", parsed, RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("syn_dropped", verdict["reason"])

    def test_partial_record_fails_closed(self):
        data = tap_stream() + b"\x00\x00\x00"
        records, leftover = evdev.decode(data)
        self.assertEqual(leftover, b"\x00\x00\x00", "a trailing fragment disappeared")
        verdict = evdev.evaluate("tap", evdev.frames(records), RANGE, ctx={"partial": True})
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("partial_record", verdict["reason"])

    def test_timeout_and_eof_fail_closed(self):
        self.assertIn("timeout", evdev.evaluate("tap", [], RANGE)["reason"])
        for context in ({"eof": True}, {"stale_contacts": True}):
            verdict = evdev.evaluate("tap", frames_of(tap_stream()), RANGE, ctx=context)
            self.assertEqual(verdict["status"], "fail", context)
            self.assertIn(list(context)[0], verdict["reason"])

    def test_contact_persistence_fails_closed(self):
        # the finger never comes up: a "tap" that never released is not a tap
        parsed = frames_of(single(0.0, 16000, 16000) + single(0.06, 16000, 16000))
        verdict = evdev.evaluate("tap", parsed, RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("never released", verdict["reason"])
        # ... and a five-finger hold that leaves fingers down
        held = five_stream(release=False)
        verdict = evdev.evaluate("five_hold", frames_of(held), RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("not every finger released", verdict["reason"])

    def test_single_touch_and_unsupported_type_a(self):
        verdict = evdev.evaluate("tap", evdev.frames(evdev.decode(tap_stream())[0], "single"), RANGE)
        self.assertEqual(verdict["status"], "pass", verdict)
        self.assertEqual(frames_of(tap_stream())[0]["kind"], "single")
        type_a = stream(tid(0.0, 5), px(0.0, 9000), py(0.0, 9000), syn(0.0))
        kind = evdev.protocol_kind(1 << evdev.ABS_MT_TRACKING_ID)
        self.assertEqual(kind, "A")
        parsed = evdev.frames(evdev.decode(type_a)[0], kind)
        self.assertEqual(evdev.evaluate("tap", parsed, RANGE, ctx={"protocol": kind})["status"], "not_tested")

    def test_motion_needs_travel_frames_and_continuity(self):
        self.assertEqual(evdev.evaluate("motion", frames_of(motion_stream()), RANGE)["status"], "pass")
        short = stream(*[single(i * 0.03, 16000, 16000) for i in range(12)],
                       single(0.36, 16000, 16000, False))
        verdict = evdev.evaluate("motion", frames_of(short), RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("travel", verdict["reason"])
        gapped = stream(*[single(t, 3000 + i * 3000, 3000 + i * 3000)
                          for i, t in enumerate((0, 0.03, 0.06, 0.09, 0.7, 0.75, 0.8))],
                        single(0.85, 21000, 21000, False))
        verdict = evdev.evaluate("motion", frames_of(gapped), RANGE)
        self.assertIn("not continuous", verdict["reason"])

    def test_edges_are_normalized_to_abs_min_max(self):
        # 1020 is 2% of the 1000..2000 range away from the edge (inside the 3%
        # edge_pct) but 3% of 0..32767 away from it (outside): the same stream
        # must pass against the device's own range and fail against a wrong one.
        tight = {"x": (1000, 2000), "y": (1000, 2000)}
        inside = stream(*[single(i * 0.05, x, y) for i, (x, y) in
                          enumerate([(1020, 1020), (1980, 1020), (1980, 1980), (1020, 1980)])],
                        single(0.25, 1500, 1500, False))
        self.assertEqual(evdev.evaluate("edges", frames_of(inside), tight)["status"], "pass")
        self.assertEqual(evdev.evaluate("edges", frames_of(inside), RANGE)["status"], "fail",
                         "edges were not normalized to the device range")
        off = stream(*[single(i * 0.05, x, y) for i, (x, y) in
                       enumerate([(1400, 1400), (1600, 1400), (1600, 1600), (1400, 1600)])],
                     single(0.25, 1500, 1500, False))
        verdict = evdev.evaluate("edges", frames_of(off), tight)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("not reached", verdict["reason"])
        self.assertEqual(verdict["metrics"]["abs_max"], [2000, 2000])

    def test_five_hold_accepts_one_lift_rejects_staggered(self):
        self.assertEqual(evdev.evaluate("five_hold", frames_of(five_stream()), RANGE)["status"], "pass")
        verdict = evdev.evaluate("five_hold", frames_of(five_stream(staggered=True)), RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("staggered", verdict["reason"])
        verdict = evdev.evaluate("five_hold", frames_of(five_stream(fingers=3)), RANGE)
        self.assertIn("only 3 concurrent contacts", verdict["reason"])

    def test_pinch_needs_both_directions_in_order(self):
        self.assertEqual(evdev.evaluate("pinch", frames_of(pinch_stream()), RANGE)["status"], "pass")
        only_out = pinch_stream(halves=(6000, 9000, 12000, 15000))
        verdict = evdev.evaluate("pinch", frames_of(only_out), RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("shrank", verdict["reason"])
        reversed_order = pinch_stream(halves=(12000, 6000, 3000, 2000, 9000, 15000))
        verdict = evdev.evaluate("pinch", frames_of(reversed_order), RANGE)
        self.assertIn("squeeze came before", verdict["reason"])

    def test_eviocgabs_encoding_is_the_kernel_one(self):
        # _IOC(_IOC_READ, 'E', 0x40 + abs, 24): a wrong literal here is an
        # ENOTTY on real hardware and every ABS range silently falls back.
        self.assertEqual(evdev.ev_iocgabs(evdev.ABS_X), 0x80184540)
        self.assertEqual(evdev.ev_iocgabs(evdev.ABS_Y), 0x80184541)
        self.assertEqual((evdev.ev_iocgabs(evdev.ABS_MT_POSITION_X) >> 16) & 0x3FFF, 24,
                         "input_absinfo size")
        self.assertEqual((evdev.ev_iocgabs(evdev.ABS_MT_POSITION_X) >> 8) & 0xFF, ord("E"))

    def test_recontact_is_not_tested_without_tracking_ids(self):
        # plain single-touch: a lift and a re-touch are indistinguishable here,
        # so the honest answer is "not tested", never a fabricated pass or fail
        verdict = evdev.evaluate("recontact", frames_of(tap_stream()), RANGE)
        self.assertEqual(verdict["status"], "not_tested", verdict)
        self.assertIn("no tracking ids", verdict["reason"])

    def test_five_hold_rejects_two_contacts_sharing_one_id(self):
        shared = stream(*[stream(*[stream(slot(t, f), tid(t, 5), px(t, 3000 + f * 2000),
                                    py(t, 6000)) for f in range(5)], syn(t))
                          for t in (0.0, 0.02)])
        released = stream(*[stream(slot(0.04, f), tid(0.04, -1)) for f in range(5)], syn(0.04))
        verdict = evdev.evaluate("five_hold", frames_of(shared + released), RANGE)
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("share one tracking id", verdict["reason"])

    def test_kernel_state_without_events_and_query_failure(self):
        keys = bytes(evdev.KEY_BYTES)
        slots = struct.pack("@3i", evdev.ABS_MT_TRACKING_ID, 42, -1)
        with mock.patch.object(evdev.fcntl, "ioctl", side_effect=[keys, slots]) as ioctl:
            state = evdev.down_state(77, "B", 2)
        self.assertTrue(state["supported"])
        self.assertEqual(state["tracking_ids"], [42])
        self.assertTrue(all(call.args[0] == 77 for call in ioctl.call_args_list))
        with mock.patch.object(evdev.fcntl, "ioctl", side_effect=[keys, OSError("lost device")]):
            self.assertFalse(evdev.down_state(77, "B", 2)["supported"])
        self.assertEqual(evdev.ev_iocgkey(96), 0x80604518)
        self.assertEqual(evdev.ev_iocgmtslots(12), 0x800C450A)
        self.assertEqual(evdev.ev_iocgname(256), 0x81004506)
        self.assertEqual(evdev.ev_iocgrab(), 0x40044590)

    def test_incomplete_frame_and_dangling_drop_fail_closed(self):
        for suffix in (tid(0.3, 99), dropped(0.3)):
            result = evdev.evaluate("tap", frames_of(tap_stream() + suffix), RANGE)
            self.assertEqual(result["status"], "fail", result)

    def test_interruption_inside_unload_marks_restore_due(self):
        spec = importlib.util.spec_from_file_location("qualification_boundary", TOOL)
        assert spec is not None and spec.loader is not None
        runner = importlib.util.module_from_spec(spec); spec.loader.exec_module(runner)
        state = {"origin": {"params": PARAMS.copy(), "driver": runner.DRIVER, "changed": False}}
        with mock.patch.object(runner, "modprobe", side_effect=KeyboardInterrupt):
            with self.assertRaises(KeyboardInterrupt):
                runner.transition_to_bridge(Path("/not-a-real-panel"), [], state)
        self.assertTrue(state["origin"]["changed"], "finally must owe a restore even inside unload")

    def test_numeric_knob_changes_the_verdict(self):
        parsed = frames_of(tap_stream())
        self.assertEqual(evdev.evaluate("tap", parsed, RANGE, {"tap_max_ms": 1000})["status"], "pass")
        verdict = evdev.evaluate("tap", parsed, RANGE, {"tap_max_ms": 50})
        self.assertEqual(verdict["status"], "fail")
        self.assertIn("press lasted", verdict["reason"])
        self.assertEqual(verdict["knobs"]["tap_max_ms"], 50)
        strict = evdev.evaluate("motion", frames_of(motion_stream()), RANGE,
                                {"min_span_pct": 500.0})
        self.assertEqual(strict["status"], "fail")


# ── half 2: the guided run, end to end, on a fake machine ──────────────────

COLLECT = '''#!/bin/bash
out=""
while [ $# -gt 0 ]; do case "$1" in --output) out="$2"; shift 2;; *) shift;; esac; done
mkdir -p "$out/kernel"; printf 'fake dmesg tail\\n' > "$out/kernel/dmesg.txt"
'''
MODPROBE = '''#!/bin/bash
H="$SL4A_QUALIFY_HOST_ROOT"
printf '%s\\n' "$*" >> "$H/modprobe.log"
if [ -n "${FAKE_FAIL_PATTERN:-}" ] && [[ "$*" == *"$FAKE_FAIL_PATTERN"* ]]; then
	echo "modprobe: FATAL: sandbox refusal ($*)" >&2
	exit 1
fi
M="$H/sys/module/sl4a_spi_hid"
P="$H/sys/bus/spi/devices/spi-MSHW0231:00"
if [ "${1:-}" = "-r" ]; then
	if [ -n "${FAKE_BUSY_UNLOAD:-}" ] && [ ! -f "$H/.unload-tried" ]; then
		touch "$H/.unload-tried"
		echo "modprobe: FATAL: Module sl4a_spi_hid is in use." >&2
		exit 1
	fi
	rm -rf "$M" "$H/sys/class/input/event11" \\
	       "$H/sys/bus/spi/devices/spi-MSHW0231:00/input/input11" "$P/driver"
	exit 0
fi
mkdir -p "$M/parameters"
for pair in raw_mode=N std_raw_transition=0 raw_input_beta=N skip_getfeat=N sl4a_debug_level=0 "win_host_addr=(null)"; do
	printf '%s\\n' "${pair#*=}" > "$M/parameters/${pair%%=*}"
done
shift
for arg in "$@"; do
	printf '%s\\n' "${arg#*=}" > "$M/parameters/${arg%%=*}"
done
mkdir -p "$H/sys/bus/spi/drivers/sl4a_spi_hid"
ln -sfn "$H/sys/bus/spi/drivers/sl4a_spi_hid" "$P/driver"
if [ "$(cat "$M/parameters/std_raw_transition")" = 1 ]; then
	cp -a "$H/.pristine/." "$H/"
fi
exit 0
'''
STUBS = {
    "uname": '#!/bin/bash\n[ "$1" = -r ] && echo ' + KERNEL + ' || echo "Linux sandbox ' + KERNEL + '"\n',
    "dkms": '''#!/bin/bash
k=sandbox
while [ $# -gt 0 ]; do case "$1" in -k) k="$2";; esac; shift; done
printf 'sl4a-touch/%s, %s, x86_64: installed\\n' \\
  "$(cat "$SL4A_QUALIFY_HOST_ROOT/var/lib/sl4a-touch/dkms-version")" "$k"
''',
    "modinfo": '''#!/bin/bash
f=""
[ "$1" = -F ] && { f="$2"; shift 2; }
case "$f" in
  vermagic) echo "$(uname -r) SMP preempt mod_unload";;
  srcversion) echo fresh1234;;
  signer) echo "sandbox signer";;
  *) echo "$f";;
esac
''',
    "mokutil": '#!/bin/bash\n[ "$1" = --list-enrolled ] && echo "Subject: sandbox" || echo "SecureBoot disabled"\n',
    "systemctl": '#!/bin/bash\n[ "$1" = is-active ] && echo active || echo enabled\n',
}


def write(path, text, mode=0o644):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)
    path.chmod(mode)


def link(path, target):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.symlink_to(target)


def git(repo, *args):
    done = subprocess.run(["git", "-c", "user.email=t@e", "-c", "user.name=t", *args],
                          cwd=repo, capture_output=True, text=True)
    assert done.returncode == 0, done.stderr


def build_world(tmp):
    """Fake repo (real git) + fake host tree + stub helpers + evdev FIFOs."""
    repo, host, stubs = tmp / "repo", tmp / "host", tmp / "bin"
    (repo / "tools/hardware_evidence").mkdir(parents=True)
    for name in ("qualify-touch.py", "touch_evdev.py"):
        shutil.copy2(ROOT / "tools" / name, repo / "tools" / name)
    write(repo / "VERSION", "1.7.1\n")
    write(repo / "tools/hardware_evidence/collect.sh", COLLECT)
    write(repo / "tools/hardware_evidence/capture_linux_trace_bundle.sh", "#!/bin/bash\nexit 0\n")
    git(repo, "init", "-q"); git(repo, "add", "-A"); git(repo, "commit", "-qm", "sandbox")
    head = subprocess.run(["git", "-C", str(repo), "rev-parse", "HEAD"],
                          capture_output=True, text=True).stdout.strip()

    for name, stub in STUBS.items():
        write(stubs / name, stub, 0o755)
    write(stubs / "modprobe", MODPROBE, 0o755)

    panel = host / "sys/bus/spi/devices/spi-MSHW0231:00"
    acpi = host / "sys/bus/acpi/devices/MSHW0231:00"
    acpi.mkdir(parents=True)
    link(acpi / "physical_node", panel)
    (host / "sys/bus/acpi/devices/AMDI0060:00").mkdir(parents=True)
    # a bound device needs the driver directory the symlink points at, exactly
    # as sysfs always has it
    (host / "sys/bus/platform/drivers/sl4a_spi_amd_v2_multi").mkdir(parents=True)
    link(host / "sys/bus/platform/devices/AMDI0060:00/driver",
         host / "sys/bus/platform/drivers/sl4a_spi_amd_v2_multi")
    (host / "sys/bus/spi/drivers/sl4a_spi_hid").mkdir(parents=True)
    link(panel / "driver", host / "sys/bus/spi/drivers/sl4a_spi_hid")
    for module in ("sl4a-spi-amd", "sl4a-spi-hid"):
        write(host / "sys/module" / module.replace("-", "_") / "srcversion", "fresh1234\n")
    for name, value in PARAMS.items():
        write(host / "sys/module/sl4a_spi_hid/parameters" / name, value + "\n")
    for uid in ("MSHW0360", "MSHW0084"):  # unrelated Surface nodes: never a candidate
        (host / "sys/bus/acpi/devices" / f"{uid}:00").mkdir(parents=True)

    # standard HID single-touch node: direct, under the panel, not named after the driver
    standard = panel / "input/input10"
    write(standard / "name", "spi 045E:0C19\n")
    write(standard / "properties", "2\n")
    write(standard / "capabilities/abs", hex((1 << evdev.ABS_X) | (1 << evdev.ABS_Y)) + "\n")
    write(standard / "capabilities/key", "400 0 0 0 0 0\n")
    write(host / "sys/class/input/event10/dev", "13:74\n")
    link(host / "sys/class/input/event10/device", standard)
    # bridge MT node, kept pristine and (re)installed by the modprobe stub
    pristine = host / ".pristine"
    bridge = pristine / "sys/bus/spi/devices/spi-MSHW0231:00/input/input11"
    write(bridge / "name", "MSHW0231 Touchscreen\n")
    write(bridge / "properties", "2\n")
    write(bridge / "capabilities/abs", MT_ABS + "\n")
    write(bridge / "capabilities/key", "400 0 0 0 0 0\n")
    write(pristine / "sys/class/input/event11/dev", "13:75\n")
    link(pristine / "sys/class/input/event11/device", host / "sys/bus/spi/devices/spi-MSHW0231:00/input/input11")

    write(host / "etc/modprobe.d/sl4a-spi-hid.conf",
          "# SL4A_TouchScreen\noptions sl4a_spi_hid raw_mode=0\n")
    write(host / "etc/systemd/system/sl4a-touch-activate.service", "[Service]\n")
    write(host / "var/lib/sl4a-touch/installed-head", head + "\n")
    write(host / "var/lib/sl4a-touch/dkms-version", "1.7.1\n")
    write(host / "proc/uptime", "12.5 9.0\n")
    write(host / "proc/sys/kernel/random/boot_id", "sandbox-boot-id\n")

    dev = host / "dev/input"
    dev.mkdir(parents=True)
    for node in ("event10", "event11"):
        os.mkfifo(dev / node)
    return {"root": tmp, "repo": repo, "host": host, "bin": stubs, "dev": dev}


def segments(mode="good"):
    """What each gesture window will be fed, keyed by (event node, gesture).

    "good" passes, "bad_standard" fails a standard window (the run must then
    stop BEFORE the reload), "bad_bridge" passes every standard window and fails
    the bridge set (the run must reload and then restore).
    """
    standard = {("event10", "tap"): tap_stream(),
                ("event10", "motion"): motion_stream(),
                ("event10", "edges"): edges_stream()}
    bridge = {("event11", "tap"): point(0.0, 16000, 16000, ident=41) + lift(0.18, 41),
              ("event11", "motion"): b_motion_stream(),
              ("event11", "edges"): b_edges_stream(),
              ("event11", "five_hold"): five_stream(),
              ("event11", "pinch"): pinch_stream(),
              ("event11", "recontact"): recontact_stream()}
    if mode == "bad_standard":
        # a dangling SYN_DROPPED and a finger that never comes up
        standard[("event10", "tap")] = tap_stream() + dropped(0.3)
        standard[("event10", "motion")] = stream(
            *[single(i * 0.03, 3000 + i * 2200, 3000 + i * 2100) for i in range(12)])
    elif mode == "bad_bridge":
        bridge[("event11", "five_hold")] = five_stream(staggered=True)
        bridge[("event11", "recontact")] = recontact_stream(new_tid=7)
        # three disconnected two-finger taps in one window: not one pinch
        taps = []
        for when, span in ((0.05, 6000), (0.60, 12000), (1.20, 3000)):
            taps.append(stream(slot(when, 0), tid(when, int(when * 100)), px(when, 16000 - span),
                               py(when, 16000), slot(when, 1), tid(when, int(when * 100) + 1),
                               px(when, 16000 + span), py(when, 16000), syn(when)))
            taps.append(stream(slot(when + 0.1, 0), tid(when + 0.1, -1),
                               slot(when + 0.1, 1), tid(when + 0.1, -1), syn(when + 0.1)))
        bridge[("event11", "pinch")] = stream(*taps)
    return {**standard, **bridge}


class Feeder(threading.Thread):
    """Writes each window's scripted stream into the FIFO the run just selected.

    The tool prints `device=eventN` inside every window marker and flushes, so
    the feeder switches FIFO exactly when the run switches device, and never
    writes to a node the run has already closed.
    """

    def __init__(self, pipe, dev, script, on_line=None):
        super().__init__(daemon=True)
        self.pipe, self.dev, self.script = pipe, dev, script
        self.on_line, self.lines = on_line, queue.Queue()
        self.fd, self.node, self.problem = None, None, ""

    def _open(self, node):
        deadline = time.monotonic() + 30
        while time.monotonic() < deadline:
            try:
                return os.open(str(self.dev / node), os.O_WRONLY | os.O_NONBLOCK)
            except OSError:
                time.sleep(0.02)
        self.problem = f"no reader for {node}"
        return None

    def run(self):
        for line in self.pipe:
            self.lines.put(line.rstrip("\n"))
            found = re.search(r"device=(event\d+)", line)
            if found and found.group(1) != self.node:
                if self.fd is not None:
                    try:
                        os.close(self.fd)
                    except OSError:
                        pass
                self.node = found.group(1)
                self.fd = self._open(self.node)
            window = re.search(r"window=(\S+)", line)
            if window and self.fd is not None:
                payload = self.script.get((self.node, window.group(1)))
                if payload:
                    try:
                        os.write(self.fd, payload)
                    except OSError as error:
                        self.problem = f"write to {self.node} failed: {error}"
            # Only after the FIFO is open and the window's bytes are written:
            # an interrupt triggered here lands inside the run's read window.
            if self.on_line:
                self.on_line(line)
        if self.fd is not None:
            try:
                os.close(self.fd)
            except OSError:
                pass


class GuidedSandbox(unittest.TestCase):
    """The guided run against a fake machine: verdicts first, safety always."""

    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp(prefix="sl4a-guided-"))
        self.addCleanup(shutil.rmtree, self.tmp, True)
        self.world = build_world(self.tmp)
        self.out = self.tmp / "bundle"

    def run_tool(self, *cli, stdin="BRIDGE\n", mode="good", env=None, interrupt=None):
        world = self.world
        full = dict(os.environ, PATH=f"{world['bin']}:{os.environ['PATH']}",
                    SL4A_QUALIFY_HOST_ROOT=str(world["host"]),
                    SL4A_QUALIFY_DEV_ROOT=str(world["dev"]), **(env or {}))
        process = subprocess.Popen(
            [sys.executable, str(world["repo"] / "tools/qualify-touch.py"),
             "--guided", "--output", str(self.out), "--gesture-timeout", "3",
             "--idle-gap", "0.3", *cli],
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            text=True, bufsize=1, env=full)
        seen = []

        def on_line(line):
            seen.append(line)
            if interrupt and interrupt in line and process.poll() is None:
                process.send_signal(signal.SIGTERM)

        feeder = Feeder(process.stdout, world["dev"], segments(mode), on_line)
        feeder.start()
        try:
            process.stdin.write(stdin)
            process.stdin.flush()
        except BrokenPipeError:
            pass
        try:
            code = process.wait(timeout=180)
        except subprocess.TimeoutExpired:
            process.kill()
            self.fail("guided run did not finish:\n" + "\n".join(seen[-40:]))
        feeder.join(timeout=30)
        for pipe in (process.stdout, process.stdin):
            try:
                pipe.close()
            except OSError:
                pass
        return code, "\n".join(seen), feeder.problem

    def status(self):
        return json.loads((self.out / "status.json").read_text())

    def results(self):
        return json.loads((self.out / "machine-results.json").read_text())

    def modprobe_log(self):
        log = self.world["host"] / "modprobe.log"
        return log.read_text().splitlines() if log.exists() else []

    def live_param(self, name):
        return (self.world["host"] / "sys/module/sl4a_spi_hid/parameters" / name).read_text().strip()

    def assert_no_unsafe_effects(self):
        log = "\n".join(self.modprobe_log())
        for forbidden in ("--force", "rmmod", "-f ", "reboot", "suspend", "systemctl",
                          "mokutil --import", "dkms install", "insmod"):
            self.assertNotIn(forbidden, log, f"unsafe host effect in the modprobe log: {forbidden}")
        self.assertIn("raw_mode=0", (self.world["host"] / "etc/modprobe.d/sl4a-spi-hid.conf").read_text(),
                      "the persistent config was rewritten")
        self.assertTrue((self.world["host"] / "etc/modprobe.d/sl4a-spi-hid.conf").read_text()
                        .startswith("# SL4A_TouchScreen\noptions sl4a_spi_hid raw_mode=0\n"))

    def assert_restored(self):
        for name, value in PARAMS.items():
            self.assertEqual(self.live_param(name), value, f"{name} not restored")
        driver = (self.world["host"] / "sys/bus/spi/devices/spi-MSHW0231:00/driver").resolve().name
        self.assertEqual(driver, "sl4a_spi_hid", "binding not restored")
        log = self.modprobe_log()
        self.assertIn("-r sl4a-spi-hid", log, "no unload before the reload")
        self.assertTrue(any(line.startswith("sl4a-spi-hid ") and "std_raw_transition=1" in line
                            for line in log), "the bridge profile was never loaded")
        self.assertTrue(any(line.startswith("sl4a-spi-hid ") and "std_raw_transition=0" in line
                            and "raw_mode=N" in line for line in log),
                        "the original profile was never reloaded")

    def test_success_is_scoped_and_restores_everything(self):
        code, output, problem = self.run_tool()
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 0, output)
        state, results = self.status(), self.results()
        self.assertEqual(state["status"], "guided-qualified", state)
        self.assertTrue(state["restore"]["ok"], state["restore"])
        self.assertEqual(len(results["gestures"]), 9, "not every gesture window ran")
        self.assertTrue(all(g["status"] == "pass" for g in results["gestures"]),
                        [g for g in results["gestures"] if g["status"] != "pass"])
        verdicts = {item["id"]: item["result"] for item in results["verdicts"]}
        self.assertEqual(verdicts["bridge_evdev_gestures"], "PASS")
        self.assertEqual(verdicts["standard_evdev_gestures"], "PASS")
        for scope in ("coldboot_initial", "resume_initial", "secureboot_enrollment"):
            self.assertEqual(verdicts[scope], "NOT_TESTED", f"{scope} claimed without evidence")
        self.assertEqual(verdicts["full_pr_pass"], "NOT_APPLICABLE", "a full PR pass was claimed")
        # every PR in the branch list has its OWN entry, honestly scoped
        self.assertEqual(verdicts["pr87_bridge_standard_transport"], "PASS",
                         "the ONE measured bridge subtest")
        for pr in ("pr88_close_contact_coalescing", "pr89_dkms_signing_identity",
                   "pr91_fixed_header_bounds"):
            self.assertEqual(verdicts[pr], "NOT_TESTED", f"{pr} claimed without evidence")
        self.assertEqual(verdicts["pr90_evidence_workflow"], "PASS",
                         "collect.sh produced a bundle in this run")
        self.assertFalse([k for k in verdicts if k.startswith("pr88_bridge")],
                         "the old wrong pr88/bridge mapping is still there")
        self.assertIn("NOT a full PR pass", (self.out / "verdicts.txt").read_text())
        self.assert_restored()
        self.assert_no_unsafe_effects()
        # raw selected-touch-only evidence: exactly the bytes fed, per window
        raw = sorted(p.name for p in (self.out / "raw").iterdir())
        self.assertEqual(raw, ["bridge-edges.evdev", "bridge-five_hold.evdev",
                               "bridge-motion.evdev", "bridge-pinch.evdev",
                               "bridge-recontact.evdev", "bridge-tap.evdev",
                               "standard-edges.evdev", "standard-motion.evdev",
                               "standard-tap.evdev"])
        self.assertEqual((self.out / "raw/standard-tap.evdev").read_bytes(), tap_stream())
        self.assertEqual((self.out / "raw/bridge-five_hold.evdev").read_bytes(), five_stream())
        self.assertIn("secure_boot_state", (self.out / "secureboot.txt").read_text())
        self.assertFalse((self.out / "recovery.txt").exists(), "recovery text without a failure")
        for name in ("status.json", "machine-results.json", "verdicts.txt", "transition.txt"):
            self.assertTrue((self.out / name).is_file(), name)
        for line in (self.out / "checksums.txt").read_text().splitlines():
            digest, name = line.split("  ", 1)
            self.assertEqual(__import__("hashlib").sha256((self.out / name).read_bytes()).hexdigest(),
                             digest, f"bad checksum for {name}")

    def test_standard_failure_stops_before_the_reload(self):
        # no bridge reload for a profile that already failed: tearing a healthy
        # touchscreen down for a verdict that cannot improve is not a trade
        code, output, problem = self.run_tool(mode="bad_standard")
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 4, output)
        state, results = self.status(), self.results()
        self.assertEqual(state["status"], "gesture-fail")
        self.assertIn("NOT attempted", state["detail"])
        self.assertFalse((self.world["host"] / "modprobe.log").exists(),
                         "the driver was reloaded anyway")
        reasons = {g["profile"] + "/" + g["name"]: (g["status"], g["reason"])
                   for g in results["gestures"]}
        self.assertEqual(reasons["standard/tap"][0], "fail")
        self.assertIn("syn_dropped", reasons["standard/tap"][1])
        self.assertEqual(reasons["standard/motion"][0], "fail")
        self.assertIn("never released", reasons["standard/motion"][1])
        self.assertEqual(reasons["standard/edges"][0], "pass")
        self.assertFalse([k for k in reasons if k.startswith("bridge/")],
                         "bridge windows ran without the bridge profile")
        verdicts = {item["id"]: item["result"] for item in results["verdicts"]}
        self.assertEqual(verdicts["bridge_evdev_gestures"], "NOT_TESTED")
        self.assertEqual(verdicts["pr87_bridge_standard_transport"], "NOT_TESTED")
        self.assertFalse((self.out / "recovery.txt").exists())
        self.assert_no_unsafe_effects()

    def test_failing_bridge_gestures_are_reported_not_rounded_up(self):
        code, output, problem = self.run_tool(mode="bad_bridge")
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 4, output)
        state, results = self.status(), self.results()
        self.assertEqual(state["status"], "gesture-fail")
        self.assertTrue(state["restore"]["ok"], "an evaluated run must still restore")
        reasons = {g["profile"] + "/" + g["name"]: (g["status"], g["reason"])
                   for g in results["gestures"]}
        self.assertEqual(reasons["standard/tap"][0], "pass")
        self.assertEqual(reasons["bridge/five_hold"][0], "fail")
        self.assertIn("staggered", reasons["bridge/five_hold"][1])
        self.assertEqual(reasons["bridge/recontact"][0], "fail")
        self.assertIn("reused", reasons["bridge/recontact"][1])
        self.assertEqual(reasons["bridge/pinch"][0], "fail")
        self.assertIn("changed identity", reasons["bridge/pinch"][1])
        verdicts = {item["id"]: item["result"] for item in results["verdicts"]}
        self.assertEqual(verdicts["bridge_evdev_gestures"], "FAIL")
        self.assertEqual(verdicts["pr87_bridge_standard_transport"], "FAIL")
        self.assert_restored()

    def test_failed_transition_restores_and_explains(self):
        code, output, problem = self.run_tool(env={"FAKE_FAIL_PATTERN": "std_raw_transition=1"})
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 3, output)
        state = self.status()
        self.assertEqual(state["status"], "transition-failed")
        self.assertIn("bridge parameters failed", state["detail"])
        self.assertTrue(state["restore"]["ok"], state["restore"])
        self.assert_restored()
        self.assert_no_unsafe_effects()
        transition = (self.out / "transition.txt").read_text()
        self.assertIn("# restore", transition)
        self.assertIn(f"all {len(PARAMS)} parameters match the original load", transition)
        self.assertFalse((self.out / "raw/bridge-tap.evdev").exists(),
                         "a bridge gesture ran without the bridge profile")

    def test_failed_restore_is_loud_and_leaves_recovery_instructions(self):
        code, output, problem = self.run_tool(env={"FAKE_FAIL_PATTERN": "std_raw_transition=0"})
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 7, output)
        state = self.status()
        self.assertEqual(state["status"], "restore-failed")
        self.assertFalse(state["restore"]["ok"])
        recovery = (self.out / "recovery.txt").read_text()
        self.assertIn("RECOVERY REQUIRED", recovery)
        self.assertIn("modprobe -r sl4a-spi-hid", recovery)
        self.assertIn("modprobe sl4a-spi-hid", recovery)
        self.assertIn("raw_mode=N", recovery)
        self.assertIn("std_raw_transition=0", recovery)
        self.assertIn("NO reboot was performed", recovery)
        self.assertIn("RECOVERY REQUIRED", output, "the failure was not printed loudly")
        self.assert_no_unsafe_effects()

    def test_busy_unload_changes_nothing_and_is_not_retried(self):
        # a refused unload must not become "try again in the restore": that
        # would tear down a healthy driver for nothing
        code, output, problem = self.run_tool(env={"FAKE_BUSY_UNLOAD": "1"})
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 3, output)
        state = self.status()
        self.assertEqual(state["status"], "transition-failed")
        self.assertIn("module busy", state["detail"])
        self.assertTrue(state["restore"]["ok"], state["restore"])
        self.assertEqual(self.modprobe_log(), ["-r sl4a-spi-hid"],
                         "the driver was reloaded after a refused unload")
        for name, value in PARAMS.items():
            self.assertEqual(self.live_param(name), value)
        self.assertFalse((self.out / "recovery.txt").exists(),
                         "a healthy machine was reported as needing recovery")

    def test_sigterm_during_bridge_restores_the_driver(self):
        code, output, problem = self.run_tool(interrupt="[bridge] window=tap")
        self.assertEqual(problem, "", problem)
        self.assertEqual(code, 130, output)
        state = self.status()
        self.assertEqual(state["status"], "interrupted", state)
        self.assertTrue(state["restore"]["ok"], state["restore"])
        self.assert_restored()
        self.assert_no_unsafe_effects()
        self.assertFalse((self.out / "verdicts.txt").read_text().startswith("FAIL"),
                         "an interrupted run must not read as a gesture verdict")

    def test_declined_consent_touches_nothing(self):
        code, output, problem = self.run_tool(stdin="no\n")
        self.assertEqual(code, 5, output)
        self.assertEqual(self.status()["status"], "consent-declined")
        log = self.modprobe_log() if (self.world["host"] / "modprobe.log").exists() else []
        self.assertEqual(log, [], f"the driver was reloaded without consent: {log}")
        self.assertEqual(self.live_param("raw_mode"), "N")


if __name__ == "__main__":
    unittest.main(verbosity=2)
