# Aligning the raw pipeline with the Windows detector (device 0x0C19)

Every row names where the Windows value comes from. "PSDB record" means the static
per-device configuration record `DAT_1808e0460` (+0x1d8c) in `TouchPenProcessor0C19.dll`,
selected by device id through `FUN_180625d60` / `FUN_180627980` and copied to
`ctx+0x194d8` for the detector `FUN_1805fba00`. The signal domain is the c590 lookup
table, which the same binary builds as

    c590[j] = 1.0 - (j * 0.00222035428 + 0.6)        (FUN_1805ff7e0, table at dev+0xc590)

so a cell rises when its raw byte falls below the resting value 180, one raw count is
0.00222035428 = 22.2035 in our x10^4 fixed-point units, and 1000 units = 0.1.

## Verified identical — no work

| item | Windows | ours |
| --- | --- | --- |
| c590 transform and rest value | table zeroed exactly at byte 180 | same transform, same LUT |
| noise floor | 0.04 (float at record +0x04) | `HEATMAP_TOUCH_MIN_ABSOLUTE 400` = 0.04 |
| low signal threshold | 0.02 (float at record +0x00) | `HEATMAP_TOUCH_MIN_RISE 200` = 0.02 |
| candidate count gate | +0x1fa = 2 | `HEATMAP_MIN_BLOB_PIXELS 2` |
| frame age gate | +0x1fb = 3 | `blob_debounce 3` (both count frames before publishing) |
| extended search | +0x1fd = 0 (off) | fixed-radius peak search, no extended pass |
| grid | 72 x 48, 3456 samples | same |
| baseline depth | 30 frames | `HEATMAP_BASELINE_FRAMES 30` |
| ghost / velocity radius | 6 cells (36.0 in squared distance) | `HEATMAP_VELOCITY_REJECT_RADIUS 6`, `ghost_dist 6` |
| edge penalty | 23% bottom, 97% top | `HEATMAP_EDGE_PENALTY_BOTTOM 23`, `_TOP 97` |
| centroid | 5x5 weighted above baseline | full-blob signal-weighted centroid (×100 fixed-point); the radius-2 scan is the peak gate, not a centroid window |

## Gaps to close

1. **The 0.1 per-cell touch gate is missing.** Windows gates a cell at +0x1fe = 135,
   which is the byte form of 0.1; the same number appears as the u16 +0x1f8 = 1000, i.e.
   0.1 in our units. We gate the touched mask at rise >= 200 (0.02) and absolute >= 400
   (0.04), so our mask is *looser* than Windows': every reference blob measured at 0.1 on
   the recovered touch session is a subset of our mask. Applying the Windows gate means
   `HEATMAP_TOUCH_MIN_ABSOLUTE` becomes the 0.1 value (1000) while 400 keeps its verified
   role as the noise floor.
   Evidence: record dump `dat_1808e0460.txt` (bytes at rec+0x1f8..+0x1fe = 1000 / 2 / 3 / 5 / 0 / 135);
   detector read of +0x1fe at 0x1805fbbac; the 0.1 identity is in
   `ghidra-findings/FINDINGS3.md`.
   Check available: `tests/real_frame_replay_test.c` with the recovered corpus prints the
   touched-mask size per frame next to the reference blob count, so the change is
   measurable rather than asserted.

2. **The 5-frame gate (+0x1fc) has no counterpart.** Windows carries two frame-age gates
   (3 and 5); we implement 3 (`blob_debounce`) and default `hold_frames` to 0. The
   *semantics* of the 5 are not established (release/hold bound or a second age test), so
   the action is to keep the value visible and tunable, not to guess a mapping.

3. **`HEATMAP_HOLD_RECOVERY_WEIGHT 4000`** sits above 36% of the real finger blobs in the
   recovered session (213 of 591; weights run 1025..9498, median 4850). It is our own
   constant; Windows' equivalent has not been located. Treat it as a tuning knob and
   document the measurement.

## Invented, with no Windows source yet — flag, do not "align"

`ASSOC_RADIUS_*` and `GHOST_RADIUS_*` (both keyed by finger count), the `HUNGARIAN_*`
cost table, `HEATMAP_SPLIT_*`, `HEATMAP_DRIFT_DIV`, the EMA alphas,
`HEATMAP_DEADBAND_THRESHOLD`, `HEATMAP_STATIONARY_FRAMES`, `HEATMAP_MISSED_FRAME_TIMEOUT_MS`.
The DLL has dynamic association radii at `config+0x8dc` / `config+0x8e0` that have not
been decoded: until they are, changing these numbers would be substituting one guess for
another, which is exactly what the recovered documents warn against.

## Provenance

- `~/sl4a-analysis/ghidra-findings/FINDINGS.md`, `FINDINGS2.md`, `FINDINGS3.md` (Ghidra
  12.1.3 on the recovered DLL, MD5 de83210b2299aaba5c2c9819f166c0fe)
- `~/sl4a-analysis/spb-findings/` (Windows SPB traces, `surface_init.csv`, `surface_touch.csv`,
  both matching the SHA-256 digests the project already recorded)
- `~/sl4a-analysis/raw-corpus/` (537 real touch frames, 44 of them with three or more fingers,
  plus `frame-stats.json` and `REPLAY-FINDINGS.md`)
- driver side: `driver/mshw0231-raw.c`, `driver/mshw0231-raw-constants.h`
