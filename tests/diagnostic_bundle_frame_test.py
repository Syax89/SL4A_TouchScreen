#!/usr/bin/env python3
"""Run the bundle's frame section for real, against a fake sysfs tree.

The contract test only greps for strings; this one slices the actual section out
of tools/sl4a-touch.sh, runs it with bash against a temp device directory and
checks the behaviour: the hex round-trips to the frame byte for byte, the
heatmap_debug fallback is labelled as truncated, an empty attribute says so
instead of claiming "0 bytes, complete", and a missing attribute is explicit.
That last pair is the common report (driver bound, touchscreen in standard mode,
no CapImg frame ever streamed) — if it regresses, the bundle lies to whoever
reads it.

Needs only bash, python3 and coreutils, so it runs in the host-tests job.

Run: python3 tests/diagnostic_bundle_frame_test.py
"""
import pathlib
import random
import re
import subprocess
import sys
import tempfile

REPO = pathlib.Path(__file__).resolve().parents[1]
tool = (REPO / "tools" / "sl4a-touch.sh").read_text()

START = "\t\t\t\t\t# Frame data, so a report can be analysed"
END = "\t\t\t\telse\n\t\t\t\t\techo \"(spi-${ts_id}:00 sysfs node not found"
section = tool[tool.index(START):tool.index(END)]
assert "--- Last captured frame ---" in section, "section slice looks wrong"


def run(spidev: pathlib.Path) -> str:
    # The section lives inside cmd_logs(), so give the slice the same context:
    # it uses `local` for its temporaries.
    script = f"set -e\nspidev={spidev}\n_f() {{\n" + section + "\n}\n_f\n"
    out = subprocess.run(["bash", "-c", script], capture_output=True, text=True)
    assert out.returncode == 0, out.stderr
    return out.stdout


with tempfile.TemporaryDirectory() as tmp:
    spidev = pathlib.Path(tmp) / "spi-MSHW0231:00"
    spidev.mkdir()
    frame = bytes(random.randrange(256) for _ in range(3456))  # 72x48 cell field
    (spidev / "heatmap_raw").write_bytes(frame)
    (spidev / "heatmap_debug").write_text("content_id=0x0c len=3456 cells=3454 grid=72x48\n")

    text = run(spidev)
    assert "-- heatmap_raw (3456 bytes, cell field complete) --" in text, text[:400]
    hexbytes = bytes.fromhex(" ".join(re.findall(r"\b[0-9a-f]{2}\b", text.split("-- heatmap_raw")[1])))
    assert hexbytes == frame, f"round trip mismatch: {len(hexbytes)} != {len(frame)}"
    print(f"complete frame: {len(hexbytes)}/{len(frame)} bytes round-tripped")

    # the file is rewritten at frame rate: the count must come from the same
    # read as the bytes, so a shorter frame cannot be labelled with an old size
    (spidev / "heatmap_raw").write_bytes(frame[:100])
    text = run(spidev)
    assert "-- heatmap_raw (100 bytes, cell field complete) --" in text, text[:300]
    print("count follows the read that produced the bytes")

    # empty attribute (driver bound, no frame ever captured): must not read as
    # "0 bytes, complete"
    (spidev / "heatmap_raw").write_bytes(b"")
    text = run(spidev)
    assert "no frame data" in text, text[:300]
    assert "0 bytes, cell field complete" not in text, text[:300]
    print("empty frame: explicit, not '0 bytes, complete'")

    # older module: no heatmap_raw at all -> labelled truncated hex view
    (spidev / "heatmap_raw").unlink()
    text = run(spidev)
    assert "truncated to one page" in text and "content_id=0x0c len=3456" in text, text[:300]
    print("older module: labelled truncated hex view")

    # nothing readable at all
    (spidev / "heatmap_debug").unlink()
    text = run(spidev)
    assert "no frame attribute" in text, text[:300]
    print("no attribute: explicit message")

print("diagnostic bundle frame section behaviour: PASS")
sys.exit(0)
