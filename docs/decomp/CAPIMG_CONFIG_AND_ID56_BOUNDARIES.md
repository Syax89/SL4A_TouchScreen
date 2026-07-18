# CapImg Configuration and ID56 Boundaries

## CapImg Sections

The recovered CapImg section header is seven bytes:

```text
u32 total_section_length
u16 section_type
u8  flags_or_format
payload starts at +7
```

The known `0x0100` section is fully constrained for MSHW0231:

```text
+0x00  u32 length = 16 + 3456
+0x04  u16 type = 0x0100
+0x06  u8  flags = 1
+0x07  u8  sample width = 8
+0x08  u32 destination = 0
+0x0c  u32 run length = 3456
+0x10  3456 samples, 72 columns by 48 rows
```

`FUN_18068e990` validates and stages this section into the Surface processor's
0x120-byte-stride grid. See
`touchpen0c19_fullheat_20260716/functions/18068e990_FUN_18068e990.c`.

## Vendor Section `0xff00`

`TouchPenProcessor0C19.dll` explicitly requests section `0xff00`, receives its
pointer as the fourth argument of `FUN_18068d620`, and derives payload pointer
`section + 7` and length `section_length - 7`.

No recovered code in this path dereferences, parses, validates, or branches on
the payload bytes. The pointer and length are forwarded as opaque data only
through a configuration-gated call. It is therefore incorrect to attribute the
detector thresholds or tracker configuration to `0xff00`.

The detector configuration is instead copied from the per-device state record
into a detector-facing object by `FUN_18068f380`; the upstream producer of that
record remains unresolved. The known detector fields are:

| Offset | Use |
| ---: | --- |
| `0x1f8` | maximum candidate magnitude |
| `0x1fa` | candidate-count gate |
| `0x1fb` | frame-age gate |
| `0x1fd` | extended-search enable |
| `0x1fe` | five-cell neighbour threshold |

The later tracker has additional dynamic per-device values. `config + 0x8dc`
and `config + 0x8e0` are association radii in grid coordinates; each is squared
before matching. The pointer at `tracker + 0xc570` selects a per-frame antenna
table whose first float is the squared-distance threshold for post-report
coalescing. These are offsets and pointers, not constants. Their numeric values
are not recoverable from the static DLL corpus.

The only statically recovered numeric suppression value is float `0.04`, used
by a separate weak-pixel class-5 path. It must not be reused as a contact merge
distance.

For the MSHW0231 selector `0x0c19`, the DLL static record supplies association
radii `0.5450090` and `1.2180980` at offsets `0x8dc` and `0x8e0`. Its default
frame-data subtable supplies coalescing threshold `36.0` at `+0x0c`, interpreted
as squared grid distance. These values are documented with the tracker in
`SURFACE_TRACKER_DECOMP.md`.

## Feature ID56

Report ID `0x56` is a real six-byte HID Feature report in a separate descriptor
collection. The Windows trace sends:

```text
03 0a 00 56 bd 0c ee 5b 44 4c
```

This must not be confused with Digitizer Usage `0x56`, which HeatCore reads as
an optional sequence value in input-frame transfer logic.

The following are proven negative boundaries:

- HeatCore's reporting-mode lifecycle selects Usage `0xc8`, which is report
  ID5, and has no recovered ID56 selection or buffer.
- `HidSpiCx` is generic SET_FEATURE transport and has no ID56 branch.
- the available `SurfaceService.exe` has no `HidD_SetFeature` import and no
  static copy of `bd 0c ee 5b 44 4c`; its named telemetry path is read-only.

The available corpus therefore cannot identify the ID56 producer. Linux does
not require ID56 to activate ID0c: the ID5-only observation is documented in
`ID5_ACTIVATION_EVIDENCE.md`.

## Consequence

Do not add an ID56 write to the Linux driver. The current useful work is the
offline tracker oracle, configuration-source recovery, and stable Type-B slot
assignment.
