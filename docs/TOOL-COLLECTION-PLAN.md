# Tool collection plan - one command, one file, everything

Status: phase 1 landed (hunt sweep; probe axes since 2026-09-17 13:15 — the
wire axis closed negative); phases 2-4
open. Written after the 2026-09-17 field regression (raw touch answered on
v1.6.3, reset-looped after the campaign's wire changes): the field bundle is
what settles questions, so the tool that produces it must be able to name,
per run, which shape was on the wire and what the OS thinks it sees.

## What the two collectors do today

- `sl4a-touch.sh logs` - static bundle: versions, DKMS/modprobe state, module
  params, sysfs stats, last frame, filtered dmesg. No variants.
- `sl4a-touch.sh hunt` - variant sweep, counters, per-variant dmesg slice,
  verdict lines; rebuilds a stale module first.

## Phase 1 - DONE (2026-09-17, with the regression hunt)

- **axis history**: wire forms (0 single / 1 doubled / 2 doubled-except-
  SET_FEATURE5) were swept on 2026-09-17 13:11 and came back negative — all
  three reset-loop; close an axis in ONE run and move on. Current axes
  (since `4525811`): 0 control / 1 `acpi_probe_power_cycle=1` / 2
  `skip_vendor_stop=1` / 3 both. Reads keep the module default (legacy).
- every variant names its probe profile in the artifact and on the terminal;
- the artifact carries the first control write's bytes (doubled vs single is
  the second byte) and an "OS binding" block: ACPI device, bound driver,
  registered input device - the "does the hardware present itself to the OS"
  question without extra commands.
- pins: the sandbox test fails if any probe arm is not loaded in order, if the
  artifact does not name the one it ran, if the live module-param readback is
  missing, or if the first control write is lost from the artifact (the
  60-line window bug the P3 wave found).

## Phase 2 - fold the static collector into the sweep

`hunt` should embed the `logs` header (versions, srcversion, DKMS, full param
list) so ONE file answers version questions too; one code path builds both.
Acceptance: a hunt file contains everything the current logs bundle does, and
a test asserts the header keys.

## Phase 3 - variant axes become first-class

`hunt --variants probe|read|all` (default probe; the wire axis closed negative
on 2026-09-17 13:11 and stays as history only). The read/transport axis
(request-resend continuations, the `0x84` window - the V-A/V-B/V-C shapes a
P13 leg proposed) returns as `read`, because the next open question after the
probe test is the segmented read chain. Acceptance: axis selector documented,
sandbox test per axis.

## Phase 4 - the sweep recommends the shape

If a variant reaches `device_desc>0` / `data>0`, hunt prints the exact
/etc/modprobe.d line to adopt. It never writes it. Acceptance: verdict text
carries the recommendation; the tool still never edits the user's profile.

## Not doing

- no auto-install of the winning profile (the user's profile is the user's);
- no network, no telemetry; output stays a single txt beside the driver;
- no new dependencies: bash + coreutils only, as today.
