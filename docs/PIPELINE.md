# Surface TouchPenProcessor0C19 — Kernel Pipeline

Canonical pipeline document for the Linux kernel multitouch driver
matching the Windows `TouchPenProcessor0C19.dll` processing chain.

File: `driver/spi-hid-core.c`, entry point `heatmap_process_frame()`.

## Pipeline Overview

```
Frame (72x48 heatmap, 3456 cells)
  │
  ├─ c590 LUT + baseline →
  │   signal rise per cell
  │
  ├─ noise floor (c590 < 400 → suppressed, Windows DAT_1806c08c8=0.04)
  │   heatmap_touched[i] = (rise >= 200)
  │
  ├─ Peak Detection GATE (cross ±5, min_rise=200, Windows FUN_1805fba00)
  │   ├─ no peak → CCL SKIPPED → 0 blobs → lift all slots
  │   └─ peaks found → run CCL
  │
  ├─ CCL Flood-Fill (4-connected BFS, Windows FUN_180600c40)
  │   │  Each connected component → blob candidate
  │   │  Filters: pixel_count≥2, max_rise≥200, w≥1000
  │   │
  │   ├─ Velocity rejection (blob within 6 cells of a peak, Windows dist²≤36.0)
  │   ├─ Edge penalty (bottom edge ×23%, other edges ×97%, config+0x8D0/0x8D4)
  │   ├─ Blob splitting (2+ peaks within blob ≥4 cells apart → split)
  │   ├─ Eigenvalues (second moments on blob bounding box)
  │   └─ Centroid (×100 fixed-point, signal-weighted on full blob extent)
  │
  ├─ Pre-merge (ghost_dist=6, distance²<36, Windows config+0xC98=36.0)
  │
  ├─ Hungarian Assignment (Windows FUN_1805fd090)
  │   │  Cost matrix: sqrt(dx²+dy²) in-range, 100 out, 1000 empty
  │   │  Multi-finger radii (config+0x8DC-0x8EC):
  │   │    1 finger ×2.2, 2×1.0, 3×2.8, 4×3.4, 5+×4.0
  │   └─ Jump rejection (beyond radius+2 cells → rejected)
  │
  ├─ Slot State Machine
  │   │  0=empty, 1=new(debounce 3), 2=claimed, 3=lift, 4=hold
  │   └─ hold_frames=0 (disabled, direct 2→3 lift)
  │
  ├─ EMA + Deadband + Stationary Lock
  │   │  EMA alpha=7 (weight 1/8), deadband ±80 (±0.8 cells)
  │   │  Stationary lock after 6 frames
  │   └─ Lift lookback (2 frames history)
  │
  └─ MT Emission (ABS_MT_POSITION, ABS_MT_TOUCH_MAJOR/MINOR/ORIENTATION)
```

## Windows Alignment Status

| Stage | Windows Function | Alignment | Notes |
|-------|-----------------|-----------|-------|
| Peak gate | FUN_1805fba00 | ~90% | Cross ±5, min_rise=200 (adapted from Windows 500) |
| CCL | FUN_180600c40 | ~70% | 4-connected BFS vs per-pixel slot records |
| Centroid | FUN_180602e60 | ~85% | Full blob extent, ×100 fixed-point |
| Velocity rejection | FUN_180600c40 | 100% | dist² ≤ 36.0 confirmed |
| Edge penalty | config+0x8D0/0x8D4 | 90% | 0.967/0.228 from DLL |
| Coalescing | FUN_1806025c0 | ~60% | Pre-merge only (post-emission reverted) |
| Hungarian | FUN_1805fd090 | ~90% | Cost matrix matches |
| Association radii | config+0x8DC-0x8EC | 100% | DLL values: 0.545, 1.218, 1.549, 1.845, 2.161 |
| EMA smoothing | FUN_180608000 | ~80% | alpha=7, weight=1/8 |
| Hold/lift | FUN_180606370 | ~40% | Simplified (hold_frames=0, Windows: history-based) |
| Candidate classif. | FUN_180601690 | 0% | Blocked — needs Mahalanobis matrix from DLL runtime |
| Per-cycle gain | FUN_180600820 | 0% | Blocked — runtime-populated config |

## Config Table Values Extracted (DLL DAT_1808e0460)

| Offset | Value | Usage |
|--------|-------|-------|
| +0x8DC | 0.545009 | Normal association radius (2 fingers) |
| +0x8E0 | 1.218098 | Single-track continuity radius |
| +0x8E4 | 1.549246 | 3-finger radius |
| +0x8E8 | 1.845492 | 4-finger radius |
| +0x8EC | 2.161228 | 5+ finger radius |
| +0x8D0 | 0.966512 | Top edge penalty |
| +0x8D4 | 0.228140 | Bottom edge penalty |
| +0x8D8 | 0xB3 (enabled) | Hold policy enable |
| +0xC98 | 36.0 | Coalescing threshold (distance²) |
| +0xECC | 0.04 | Noise floor |
| +0x8C0 | 0.611209 | Pre-assoc filter (disabled for us) |
| +0x958 | 0.17 | Touch detection pct (not standalone — needs gain pipeline) |

## Module Parameters

| Param | Default | Description |
|-------|---------|-------------|
| `raw_mode` | 1 | Enable heatmap + multitouch |
| `skip_getfeat` | 1 | Vendor-init + SET_FEATURE activation |
| `ema_alpha` | 7 | EMA smoothing weight |
| `blob_max_distance` | 3 | Hungarian association radius (cells) |
| `blob_min_weight` | 1000 | Minimum blob weight |
| `blob_debounce` | 3 | Debounce frames |
| `blob_lift_frames` | 3 | Missed frames before lift |
| `hold_frames` | 0 | Hold grace frames (0=disabled) |
| `ghost_dist` | 6 | Pre-merge radius (cells) |
| `pre_assoc_ratio` | 0 | Pre-assoc weight filter (0=disabled) |
| `grid_cols/rows` | 72/48 | Grid dimensions |
| `calib_scale_x/y` | 0 | Scale ×1000 (0=auto) |
| `calib_offset_x/y` | 0 | Screen offset |
| `invert_x/y` | 0 | Invert axis |
| `swap_xy` | 0 | Swap axes |

## Differences from Windows (deliberate)

1. **4-connected BFS** vs per-pixel CCL with 10-neighbor graph: ours is simpler but
   functionally equivalent for finger detection on 72×48 grid.

2. **Pre-merge** vs post-emission coalescing: ours merges at blob level before Hungarian;
   Windows merges at contact level after emission. Similar outcome.

3. **No Mahalanobis classifier**: requires 10×11 float matrix from DLL runtime data
   segment — not present in the static binary.

4. **No per-cycle gain**: requires runtime callback from device firmware — not
   available on Linux.

5. **No hold state**: Windows enables it (config+0x8D8=0xB3) but only for tracks
   meeting strict history quality checks (duration≤2, pixel count bounds,
   eigenratio<4.0, signal>0.095). Our simplified hold caused scroll brake.
