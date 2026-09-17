# Touch Pipeline (Raw Mode)

The raw multi-touch pipeline converts the device's capacitive sensor grid
into HID multitouch contacts. It mirrors the Windows
`TouchPenProcessor0C19.dll` processing chain (~85% functional alignment) and
is compiled in `driver/mshw0231-raw.c` with every constant in
`driver/mshw0231-raw-constants.h`.

The input is a **CapImg frame** — 3456 cells (72×48) on SL4 `MSHW0231`,
4056 cells (78×52) on SL3 `MSHW0162`; the grid is selected by ACPI ID at
probe (see [Architecture](Architecture)). The raster is one byte per cell,
row-major, **row stride = grid width** (no padding); the resting level is
`0xB4` (180) and a touch *lowers* the byte.

## Reference chain: the Windows detector

The DLL splits the work differently from the Linux pipeline below: a
per-frame **detector** selects one peak, and a separate **tracker** turns the
sequence of peaks into contacts. The detector's stages:

```
frame (raster + per-frame lists)
  │
  ├─ a. Candidates — from the frame's blob list, each blob contributes
  │      itself plus its four distance-1 neighbours, deduped, in one flat
  │      list (capacity 25)
  ├─ b. Isolation gate — reject when any of the four cells at ±5 cells
  │      (cardinal axes) reads below 135; out-of-raster samples pass
  ├─ c. Score — 11×11 kernel (Σ = 1.5513) × c590 over the window
  └─ d. Publish — the highest-scoring survivor, if it reaches 0.05
         (0.04 in the alternate state); one peak per frame
```

Notes that matter when comparing with the Linux chain:

- the image the detector reads is a 288-byte-row plane where only the first
  72 bytes of each row carry the SL4 image; the raster layout of the frame
  itself is the 72-byte stride documented above;
- the frame is filled by the transport side, and the DLL publishes its result
  back into the same object (peak position, flags) — the tracker reads it
  from there;
- one peak per frame means multi-contact output comes from the tracker
  (see [Multi-touch (Experimental)](Multi-touch-Experimental)), never from
  the detector.

| Windows stage | Linux equivalent |
|---|---|
| a. candidates from the frame's lists | 4. peak detection, 5. CCL |
| b. ±5 isolation gate | (no direct equivalent — see below) |
| c. 11×11 kernel score | 3. signal rise + noise floor |
| d. one peak per frame | 11. MT emission (driver publishes all blobs) |

The ±5 gate has no counterpart in the Linux chain: the driver keeps a blob
when its rise passes the touch threshold and its weight passes
`blob_min_weight`, while the reference implementation additionally requires
the deflection to have decayed at five cells in all four cardinal directions.

## Processing chain

```
CapImg frame (0x0C)
  │
  ├─ 1. c590 LUT — raw byte → fixed-point signal
  ├─ 2. Baseline — per-cell ambient model (asymmetric EMA)
  ├─ 3. Signal rise + noise floor
  ├─ 4. Peak detection — local maxima
  ├─ 5. CCL flood-fill — connected components → blobs
  ├─ 6. Velocity rejection + blob splitting
  ├─ 7. Ghost merge — close-blob consolidation
  ├─ 8. Centroid + weight (EMA)
  ├─ 9. Hungarian assignment — blobs ↔ tracked slots
  ├─10. Position EMA + deadband + stationary lock
  └─11. MT emission (input_mt, 47 slots)
```

## 1. c590 lookup table

Each raw byte is mapped to a fixed-point signal via a precomputed LUT:

```
c590[i] = max(0, 10000 − ( (i·22204 + 500) / 1000 + 6000 ))
```

(`C590_BASE=10000`, `C590_STEP_NUM=22204` ≈ 0.00222035428 per step,
`C590_STEP_DEN=1000`, `C590_OFFSET=6000` — Windows-verified.) The table is
built once at `mshw0231_raw_init()`.

## 2. Baseline (asymmetric per-cell EMA)

Each cell tracks a resting baseline. Building it takes **30 frames on SL4 /
33 frames on SL3** (per-device `heatmap_baseline_needed`); during this window
the baseline is the per-cell **maximum** observed raw value (a touch only
lowers the raw byte).

After stabilization, tracking is asymmetric:

| Condition | Rule |
|---|---|
| `raw ≥ baseline` (finger lifted / resting) | Recover toward raw: `new = (7·base + raw)/8` — the Windows-documented **12.5% recovery rate** (EMA alpha 7) |
| `raw < baseline` (touch) | Do **not** chase the touch down; only a very slow decay tracks downward thermal drift, so a held finger never fades into the baseline |

## 3. Signal rise and noise floor

- `rise = c590[baseline] − c590[raw]` (touch lowers raw, so rise is positive)
- Cells with absolute c590 < **400** are suppressed (noise floor, DLL
  config +0xECC = 0.04)
