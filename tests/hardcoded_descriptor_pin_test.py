#!/usr/bin/env python3
"""Pin the hardcoded report descriptor against the device's own capture.

`hardcoded_rd.h` is what the driver hands HID when the wire descriptor read
fails; its 936 bytes are byte-identical to the descriptor the device served in
the surface_init capture. No C test includes that header (the host suite only
string-greps core.c), so a corrupted array would pass every host test and
surface only on a device. This reads BOTH sides from the repository: the array
from the header, the descriptor from the capture's own RX row (five-byte
preamble, `ab 03 00` content header, 936 data bytes, one pad byte).
"""
from __future__ import annotations

import csv
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HARDCODED_RD = ROOT / "driver" / "hardcoded_rd.h"
CAPTURE = ROOT / "captures" / "wintrace" / "surface_init.csv"

RD_BYTES = 936
FRAME_BYTES = 945  # 5 preamble + 3 content header + 936 data + 1 pad


def array_bytes() -> bytes:
    text = HARDCODED_RD.read_text()
    m = re.search(r"#define HARDCODED_RD_SIZE (\d+)", text)
    assert m is not None, "HARDCODED_RD_SIZE not found in hardcoded_rd.h"
    size = int(m.group(1))
    body = text[text.index("{") + 1:text.index("}")]
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    data = bytes(int(tok, 16) for tok in re.findall(r"0x([0-9a-fA-F]{2})", body))
    assert size == len(data) == RD_BYTES, (size, len(data))
    return data


def captured_descriptor() -> bytes:
    with CAPTURE.open(encoding="latin-1") as f:
        for row in csv.reader(f):
            if len(row) > 21 and row[1].strip() == "IoSpbPayloadTdBuffer":
                h = row[21].strip().strip('"').strip()
                if h.lower().startswith("0x"):
                    h = h[2:]
                if len(h) == FRAME_BYTES * 2 and h.lower().startswith("ffffffffffab0300"):
                    return bytes.fromhex(h[16:16 + RD_BYTES * 2])
    raise AssertionError("no report-descriptor response row found in the capture")


def main() -> int:
    want = captured_descriptor()
    got = array_bytes()
    if got != want:
        off = next(i for i, (a, b) in enumerate(zip(got, want)) if a != b)
        raise AssertionError(
            f"hardcoded_rd.h diverges from the captured descriptor at offset {off}: "
            f"header 0x{got[off]:02x} vs capture 0x{want[off]:02x}")
    print("hardcoded_descriptor_pin_test: PASS "
          f"({RD_BYTES} bytes identical to the surface_init capture)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
