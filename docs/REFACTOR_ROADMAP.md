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

Update this table in the same commit as every completed refactor phase or
evidence-backed behavioral change.
