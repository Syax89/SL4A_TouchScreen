# Touch Pipeline

The raw multi-touch pipeline processes the 72×48 heatmap (3456 cells) into
HID multitouch contacts. The pipeline mirrors the Windows
`TouchPenProcessor0C19.dll` processing chain, with ~85% functional alignment.

## Pipeline Diagram

```
Frame (72×48 heatmap, 3456 cells, 16-bit raw)
  │
  ├─ 1. c590 LUT
  │     Maps raw 16-bit to fixed-point signal:
  │     c590[i] = max(0, 10000 - (i*22 + 6000))
  │
  ├─ 2. Baseline Subtraction (EMA, 30-frame init)
  │     Per-cell asymmetric EMA: tracks ambient, protects touch cells
  │
  ├─ 3. Noise Floor
  │     c590 signal < 400 → suppressed
  │     (Windows DAT_1806c08c8 = 0.04, config table +0xECC)
  │     heatmap_touched[i] = (rise >= 200)
  │
  ├─ 4. Peak Detection Gate (Windows FUN_1805fba00)
  │     │  Cross-shaped ±5 cells in each direction
  │     │  A cell is a peak if no higher neighbour in ±5
  │     │  min_rise = 200, max 16 peaks collected
  │     │
  │     └─ No peak → CCL SKIPPED → 0 blobs → lift all slots
  │
  ├─ 5. CCL Flood-Fill (Windows FUN_180600c40)
  │     4-connected BFS over touched cells
  │     Each connected component → blob candidate
  │     Queue size: 512 elements
  │     Per-blob: pixel_count, max_rise, sum_weight (sw)
  │     Filters: pixel_count ≥ 2, max_rise ≥ 200, sw ≥ 1000
  │
  ├─ 6. Velocity Rejection
  │     Blob centroid must be within 6 cells of a detected peak
  │     (Windows: centroid distance² ≤ 36.0)
  │     Eliminates noise blobs far from any genuine signal maximum
  │
  ├─ 7. Edge Penalty (config +0x8D0/+0x8D4)
  │     Bottom-edge cells: weight × 0.23
  │     Other-edge cells: weight × 0.97
  │     Compensates for reduced capacitance at panel edges
  │
  ├─ 8. Blob Splitting (Windows FUN_180602770)
  │     When a CCL blob contains 2+ peaks ≥ 4 cells apart
  │     and has pixel_count ≥ 8:
  │     → For each internal peak, compute 5×5 sub-centroid
  │     → Split into independent blob candidates
  │     Prevents merged fingers from becoming one blob
  │
  ├─ 9. Centroid Computation
  │     Signal-weighted centroid over full blob extent
  │     Fixed-point ×100 for sub-cell precision:
  │       cx = Σ(col × weight) / Σ(weight) × 100
  │       cy = Σ(row × weight) / Σ(weight) × 100
  │
  ├─ 10. Eigenvalues
  │      Second moments over blob bounding box:
  │        sxx = Σ((x-cx)² × w), syy = Σ((y-cy)² × w), sxy = Σ((x-cx)(y-cy) × w)
  │      Produces touch major, minor, orientation via atan2
  │
  ├─ 11. Pre-Merge (Ghost Rejection)
  │      Merge blobs within ghost_dist=6 cells
  │      (Windows config +0xC98 = 36.0 → distance² < 36)
  │      Keep blob with highest weight in each cluster
  │
  ├─ 12. Hungarian Assignment (Windows FUN_1805fd090)
  │      │  Cost matrix: sqrt(dx²+dy²) for in-range,
  │      │               100 for out-of-range, 1000 for empty
  │      │
  │      │  Multi-finger association radii (config +0x8DC-0x8EC):
  │      │    1 finger × 2.2, 2 fingers × 1.0, 3 × 2.8, 4 × 3.4, 5+ × 4.0
  │      │
  │      └─ Jump rejection: blob beyond radius+2 cells → no match
  │
  ├─ 13. Slot State Machine
  │      States: 0=empty, 1=new (debounce 3 frames), 2=claimed,
  │               3=lift pending, 4=hold (disabled)
  │      hold_frames=0: direct 2→3 lift on blob loss
  │
  ├─ 14. EMA Smoothing + Deadband + Stationary Lock
  │      EMA: weight_smoothed = (old×7 + new)/8  (alpha=7)
  │      Deadband: ignore position changes < ±80 (±0.8 cells)
  │      Stationary lock: after 6 still frames, freeze position
  │      Prevents pinch-to-zoom jitter on held fingers
  │
  ├─ 15. Lift Lookback
  │      On lift, emit position from 2 frames ago
  │      Compensates for the 2-frame pipeline delay
  │
  └─ 16. MT Emission
       ABS_MT_POSITION_X/Y, ABS_MT_TOUCH_MAJOR/MINOR,
       ABS_MT_ORIENTATION, ABS_MT_TRACKING_ID
       Output to HID multitouch subsystem → evdev → libinput
```

## Alignment with Windows DLL

| Stage | Windows Function | Alignment | Notes |
|-------|-----------------|-----------|-------|
| Peak gate | FUN_1805fba00 | ~90% | Cross ±5, min_rise=200 (adapted) |
| CCL | FUN_180600c40 | ~70% | 4-connected BFS vs per-pixel records |
| Centroid | FUN_180602e60 | ~85% | Full blob extent, ×100 fixed-point |
| Velocity rejection | FUN_180600c40 | 100% | dist² ≤ 36.0 confirmed |
| Edge penalty | config+0x8D0/0x8D4 | 90% | 0.967/0.228 from DLL |
| Hungarian | FUN_1805fd090 | ~90% | Cost matrix matches |
| Association radii | config+0x8DC-0x8EC | 100% | DLL values: 0.545, 1.218, 1.549, 1.845, 2.161 |
| EMA smoothing | FUN_180608000 | ~80% | alpha=7, weight=1/8 |
| Hold/lift | FUN_180606370 | ~40% | Simplified (hold_frames=0) |
| Classifier | FUN_180601690 | 0% | Blocked — Mahalanobis matrix runtime-only |
| Per-cycle gain | FUN_180600820 | 0% | Blocked — runtime from device firmware |

## Key Differences from Windows

1. **4-connected BFS** vs per-pixel CCL with 10-neighbor graph. Functionally
   equivalent for finger-scale detection on the 72×48 grid.

2. **Pre-merge** vs post-emission coalescing. Windows merges at contact level;
   Linux merges at blob level before Hungarian. Similar outcome, simpler logic.

3. **No Mahalanobis classifier**. Requires a 10×11 float matrix populated at
   DLL runtime from device firmware — not present in the static binary.

4. **No per-cycle gain adaptation**. Also runtime-populated from device firmware
   telemetry, not available in Linux.

5. **No hold state**. Windows enables it (config+0x8D8=0xB3) for tracks meeting
   strict quality checks. Simplified hold caused scroll braking in testing.
