#!/usr/bin/env python3
"""Decode Linux evdev records and score one touchscreen gesture. Stdlib only.

This module never opens a device, never prompts and never writes anything: the
guided runner (tools/qualify-touch.py) owns the file descriptor, the bounded
window and the artefacts, and hands the bytes here. Keeping the decoder and the
numeric checks in one importable place is what makes every rule below testable
without a touchscreen and without root.

What a PASS here means, and what it does not: these are observations on the
raw evdev stream of ONE selected touch node — contacts, slots, tracking ids,
SYN_REPORT boundaries, ABS ranges. They are not evidence that a desktop, a
compositor or an application accepted a gesture (that is policy above evdev),
and they are not evidence about cold boot, resume or Secure Boot enrollment.
The runner reports them under that scope.
"""
import fcntl
import math
import os
import select
import struct
import time

EV_SYN, EV_KEY, EV_ABS = 0x00, 0x01, 0x03
SYN_REPORT, SYN_DROPPED = 0x00, 0x03
BTN_TOUCH = 0x14A
ABS_X, ABS_Y = 0x00, 0x01
ABS_MT_SLOT, ABS_MT_POSITION_X, ABS_MT_POSITION_Y = 0x2F, 0x35, 0x36
ABS_MT_TRACKING_ID = 0x39
INPUT_PROP_DIRECT = 1  # bit index in the input device's propbit
# EVIOCGABS(abs) = _IOR('E', 0x40 + abs, struct input_absinfo) = dir 2 (read),
# size 24, type 'E' (0x45), nr 0x40 + abs. Written out because a wrong number
# here is an ENOTTY that silently degrades every range to the fallback.
def ev_iocgabs(code):
    return (2 << 30) | (24 << 16) | (ord("E") << 8) | (0x40 + code)


# The other three requests this file needs, written out for the same reason.
def ev_iocgkey(length):  # EVIOCGKEY(len) = _IOC(_IOC_READ, 'E', 0x18, len)
    return (2 << 30) | (length << 16) | (ord("E") << 8) | 0x18


def ev_iocgmtslots(length):  # EVIOCGMTSLOTS(len) = _IOC(_IOC_READ, 'E', 0x0A, len)
    return (2 << 30) | (length << 16) | (ord("E") << 8) | 0x0A


def ev_iocgname(length):  # EVIOCGNAME(len) = _IOC(_IOC_READ, 'E', 0x06, len)
    return (2 << 30) | (length << 16) | (ord("E") << 8) | 0x06


def ev_iocgrab(on=True):  # EVIOCGRAB = _IOW('E', 0x90, int)
    return (1 << 30) | (4 << 16) | (ord("E") << 8) | (0x90 if on else 0x91)


# (KEY_MAX + 1) bits: EVIOCGKEY wants a whole number of bytes, and BTN_TOUCH is
# inside the first 96. Sending a bigger buffer than the kernel's keybit is fine;
# sending a smaller one would silently hide BTN_TOUCH.
KEY_BYTES = 96
NAME_BYTES = 256
EV_ABS_BIT = {"x": 1 << ABS_X, "y": 1 << ABS_Y, "slot": 1 << ABS_MT_SLOT,
              "mtx": 1 << ABS_MT_POSITION_X, "mty": 1 << ABS_MT_POSITION_Y,
              "tid": 1 << ABS_MT_TRACKING_ID}

# One `struct input_event`: timeval + type + code + value. On 64-bit Linux that
# is 24 bytes, on 32-bit 16 — calcsize keeps the same literal honest on both.
_FMT = "@llHHi"
RECORD_SIZE = struct.calcsize(_FMT)

# Numeric thresholds, overridable one by one from the command line
# (--knob name=value) so a panel can be tuned without editing code. Percents
# are fractions of the ABS range the device itself reports, never pixels.
KNOBS = {
    "tap_max_ms": 400.0,        # longest press that still counts as a tap
    "tap_move_pct": 5.0,        # allowed finger travel during a tap
    "min_span_pct": 30.0,       # minimum travel for the single-motion gesture
    "min_frames": 5.0,          # SYN_REPORT frames the motion must span
    "max_gap_ms": 250.0,        # longest silence inside a contact (continuity)
    "edge_pct": 3.0,            # how close to min/max counts as "reached"
    "fingers": 5.0,             # concurrent contacts the multi-finger hold needs
    "release_skew_frames": 1.0, # frames with an intermediate count on release
    "span_out_pct": 25.0,       # pinch: minimum spread relative to start
    "span_in_pct": 20.0,        # pinch: minimum squeeze relative to start
    "recontact_ms": 800.0,      # lift -> new contact window for recontact
    "recontact_pct": 8.0,       # allowed positional drift on recontact
    "hold_min_ms": 300.0,       # shortest sustained five-finger hold
}

