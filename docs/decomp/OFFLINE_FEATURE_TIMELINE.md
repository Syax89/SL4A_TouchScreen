# Offline Feature Timeline

This dossier is derived only from the hash-pinned Windows
`traces/surface_boot_auto.csv`. Run
`python3 tools/analyze_post_set_timeline.py` to reproduce it.

## Observed Sequence

| Relative time | V0 transaction | Direct interpretation |
| --- | --- | --- |
| 0 ms | SET_FEATURE ID `0x05`, data `01` | One-byte vendor Feature. |
| 2.19-93.24 ms | Ten DATA ID `0x08` bodies | 216-byte aligned, 214-byte semantic bodies. |
| 7764.53 ms | SET_FEATURE ID `0x56`, data `bd 0c ee 5b 44 4c` | Six-byte Feature report. |
| 36288.63-36301.52 ms | Two DATA ID `0x08` bodies | Same body envelope. |
| 36311.95 ms | DATA ID `0x0c` | First observed 4304-byte raw/Heat body. |

## Submitter Attribution

The SPB `Start` event for the canonical SET ID5 is submitted by PID 4400, TID
4488, Activity `{c6225441-0bb8-0005-e654-22c6b80bdd01}`. The ID56 SET is a
different request: PID 2364, TID 2536, Activity
`{c6225441-0bb8-0000-fe5b-22c6b80bdd01}`. It is therefore not a continuation
of the ID5 request context.

`tools/parse_spb_csv.py` now preserves `submitter_pid` and `submitter_tid` from
the SPB `Start` event; `tools/analyze_post_set_timeline.py` reports both. The
hash-pinned regression asserts PID 4400 for ID5 and PID 2364/TID 2536 for ID56.
The subsequent DATA frames are processed on kernel interrupt threads (PID 4),
which is expected and does not identify their initiating user-mode client.

## Descriptor Evidence

The report descriptor defines Report ID `0x56` as a six-byte Feature payload
followed by a one-bit Feature and seven constant bits:
`/home/simone/wintrace/mshw0231_report_descriptor.txt:409-427`.

The V0 output body is `content_type`, semantic length, report ID, and data.
For the ID56 write, `03 0a 00 56 bd 0c ee 5b 44 4c` is therefore SET_FEATURE,
length 10, ID56, six data bytes. It is not a raw frame and its semantic effect
is not yet established.

## Constraints

- ID56 is a concrete configuration candidate, but timing alone does not prove
  it enables ID0c.
- The first ID0c arrives roughly 28.55 seconds after ID56, so a direct causal
  claim needs the user-mode caller chain or additional passive evidence.

## HidSpiCx Boundary

Disassembly of the PDB-matched `/home/simone/windrivers/HidSpiCx.sys` confirms
that `HidSpiRequestManager::HidSetFeature` is generic transport glue. Its
actual prologue starts at `0x14000EC78`; the public PDB address
`0x14000ED28` is an instruction within that function. It marks the request as
SET_FEATURE operation `3` and dispatches the supplied `HID_XFER_PACKET`.
`HidGetFeature` has the analogous generic GET_FEATURE operation `2` path
(prologue `0x14000EA6C`, public PDB address `0x14000EA90`). Neither function
contains a comparison against report ID `0x05` or `0x56`.

`CxClient::ProcessBody` at `0x14000C8D8` is the response/input state-machine:
it validates V0 content type, semantic length and expected report ID before it
copies or completes a transfer. It also has no ID5/ID56 semantic branch.

Therefore the code that chooses ID5 and ID56 is above HidSpiCx, in the Windows
HID client stack. The remaining static targets are that caller and the
HeatCore/TouchPen report consumers; HidSpiCx cannot establish their meaning.

## Static Payload Search

PCRE2 byte searches found no `bd 0c ee 5b 44 4c` sequence, with or without a
leading `56`, in the captured `HidSpiCx.sys`, `hidspi.sys`, `amdspi.sys`, or
`SurfaceVirtualFunctionEnum.sys`. This is negative evidence only: it rules out
a static copy of this exact ID56 payload in the available kernel corpus. The
payload is consequently either built at runtime or supplied by a component not
present in that corpus.

## User-Mode Candidates

`TouchPenProcessor0C19.dll` is a report consumer, not a Feature producer: its
only HID imports are `HidP_GetCaps` and `HidP_GetValueCaps`.

`SurfaceService.exe` contains `CTHCTelemetry`, which enumerates digitizer HID
TLCs, checks report capability, and has a named
`CTHCTelemetry::HidDeviceFeatureReportGet` path using `HidD_GetFeature`. Its
imports and embedded dynamic-import strings contain `HidD_GetFeature` but not
`HidD_SetFeature`; the captured ID56 byte sequence is also absent. This makes
that named telemetry path read-only, not evidence for the ID56 producer. It
does not exclude other `SurfaceService.exe` code: the executable also uses the
WinRT `Windows.Devices.HumanInterfaceDevice.HidDevice` interface, whose
Feature setter can be invoked through a vtable without a `HidD_SetFeature`
import.

The `HidDevice` runtime-class string is referenced only through its HSTRING
descriptor at `0x140208008`, from `0x14001099E` and `0x140010DED`. Both blocks
obtain the WinRT activation factory and call static-factory vtable slots
`+0x30` or `+0x40` while building a device selector/opening an asynchronous
device operation. They are not calls on an `IHidDevice` instance and cannot by
themselves send a Feature report. The later consumer of the resulting async
device object remains unresolved in this stripped binary. RTTI identifies that
consumer as a captured C++/WinRT
`AsyncOperationCompletedHandler<HidDevice>` lambda, but does not expose a
recoverable `Invoke` symbol or vtable address in this artifact.

The read-only conclusion for `CTHCTelemetry` is instruction-level evidence:
at `SurfaceService.exe:0x14003E6B5-0x14003E6D5`, it places the configured
report ID at byte zero of its allocated buffer and calls the `HidD_GetFeature`
wrapper with the HID handle, that buffer, and the configured report length.
The following block only logs bytes zero through two of the returned buffer.

Independent runtime ETL exports show `SurfaceService` attempting
`CHidDevice::GetDeviceDetails` on MSHW0231 `Col04` and `Col08` (PID 5456 in
the boot capture and PID 5340 in the runtime capture). These are separate
Windows sessions from `surface_boot_auto.etl`; their PIDs cannot identify PID
2364 in that trace. They support only the narrower fact that SurfaceService
enumerates MSHW0231 HID collections.

The available `SurfaceCFUOverHid.dll` imports HID parser functions but no
`HidD_SetFeature`; `SurfacePenCfuOverBleLc.dll` has no HID API import. Neither
contains the ID56 payload. No acquired user-mode binary can presently be
attributed as its producer.

The SPB CSV has the originating PID/TID on its `Start` event, but
`SurfaceHidBoot.wprp` enables only SPB, GPIO, HIDCLASS, and SurfaceHidMini
providers. It does not collect Kernel-Process events or stackwalks, so PID
4400/PID 2364 cannot be mapped to an executable from this ETL. HIDCLASS events
also do not share either SET's Activity or Related Activity ID. The capture
cannot resolve this remaining user-mode ambiguity.

The future `tools/windows_capture/touch_boot.wprp` adds
`Microsoft-Windows-Kernel-Process` (`22fb2cd6-0e7b-422b-a0c7-2fad1fd0e716`),
so a new ETL can map each SPB submitter PID/TID to its process image. It does
not alter existing captures and requires no Linux driver action.
