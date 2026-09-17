#!/usr/bin/env python3
"""Structural sanity for the driver sources.

driver/spi-hid-core.c cannot be compiled on this host (no kernel headers), so
the kernel build in CI is the only *compile* gate and it runs minutes later, on
another machine. Two edits in this campaign were shipped broken because of
exactly that gap (a naive function deletion that cut a doc comment in half, and
a comment that swallowed the following declaration). This test is the cheap
local gate: it does not need headers because it only looks at structure.

Checks, per file: every /* has a */, and braces/parens/brackets balance outside
comments and string literals. A file that fails here cannot compile, whatever
the kernel headers would have said.
"""

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
FILES = sorted((ROOT / "driver").glob("*.c")) + sorted((ROOT / "driver").glob("*.h"))



def strip_comments_and_strings(text):
    """Return (stripped_text, unterminated_comment_line)."""
    out = []
    i, n = 0, len(text)
    line = 1
    while i < n:
        c = text[i]
        if c == "\n":
            line += 1
            out.append(c)
            i += 1
        elif text.startswith("/*", i):
            start = line
            end = text.find("*/", i + 2)
            if end == -1:
                return "".join(out), start
            line += text.count("\n", i, end + 2)
            out.append(" " * 2)
            i = end + 2
        elif text.startswith("//", i):
            end = text.find("\n", i)
            i = n if end == -1 else end
        elif c in "\"'":
            quote = c
            i += 1
            while i < n and text[i] != quote:
                if text[i] == "\\":
                    i += 1
                elif text[i] == "\n":
                    break
                i += 1
            i += 1
            out.append('""')
        else:
            out.append(c)
            i += 1
    return "".join(out), None


