# Config Table

Configuration values extracted from the `TouchPenProcessor0C19.dll`
data segment at virtual address `0x1808E0460` (file offset `0x8DF060`).
These control the Windows touch pipeline and have been ported to the
Linux kernel driver with adaptation where needed.

## Association Radii (+0x8DC through +0x8EC)

Control the maximum distance a blob can move between frames based on
the number of currently tracked fingers. The Hungarian assignment
cost scales with the product of this radius and the `blob_max_distance`
module parameter.

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x8DC | 0.545009 | Normal radius multiplier (2 fingers) |
| +0x8E0 | 1.218098 | Single-track continuity radius |
| +0x8E4 | 1.549246 | 3-finger radius multiplier |
| +0x8E8 | 1.845492 | 4-finger radius multiplier |
| +0x8EC | 2.161228 | 5+ finger radius multiplier |

Usage: `effective_radius = blob_max_distance × multiplier[n_fingers]`

## Edge Contact Weights (+0x8D0, +0x8D4)

At panel edges, the finger signal weight is artificially reduced due
to partial contact with fewer TX/RX lines. The edge penalty compensates
by downscaling expected blob weight.

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x8D0 | 0.966512 | Side/top edge penalty multiplier |
| +0x8D4 | 0.228140 | Bottom edge penalty multiplier |

Linux implementation: cells in the first/last column/row have their
weight multiplied by the corresponding factor before blob centroid
computation.

## Coalescing Threshold (+0xC98)

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0xC98 | 36.0 | Distance² threshold for blob merging |

Linux `ghost_dist` parameter = sqrt(36.0) = 6 cells.

## Pre-Association Filter (+0x8C0, +0x8C4)

These values control a pre-Hungarian filter that rejects blobs whose
signal weight is below a fraction of the strongest blob's weight.

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x8C0 | 0.611209 | Pre-assoc ratio threshold |
| +0x8C4 | 0.754732 | Secondary ratio threshold |

**Disabled in Linux** (`pre_assoc_ratio=0`). Testing showed this
filter is too aggressive for multi-finger scenarios without the
Mahalanobis classifier to reject false positives.

## Noise Floor (+0xECC)

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0xECC | 0.04 | Noise floor as fraction of baseline range |

Linux: cells with absolute c590 < 400 (0.04 × 10000) are suppressed.
This is applied during baseline processing, not after.

## Hold Policy (+0x8D8)

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x8D8 | 0xB3 (enabled/true) | Hold state enable flag |

**Disabled in Linux** (`hold_frames=0`). Windows enables hold only for
tracks meeting strict quality checks (duration ≤ 2 frames, pixel count
within bounds, eigenratio < 4.0, signal > 0.095). Our placeholder hold
logic lacked these checks and caused scroll-brake behavior.

## Touch Detection Threshold (+0x958)

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x958 | 0.17 | Touch detection as fraction of peak signal |

**Not standalone** — requires the per-cycle gain adaptation pipeline
(FUN_180600820) which is runtime-populated from device firmware.
Cannot be used directly in Linux.

## Runtime-Populated Values (Not Available)

These config table entries contain zeros in the static DLL binary
and are populated at runtime after gain calibration. They cannot
be ported to Linux without equivalent device communication:

| Range | Purpose |
|-------|---------|
| +0xE60-0xE98 | Per-cycle gain calibration data |
| +0xD40-0xD78 | Mahalanobis classification matrix (10×11 floats) |
| +0xEA0-0xEC0 | Background noise tracking |

## Module Parameter Mapping

| Config Value | Linux Parameter | Default | Notes |
|--------------|-----------------|---------|-------|
| +0xC98 | `ghost_dist` | 6 | sqrt(36.0) |
| +0x8DC-0x8EC | `blob_max_distance` | 3 | Base radius (cells) |
| +0x8D8 | `hold_frames` | 0 | Disabled |
| +0x8C0 | `pre_assoc_ratio` | 0 | Disabled |
