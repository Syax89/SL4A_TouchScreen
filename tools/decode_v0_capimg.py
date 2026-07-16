#!/usr/bin/env python3
"""Decode the proven CapImg heatmap section from retained V0 0x0c bodies.

Offline only: this reads capture files and never opens a device node.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from dataclasses import dataclass
from pathlib import Path


BODY_LENGTH = 4304
USAGE_61_OFFSET = 5
USAGE_61_LENGTH = 4297
SECTION_HEADER_SIZE = 7
HEATMAP_SECTION_TYPE = 0x100
VENDOR_SECTION_TYPE = 0xFF00
RASTER_SAMPLE_COUNT = 3456


@dataclass(frozen=True)
class DecodedFrame:
    raster: bytes
    vendor_payload: bytes


def _section_at(payload: bytes, offset: int, limit: int) -> tuple[int, int, int]:
    if offset + SECTION_HEADER_SIZE > limit:
        raise ValueError("truncated CapImg section header")
    length, section_type = struct.unpack_from("<IH", payload, offset)
    if length < SECTION_HEADER_SIZE or offset + length > limit:
        raise ValueError("invalid CapImg section length")
    return length, section_type, payload[offset + 6]


def decode_body(body: bytes) -> DecodedFrame:
    """Return the single-run, 8-bit heatmap and vendor payload from one V0 body."""
    if len(body) != BODY_LENGTH or body[:3] != b"\xce\x10\x0c":
        raise ValueError("expected a complete V0 0x0c body")

    payload = body[USAGE_61_OFFSET:USAGE_61_OFFSET + USAGE_61_LENGTH]
    container_length, root_type = struct.unpack_from("<IH", payload)
    if root_type != 0 or payload[6] != 0:
        raise ValueError("unexpected CapImg container header")
    if container_length > len(payload) or container_length < SECTION_HEADER_SIZE:
        raise ValueError("invalid CapImg container length")

    heatmap = vendor = None
    offset = SECTION_HEADER_SIZE
    while offset < container_length:
        length, section_type, flags = _section_at(payload, offset, container_length)
        if section_type == HEATMAP_SECTION_TYPE:
            if heatmap is not None:
                raise ValueError("duplicate heatmap section")
            if flags != 1 or length < 16 or payload[offset + 7] != 8:
                raise ValueError("unsupported heatmap encoding")
            destination, run_length = struct.unpack_from("<II", payload, offset + 8)
            if destination != 0 or run_length != RASTER_SAMPLE_COUNT or length != 16 + run_length:
                raise ValueError("unsupported heatmap RLV runs")
            heatmap = payload[offset + 16:offset + 16 + run_length]
        elif section_type == VENDOR_SECTION_TYPE:
            if vendor is not None:
                raise ValueError("duplicate vendor section")
            vendor = payload[offset + SECTION_HEADER_SIZE:offset + length]
        offset += length

    if offset != container_length or heatmap is None or vendor is None:
        raise ValueError("incomplete CapImg container")
    return DecodedFrame(raster=heatmap, vendor_payload=vendor)


def _paths(inputs: list[Path]) -> list[Path]:
    paths = []
    for input_path in inputs:
        paths.extend(sorted(input_path.rglob("*.v0")) if input_path.is_dir() else [input_path])
    return paths


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", nargs="+", type=Path, help=".v0 files or directories")
    args = parser.parse_args()
    frames = []
    for path in _paths(args.input):
        decoded = decode_body(path.read_bytes())
        frames.append({
            "path": str(path),
            "raster_sha256": hashlib.sha256(decoded.raster).hexdigest(),
            "raster_bytes": len(decoded.raster),
            "vendor_bytes": len(decoded.vendor_payload),
        })
    print(json.dumps({"frames": frames, "count": len(frames)}, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
