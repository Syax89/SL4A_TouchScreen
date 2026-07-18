# Touch/Pen Heat Resource Inventory

Date: 2026-07-16

Scope: offline, static examination only. No device, HID, driver, ETW session, service, network, or other hardware access was performed.

## Inputs and provenance

| Input | SHA-256 |
|---|---|
| `windrivers/TouchPenProcessor0C19.dll` | `f05828f04eb7df02ad5a2eeae136ef19d504f987afe2c06d0375ee1264300b4a` |
| `windrivers/TouchPenProcessor0C18.dll` | `863c824f092ddd2f6adf61b5431f51e70a6a99ac495903190ddf4825ba99b5ca` |
| `Scaricati/SurfaceLaptop4_AMD_Win11_22631_26.040.2105.0.msi` | `221bcadf4b2f156719853059d679594347d64d965cd7672613dbd4f0f5aba86d` |

The files are 9,770,336-byte x64 PE32+ GUI DLLs, version `0.5.127.137` (`5.127.137.0`), built 2020-09-01 11:35:14. They are not byte-identical: 8,281 byte positions differ. `.text`, `.pdata`, `.reloc`, PE resource data, the embedded ZIP, and extracted XML are identical. `.rdata`, `.data`, and Authenticode certificate blobs differ. Per-artifact and per-section hashes are in `metadata/`.

## Preserved extraction

* `artifacts/0C19/` and `artifacts/0C18/`: original DLLs and their MSI-extracted INF/CAT package artifacts.
* `pe/TouchPenProcessor0C19/` and `pe/TouchPenProcessor0C18/`: all PE sections exposed by 7-Zip, certificate blob, PE resources, carved embedded ZIP, and expanded XML.
* `msi/SurfaceUpdate/`: complete MSI payload extraction, including both `surfacetouchpenprocessorupdate_*` packages.
* `msi/tables/`: relevant extracted MSI database tables.
* `strings/`: full ASCII and UTF-16LE inventories plus classified candidate strings.
* `metadata/`: PE/installer listings, object metadata, extraction logs, and hashes.

## PE resources and embedded containers

Both DLLs contain exactly these PE resources:

| Resource | Size | Result |
|---|---:|---|
| `.rsrc/version.txt` | 1,432 bytes | Version metadata; Microsoft, internal/original name `TouchPenProcessor.dll`, file/product version `0.5.127.137`. |
| `.rsrc/MANIFEST/2` | 381 bytes | XML assembly manifest, `asInvoker`, `uiAccess=false`. |
| `GenericCliDescriptor.zip` | 2,048 bytes | Valid single-file ZIP carved at file offset `9702896` (`0x940df0`); 7-Zip validates its 2,047-byte archive body and reports one trailing byte. |
| `GenericCliDescriptor/GenericCliDescriptor.xml` | 8,939 bytes uncompressed | Command descriptor expanded from the ZIP. |

No other ZIP local-file signature, XML document, or PE resource type was found. The PE certificate is preserved as `CERTIFICATE`; it was not interpreted as configuration. The resource XML and descriptor XML are byte-identical between 0C18 and 0C19.

## Heat-related installation configuration

The INF comment identifies this as the `Surface Touch Heat MW Update` driver. Both packages are Extension-class drivers for Windows x64 10.0.18362 and later, installed to directory ID 13 (`%SystemRoot%\\System32`). Each writes `HKR,Heat,SoftwareProcessor` with the corresponding DLL path.

| Variant | HID hardware ID | ExtensionId | `Heat\\SoftwareProcessor` value |
|---|---|---|---|
| 0C19 | `HID\\MSHW0231&Col02` | `{57812147-5605-4605-A884-CC5B7A0FBA45}` | `%13%\\TouchPenProcessor0C19.dll` |
| 0C18 | `HID\\MSHW0230&Col02` | `{47BAA3FA-E662-4FF6-9BE1-1892E69B4B68}` | `%13%\\TouchPenProcessor0C18.dll` |

Shared INF values: class GUID `{E2F84CE7-8EFA-411C-AA69-97454CA4CB57}`, provider `Surface`, `PnpLockdown=1`, catalog `SurfaceTouchPenProcessorUpdate.cat`, and driver version `09/01/2020,5.127.137.0`. The hardware registry security descriptor grants GA to DWM (`S-1-5-90-0`), administrators, and SYSTEM; GR to Users and app packages.

