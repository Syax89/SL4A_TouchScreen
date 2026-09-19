# Config Table — Windows DLL Values and Linux Mapping

Configuration values extracted from the `TouchPenProcessor0C19.dll` data
segment at virtual address `0x1808E0460` (file offset `0x8DF060`). These
control the Windows touch pipeline; the Linux driver ports them with
adaptation where noted. Cross-reference: [Touch Pipeline](Pipeline).

## Association radii (+0x8DC … +0x8EC)

Maximum distance a blob may move between frames, scaled by the number of
tracked fingers. Linux: `effective_radius = blob_max_distance × multiplier`.

| Offset | DLL value | Meaning |
|---|---:|---|
| +0x8DC | 0.545009 | 2-finger radius multiplier |
| +0x8E0 | 1.218098 | Single-track continuity radius |
| +0x8E4 | 1.549246 | 3-finger radius multiplier |
| +0x8E8 | 1.845492 | 4-finger radius multiplier |
| +0x8EC | 2.161228 | 5+-finger radius multiplier |

## Edge contact weights (+0x8D0, +0x8D4)

Finger weight is reduced at panel edges (partial TX/RX coverage). Linux
multiplies first/last row/column cells before centroid computation:
`HEATMAP_EDGE_PENALTY_TOP = 97` (0.966512 × 100), `BOTTOM = 23` (0.228140 × 100).

| Offset | DLL value | Linux |
|---|---:|---|
| +0x8D0 | 0.966512 | top/side penalty ×0.97 |
| +0x8D4 | 0.228140 | bottom penalty ×0.23 |

## Coalescing threshold (+0xC98)

| Offset | DLL value | Linux |
|---|---:|---|
| +0xC98 | 36.0 | `ghost_dist = sqrt(36.0) = 6` cells |

## Pre-association filter (+0x8C0, +0x8C4)

Rejects blobs below a fraction of the strongest blob's weight.

| Offset | DLL value | Meaning |
|---|---:|---|
| +0x8C0 | 0.611209 | Primary ratio threshold |
| +0x8C4 | 0.754732 | Secondary ratio threshold |

**Disabled in Linux** (`pre_assoc_ratio=0`): testing showed it is too
aggressive for multi-finger scenarios without the Mahalanobis classifier.

## Noise floor (+0xECC)

| Offset | DLL value | Linux |
|---|---:|---|
| +0xECC | 0.04 | cells with c590 < 400 (0.04 × 10000) suppressed |

Applied during baseline processing, not after.

## Hold policy (+0x8D8)

| Offset | DLL value | Linux |
|---|---:|---|
| +0x8D8 | 0xB3 (enabled) | `hold_frames=0` — **disabled** |

Windows enables hold only for tracks passing strict quality checks (duration
≤ 2 frames, pixel count in bounds, eigenratio < 4.0, signal > 0.095). The
initial Linux placeholder lacked these checks and caused scroll-brake
behavior, so hold is off by default.

## Touch detection threshold (+0x958)

| Offset | DLL value | Meaning |
|---|---:|---|
| +0x958 | 0.17 | Touch detection as a fraction of peak signal |

**Not standalone** — requires the per-cycle gain adaptation pipeline
(FUN_180600820), which is runtime-populated from device firmware. Cannot be
used directly in Linux.

## Peak isolation gate (+0x1FE)

The detector qualifies each candidate against the raw byte plane (see
[Touch Pipeline](Pipeline)): it is **rejected when any of the four cells five
cells away on the cardinal axes reads below 135**. The threshold and the
distance are hard-coded in the detector, not read from the table.

| Encoding | Field | Value |
|---|---|---:|
| raw byte (u8) | +0x1FE | 135 |
| fixed point (u16) | +0x1F8 | 1000 (= 0.1 × 10⁴) |
| signal domain (float) | — | 0.1 |

A neighbour at or above 135 is *quiet* (`c590 ≤ 0.10025`, i.e. ≤ 1002 fixed
point); the test rejects the candidate when the deflection has not decayed
below that level within five cells. Samples that fall outside the raster
(x < 5, x + 5 ≥ columns, y < 5, y + 5 ≥ rows) **pass** — edge candidates are
never rejected by this gate.

