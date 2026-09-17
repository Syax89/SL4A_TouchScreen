# Tool collection plan - one command, one file, everything

Status: phase 1 landed with the hunt wire-form sweep (2026-09-17); phases 2-4
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

- hunt sweeps `wire_double_opcode` / `setfeat_no_double`:
  0 = single opcode (the failing shape), 1 = doubled (every control frame,
  v1.6.3's shape), 2 = doubled except SET_FEATURE5. Reads keep the module
  default (legacy, as in v1.6.3).
- every variant names its wire profile in the artifact and on the terminal;
- the artifact carries the first control write's bytes (doubled vs single is
  the second byte) and an "OS binding" block: ACPI device, bound driver,
  registered input device - the "does the hardware present itself to the OS"
  question without extra commands.
- pins: the sandbox test fails if any of the three wire shapes is not loaded,
  or if the artifact does not name the one it ran.

## Phase 2 - fold the static collector into the sweep

`hunt` should embed the `logs` header (versions, srcversion, DKMS, full param
list) so ONE file answers version questions too; one code path builds both.
Acceptance: a hunt file contains everything the current logs bundle does, and
a test asserts the header keys.

## Phase 3 - variant axes become first-class

`hunt --variants wire|read|all` (default wire). The read/transport axis
(request-resend continuations, the `0x84` window - the V-A/V-B/V-C shapes a
P13 leg proposed) returns as `read`, because the next open question after the
wire test is the segmented read chain. Acceptance: axis selector documented,
sandbox test per axis.

## Phase 4 - the sweep recommends the shape

If a variant reaches `device_desc>0` / `data>0`, hunt prints the exact
/etc/modprobe.d line to adopt. It never writes it. Acceptance: verdict text
carries the recommendation; the tool still never edits the user's profile.

## Not doing

- no auto-install of the winning profile (the user's profile is the user's);
- no network, no telemetry; output stays a single txt beside the driver;
- no new dependencies: bash + coreutils only, as today.
