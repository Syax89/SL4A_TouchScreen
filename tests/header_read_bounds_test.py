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

check(core.count("WARN_ON_ONCE(hdr_len > sizeof(hdr))") == 3,
      "expected bounds guards at all three fixed hdr[16] call sites")

m = re.search(
    r"static inline unsigned int spi_hid_hdr_len\(struct spi_hid \*shid\)"
    r"\s*\{(?P<body>.*?)\n\}",
    core,
    re.S,
)
check(m is not None, "could not locate spi_hid_hdr_len()")
if m:
    returns = [int(x) for x in re.findall(r"return\s+(\d+)\s*;", m.group("body"))]
    check(bool(returns), "spi_hid_hdr_len() has no statically inspectable numeric return")
    check(not returns or max(returns) <= 16,
          "spi_hid_hdr_len() can exceed the fixed 16-byte stack buffers")

for fn, end_marker in [
    ("static void spi_hid_seq_descreq_work", "static void spi_hid_poll_work"),
    ("static void spi_hid_poll_work", "static void spi_hid_stream_watchdog_work"),
    ("static irqreturn_t spi_hid_seq_thread", "static void spi_hid_create_device"),
]:
    start = core.find(fn)
    end = core.find(end_marker, start + 1)
    check(start >= 0 and end > start, f"cannot isolate {fn}")
    if start >= 0 and end > start:
        block = core[start:end]
        guard = block.find("WARN_ON_ONCE(hdr_len > sizeof(hdr))")
        read = block.find("spi_hid_seq_read(shid, hdr, hdr_len)")
        check(guard >= 0, f"{fn} lacks hdr[] bounds guard")
        check(read < 0 or guard < read, f"{fn} checks hdr[] only after the read")

if errors:
    for e in errors:
        print("FAIL:", e)
    sys.exit(1)

print("header read bounds source test: PASS")