## Peak acceptance and scoring kernel (+0x04, +0x08, +0x14)

Surviving candidates are scored with a **11 × 11 float kernel** stored at
+0x14 (121 floats, Σ = 1.5513; positive ring, negative corners, near-zero
core): `score = Σ kernel·c590` over the window, where columns outside the
raster contribute 0 and the window rows must fit inside the raster.

| Offset | Value | Meaning |
|---|---:|---|
| +0x04 | 0.04 | Peak acceptance in the alternate state |
| +0x08 | 0.05 | Peak acceptance, default |

Only the frame's highest-scoring candidate is published, and only when its
score reaches the threshold. The pipeline publishes **one peak per frame**;
multi-contact logic happens in the tracker that consumes it (see
[Multi-touch (Experimental)](Multi-touch-Experimental)).

## Published heat map (+0xE92, +0xE93)

The DLL publishes a decimated copy of the raster for the OS:

| Field | Value | Effect |
|---|---:|---|
| +0xE92 | 4 | Decimation factor `D`; rows and columns are divided by it (SL4: 12 × 18), shift = `log2(D)` = 2 |
| +0xE93 | 1 | Enable flag (the decimated copy is skipped when zero) |
| +0xE92-derived | — | Cell values are divided by 10 before publication |

Destination is a 5616-byte buffer in the device object (offset +0xAF80) with
a **72-byte row stride**, zeroed before each write.

## Tracking constants

| Source | Value | Meaning |
|---|---:|---|
| image constant | 36.0 | Squared **handover radius**: a new frame peak within 6 cells of a track's prediction takes the track over; beyond that the track keeps its position |
| image constant | 0.04 | **Per-track liveness**: a track that was not updated survives while its own cell reads `c590 ≥ 0.04` (400 fixed point), an absolute test — not a comparison with the strongest contact |
| +0xC98 | 36.0 | Coalescing / merge threshold (same value, see above) |

## Extended re-search (present, inactive)

The record carries a second-search family that widens the search around the last
published peak when a frame yields nothing:

| Offset | Value | Meaning |
|---|---:|---|
| +0x1FA | 2 | Maximum candidate count that allows the re-search |
| +0x1FB | 3 | Minimum consecutive empty frames before it arms |
| +0x1FC | 5 | Maximum consecutive empty frames it tolerates |
| +0x1FD | 0 | Enable flag — **0 in every device record**, so the path arms nothing |

## Runtime-populated values (unavailable)

Zeros in the static DLL binary, populated at runtime after gain calibration;
not portable without equivalent device communication:

| Range | Purpose |
|---|---|
| +0xE60–0xE98 | Per-cycle gain calibration data |
| +0xD40–0xD78 | Mahalanobis classification matrix (10×11 floats) |
| +0xEA0–0xEC0 | Background noise tracking |

## Module parameters

Every Linux module parameter, its class and its load-time default. Defaults are
read from the driver source; the raw controls are load-time only, so every
profile is a fresh module load. The classes match the parameter contract in
`docs/PARAMETERS.md`.

