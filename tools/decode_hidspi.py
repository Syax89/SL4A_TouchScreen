#!/usr/bin/env python3
"""Decode HID-over-SPI byte sequences found in Windows/Linux traces."""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass


@dataclass(frozen=True)
class HidSpiHeader:
    offset: int
    raw: bytes
    report_type: int
    version: int
    body_len: int


def parse_hex(s: str) -> bytes:
    s = s.strip()
    if s.startswith("0x") or s.startswith("0X"):
        s = s[2:]
    s = re.sub(r"[^0-9a-fA-F]", "", s)
    if len(s) % 2:
        raise ValueError("hex string has an odd number of digits")
    return bytes.fromhex(s)


def find_header(data: bytes) -> HidSpiHeader | None:
    for off in range(0, max(0, len(data) - 3)):
        b0, b1, b2, b3 = data[off : off + 4]
        if b3 != 0x5A:
            continue
        if (b0 & 0x0F) != 2:
            continue
        if b1 & 0x0F:
            continue
        body_len = ((b1 >> 4) | (b2 << 4)) * 4
        return HidSpiHeader(
            offset=off,
            raw=data[off : off + 4],
            report_type=(b0 >> 4) & 0x0F,
            version=b0 & 0x0F,
            body_len=body_len,
        )
    return None


def describe(data: bytes) -> str:
    header = find_header(data)
    if header:
        return (
            f"hdr_off={header.offset} type={header.report_type} "
            f"ver={header.version} body_len={header.body_len} "
            f"hdr={header.raw.hex(' ').upper()}"
        )
    if len(data) >= 4 and data[:4] == b"\x03\x00\x00\x00":
        return "ack_or_reset_body=03 00 00 00"
    if len(data) >= 9 and data[5:9] == b"\x03\x00\x00\x00":
        return "sync_ack_or_reset_body=FFx5 + 03 00 00 00"
    return "no_hidspi_header"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("hex", nargs="+", help="hex bytes, with or without 0x prefix")
    args = parser.parse_args()

    data = parse_hex("".join(args.hex))
    print(describe(data))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
