#!/usr/bin/env python3
"""Regression checks for the immutable Windows V0 trace corpus."""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
result = subprocess.run(
    [sys.executable, "tools/analyze_raw_corpus.py"],
    cwd=ROOT,
    check=True,
    capture_output=True,
    text=True,
)
output = json.loads(result.stdout)
report = {trace["trace"]: trace for trace in output["traces"]}

expected = {
    "surface_boot_auto.csv": (638, {"0x08": 34, "0x0c": 638}),
    "surface_init.csv": (247, {"0x08": 2, "0x0c": 247}),
    "surface_touch.csv": (537, {"0x08": 12, "0x0c": 537}),
}

expected_falsification = {
    "surface_boot_auto.csv": {
        "payload_stream_sha256": "aef0f6da2a60ea872dd2cfa0d07253bc7a581970ce4277463b1a765eeb2acd6d",
        "cadence_20ms_runs": [30, 25, 25, 21, 29, 26, 24, 25, 24, 16,
                               8, 17, 8, 61, 32, 23, 25, 19, 114, 86],
        "body_delta_ticks": [58577, 99956, 6995120],
        "header_to_body_ticks": [14225, 15145.5, 19307],
        "transitions": [[637, 895], [0, 0], [430, 144449], [0, 0],
                        [637, 5113], [0, 0]],
    },
    "surface_init.csv": {
        "payload_stream_sha256": "ebe1eed7fb69c5c3a580190febe1678952de189c07601abcd5eb7e889b5accc0",
        "cadence_20ms_runs": [92, 34, 26, 95],
        "body_delta_ticks": [61493, 99916, 61203463],
        "header_to_body_ticks": [13766, 14648, 17605],
        "transitions": [[246, 345], [0, 0], [168, 61397], [0, 0],
                        [246, 1986], [0, 0]],
    },
    "surface_touch.csv": {
        "payload_stream_sha256": "3d2122e71d0855771897dbb35fb00fc3dfc4183be8521b5dae76825109851aa2",
        "cadence_20ms_runs": [109, 129, 92, 73, 87, 47],
        "body_delta_ticks": [59733, 99955, 2094470],
        "header_to_body_ticks": [13954, 15200, 19235],
        "transitions": [[536, 748], [0, 0], [454, 63567], [0, 0],
                        [536, 4582], [0, 0]],
    },
}

expected_segments = [
    ([0, 2], 1391, 2, None),
    ([2, 25], 1, 0, "eb8a0b048e76eca1103bd43de3207c9d0e35ad2daf5288e2884e66cef89e342f"),
    ([25, 3481], 983, 3456, None),
    ([3481, 3494], 1, 0, "9066d0fb23d257fc1a14069c47945ce89a7060152c6154ba3b87d9dc8204c199"),
    ([3494, 3590], 1422, 22, None),
    ([3590, 4299], 1, 0, "c94c2c5b8a028dde2351dd714f60320f29b1e290a6784dca210f702d3ad933fd"),
]

for name, (raw_count, content_ids) in expected.items():
    trace = report[name]
    assert trace["data_body_capture"] == {"complete": sum(content_ids.values())}
    assert trace["data_content_ids"] == content_ids
    assert len(trace["raw_0c_frames"]) == raw_count
    for frame in trace["raw_0c_frames"]:
        assert frame["aligned_body_length"] == 4304
        assert frame["total_length"] == 4302
        assert frame["content_id"] == "0x0c"
        assert frame["content_length"] == 4299
        assert len(frame["tail_hex"]) == 4

assert output["falsification"]["per_trace"] == expected_falsification
assert [
    (segment["payload_range"], segment["unique_spans"],
     segment["variable_byte_offsets"], segment["static_sha256"])
    for segment in output["falsification"]["segments"]
] == expected_segments

print("raw_corpus_test: 3 traces, 1422 complete 0x0c frames")
