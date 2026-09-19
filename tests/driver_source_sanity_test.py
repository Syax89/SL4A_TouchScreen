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
                    # A backslash escapes the NEXT character: a `\"` inside
                    # the literal must not end it. The P15 rewrite advanced
                    # by one and handed everything after a `\"` to the code
                    # view — a decoy string then carried a pin's needle while
                    # the real code was reverted (P16 wave, in-tree proven).
                    if i + 1 < n and text[i + 1] == "\n":
                        line += 1
                        out.append("\n")
                    i += 2
                elif text[i] == "\n":
                    # Unterminated literal (gcc rejects it): the next line must
                    # NOT reach the code view. The old break-and-skip handed it
                    # to the scanner as code, so a quote at end-of-line put the
                    # needle right back (P15 wave bypass). Keep scanning as
                    # literal; emit the newline for line accounting.
                    line += 1
                    out.append("\n")
                    i += 1
                else:
                    i += 1
            i += 1
            out.append('""')
        else:
            out.append(c)
            i += 1
    return "".join(out), None


# ── The conditional-compilation ladder ─────────────────────────────────
# `#if 0` carried needles until the P15 wave banned the literal-zero
# family; the P16 wave widened the lens — `#ifdef NEVER_DEFINED_X`,
# `#if defined(X) && 0` and `#if <macro #define'd 0>` let a needle sit in
# a block the compiler drops. A directive is refused (and its block
# dropped from the code view) only when its condition is PROVABLY false
# for this tree: every symbol it tests has no `#define` in any driver
# file and is not kernel-provided. The `#ifndef X` + `#define X` idiom
# (include guards, default-if-undefined) is kept. Unknown shapes are
# kept too — the ladder refuses only what it can prove, and the
# code-view strip is what makes a hidden needle fail its pin.
_KERNEL_PROVIDED = {"__KERNEL__", "LINUX_VERSION_CODE", "KERNEL_VERSION",
                    "TRACE_HEADER_MULTI_READ", "__has_include"}


def _driver_macro_sets():
    defined, zero = set(), set()
    for _p in FILES:
        _src = _p.read_text()
        defined |= set(re.findall(r"^[ \t]*#[ \t]*define[ \t]+(\w+)", _src, re.M))
        zero |= set(re.findall(
            r"^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+(?:0[xX]0*|[0]+[uUlL]*)"
            r"[ \t]*(?://[^\n]*)?$", _src, re.M))
    return defined, zero


_DEFINED_MACROS, _ZERO_MACROS = _driver_macro_sets()


def _sym_never_defined(sym):
    return (sym not in _DEFINED_MACROS
            and not sym.startswith("CONFIG_")
            and sym not in _KERNEL_PROVIDED)


def _guards_itself(directive, following_text):
    """`#ifndef X` whose block opens with `#define X`: the guard idiom."""
    m = re.match(r"[ \t]*#[ \t]*ifndef[ \t]+(\w+)", directive)
    if not m:
        return False
    n = re.match(r"[ \t]*#[ \t]*define[ \t]+(\w+)", following_text or "")
    return bool(n and n.group(1) == m.group(1))


def _directive_is_provably_false(directive):
    d = directive.strip()
    m = re.match(r"#\s*ifdef\s+(\w+)", d)
    if m:
        return _sym_never_defined(m.group(1))
    m = re.match(r"#\s*ifndef\s+(\w+)", d)
    if m:
        return (m.group(1) in _DEFINED_MACROS
                and m.group(1) not in _KERNEL_PROVIDED)
    m = re.match(r"#\s*if\s+(.+)$", d)
    if m:
        cond = m.group(1).strip()
        if re.search(r"&&[ \t]*\(*[ \t]*(?:0[xX]0*|[0]+[uUlL]*)[ \t]*\)*"
                     r"[ \t]*(?://.*)?$", cond):
            return True                     # `<anything> && 0`
        if re.match(r"^\w+$", cond):
            return cond in _ZERO_MACROS or _sym_never_defined(cond)
        syms = re.findall(r"\bdefined\s*\(\s*(\w+)\s*\)", cond)
        if syms and not re.search(r"[&|]", cond):
            return all(_sym_never_defined(s) for s in syms)
    return False


def _strip_preproc_disabled(text):
    # Literal-zero family (P15 wave): kept as its own pass, mutation-proven.
    text = re.sub(
        r"#if[ \t]*\(*[ \t]*(?:0[xX]0*|[0]+[uUlL]*)[ \t]*\)*[ \t]*"
        r"(?://[^\n]*)?\n.*?#endif", " ", text, flags=re.S)

    def _repl(m):
        body = m.group("body")
        first = next((ln for ln in body.split("\n") if ln.strip()), "")
        if _guards_itself(m.group("dir"), first):
            return m.group(0)
        return " " if _directive_is_provably_false(m.group("dir")) else m.group(0)

    return re.sub(
        r"^[ \t]*(?P<dir>#\s*(?:ifdef|ifndef|if)[^\n]*)\n"
        r"(?P<body>.*?)^[ \t]*#\s*endif[^\n]*",
        _repl, text, flags=re.S | re.M)


def code_view(text, keep_strings=False):
    """The code view every structural pin reads.

    Ladder, each rung shown necessary by a leg that kept a pin green while
    the real code was neutralised: comments and strings (P2 wave), the
    `#if 0` family (P15 wave), provably-false macro conditionals (P16 wave).
    `keep_strings=True` is for the few pins whose subject IS a log line's
    text.
    """
    if keep_strings:
        text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
        text = re.sub(r"//[^\n]*", " ", text)
    else:
        text, _ = strip_comments_and_strings(text)
    return _strip_preproc_disabled(text)


