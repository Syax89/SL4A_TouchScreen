# Compatibility Matrix

Only rows with complete provenance are release-qualified. An empty matrix means
the code has not yet earned a compatibility claim for that environment.

## Release-Qualified Results

| Date | Device | BIOS/firmware | Kernel | Distribution | Profile | Secure Boot | Result | Evidence |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| No qualified result recorded | - | - | - | - | - | - | - | - |

## Required Fields

Each new row must include:

- Surface model and ACPI identifiers found at runtime.
- BIOS/UEFI and touchscreen firmware versions when available.
- Exact kernel release and distribution version.
- Standard or raw profile, including non-default module parameters.
- Secure Boot state and module-signing result.
- Test procedure from `TESTING.md`, date, outcome, and capture/log location.

## Non-Qualified Observations

These observations are not release-qualified. They record single-session standard
HID results with temporary local artifacts; permanent E1 qualification requires
archived evidence with verified checksums and a completed matrix.

| Date | Session | Cases | Artifact | Outcome |
| --- | --- | --- | --- | --- |
| 2026-07-23 | Cold boot | No auto-binding; post-login activation ok; descriptor 936 B; ready; 0 drops | evidence/cold-boot-state | pass |
| 2026-07-23 | Post-activation | Controller+transport bound; ready; data=267; 0 drops | evidence/post-activation-state | pass |
| 2026-07-23 | Suspend/resume | Touch returned ready; descriptor re-read (reset 2→4, re-read desc); 0 drops. Platform warnings from surface_aggregator/charger, not SL4A stack | evidence/pre-suspend, evidence/post-resume | functional, platform deviation |
| 2026-07-23 | Stylus | HID 045E:0C19 Stylus discovered (pen, pressure, tilt); no pen hardware available for input test | device discovery only | not applicable |

`captures/id5-20260718/raw_capture_status` records valid raw captures, but has
no associated firmware, kernel, distribution, or gesture-output result. It is
kept as an investigation artifact and is not a compatibility result.

## Known Constraints

- The controlled-release profile is standard HID mode.
- Raw mode is explicit opt-in through `install.sh --raw` and remains
  experimental until cold boot, resume, reload, pen, one through five fingers,
  and stress testing are recorded as E1 evidence.
- Secure Boot is unqualified until a signed-module install result is recorded.
