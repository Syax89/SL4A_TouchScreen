# Reverse Engineering

This driver was developed by observing how the reference Windows touch stack
brings the Surface Laptop 4 (AMD) panel up and drives it, then reimplementing
that behaviour from scratch on Linux. This page describes the reference model,
how the driver was derived and validated, and the gaps that are still open. The
SL3 AMD panel (`MSHW0162`) was added later from the same protocol with geometry
values contributed and tested on real hardware.

## The reference model

The reference implementation splits the work across three layers:

```
User mode
  a touch processor          ← touch pipeline: configuration, detection, tracking
    │
Kernel mode
  a HID-over-SPI transport   ← V0 framing, descriptors, enumeration, reset
  an SPI controller driver   ← AMD FCH SPI controller transactions
```

The Linux driver mirrors this split: `spi-amd` owns the controller, `spi-hid`
owns the V0 transport and sequencer, and the host-side pipeline in
`mshw0231-raw.c` replaces the user-mode processor (see
[Architecture](Architecture) and [Touch Pipeline](Pipeline)).

## How the driver was derived

1. **Bus behaviour** — the reference's host-to-device frames and its
   enumeration order were captured and reproduced byte for byte; every
   sequencer frame is defined once and asserted in `tests/wire_frames_test.c`.
2. **Register behaviour** — the AMD FCH SPI controller's transaction
   sequence, FIFO geometry and speed tiers were reproduced from the observed
   controller traffic (see [Hardware](Hardware) and [Wire Protocol](Wire-Protocol)).
3. **Pipeline behaviour** — the touch pipeline's stages (baseline, signal
   detection, peak selection, connected components, assignment, slot
   emission) were reproduced as host-side signal processing, with every
   constant in `driver/mshw0231-raw-constants.h` (see [Touch Pipeline](Pipeline)).

## How it was validated

- the V0 protocol against the reference's own enumeration frames and the
  HID-over-SPI framing rules;
- the pipeline against synthetic heatmap fixtures replayed through the **real**
  pipeline code (`tests/fixtures/raw-replay/`, driven by
  `raw_pipeline_replay_test`), and against mathematical properties of the
  fixed-point stages;
- the transport against the real panel's answers on the SPI bus.

## Known gaps

### Detector input plane

The per-frame detector reads a byte plane whose row stride is wider than the
image the panel fills. Scoring the raw CapImg raster with the reference's
kernel and peak threshold does not reproduce the published contacts.

**Impact**: the Linux peak criterion is the driver's own (rise threshold,
noise floor, weight gate); it cannot be aligned with the reference until the
plane's contents are understood.

### Per-frame list provenance

The reference frame carries two lists the detector consumes: a blob list and a
fixed-point list. Their writer is not established, and their values are not
found in the raw device payload region.

**Impact**: the Linux pipeline detects its own peaks and blobs; the reference
candidate source cannot be replicated directly.

### Centroid refinement

The published peak position is refined by a small centroid step whose moment
accumulators and clamping are not fully modelled.

**Impact**: reference positions can be compared only to the nearest cell.

### Contact classifier

The reference classifies each blob as `finger` / `palm` / `noise` with a
Mahalanobis matrix populated **at runtime** from device firmware, not present
in the static image.

**Impact**: the Linux driver treats all blobs as potential contacts; noise
rejection relies on the peak gate + velocity rejection instead. There is no
palm rejection.

### Per-cycle gain adaptation

The reference adjusts per-cell gain each frame from noise-floor measurements
and signal distribution, populated through the same firmware channel.

**Impact**: the Linux driver uses the static c590 table and a fixed noise
floor; accuracy may degrade over long sessions.

### Hold/lift policy

The reference hold is a quality-gated state machine (duration, pixel count,
eigenratio, signal level). The Linux driver uses frame-count-based lift
(`blob_lift_frames`) with hold disabled by default.

**Impact**: temporary contact loss during fast movement may be handled less
gracefully than on the reference.

## Further evidence

- the repo's `docs/` directory holds the protocol, register and pipeline
  references and the deeper investigation record;
- `tests/` holds the host suite, including the protocol, decoder and
  raw-pipeline replay tests.
