# Windows Contact ABI

The Windows `TouchPenProcessor0C19.dll` uses a structured contact
representation for transferring touch data between pipeline stages.
This document describes the ABI as observed from decompilation.

## Contact Structure (Inferred)

Each contact (slot) tracks:

| Field | Type | Description |
|-------|------|-------------|
| contact_id | u16 | Unique contact identifier |
| flags | u8 | State flags (empty, pending, active, lifted) |
| classification | u8 | Contact type (finger=0, palm=1, noise=2) |
| centroid_x | float | X centroid in grid cells (sub-cell precision) |
| centroid_y | float | Y centroid in grid cells |
| touch_major | float | Major axis of touch ellipse |
| touch_minor | float | Minor axis of touch ellipse |
| orientation | float | Rotation angle (radians) |
| signal | float | Total signal weight |
| pixel_count | u16 | Number of cells in blob |
| eigenratio | float | major/minor ratio |
| duration | u16 | Frames this contact has existed |
| hold_frames | u16 | Frames in hold state |
| history[8] | struct | Position history ring buffer |

## Classification (FUN_180601690)

The classifier maps each CCL blob to one of:

| Value | Type | Description |
|-------|------|-------------|
| 0 | Finger | Normal touch contact |
| 1 | Palm | Large low-signal contact (palm rejection) |
| 2 | Noise | Transient noise blob |
| 3 | Coalesced | Multiple contacts merged (post-processing) |

Classification uses a 10×11 Mahalanobis distance matrix with the
following features: centroid_x, centroid_y, touch_major, touch_minor,
signal, pixel_count, eigenratio, and 3 temporal features.

The matrix is populated at runtime from device firmware and is not
present in the static DLL binary. This is why the Linux driver
does not implement classification.

## Contact Flags

| Bit | Name | Description |
|-----|------|-------------|
| 0 | EMPTY | Slot unused |
| 1 | NEW | Just created (debounce period) |
| 2 | ACTIVE | Actively tracked contact |
| 3 | LIFTED | Contact terminated |
| 4 | HOLD | In hold state (temporary loss) |
| 5 | STATIONARY | Position locked (stationary lock) |
| 6 | EDGE | Contact near panel edge |

## Contact Limits

| Parameter | Value | Source |
|-----------|-------|--------|
| Max simultaneous contacts | 10 | DLL blob slot count |
| Max blob candidates | 64 | Per-frame extraction limit |
| History ring size | 8 | Position history depth |
| Hold timeout | ~8 frames | Before forced lift |
| Stationary lock frames | 6 | Before position freeze |

## References

- `docs/decomp/SURFACE_TRACKER_DECOMP.md` — Pipeline function mapping
- `docs/decomp/MULTITOUCH_STATIC_DECOMP.md` — Static data and tables
- Windows `TouchPenProcessor0C19.dll` decompilation (Ghidra)
