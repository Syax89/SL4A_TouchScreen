# Next Steps

## Current State

- Standard HID touch and pen are the supported runtime path.
- `raw_capture_only=1` only preserves complete independently arriving V0 `0x0c`
  bodies; it does not request raw mode.
- `raw_input_beta=1` can decode a validated CapImg 72 by 48 raster only when
  such a body arrives. No live raw stream is currently available after normal
  Linux startup.
- The exact Windows GET_FEATURE vector was sent in a controlled one-shot test
  and produced no GET_FEATURE response. No SET_FEATURE was sent.

## Priorities

1. Compare the Linux `spi_hid_seq_write` and AMD controller transaction path
   with the recorded Windows transaction at the controller boundary. The
   software feature buffer now matches the frozen Windows vector, but the
   device does not respond.
2. Keep raw transition work diagnostic-only. A future authorized one-shot must
   read `raw_transition_status` and preserve its capture ring before any
   recovery. Do not scan selectors, retry feature commands, or use direct GPIO.
3. Move CapImg contact extraction into deterministic offline tests. Validate
   baseline, thresholding, connected components, slot assignment, lift decay,
   and coordinate error against the retained labelled captures before treating
   beta output as usable input.
4. Keep the passive beta separate from standard HID until duplicate events,
   coordinate error, and false contacts are measured on a live raw stream.
5. Continue cleanup and upstream preparation only for the stable HID transport
   and controller paths.

## Historical Research

The chronological reverse-engineering record and retained experiments remain in
`GROUND_TRUTH.md`, `RAW_TRANSITION_EVIDENCE.md`, `decomp/`, and capture-analysis
reports. They are evidence, not current runtime instructions.
