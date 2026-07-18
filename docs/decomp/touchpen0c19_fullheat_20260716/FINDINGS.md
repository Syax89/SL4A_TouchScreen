# TouchPenProcessor0C19 Heat Path Findings

## Analysis Boundary

Offline Ghidra 12.1.2_DEV analysis of `/home/simone/windrivers/TouchPenProcessor0C19.dll` only. No device, HID endpoint, driver load, service, network, or hardware access was performed. The PE is x86-64 (Windows ABI), MD5 `de83210b2299aaba5c2c9819f166c0fe`. Its embedded PDB path is `C:\w\69\b\Release\x64\bin\TouchPenProcessor.pdb`; the PDB was not present locally, so all names are PE exports, RTTI/demangler output, or Ghidra-generated `FUN_*` labels.

## Exact Scope And Counts

- 12 PE exports: complete list in `exports.tsv`.
- 18 scope roots: 10 named `SurfaceHeatProcessor` methods and 8 unnamed vtable-only methods.
- 468 direct-call-reachable internal functions, each with complete decompiler C in `functions/` and metadata in `functions/index.tsv`.
- 1 relevant vtable at `1806c07b8`, with 13 method slots in `vtables.tsv`.
- 1,090 direct internal call edges in `callgraph.tsv`.
- 83 keyword-matching strings and 94 total unique strings referenced or matched in `strings.tsv` (one header row excluded from each count).
- 6,027 direct scoped-code to non-executable-static-data references in `static_data_references.tsv`.

The function closure intentionally includes shared helpers, CRT, math, and allocator code reached by the processor path. It does not claim that every closure member is heat-specific business logic. The eight vtable-only roots have no direct callers in Ghidra's direct-call graph, and are the identified unreferenced-but-relevant methods: `180694050`, `1806941f0`, `180694390`, `180694900`, `180694a20`, `180694a30`, `180694a80`, and `180694d60`.

## Exposed Processor Interface

`SurfaceHeatProcessor` exports `GetProcessorCapabilities`, `HandleRegionConfigMessage`, four context handlers, `OnDeviceAttached`, `OnDeviceDetached`, `OnDeviceReset`, and `OnSystemContextMessage`. `CreateHeatProcessor` is also exported. The vtable slots 8 through 12 are the five virtual named methods: capabilities, attach, detach, system-context dispatch, reset. Slots 0 through 7 are currently unnamed and retained in full.

## Recovered Data Flow

1. `OnSystemContextMessage` at `1806957b0` obtains a context type through an indirect interface call. Values `1`, `2`, `3`, and `4` dispatch respectively to display-change, input-stitching, hinge-angle, and region-config handlers. On a zero status it invokes post-dispatch processing through `this + 0x31f00`.
2. Display-change at `180692900` queries the display context interface using `DAT_1806c0758`, reads values by indirect calls, and passes them to the device logic rooted at `this + 8` via `FUN_18068fe50`.
3. Hinge-angle at `180693490` queries using `DAT_1806c0868`; on successful value extraction it calls `FUN_18068fc50(this + 8, angle, out_flag)` and conditionally sends the resulting value through the queried interface.
4. Input-stitching at `180692d00` queries using `DAT_1806c0878`, updates a two-entry stride-`0x18` per-device configuration region at `this + 0x31e80`, selects a device through `*(this + 0x31f08) + 4`, and updates derived span `*(uint *)(*(this + 0x31f08) + 0x2c)` from `this + 0x31ec0 - this + 0x31eb8`.
5. Region-config wrapping at `180693820` queries `IHeatRegionConfigMessage` using `DAT_1806c06b8` and passes it to `HandleRegionConfigMessage` at `180693ac0`.
6. Region-config processing first obtains a selector and payload via indirect calls, dispatches selectors `0`, `1`, and `2`, writes cached 64-bit values at `this + 0x31ed4/0x31edc` or `this + 0x31ee4/0x31eec`, and drives the object at `*(this + 0x31f08)`. That object has observed fields `+0x08/+0x09` (two enable bytes), `+0x20` (byte), `+0x24` (dword), and `+0x40` (qword).
7. Capability reporting at `180695060` stores a configuration pointer at `this + 0x31ef8`; the pointed structure is based at `*FUN_18060d340() + 0x834`. Observed feature checks are bytes `+0x0f`, `+0x10`, `+0x11`, `+0x12`, and `+0x21`, with a dword at `+0x04`.
8. Attach, detach, and reset delegate to device-logic functions with `this + 8`: `FUN_18068f6b0`, `FUN_18068fa60`, and `FUN_18068f580`, respectively. Attach maps `-0x7ffffffd` to `-0x7ff8ffa9`; detach does the same; reset returns zero after delegation.

## Heatmap And Virtual HID Evidence

The binary contains the telemetry strings `SurfaceHeatProcessor_ProcessHeatmap`, `SurfaceHeatProcessor_SanitizeHeatmap`, `GeneralProcessHeatmapEntry`, `GeneralProcessHeatmapHeatDeviceDetails`, `HeatProcessHeatMapGetSectionPointer`, and `HeatProcessHeatMapGetSectionPointerVendorHeatSection`. It also contains `VirtualHidManager_Init`, `CreateVirtualHidFailed`, `VirtualHidManager_SendCnmData_SendInput`, and `SendCnmData_CnmVirtualHidDeviceIsInvalid`. These are preserved with addresses in `strings.tsv`.

No complete symbolic names for the heatmap or VirtualHid implementations survive in the DLL. Their interface calls are CFG-dispatch indirect calls and thus cannot be added as proven direct callgraph edges without a PDB, type definitions, or dynamic execution. All candidate reachable helpers and their complete decompilation are nevertheless retained in `functions/`; the static-data reference table gives the concrete code-to-data evidence needed to continue resolution offline.

## Static Structures And Offsets

Every direct code-to-static address reference is exhaustively tabulated in `static_data_references.tsv`. Every recovered `this`/context member access is retained verbatim in the corresponding full decompilation; Ghidra did not recover complete class layouts because the PDB is unavailable. The offsets listed in the data-flow section are the observed field accesses, not asserted field names or sizes beyond their load/store widths.

`DAT_1809429f0` gates ETW tracing; `DAT_180942a10` is the ETW registration handle used by the processor methods. The teardown helper at `180692270` writes `SurfaceHeatProcessor::vftable`, unregisters both ETW handles, clears `DAT_180942a10`, `DAT_1809429f0`, `DAT_1808f3fd8`, and `DAT_1808f3fb8`, and destroys the subobject at `this + 8`.

## Method Limit

Vtable discovery is a conservative static scan for runs of at least three initialized, non-executable 64-bit function pointers that contain a scope root. Calls are direct edges only; indirect virtual/interface dispatches are represented in the decompiled C but are not falsely reported as resolved callgraph targets.
