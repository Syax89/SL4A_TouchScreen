#!/usr/bin/env python3
"""Offline regression test for the proven V0 CapImg heatmap layout."""
import hashlib
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from decode_v0_capimg import RASTER_SAMPLE_COUNT, decode_body


def main() -> int:
    paths = sorted((ROOT / "captures").rglob("*.v0"))
    assert len(paths) == 15213
    decoded_frames = [decode_body(path.read_bytes()) for path in paths]
    assert all(len(decoded.raster) == RASTER_SAMPLE_COUNT for decoded in decoded_frames)
    assert {len(decoded.vendor_payload) for decoded in decoded_frames} == {104, 116}

    fixture = next(path for path in paths if path.parent.name == "raw-20260716-one-shot-baseline")
    body = fixture.read_bytes()
    decoded = decode_body(body)
    assert len(decoded.raster) == RASTER_SAMPLE_COUNT
    assert decoded.raster == body[28:28 + RASTER_SAMPLE_COUNT]
    assert len(decoded.vendor_payload) == 104
    assert hashlib.sha256(decoded.raster).hexdigest() == hashlib.sha256(body[28:3484]).hexdigest()

    def rejects(mutator, message):
        malformed = bytearray(body)
        mutator(malformed)
        try:
            decode_body(malformed)
        except ValueError:
            return
        raise AssertionError(message)

    rejects(lambda frame: frame.__setitem__(19, 7), "invalid RLV width was accepted")
    rejects(lambda frame: frame.__setitem__(20, 1), "nonzero RLV destination was accepted")
    rejects(lambda frame: frame.__setitem__(12, 6), "undersized section was accepted")
    rejects(lambda frame: frame.__setitem__(3489, 0), "missing vendor section was accepted")
    print("v0_capimg_decoder_test: 15213 proven 0x100 RLV frames passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
