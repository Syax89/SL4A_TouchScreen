#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parent.parent
core = (root / "driver/spi-hid-core.c").read_text()
hdr = (root / "driver/spi-hid-core.h").read_text()
errors = []

def check(cond, msg):
    if not cond:
        errors.append(msg)

marker = "Opt-in standard-transport CapImg activation."
check(marker in core, "standard-transport activation block missing")
if marker in core:
    start = core.index(marker)
    end = core.find("shid->transition_done = true;", start)
    check(end > start, "cannot isolate activation block")
    if end > start:
        block = core[start:end]
        for needle, msg in [
            ("spi_hid_seq_write_get_feature6(shid)", "write-only GET6 missing"),
            ("usleep_range(4500, 5500)", "~5 ms GET6->SET5 delay missing"),
            ("spi_hid_seq_write_setfeat(shid)", "SET5 activation missing"),
        ]:
            check(needle in block, msg)
        check("spi_hid_getfeat6_read(shid)" not in block,
              "activation consumed GET6 reply")
        p1 = block.find("spi_hid_seq_write_get_feature6(shid)")
        p2 = block.find("usleep_range(4500, 5500)")
        p3 = block.find("spi_hid_seq_write_setfeat(shid)")
        check(0 <= p1 < p2 < p3, "activation ordering changed")
        check("spi_hid_std_capimg_enabled(shid)" in block,
              "activation is not gated through qualified standard CapImg policy")

helper = """\
return std_raw_transition == 1 &&
\t       raw_input_beta &&
\t       shid->std_capimg_qualified;"""
check(helper in core,
      "standard CapImg policy must require mode 1, raw_input_beta and qualification")
check('match && !strcmp(match->id, "MSHW0231")' in core,
      "hardware qualification is not pinned to exact MSHW0231 ACPI ID")
check("bool std_capimg_qualified;" in hdr,
      "per-device standard CapImg qualification state missing")

if errors:
    for e in errors:
        print("FAIL:", e)
    sys.exit(1)
print("standard CapImg activation source test: PASS")