def check_control_flow_pins():
    """Two control-flow shapes this campaign already paid for, pinned cheaply.

    Neither needs kernel headers: both are shapes in a function body, so this
    catches them minutes before the CI kernel build would (on another machine).
    """
    failures = 0
    core = (ROOT / "driver/spi-hid-core.c").read_text()
    wire = (ROOT / "driver" / "spi-hid-wire-frames.h").read_text()

    # Checks below that care whether code RUNS read these instead: a raw-text
    # pin is satisfied by a comment, which is how three pins in this file were
    # shown decorative by an adversarial leg that moved the real code into a
    # comment and left the literal behind. Strip comments, keep strings.
    def strip_c_comments(text):
        text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
        text = re.sub(r"//[^\n]*", " ", text)
        # Preprocessor-disabled code is not code either: a leg neutralised a
        # guard inside `#if 0` and the comment-stripping pins stayed green.
        text = re.sub(r"#if\s+0\b.*?#endif", " ", text, flags=re.S)
        return text

    core_code = strip_c_comments(core)
    wire_code = strip_c_comments(wire)

    # 0a. The read-frame default must be the REFERENCE shape. The field sweep
    # that once argued for LEGACY measured the device's state, not the frame: a
    # stream-stuck device answers whatever it is handed. The five-byte form
    # cannot carry a content type or id, so a descriptor read and a feature read
    # leave the host byte-identical and the device cannot tell them apart —
    # found independently by two blind legs, and against the header's own
    # comment on the builder. The stop frame above removes the stream state that
    # made the crude form look necessary; `hunt` still sweeps all three.
    if "static int read_frame_variant = SPI_HID_READ_FRAME_LEGACY;" not in core_code:
        print("FAIL driver/spi-hid-core.c: read_frame_variant no longer defaults to the "
              "shape this PANEL answers. The reference shape is the authority on the "
              "sequence, but the field sweep of 2026-09-16 19:11 says the encoding this "
              "device accepts is the five-byte one — variant 0 silent, variant 1 reaching "
              "DONE with ready set and the reset count down from 47 to 4")
        failures += 1

    # 0b. The stream that survives a host reboot has to be torn down before the
    # descriptor handshake, and the teardown is the reference's all-FF
    # SET_FEATURE(0x56) (surface_init.csv #0257). Without it the device keeps
    # streaming and device_desc stays 0 — the field stall.
    if "spi_hid_wire_vendor_stop" not in wire_code:
        print("FAIL driver/spi-hid-wire-frames.h: the all-FF stream-stop frame is gone "
              "(the device keeps streaming and the handshake never completes)")
        failures += 1
    else:
        body = core_code.split("static int spi_hid_vendor_init(struct spi_hid *shid)", 1)[1].split("\n}", 1)[0]
        if "spi_hid_wire_vendor_stop" not in body:
            print("FAIL driver/spi-hid-core.c: the probe no longer sends the stream stop")
            failures += 1
        else:
            # Pin the WRITE ORDER, not the position of a declaration: the
            # previous form matched the `struct ... stop = spi_hid_wire_vendor_stop()`
            # line, which sits before the d2 declaration whatever the code
            # below does with it. An adversarial leg moved the actual write
            # after the D0 write and the pin stayed green.
            stop_w = body.find("spi_hid_seq_write(shid, stop.bytes")
            d2_w = body.find("spi_hid_seq_write(shid, d2.bytes")
            if stop_w < 0 or d2_w < 0:
                print("FAIL driver/spi-hid-core.c: cannot find the stop / d2 send sites")
                failures += 1
            elif stop_w > d2_w:
                print("FAIL driver/spi-hid-core.c: the stream stop is SENT after the power "
                      "sequence instead of before the handshake")
                failures += 1

    # 1. spi_hid_ll_parse(): the mutex_unlock must not be the body of an `else`.
    # A dangling `else` had put the unlock in the success branch only, so a
    # failed hardcoded-descriptor parse returned with shid->lock held — and
    # every later lock taker (IRQ thread, sysfs readers, remove) waits forever.
    body = core_code.split("static int spi_hid_ll_parse", 1)[1].split("\n}", 1)[0]
    tail = body.rsplit("HARDCODED_RD_SIZE);", 1)[-1].split("mutex_unlock", 1)[0]
    tail, _ = strip_comments_and_strings(tail)
    if "else" in tail:
        print("FAIL driver/spi-hid-core.c: spi_hid_ll_parse() — mutex_unlock is "
              "conditional again (dangling else), so the parse-failure path "
              "returns holding shid->lock")
        failures += 1

    # 2. spi_hid_seq_set_state(): the raw discovery watchdog must be armed for
    # WAIT_DESC/WAIT_RPT *before* the unchanged-state early return — the
    # RESET_RSP loop re-enters WAIT_DESC with the state unchanged, which is the
    # field stall: silent at level 0, and permanent.
    body = core_code.split("spi_hid_seq_set_state(struct spi_hid *shid", 1)[1].split("\n}", 1)[0]
    if "SPI_HID_SEQ_WAIT_DESC || new_state == SPI_HID_SEQ_WAIT_RPT" not in body:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_set_state() no longer arms "
              "the raw watchdog for WAIT_DESC/WAIT_RPT (pre-DONE stall is silent "
              "and permanent again)")
        failures += 1
    elif "SPI_HID_SEQ_WAIT_RESET ||" not in body:
        print("FAIL driver/spi-hid-core.c: the raw watchdog no longer covers "
              "WAIT_RESET at cold probe (a controller that never sends a "
              "RESET_RSP has no timer at all)")
        failures += 1
    elif body.index("raw_handshake_watchdog") > body.index("if (old_state == new_state)"):
        print("FAIL driver/spi-hid-core.c: the pre-DONE watchdog arm sits after "
              "the unchanged-state return, so a re-entered WAIT_DESC never arms it")
        failures += 1

    # 3. spi_hid_resume(): it assigns the state directly instead of going
    # through spi_hid_seq_set_state(), and the standard-mode arm it does call is
    # a no-op in raw mode — so without its own arm a controller that comes back
    # from resume without a RESET_RSP has no timer at all and stays dead.
    body = core_code.split("static int spi_hid_resume", 1)[1].split("\n}", 1)[0]
    if "raw_handshake_watchdog" not in body:
        print("FAIL driver/spi-hid-core.c: spi_hid_resume() no longer arms the raw "
              "watchdog (raw mode has no timer for a silent post-resume controller)")
        failures += 1

    # 4. spi_hid_raw_handshake_watchdog(): its "fall back to standard HID" branch
    # must install the hardcoded descriptors first. Without them
    # spi_hid_create_device_work() sees version 0, refuses to publish the device
    # and schedules the ACPI power cycle — the field case where the raw handshake
    # failed and the fallback left the panel deader than before.
    body = core_code.split("static void spi_hid_raw_handshake_watchdog", 1)[1].split("\n}", 1)[0]
    if "spi_hid_use_hardcoded_desc" not in body:
        print("FAIL driver/spi-hid-core.c: the raw watchdog's standard-HID fallback "
              "no longer installs the hardcoded descriptors (create_device_work() "
              "will reject version 0 and power the panel down instead)")
        failures += 1

    # 5. the descriptor response path. Windows answers a request on the output
    # register (the boot trace reads the DEVICE_DESC and the RPT_DESC from
    # register 3 right after the request, with no interrupt in between), while
    # the device pushes its events on the input register. Reading only the input
    # register is what kept the field unit in WAIT_DESC forever.
    if "spi_hid_seq_read_resp" not in core_code:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read_resp() is gone; the "
              "descriptor bodies would only ever be read from the input register")
        failures += 1
    body = core_code.split("static void spi_hid_seq_descreq_work", 1)[1].split("\n}", 1)[0]
    if "desc.output_register" not in body or "desc.input_register" not in body:
        print("FAIL driver/spi-hid-core.c: the descriptor poller no longer tries "
              "both registers (responses live on the output register, events on "
              "the input one)")
        failures += 1

    # 6. the read approval frame: nine bytes, the register at offset 7, the
    # address field zero. The device decodes the register from that offset; a
    # five-byte frame carrying it in the address field asks for register 0 and
    # is answered with the device's RESET_RSP — which is how discovery stalled
    # while the host thought it was asking for the descriptor.
    read_reg = core_code.split("static int spi_hid_seq_read_reg", 1)[1].split("\n}", 1)[0]
    if "rx_len < n ? n :" in read_reg or "tx_len = (u32)rx_len" in read_reg:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read_reg() pads the request "
              "to the response length again — that clocks stray bytes out and the "
              "device stops answering (field bundle: RESET_RSP per second -> none)")
        failures += 1
    if "shid->read_resp_type" not in read_reg or "shid->read_resp_content_id" not in read_reg:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read_reg() no longer names the "
              "request it reads the response of (read_resp_type/_content_id, offsets "
              "6 and 8 of the read approval)")
        failures += 1
    # 6b. The reset marker and the reset reaction. Windows' VerifyResetResponse
    # tests the WHOLE first byte (msg[0] == 3, hidspicx_dd64); this parser derives
    # the type from a nibble, and the two disagree on the device's idle frame
    # `32 10 00 5a` — which, unguarded, is answered as a reset with a DESCREQ, 47
    # times in one field pass. And the reference's reaction to a real reset is
    # named in the PDB: ResettingSyncEntry — ResetDevice, then a 2000 ms timer.
    # The detector's LOGIC is not pinned by text any more: it lives in
    # spi-hid-protocol.h and tests/wire_frames_test.c calls it with the
    # reference's buffers AND with this panel's own prefixed answers (the
    # assertions added after a leg showed no test held a real field buffer). That is the one form of this check a comment, an
    # `#if 0` block or a string literal cannot satisfy — five text pins in this
    # file were demonstrated decorative by adversarial legs. What remains here is
    # the routing, which a call-based test cannot see: if the driver stops going
    # through that function, the host test would still pass while the driver used
    # something else.
    if "spi_hid_protocol_frame_type" not in core_code:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_hdr_type() no longer routes through "
              "spi_hid_protocol_frame_type() — the frame typing the driver runs is no "
              "longer the one the host test exercises with real buffers")
        failures += 1
    # Every static function must be DECLARED or defined before its first call.
    # Twice tonight a helper was called from far above its own definition and
    # only the kernel build noticed, because this suite never compiles the
    # translation unit. The check below is the general form of that trap: for
    # each function the file defines, the first line mentioning it as a
    # declaration/definition must come before the first line that calls it.
    _ls = core_code.splitlines()
    # TWO passes, and the first version of this check was a lesson in why: it
    # built the definition table in the same pass as the call scan, so a name
    # defined further down was not yet in the table when the call was seen —
    # it could not fire for the exact case it exists to catch.
    _first_static = {}
    for _i, _line in enumerate(_ls, 1):
        _m = re.match(r"\s*static\s+[A-Za-z_][\w ]*?(\w+)\s*\(", _line)
        if _m and _m.group(1) not in _first_static:
            _first_static[_m.group(1)] = _i
    _first_call = {}
    for _i, _line in enumerate(_ls, 1):
        _m = re.match(r"\s*static\s+[A-Za-z_][\w ]*?(\w+)\s*\(", _line)
        for _name in re.findall(r"\b(\w+)\s*\(", _line):
            if _name in _first_call or _name not in _first_static:
                continue
            if _m and _name == _m.group(1):
                continue          # the declaration/definition line itself
            # NOT "skip lines ending in ';'": that was this check's second
            # draft, and every CALL also ends in a semicolon — it skipped
            # precisely the lines it existed to inspect. A prototype is already
            # excluded above, by being the same name as the static match.
            _first_call[_name] = _i
    _early = {k: (_first_call[k], _first_static[k]) for k in _first_call
              if _first_call[k] < _first_static[k]}
    if _early:
        print(f"FAIL driver/spi-hid-core.c: called before declared/defined "
              f"{dict(list(_early.items())[:4])} — the kernel build is the only thing that "
              f"catches this; add the forward declaration")
        failures += 1

    # The handshake reads register 0, the stream register only after arming.
    # The reference's boot reads register 0 (parser: TX 0B 00 00 00 FF 00 00 00
    # 00) and its answer arrives there; this driver pointed every read at the
    # stream register from probe setup, before the stream existed.
    _rd = (core_code.split("static int spi_hid_seq_read(struct", 1)[1].split("\n}", 1)[0]
           if "static int spi_hid_seq_read(struct" in core_code else "")
    if not ("SPI_HID_SEQ_WAIT_RESET" in _rd and "output_register" in _rd
            and "SPI_HID_SEQ_DONE" in _rd):
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read() no longer points the handshake "
              "reads at register 0 — that is the reference's own register for them")
        failures += 1

    # The stream enable must not run before the descriptor exchange: the
    # reference configures the stream after it (boot trace TXN#9+), and doing
    # it first is what this driver did while the device answered every DESCREQ
    # with a reset. One call site, inside the idempotent arming helper.
    if core_code.count("spi_hid_raw_enable_stream(shid);") != 1:
        print("FAIL driver/spi-hid-core.c: the stream enable must be called from exactly one "
              "place — the arming helper that runs at DONE — not from probe setup")
        failures += 1
    if core_code.count("spi_hid_raw_stream_arm(shid);") < 1:
        print("FAIL driver/spi-hid-core.c: nothing arms the raw stream any more")
        failures += 1

    # And the other half of that rule: the poller must keep its DONE gate,
    # because that gate is what makes reading the stream register safe. Without
    # it, spi_hid_seq_read() would answer with register 0 while the stream is up.
    _pw = (core_code.split("static void spi_hid_poll_work", 1)[1].split("\n}", 1)[0]
           if "static void spi_hid_poll_work" in core_code else "")
    if "SPI_HID_SEQ_DONE" not in _pw or "spi_hid_seq_read(" not in _pw:
        print("FAIL driver/spi-hid-core.c: spi_hid_poll_work no longer reads the stream "
              "under its DONE gate — the register rule flips on that state")
        failures += 1

    # Duplicate definitions. Slice arithmetic of mine once duplicated 850 lines
    # of spi-hid-core.c; the host suite never compiles that translation unit, so
    # local runs stayed green and only the kernel build caught it. The rule is
    # one line and would have caught it in a second: every function is defined
    # exactly once, and prototypes (lines ending in ';') do not count.
    import re as _re

    _lines = core_code.splitlines()
    defs = {}
    for _i, _line in enumerate(_lines, 1):
        _m = _re.match(r"\s*static\s+[A-Za-z_][\w ]*?(\w+)\s*\(", _line)
        # A DEFINITION is a matching line whose next line opens the body. This is
        # stricter than "does not end in ';'" on purpose: a prototype split over
        # two lines (spi_hid_seq_set_state) ends with a comma, not a semicolon,
        # and counting it was the false positive in this check's first draft.
        if (_m and _i < len(_lines)
                and _lines[_i].strip() == "{"):
            defs.setdefault(_m.group(1), []).append(_i)
    _dups = {k: v for k, v in defs.items() if len(v) > 1}
    if _dups:
        print(f"FAIL driver/spi-hid-core.c: functions defined more than once "
              f"{dict(list(_dups.items())[:4])} — the file was assembled by concatenation, "
              f"not edited")
        failures += 1

    # 6b. The reset path, as the reference's own trace shows it: a nine-byte
    # read of register 0 answers with 32 10 00 5A (RESET_RSP), the next read
    # drains it (03 00 00 00), and the DESCREQ follows ~156 us later. No wait,
    # no rate limit, no device reset — I shipped all three, and the device
    # behaved correctly the whole time. The reset-path checks below are the
    # ones that survive: the sites must still recover, and they must not sleep.
    if core_code.count("spi_hid_seq_restart_discovery(shid,") < 4:
        print("FAIL driver/spi-hid-core.c: fewer than four reset sites recover "
              "(WAIT_RESET, WAIT_DESC, WAIT_RPT/WAIT_FEATURE, DONE)")
        failures += 1
    hdrfn = (core_code.split("static int spi_hid_seq_hdr_type", 1)[1].split("\n}", 1)[0]
             if "static int spi_hid_seq_hdr_type" in core_code else "")
    if "spi_hid_protocol_frame_type" not in hdrfn:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_hdr_type() no longer routes through "
              "spi_hid_protocol_frame_type() — the typing the driver runs would no longer be "
              "the one the host test exercises")
        failures += 1
    # A pin against a defect CLASS, kept because the defect was nearly shipped:
    # the reference continues 156 us after a reset, so nothing in this path may
    # sleep. A 2000 ms wait here is not a timeout, it is a delay the panel does
    # not have — and under seq_lock it is a livelock.
    rp = core_code.split("static int spi_hid_seq_restart_discovery", 1)[1].split("\n}", 1)[0]
    if "msleep" in rp or "udelay" in rp:
        print("FAIL driver/spi-hid-core.c: the reset recovery path sleeps — the reference "
              "continues 156 us after draining a reset, and a wait here (especially under "
              "seq_lock) is the livelock this campaign already measured once")
        failures += 1

    if "spi_hid_wire_read_approval" in wire:
        approval = wire.split("spi_hid_wire_read_approval_variant", 1)[1].split("\n}", 1)[0]
        for needle, why in (
            ("out[7] = reg & 0xff", "the register no longer sits at offset 7"),
            ("out[1] = 0;",
             "the frame is no longer zeroed first, so the address field comes from "
             "whatever was in the buffer before"),
            ("out[6] = content_type", "the request's content type is no longer at offset 6"),
            ("out[9] = content_id", "the request's content id is no longer at offset 9"),
            ("SPI_HID_WIRE_OPCODE_READ", "the read opcode is gone"),
        ):
            if needle not in approval:
                print(f"FAIL driver/spi-hid-wire-frames.h: spi_hid_wire_read_approval(): {why} "
                      "— the device reads the register from offset 7 and ignores the "
                      "address field, so the frame would ask for register 0")
                failures += 1
    else:
        print("FAIL driver/spi-hid-wire-frames.h: spi_hid_wire_read_approval() is gone")
        failures += 1

    # 7. the raw stream enable, and the content id rule.
    # The reference enables the stream with one SET_FEATURE (#0531) — content id
    # 0x56 on register 0x0A, payload BD 0C EE 5B 44 4C 00 — and names a content
    # id only when it reads a body. Without the enable the device never streams;
    # with the id on a header read the frame differs from the reference.
    for needle, why in (
        ("spi_hid_wire_vendor_init(0)", "the stream enable no longer sends the reference's frame"),
        ("SPI_HID_RAW_STREAM_CONTENT_ID", "the stream enable no longer names content id 0x56"),
        ("SPI_HID_RAW_STREAM_REGISTER 0x0A", "the stream register is no longer 0x0A"),
    ):
        if needle not in core_code:
            print(f"FAIL driver/spi-hid-core.c: {why} (trace #0531 / #0004-#0873)")
            failures += 1
    # The writes that ask for a response must record which request they are,
    # or the read that follows names nothing (trace: 00 04 03 00 06,
    # 00 03 0A 00 56). The descriptor requests are the 0/0 case.
    for fn, want in (("vendor_init", "SPI_HID_CONTENT_TYPE_SET_FEATURE"),
                     ("get_feature6", "SPI_HID_CONTENT_TYPE_GET_FEATURE"),
                     ("setfeat", "shid->read_resp_content_id = 5")):
        marker = f"static int spi_hid_seq_write_{fn}"
        # rfind: the forward declarations at the top of the file would
        # otherwise be the match, and a prototype has no body to check.
        seg = core_code.rsplit(marker, 1)[1][:900] if marker in core_code else ""
        if want not in seg:
            print(f"FAIL driver/spi-hid-core.c: spi_hid_seq_write_{fn}() does not "
                  f"record the request its response belongs to — its reads go out "
                  f"naming nothing")
            failures += 1
    if "shid->desc.max_input_length = 0x2000;" not in core_code:
        print("FAIL driver/spi-hid-core.c: the fallback's max_input_length is not "
              "0x2000 — a 4096 cap truncates the 4309-byte raw frames")
        failures += 1
    if "rx_len > SPI_HID_READ_APPROVAL_LEN ?" not in read_reg:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read_reg() names a content id "
              "on nine-byte reads again — the reference names it only on bodies")
        failures += 1

    # 8. the segmented read in spi-amd.c. The FIFO holds the request, the
    # answer and the controller's extra byte, so a chunk that does not fit is
    # rejected outright (tx + rx + 1 > 70) — a fixed 64-byte first chunk only
    # fits a five-byte request, and the reference's request is nine or ten.
    # Every long read (the 32-byte descriptor body, the 940-byte report
    # descriptor, the 4304-byte raw frames) goes through this path.
    amd = strip_c_comments((ROOT / "driver" / "spi-amd.c").read_text())
    for needle, why in (
        ("AMD_SPI_FIFO_SIZE - tx_len - 1",
         "the first chunk is no longer computed from what is left of the FIFO"),
        ("AMD_SPI_CONT_CMD_LEN", "the continuation command is gone"),
        ("cont_cmd, sizeof(cont_cmd)", "a continuation sends the whole request again"),
    ):
        if needle not in amd:
            print(f"FAIL driver/spi-amd.c: {why} — long reads cannot fit the "
                  f"70-byte FIFO and the segment math is the only thing that "
                  f"makes them work")
            failures += 1

    # 9. a module parameter must be declared above the code that reads it. Twice
    # in this campaign one was added next to its neighbours and used higher up
    # the file; only the kernel build noticed, minutes later on CI. Comments and
    # strings are stripped and the match is a whole identifier, so `raw_mode` is
    # not confused with `raw_mode_active` — and the first mention has to be a
    # declaration line, not a use inside a function.
    code, _ = strip_comments_and_strings(core)
    for m in re.finditer(r"module_param\((\w+)", code):
        name = m.group(1)
        first = re.search(r"\b" + re.escape(name) + r"\b", code)
        if first is None:
            continue
        line = code[:first.start()].rsplit("\n", 1)[-1].strip()
        if not line.startswith(("static", "int", "bool", "unsigned", "char",
                                "u8", "u16", "u32", "u64", "const", "struct")):
            print(f"FAIL driver/spi-hid-core.c: '{name}' is first mentioned as "
                  f"'{line[:60]}', not as a declaration — the kernel build will "
                  f"reject the use above the declaration")
            failures += 1


    # 10. the read-path peeks all three candidate regions. The RX offset for a
    # read command is an open question (fixed 0x84 in the decomp's three-byte
    # example, tx_len + 1 in ours) and only the field can answer it; if this
    # line disappears the next bundle cannot either.
    amd = strip_c_comments((ROOT / "driver" / "spi-amd.c").read_text())
    for needle, why in (
        ("TRACE peek tx_len=", "the read-path region peek is gone"),
        # The full label set, in order: window 1 named 0x80 (it reads
        # base+fifo_pos), window 2 the computed 0x80+tx_len (the doc's
        # 0x80+TX_COUNT form — shipped unobserved until f27c651's mislabelled
        # window 1 was caught), window 3 the literal 0x84, window 4 the
        # computed tx_len+1.
        ("0x80=[%*ph] 0x%02x=[%*ph] 0x84=[%*ph] 0x%02x=[%*ph]",
         "the peek's window/label set changed — a label that no longer mirrors "
         "the address it prints can settle the RX offset question WRONG"),
        ("0x84=[%*ph]", "the fixed-0x84 candidate is no longer logged"),
        # Pin the DERIVATION, not a hand-written address: the label used to
        # say 0x89 for every request length, which was a lie for all but the
        # eight-byte one. What matters is that the third candidate is read at
        # tx_len+1 and that its label is computed from the same expression.
        ("0x80u + (unsigned int)tx_len + 1u",
         "the tx_len+1 candidate is no longer logged, or its address is no longer "
         "computed — the RX offset question goes back to being settled by argument"),
        # And the label/pointer ADJACENCY for the two computed windows: a label
        # and its address are one pair, and f27c651 proved one can be edited
        # without the other (window 1 printed 0x80+tx_len over base+fifo_pos
        # and the suite stayed green).
        ("0x80u + (unsigned int)tx_len,\n\t\t\t\t16, base + fifo_pos + tx_len,",
         "window 2's label and the address it prints have drifted apart"),
        ("0x80u + (unsigned int)tx_len + 1u,\n\t\t\t\t16, base + fifo_pos + tx_len + 1);",
         "window 4's label and the address it prints have drifted apart"),
    ):
        if needle not in amd:
            print(f"FAIL driver/spi-amd.c: {why} — the RX offset question goes "
                  f"back to being settled by argument")
            failures += 1

    # 11. A per-frame failure may not be a per-frame log line: the CapImg
    # decode failures fire once per received frame (up to ~100 Hz on a
    # wrong-SKU or truncated stream), and an unratelimited dev_warn would
    # flood the ring buffer the field bundles are read from. Both paths.
    for needle in (
        'dev_warn_ratelimited(dev, "SEQ: CapImg decode failed',
        'dev_warn_ratelimited(dev, "SEQ: poller CapImg decode failed',
    ):
        if needle not in core_code:
            print(f"FAIL driver/spi-hid-core.c: {needle!r} missing — a per-frame "
                  f"CapImg decode failure is back to flooding the log")
            failures += 1

    return failures


