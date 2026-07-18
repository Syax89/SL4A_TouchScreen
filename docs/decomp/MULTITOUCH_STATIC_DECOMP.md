# MSHW0231 Multitouch Static Decompilation

## Scope and confidence

This report reconstructs the Windows static path that consumes MSHW0231 heat
frames. It is based on the descriptor and already-decompiled `HeatCore.dll`
and `TouchPenProcessor0C19.dll` artifacts. It does not activate a device,
send a Feature report, or claim that temporal correlation proves causality.

Confidence labels:

- **Proven**: direct descriptor, trace, or decompiled-code evidence.
- **Derived**: a direct composition of proven facts.
- **Unknown**: needs more decompilation or a labelled runtime trace.

## Architecture

**Proven:** the MSHW0231 collection is bound to the generic `hidspi` transport
and the Surface software processor `TouchPenProcessor0C19.dll` through
`HeatCore.dll`. See
`/home/simone/spi-hid-standard-baseline/docs/decomp/inf_binding_20260716/REPORT.md`.

The input path is:

```text
MSHW0231 SPI DATA report
  -> hidspi.sys
  -> HeatCore CapImg HID protocol
  -> TouchPenProcessor0C19.dll
  -> HeatCore contact-frame finalization
  -> Heat HID / pointer injection
  -> Windows touch stack
```

The normal HID touchscreen collection is separate. The descriptor exposes
only `BTN_TOUCH`, `ABS_X`, and `ABS_Y` there, so it cannot represent Linux
multitouch on its own.

## Descriptor Contract

**Proven:** the Digitizer/CapImg application collection contains frame-data
input reports ID `0x07`, `0x08`, `0x0a`, `0x0b`, `0x0c`, `0x0d`, `0x1c`, and
`0x1a`. Each begins with a 16-bit `SurfaceSwitch` and follows with an opaque
array. ID `0x0c` has 4299 bytes of report data, matching the 4304-byte aligned
SPI body after its five-byte transport prefix. See
`/home/simone/wintrace/mshw0231_report_descriptor.txt:72-165`.

**Proven:** the same collection declares:

| Report | Descriptor field | Meaning in HeatCore |
| --- | --- | --- |
| `0x06` | `DeviceMode`, 119 bytes | Feature read by the boot sequence. |
| `0x05` | page `0xff00`, Usage `0xc8`, 1 byte | Heat reporting-mode Feature. |
| `0x09` | Usage `0xc9`, 63 bytes | Additional Feature; purpose unresolved. |
| `0x0c` | Usage `0x61` array within its report | CapImg frame-data carrier. |

ID `0x56` is a six-byte Feature in a different vendor collection. It is not
the Usage `0x56` input value that HeatCore optionally reads as a frame sequence
number. These names share a numeric value but are different descriptor fields.

## Heat Reporting Mode

**Proven:** `HeatCore!HIDDeviceInterface::SetHeatReportingMode` is at
`0x180016e10`. It allocates the descriptor-defined Feature report length,
calls `CapImg::Protocol::HID::BuildFeatureReportForModeSwitch`, then invokes
the HID interface SetFeature vtable method. See
`heatcore_20260716/functions/180016e10_FUN_180016e10.c:29-60`.

**Proven:** `BuildFeatureReportForModeSwitch` is `0x180016fb4`. It writes the
cached Feature report ID to byte zero, then calls:

```text
HidP_SetUsageValue(Feature, page, link=0, usage=0xc8,
                   value=(mode == Heatmap), preparsed_data, report)
```

The page is `0xff00` for this MSHW0231 descriptor. The report ID is discovered
by `BuildDevice` from the unique Usage `0xc8` Feature cap. See
`heatcore_20260716/functions/180016fb4_FUN_180016fb4.c:9-19` and
`heatcore_20260716/functions/18001bac0_FUN_18001bac0.c:201-236`.

**Derived:** on MSHW0231, requesting `Heatmap` constructs report ID `0x05`
with Usage `0xc8` set to `1`. This exactly explains the semantic portion of
the observed Windows SET FIFO payload:

```text
02 00 00 03 82 00 03 04 00 05 01 00 00 00
                              ^^ ^^ 
                              ID value
```

This establishes ID5 as the HeatCore reporting-mode operation, rather than a
generic unknown vendor write. It does not prove that ID5 alone makes ID0c
appear, nor does it attribute the later ID56 operation.

