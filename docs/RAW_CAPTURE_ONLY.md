# Passive Raw Capture

`raw_capture_only=1` is an acquisition aid, not a raw-mode switch. It keeps the
normal, verified descriptor-discovery path and never sends GET_FEATURE,
SET_FEATURE, vendor-init, raw watchdog retries, direct GPIO, reset, or power
commands for capture.

If the controller independently emits a complete V0 DATA body whose envelope is
exactly `ce 10 0c` and whose aligned body length is 4304 bytes, the driver:

1. copies the unmodified 4304-byte body to a bounded ring of eight slots;
2. records a sequence number and monotonic timestamp; and
3. returns without HID delivery, CCL, calibration, coordinate conversion, or
   multitouch event generation unless the separate beta gate is enabled.

All other reports retain their standard HID handling. An absent raw frame is a
neutral observation and must not trigger retries, selector changes, or another
command.

## Interface

With the module loaded in capture-only mode, each SPI device exposes:

| File | Access | Meaning |
| --- | --- | --- |
| `raw_capture_status` | read-only text | Capture count, invalid count, ring position, and the sequence/timestamp of every slot. |
| `raw_transition_status` | read-only text | One-shot GET scheduling, send, response, skip and reset counters. |
| `raw_transition_response` | read-only text | Complete body from the one-shot GET response, up to 256 bytes. |
| `raw_capture` | read-only binary | Eight fixed, ordered 4304-byte V0-body slots, for a total of 34432 bytes. A zero sequence in status marks a slot with no captured frame. |
| `isolated_set_status` | read-only text | State, one-shot GET/SET counters, timeout/reset result, and ring layout for the separately gated SET experiment. |
| `isolated_set_ring` | read-only binary | Eight fixed records containing timestamp, transport type/length, four-byte header, and up to 512 bytes of post-SET body. |

The binary slots are raw bodies, not HID reports and not payload-only buffers:

```text
offset 0..1:  semantic total length, little-endian (must be 4302)
offset 2:     content ID (must be 0x0c)
offset 3..4301: opaque 4299-byte content
offset 4302..4303: aligned tail, preserved without interpretation
```

`tools/extract_raw_capture.py` validates a saved snapshot, reports SHA-256
hashes, and can write only valid slots as `.v0` files. It does not access a
device. The sysfs binary file is root-readable, so validate it directly with
`sudo python3 tools/extract_raw_capture.py /sys/bus/spi/devices/spi-MSHW0231:00/raw_capture`.

The installer prepares passive capture for the next boot only when invoked as
`sudo RAW_CAPTURE_ONLY=1 ./tools/install.sh`. Its resulting modprobe setting is
`raw_mode=N raw_capture_only=Y`; it does not activate raw mode.

## Multitouch Pipeline

`raw_input_beta=1` activates the Surface multitouch pipeline, a kernel-side
replica of the Windows TouchPenProcessor0C19 tracker. It requires
`raw_capture_only=1` and `raw_mode=1`. Every passively arriving `0x0c` CapImg
body is validated and decoded into a 72×48 heatmap raster, then processed
through the pipeline:

1. **Peak detection** (5-cell radius, min rise 500)
2. **Flood-fill centroid** (signal-weighted, noise-floor subtraction, ×256 fixed-point)
3. **Eigenellipsis** (per-blob second moments for touch major/minor/orientation)
4. **Hungarian global assignment** (3-cell association radius, matching Windows 0.545)
5. **Track update** with EMA weight filter, 10-sample history ring, moving-average
   smoothing, stationary lock, and lift history lookback
6. **Post-emission coalescence** (suppress weaker contacts within 6 cells)

The pipeline registers a dedicated input device named "Surface Touchscreen"
while the standard HID touch device remains active for Report ID 0x40 events.
See `docs/PIPELINE.md` for the complete stage-by-stage mapping.

No live raw stream is necessary; the pipeline operates on passively arriving
frames. Malformed CapImg bodies increment `raw_input_invalid` and produce no
input event.

## One-Shot Transition

`raw_transition_once=1` is retained for controlled diagnostics only. It sends
exactly one frozen Windows GET selector-4 vector. A GET_FEATURE response is
recorded and normal HID runtime resumes; it never sends SET_FEATURE, retries,
or a recovery command. If no response arrives within one second, it returns to
normal HID runtime without sending another message. It is not a recovery or
activation mechanism.
The controller may still become silent; reboot is the only supported recovery.

The first authorized one-shot capture is recorded in
`RAW_TRANSITION_EVIDENCE.md`. The capture artifacts remain opaque and must not
be used as coordinate or geometry evidence.

Use `raw_transition_status` after an authorized experiment to distinguish a
delayed GET skipped because the sequencer changed state, a GET write failure,
a missing GET response, and a received GET response. These counters observe
the one-shot path only; they do not schedule a retry or change device state.

## Isolated SET Harness

`isolated_set_test=1` is disabled by default, is read-only at runtime, and is
accepted only with `raw_capture_only=1 raw_transition_once=0`. It is a separate
boot-time experiment, not an extension of `raw_transition_once` or legacy raw
mode. After normal HID runtime has been established, it waits the Windows-observed
5.9 s settle interval, sends the controller-boundary GET ID6 source vector,
waits for its response, waits 4.5 to 5.5 ms, then sends exactly one
15-byte SET source vector. The AMD controller consumes the first source byte, so
the SET FIFO payload is the frozen 14-byte Windows vector.

After the SET, the harness enters a 45-second passive observation state, longer
than the approximately 36-second Windows delay before the first observed `0x0c`
frame. HID
`raw_request` and `output_report` writes return `-EBUSY`; no retry, vendor-init,
DESCREQ, watchdog, reset/recovery, or power write is scheduled. Types 1, 3, 4,
and 5 are captured in `isolated_set_ring`; type 1 still follows normal HID
delivery except for the existing `0x0c` passive capture rule. A reset is terminal
and does not cause a recovery write. The timeout only ends observation; the
armed no-recovery lock remains until driver cleanup. The general ring retains
eight records and reports overwrites; the raw ring independently retains the
last eight complete `0x0c` bodies.

## Safety Contract

- `raw_mode=1` is the default and enables passive observation of arriving
  CapImg frames; it does not send feature commands or change device state.
- `raw_capture_only=1` does not make a raw frame appear. It is safe to leave
  enabled while gathering observations, but cannot establish selector semantics
  or raw-mode causality.
- Any complete V0 envelope marked `0x0c` is never sent to
	`hid_input_report()` in capture-only mode. `raw_input_beta=1` activates
	the Surface multitouch pipeline and emits contacts via a dedicated input
	device ("Surface Touchscreen") with strict CapImg validation. A wrong
	raw length or malformed CapImg never produces an input event.
- The pipeline replicates the Windows Surface TouchPenProcessor0C19 tracker:
	peak detection, flood-fill centroid, Hungarian global assignment, EMA
	weight filter, stationary lock, lift history lookback, and post-emission
	coalescence. See `docs/PIPELINE.md` for the full stage-by-stage mapping.
- The ring overwrites its oldest slot after eight accepted frames. Read status
  before and after retrieving the binary file to detect concurrent overwrite.
- Hardware testing remains deferred. The offline test suite is the only
  validation performed by this change.