## MSI/WiX configuration

The MSI reports WiX Toolset `3.11.2.4516`, product `Surface Laptop 4 Update 26.040.2105.0`, product code `{B2046FCC-A864-449E-935E-51C92E7A3E5B}`, upgrade code `{C9ECCDE4-9532-4242-8D2E-2CC92C267765}`, and Windows 11/Surface Laptop 4 prerequisite gating.

Relevant File table rows:

| Payload | File key | Component key | Component GUID |
|---|---|---|---|
| 0C19 INF | `filD63AACF335F3E78437A8EBBBE21C6A20` | `cmp639E97D4E3B9E8DBADF8D25E646BF52C` | `{5B430414-EA98-5DE3-84DA-5983FFABF2D0}` |
| 0C19 DLL | `fil09CD7482B62860D3B6AA344B3C0EE255` | `cmpEA65F5161314F9CECD2E399805A372BD` | `{CD19FB98-73A4-5EC7-8132-BAA9E7632DBB}` |
| 0C18 INF | `fil1A9A16CC62426972327AD98FDB1FBA2B` | `cmp8C2FCCB318E5C556BBA6C3B2C170EE27` | `{3910E3B6-6464-5AC5-8242-4A8B8BD50159}` |
| 0C18 DLL | `filE3A8C2A39102A957A6FA39C784F67C0E` | `cmp8D55AD8A3AA220DAE10A05B7ACADFB20` | `{54746112-740E-5E39-BADA-F0EE4EB8DD27}` |

The MSI carries the standard WiX UI binary streams, `WixUIValidatePath` and `WixUIPrintEula` custom actions, and the stated upgrade/downgrade detection properties. These are installer mechanics, not Heat runtime settings.

## TraceLogging/ETW and event schemas

The DLL imports `EventRegister`, `EventUnregister`, `EventSetInformation`, and `EventWriteTransfer` from `ADVAPI32.dll`, proving ETW API capability. Static names include `ETW0`, `.rdata$zETW0`, `.rdata$zETW1`, `.rdata$zETW2`, and `.rdata$zETW9`. `GenericCliDescriptor.xml` exposes `SetEosTraceLoggingSeverity`/`GetEosTraceLoggingSeverity`, with severities `0=None`, `1=Critical`, `2=Error`, `3=Warning`, `4=Information (default)`, and `5=Verbose`.

No ETW provider name, provider GUID, event manifest (`.man`), TraceLogging metadata name, or static event-ID schema was recovered from the PE resources or printable strings. This inventory therefore does not assign a provider GUID or infer event IDs. The named counters/state labels in `strings/TouchPenProcessor0C19.classified-candidates.txt` are implementation labels, not an event schema.

The only recovered declarative schema is `GenericCliDescriptor.xml`: command IDs 0-48, covering descriptor access, Pen ID/offset logic, pen-edge and hover-IIR thresholds, virtual-antenna factors, XGBoost mode, border coefficients/penalty, panel alignment offsets, palm-rejection switches, hinge/display/stitching state, trace severity, zero-force thresholds, pressure enablement, and touch-report count. It defines command inputs/outputs and types (`B`, `H`, `L`, `f`, `AB`), not ETW events.

## Static calibration/configuration versus schema strings

`GenericCliDescriptor.xml` is a control/interface schema, not a calibration payload. Its Get/Set entries identify configuration that may be stored or supplied elsewhere, including edge thresholds, IIR factors and thresholds, virtual-antenna factors, tip-edge border values, panel alignment offsets, zero-force thresholds, pressure state, and trace severity. It contains no actual calibrated numeric values.

The string inventory contains names such as `CalibrationMapsNotCompatible`, `CalibrationMapsNotCompatibleFwSupportedMapVersionLatest`, `MwConfigVersion`, `FwConfigVersion`, `GainSet`, `PenThreshold[0]`, `PenThreshold[1]`, and `HIGH_PEN_SEARCH_THR`. These are schema/diagnostic identifiers only; no standalone calibration map, JSON/XML configuration payload, or other recoverable static calibration table was identified. The differing `.rdata` and `.data` sections may contain variant-specific binary data, but static analysis here cannot safely label that opaque data as calibration material.

## Reproduction notes

Extraction used local `7z`, `msiextract`, `objdump`, `strings`, `rg`, and `dd` only. Full tool output is preserved in `metadata/`; no source artifact was modified.
