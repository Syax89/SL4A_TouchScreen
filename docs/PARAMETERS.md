# Module Parameter Contract

No input behavior is release-qualified yet. This table classifies the current
parameter surface so the standard profile remains fail-closed while raw work is
reviewable and reproducible.

| Class | Parameters | Contract |
| --- | --- | --- |
| Standard safety | `raw_mode=0` | The only normal profile control. It selects standard HID and does not run the raw activation/pipeline. |
| Diagnostic | `debug_level`, controller `debug_trace`, `std_liveness_ms` | Logging only. All default to zero. |
| Experimental activation | `raw_input_beta`, `skip_getfeat`, `getfeat_delay_ms`, `setfeat_speed_hz`, `setfeat_no_double`, `acpi_probe_power_cycle`, `sync_timeout_ms`, `stream_watchdog_ms`, `stream_watchdog_max_retries` | Can change feature traffic, power sequencing, recovery, or raw input publication. Never set by the standard profile. |
| Experimental standard-mode recovery (issue #4) | `std_liveness_ms`, `std_liveness_recover`, `skip_std_getfeat`, `wait_reset_kick_ms` | Off by default, never set by the installer. `std_liveness_ms` only logs: it counts controller activity (IRQs) in the window after `DONE`, so a healthy idle device reports silence exactly like a dead one. `std_liveness_recover` additionally runs the existing ACPI recovery when no activity arrives, once per boot and once per resume; `skip_std_getfeat` answers feature reads with `-EOPNOTSUPP` in standard mode so nothing is written to SPI for a feature query. `wait_reset_kick_ms` only kicks discovery (a `DESCREQ` write, no power sequencing) when the controller has produced no IRQ edge since the timer was armed: at most one successful kick per entry into `WAIT_RESET`, with up to three write attempts and the attempts stopping at the first write that goes out. The clock starts only once the IRQ is armed — after the probe's settle window — so an interval shorter than that window cannot kick a device that was never given the chance to answer. After the kick the descriptor poller keeps reading every 100 ms, so a dead controller is polled rather than left silent. Field-tested before any of these become defaults. |
| Experimental raw pipeline | `blob_min_weight`, `ema_alpha`, `dfa_data_offset`, `touch_signal_mode`, `touch_threshold_pct`, `ghost_dist`, `grid_cols`, `grid_rows`, `blob_debounce`, `blob_lift_frames`, `hold_frames`, `pre_assoc_ratio`, `blob_max_distance` | Applies only to decoded raw frames. Geometry and tracker behavior are not qualified. |
| Experimental raw calibration | `invert_x`, `invert_y`, `swap_xy`, `calib_scale_x`, `calib_scale_y`, `calib_offset_x`, `calib_offset_y` | Applies only to raw contact publication. |

Raw calibration and pipeline controls are read-only after module load. Run a new
controlled profile for every parameter set; do not mutate a live touch stream.

`getfeat_delay_ms` defaults to `0` (immediate). Windows traces measure a
~3.6 s device settle before it answers feature queries (the original
protocol doc cited ~5.9 s, not reproducible from trace rows); neither
figure is a Linux default.

`sync_timeout_ms` defaults to `6000` and bounds every synchronous request
(report-descriptor and feature queries). 6000 ms covers the measured ~3.6 s
settle plus the ~5.9 s documented worst case; the pre-fix hardcoded 1000 ms
timed out on a cold-boot feature query and killed the touchscreen (issue
#4). A feature-query timeout is non-fatal — the input stream is IRQ-driven
and independent of feature queries — while a descriptor-request timeout
still triggers recovery.

`touch_signal_mode` and
`touch_threshold_pct` are currently unused placeholders.

## Per-device defaults (selected by ACPI ID at probe)

The probe logs a `device config:` line in dmesg with the active geometry.
`grid_cols`/`grid_rows` are set from this config when they are left at 0:

| Device | ACPI ID | Grid | CapImg raster samples | Baseline frames | Baseline recovery EMA alpha |
| --- | --- | --- | --- | --- | --- |
| Surface Laptop 4 AMD | `MSHW0231` | 72×48 | 3456 | 30 | 7 |
| Surface Laptop 3 AMD | `MSHW0162` | 78×52 | 4056 | 33 | 7 |

The baseline recovery alpha (7 on both devices) is the Windows-documented
12.5% recovery rate: `new = (7·base + raw)/8`. The `ema_alpha` parameter is
position smoothing only and is unrelated (default 2).
