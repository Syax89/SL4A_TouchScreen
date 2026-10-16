#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parent.parent
core = (root / "driver/spi-hid-core.c").read_text()
raw = (root / "driver/mshw0231-raw.c").read_text()
errors = []

def check(cond, msg):
    if not cond:
        errors.append(msg)

probe_marker = "Create the heatmap-backed MT input device only when it can receive"
check(probe_marker in core, "standard-transport input registration block missing")
if probe_marker in core:
    p = core.index(probe_marker)
    block = core[p:core.find("mshw0231_raw_init(shid);", p)]
    check("spi_hid_std_capimg_enabled(shid)" in block,
          "probe does not use qualified standard CapImg policy")
    check("mshw0231_raw_input_register(shid)" in block,
          "tracker registration call missing")

data_marker = "body[7] == 0x0C && shid->touch_input"
check(data_marker in core, "CapImg consume gate missing")
if data_marker in core:
    p = core.index(data_marker)
    block = core[max(0, p - 300):p + 100]
    check("spi_hid_std_capimg_enabled(shid)" in block,
          "standard CapImg frames do not use qualified policy")
    check("shid->raw_mode_active" in block,
          "raw-mode tracker path was lost")

check("if (!shid->raw_mode_active)\n\t\treturn 0;" not in raw,
      "raw input helper still rejects standard transport")
check("The caller decides whether the heatmap-backed MT device is needed." in raw,
      "caller-owned registration contract missing")

if errors:
    for e in errors:
        print("FAIL:", e)
    sys.exit(1)
print("standard CapImg input source test: PASS")
