#!/usr/bin/env python3
"""Regression checks for the offline Windows post-SET timeline."""

import subprocess
import sys
from pathlib import Path
from optional_contract import skip_optional_contract


root = Path(__file__).parents[1]
tool = root / "tools" / "analyze_post_set_timeline.py"
if not tool.exists():
    skip_optional_contract("offline post-SET analyzer is not present")

result = subprocess.run(
    [sys.executable, str(tool)],
    cwd=root,
    check=True,
    text=True,
    capture_output=True,
)
rows = result.stdout.splitlines()
data = [row.split(",") for row in rows if row.startswith("data,")]

assert data[0][6] == "0x08"
assert data[-1][6] == "0x0c"
assert 36_000 < float(data[-1][1]) < 37_000
assert "set_submitter_pid=4400" in rows
assert any("write,7764." in row and "id=0x56:data=bd 0c ee 5b 44 4c" in row
           and row.endswith(",2364,2536") for row in rows)

print("post-SET timeline: PASS")