- A cell is touched when `rise ≥ 200` (`HEATMAP_TOUCH_MIN_RISE`)

## 4. Peak detection

A true **local-maximum scan** over the full neighborhood
(`HEATMAP_PEAK_RADIUS = 2`): a cell is a peak when no neighbor within radius 2
is strictly higher. Only the blob's true center qualifies — this replaced an
earlier 4-point probe that over-counted ~13 peaks per blob and exhausted the
shared **16-peak budget** (`HEATMAP_MAX_PEAKS`), silently dropping any 3rd+
simultaneous finger.

## 5. CCL flood-fill

4-connected BFS over touched cells (`raw_ccl_flood_fill()`, queue 512).
Each connected component becomes a blob candidate, gated by:

| Filter | Threshold |
|---|---|
| Pixel count | ≥ 2 (`HEATMAP_MIN_BLOB_PIXELS`) |
| Max rise | ≥ 200 |
| Signal weight | ≥ 1000 (`blob_min_weight`, module param) |
| Velocity rejection | centroid within **6 cells** of a detected peak (`HEATMAP_VELOCITY_REJECT_RADIUS`, DLL 36.0 = 6²) |

## 6. Blob splitting

Overlapping components (e.g. two close fingers) are split when they contain
≥ 2 peaks (`HEATMAP_SPLIT_MIN_PEAKS`) separated by ≥ 4 cells
(`HEATMAP_SPLIT_MIN_DIST`), splitting radius 2.

## 7. Ghost merge

Genuinely distinct close fingers used to be merged into one blob at high
density. The merge radius now **scales down** as the finger count rises
(inverse of the association radii):

| Fingers | 1 | 3 | 4 | 5 |
|---:|---:|---:|---:|---:|
| `GHOST_RADIUS_*` | 10 | 7 | 6 | 5 |

## 8. Centroid and weight

- Blob centroid from weighted pixel positions (edge penalty: top/side
  cells ×0.97, bottom ×0.23 — DLL +0x8D0/+0x8D4)
- Weight EMA is fixed at the Windows-verified value: `weight = (old·7 + new)/8`
  (`HEATMAP_WEIGHT_EMA_ALPHA = 7`, independent of `ema_alpha`)

## 9. Hungarian assignment

Blobs are matched to tracked slots with a Kuhn–Munkres augmenting-path
solver (`raw_hungarian_match()`, replacing an earlier greedy matcher).
Cost model (×`HUNGARIAN_COST_SCALE` = 100):

| Cost | Value | Meaning |
|---|---:|---|
| In-range | 10 | Preferred valid assignment |
| Empty slot | 1000 | Leaving a slot empty |
| Out-of-range | 1500 | Forcing a doomed match (must stay > EMPTY) |
| Continuity bonus | 5 | Keep two actively-tracked fingers from swapping mid-gesture |
| Jump-reject margin | 200 | Reject implausible jumps |

Association radii widen with finger count: 22 / 28 / 34 / 40 cells
(`ASSOC_RADIUS_1/3/4/5_FINGERS`).

## 10. Position smoothing, deadband, stationary lock

- **Position EMA**: `new = (old·α + gx)/(α+1)` with α = `ema_alpha`
  module parameter (default **2**; lower = more responsive, more jitter)
- **Deadband**: `HEATMAP_DEADBAND_THRESHOLD = 20` (fixed-point units) —
  tiny movements inside the deadband do not move the contact
- **Stationary lock**: after `HEATMAP_STATIONARY_FRAMES = 2` in place, the
  contact is locked — this is what eliminates pinch-to-zoom jitter
- Hold-state recovery weight: 4000 (`HEATMAP_HOLD_RECOVERY_WEIGHT`)

## 11. MT emission

Blobs are published through the input subsystem's multitouch protocol
(`input_mt_init_slots` with **47 slots**, `HEATMAP_MAX_SLOTS`) with
`TOUCH_MAJOR/MINOR/ORIENTATION` from per-blob eigenvalues. Missed frames
release slots after `HEATMAP_MISSED_FRAME_TIMEOUT_MS = 60`.

## Parameter mapping

| Stage | Module parameter | Default |
|---|---|---|
| Weight gate | `blob_min_weight` | 1000 |
| New-touch debounce | `blob_debounce` | 3 |
| Lift after missed frames | `blob_lift_frames` | 3 |
| Ghost merge radius | `ghost_dist` | 6 |
| Association base | `blob_max_distance` | 3 |
| Pre-association filter | `pre_assoc_ratio` | 0 (disabled) |
| Position smoothing | `ema_alpha` | 2 |
| Grid geometry | `grid_cols` / `grid_rows` | per-device (72×48 / 78×52) |

Everything is host-side signal processing on raw sensor data: no firmware
calibration, no Mahalanobis classifier, no per-cycle gain adaptation (both
require device firmware access). See [Config Table](Config-Table) for the
Windows DLL provenance of these values.
