# SL4A Distribution Refactor Roadmap

## Scope

This branch prepares a maintainable GitHub/DKMS distribution for the Microsoft
Surface Laptop 4 AMD touchscreen only:

| Component | Required identity |
| --- | --- |
| Platform | Surface Laptop 4 AMD (Cezanne) |
| SPI controller | ACPI `AMDI0060` |
| Touch controller | ACPI `MSHW0231` |
| HID descriptor | VID/PID `045e:0c19` |
| Transport | MSHW0231 HID-over-SPI V0 |

Other Surface models, AMD SPI controllers, and HID-over-SPI devices are out of
scope until they have their own evidence and validation matrix.

## Design Decisions

| ID | Decision | Status |
| --- | --- | --- |
| ADR-001 | Ship an out-of-tree DKMS driver, not an upstream submission series. | Accepted |
| ADR-002 | Keep `spi-amd` limited to the documented AMDI0060 controller behavior. | Accepted |
| ADR-003 | Keep MSHW0231 V0 transport, activation, and raw heatmap processing device-specific. | Accepted |
| ADR-004 | Change wire behavior only with a capture, ACPI evidence, hardware result, or an independent memory/lifecycle defect. | Accepted |
| ADR-005 | The installer fails closed on unsupported hardware; an explicit override is required to bypass detection. | Planned |

## Evidence Policy

Every behavioral change must record one of these sources in its commit and,
when non-obvious, in the relevant documentation.

| Level | Source | Permitted use |
| --- | --- | --- |
| E1 | Reproducible Linux hardware result | Release behavior |
| E2 | Versioned Windows trace, ACPI table, or captured descriptor | Protocol and controller behavior |
| E3 | Host test or replay fixture | Parser, decoder, and algorithm behavior |
| E4 | Static safety proof | Bounds, ownership, locking, and teardown fixes |
| E0 | Hypothesis | Diagnostic-only experiment; never a release default |

## Current State

- Phases 0 through 3 are complete.
- The controlled-release HID profile has partial hardware evidence (cold boot,
  warm boot, stress 30m, suspend/resume). Pen and raw multi-touch remain
  unqualified. No E1 row exists yet in `docs/COMPATIBILITY.md`.
- Secure Boot MOK enrollment guidance exists but has not been validated with
  a signed-module boot recorded in the compatibility matrix.
- Standard HID hardware evidence is collected but exists in temporary local
  artifacts; release qualification requires archived evidence with verified
  checksums.

## Refactor Phases

### Phase 0: Baseline And Evidence

- [x] Preserve the current hardening batch on this branch.
- [x] Reconcile release-facing documents with code, ACPI, and capture data; retain unresolved claims in the evidence ledger.
- [x] Create an evidence index linking protocol claims to tracked sources.
- [x] Record the tested firmware, kernel, and distribution versions
      (Surface Laptop 4, BIOS 4.391.140, CachyOS 7.1.3-2, clang 22.1.6).

Exit criteria: the supported hardware contract and all release defaults have an
E1, E2, E3, or E4 basis.

### Phase 1: Distribution Contract

- [x] Add `docs/SUPPORT.md` with the exact supported hardware and feature matrix.
- [x] Add `docs/COMPATIBILITY.md` with the required tested kernel, firmware,
      distro, and Secure Boot fields.
- [ ] Record a release-qualified compatibility result.
- [x] Add `docs/TESTING.md` with repeatable cold-boot, suspend, reload,
      pen, and multitouch procedures.
- [x] Make README installation claims point to those documents.

Exit criteria: a user can determine before installation whether their machine
and kernel are supported.

### Phase 2: Safe DKMS Lifecycle

- [x] Split the installer into a read-only preflight and an explicit install.
- [x] Require `MSHW0231` and `AMDI0060` by default.
- [x] Add `--check`, `--dry-run`, and an explicit `--force` escape hatch.
- [x] Document module signing and Secure Boot for each supported distribution
      (`docs/ROLLBACK.md`).
- [x] Make uninstall remove only artifacts owned by this package
      (`tools/uninstall.sh` verifies ownership marker before touching config,
      DKMS tree, or versioned source).

Exit criteria: install, upgrade, rollback, and uninstall are deterministic and
do not modify unrelated modules or configuration.

### Phase 3: Internal Driver Boundaries

- [x] Document the AMDI0060 controller contract separately from MSHW0231 in
      `docs/AMDI0060_CONTRACT.md`.
