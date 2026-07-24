#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""
Generates the synthetic 72x48 heatmap frame fixtures under this
directory, consumed by tests/raw_pipeline_replay_test.c to replay real
frames through the real driver/mshw0231-raw.c pipeline on host.

Byte format (see README.md for the full description): each fixture is
a flat 3456-byte file, 72 columns x 48 rows, row-major (index = row*72
+ col), one byte per grid cell. Resting/no-touch cells sit around
0xb4-0xb5; a touch LOWERS the byte value at and around the contact
point (the driver's c590 lookup table is a decreasing function of the
byte index, so a lower byte maps to a higher post-baseline "signal
rise", which is what the pipeline treats as touch strength).

Deterministic and reproducible: every random-looking value here (the
baseline dither) is derived from a fixed integer hash of (frame index,
cell index), not from Python's `random` module, so the exact same
bytes are produced on any Python version/platform. Re-run this script
to regenerate the fixtures byte-for-byte, or copy the stamp_blob()/
hash_noise() helpers to add a new one.

Usage: python3 generate_fixtures.py
"""
import os

COLS = 72
ROWS = 48
CELLS = COLS * ROWS
BASELINE = 0xB4          # resting raw byte, matches real hardware idle frames
CENTER_DIP = 0x35         # 53 decimal: baseline 0xB4 -> center 0x7F, matching
                          # a real confirmed-working single-touch capture

OUT_DIR = os.path.dirname(os.path.abspath(__file__))

# Relative dip fractions for a small, roughly circular blob footprint,
# tapering from the contact center out to grid distance 2. This keeps
# each touch a compact ~13-cell blob (>= HEATMAP_MIN_BLOB_PIXELS=2,
# well inside the HEATMAP_PEAK_RADIUS=5 cross-check radius) rather
# than a single hot pixel, matching what a real fingertip footprint
# looks like on this sensor.
BLOB_SHAPE = {
    (0, 0): 1.00,
    (1, 0): 0.75, (-1, 0): 0.75, (0, 1): 0.75, (0, -1): 0.75,
    (1, 1): 0.55, (1, -1): 0.55, (-1, 1): 0.55, (-1, -1): 0.55,
    (2, 0): 0.25, (-2, 0): 0.25, (0, 2): 0.25, (0, -2): 0.25,
    (2, 1): 0.12, (2, -1): 0.12, (-2, 1): 0.12, (-2, -1): 0.12,
    (1, 2): 0.12, (1, -2): 0.12, (-1, 2): 0.12, (-1, -2): 0.12,
}


def hash_noise(frame_idx, cell_idx):
    """Deterministic pseudo-noise in {-1, 0, +1}, derived from a fixed
    integer hash of (frame_idx, cell_idx). Not Python's random module,
    so results never depend on interpreter/version-specific PRNG
    behaviour -- re-running this script always regenerates identical
    bytes."""
    h = (cell_idx * 1103515245 + frame_idx * 12345 + 12345) & 0xFFFFFFFF
    return (h >> 16) % 3 - 1  # -> -1, 0, or +1


def new_baseline_frame(frame_idx, with_noise=True):
    frame = bytearray(CELLS)
    for i in range(CELLS):
        v = BASELINE
        if with_noise:
            v += hash_noise(frame_idx, i)
        frame[i] = max(0, min(255, v))
    return frame


def stamp_blob(frame, cx, cy, center_dip=CENTER_DIP):
    """Stamp one finger-shaped dip centered at grid (cx, cy). Uses
    min() so overlapping blobs (close-together multi-touch fixtures)
    combine as "strongest touch wins" rather than overwriting."""
    for (dx, dy), frac in BLOB_SHAPE.items():
        x, y = cx + dx, cy + dy
        if 0 <= x < COLS and 0 <= y < ROWS:
            idx = y * COLS + x
            dip = round(center_dip * frac)
            val = max(0, BASELINE - dip)
            frame[idx] = min(frame[idx], val)


def write_frame(name, frame):
    path = os.path.join(OUT_DIR, name)
    assert len(frame) == CELLS, f"{name}: expected {CELLS} bytes, got {len(frame)}"
    with open(path, "wb") as f:
        f.write(bytes(frame))
    print(f"wrote {name} ({len(frame)} bytes)")


def main():
    # ── 30 baseline/resting frames ──────────────────────────────
    # HEATMAP_BASELINE_FRAMES=30: the driver needs exactly this many
    # content_id==0x0C frames with >=1000 cells before it considers
    # the baseline established.
    for i in range(30):
        write_frame(f"baseline_{i:02d}.bin", new_baseline_frame(i))

    # ── Single well-isolated 1-finger touch ─────────────────────
    f = new_baseline_frame(100, with_noise=False)
    stamp_blob(f, 20, 15)
    write_frame("touch_1finger.bin", f)

    # ── 2-finger, far apart (opposite corners) ──────────────────
    f = new_baseline_frame(101, with_noise=False)
    stamp_blob(f, 5, 5)
    stamp_blob(f, 65, 42)
    write_frame("touch_2finger_far.bin", f)

    # ── 2-finger, close together (~9 cells, adjacent-finger spread) ──
    f = new_baseline_frame(102, with_noise=False)
    stamp_blob(f, 30, 20)
    stamp_blob(f, 38, 24)  # dx=8, dy=4 -> ~8.9 cells apart
    write_frame("touch_2finger_close.bin", f)

    # ── 3-finger ──────────────────────────────────────────────
    f = new_baseline_frame(103, with_noise=False)
    for (cx, cy) in [(15, 10), (36, 24), (57, 38)]:
        stamp_blob(f, cx, cy)
    write_frame("touch_3finger.bin", f)

    # ── 4-finger ──────────────────────────────────────────────
    f = new_baseline_frame(104, with_noise=False)
    for (cx, cy) in [(10, 8), (28, 20), (46, 32), (64, 44)]:
        stamp_blob(f, cx, cy)
    write_frame("touch_4finger.bin", f)

    # ── 5-finger ──────────────────────────────────────────────
    f = new_baseline_frame(105, with_noise=False)
    for (cx, cy) in [(8, 6), (22, 15), (36, 24), (50, 33), (64, 42)]:
        stamp_blob(f, cx, cy)
    write_frame("touch_5finger.bin", f)

    # ── Hold-and-move sequence: single blob drifting diagonally ──
    # 16 frames, +1 col / +1 row every frame starting at (10,10).
    for i in range(16):
        f = new_baseline_frame(200 + i, with_noise=False)
        stamp_blob(f, 10 + i, 10 + i)
        write_frame(f"move_{i:02d}.bin", f)


if __name__ == "__main__":
    main()
