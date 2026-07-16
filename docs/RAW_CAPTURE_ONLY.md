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
| `raw_transition_status` | read-only text | One-shot GET/SET scheduling, send, response, skip and reset counters. |
| `raw_capture` | read-only binary | Eight fixed, ordered 4304-byte V0-body slots, for a total of 34432 bytes. A zero sequence in status marks a slot with no captured frame. |

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

## Passive Multitouch Beta

`raw_input_beta=1` is a separate, read-only boot-time gate and requires
`raw_capture_only=1`. It never changes the acquisition behavior: every `0x0c`
body is still captured first, and the driver emits input only after validating
the complete CapImg container and its single 72 by 48 RLV heatmap section.
Malformed CapImg bodies remain available in the raw capture ring but increment
`raw_input_invalid` and produce no input event.

No live raw stream is currently available after normal Linux startup, so this
path is conditional rather than a usable multitouch mode.

The beta registers a dedicated multitouch input device while the standard HID
touch device remains active. This may produce duplicate events and is intended
only for controlled testing. It does not request raw mode, suppress standard
HID, or retry when no raw frame arrives.

## One-Shot Transition

`raw_transition_once=1` is retained for controlled diagnostics only. In the
latest instrumented run the frozen Windows GET selector-4 vector was sent
without an SPI error, received no GET_FEATURE response, and therefore did not
send SET_FEATURE. It is not a recovery or activation mechanism.
The controller may still become silent; reboot is the only supported recovery.

The first authorized one-shot capture is recorded in
`RAW_TRANSITION_EVIDENCE.md`. The capture artifacts remain opaque and must not
be used as coordinate or geometry evidence.

Use `raw_transition_status` after an authorized experiment to distinguish a
delayed GET skipped because the sequencer changed state, a GET write failure, a
missing GET response, and a SET write failure. These counters observe the
one-shot path only; they do not schedule a retry or change device state.

## Safety Contract

- `raw_mode=1` is deprecated and deliberately inert. It logs a warning and
  cannot schedule the historical raw handshake.
- `raw_capture_only=1` does not make a raw frame appear. It is safe to leave
  enabled while gathering observations, but cannot establish selector semantics
  or raw-mode causality.
- Any complete V0 envelope marked `0x0c` is never sent to
	`hid_input_report()` in capture-only mode. `raw_input_beta=1` may instead
	decode it into the dedicated beta device after strict CapImg validation. A
	wrong raw length or malformed CapImg never produces an input event.
- The ring overwrites its oldest slot after eight accepted frames. Read status
  before and after retrieving the binary file to detect concurrent overwrite.
- Hardware testing remains deferred. The offline test suite is the only
  validation performed by this change.
