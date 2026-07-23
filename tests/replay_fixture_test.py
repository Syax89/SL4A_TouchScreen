#!/usr/bin/env python3
"""Validate the minimal, evidence-bounded V0 replay corpus."""

from __future__ import annotations

import hashlib
import json
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "tests/fixtures/replay/v1/manifest.json"
sys.path.insert(0, str(ROOT / "tools"))
from decode_v0_capimg import decode_body


def main() -> int:
    manifest = json.loads(MANIFEST.read_text(encoding="ascii"))
    assert manifest["format"] == "sl4a-replay-manifest"
    assert manifest["version"] == 1
    assert len(manifest["raw_frames"]) == 8

    frames = {}
    for frame in manifest["raw_frames"]:
        path = ROOT / frame["path"]
        subprocess.run(["git", "ls-files", "--error-unmatch", frame["path"]],
                       cwd=ROOT, check=True, capture_output=True)
        body = path.read_bytes()
        assert len(body) == frame["bytes"]
        assert hashlib.sha256(body).hexdigest() == frame["sha256"]
        decode_body(body)
        frames[frame["id"]] = body

    for malformed in manifest["malformed_binary_inputs"]:
        body = bytearray(frames[malformed["source"]])
        mutation = malformed["mutation"]
        body[mutation["offset"]] = int(mutation["value_hex"], 16)
        body = bytes(body)
        assert hashlib.sha256(body).hexdigest() == malformed["sha256"]
        try:
            decode_body(body)
        except ValueError as error:
            assert str(error) == malformed["expected_rejection"]
        else:
            raise AssertionError(f"{malformed['id']} was accepted")

    lifecycle = {entry["category"]: entry for entry in manifest["lifecycle_fixtures"]}
    assert set(lifecycle) == {"reset", "descriptor", "activation", "gap", "malformed"}
    assert lifecycle["reset"]["evidence_status"] == "absent"
    assert lifecycle["descriptor"]["evidence_status"] == "absent"
    assert lifecycle["activation"]["evidence_status"] == "unqualified"
    assert lifecycle["gap"]["evidence_status"] == "absent"
    assert lifecycle["malformed"]["evidence_status"] == "derived"
    print("replay_fixture_test: 8 raw frames, 3 malformed inputs, 5 lifecycle records")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