`HeatCore!0x180017038` logs mode `0` as `Disabled` and mode `1` as `Heatmap`.
See `heatcore_20260716/functions/180017038_FUN_180017038.c:11-24`.

**Proven:** the method is vtable slot `+0x18` of `HIDDeviceInterface`; the
exported global vtable index is misleading because adjacent vtables were merged
by the exporter. `HeatDevice` invokes this slot with mode `1` when the processor
loads, after hardware reset, and on its power notification path. It invokes
mode `0` while deinitializing hardware. Relevant functions are:

| Lifecycle action | Function |
| --- | --- |
| processor loaded, mode `1` | `heatcore_20260716/functions/180034900_FUN_180034900.c` |
| reset hardware, mode `1` | `heatcore_20260716/functions/180004790_FUN_180004790.c` |
| power notification, mode `1` | `heatcore_20260716/functions/18001ade0_FUN_18001ade0.c` |
| deinitialize, mode `0` | `heatcore_20260716/functions/180033f30_FUN_180033f30.c` |

This makes the ID5 write part of the HeatCore lifecycle, not an arbitrary
client operation. It still does not prove that ID5 alone is sufficient for a
raw stream on Linux.

## Feature ID56 Boundary

**Proven:** Feature report ID `0x56` belongs to another descriptor collection
and carries six data bytes in the observed Windows SET. It is distinct from
the Digitizer Usage `0x56` that HeatCore uses as an optional input-frame
sequence number.

**Proven:** the recovered HeatCore device lifecycle selects Usage `0xc8` and
therefore report ID5 for its reporting-mode write. It contains no recovered
ID56 report selection, payload, or caller.

**Proven:** the available `SurfaceService.exe` has no `HidD_SetFeature` import
and no static copy of `bd 0c ee 5b 44 4c`. Its recovered
`CTHCTelemetry::PostTelemetry` path calls `HidD_GetFeature` only, with a
dynamically selected telemetry report ID. Its WinRT `HidDevice` async handlers
remain stripped and cannot be connected statically to MSHW0231 or ID56.

**Unknown:** the ID56 producer lies outside the recovered HeatCore lifecycle
and cannot be identified from the current binary corpus. Candidates must not
be promoted to evidence without a caller, a buffer construction path, or a
trace that maps its submitter PID to an image and stack.

## CapImg Transport State Machine

**Proven:** `CapImg::Protocol::HID::BuildDevice` (`0x18001bac0`) admits only
Digitizer page `0x0d`, usage `0x0f` devices. It discovers at most 16 frame-data
reports, requires each to have 8-bit items and at least seven bytes, then
records report ID and payload length. It separately records which reports carry
Usage `0x62` chunk count and Usage `0x56` sequence. See
`heatcore_20260716/functions/18001bac0_FUN_18001bac0.c:54-153`.

**Proven:** `UpdateFrameDataTransferForReport` (`0x180008930`) performs these
operations for every admitted report:

1. Finds the report descriptor entry by report ID.
2. Reads Usage `0x62` when that report declares a chunk-count field.
3. Rejects a changed report ID while an incomplete transfer exists.
4. Rejects inconsistent chunk count or a destination-buffer overflow.
5. Reads Usage `0x56` as an optional sequence value at transfer completion.
6. Copies the Usage `0x61` value array into the aggregate transfer buffer.
7. Marks a frame complete only after the expected byte count and chunk count
   are satisfied.

See `heatcore_20260716/functions/180008930_FUN_180008930.c:55-171`.

**Derived:** MSHW0231 ID0c is eligible for HeatCore because its report size is
known to the descriptor-derived table and its array is read through Usage
`0x61`. It is not interpreted by the generic HID input mapper as multitouch.

## CapImg Frame Content

**Proven:** the local Linux decoder and the retained corpus agree on a complete
ID0c body:

```text
offset 0..1       semantic length 4302, little-endian
offset 2          content ID 0x0c
offset 3..4301    4299-byte CapImg content
offset 4302..4303 transport-aligned tail
```

The CapImg content has one 3456-byte run, 72 columns by 48 rows, and a vendor
section `0xff00`. The Surface plugin explicitly requests sections `0x100` and
`0xff00` before processing. See
`touchpen0c19_fullheat_20260716/functions/180694390_FUN_180694390.c:126-159`.

**Unknown:** the semantic meaning of the `0xff00` section and the complete
device configuration are not recovered. They can affect calibration, baseline,
thresholds, and contact classification.

