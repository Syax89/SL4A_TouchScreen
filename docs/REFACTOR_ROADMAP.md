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

## Refactor Phases

### Phase 0: Baseline And Evidence

- [x] Preserve the current hardening batch on this branch.
- [x] Reconcile release-facing documents with code, ACPI, and capture data; retain unresolved claims in the evidence ledger.
- [x] Create an evidence index linking protocol claims to tracked sources.
- [ ] Record the tested firmware, kernel, and distribution versions.

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
- [ ] Document module signing and Secure Boot for each supported distribution.
- [ ] Make uninstall remove only artifacts owned by this package.

Exit criteria: install, upgrade, rollback, and uninstall are deterministic and
do not modify unrelated modules or configuration.

### Phase 3: Internal Driver Boundaries

- [ ] Document the AMDI0060 controller contract separately from MSHW0231.
- [ ] Isolate MSHW0231 V0 sequencing and activation from raw frame processing.
- [ ] Classify each module parameter as release, diagnostic, or experimental.
- [ ] Remove or gate experimental behavior from the release profile.

Exit criteria: controller, transport, and touch-pipeline changes can be tested
and reviewed independently without changing the wire contract accidentally.

### Phase 4: Replay And Hardware Validation

- [ ] Version replay fixtures for reset, descriptor, feature activation, raw
      frame, frame gap, and malformed input.
- [ ] Exercise protocol/decoder tests under sanitizers.
- [ ] Capture a Linux trace format comparable to the versioned Windows traces.
- [ ] Execute the hardware matrix: cold boot, warm boot, suspend/resume,
      repeated reload, pen, one through five fingers, and a 30-minute stress run.

Exit criteria: every release claim is backed by a named fixture or a dated
hardware result in `docs/COMPATIBILITY.md`.

### Phase 5: CI And Releases

- [ ] Add GitHub Actions for formatting, host tests, and DKMS builds against
      the supported kernel header matrix.
- [ ] Publish signed, versioned GitHub releases with source checksums and a
      compatibility table.
- [ ] Keep a rollback procedure for every release.

Exit criteria: a tagged release can be rebuilt, installed, verified, and
removed using only repository documentation.

## Branch Log

| Date | Change | Evidence | Verification |
| --- | --- | --- | --- |
| 2026-07-22 | Created `refactor/sl4a-distribution`; preserved current hardening work and established this roadmap. | E3 | Host protocol and CapImg tests previously passed, including ASan/UBSan. |
| 2026-07-22 | Sprint 0: added support contract, evidence ledger, compatibility matrix, and validation procedure; added fail-closed ACPI/DMI preflight, ownership-safe install/uninstall behavior, and standard installer profile. | E2, E3 | Double-blind review; protocol and CapImg host tests passed with ASan/UBSan. |

Update this table in the same commit as every completed refactor phase or
evidence-backed behavioral change.