def check_trace_event_liveness():
    """No trace event may outlive its last producer.

    Two sweeps already deleted producer-less trace events (their DEFINE_EVENT
    instances went in af72664 / 8d33a8f); this campaign found two more — whole
    event classes with no instance and no `trace_<event>()` producer anywhere,
    i.e. tracepoints the field can enable and that can never fire. The rule is
    structural: a class needs an instance, an instance needs a producer.
    Comments and #if 0 blocks are stripped before matching — a pin a comment
    can satisfy is decorative.
    """
    failures = 0

    def live_code(text):
        text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
        text = re.sub(r"//[^\n]*", " ", text)
        text = re.sub(r"#if\s+0\b.*?#endif", " ", text, flags=re.S)
        return text

    trace = live_code((ROOT / "driver" / "spi-hid_trace.h").read_text())
    producers = "\n".join(
        live_code(p.read_text())
        for p in sorted((ROOT / "driver").glob("*.c"))
        + sorted((ROOT / "driver").glob("*.h"))
        if p.name != "spi-hid_trace.h"
    )

    classes = set(re.findall(r"\bDECLARE_EVENT_CLASS\(\s*(\w+)", trace))
    instances = re.findall(r"\bDEFINE_EVENT\(\s*(\w+)\s*,\s*(\w+)", trace)
    event_names = {name for _, name in instances} | set(
        re.findall(r"\bTRACE_EVENT\(\s*(\w+)", trace))

    for cls in sorted(classes - {cls for cls, _ in instances}):
        print(f"FAIL driver/spi-hid_trace.h: event class '{cls}' has no "
              f"DEFINE_EVENT instance — it can never fire; delete it or wire it")
        failures += 1
    for event in sorted(event_names):
        if f"trace_{event}(" not in producers:
            print(f"FAIL driver/spi-hid_trace.h: trace event '{event}' has no "
                  f"trace_{event}() producer — it can never fire; delete it or wire it")
            failures += 1
    return failures