| Parameter | Class | Default | Notes |
|---|---|---|---|
| `raw_mode` | Standard safety | `0` | The only normal profile control; `0` selects standard HID and skips the raw activation/pipeline |
| `sl4a_debug_level` | Diagnostic | `0` | Log verbosity: 0=errors, 1=transitions, 2=per-frame, 3=full hex |
| `debug_trace` | Diagnostic | `0` | Controller-hop trace: 0=off, 1=lifecycle, 2=SPI segments, 3=read data |
| `std_liveness_ms` | Diagnostic | `0` | Standard-mode startup liveness window (ms); logs only, never recovers |
| `raw_input_beta` | Experimental activation | `0` | Publish beta multi-touch from raw CapImg frames |
| `skip_getfeat` | Experimental activation | `1` | Skip the standard-mode feature-read handshake (the raw Report ID 6 read still runs) |
| `getfeat_delay_ms` | Experimental activation | `0` | Delay (ms) between RPT_DESC and GET_FEATURE |
| `setfeat_speed_hz` | Experimental activation | `0` | SPI clock (Hz) for the SET_FEATURE write only; 0 = bus default |
| `wire_double_opcode` | Experimental activation | `0` | `1` sends the legacy doubled leading opcode on every sequencer frame |
| `setfeat_no_double` | Experimental activation | `0` | Deprecated alias: `1` sends SET_FEATURE without the doubled opcode |
| `read_frame_variant` | Experimental activation | `1` | Read-approval shape: 0=reference, 1=legacy five-byte, 2=both |
| `acpi_probe_power_cycle` | Experimental activation | `0` | ACPI `_PS3`→`_PS0` power cycle at probe |
| `sync_timeout_ms` | Experimental activation | `6000` | Timeout (ms) for synchronous requests; clamped to [100, 60000] |
| `stream_watchdog_ms` | Experimental activation | `2000` | Runtime streaming watchdog interval (ms); 0=disable |
| `stream_watchdog_max_retries` | Experimental activation | `3` | Max re-init retries before giving up |
| `skip_vendor_stop` | Experimental activation | `0` | Omit the pre-`DESCREQ` `vendor_stop` + `D2`/`D0` preamble |
| `raw_fallback_on_reset` | Experimental activation | `0` | First poller `RESET_RSP` gives up to the hardcoded fallback descriptors |
| `raw_pre_desc_reg0` | Experimental activation | `0` | Pre-`DONE` reads go to register 0 (`input_register`) instead of the `{3, 0x0A}` selector |
| `raw_b1f8109_preset` | Experimental activation | `0` | One switch restoring the `b1f8109` raw dialect |
| `std_liveness_recover` | Experimental standard-mode recovery (issue #4) | `0` | Run ACPI recovery when the liveness check sees no activity after `DONE` (needs `std_liveness_ms`) |
| `skip_std_getfeat` | Experimental standard-mode recovery (issue #4) | `0` | Answer feature GET_REPORT with `-EOPNOTSUPP` in standard HID |
| `wait_reset_kick_ms` | Experimental standard-mode recovery (issue #4) | `0` | Kick discovery (one `DESCREQ`) when no IRQ edge since power-up/resume; 0=disable |
| `blob_min_weight` | Experimental raw pipeline | `1000` | Minimum blob signal rise |
| `ema_alpha` | Experimental raw pipeline | `2` | Position EMA smoothing |
| `dfa_data_offset` | Experimental raw pipeline | `0` | DFT antenna frame data offset in bytes (0 = decoded raster) |
| `ghost_dist` | Experimental raw pipeline | `6` | Coalescence radius (cells) |
| `grid_cols` | Experimental raw pipeline | `0` | Grid columns; 0 = per-device default |
| `grid_rows` | Experimental raw pipeline | `0` | Grid rows; 0 = per-device default |
| `blob_debounce` | Experimental raw pipeline | `3` | New-contact debounce (frames) |
| `blob_lift_frames` | Experimental raw pipeline | `3` | Missed frames before a contact lifts |
| `hold_frames` | Experimental raw pipeline | `0` | Contact hold grace frames (disabled) |
| `pre_assoc_ratio` | Experimental raw pipeline | `0` | Pre-association ratio ×1000 (disabled) |
| `blob_max_distance` | Experimental raw pipeline | `3` | Slot reassignment distance (cells) |
| `invert_x` | Experimental raw calibration | `0` | Invert the X axis |
| `invert_y` | Experimental raw calibration | `0` | Invert the Y axis |
| `swap_xy` | Experimental raw calibration | `0` | Swap the X/Y axes |
| `calib_scale_x` | Experimental raw calibration | `0` | X scale ×1000 (0 = derived from the grid) |
| `calib_scale_y` | Experimental raw calibration | `0` | Y scale ×1000 |
| `calib_offset_x` | Experimental raw calibration | `0` | X pixel offset |
| `calib_offset_y` | Experimental raw calibration | `0` | Y pixel offset |

## Per-device geometry

The DLL values were extracted from the SL4 (`MSHW0231`) pipeline. The SL3
AMD controller (`MSHW0162`) runs the same pipeline with a different sensor
grid selected by ACPI ID at probe: 78×52 (4056 CapImg samples) instead of
72×48 (3456), and a 33-frame resting baseline instead of 30. See
[Hardware](Hardware) and [Multi-touch (Experimental)](Multi-touch-Experimental).