# Every knob needs a floor and a ceiling: a NaN, an infinity or a nonsense
# percentage would otherwise make a check pass (or fail) for arithmetic
# reasons instead of because of the panel.
KNOB_BOUNDS = {
    "tap_max_ms": (1.0, 60000.0),
    "tap_move_pct": (0.0, 100.0),
    "min_span_pct": (0.0, 1000.0),
    "min_frames": (1.0, 10000.0),
    "max_gap_ms": (1.0, 600000.0),
    "edge_pct": (0.0, 49.0),
    "fingers": (1.0, 10.0),
    "release_skew_frames": (0.0, 1000.0),
    "span_out_pct": (0.0, 100000.0),
    "span_in_pct": (0.0, 100000.0),
    "recontact_ms": (1.0, 600000.0),
    "recontact_pct": (0.0, 100.0),
    "hold_min_ms": (1.0, 600000.0),
}


def decode(data):
    """Split a byte stream into (sec, usec, type, code, value) records.

    Returns (records, leftover) where leftover is a trailing fragment: a real
    evdev read can end mid-record, and silently dropping or padding it would
    turn an unreadable window into a clean one.
    """
    count = len(data) // RECORD_SIZE
    if not count:
        return [], bytes(data)
    records = [struct.unpack_from(_FMT, data, i * RECORD_SIZE) for i in range(count)]
    return records, bytes(data[count * RECORD_SIZE:])


def _contact():
    return {"tid": None, "x": None, "y": None}


def protocol_kind(abs_bits):
    """The evdev protocol a node's ABS capability bitmap advertises.

    "B" = ABS_MT_SLOT (slots + tracking ids, the driver's MT device),
    "A" = MT positions/ids without slots (protocol A: a SYN_MT_REPORT packet
    per contact — this decoder does not score it and says so),
    "single" = BTN_TOUCH + ABS_X/ABS_Y (the standard HID single-touch node),
    "none" = neither, so not a touch surface this tool can score.
    """
    if abs_bits & EV_ABS_BIT["slot"]:
        return "B"
    if abs_bits & (EV_ABS_BIT["mtx"] | EV_ABS_BIT["mty"] | EV_ABS_BIT["tid"]):
        return "A"
    if abs_bits & EV_ABS_BIT["x"] and abs_bits & EV_ABS_BIT["y"]:
        return "single"
    return "none"


def _contacts(kind, slots, btn, sx, sy):
    if kind is None:  # replay without capabilities: the slots decide
        kind = "B" if slots else "single"
    if kind == "B":  # Type B: a slot holds a contact until TRACKING_ID -1
        return [dict(slot=s, **v) for s, v in sorted(slots.items())]
    if kind == "single":  # plain single-touch HID: BTN_TOUCH + ABS_X/ABS_Y
        if btn and sx is not None and sy is not None:
            return [{"slot": 0, "tid": None, "x": sx, "y": sy}]
    return []


