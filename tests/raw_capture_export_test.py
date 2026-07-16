#!/usr/bin/env python3
"""Offline contract tests for raw_capture binary slot validation."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parents[1] / "tools"))
from extract_raw_capture import BODY_LENGTH, valid


def main() -> int:
    body = bytearray(BODY_LENGTH)
    body[:3] = b"\xce\x10\x0c"
    assert valid(body)

    body[2] = 0x08
    assert not valid(body)
    body[2] = 0x0c
    assert not valid(body[:-1])
    body[0] = 0xcd
    assert not valid(body)

    print("raw_capture_export_test: slot boundary validation passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
