#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parent.parent
core = (root / "driver/spi-hid-core.c").read_text()
errors = []

def check(cond, msg):
    if not cond:
        errors.append(msg)

start = core.find("static void seq_handle_data(struct spi_hid *shid, int type, u16 blen)\n{")
end = core.find("\nstatic irqreturn_t spi_hid_dev_irq", start)
check(start >= 0 and end > start, "cannot isolate seq_handle_data")
if start >= 0 and end > start:
    block = core[start:end]
    check("if (shid->hid_creating ||" not in block,
          "DATA still returns before body drain during HID creation")
    check("!shid->hid && !shid->hid_creating" in block,
          "early DATA drain eligibility missing")
    drain_pos = block.find("spi_hid_seq_read(shid, body, rblen)")
    publish_pos = block.find("if (shid->hid && !shid->hid_creating) {")
    check(drain_pos >= 0, "DATA body read missing")
    check(publish_pos > drain_pos,
          "HID publication must remain suppressed until after drain")

if errors:
    for e in errors:
        print("FAIL:", e)
    sys.exit(1)
print("standard CapImg DATA-drain source test: PASS")
