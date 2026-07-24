# raw-replay fixtures

Synthetic heatmap frames for `tests/raw_pipeline_replay_test.c`, which
feeds them through the **real** `driver/mshw0231-raw.c` pipeline
(compiled and linked for host, see `tests/linux-compat/raw-pipeline-stubs/`)
via `mshw0231_raw_consume_samples()`, exactly as the kernel driver
would on a live device.

## Byte format

Each `.bin` file is a flat **3456-byte** raw frame:

- 72 columns x 48 rows, **row-major** (`index = row * 72 + col`).
- One byte per grid cell.
- Resting/no-touch cells sit around **0xB4-0xB5**.
- A touch **lowers** the byte value at and around the contact point.
  The driver's `c590` lookup table (`shid->c590_lut`, built in
  `mshw0231_raw_init()`) is a *decreasing* function of the raw byte,
  so a lower byte maps to a higher signal after baseline subtraction
  — that's what the pipeline treats as "touch strength" (see
  `raw_compute_signal()` in `driver/mshw0231-raw.c`).

This mirrors the real wire format: `mshw0231_raw_consume_samples(shid,
samples, sample_count, content_id)` is called with `sample_count`
bytes starting at `dfa_data_offset` (module param, defaults to 0), and
with the default 72x48 grid and offset 0, `sample_count` is exactly
3456 bytes = the whole file.

## Regenerating / adding fixtures

Run `python3 generate_fixtures.py` from this directory. It is fully
deterministic: baseline dither uses a fixed integer hash of `(frame
index, cell index)`, not `random`, so re-running the script reproduces
byte-identical output on any machine/Python version. To add a new
fixture, add a call to `stamp_blob(frame, col, row)` (see the script)
for each finger you want and `write_frame("name.bin", frame)`.

`stamp_blob()` stamps a small (~13-cell) tapering dip, not a single
pixel: `HEATMAP_MIN_BLOB_PIXELS=2` and the cross-shaped peak-detection
radius `HEATMAP_PEAK_RADIUS=5` mean a true single-pixel dip is
invisible to the pipeline. The center dip (baseline `0xB4` -> `0x7F`,
a drop of 0x35=53) matches a real confirmed-working single-touch
capture from live hardware testing.

## Fixtures in this directory

| File(s) | Frames | Purpose |
|---|---|---|
| `baseline_00.bin` .. `baseline_29.bin` | 30 | Resting frames (uniform ~0xB4 + tiny deterministic dither) to feed first and establish the driver's per-cell baseline. `HEATMAP_BASELINE_FRAMES=30`, `content_id=0x0C`, cell_count=3456 (>=1000) — exactly what `raw_compute_signal()` requires before `heatmap_have_baseline` flips true. |
| `touch_1finger.bin` | 1 | One isolated blob at grid (20,15). |
| `touch_2finger_far.bin` | 1 | Two blobs at opposite corners: (5,5) and (65,42). Key fixture for the "does the pipeline see 2 well-separated fingers" question. |
| `touch_2finger_close.bin` | 1 | Two blobs ~8.9 grid cells apart: (30,20) and (38,24) — typical adjacent-finger spread-gesture distance. Probes ghost-merge (`raw_ghost_merge()`) without tripping it: default `ghost_dist=6` cells is smaller than 8.9, so on frame 1 (0 claimed slots, unscaled merge radius) these must NOT be merged into one blob. |
| `touch_3finger.bin` | 1 | Three blobs, diagonally spread: (15,10), (36,24), (57,38). |
| `touch_4finger.bin` | 1 | Four blobs: (10,8), (28,20), (46,32), (64,44). |
| `touch_5finger.bin` | 1 | Five blobs: (8,6), (22,15), (36,24), (50,33), (64,42). |
| `move_00.bin` .. `move_15.bin` | 16 | One blob sliding diagonally by +1 grid cell/frame, from (10,10) to (25,25). Exercises the slot state machine's EMA/deadband/continuity logic across frames, not just single-frame blob detection. |

All fixtures assume the default grid (`grid_cols=grid_rows=0` module
params, i.e. 72x48) and default `dfa_data_offset=0`.
