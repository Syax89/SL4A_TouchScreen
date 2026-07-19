# Surface TouchPenProcessor0C19 — Kernel Pipeline

Linux kernel multitouch pipeline matching the Windows
`TouchPenProcessor0C19.dll` processing chain.

## Pipeline Overview

```
Frame (72×48 heatmap, 3456 cells)
  │
  ├─ c590 LUT + baseline → signal rise per cell
  │   c590[i] = max(0, 10000 - (i·22 + 6000))
  │   Baseline: 30-frame asymmetric per-cell EMA
  │
  ├─ Noise floor (c590 < 400 → suppressed, config+0xECC=0.04)
  │   heatmap_touched[i] = (rise >= 200)
  │
  ├─ Peak Detection Gate (cross ±5, min_rise=200, FUN_1805fba00)
  │   ├─ no peak → CCL SKIPPED → 0 blobs → lift all slots
  │   └─ peaks found → run CCL
  │
  ├─ CCL Flood-Fill (4-connected BFS, FUN_180600c40)
  │   Each connected component → blob candidate
  │   Filters: pixel_count ≥ 2, max_rise ≥ 200, weight ≥ 1000
  │
  ├─ Velocity Rejection (blob within 6 cells of peak, dist² ≤ 36.0)
  │
  ├─ Edge Penalty (config+0x8D0/0x8D4)
  │   Bottom edge: weight × 0.23
  │   Other edges: weight × 0.97
  │
  ├─ Blob Splitting (FUN_180602770)
  │   Multi-peak blobs ≥ 4 cells apart, pixel_count ≥ 8 → split
  │
  ├─ Centroid + Eigenvalues
  │   Centroid: signal-weighted ×100 fixed-point on full blob extent
  │   Eigenvalues: second moments on bounding box → major/minor/orientation
  │
  ├─ Pre-merge (ghost_dist=6, distance² < 36, config+0xC98=36.0)
  │
  ├─ Hungarian Assignment (FUN_1805fd090)
  │   Cost matrix with multi-finger radii from config:
  │     1 finger ×2.2, 2×1.0, 3×2.8, 4×3.4, 5+×4.0
  │
  ├─ Slot State Machine (0=empty, 1=new, 2=claimed, 3=lift, 4=hold)
  │   hold_frames=0 (disabled)
  │
  ├─ EMA + Deadband + Stationary Lock
  │   EMA alpha=7, deadband ±80, lock after 6 still frames
  │
  ├─ Lift Lookback (2 frames history)
  │
  └─ MT Emission (ABS_MT_POSITION, MAJOR/MINOR/ORIENTATION)
```

## Windows Alignment

| Stage | Windows Function | Alignment |
|-------|-----------------|-----------|
| Peak gate | FUN_1805fba00 | ~90% (cross ±5, min_rise adapted) |
| CCL | FUN_180600c40 | ~70% (4-connected BFS vs per-pixel) |
| Centroid | FUN_180602e60 | ~85% (full blob, ×100 fixed-point) |
| Velocity rejection | FUN_180600c40 | 100% (dist² ≤ 36.0) |
| Edge penalty | config+0x8D0/0x8D4 | 90% (0.967/0.228 from DLL) |
| Hungarian | FUN_1805fd090 | ~90% (cost matrix matches) |
| Association radii | config+0x8DC-0x8EC | 100% (DLL values verified) |
| EMA smoothing | FUN_180608000 | ~80% (alpha=7) |
| Hold/lift | FUN_180606370 | ~40% (simplified, hold_frames=0) |
| Classifier | FUN_180601690 | 0% (Mahalanobis runtime-matrix) |
| Per-cycle gain | FUN_180600820 | 0% (runtime from firmware) |

## Config Table Values

Extracted from DLL `DAT_1808e0460` (file offset `0x8DF060`):

| Offset | Value | Usage |
|--------|-------|-------|
| +0x8DC | 0.545009 | Normal association radius (2 fingers) |
| +0x8E0 | 1.218098 | Single-track continuity radius |
| +0x8E4 | 1.549246 | 3-finger radius |
| +0x8E8 | 1.845492 | 4-finger radius |
| +0x8EC | 2.161228 | 5+ finger radius |
| +0x8D0 | 0.966512 | Top/side edge penalty |
| +0x8D4 | 0.228140 | Bottom edge penalty |
| +0x8D8 | 0xB3 (enabled) | Hold policy enable |
| +0xC98 | 36.0 | Coalescing threshold (distance²) |
| +0xECC | 0.04 | Noise floor |
| +0x8C0 | 0.611209 | Pre-assoc ratio (disabled) |
| +0x958 | 0.17 | Touch detect pct (not used standalone) |

## Module Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `raw_mode` | 1 | Raw heatmap + multi-touch |
| `skip_getfeat` | 1 | Vendor-init activation path |
| `ema_alpha` | 7 | EMA smoothing weight |
| `blob_max_distance` | 3 | Hungarian base radius (cells) |
| `blob_min_weight` | 1000 | Minimum blob weight |
| `blob_debounce` | 3 | Debounce frames for new touch |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace period (0=disabled) |
| `ghost_dist` | 6 | Pre-merge radius (cells) |
| `pre_assoc_ratio` | 0 | Pre-assoc filter (0=disabled) |
| `grid_cols/rows` | 72/48 | Heatmap dimensions |
| `calib_scale_x/y` | 0 | Coordinate scaling (0=auto) |
| `calib_offset_x/y` | 0 | Screen offset |
| `invert_x/y` | 0 | Invert axis |
| `swap_xy` | 0 | Swap axes |

## See Also

- `docs/CONFIG_TABLE.md` — Complete DLL config table reference
- `docs/decomp/SURFACE_TRACKER_DECOMP.md` — Windows DLL decompilation notes
- `docs/decomp/MULTITOUCH_STATIC_DECOMP.md` — Static analysis and data tables