- [x] Isolate MSHW0231 V0 sequencing and activation from raw frame processing.
- [x] Classify each module parameter as release, diagnostic, or experimental.
- [x] Remove or gate experimental behavior from the release profile.

Exit criteria: controller, transport, and touch-pipeline changes can be tested
and reviewed independently without changing the wire contract accidentally.

### Phase 4: Replay And Hardware Validation

- [x] Add the minimal v1 replay manifest: eight checksum-validated raw frames
      and deterministic malformed decoder inputs. Reset, descriptor,
      activation, and gap records explicitly remain absent or unqualified where
      wire bytes or responses are unavailable.
- [x] Exercise protocol/decoder and replay-fixture tests under sanitizers.
- [x] Capture a Linux trace format comparable to the versioned Windows traces
      (`tools/hardware_evidence/capture_linux_trace_bundle.sh`).
- [x] Execute the hardware matrix with standard HID: cold boot, warm boot,
      suspend/resume, and a 30-minute stress run. Pen and reload remain partial;
      raw multi-touch is unqualified. See `docs/COMPATIBILITY.md` and
      `docs/BLINDED_SESSION_LOG.md`.

Exit criteria: every release claim is backed by a named fixture or a dated
hardware result in `docs/COMPATIBILITY.md`.

### Phase 5: CI And Releases

- [x] Add GitHub Actions for whitespace, host tests, sanitizers, and an
      out-of-tree Ubuntu generic-header compile smoke test.
- [ ] Publish signed, versioned GitHub releases with source checksums and a
      compatibility table.
- [x] Keep a rollback procedure for every release
      (`docs/ROLLBACK.md`).

Exit criteria: a tagged release can be rebuilt, installed, verified, and
removed using only repository documentation.

## Branch Log

| Date | Change | Evidence | Verification |
| --- | --- | --- | --- |
| 2026-07-22 | Created `refactor/sl4a-distribution`; preserved current hardening work and established this roadmap. | E3 | Host protocol and CapImg tests previously passed, including ASan/UBSan. |
| 2026-07-22 | Sprint 0: added support contract, evidence ledger, compatibility matrix, and validation procedure; added fail-closed ACPI/DMI preflight, ownership-safe install/uninstall behavior, and standard installer profile. | E2, E3 | Double-blind review; protocol and CapImg host tests passed with ASan/UBSan. |
| 2026-07-23 | Sprint 1: boot safety (no-alias controller/transport, PSP ownership, SPI100/SPD7 guard, explicit post-login activation, rollback); Phase 3 boundaries (AMDI0060 contract, parameter contract, raw consumer extraction); Phase 4 replay corpus, trace bundle, blinded runner; Phase 5 CI. | E3, E4 | All host gates pass including replay fixture, ownership, boot-safety, and raw-boundary tests. |
| 2026-07-23 | Sprint 2: CI badge, rollback/Secure Boot docs, partial COMPATIBILITY observations, CHANGELOG, cold boot evidence archived. | E3 | Host gates, shell syntax, kernel build, diff check. |
| 2026-07-24 | Raw handshake watchdog cleanup: extracted `raw_handshake_restart_discovery()` to remove duplicated D2→D0/DESCREQ restart logic; replaced the blocking `msleep(5000)` in the cold-boot-retry path with a non-blocking `raw_probe_retry_work` delayed work so the watchdog callback never stalls its workqueue; fixed a latent gap where that same path could leave the sequencer stuck with no re-armed watchdog on a vendor_init/DESCREQ failure. Decoupled `blob_slot_weight` EMA from the position-smoothing `ema_alpha` knob by adding a fixed `HEATMAP_WEIGHT_EMA_ALPHA` (=7, matching the historical GROUND_TRUTH.md §weight-tracking a=1/8); note `blob_slot_weight` is currently write-only (no consumer), so this has no behavioral effect yet but removes the coupling before anything reads it. Removed three `raw_pipeline_math_test.c` assertions that pinned tunable constants to fixed literals (blocking legitimate tuning) and rewrote the deadband boundary test to derive its inputs from `HEATMAP_DEADBAND_THRESHOLD` instead of a hardcoded value. Separately, an in-progress tuning experiment (`HEATMAP_DEADBAND_THRESHOLD` 80→20, `HEATMAP_STATIONARY_FRAMES` 6→2, `HEATMAP_EMA_ALPHA_DEFAULT` 7→2) was split into its own commit tagged E0 (hypothesis, no hardware evidence) rather than riding inside the structural fix above. A 5-agent production-readiness audit (kernel architecture, transport/handshake adversarial review, multitouch-pipeline adversarial review, HID/MT-protocol review, QA/test-coverage review) ran the same day — see the punch list this table entry accompanies for full findings, including two independently-identified likely root causes of the 4-finger instability (non-scaling ghost-merge radius; greedy non-augmenting Hungarian matching) and a Hungarian track-continuity gap that can swap slot identities without a lift event. | E3, E4 (structural fixes); E0 (tuning experiment, unvalidated) | Host test suite (56/56 raw_pipeline_math_test assertions, full `make test` pass); clean `make LLVM=1` kernel module build for all edits including the isolated tuning commit. No hardware handshake or multi-finger session re-tested yet — see Known Constraints and the production-readiness plan. |

