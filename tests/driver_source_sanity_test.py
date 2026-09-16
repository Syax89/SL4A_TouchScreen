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

    # 1. spi_hid_ll_parse(): the mutex_unlock must not be the body of an `else`.
    # A dangling `else` had put the unlock in the success branch only, so a
    # failed hardcoded-descriptor parse returned with shid->lock held — and
    # every later lock taker (IRQ thread, sysfs readers, remove) waits forever.
    body = core.split("static int spi_hid_ll_parse", 1)[1].split("\n}", 1)[0]
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
    body = core.split("spi_hid_seq_set_state(struct spi_hid *shid", 1)[1].split("\n}", 1)[0]
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
    body = core.split("static int spi_hid_resume", 1)[1].split("\n}", 1)[0]
    if "raw_handshake_watchdog" not in body:
        print("FAIL driver/spi-hid-core.c: spi_hid_resume() no longer arms the raw "
              "watchdog (raw mode has no timer for a silent post-resume controller)")
        failures += 1

    # 4. spi_hid_raw_handshake_watchdog(): its "fall back to standard HID" branch
    # must install the hardcoded descriptors first. Without them
    # spi_hid_create_device_work() sees version 0, refuses to publish the device
    # and schedules the ACPI power cycle — the field case where the raw handshake
    # failed and the fallback left the panel deader than before.
    body = core.split("static void spi_hid_raw_handshake_watchdog", 1)[1].split("\n}", 1)[0]
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
    if "spi_hid_seq_read_resp" not in core:
        print("FAIL driver/spi-hid-core.c: spi_hid_seq_read_resp() is gone; the "
              "descriptor bodies would only ever be read from the input register")
        failures += 1
    body = core.split("static void spi_hid_seq_descreq_work", 1)[1].split("\n}", 1)[0]
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
    read_reg = core.split("static int spi_hid_seq_read_reg", 1)[1].split("\n}", 1)[0]
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
        if needle not in core:
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
        seg = core.rsplit(marker, 1)[1][:900] if marker in core else ""
        if want not in seg:
            print(f"FAIL driver/spi-hid-core.c: spi_hid_seq_write_{fn}() does not "
                  f"record the request its response belongs to — its reads go out "
                  f"naming nothing")
            failures += 1
    if "shid->desc.max_input_length = 0x2000;" not in core:
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
    amd = (ROOT / "driver" / "spi-amd.c").read_text()
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
    amd = (ROOT / "driver" / "spi-amd.c").read_text()
    for needle, why in (
        ("TRACE peek tx_len=", "the read-path region peek is gone"),
        ("0x84=[%*ph]", "the fixed-0x84 candidate is no longer logged"),
        ("0x89=[%*ph]", "the tx_len+1 candidate is no longer logged"),
    ):
        if needle not in amd:
            print(f"FAIL driver/spi-amd.c: {why} — the RX offset question goes "
                  f"back to being settled by argument")
            failures += 1

    return failures


def main():
    failures = check_control_flow_pins()
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
    if failures:
        print(f"driver source sanity: {failures} failure(s)")
        return 1
    print(f"driver source sanity: PASS ({len(FILES)} files)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
