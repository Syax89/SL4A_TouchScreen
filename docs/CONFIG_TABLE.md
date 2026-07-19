# DLL Config Table Reference

Complete configuration table extracted from `TouchPenProcessor0C19.dll`
data segment at virtual address `0x1808E0460` (file offset `0x8DF060`,
section `.rdata`, product ID 0x0C19).

## Association Radii (offset +0x8DC to +0x8EC)

Maximum distance a tracked contact can move between frames, scaled by
the number of currently tracked fingers. Used in the Hungarian cost
matrix: `effective_radius = blob_max_distance × multiplier[n_fingers]`.

| Offset | DLL Value | Usage | Linux Equivalent |
|--------|-----------|-------|-----------------|
| +0x8DC | 0.545009 | Normal radius (2 fingers) | `blob_max_distance=3` |
| +0x8E0 | 1.218098 | Single-track continuity | 3 × 1.218 = 3.65 cells |
| +0x8E4 | 1.549246 | 3-finger radius | 3 × 1.549 = 4.65 cells |
| +0x8E8 | 1.845492 | 4-finger radius | 3 × 1.845 = 5.54 cells |
| +0x8EC | 2.161228 | 5+ finger radius | 3 × 2.161 = 6.48 cells |

## Edge Contact Weights (offset +0x8D0, +0x8D4)

Blob signal weight compensation at panel edges, where partial contact
with fewer TX/RX lines reduces measured capacitance.

| Offset | DLL Value | Applied To |
|--------|-----------|------------|
| +0x8D0 | 0.966512 | Top, left, right edges |
| +0x8D4 | 0.228140 | Bottom edge |

Applied to individual cell weights during CCL flood-fill. The bottom
edge has a much stronger penalty because the bottom bezel blocks
most of the electric field.

## Coalescing / Ghost Rejection (offset +0xC98)

| Offset | DLL Value | Description | Linux Equivalent |
|--------|-----------|-------------|-----------------|
| +0xC98 | 36.0 | Distance² threshold for merging nearby blobs | `ghost_dist=6` (√36) |

## Pre-Association Filter (offset +0x8C0, +0x8C4)

Rejects blob candidates whose signal weight is below a fraction of
the strongest candidate's weight.

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x8C0 | 0.611209 | Primary pre-assoc ratio threshold |
| +0x8C4 | 0.754732 | Secondary ratio threshold |

Disabled in Linux (`pre_assoc_ratio=0`). Without the Mahalanobis
classifier to reject false positives, this filter is too aggressive
for multi-finger scenarios.

## Noise Floor (offset +0xECC)

| Offset | DLL Value | Description | Linux Equivalent |
|--------|-----------|-------------|-----------------|
| +0xECC | 0.04 | Signal fraction below which cells are noise | c590 < 400 suppressed |

0.04 × 10000 (c590 max range) = 400. Cells with absolute c590 value
below this threshold are treated as noise in baseline subtraction.

## Hold Policy (offset +0x8D8)

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x8D8 | 0xB3 (enabled) | Enable hold state for brief contact loss |

Disabled in Linux (`hold_frames=0`). Windows hold (FUN_180606370) is
a complex quality-gated policy: it only applies to tracks with
duration ≤ 2 frames, pixel count in bounds, eigenratio < 4.0, and
signal > 0.095. Our simplified implementation caused scrolling to
"brake" on finger lift.

## Touch Detection Threshold (offset +0x958)

| Offset | DLL Value | Description |
|--------|-----------|-------------|
| +0x958 | 0.17 | Touch detection as fraction of peak signal |

Not directly used in Linux. This threshold applies after per-cycle
gain adaptation (FUN_180600820), which adjusts global signal gain
based on noise floor measurements. Without the gain pipeline, this
threshold is meaningless as a standalone value.

## Runtime-Only Values (Not Ported)

These config table ranges contain zeros in the static DLL binary and
are populated at runtime by the Windows user-mode service or device
firmware. They cannot be ported to Linux without equivalent
communication channels.

| Offset Range | Purpose | Source |
|-------------|---------|--------|
| +0xE60 - +0xE98 | Per-cycle gain calibration coefficients | Device firmware via SurfaceSystemTelemetryDriver |
| +0xD40 - +0xD78 | Mahalanobis classification matrix (10×11 floats) | Device firmware |
| +0xEA0 - +0xEC0 | Background noise adaptation tracking | Runtime sampling |
| +0xE20 - +0xE48 | Contact size calibration | Device calibration |
| +0xED0 - +0xF10 | Regional sensitivity maps | Runtime learning |

## Data Type Notes

All values at offsets +0x8C0 through +0xECC are IEEE 754 single-precision
floats (32-bit). The hold flag at +0x8D8 is a byte interpreted as boolean
(0xB3 is a non-zero "enabled" value, though the exact interpretation may
depend on the pipeline logic at FUN_180606370).

The base address `0x1808E0460` was identified via Ghidra cross-reference
analysis of FUN_180600c40 (CCL initialize), which references this table
for per-cycle configuration.
