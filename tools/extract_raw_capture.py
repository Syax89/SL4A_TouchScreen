#!/usr/bin/env python3
"""Validate and extract fixed slots from the passive raw_capture sysfs file."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path


BODY_LENGTH = 4304
TOTAL_LENGTH = 4302
CONTENT_ID = 0x0C
SLOTS = 8


def valid(body: bytes) -> bool:
    return (len(body) == BODY_LENGTH and body[0:2] == b"\xce\x10" and
            body[2] == CONTENT_ID)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dump", type=Path, help="binary raw_capture sysfs snapshot")
    parser.add_argument("--output", type=Path,
                        help="directory for validated complete V0 bodies")
    args = parser.parse_args()

    dump = args.dump.read_bytes()
    if len(dump) != SLOTS * BODY_LENGTH:
        raise ValueError(f"expected {SLOTS * BODY_LENGTH} bytes, got {len(dump)}")

    report = []
    if args.output:
        args.output.mkdir(parents=True, exist_ok=True)
    for slot in range(SLOTS):
        body = dump[slot * BODY_LENGTH:(slot + 1) * BODY_LENGTH]
        accepted = valid(body)
        item = {
            "slot": slot,
            "valid": accepted,
            "body_sha256": hashlib.sha256(body).hexdigest(),
        }
        if accepted:
            item["content_sha256"] = hashlib.sha256(body[3:TOTAL_LENGTH]).hexdigest()
            item["tail_hex"] = body[TOTAL_LENGTH:].hex()
            if args.output:
                (args.output / f"raw-slot-{slot}.v0").write_bytes(body)
        report.append(item)
    print(json.dumps({"slots": report}, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
