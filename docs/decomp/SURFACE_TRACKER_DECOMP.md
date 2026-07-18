# Surface CapImg Tracker Decompilation

## Scope

This report covers the recovered Surface contact tracker after heatmap parsing
and local centroid detection. It explains why the current Linux beta can expose
an extra multitouch slot during a two-finger gesture: its component-to-slot
logic is not equivalent to the Windows tracker.

All names are evidence-based where a symbol exists. State names are descriptive
only where the binary exposes values but not semantic labels.

## Processing Order

`FUN_1805fba00` detects a local extremum and writes its weighted 5 by 5 centroid
to the frame state. It is not the tracker.

The following `FUN_1805ffd00` pipeline owns tracking:

```text
frame setup                 FUN_1806003b0
segmentation preparation    FUN_180600820
candidate/blob extraction   FUN_180600c40
candidate classification    FUN_180601690
candidate/track association FUN_180606040 -> FUN_180607a20
track update/allocation     FUN_180608000 / FUN_180607c60
state selection             FUN_1806067d0
contact emission            FUN_180601c10 -> FUN_180601dd0 -> FUN_180602a90
report coalescing           FUN_1806025c0
```

See `touchpen0c19_fullheat_20260716/functions/1805ffd00_FUN_1805ffd00.c:64-205`.

The frame counter is compared against the prior frame; a gap greater than five
forces release of all slots with `FUN_180600ad0`. This is a concrete recovery
rule that the Linux beta does not presently mirror.

## Persistent Records

| Record | Base and stride | Recovered fields |
| --- | --- | --- |
| candidate/blob | `tracker + 0xca58`, `0xb0` | centroid near `+0x24/+0x28`; bounds near `+0x30..+0x36`; class/validity `+0x40/+0x41`; derived features from `+0xa0`. |
| track | `tracker + 0xf3d8`, `0x254`, 48 maximum | state `+0x3c`; candidate index `+0x30`; history count `+0x34`; history ring index `+0x251`; ten history entries at `+0x58`, each `0x30` bytes; per-sample class at history `+0x70`. |
| emitted plugin contact | `frame + 0xaf84`, `0x34` | class/status `+0x2a`; mutable merge-group label `+0x2b`; source track index `+0x2f`. |

The tracker maintains allocated count at `+0x16399` and active count at
`+0x16398`. `FUN_180608430` is the counter-safe state setter.

## Track State Machine

`FUN_180608430` proves the primary state field is `track + 0x3c`:

| State | Accounting evidence | Practical interpretation |
| --- | --- | --- |
| `0` | frees allocated count; also removes active count if old state was 1 or 2 | free track |
| `1` | allocated and active | normal active track |
| `2` | allocated and active | alternate active track |
| `3` | inactive count; cleared to 0 before final report pass | transitional cleanup |
| `4` | inactive count; may retain history-derived output | hold/lift-pending |

State `4` is not an immediate release. `FUN_180606370` evaluates temporal
history, neighbourhood state, size, confidence and configured thresholds before
placing a state-1 track in hold. It then clears the current sample class while
the retained history can still participate in output. See
`180606370_FUN_180606370.c:37-220` and
`180608430_FUN_180608430.c:60-198`.

## Candidate Association

`FUN_180607a20` associates candidates with tracks. It computes squared centroid
distance in grid coordinates and uses the per-device float at `config + 0x8dc`;
the special single-track continuity case uses the per-device float at
`config + 0x8e0` instead. The selected radius is squared before comparison. A
missing assignment or a distance rejection calls `FUN_180607c60` to allocate a
new track. A valid assignment calls `FUN_180608000` to update the existing
track.

See `180607a20_FUN_180607a20.c:27-65`.

This directly explains one source of the Linux beta's observed third slot: a
single physical contact split into two candidates can become a new track before
the later Windows suppression and coalescing stages run.

## Global Assignment