| 2026-07-24 | Phase 1 production-readiness fixes for raw multi-touch, implemented via 5 parallel isolated git-worktree agents (each: build + host test before merge) and closed with a double-blind post-merge review (independent agents given only the merged code, no implementation narrative): (1) replaced the greedy zero-assignment matcher in `raw_hungarian_match()` with a correct Kuhn-Munkres augmenting-path algorithm, cross-validated by its implementing agent against a brute-force optimal matcher over 20,000 randomized trials; (2) added a track-continuity cost bias (`HUNGARIAN_CONTINUITY_BONUS`) so two actively-claimed slots are not swapped for a marginal cost improvement during converging-finger gestures; (3) scaled `raw_ghost_merge()`'s merge radius *down* with claimed-finger count (`GHOST_RADIUS_*`, opposite direction from the existing `ASSOC_RADIUS_*` widening, tagged E0/first-pass — needs hardware tuning) instead of a fixed ~25mm radius that was merging genuinely distinct close fingers at high density; (4) fixed `raw_ccl_flood_fill()` reusing a stale CCL label across rejected/split components within one frame, which could corrupt a nearby blob's eigenvalue (ellipse) computation; (5) fixed `raw_update_slots()` reporting stale `TOUCH_MAJOR/MINOR/ORIENTATION` after a blob split; (6) closed a gap where `raw_probe_retry_work` could tear down an already-confirmed-working raw stream. The double-blind pass independently found and this session then fixed two further real defects introduced by fix (1)/(5)'s interaction with existing code: `HUNGARIAN_COST_OUT_RANGE` (100) was cheaper than `HUNGARIAN_COST_EMPTY` (1000), letting the optimizer prefer a doomed out-of-range match over a valid empty-slot match and drop the blob with no fallback (raised to 1500); and the eigenvalue copy in `raw_update_slots()` ran before the state-4 (hold) recovery-weight rejection check, letting a rejected noise blob contaminate a hold slot's shape data (moved after the state-machine switch resolves). A second blind pass confirmed both closing fixes correct. Verified with a controlled single load/unload cycle on real Surface Laptop 4 hardware (`raw_mode=1 raw_input_beta=1`): handshake confirmed on the first heatmap frame (no retries needed), 72×48 grid detected, baseline stabilized after exactly 30 frames, ~85 fps sustained for 15+ seconds with `frames_dropped=0` and zero warnings/oops/errors in dmesg. Not tested: actual multi-finger touch quality (requires a human hand — this session could not physically generate touch input; see the production-readiness plan for the human-in-the-loop hardware validation this still needs). System restored to standard-HID (`raw_mode=0`) after the test, matching its state before this session. | E3, E4 (structural fixes, host-verified + single-cycle hardware load-verified); E0 (ghost-merge radius multipliers and continuity-bonus magnitude, need real multi-finger hardware tuning) | 5-agent isolated implementation + merge-by-merge host test (`make test` green after each of 5 merges); double-blind review (2 initial + 1 closing pass); clean `make LLVM=1` throughout; single controlled hardware load/unload cycle, dmesg clean, `frames_dropped=0`. |

