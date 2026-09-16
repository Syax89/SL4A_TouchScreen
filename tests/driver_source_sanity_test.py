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
