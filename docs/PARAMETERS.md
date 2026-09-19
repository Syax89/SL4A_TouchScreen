# Module Parameter Contract

The standard profile is the qualified profile; the raw pipeline is beta —
functional on hardware, still under field review — and is never set by the
standard profile. This table is the fail-closed contract over the whole
parameter surface: every parameter is listed exactly once, so the standard
profile stays closed while the raw path remains reviewable and reproducible.

| Class | Parameters | Contract |
| --- | --- | --- |
| Standard safety | `raw_mode=0` | The only normal profile control. It selects standard HID and does not run the raw activation/pipeline. |
| Diagnostic | `sl4a_debug_level`, controller `debug_trace` | Logging only. Both default to zero. |
| Experimental activation | `raw_input_beta`, `skip_getfeat`, `getfeat_delay_ms`, `setfeat_speed_hz`, `wire_double_opcode`, `setfeat_no_double`, `read_frame_variant`, `skip_vendor_stop`, `raw_fallback_on_reset`, `raw_pre_desc_reg0`, `raw_b1f8109_preset`, `acpi_probe_power_cycle`, `sync_timeout_ms`, `stream_watchdog_ms`, `stream_watchdog_max_retries`, `get_noread`, `raw_handshake_first_ms`, `raw_no_enable`, `raw_watchdog_teardown`, `std_raw_transition` | Can change feature traffic, power sequencing, recovery, or raw input publication. Never set by the standard profile, except `wire_double_opcode=1`, which both installed profiles carry (doubled DESCREQ). |
| Experimental standard-mode recovery (issue #4) | `std_liveness_ms`, `std_liveness_recover`, `skip_std_getfeat`, `wait_reset_kick_ms` | Off by default, never set by the installer. `std_liveness_ms` only logs: it counts controller activity (IRQs) in the window after `DONE`, so a healthy idle device reports silence exactly like a dead one. `std_liveness_recover` additionally runs the existing ACPI recovery when no activity arrives — once per silent episode (observed activity and resume restore the allowance, so a healthy idle device cannot be power-cycled repeatedly); `skip_std_getfeat` answers feature reads with `-EOPNOTSUPP` in standard mode so nothing is written to SPI for a feature query. `wait_reset_kick_ms` only kicks discovery (a `DESCREQ` write, no power sequencing) when the controller has produced no IRQ edge since the timer was armed: at most one successful kick per entry into `WAIT_RESET`, with up to three write attempts and the attempts stopping at the first write that goes out. The clock starts only once the IRQ is armed — after the probe's settle window — so an interval shorter than that window cannot kick a device that was never given the chance to answer. After the kick the descriptor poller keeps reading every 100 ms, so a dead controller is polled rather than left silent. None of these becomes a default before it is field-tested. |
| Experimental raw pipeline | `blob_min_weight`, `ema_alpha`, `dfa_data_offset`, `ghost_dist`, `grid_cols`, `grid_rows`, `blob_debounce`, `blob_lift_frames`, `hold_frames`, `pre_assoc_ratio`, `blob_max_distance` | Applies only to decoded raw frames. Geometry and tracker behavior are not qualified. |
| Experimental raw calibration | `invert_x`, `invert_y`, `swap_xy`, `calib_scale_x`, `calib_scale_y`, `calib_offset_x`, `calib_offset_y` | Applies only to raw contact publication. |

Raw calibration and pipeline controls are read-only after module load. Run a new
controlled profile for every parameter set; do not mutate a live touch stream.

### Wire format of the host→device frames

`wire_double_opcode` defaults to `0`, which sends exactly what the Windows stack
puts on the bus: one `0x02` opcode, and the constant `0C EE 5B` trailer on the
short command bodies (SET_POWER, SET_FEATURE Report ID 5). Setting
`wire_double_opcode=1` restores the legacy Linux form — the opcode sent twice
(`02 02 ..`) and a zeroed trailer — for A/B experiments. Note: both installer
profiles currently set `wire_double_opcode=1`; the single-opcode form is the
code default, not the installed default.

The frames live in `driver/spi-hid-wire-frames.h`; `tests/wire_frames_test.c`
compares every frame in both modes against the reference bytes
(`captures/wintrace/surface_init.csv`, TXN 634377432 onwards) and runs in
`make -C tests test`.

`setfeat_no_double` is deprecated and kept only so existing `modprobe.d` drop-ins
keep loading. `setfeat_no_double=1` asks for the SET_FEATURE frame without the
doubled opcode, which is the default now, so it only takes effect as an override
of `wire_double_opcode=1` — and then on the SET_FEATURE frame alone. Use
`wire_double_opcode=1` to experiment with the legacy form.

### Raw-mode regression triage

Four load-time-only knobs restore or bisect the raw dialect the panel last
answered on. All default to `0` and are set only for a labelled run.

- `skip_vendor_stop` skips the pre-`DESCREQ` teardown and power preamble
  (`vendor_stop`, D2, D0) in `spi_hid_vendor_init()`.
- `raw_pre_desc_reg0` skips the probe-time stream-register force and reads every
  pre-`DONE` frame from the descriptor's input register — still `0` until a
  descriptor parses, the register the panel answers on — instead of `{3, 0x0A}`.
- `raw_fallback_on_reset` gives up to the hardcoded fallback descriptors on the
  first poller `RESET_RSP` instead of draining the reset and retrying.
- `raw_b1f8109_preset` is the one-switch restore of that whole dialect: doubled
  writes on every frame (`02 02 ..`), the pre-`DONE` register-0 reads, the poller
  give-up, and no `STOP` frame (D2/D0 unchanged). It turns those behaviors on
  regardless of their own knobs, so it is not combined with the three above.

### Read-approval shape

`read_frame_variant` (default `1`, LEGACY) selects the read-approval frame:
`1` = the five-byte form (`0B <reg3> FF`, register in the address field) — the
only shape this panel answers; `0` = the reference's
nine-byte shape (register at offset 7), silent on this panel; `2` = both. Do
not flip it without a labelled run.

Header-read length is separate from the approval shape: pre-DONE header reads
are nine bytes in both modes (`spi_hid_hdr_len()`), sixteen only for the raw
DONE stream. A sixteen-byte pre-DONE read over-clocks the bare answers and
stalls discovery the same way a wrong approval does.

### Feature reads

`skip_getfeat` (default `1`, and what the installer's raw profile sets) skips the
standard-mode feature-read handshake: with it set, nothing parks in `WAIT_FEATURE`
waiting for a GET_FEATURE reply before the heatmap is enabled. It does **not**
suppress the raw-mode Report ID 6 configuration read, which is part of the
activation sequence (Windows reads it between the report descriptor and the
SET_FEATURE that enables the heatmap) and runs either way. That reply is kept in
`struct spi_hid_getfeat6` and logged as hex plus IEEE-754 values at debug level 2;
nothing acts on the values yet and a failed read never delays the handshake.

`getfeat_delay_ms` defaults to `0` (immediate). Windows traces measure a
~3.6 s device settle before it answers feature queries (the original
protocol doc cited ~5.9 s, not reproducible from trace rows); neither
figure is a Linux default.

`sync_timeout_ms` defaults to `6000` and bounds every synchronous request
this driver issues — feature queries; descriptor geometry is learned through
the DESCREQ sequencer path, not through synchronous requests. Probe clamps
the read-only value into `[100, 60000]`: a negative value would wrap
`msecs_to_jiffies()` into the far future and a `0` would turn every missed
response into an instant timeout. 6000 ms covers the measured ~3.6 s
settle plus the ~5.9 s documented worst case; the pre-fix hardcoded 1000 ms
timed out on a cold-boot feature query and killed the touchscreen (issue
#4). A feature-query timeout is non-fatal — the input stream is IRQ-driven
and independent of feature queries.

### Frame-age gates (open item)

The Windows PSDB record carries **two** frame-age gates: `+0x1fb = 3` and
`+0x1fc = 5`. We implement the `3` as `blob_debounce` (both count frames before a
new contact is published); the `5` has **no counterpart**. Its semantics are not
established — it could be a release/hold bound or a second age test — so it is
recorded here as an open item rather than guessed into a knob. `hold_frames`
therefore keeps its default of `0`; do not raise it to `5` until the Windows
meaning of `+0x1fc` is decoded. See `docs/WINDOWS-ALIGNMENT.md`.

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