def check_control_flow_pins():
    """Two control-flow shapes this campaign already paid for, pinned cheaply.

    Neither needs kernel headers: both are shapes in a function body, so this
    catches them minutes before the CI kernel build would (on another machine).
    """
    failures = 0
    core = (ROOT / "driver/spi-hid-core.c").read_text()
    wire = (ROOT / "driver" / "spi-hid-wire-frames.h").read_text()

    # Checks below that care whether code RUNS read these. The stripping ladder
    # has climbed four rungs, each shown necessary by a leg that kept a pin
    # green while the real code was neutralised: comments (code moved into a
    # comment), `#if 0` blocks, — P2 double-blind wave — STRING LITERALS
    # (a decoy string carrying the pin's needle while the real call was
    # reverted), and — P16 wave — the macro extension of the literal-zero
    # family (`#ifdef NEVER_DEFINED_X`, `#if defined(X) && 0`, `#if <macro
    # #define'd 0>`), all in code_view(). `if (0)` decoys are refused
    # outright by check_no_dead_code_decoys(). Text pins still have a
    # ceiling: any deeper unreachable-code shape defeats them, which is why
    # the checks that CAN run as code live in headers the host tests call.
    #
    # keep_strings=True is for the few pins whose subject IS a log line's text
    # (the spi-amd peek labels, the CapImg ratelimit messages). A decoy string
    # can still satisfy those text-only pins — accepted and written down,
    # because the message text cannot live anywhere but in a string; where a
    # code-view sibling pin exists, removing the real call fails the suite.
    core_code = code_view(core)
    core_text = code_view(core, keep_strings=True)
    wire_code = code_view(wire)

    # 0a. The read-frame default must be the REFERENCE shape. The field sweep
    # that once argued for LEGACY measured the device's state, not the frame: a
    # stream-stuck device answers whatever it is handed. The five-byte form
    # cannot carry a content type or id, so a descriptor read and a feature read
    # leave the host byte-identical and the device cannot tell them apart —
    # found independently by two blind legs, and against the header's own
    # comment on the builder. The stop frame above removes the stream state that
    # made the crude form look necessary; the read shape stays at this default
    # and `hunt` sweeps the probe axes (power cycle, pre-DESCREQ preamble) instead.
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

    # 0c. Header-read lengths: spi_hid_hdr_len() is the one rule — nine
    # everywhere (field bisect 2026-09-19: doubled DESCREQ + hdr16 loops
    # forever, + hdr9 reaches DONE in 2 resets; the reference reads stream
    # headers as nine bytes on 0x0A, sixteen-byte windows caught fragments).
    # All three header sites (descriptor poller, IRQ thread, DONE poller) must
    # go through the helper; a local sixteen at any one of them re-wedges
    # discovery while the others still pass.
    _hl = core_code.split("static inline unsigned int spi_hid_hdr_len", 1)
    if len(_hl) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_hdr_len() is gone — header "
              "lengths are local magic numbers again")
        failures += 1
    else:
        _hlw = "".join(_hl[1].split("\n}", 1)[0].split())
        if "return9" not in _hlw:
            print("FAIL driver/spi-hid-core.c: spi_hid_hdr_len() no longer yields "
                  "nine everywhere — a local sixteen re-wedges discovery")
            failures += 1
    for _fn in ("static void spi_hid_seq_descreq_work",
                "spi_hid_seq_thread",
                "static void spi_hid_poll_work"):
        _win = core_code.split(_fn, 1)
        if len(_win) != 2:
            print(f"FAIL driver/spi-hid-core.c: {_fn} is gone — the header-length "
                  f"pin has nothing to hold")
            failures += 1
        elif "spi_hid_hdr_len(shid)" not in "".join(_win[1].split("\n}", 1)[0].split()):
            print(f"FAIL driver/spi-hid-core.c: {_fn} no longer reads headers through "
                  f"spi_hid_hdr_len() — a local length re-wedges one path silently")
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
    _bd = "".join(body.split())
    if "desc.input_register" not in body:
        print("FAIL driver/spi-hid-core.c: the descriptor poller no longer reads "
              "the input register")
        failures += 1
    elif "spi_hid_resp_reg(shid)" not in body:
        print("FAIL driver/spi-hid-core.c: the descriptor poller no longer tries "
              "the response register — DEVICE_DESC/RPT_DESC live on reg 3 "
              "(Windows boot trace #0003/#0006)")
        failures += 1
    # read_resp: response register first, input register as fallback.
    _rr = core_code.split("static int spi_hid_seq_read_resp(struct", 1)[1].split("\n}", 1)[0]
    if "spi_hid_resp_reg(shid)" not in _rr or "desc.input_register" not in _rr:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read_resp() lost the "
              "response-first order — descriptors live on reg 3 (Windows trace)")
        failures += 1
    # spi_hid_seq_read: standard pre-DONE follows the Windows phase map
    # (WAIT_RESET -> 0, WAIT_DESC/WAIT_RPT -> resp reg), not input-only: the
    # 9-byte TX selects the register, reg 0 never sees the desc on reg 3.
    _sr = core_code.split("static int spi_hid_seq_read(struct", 1)[1].split("\n}", 1)[0]
    _srw = "".join(_sr.split())
    if "WAIT_RESET" not in _sr or "spi_hid_resp_reg(shid)" not in _sr:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read() lost the Windows "
              "phase map — standard pre-DONE must read reg 3 for descriptors")
        failures += 1
    # raw DONE reads the stream register (probe 0x0A force is overwritten by
    # the DEVICE_DESC parse, so desc.input_register is 0 here).
    if "SPI_HID_RAW_STREAM_REGISTER" not in _sr:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read() lost the raw DONE "
              "stream register — DONE polled reg 0 until reset (2026-09-19)")
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
    if not ("SPI_HID_SEQ_WAIT_RESET" in _rd and "SPI_HID_SEQ_DONE" in _rd
            and ("output_register" in _rd or "spi_hid_resp_reg(shid)" in _rd)):
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
        ("SPI_HID_RAW_STREAM_CONTENT_ID", "the stream enable no longer names content id 0x56"),
        ("SPI_HID_RAW_STREAM_REGISTER 0x0A", "the stream register is no longer 0x0A"),
    ):
        if needle not in core_code:
            print(f"FAIL driver/spi-hid-core.c: {why} (trace #0531 / #0004-#0873)")
            failures += 1
    # The enable must go through the shared builder WITH the wire_double_opcode
    # knob, like every other sequencer write: with the knob off (default) it
    # sends the reference's frame unchanged (the builder's default form is
    # byte-pinned in wire_frames_test.c), and with the knob on the A/B
    # experiment must not silently skip this one frame — which is exactly what
    # the old literal `vendor_init(0)` did (P1 double-blind wave: both legs
    # found the knob bypassed here, and this needle used to pin the literal
    # that made it look deliberate). Scoped to the function body via rsplit
    # (the last occurrence is the definition): the handshake's identical call
    # up the file must not satisfy it.
    _en = core_code.rsplit("static int spi_hid_raw_enable_stream", 1)[1].split("\n}", 1)[0]
    if "spi_hid_wire_vendor_init(spi_hid_wire_doubled())" not in _en:
        print("FAIL driver/spi-hid-core.c: the stream enable no longer goes through "
              "the shared vendor-init builder with spi_hid_wire_doubled() — "
              "wire_double_opcode=1 would apply to the handshake and not to the enable")
        failures += 1

    # raw_fallback_on_reset (2026-09-17 triage): declared, published, and
    # consulted by the poller's RESET_RSP branch BEFORE the retry — the knob
    # restores b1f8109's give-up-to-hardcoded-fallback, the one shape the
    # field has never re-tested (b1f8109 = the last build the panel answered
    # on; the retry replaced it afterwards). Unwiring the branch silently
    # restores the endless re-drive the field ran against (H1 double-blind
    # wave: both legs landed on this branch as the loop engine). The H2
    # fix-verification leg ranked the hole signatures: reorder after the
    # retry, dropped goto, dropped ready/notify, wrong target state — all
    # four are pinned below, window-scoped to the function body and the
    # branch, so a gutted-but-needled branch cannot stay green.
    if "static bool raw_fallback_on_reset;" not in core_code:
        print("FAIL driver/spi-hid-core.c: raw_fallback_on_reset is no longer declared")
        failures += 1
    if "module_param(raw_fallback_on_reset, bool, 0444);" not in core_code:
        print("FAIL driver/spi-hid-core.c: raw_fallback_on_reset is no longer loadable")
        failures += 1
    _dw = core_code.split("static void spi_hid_seq_descreq_work", 1)
    if len(_dw) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_descreq_work is gone")
        failures += 1
    else:
        _body = _dw[1].split("\n}", 1)[0]
        _fb = _body.split("if (raw_fallback_on_reset", 1)
        if len(_fb) != 2:
            print("FAIL driver/spi-hid-core.c: the raw_fallback_on_reset branch is gone "
                  "from the poller")
            failures += 1
        else:
            # The five shape needles below must be scoped to the BRANCH ITSELF,
            # not to a fixed byte reach past its opening. The stripped code view
            # packs the RETRY path right below the branch, and that path's own
            # text already carries every one of these needles (`goto out;`,
            # `shid->ready = true;`, `sysfs_notify`, `SPI_HID_SEQ_DONE`,
            # `spi_hid_use_hardcoded_desc(shid);`). The old `_fb[1][:900]` reach
            # therefore satisfied them from code the branch never runs, so
            # dropping any one from the branch stayed green (H2 leg B, M2-M4,
            # measured). End the window at the branch's own closing line: the
            # branch opens at 2-tab indent and every interior line indents 3+
            # tabs, so the first `\n\t\t}` after the opening is that close (and
            # it is the only 2-tab `}` in the function).
            _br = _fb[1].split("\n\t\t}", 1)
            if len(_br) != 2:
                print("FAIL driver/spi-hid-core.c: the raw_fallback_on_reset branch is "
                      "not closed at its own indent — the give-up shape cannot be "
                      "scoped to the branch (H2 leg B)")
                failures += 1
            if "spi_hid_seq_restart_discovery" not in _body:
                print("FAIL driver/spi-hid-core.c: the poller's restart path is gone — "
                      "the give-up knob rides a branch that no longer has its retry")
                failures += 1
            elif _body.index("if (raw_fallback_on_reset") > \
                    _body.index("spi_hid_seq_restart_discovery"):
                print("FAIL driver/spi-hid-core.c: the raw_fallback_on_reset branch sits "
                      "AFTER the retry — the first reset always restarts and the knob "
                      "never fires on a live device (H2 leg B, M1)")
                failures += 1
            for _needle, _why in (
                ("goto out;",
                 "a fall-through into the retry clobbers DONE back to WAIT_DESC (M2)"),
                ("ready = true;",
                 "DONE with nobody woken hangs every `ready` client (M3)"),
                ("sysfs_notify",
                 "a `ready` flip nobody is notified of is invisible to clients (M3)"),
                ("SPI_HID_SEQ_DONE",
                 "the branch no longer targets DONE and never completes (M4)"),
                ("spi_hid_use_hardcoded_desc(shid);",
                 "the give-up no longer installs the fallback descriptor"),
            ):
                if len(_br) == 2 and _needle not in _br[0]:
                    print(f"FAIL driver/spi-hid-core.c: the raw_fallback_on_reset branch "
                          f"lost '{_needle}' — {_why}")
                    failures += 1

    # raw_pre_desc_reg0 (H4 double-blind falsifier, the cross-family wave's
    # delta-of-deltas #1): declared, loadable, and consulted at BOTH spots the
    # knob must reach — the probe-time stream-register force AND the raw
    # pre-DONE branch of spi_hid_seq_read(). The H4 legs gave this the
    # falsifier "one raw sweep with the probe force removed (reads reg 0
    # through WAIT_DESC) showing a first-try DEVICE_DESC and
    # stat_device_desc > 0". Removing either consultation leaves the knob
    # declared and loadable but inert: the force still points the handshake at
    # 0x0A (or the read still selects {3, 0x0A}), so the sweep is the parent
    # byte-for-byte and proves nothing. Each site is window-scoped, not a fixed
    # byte reach (the raw_fallback_on_reset lesson): the probe window is the
    # probe body up to the guard on the assignment, the read window is the raw
    # branch up to its own closing brace.
    if "static bool raw_pre_desc_reg0;" not in core_code:
        print("FAIL driver/spi-hid-core.c: raw_pre_desc_reg0 is no longer declared")
        failures += 1
    if "module_param(raw_pre_desc_reg0, bool, 0444);" not in core_code:
        print("FAIL driver/spi-hid-core.c: raw_pre_desc_reg0 is no longer loadable")
        failures += 1
    _pr0_probe = (core_code.rsplit("static int spi_hid_probe(struct spi_device *spi)", 1)[1]
                  .split("\n}", 1)[0]
                  if "static int spi_hid_probe(struct spi_device *spi)" in core_code else "")
    _pr0_force = _pr0_probe.split(
        "shid->desc.input_register = SPI_HID_RAW_STREAM_REGISTER;", 1)
    if len(_pr0_force) != 2:
        print("FAIL driver/spi-hid-core.c: the probe's raw stream-register force is gone — "
              "raw_pre_desc_reg0 has nothing to skip")
        failures += 1
    else:
        # The guard is the `if (` immediately before the assignment: a knob
        # consultation anywhere else in the (huge) probe body would not gate
        # the force. Whitespace-normalised so the exact spelling of the
        # condition does not matter, only that the knob is in it — and negated
        # (skip the force WHEN SET, spec item 1), so a polarity flip that keeps
        # the knob name but forces 0x0A whenever the knob is set is also red.
        _pr0_guard = _pr0_force[0].rsplit("if (", 1)
        _pr0_g = "".join(_pr0_guard[1].split()) if len(_pr0_guard) == 2 else ""
        if "raw_pre_desc_reg0" not in _pr0_g:
            print("FAIL driver/spi-hid-core.c: the probe stream-register force no longer "
                  "consults raw_pre_desc_reg0 — with the knob set input_register is still "
                  "forced to 0x0A and the H4 sweep can never read register 0")
            failures += 1
        elif ("!raw_pre_desc_reg0" not in _pr0_g and
              "!(raw_pre_desc_reg0" not in _pr0_g):
            print("FAIL driver/spi-hid-core.c: the probe stream-register force consults "
                  "raw_pre_desc_reg0 without negating it — the force now fires exactly when "
                  "the H4 sweep sets the knob, the opposite of spec item 1")
            failures += 1
    _pr0_rd = (core_code.split("static int spi_hid_seq_read(struct", 1)[1].split("\n}", 1)[0]
               if "static int spi_hid_seq_read(struct" in core_code else "")
    _pr0_raw = _pr0_rd.split("if (shid->raw_mode_active) {", 1)
    if len(_pr0_raw) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read()'s raw branch is gone — "
              "raw_pre_desc_reg0 has no read destination to reroute")
        failures += 1
    else:
        _pr0_body = _pr0_raw[1].split("\n\t}", 1)
        if len(_pr0_body) != 2:
            print("FAIL driver/spi-hid-core.c: the raw read branch is not closed at its own "
                  "indent — raw_pre_desc_reg0 cannot be scoped to the branch (H4)")
            failures += 1
        elif "raw_pre_desc_reg0" not in _pr0_body[0]:
            print("FAIL driver/spi-hid-core.c: the raw read branch no longer consults "
                  "raw_pre_desc_reg0 — pre-DONE reads stay on {3, 0x0A} and the H4 sweep "
                  "still points the handshake at the stream register")
            failures += 1
        elif "shid->desc.input_register" not in \
                _pr0_body[0].split("raw_pre_desc_reg0", 1)[1]:
            print("FAIL driver/spi-hid-core.c: the raw read branch consults raw_pre_desc_reg0 "
                  "but no longer routes the pre-DONE read to input_register — the register "
                  "destination is the H4 falsifier's whole subject")
            failures += 1
        elif "!raw_pre_desc_reg0" in "".join(_pr0_body[0].split()):
            print("FAIL driver/spi-hid-core.c: the raw read branch negates raw_pre_desc_reg0 — "
                  "pre-DONE would then route to input_register when the knob is OFF, inverting "
                  "spec item 1 (the default build must keep {3, 0x0A})")
            failures += 1

    # raw_b1f8109_preset (H9): the one-switch restore of the working raw
    # dialect. b1f8109 = v1.6.3, the last build the panel answered on in RAW
    # (2026-09-15 bundle: descriptors received 19x, data=6073, 26 resets; the
    # 2026-09-17 battery's single positive was wire_double_opcode=1 alone
    # delivering a DEVICE_DESC where the other 13 variants stayed at +0).
    # Declared, loadable, and CONSULTED at every site the dialect names:
    #   1. the doubled-opcode consumption (spi_hid_wire_doubled() and its
    #      SET_FEATURE sibling spi_hid_wire_doubled_setfeat()),
    #   2a. the probe-time stream-register force guard,
    #   2b. spi_hid_seq_read()'s raw pre-DONE branch,
    #   3. the poller's RESET_RSP branch (joins raw_fallback_on_reset), and
    #   4. spi_hid_vendor_init's STOP frame (skip ONLY the stop; D2/D0 stand).
    # Each window is scoped to the branch/guard that consumes the knob, never a
    # fixed byte reach (the raw_fallback_on_reset lesson): removing any one
    # consultation leaves the knob declared and loadable but inert, and the
    # default build silently stops being byte-for-byte the parent at that site.
    if "static bool raw_b1f8109_preset;" not in core_code:
        print("FAIL driver/spi-hid-core.c: raw_b1f8109_preset is no longer declared")
        failures += 1
    if "module_param(raw_b1f8109_preset, bool, 0444);" not in core_code:
        print("FAIL driver/spi-hid-core.c: raw_b1f8109_preset is no longer loadable")
        failures += 1
    # 1. Doubled opcode on every write: spi_hid_wire_doubled() must OR the
    # preset into the doubled flag (and its SET_FEATURE sibling — b1f8109's
    # sf_cmd was doubled as well).
    _bp_rd = core_code.split("static bool spi_hid_wire_doubled(void)", 1)
    if len(_bp_rd) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_wire_doubled() is gone — "
              "raw_b1f8109_preset has no doubled-flag consumption to reach")
        failures += 1
    else:
        _bp_rdb = _bp_rd[1].split("\n}", 1)[0]
        if ("wire_double_opcode" not in _bp_rdb or
                "raw_b1f8109_preset" not in _bp_rdb):
            print("FAIL driver/spi-hid-core.c: spi_hid_wire_doubled() no longer ORs "
                  "raw_b1f8109_preset with wire_double_opcode — with the preset set every "
                  "sequencer write would still go out single-opcode, not b1f8109's doubled form")
            failures += 1
        elif "||" not in "".join(_bp_rdb.split()):
            print("FAIL driver/spi-hid-core.c: spi_hid_wire_doubled() mentions the preset "
                  "without ORing it (spec item 1 is an explicit OR, not an overwrite)")
            failures += 1
    _bp_sf = core_code.split("static bool spi_hid_wire_doubled_setfeat(void)", 1)
    if len(_bp_sf) == 2 and \
            "raw_b1f8109_preset" not in _bp_sf[1].split("\n}", 1)[0]:
        print("FAIL driver/spi-hid-core.c: spi_hid_wire_doubled_setfeat() ignores "
              "raw_b1f8109_preset — b1f8109's SET_FEATURE 5 frame (sf_cmd) was doubled too")
        failures += 1
    # 2a. The probe-time stream-register force guard must name the preset
    # (negated with raw_pre_desc_reg0), so with it set the 0x0A force is
    # skipped and input_register stays 0, b1f8109's pre-DONE read destination.
    _bp_probe = (core_code.rsplit("static int spi_hid_probe(struct spi_device *spi)", 1)[1]
                 .split("\n}", 1)[0]
                 if "static int spi_hid_probe(struct spi_device *spi)" in core_code else "")
    _bp_force = _bp_probe.split(
        "shid->desc.input_register = SPI_HID_RAW_STREAM_REGISTER;", 1)
    if len(_bp_force) != 2:
        print("FAIL driver/spi-hid-core.c: the probe's raw stream-register force is gone — "
              "raw_b1f8109_preset has nothing to skip")
        failures += 1
    else:
        _bp_fg = _bp_force[0].rsplit("if (", 1)
        _bp_g = "".join(_bp_fg[1].split()) if len(_bp_fg) == 2 else ""
        if "raw_b1f8109_preset" not in _bp_g:
            print("FAIL driver/spi-hid-core.c: the probe stream-register force no longer "
                  "consults raw_b1f8109_preset — with the preset set input_register is still "
                  "forced to 0x0A, not b1f8109's register 0")
            failures += 1
        elif ("!raw_b1f8109_preset" not in _bp_g and
              "!(raw_pre_desc_reg0" not in _bp_g):
            print("FAIL driver/spi-hid-core.c: the probe force consults raw_b1f8109_preset "
                  "without negating it — the force would fire exactly when the preset is set, "
                  "inverting the dialect")
            failures += 1
    # 2b. spi_hid_seq_read()'s raw branch: pre-DONE reads must be routed to
    # input_register when the preset is set, alongside raw_pre_desc_reg0.
    _bp_rd2 = (core_code.split("static int spi_hid_seq_read(struct", 1)[1].split("\n}", 1)[0]
               if "static int spi_hid_seq_read(struct" in core_code else "")
    _bp_raw = _bp_rd2.split("if (shid->raw_mode_active) {", 1)
    if len(_bp_raw) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read()'s raw branch is gone — "
              "raw_b1f8109_preset has no read destination to reroute")
        failures += 1
    else:
        _bp_rb = _bp_raw[1].split("\n\t}", 1)
        if len(_bp_rb) != 2:
            print("FAIL driver/spi-hid-core.c: the raw read branch is not closed at its own "
                  "indent — raw_b1f8109_preset cannot be scoped to the branch")
            failures += 1
        elif "raw_b1f8109_preset" not in _bp_rb[0]:
            print("FAIL driver/spi-hid-core.c: the raw read branch no longer consults "
                  "raw_b1f8109_preset — pre-DONE reads stay on {3, 0x0A} under the preset")
            failures += 1
        elif "shid->desc.input_register" not in \
                _bp_rb[0].split("raw_b1f8109_preset", 1)[1]:
            print("FAIL driver/spi-hid-core.c: the raw read branch consults "
                  "raw_b1f8109_preset but no longer routes the read to input_register — "
                  "the register destination is the preset's whole subject")
            failures += 1
    # 3. The poller RESET_RSP branch: the preset must join raw_fallback_on_reset
    # in the branch guard (b1f8109 gave up after one reset).
    _bp_dw = core_code.split("static void spi_hid_seq_descreq_work", 1)
    if len(_bp_dw) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_descreq_work is gone — "
              "raw_b1f8109_preset has no poller branch to reach")
        failures += 1
    else:
        _bp_body = _bp_dw[1].split("\n}", 1)[0]
        if "if (raw_fallback_on_reset" not in _bp_body:
            print("FAIL driver/spi-hid-core.c: the poller's RESET_RSP branch is gone — "
                  "raw_b1f8109_preset has nothing to join")
            failures += 1
        else:
            _bp_guard = "".join(
                _bp_body.split("if (raw_fallback_on_reset", 1)[1]
                .split(") {", 1)[0].split())
            if "raw_b1f8109_preset" not in _bp_guard:
                print("FAIL driver/spi-hid-core.c: the poller's RESET_RSP branch no longer "
                      "consults raw_b1f8109_preset — with the preset set it retries forever "
                      "instead of giving up to the hardcoded fallback as b1f8109 did")
                failures += 1
    # 4. spi_hid_vendor_init skips ONLY the STOP frame under the preset: the
    # stop write must still exist (the default path sends it) AND be gated by
    # the preset, with the D2/D0 writes OUTSIDE the gate (b1f8109 sent them).
    _bp_vi = core_code.rsplit("static int spi_hid_vendor_init(struct spi_hid *shid)", 1)
    if len(_bp_vi) != 2:
        print("FAIL driver/spi-hid-core.c: spi_hid_vendor_init is gone")
        failures += 1
    else:
        _bp_vb = _bp_vi[1].split("\n}", 1)[0]
        if "stop.bytes" not in _bp_vb:
            print("FAIL driver/spi-hid-core.c: spi_hid_vendor_init no longer writes the STOP "
                  "frame — raw_b1f8109_preset has nothing to skip")
            failures += 1
        elif "raw_b1f8109_preset" not in _bp_vb:
            print("FAIL driver/spi-hid-core.c: spi_hid_vendor_init ignores "
                  "raw_b1f8109_preset — the STOP frame is never skipped under b1f8109's dialect")
            failures += 1
        elif _bp_vb.index("raw_b1f8109_preset") > _bp_vb.index("stop.bytes"):
            print("FAIL driver/spi-hid-core.c: the preset gate in spi_hid_vendor_init sits "
                  "AFTER the STOP write — it can never skip the frame b1f8109 never sent")
            failures += 1
        else:
            # Scope the gate to its own braces: the STOP write must be INSIDE it
            # and the D2/D0 writes OUTSIDE it. A gate that wraps all three is
            # skip_vendor_stop's semantics, not b1f8109's D2/D0-only dialect, and
            # the two ordering checks above would both stay green on it.
            _bp_gate = _bp_vb.split("if (!raw_b1f8109_preset) {", 1)
            if len(_bp_gate) != 2:
                print("FAIL driver/spi-hid-core.c: the preset gate in spi_hid_vendor_init is "
                      "not `if (!raw_b1f8109_preset) {` — the STOP-only skip cannot be scoped")
                failures += 1
            else:
                _bp_gb = _bp_gate[1].split("\n\t}", 1)
                if len(_bp_gb) != 2:
                    print("FAIL driver/spi-hid-core.c: the preset gate in spi_hid_vendor_init "
                          "is not closed at its own indent — the STOP-only skip cannot be scoped")
                    failures += 1
                elif "stop.bytes" not in _bp_gb[0]:
                    print("FAIL driver/spi-hid-core.c: the preset gate no longer wraps the STOP "
                          "write — nothing is skipped under the preset")
                    failures += 1
                elif "d2.bytes" in _bp_gb[0] or "d0.bytes" in _bp_gb[0]:
                    print("FAIL driver/spi-hid-core.c: the preset gate wraps D2/D0 too — that is "
                          "skip_vendor_stop's all-three semantics, not b1f8109's D2/D0-only "
                          "dialect (the preset must skip ONLY the STOP frame)")
                    failures += 1

    # 7b. sync_timeout_ms is clamped at probe into the protocol's bounds — the
    # same class as the getfeat_delay_ms clamp above it: negative wraps
    # msecs_to_jiffies() into the far future (a synchronous request waits
    # forever holding response_mutex), 0 turns every missed response into an
    # instant timeout storm, and a typo like 600000 hangs a workqueue for
    # minutes. The bound VALUES are pinned by protocol_test.c; this pins that
    # the clamp is actually applied (kernel-only code). Scoped to probe.
    _probe = core_code.rsplit("static int spi_hid_probe(struct spi_device *spi)", 1)[1].split("\n}", 1)[0]
    if "clamp_t(int, sync_timeout_ms" not in _probe:
        print("FAIL driver/spi-hid-core.c: probe no longer clamps sync_timeout_ms — "
              "a negative or zero modprobe value loses the fail-safe timeout "
              "(negative = never returns, zero = instant timeout storm)")
        failures += 1

    # 7c. The read-approval pair is written under seq_lock like every other
    # writer and reader of it (spi_hid_seq_read_reg() asserts the lock; the
    # sequencer-side writers hold it). Without the lock, this client-context
    # write in spi_hid_sync_request() races the IRQ thread's reads
    # (P1 double-blind wave, one leg). Order matters: the lock goes around the
    # response_lock section, before the send.
    # The region is selected by CONTENT, not by position (P15 wave): rsplit to
    # the last marker let a definition-shaped decoy appended below the real
    # function deflect every check here while staying green. Exactly one
    # marker segment may contain the lock; anything else is refused.
    _sr_segs = [seg.split("\n}", 1)[0] for seg in
                core_code.split("static int spi_hid_sync_request(struct spi_hid *shid")[1:]]
    _sr_regions = [seg for seg in _sr_segs if "mutex_lock(&shid->seq_lock)" in seg]
    _sr = _sr_regions[0] if len(_sr_regions) == 1 else ""
    if len(_sr_regions) != 1:
        print("FAIL driver/spi-hid-core.c: sync_request()'s seq_lock region is not "
              f"identifiable ({len(_sr_regions)} candidate regions) — a second "
              "definition-shaped occurrence deflects this check (P15 wave)")
        failures += 1
    _l = _sr.find("mutex_lock(&shid->seq_lock)")
    _w = _sr.find("shid->read_resp_type = report->content_type;")
    _u = _sr.find("mutex_unlock(&shid->seq_lock)")
    if _l < 0 or _w < 0 or _u < 0 or not (_l < _w < _u):
        print("FAIL driver/spi-hid-core.c: sync_request() no longer writes the "
              "read-approval pair inside seq_lock — the write races the "
              "sequencer's reads across threads again")
        failures += 1
    # A leg kept that order pin green while leaking the mutex: an `if (!ready)
    # goto out;` planted between lock and unlock leaves the write inside the
    # region, but the `out:` path (which drops only response_mutex) then runs
    # with seq_lock held. No exit other than the unlock may live in there —
    # and `goto` was the whole ban (P15 wave: an early `return` leaked the
    # same mutex with the pin green; return/break/continue now count too).
    if _l >= 0 and _u > _l and re.search(r"\b(?:goto|return|break|continue)\b", _sr[_l:_u]):
        print("FAIL driver/spi-hid-core.c: sync_request() can leave the seq_lock "
              "region other than through the unlock — out: does not drop seq_lock, "
              "so the mutex leaks (P2/P15 wave bypass)")
        failures += 1

    # 7d. descreq_work() holds seq_lock for its whole body; out: is the only
    # unlock. The P15 wave: the reset branch's plain `return` leaked the mutex
    # whenever it fired, and its guard ran the hardcoded fallback on a
    # SUCCESSFUL restart (the w7/w8 commits fixed exactly the opposite: the
    # fallback runs only when the restart is refused). Pin both: no plain
    # return in the body, and the restart decided by `!` with the exit through
    # out:.
    # Selected by CONTENT like 7c (P16 wave): rsplit took the LAST
    # occurrence, so a definition-shaped shadow appended below the real
    # function carried the pinned text while the real body leaked the lock
    # (in-tree proven). Exactly one marker segment may contain the lock.
    _dw_segs = [seg.split("\n}", 1)[0] for seg in
                core_code.split("static void spi_hid_seq_descreq_work")[1:]]
    _dw_regions = [seg for seg in _dw_segs if "mutex_lock(&shid->seq_lock)" in seg]
    _dw = _dw_regions[0] if len(_dw_regions) == 1 else ""
    if len(_dw_regions) != 1:
        print("FAIL driver/spi-hid-core.c: descreq_work()'s seq_lock region is not "
              f"identifiable ({len(_dw_regions)} candidate regions) — a second "
              "definition-shaped occurrence deflects this check (P16 wave)")
        failures += 1
    if re.search(r"\breturn\b", _dw):
        print("FAIL driver/spi-hid-core.c: descreq_work() exits with a plain return "
              "while holding seq_lock — out: is the only unlock, so the mutex "
              "leaks (P15 wave)")
        failures += 1
    if not re.search(
            r"if\s*\(\s*!\s*spi_hid_seq_restart_discovery\(shid,\s*SPI_HID_SEQ_RESET_RESPONSE\)\)\s*\n\s*goto out;",
            _dw):
        print("FAIL driver/spi-hid-core.c: descreq_work()'s reset branch no longer "
              "gates on the restart's REFUSAL (negated) with an exit through out: — "
              "the w7/w8 shape the P15 wave re-derived")
        failures += 1
    # P16 wave, B:C1: banning `return` was not enough — nothing required every
    # exit to reach `out`. A `goto out2;` whose `out2:` label sat after the
    # unlock jumped past it and leaked seq_lock with this check green
    # (in-tree proven). Every goto must target `out`, and `out:` must be the
    # label that unlocks.
    if any(_t != "out" for _t in re.findall(r"\bgoto\s+(\w+)\s*;", _dw)):
        print("FAIL driver/spi-hid-core.c: descreq_work() jumps to a label other "
              "than `out` while holding seq_lock — only out: drops the mutex, so "
              "any other target can leave it held (P16 wave, B:C1)")
        failures += 1
    if not re.search(r"\bout:\s*\n\s*mutex_unlock\(&shid->seq_lock\);", _dw):
        print("FAIL driver/spi-hid-core.c: descreq_work()'s `out:` no longer "
              "unlocks seq_lock directly (P16 wave, B:C1)")
        failures += 1

    # 7e. The stripper's own contract, executable: a quote at end-of-line must
    # not hand the next line to the code view (P15 wave: that bypass satisfied
    # the enable pin over reverted code).
    _decoy = 'const char *d = "\nspi_hid_wire_vendor_init(spi_hid_wire_doubled())";\n'
    if "spi_hid_wire_vendor_init" in strip_comments_and_strings(_decoy)[0]:
        print("FAIL tests/driver_source_sanity_test.py: strip_comments_and_strings() "
              "hands a newline-terminated literal's content to the code view (P15 wave)")
        failures += 1
    # P16 wave: a backslash-escaped quote must not END the literal. The P15
    # rewrite advanced by one, so everything after a `\"` reached the code
    # view — an in-tree decoy string then satisfied the 0a default pin while
    # the real default was reverted.
    _decoy2 = 'const char *d = "x\\"; spi_hid_wire_vendor_init(spi_hid_wire_doubled()); \\"";\n'
    if "spi_hid_wire_vendor_init" in strip_comments_and_strings(_decoy2)[0]:
        print("FAIL tests/driver_source_sanity_test.py: strip_comments_and_strings() "
              "ends a literal at a backslash-escaped quote — the smuggled text "
              "reaches the code view (P16 wave)")
        failures += 1
    # 7f. raw_detect_peaks()'s gate is a FULL (2R+1)^2 neighbourhood scan (P16
    # wave, A:C3): the radius-literal pin missed every shape that changes the
    # EFFECTIVE radius — a `+ 1` on either walk bound and a 4-point cross probe
    # at the pinned R stayed green on every fixture (in-tree proven). The scan
    # is static kernel-only code, so pin its shape: both walks span
    # -HEATMAP_PEAK_RADIUS..+HEATMAP_PEAK_RADIUS over the full square.
    _raw = code_view((ROOT / "driver" / "mshw0231-raw.c").read_text())
    for _needle, _why in (
            ("for (dr = -(s32)HEATMAP_PEAK_RADIUS; ok && dr <= (s32)HEATMAP_PEAK_RADIUS; dr++)",
             "the peak gate's row walk no longer spans -R..+R over the full "
             "square (a widened or narrowed bound is a different effective "
             "radius — the 4-point cross probe this replaced lived there)"),
            ("for (dc = -(s32)HEATMAP_PEAK_RADIUS; dc <= (s32)HEATMAP_PEAK_RADIUS; dc++)",
             "the peak gate's column walk no longer spans -R..+R over the full "
             "square"),
    ):
        if _needle not in _raw:
            print(f"FAIL driver/mshw0231-raw.c: {_why} (P16 wave, A:C3)")
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
    amd = code_view((ROOT / "driver" / "spi-amd.c").read_text())
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
    amd = code_view((ROOT / "driver" / "spi-amd.c").read_text())
    amd_text = code_view((ROOT / "driver" / "spi-amd.c").read_text(),
                         keep_strings=True)
    # The message-text needles read the strings-kept view; the code-view loop
    # below pins the pr_info call's own argument expressions, which a decoy
    # string cannot supply.
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
    ):
        if needle not in amd_text:
            print(f"FAIL driver/spi-amd.c: {why} — the RX offset question goes "
                  f"back to being settled by argument")
            failures += 1
    for needle, why in (
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
        if needle not in core_text:          # message text: strings-kept view
            print(f"FAIL driver/spi-hid-core.c: {needle!r} missing — a per-frame "
                  f"CapImg decode failure is back to flooding the log")
            failures += 1

    return failures


def check_no_dead_code_decoys():
    """An `if (0)` statement is dead code, and dead code is where pins go to die.

    The P2 double-blind wave reintroduced a defect behind
    `if (0) (void)spi_hid_wire_vendor_init(spi_hid_wire_doubled());` while the
    real call reverted to the literal `0` — every text pin stayed green (they
    strip comments, #if 0 and strings, but a C-level `if (0)` is none of the
    three). The P15 double-blind wave showed the exact spelling was the whole
    check: `if ((0))`, `if (0u)`, `if (0x0)`, `if (false)`, `while (0)`,
    `switch (0)` and `#if 00` each carried the needle green, valid C. The
    family below is the shapes both legs demonstrated, extended to their
    obvious spellings; the repo carries none of it (verified: zero hits), so
    a planted one IS the decoy; refuse it. The stripping ladder ends here on
    purpose: any deeper unreachable-code shape still defeats a text pin, which
    is why the checks that CAN run as code live in headers the host tests call.
    """
    failures = 0
    dead = re.compile(
        # Statement keywords with a constant-false condition. `(#` excludes
        # preprocessor directives; `\(*` tolerates wrapping parentheses; the
        # `}` before `while` excludes the `do {} while (0)` macro idiom, which
        # RUNS its body once (all three driver copies use it).
        r"(?<!#)\b(?:if|switch)\(\(*"
        r"(?:false|!1|1==0|0==1|0[xX]0*|[0]+[uUlL]*)(?:\)|&&)"
        r"|(?<!#)(?<!\})\bwhile\(\(*"
        r"(?:false|!1|1==0|0==1|0[xX]0*|[0]+[uUlL]*)(?:\)|&&)"
        # A for header whose condition is a zero literal.
        r"|(?<!#)\bfor\(;[0]+[uUlL]*;\)"
        # Constant-false preprocessor conditions: refused outright because the
        # code-view strip above is a regex too, and `#if 00` slipped past it.
        r"|#if\(*(?:0[xX]0*|[0]+[uUlL]*)\)*(?=[^0-9a-zA-Z]|$)"
    )
    for path in FILES:
        text, _ = strip_comments_and_strings(path.read_text())
        # Whitespace-free view: spacing must not defeat the family.
        if dead.search(re.sub(r"\s+", "", text)):
            print(f"FAIL {path.name}: a constant-false / dead `if (0)`-style statement "
                  f"or `#if 0`-style directive appeared — dead code that carries a pin's "
                  f"needle while the real code is reverted keeps every text pin green "
                  f"(P2/P15 waves); remove it")
            failures += 1
        # The P16 wave lifted this rung past literals: a macro-conditional
        # block that cannot be live for this tree (its symbol has no
        # `#define` anywhere in the driver tree and is not kernel-provided)
        # is refused like the literal-zero family it extends. The guard idiom
        # and anything the checker cannot prove are left alone.
        _lines = text.split("\n")
        for _idx, _ln in enumerate(_lines):
            if not re.match(r"[ \t]*#[ \t]*(?:ifdef|ifndef|if)\b", _ln):
                continue
            if re.match(r"[ \t]*#[ \t]*if[ \t]*\(*[ \t]*(?:0[xX]0*|[0]+[uUlL]*)", _ln):
                continue        # literal-zero family: refused by the regex above
            _nxt = next((l for l in _lines[_idx + 1:] if l.strip()), "")
            if _guards_itself(_ln, _nxt):
                continue
            if _directive_is_provably_false(_ln):
                print(f"FAIL {path.name}: a conditional-compilation block on a symbol "
                      f"no driver file defines ({_ln.strip()!r}) — dead code that "
                      f"carries a pin's needle while the real code is reverted keeps "
                      f"every text pin green (P16 wave); remove it")
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
        return code_view(text, keep_strings=True)

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
    failures += check_no_dead_code_decoys()
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
    core_src = code_view((ROOT / "driver" / "spi-hid-core.c").read_text())
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
