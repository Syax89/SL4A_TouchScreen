# Module Parameter Contract

No input behavior is release-qualified yet. This table classifies the current
parameter surface so the standard profile remains fail-closed while raw work is
reviewable and reproducible.

| Class | Parameters | Contract |
| --- | --- | --- |
| Standard safety | `raw_mode=0` | The only normal profile control. It selects standard HID and does not run the raw activation/pipeline. |
| Diagnostic | `debug_level`, controller `debug_trace` | Logging only. Both default to zero. |
| Experimental activation | `raw_input_beta`, `skip_getfeat`, `getfeat_delay_ms`, `setfeat_speed_hz`, `setfeat_no_double`, `acpi_probe_power_cycle`, `stream_watchdog_ms`, `stream_watchdog_max_retries` | Can change feature traffic, power sequencing, recovery, or raw input publication. Never set by the standard profile. |
| Experimental raw pipeline | `blob_min_weight`, `ema_alpha`, `dfa_data_offset`, `touch_signal_mode`, `touch_threshold_pct`, `ghost_dist`, `grid_cols`, `grid_rows`, `blob_debounce`, `blob_lift_frames`, `hold_frames`, `pre_assoc_ratio`, `blob_max_distance` | Applies only to decoded raw frames. Geometry and tracker behavior are not qualified. |
| Experimental raw calibration | `invert_x`, `invert_y`, `swap_xy`, `calib_scale_x`, `calib_scale_y`, `calib_offset_x`, `calib_offset_y` | Applies only to raw contact publication. |

Raw calibration and pipeline controls are read-only after module load. Run a new
controlled profile for every parameter set; do not mutate a live touch stream.

`getfeat_delay_ms` defaults to `0` (immediate). About 5900 ms is a Windows
observation, not a Linux default. `touch_signal_mode` and
`touch_threshold_pct` are currently unused placeholders.
