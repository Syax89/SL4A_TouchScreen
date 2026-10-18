#!/usr/bin/env python3
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent.parent
core = (ROOT / "driver" / "spi-hid-core.c").read_text()
errors = []

def check(cond, msg):
    if not cond:
        errors.append(msg)

def extract_function(signature):
    start = core.find(signature)
    if start < 0:
        return None
    brace = core.find("{", start)
    if brace < 0:
        return None
    depth = 0
    for i in range(brace, len(core)):
        if core[i] == "{":
            depth += 1
        elif core[i] == "}":
            depth -= 1
            if depth == 0:
                return core[start:i + 1]
    return None

def guard_before_read(block, read_needle):
    if block is None:
        return False
    guard = block.find("WARN_ON_ONCE(hdr_len > sizeof(hdr))")
    read = block.find(read_needle)
    return guard >= 0 and read >= 0 and guard < read

check(core.count("WARN_ON_ONCE(hdr_len > sizeof(hdr))") == 3,
      "expected bounds guards at all three fixed hdr[16] call sites")

helper = extract_function(
    "static inline unsigned int spi_hid_hdr_len(struct spi_hid *shid)"
)
check(helper is not None, "could not isolate spi_hid_hdr_len()")
if helper:
    returns = [int(x) for x in re.findall(r"return\s+(\d+)\s*;", helper)]
    check(bool(returns),
          "spi_hid_hdr_len() has no statically inspectable numeric return")
    check(not returns or max(returns) <= 16,
          "spi_hid_hdr_len() can exceed the fixed 16-byte stack buffers")

cases = [
    (
        "static void spi_hid_seq_descreq_work",
        "spi_hid_seq_read_reg(shid, reg, hdr, hdr_len)",
    ),
    (
        "static void spi_hid_poll_work",
        "spi_hid_seq_read(shid, hdr, hdr_len)",
    ),
    (
        "static irqreturn_t spi_hid_seq_thread",
        "spi_hid_seq_read(shid, hdr, hdr_len)",
    ),
]

for signature, read_needle in cases:
    block = extract_function(signature)
    check(block is not None, f"cannot isolate {signature}")
    if block is None:
        continue
    check(read_needle in block,
          f"{signature} does not contain expected read call {read_needle}")
    check(guard_before_read(block, read_needle),
          f"{signature} does not guard hdr[] before its actual read")

    # Mutation check: move only the guard marker after the real read and make
    # sure the ordering predicate rejects it. This proves a missing/late guard
    # cannot pass merely because the expected read call was not found.
    guard_marker = "WARN_ON_ONCE(hdr_len > sizeof(hdr))"
    mutant = block.replace(guard_marker, "/* guard moved */", 1)
    mutant = mutant.replace(
        read_needle,
        read_needle + "\n\t/* mutation */ " + guard_marker,
        1,
    )
    check(not guard_before_read(mutant, read_needle),
          f"{signature} negative mutation (guard after read) was not rejected")

if errors:
    for e in errors:
        print("FAIL:", e)
    sys.exit(1)

print("header read bounds source test: PASS")