def main():
    failures = check_control_flow_pins()
    failures += check_trace_event_liveness()
    for path in FILES:
        text = path.read_text()
        stripped, unterminated = strip_comments_and_strings(text)
        if unterminated is not None:
            print(f"FAIL {path.name}: comment opened at line {unterminated} is never closed")
            failures += 1
            continue
        for opener, closer in (("{", "}"), ("(", ")"), ("[", "]")):
            depth = 0
            for line_no, line in enumerate(stripped.splitlines(), 1):
                depth += line.count(opener) - line.count(closer)
                if depth < 0:
                    print(f"FAIL {path.name}:{line_no}: unbalanced '{closer}'")
                    failures += 1
                    break
            else:
                if depth != 0:
                    print(f"FAIL {path.name}: {depth:+d} unclosed '{opener}' at end of file")
                    failures += 1
    # The probe's frame-typing self-check went DECORATIVE for days: it asserted
    # only the reference traces' buffers while every frame this driver actually
    # received typed as -1, so the one diagnostic meant to catch exactly that
    # printed OK. Pin the panel's own shapes into it. A text pin is weak — what
    # made this one count was the mutation run that proved it fails when the
    # shapes are removed (the first version of this check was dead code: it sat
    # outside main() and used a root variable that does not exist here).
    core_src, _ = strip_comments_and_strings((ROOT / "driver" / "spi-hid-core.c").read_text())
    # The body offset helper returns the struct offset; an `off += 3` after it
    # reads three bytes late and rejects every real descriptor (8+3+28 > 37 on
    # the capture's 37-byte body). This exact mistake shipped once.
    # The caller side too: inside seq_handle_desc the helper's result is the
    # struct offset, so neither an `off += 3` nor a `+ 3` in the guard may
    # reappear. A leg demonstrated this exact mutation as suite-green once.
    # Anchor on the DEFINITION — the prototype has no braces, and an earlier
    # version of this check walked into the next function's body instead.
    _needle, _pos, _fn = "static void seq_handle_desc(", -1, ""
    while True:
        _pos = core_src.find(_needle, _pos + 1)
        if _pos < 0:
            break
        _cand = core_src[_pos:]
        if _cand[:_cand.index("\n")].rstrip().endswith(";") is False:
            _depth, _end = 0, len(_cand)
            for _i, _ch in enumerate(_cand):
                if _ch == "{":
                    _depth += 1
                elif _ch == "}":
                    _depth -= 1
                    if _depth == 0:
                        _end = _i
                        break
            _fn = _cand[:_end]
            break
    if _fn and ("off += 3" in _fn or "off + 3 + required" in _fn):
        print("FAIL body offset: seq_handle_desc adds the content header a second time")
        failures += 1
    # The body-offset helper returns the STRUCT offset; the guard must not add
    # the content header a second time. 8 + 3 + 28 > 37 rejected the capture's
    # 37-byte body once, in a batch that claimed to fix discovery, so both
    # halves are pinned — and the shape of this check is proven by mutation.
    if "off + 3 + required" in core_src:
        print("FAIL body offset: the guard adds the reserved 3 a second time")
        failures += 1
    if "off + required > rblen" not in core_src:
        print("FAIL body offset: the guard no longer fits the capture's 37-byte body")
        failures += 1

    for _name in ("self_panel_reset", "self_panel_desc"):
        if f"static const u8 {_name}[12]" not in core_src:
            print(f"FAIL self-check: {_name} missing from the probe's frame-typing self-check")
            failures += 1
        elif core_src.count(_name) < 2:
            print(f"FAIL self-check: {_name} declared but never used")
            failures += 1
    if "self_off == 8" not in core_src:
        print("FAIL self-check: the probe's self-check no longer asserts the panel's frame offset")
        failures += 1

    if failures:
        print(f"driver source sanity: {failures} failure(s)")
        return 1
    print(f"driver source sanity: PASS ({len(FILES)} files)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