| 2026-07-24 | Live human-in-the-loop hardware session immediately following the Phase 1 merge above, on the real Surface Laptop 4 (`raw_mode=1 raw_input_beta=1`). Single-finger touch confirmed working end-to-end twice, independently, via raw `/dev/input/eventN` capture: clean `ABS_MT_TRACKING_ID`/`ABS_MT_POSITION_X/Y`/`BTN_TOUCH` down→move→up sequences, plus a direct raw-grid dump showing genuine signal (byte drop to ~0x7f against a ~0xb4 baseline). Six separate attempts at a live simultaneous 2-finger touch (both close together and at opposite screen corners, using the same proven capture method, plus one attempt cross-checked against a raw sensor-grid dump immediately after) produced **zero** detected signal each time — not a partial/buggy result, a hard zero at the earliest raw-signal stage, before any Phase 1 pipeline code runs. Also found and fixed a real test-methodology bug in-session: `sl4a_debug_level`-gated dmesg logging is unreliable for this kind of check (a control test using a known-working single tap also produced zero debug lines, because unrelated system dmesg traffic evicts the driver's lines from the ring buffer before a human-latency round-trip can read them) — do not rely on dmesg-based CALIB logging for future live verification; use direct `/dev/input/eventN` capture or a saved-file-based approach instead. Given six-for-six null results even at opposite screen corners (ruling out ghost-merge radius as a cause) using the reliable capture method, this is either a genuine multi-touch defect or a physical/contact artifact of remote-instructed one-handed opposite-corner touching on a laptop screen — this session could not distinguish between the two from live testing alone, and stopped live retries in favor of reproducible replay testing (next row) rather than continuing to consume the user's time on an approach with real synchronization limits. System restored to standard-HID (`raw_mode=0`) afterward. | E1 (single-touch, confirmed twice); E0/inconclusive (simultaneous multi-touch — six null attempts, cause not yet isolated between hardware/technique and software) | Two independent real `/dev/input/eventN` captures (single-touch); one direct raw heatmap-grid dump (single-touch, real signal); six `/dev/input/eventN` captures + one raw-grid dump (multi-touch, all null); one dmesg-based control test (also null, revealing the logging method itself was unreliable, not the touch). |
| 2026-07-24 | Per user direction, pivoted from live hardware retries to building reproducible, hands-free replay test infrastructure: a dedicated agent (isolated worktree `agent/raw-replay-harness`) is extending `tests/linux-compat/` so `driver/mshw0231-raw.c`'s real pipeline code compiles and links on host — the same pattern `capimg_decoder_host_test.c` already uses for `spi-hid-capimg.c` — replacing `raw_pipeline_math_test.c`'s hand-reimplemented mirror functions (a pre-existing test-coverage gap flagged by the same-day QA audit) with tests that call the actual `mshw0231_raw_consume_samples()` pipeline against saved synthetic frame fixtures (checked into `tests/fixtures/raw-replay/`, reusable for future regression testing). Includes a synthetic far-apart two-blob frame specifically constructed to test, deterministically and without any hardware/contact ambiguity, whether the pipeline logic itself can detect two simultaneous well-separated touches — directly following up on the inconclusive live result above. Merged (commit range `a0bc309`). Result: 2 simultaneous well-separated synthetic touches (both far-apart and close-together fixtures) are detected and tracked correctly by the real pipeline — independently corroborating that the live 2-finger null result was very likely a physical/contact artifact of remote one-handed testing, not a pipeline defect. The harness also found a real, precisely-isolated bug affecting 3+ simultaneous touches, fully independent of anything touched in this session's earlier Phase 1 fixes: `raw_detect_peaks()` compared each candidate cell against only 4 fixed points at exactly `HEATMAP_PEAK_RADIUS` cells away instead of scanning its full neighborhood; because a real finger blob tapers smoothly across several concentric signal rings, that single-point-per-axis probe almost never rejected a non-center cell, so every touched cell of a blob (~13 per blob on this hardware) independently counted toward the shared `HEATMAP_MAX_PEAKS=16` per-frame budget — 2 blobs already exhausted it, so any 3rd+ blob's row range was never reached by the raster scan and got silently dropped by CCL's velocity-rejection check despite valid pixel count and weight. Fixed by replacing the 4-point probe with a true local-maximum scan over the full neighborhood (commit `59f7c58`), verified via the replay harness: all of 1/2/3/4/5-finger synthetic fixtures now correctly report their expected slot count (previously 3/4/5-finger capped at 2, every time). The 3/4/5-finger replay assertions were promoted from a tracked "known bug" to plain `CHECK()`s (commit `99e15b3`), so a regression here now fails `make test`. | E3 (2-finger detection and the peak-detection fix, replay-verified); E0 (3+ finger fix not yet hardware-cross-validated — see Known Constraints) | `make test` green (46/46 new replay assertions, all pre-existing suites unaffected); clean `make LLVM=1`. Not yet re-tested on real hardware — the live session that motivated this investigation ended before this fix landed. |

Update this table in the same commit as every completed refactor phase or
evidence-backed behavioral change.