def frames(records, kind=None):
    """Group records into SYN_REPORT-bounded frames.

    `kind` is the protocol the node's OWN capabilities advertise ("B" or
    "single"), passed in by the caller: a Type B touchscreen also emits
    ABS_X/ABS_Y and BTN_TOUCH, so guessing from the stream makes the same bytes
    mean different things depending on who is holding the node. kind=None keeps
    a replay of a bare bundle working, and a Type A node is rejected by
    `evaluate` instead of being half-decoded.

    Two shapes end a window without a closing SYN_REPORT or with a protocol the
    caller did not ask for; both are flagged on a frame instead of being
    silently dropped, because either one means the stream cannot be scored.
    """
    mode, mismatch = kind if kind in ("B", "single") else None, ""
    out, slot, slots = [], 0, {}
    btn, sx, sy = 0, None, None
    dropped, released, last = False, [], (0, 0)
    dirty = False  # a change happened with no SYN_REPORT after it
    for sec, usec, typ, code, val in records:
        last = (sec, usec)
        now = sec + usec / 1e6
        if typ == EV_SYN and code == SYN_DROPPED:
            # The kernel's queue overflowed: cached slot state is unverifiable
            # from here on. Report the drop as its own frame (so a drop at the
            # very END of a window is not swallowed) and throw the state away —
            # a window containing a drop can never be read as a clean gesture.
            out.append({"t": now, "kind": mode or "single", "contacts": [],
                        "released": [], "dropped": True, "btn": bool(btn)})
            dropped, dirty = True, False
            slots.clear(); released.clear(); sx = sy = None; slot = 0
            continue
        if typ == EV_SYN and code == SYN_REPORT:
            out.append({"t": now, "kind": mode or "single",
                        "contacts": _contacts(mode, slots, btn, sx, sy),
                        "released": list(released), "dropped": dropped,
                        "btn": bool(btn)})
            released.clear(); dropped = False; dirty = False
            continue
        if typ == EV_KEY:
            dirty = True
            if code == BTN_TOUCH:
                btn = val
            continue
        if typ != EV_ABS:
            continue
        dirty = True
        if code in (ABS_X, ABS_Y):  # the plain single-touch axes
            if code == ABS_X:
                sx = val
            else:
                sy = val
        elif code == ABS_MT_SLOT:
            if mode is None:
                mode = "B"
            elif mode != "B":
                mismatch = mismatch or ("the node advertises plain single-touch but reported"
                                        " ABS_MT_SLOT")
            slot = val
        elif code == ABS_MT_TRACKING_ID:
            if mode is None:  # tracking ids without a slot: Type B, implicit slot 0
                mode = "B"
            if mode != "B":
                mismatch = mismatch or ("the node advertises plain single-touch but reported"
                                        " ABS_MT_TRACKING_ID")
            elif val == -1:
                gone = slots.pop(slot, None)
                if gone and gone.get("tid") is not None:
                    released.append((slot, gone["tid"]))
            else:
                slots[slot] = _contact()
                slots[slot]["tid"] = val
        elif code in (ABS_MT_POSITION_X, ABS_MT_POSITION_Y):
            axis = "x" if code == ABS_MT_POSITION_X else "y"
            if mode is None:
                mismatch = mismatch or ("MT positions without slots or tracking ids: protocol A"
                                        " (SYN_MT_REPORT packets) is not decoded")
            elif mode == "single":
                mismatch = mismatch or ("the node advertises plain single-touch but reported"
                                        " ABS_MT_POSITION_*")
            elif slot in slots:  # a position without an open slot is not a contact
                slots[slot][axis] = val
    if dirty:
        # The last change never got its SYN_REPORT: a truncated frame.
        out.append({"t": last[0] + last[1] / 1e6, "kind": mode or "single",
                    "contacts": [], "released": [], "dropped": dropped,
                    "btn": bool(btn), "incomplete": True})
    if mismatch:
        out.append({"t": last[0] + last[1] / 1e6, "kind": mode or "single",
                    "contacts": [], "released": [], "dropped": False,
                    "btn": bool(btn), "mismatch": mismatch})
    return out


def read_window(fd, seconds, idle_gap=2.0, kind=None):
    """Read the node for at most `seconds`, ending early once it went quiet AND
    the last report shows no contact left.

    EOF is reported, not hidden: on a real evdev node it never happens (the
    node blocks), so seeing it means the device node went away and every later
    gesture must fail closed instead of scoring an empty window as a pass.
    """
    data, eof, saw = bytearray(), False, False
    deadline = time.monotonic() + seconds
    last = time.monotonic()
    while True:
        left = deadline - time.monotonic()
        if left <= 0:
            break
        try:
            ready = select.select([fd], [], [], min(left, 0.2))[0]
        except (OSError, ValueError):
            eof = True
            break
        now = time.monotonic()
        if ready:
            try:
                chunk = os.read(fd, 65536)
            except BlockingIOError:
                continue
            except OSError:
                eof = True
                break
            if not chunk:
                eof = True
                break
            data += chunk
            saw, last = True, now
            continue
        if saw and idle_gap and now - last >= idle_gap and _settled(bytes(data), kind):
            break
    return bytes(data), eof


