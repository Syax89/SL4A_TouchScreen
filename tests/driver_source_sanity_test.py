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


def main():
    failures = 0
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
