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

`captures/id5-20260718/raw_capture_status` records valid raw captures, but has
no associated firmware, kernel, distribution, or gesture-output result. It is
kept as an investigation artifact and is not a compatibility result.

## Known Constraints

- The controlled-release profile is standard HID mode.
- Raw mode is explicit opt-in through `install.sh --raw` and remains
  experimental until cold boot, resume, reload, pen, one through five fingers,
  and stress testing are recorded as E1 evidence.
- Secure Boot is unqualified until a signed-module install result is recorded.