def _settled(data, kind=None):
    """True when the stream ends on a report that has no active contact."""
    parsed = frames(decode(data)[0], kind)
    return (bool(parsed) and not parsed[-1]["contacts"] and not parsed[-1]["dropped"]
            and not parsed[-1].get("incomplete") and not parsed[-1].get("mismatch"))


def down_state(fd, kind="B", slots=0):
    """Contacts the kernel still holds DOWN, read from the node itself.

    Draining a fresh file descriptor cannot answer this: while a finger is held
    the kernel sends nothing, so a brand-new fd reads an empty stream both when
    the panel is idle and when a contact is stuck. EVIOCGKEY reports the
    BTN_TOUCH bit as the kernel has it, EVIOCGMTSLOTS reports the live
    ABS_MT_TRACKING_ID of every slot — both on the SAME fd that is being read.
    """
    out = {"supported": False, "btn_touch": None, "tracking_ids": [],
           "slots": slots, "error": ""}
    try:
        keys = fcntl.ioctl(fd, ev_iocgkey(KEY_BYTES), b"\0" * KEY_BYTES)
        out["btn_touch"] = bool(keys[BTN_TOUCH // 8] & (1 << (BTN_TOUCH % 8)))
        out["supported"] = True
    except OSError as error:
        out["error"] = f"EVIOCGKEY: {error}"
    if kind == "B" and slots:
        size = 4 + 4 * slots
        try:
            buf = fcntl.ioctl(fd, ev_iocgmtslots(size),
                              struct.pack("@i", ABS_MT_TRACKING_ID) + b"\0" * 4 * slots)
            out["tracking_ids"] = [tid for tid in struct.unpack_from(f"@{slots}i", buf, 4)
                                   if tid >= 0]
        except (OSError, struct.error) as error:
            out["supported"] = False
            out["error"] += f" EVIOCGMTSLOTS: {error}"
    return out


def device_name(fd):
    """EVIOCGNAME of an open fd, or "" — compared against sysfs on a live host."""
    try:
        return fcntl.ioctl(fd, ev_iocgname(NAME_BYTES),
                           b"\0" * NAME_BYTES).split(b"\0")[0].decode()
    except (OSError, UnicodeDecodeError):
        return ""


def grab(fd, on=True):
    """EVIOCGRAB: while held, the desktop sees no events from this node."""
    fcntl.ioctl(fd, ev_iocgrab(on), struct.pack("@i", 1 if on else 0))


def _width(rng):
    return max(1, rng["x"][1] - rng["x"][0])


def _height(rng):
    return max(1, rng["y"][1] - rng["y"][0])


def _points(frames_):
    """(frame time, track key, x, y) for every reported contact, slot or tid.

    Type B keeps a contact on its slot, so the slot is the stable key; a device
    that recycles slots would otherwise look like an unmoving finger. When a
    tracking id exists it wins, because that is the identity userspace sees.
    """
    out = []
    for frame in frames_:
        for contact in frame["contacts"]:
            if contact.get("x") is None or contact.get("y") is None:
                continue
            key = contact["tid"] if contact.get("tid") is not None else contact["slot"]
            out.append((frame["t"], key, contact["x"], contact["y"]))
    return out


def _tracks(frames_):
    tracks = {}
    for t, key, x, y in _points(frames_):
        tracks.setdefault(key, []).append((t, x, y))
    return tracks


def _counts(frames_):
    return [len(frame["contacts"]) for frame in frames_]


def _span(contact_list):
    """Longest pairwise distance between concurrent contacts (pinch span)."""
    best = 0.0
    for i, a in enumerate(contact_list):
        for b in contact_list[i + 1:]:
            if None in (a.get("x"), a.get("y"), b.get("x"), b.get("y")):
                continue
            best = max(best, math.hypot(a["x"] - b["x"], a["y"] - b["y"]))
    return best


def _key(contact):
    """The identity userspace sees: the tracking id, else the slot."""
    return contact["tid"] if contact.get("tid") is not None else contact["slot"]


def _positioned(contact):
    return contact.get("x") is not None and contact.get("y") is not None


def _in_range(contact, rng):
    return (rng["x"][0] <= contact["x"] <= rng["x"][1]
            and rng["y"][0] <= contact["y"] <= rng["y"][1])


def _sane(frames_, rng):
    """Window-wide validity: no honest PASS over an unreadable stream.

    A truncated frame, a coordinate the device cannot produce, a timestamp that
    goes backwards and a report without a position all mean the numbers below
    would be computed from damage, so they fail the window before any rule runs.
    """
    if any(frame.get("incomplete") for frame in frames_):
        return "truncated_frame: the window ends mid-frame, with no closing SYN_REPORT"
    mismatch = next((frame["mismatch"] for frame in frames_ if frame.get("mismatch")), "")
    if mismatch:
        return f"protocol_mismatch: {mismatch}"
    times = [frame["t"] for frame in frames_]
    if any(later < earlier for earlier, later in zip(times, times[1:])):
        return "timestamps: the report times go backwards inside the window"
    for frame in frames_:
        for contact in frame["contacts"]:
            if not _positioned(contact):
                return "position: a contact was reported without a position"
            if not _in_range(contact, rng):
                return (f"position: a contact at ({contact['x']}, {contact['y']}) is outside the"
                        f" device range x={list(rng['x'])} y={list(rng['y'])}")
    return ""


def closed_reason(frames_, ctx):
    """The rule every gesture shares: no honest PASS without a clean window."""
    if ctx.get("stale_contacts"):
        return "stale_contacts: contacts were already active when the window opened"
    if ctx.get("syn_dropped") or any(frame["dropped"] for frame in frames_):
        return "syn_dropped: the kernel dropped events, slot state is unverifiable"
    if ctx.get("eof"):
        return "eof: the event stream ended inside the window"
    if ctx.get("partial"):
        return "partial_record: a trailing fragment is not a complete event"
    if not frames_:
        return "timeout: no SYN_REPORT frame arrived inside the window"
    return ""


def _released(frames_):
    return not frames_[-1]["contacts"]


def check_tap(frames_, rng, knobs):
    """ONE finger down and up: a two-finger press is not a single tap, and a
    press whose reports carry no position has no location to score."""
    counts = _counts(frames_)
    first = next((i for i, n in enumerate(counts) if n), None)
    if first is None:
        return False, "no contact was reported", {"peak": 0}
    last = next((i for i in range(first, len(counts)) if not counts[i]), None)
    peak = max(counts)
    if last is None:
        return False, "contact never released (persistence)", {"peak": peak}
    duration = (frames_[last]["t"] - frames_[first]["t"]) * 1000.0
    span = frames_[first:last]  # the reports that still carry the contact
    press = _points(span)
    move = 0.0
    if press:
        x0, y0 = press[0][2], press[0][3]
        move = max(math.hypot((x - x0) / _width(rng), (y - y0) / _height(rng)) * 100.0
                   for _, _, x, y in press)
    # A press at frame 0 has no preceding frame: count it as the first cycle.
    cycles = sum(1 for i, n in enumerate(counts) if n and (i == 0 or not counts[i - 1]))
    metrics = {"peak": peak, "duration_ms": round(duration, 1),
               "move_pct": round(move, 2), "presses": cycles, "positioned": len(press)}
    if peak != 1:
        return False, f"a tap is one finger: {peak} contacts were down together", metrics
    if len({_key(contact) for frame in span for contact in frame["contacts"]}) != 1:
        return False, "the contact identity changed inside the press: not one tap", metrics
    if cycles != 1:
        return False, f"expected exactly 1 press/release cycle, saw {cycles}", metrics
    if len(press) != len(span):
        return False, "a press report carried no position: the tap has no location", metrics
    if duration > knobs["tap_max_ms"]:
        return False, f"press lasted {duration:.0f} ms (> {knobs['tap_max_ms']:.0f})", metrics
    if move > knobs["tap_move_pct"]:
        return False, f"finger moved {move:.1f}% of range (> {knobs['tap_move_pct']:.1f}%)", metrics
    if not _released(frames_):
        return False, "contact still active at the end of the window", metrics
    return True, "", metrics


def check_motion(frames_, rng, knobs):
    """ONE uninterrupted contact lifecycle that travels far enough.

    A drag whose tracking id is swapped while the finger stays down is not one
    continuous contact (userspace would see the finger teleport or die), and a
    second press inside the same window means the window captured more than one
    gesture.
    """
    counts = _counts(frames_)
    first = next((i for i, n in enumerate(counts) if n), None)
    if first is None:
        return False, "no contact was reported", {"tracks": 0}
    peak = max(counts)
    last = next((i for i in range(first, len(counts)) if not counts[i]), None)
    cycles = sum(1 for i, n in enumerate(counts) if n and (i == 0 or not counts[i - 1]))
    if last is None:
        return False, "contact never released (persistence)", {"tracks": peak}
    span = frames_[first:last]  # the reports that still carry the contact
    keys = sorted({_key(contact) for frame in span for contact in frame["contacts"]}, key=str)
    points = [(frame["t"], contact["x"], contact["y"])
              for frame in span for contact in frame["contacts"]
              if contact.get("x") is not None and contact.get("y") is not None]
    metrics = {"track": keys[0] if len(keys) == 1 else keys, "frames": len(points),
               "tracks": len(keys), "cycles": cycles}
    if peak != 1:
        return False, f"a drag is one finger: {peak} contacts were down together", metrics
    if cycles != 1:
        return False, f"expected exactly 1 uninterrupted contact lifecycle, saw {cycles}", metrics
    if len(keys) != 1:
        return False, ("the contact identity changed mid-drag: a tracking id was replaced"
                       " without a release"), metrics
    if len(points) != len(span):
        return False, "a drag report carried no position", metrics
    xs = [p[1] for p in points]
    ys = [p[2] for p in points]
    travel = math.hypot((max(xs) - min(xs)) / _width(rng),
                        (max(ys) - min(ys)) / _height(rng)) * 100.0
    gaps = [(b[0] - a[0]) * 1000.0 for a, b in zip(points, points[1:])]
    gap = max(gaps) if gaps else 0.0
    metrics.update(travel_pct=round(travel, 2), max_gap_ms=round(gap, 1))
    if len(points) < knobs["min_frames"]:
        return False, f"contact lasted {len(points)} frames (< {knobs['min_frames']:.0f})", metrics
    if travel < knobs["min_span_pct"]:
        return False, f"travel {travel:.1f}% of range (< {knobs['min_span_pct']:.1f}%)", metrics
    if gap > knobs["max_gap_ms"]:
        return False, f"position gap {gap:.0f} ms (> {knobs['max_gap_ms']:.0f}): not continuous", metrics
    if not _released(frames_):
        return False, "contact still active at the end of the window", metrics
    return True, "", metrics


def check_edges(frames_, rng, knobs):
    points = _points(frames_)
    if not points:
        return False, "no contact was reported", {}
    counts = _counts(frames_)
    xs = [p[2] for p in points]
    ys = [p[3] for p in points]
    ex = _width(rng) * knobs["edge_pct"] / 100.0
    ey = _height(rng) * knobs["edge_pct"] / 100.0
    hit = {"x_min": min(xs) <= rng["x"][0] + ex, "x_max": max(xs) >= rng["x"][1] - ex,
           "y_min": min(ys) <= rng["y"][0] + ey, "y_max": max(ys) >= rng["y"][1] - ey}
    metrics = {"hit": hit, "x_range": [min(xs), max(xs)], "y_range": [min(ys), max(ys)],
               "abs_min": [rng["x"][0], rng["y"][0]], "abs_max": [rng["x"][1], rng["y"][1]],
               "peak": max(counts)}
    if max(counts) != 1:
        return False, f"the edge drag must be one finger: {max(counts)} contacts at once", metrics
    if len(points) != sum(counts):
        return False, "an edge report carried no position", metrics
    missing = [name for name, ok in hit.items() if not ok]
    if missing:
        return False, "not reached (normalized to ABS min/max): " + ", ".join(missing), metrics
    if not _released(frames_):
        return False, "contact still active at the end of the window", metrics
    return True, "", metrics


def check_five_hold(frames_, rng, knobs):
    """Five DIFFERENT contacts, each with a real position, held together for a
    sustained time, then released in the same report.

    One report with five slots is not a hold (a duplicated or instantaneous
    report would pass forever), and five slots sharing one tracking id are one
    finger reported five times, not five fingers.
    """
    want = int(knobs["fingers"])
    counts = _counts(frames_)
    peak = max(counts)
    if peak < want:
        return False, f"only {peak} concurrent contacts (< {want})", {"peak": peak}
    full = [i for i, n in enumerate(counts) if n >= want]
    first_full, last_full = full[0], full[-1]
    held = (frames_[last_full]["t"] - frames_[first_full]["t"]) * 1000.0
    peak_contacts = frames_[first_full]["contacts"]
    tids = [contact.get("tid") for contact in peak_contacts]
    metrics = {"peak": peak, "held_ms": round(held, 1), "tids": tids,
               "distinct_tids": len(set(tids)), "full_frames": len(full)}
    if len(peak_contacts) != want:
        return False, f"{len(peak_contacts)} contacts reported at the peak (> {want})", metrics
    if any(tid is None for tid in tids):
        return False, "the peak report has a contact without a tracking id", metrics
    if len(set(tids)) != len(tids):
        return False, "two concurrent contacts share one tracking id", metrics
    if not all(_positioned(contact) and _in_range(contact, rng) for contact in peak_contacts):
        return False, "a peak contact has no valid position inside the device range", metrics
    if held < knobs["hold_min_ms"]:
        return False, (f"all {want} contacts were held only {held:.0f} ms"
                       f" (< {knobs['hold_min_ms']:.0f})"), metrics
    if any(n < want for n in counts[first_full:last_full + 1]):
        return False, "the five contacts were not held together for the whole interval", metrics
    if not _released(frames_):
        return False, "not every finger released: contacts still active",             {"peak": peak, "tail": counts[first_full:][-8:]}
    # A simultaneous lift goes peak -> 0 with no in-between count; anything else
    # is a staggered release (one finger at a time).
    skew = sum(1 for n in counts[last_full + 1:] if 0 < n < want)
    metrics["release_skew_frames"] = skew
    if skew > int(knobs["release_skew_frames"]):
        return False, f"release was staggered over {skew} reports", metrics
    return True, "", metrics


def check_pinch(frames_, rng, knobs):
    """The SAME two contacts, present without a gap, spread then squeezed.

    Three separate two-finger taps of different spans add up to a perfect
    out-then-in span history without ever being one pinch, so the pair must keep
    both identities and stay down for the whole gesture.
    """
    pairs = [(i, frame) for i, frame in enumerate(frames_) if len(frame["contacts"]) == 2]
    if not pairs:
        return False, "never saw exactly two concurrent contacts", {}
    identities = []
    for index, frame in pairs:
        keys = tuple(sorted((_key(contact) for contact in frame["contacts"]), key=str))
        if len(set(keys)) != 2:
            return False, "both concurrent contacts share one identity", {"frame": index}
        identities.append(keys)
    first, last = pairs[0][0], pairs[-1][0]
    spans = [_span(frame["contacts"]) for _, frame in pairs]
    base = spans[0]
    metrics = {"base_span": round(base, 1), "max_span": round(max(spans), 1),
               "min_span": round(min(spans), 1), "pairs": len(pairs),
               "identities": len(set(identities))}
    if base <= 0:
        return False, "the two contacts reported the same position", metrics
    if len(set(identities)) != 1:
        return False, ("the two-finger pair changed identity mid-gesture: separate taps,"
                       " not one pinch"), metrics
    if any(len(frame["contacts"]) != 2 for frame in frames_[first:last + 1]):
        return False, "a finger was lifted in the middle of the pinch: not one gesture", metrics
    out_i = spans.index(max(spans))
    in_i = spans.index(min(spans))
    out_pct = 100.0 * (max(spans) - base) / base
    in_pct = 100.0 * (base - min(spans)) / base
    metrics.update(span_out_pct=round(out_pct, 1), span_in_pct=round(in_pct, 1),
                   order="out_then_in" if out_i < in_i else "in_then_out")
    if out_pct < knobs["span_out_pct"]:
        return False, f"span only grew {out_pct:.1f}% (< {knobs['span_out_pct']:.1f}%)", metrics
    if in_pct < knobs["span_in_pct"]:
        return False, f"span only shrank {in_pct:.1f}% (< {knobs['span_in_pct']:.1f}%)", metrics
    if out_i >= in_i:
        return False, "the squeeze came before the spread (only one direction seen)", metrics
    if not _released(frames_):
        return False, "contacts still active at the end of the window", metrics
    return True, "", metrics


def check_recontact(frames_, rng, knobs):
    """Lift and re-touch: the new contact must be a NEW tracking id, and slot
    reuse must be visible when the device uses slots.

    A device that keeps reporting the same tracking id across a lift is not
    tracking contacts at all (userspace would see one endless finger), so that
    case fails even when the position is right.
    """
    lifts = []
    for index, frame in enumerate(frames_):
        for slot, tid in frame["released"]:
            lifts.append((index, slot, tid))
    if not lifts:
        if not any(c.get("tid") is not None for f in frames_ for c in f["contacts"]):
            # Type A/single node: without tracking ids a lift and a re-touch are
            # indistinguishable here, so this is not a failure of the panel.
            return None, "the selected node reports no tracking ids: a lift cannot be" \
                         " told apart from a re-touch", {}
        return False, "no contact release was reported (nothing to re-touch)", {}
    index, slot, tid = lifts[0]
    spots = [(i, c) for i, f in enumerate(frames_) if i > index
             for c in f["contacts"] if c.get("x") is not None and c.get("y") is not None]
    if not spots:
        return False, "the finger never came back down", {"released_tid": tid}
    at = None
    for i, contact in spots:  # first contact after the lift at the same place
        same = (abs(contact["x"] - _first_position(frames_, slot, tid, "x")) / _width(rng) * 100.0
                <= knobs["recontact_pct"]
                and abs(contact["y"] - _first_position(frames_, slot, tid, "y")) / _height(rng) * 100.0
                <= knobs["recontact_pct"])
        if same:
            at = (i, contact)
            break
    if at is None:
        return False, f"re-contact was farther than {knobs['recontact_pct']:.1f}% of range", \
            {"released_tid": tid}
    i, contact = at
    delay = (frames_[i]["t"] - frames_[index]["t"]) * 1000.0
    new_tid = contact.get("tid")
    metrics = {"released_tid": tid, "new_tid": new_tid, "delay_ms": round(delay, 1),
               "slot_reused": contact.get("slot") == slot, "release_slot": slot,
               "new_slot": contact.get("slot")}
    if new_tid is not None and new_tid == tid:
        return False, f"tracking id {tid} was reused after a lift: no lifecycle", metrics
    if delay > knobs["recontact_ms"]:
        return False, f"re-contact after {delay:.0f} ms (> {knobs['recontact_ms']:.0f})", metrics
    if not _released(frames_):
        return False, "contacts still active at the end of the window", metrics
    return True, "", metrics


def _first_position(frames_, slot, tid, axis):
    for frame in frames_:
        for contact in frame["contacts"]:
            if (contact.get("tid") == tid or contact.get("slot") == slot) \
                    and contact.get(axis) is not None:
                return contact[axis]
    return 0


CHECKS = {"tap": check_tap, "motion": check_motion, "edges": check_edges,
          "five_hold": check_five_hold, "pinch": check_pinch, "recontact": check_recontact}


def evaluate(name, frames_, rng, knobs=None, ctx=None):
    """Score one gesture window; a fail-closed condition always wins.

    `rng` is the ABS range the device reports: {"x": (min, max), "y": (min, max)}.
    `ctx` carries the window facts the decoder cannot know by itself (EOF, a
    trailing fragment, a truncated frame, contacts that were already down when
    the window opened, and the protocol the node's capabilities advertise).
    A check may return ok=None, which means "this node cannot answer it"
    (not_tested) — never "it looked fine".
    """
    settings = dict(KNOBS); settings.update(knobs or {})
    context = ctx or {}
    check = CHECKS.get(name)
    if check is None:
        return {"name": name, "status": "not_tested", "reason": f"no check for {name}",
                "metrics": {}, "knobs": settings}
    if context.get("protocol") == "A":
        return {"name": name, "status": "not_tested",
                "reason": "protocol_a: this node reports Type A (SYN_MT_REPORT) packets, which"
                          " this decoder does not score: only Type B slots and plain"
                          " single-touch are qualified",
                "metrics": {}, "knobs": settings}
    reason = closed_reason(frames_, context) or _sane(frames_, rng)
    if reason:
        return {"name": name, "status": "fail", "reason": reason,
                "metrics": {"frames": len(frames_)}, "knobs": settings}
    ok, why, metrics = check(frames_, rng, settings)
    status = "not_tested" if ok is None else ("pass" if ok else "fail")
    return {"name": name, "status": status, "reason": why,
            "metrics": metrics, "knobs": settings}
