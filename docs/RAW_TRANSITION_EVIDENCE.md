# One-Shot Raw Transition Evidence

## 2026-07-16

### Instrumented One-Shot Follow-Up

The later instrumented one-shot first exposed a driver scheduling bug: the
required 5900 ms delay let normal HID setup advance the sequencer from state 2
to state 4, so the GET worker skipped its request. After allowing the stable
state-4 path, the driver sent the frozen ten-byte Windows GET vector exactly:

```text
02 00 00 03 42 00 04 03 00 06
```

The write completed without an SPI error, but no `GET_FEATURE_RESP`, V0 `0x0c`
body, or reset response arrived. The driver remained in state 5 until it was
explicitly reloaded into passive mode. No SET_FEATURE was sent. This rules out
the one-byte duplicated-opcode bug in the one-shot buffer as the immediate
cause of the missing response; it does not prove the Linux controller path is
wire-equivalent to the historical Windows transaction.

### 2026-07-18 Controller-Boundary Correction

The first Linux controller trace proved that the ten-byte source buffer was
not the ten-byte FIFO payload observed on Windows. `spi-amd` consumes byte zero
as controller opcode `0x02`; it logged only these nine FIFO bytes:

```text
00 00 03 42 00 04 03 00 06
```

The normal DESCREQ succeeds because its source buffer duplicates that opcode:
the controller consumes the first byte and its FIFO receives the Windows
payload starting with `02`. The GET-only transaction now uses the same
controller-boundary form, an eleven-byte source buffer whose FIFO payload is:

```text
02 00 00 03 42 00 04 03 00 06
```

This corrects a local framing defect. It does not prove selector semantics or
authorize SET_FEATURE.

### 2026-07-18 GET Response Decoding

With the corrected FIFO, GET_FEATURE_RESP arrived about 2 ms after the write.
Its 129-byte transport body has five prefix bytes followed by semantic length
`0x007a`, content ID `0x06`, and 119 bytes of content. The report descriptor
defines Report ID `0x06` as the 119-byte `DeviceMode` Feature report.

The observed Windows SET vector is a different report: its output body decodes
as SET_FEATURE with content ID `0x05` and three data bytes `01 00 00`; the
descriptor defines Report ID `0x05` as vendor Usage `0xc8`. Therefore GET ID 6
and SET ID 5 are correlated in time but are not a request/response pair. The
meaning and post-SET completion contract of Report ID 5 remain unproven, so
SET_FEATURE remains disabled.

### Isolated SET Harness Template Correction

The first isolated harness run sent a malformed SET body with semantic length
`0x0000` because its local source template omitted the captured `0x04` byte.
The run must not be interpreted as a SET result, even though standard `0x08`
DATA frames followed. The template is now checked against the complete Windows
FIFO payload `02 00 00 03 82 00 03 04 00 05 01 00 00 00` before any future run.

The explicitly authorized one-shot experiment booted with:

```text
raw_mode=N
raw_capture_only=Y
raw_transition_once=Y
```

Observed read-only state after the transition:

```text
ready
reset_rsp=2
device_desc=1
rpt_desc=1
getfeat_resp=1
frames_dropped=0
```

The passive ring accepted 1616 complete V0 `0x0c` bodies with zero invalid
frames. The slot timestamps show a cadence of approximately 10 ms. All captured
frames have the expected 4304-byte aligned body, semantic `total_len=4302`,
4299-byte opaque content, and tail `b0 76`.

Saved, SHA-256-identified artifacts:

| Directory | Snapshots | Unique bodies | Result |
| --- | ---: | ---: | --- |
| `captures/raw-20260716-one-shot-baseline/` | 100 | 8 | valid V0 `0x0c` bodies |
| `captures/raw-20260716-one-shot-interaction/` | 397 | 8 | same eight body hashes |

The second capture has no independent contact label and has the same hashes as
the first. It cannot demonstrate that any byte represents a touch, position,
geometry, or calibration value.

## Structural Comparison

`tools/analyze_captured_raw_frames.py` compares retained bodies against the
hash-pinned Windows corpus. Both saved capture sets have these reproducible
properties:

- all eight content hashes differ from every Windows content hash, but each has
  a nearest Windows body at exactly 23 differing bytes out of 4299 (99.4650%
  byte equality);
- the eight retained local contents differ pairwise at only 6--10 bytes;
- only 10 of 4299 content offsets vary among the eight bodies: `0`, `1`,
  `3494`, `3496..3498`, `3577..3578`, `3582`, and `3588`;