## Surface Contact Processing

**Proven:** the processor uses a 0x120-byte row stride for its staged grid.
`FUN_1805fba00` searches candidate extrema against pixels five cells away,
uses a configuration threshold at `config + 0x1fe`, and computes a weighted
centroid over a 5 by 5 neighbourhood above a baseline. It also applies
temporal counters and configuration gates. See
`touchpen0c19_fullheat_20260716/functions/1805fba00_FUN_1805fba00.c:75-200`.

**Proven:** the recovered detector configuration has this partial layout:

| Offset | Width | Use |
| --- | --- | --- |
| `0x1f8` | `u16` | maximum allowed candidate magnitude |
| `0x1fa` | `u8` | candidate-count gate |
| `0x1fb` | `u8` | frame-age gate |
| `0x1fd` | `u8` | enables extended local search when `1` |
| `0x1fe` | `u8` | neighbour/sample threshold |

`FUN_18068f380` propagates these values from the per-device state record; the
known code does not show it reading them directly from the `0xff00` section.
The upstream parser that populates that record remains unknown. The detector
publishes centroid X/Y and an active byte at frame-state offsets `0xb75c`,
`0xb760`, and `0xb764` respectively. See
`touchpen0c19_fullheat_20260716/functions/18068f380_FUN_18068f380.c` and
`touchpen0c19_fullheat_20260716/functions/1805fba00_FUN_1805fba00.c:154-200`.

**Proven:** `FUN_180602a90` writes one plugin contact record per accepted
contact. The record is 0x34 bytes, sourced from a per-contact state object of
size 0x254. It contains float coordinates, bounds/history fields, flags,
confidence-like values, and temporal state. See
`touchpen0c19_fullheat_20260716/functions/180602a90_FUN_180602a90.c:36-149`.

**Unknown:** field names and the complete tracker state machine remain
unresolved. The recovered code is enough to reject the current Linux
assumption that connected components plus a global affine transform are a
faithful Surface implementation.

## Contact Egress

**Proven:** HeatCore finalizes a plugin frame through a processor-host vtable:
`HeatDevice::FinalizeContactFrame` at `0x1800047f0`. See
`heatcore_20260716/functions/1800047f0_FUN_1800047f0.c:15-35`.

**Proven:** `HeatHidInjectionProvider::InjectHeatContactFrame`
(`0x180015ff0`) converts contact records to injected HID records. Its first
format accepts 0x3a-byte source records and emits 0x21-byte records in groups
of at most five contacts; other source formats are handled separately. See
`heatcore_20260716/functions/180015ff0_FUN_180015ff0.c:28-97`.

**Proven:** `HeatPointerInputReporter::ReportTouchContactFrame`
(`0x1800351a0`) builds 0x3a-byte pointer contact records. It copies the input
contact ID, flag bits, two coordinate/history pairs, shape-like fields, and
other state before invoking the pointer reporter. See
`heatcore_20260716/functions/1800351a0_FUN_1800351a0.c:34-100`.

## Linux Consequences

1. The standard HID node is correctly single-touch. It must remain the stable
   path until the heat pipeline is independently validated.
2. The passive Linux beta is an acquisition and prototype path, not a port of
   the Surface algorithm. Its baseline, thresholding, merge/split, tracking,
   and global affine calibration need an offline oracle.
3. ID5 has a static semantic explanation: `SetHeatReportingMode(Heatmap)`.
   ID56 is not referenced by the recovered HeatCore lifecycle and still has no
   static caller, semantic decoder, or demonstrated need.
4. Any future hardware experiment must remain separately authorized and must
   not add ID56 merely because it precedes ID0c in one trace.

## Next Decompilation Targets

1. Trace callers of `SetHeatReportingMode` to establish when HeatCore requests
   `Heatmap` and whether it also touches DeviceMode ID6.
2. Recover the parser for plugin section `0xff00` and the configuration table
   behind `FUN_1805fba00` offsets `0x1f8..0x1fe`.
3. Recover the tracker and contact-record field names around `FUN_180602a90`.
4. Type the HeatCore contact-frame structures at `0x1800047f0`, `0x180015ff0`,
   and `0x1800351a0`, then compare their coordinates and flags with labelled
   CapImg corpus sessions.
5. Locate the independent producer of Feature ID56. It must be treated as a
   separate protocol question from HeatCore's ID5 reporting-mode operation.