`FUN_180606040` compacts valid observations and active tracks, then builds a
square cost matrix. `FUN_1805fd090` stores Euclidean grid distance for an
in-radius observation/track pair, cost `100` for an out-of-radius real pair,
and cost `1000` for padding. `FUN_1805fd230` is a Hungarian/primal-dual
minimum-cost assignment solver.

`FUN_180607a20` consumes the selected pair, rechecks the strict float radius,
and either updates the existing track or creates one with `FUN_180607c60`.
The recovered stack-local matrix orientation is ambiguous in the decompiler;
the portable Linux representation must be explicit:

```text
observation index -> physical track index
```

The safe portable algorithm differs deliberately from the vendor's padded
matrix: build costs only for real in-radius edges, solve one-to-one assignment,
update matched tracks, create at most one track for each unmatched observation,
and let unmatched tracks enter their normal lift policy. Never spawn from a
padded matrix entry or an out-of-radius selected cost.

This is the required replacement for the beta's greedy slot walk. It removes
the ambiguity that can allocate a new Linux slot while a globally better match
already exists.

## Report Coalescing

`FUN_1806025c0` runs after contact records exist. It considers record pairs
when both classes are eligible (`1` or `3`), their merge-group labels differ,
and squared centroid distance is lower than the dynamic float addressed by the
pointer at `tracker + 0xc570`. Per-frame setup selects that pointer from the
per-device frame-data table. Neither this ghost radius nor the association
radii are recoverable as static numeric constants.

For each pair it:

1. saves record indices in `tracker + 0xeb58`;
2. marks both source tracks at track offset `+0x40`;
3. rewrites every record using the second group label to the first label;
4. changes eligible class `1` to class `7` for both pair members.

See `1806025c0_FUN_1806025c0.c:20-81`.

The Windows function keeps both records in its local list. The group label is
therefore not a safe external tracking ID, and the Linux driver must not expose
both records as independent fingers merely because both were emitted.

## MSHW0231 Static Thresholds

The 0C19 processor selector maps to static record `DAT_1808e0460` in
`TouchPenProcessor0C19.dll`. Its recovered defaults are:

| Role | Record field | Value | Units |
| --- | ---: | ---: | --- |
| normal association radius | `config + 0x8dc` | `0.5450090` | grid coordinate distance |
| single-track continuity radius | `config + 0x8e0` | `1.2180980` | grid coordinate distance |
| report coalescing threshold | `frame_data + 0x0c` | `36.0` | squared grid distance |

`FUN_180607a20` squares the first two radii before comparing centroid distance.
`FUN_1806025c0` directly compares squared centroid distance with `36.0`; this
is a coalescing radius of six grid cells. The default per-frame selector points
`tracker + 0xc570` at `frame_data + 0x0c` for 0C19.

The Linux beta's current `ghost_dist=15` test is an axis-aligned box and does
not implement this contract. Replacing it alone would be unsafe: the preceding
Surface association, suppression and hold/lift phases decide which records are
eligible for coalescing.

## Linux Requirements

Before beta multitouch can be considered usable, Linux needs:

1. persistent tracks rather than component-local slots;
2. a bounded association distance and continuity exception;
3. a ten-sample history or an equivalently tested hold/lift policy;
4. pre-report split suppression and post-report merge grouping;
5. a stable Linux tracking-ID allocator independent of mutable merge labels;
6. release-all behavior after an acquisition discontinuity.

Do not substitute a guessed value for the missing per-device radii. The next
offline oracle must accept captured radii/configuration as inputs and test
association/coalescing parametrically. The only static signal threshold
recovered so far is `0.04` for a separate weak-pixel class-5 path; it is not a
merge radius.

## Unknowns

- The exact assignment cost matrix and tie breaking before `FUN_180607a20`.
- The names of per-sample classes. Classes `1`, `3`, `4`, `5`, and `7` have
  demonstrated control-flow roles but not recovered semantic names.
- The physical calibration and threshold values behind the tracker config.
- The mapping from internal source track to the final Windows virtual-HID
  tracking ID.