- the Windows corpus has 1422 unique `0x0c` bodies, so the eight retained local
  bodies are not evidence of a complete periodic stream.

These facts strengthen the framing match and locate a small state-dependent
boundary. They do not identify fields, a scan counter, a raster, touch state,
or coordinates. The initial collector kept a deduplicated set only; its updated
form now records ordered observations for future separately labelled sessions.

## Guided Interaction Capture

`captures/raw-20260716-one-shot-guided/` was collected with the local visual
guide: idle, centre hold, horizontal drag, lift, upper-left hold, and
lower-right hold. It contains 2913 snapshots, 23456 ordered slot observations,
and 3572 unique complete bodies.

| Phase | Observations | Unique bodies | Variable content offsets | New offsets versus idle |
| --- | ---: | ---: | ---: |
| Idle | 3928 | 8 | 10 | 0 |
| Centre hold | 3864 | 913 | 3479 | 3478 |
| Horizontal drag | 3872 | 861 | 3478 | 3477 |
| Lift | 3912 | 58 | 3473 | 3475 |
| Upper-left hold | 3872 | 890 | 3479 | 3478 |
| Lower-right hold | 3856 | 876 | 3478 | 3477 |

The dominant observed change range is content offsets `25..3480`, exactly 3456
bytes. Smaller changing metadata/footer ranges also occur at content offsets
`0..1`, `3494..3499`, periodic offsets `3520..3548`, `3577..3582`, and
`3588..3589`.

This is strong evidence that user-labelled screen interaction changes the
3456-byte region, and that lift retains a decaying or stateful response for at
least the captured ten-second window. It does not prove a 72x48 raster, an axis
orientation, one-byte cell semantics, or a coordinate transform. The labels are
human-timed rather than an independently timestamped physical oracle.

## Grid-Ordering Hypothesis

The separately guided 3x3 corpus is stored in
`captures/raw-20260716-one-shot-grid/`. `test_raw_grid_hypotheses.py` computes
the median absolute delta from idle over `content[25..3480]` and tests simple
row-major shapes by correlating each target's delta-energy centroid with its
known screen position.

| Candidate columns x rows | X correlation | Y correlation |
| --- | ---: | ---: |
| 72 x 48 | 0.99894 | 0.99934 |
| 48 x 72 | -0.98441 | 0.99936 |
| 288 x 12 | 0.82640 | 0.99958 |
| 12 x 288 | 0.98249 | 0.99944 |

The 72-column, 48-row row-major hypothesis is the best tested simple ordering:
the observed delta centroids move right for right targets and down for lower
targets. This is evidence for a 72x48 response layout with positive X to the
right and positive Y down in the captured ordering.

It remains a hypothesis rather than a deployed decoder. The centroid spans only
part of the response region, alternative transforms have not been exhaustively
searched, and the mapping needs an affine fit validated against held-out screen
targets before it can be used for input events.

## Affine Hold-Out Result

The first affine evaluation incorrectly treated the visual 3x3 guide targets
at `x=15/50/85%`, `y=20/50/80%` as physical screen edges. Re-evaluation with
their actual CSS positions and only the final three seconds of each eight-second
hold gives an internal leave-one-out RMS error of 2.57% and maximum 4.20%.

The five separate intermediate targets in
`captures/raw-20260716-one-shot-holdout/` then give a held-out RMS error of
9.16% and maximum 15.37%. This supports the centroid-based 72x48 response as a
useful calibration signal, but the non-uniform errors near upper/left edges
still make a global affine map unsuitable for emitted input coordinates.

Next calibration work must use more target positions, a stabilisation interval
before each sampling window, separately measured lift decay, a non-linear edge
model, and held-out validation. No coordinate transform from these captures may
be emitted as an input event.

## Lift Decay

`captures/raw-20260716-one-shot-decay/` uses 30 seconds idle, 15 seconds of a
stable centre hold, and 30 seconds after lift. The median interaction-region
delta energy was 0 at idle and 1.1317 during the hold. It fell below the 5%
recovery threshold 0.450 seconds after lift and remained below it for at least
one subsequent second.

Subsequent labelled target captures must therefore wait at least one second
after lift before establishing their next idle baseline. This timing applies to
the current device/session and is an empirical collection constraint, not a
protocol guarantee.

After preserving the corpus, persistent configuration was returned to
`raw_mode=N raw_capture_only=Y`. A later reboot will not repeat the raw
transition unless explicitly configured again.
