# TouchPenProcessor0C19 Heat Tables

Offline static-analysis artifact for `TouchPenProcessor0C19.dll`, SHA-256
`f05828f04eb7df02ad5a2eeae136ef19d504f987afe2c06d0375ee1264300b4a`.
No hardware or Windows execution was used.

## Confidence terms

- **High**: initialization and a runtime consumer are both decompiled.
- **Medium**: consumer and layout are decompiled, but values come from the device/config at runtime.
- **Low**: a schema/debug name exists but no value-producing consumer was found.
- **None**: raw bytes/string exists, but static xrefs did not reach a code consumer.

## Recovered Tables And Constants

| Item | Values or formula | Consumer evidence | Confidence | V0 captured-frame connection |
|---|---|---|---|---|
| `c590` signal LUT | Context `+0xc590`; 256 float slots. Init writes `i=0..254`: `c590[i] = 1.0f - (i * 0.00222035428f + 0.600000024f)`. Thus `c590[0]=0.399999976`, zero crossing is about `i=180.151`, and `c590[254]` is about `-0.16396998`. The loop is strictly `i < 0xff`; slot 255 is not written by this initializer. | `FUN_1805ff7e0` at `0x1805ff7e0`, `ccl_consumers.c:3512-3531`; used for thresholding, neighborhood tests, and centroid weighting in `FUN_180600c40` / `FUN_180602e60`. | High | **Conditional, not proven.** The CCL input is a 288-byte-stride converted buffer, and each byte indexes this LUT. No recovered code maps a captured 4,297-byte V0 payload directly to this buffer. |
| LUT scale constants | Base `DAT_180716340=1.0f`; step `DAT_1806c089c=0.00222035428f`; offset `DAT_1806cd848=0.600000024f`; `DAT_1806c6458=0.5f`. | Raw `.rdata` reads and the LUT initializer above. | High | Same conditional connection as `c590`. |
| Frame stride and geometry | Data scanline stride is `0x120 = 288` bytes. Context width/height are `u16` at `+0x1639c/+0x1639e`; subsampled dimensions are `ceil(width/subsample_factor)` and `ceil(height/subsample_factor)`. | `FUN_180602e60`, `FUN_1805ff7e0`: `ccl_consumers.c:2897-2903`, `3502-3503`, `3621-3623`. | High | **No direct connection.** Captured V0 payload length is non-rectangular; this is a post-conversion CCL representation. |
| Per-frame global gain | `global_gain = u16(frame_data + 8) * 0.00999999978f`; stored at context `+0xc580`. `frame_data + 0x0c` is stored at `+0xc570`. | `FUN_1805ff7e0`: `ccl_consumers.c:3506-3510`; consumed by `FUN_180602e60`: `2883-2886`. | High | **Conditional.** It uses the host callback's frame-data object, not a demonstrated offset in captured V0 bytes. |
| CCL local threshold and background | For a blob/pixel record, local threshold is `c590[record[0x4b]] * global_gain`; background is `c590[*(u16 *)(*(u64 *)(ctx+0xc570)+8)]`, optionally replaced by `*(float *)(ctx+0xc858)`. A neighbor is admitted when its LUT signal exceeds the threshold. Centroid weights use `c590[raw] - background` when positive in the overflow/edge paths. | `FUN_180602e60`: `ccl_consumers.c:2883-2892`, `2907-2949`, `3043-3064`. | High | **Conditional.** Applicable only after a V0 frame has been converted to the internal 288-stride buffer and associated frame-data object. |
| Centroid formula | `x = sum(x * w) / sum(w)` and `y = sum(y * w) / sum(w)`, with `w = c590[raw] - background` on accepted pixels. | `FUN_180602e60`: `ccl_consumers.c:2909-2913`, `3062-3064`. | High | **Conditional.** This produces internal grid coordinates, not a V0 byte offset or screen coordinate. |
| `c98c` antenna/group map | Context `+0xc98c`, **156 bytes**, initialized to `0xff`. For each non-`0xff` raw antenna index in device configuration, store the lowest processing-group ordinal that references it. Two layouts: dimensions at config `+4/+8/+0xc` with byte data `+0x10`, or dimensions at `+0x138/+0x13c` with byte data at `+0x140` (`(group+0x14)*0x10`). | `FUN_1805ff7e0`: `ccl_consumers.c:3536-3605`; runtime indexing `3154-3157`. | Medium | **No direct connection.** Table content is device/config supplied; no V0 byte-to-antenna mapping was recovered. |
| Edge/border behavior | Grid bounds are width/height minus one. CCL tests right/bottom bounds and marks edge/border conditions; frame memory remains 288-stride even where active width is smaller. | `FUN_180600c40`: `ccl_consumers.c:1217-1218`, `1385-1410`; centroid neighbor tests above. | High for behavior; no numeric edge table | **Conditional.** Only after conversion. No per-edge numeric calibration table was found. |
| Active-frame lifetime | Default max active frames is `100`. If config `+0xe68 == 1`, use `max(100, u16(config+0xe64))`; stored at context `+0x194aa`. | `FUN_1805ff7e0`: `ccl_consumers.c:3633-3644`. | High | No. This is state/config, not payload decoding. |
| Grid-to-host scaling | Raw-device context: `x_scale=(phys_x*global_scale)/(grid_width-1)` at `+0x2fe8`; `y_scale=(phys_y*global_scale)/(grid_height-1)` at `+0x2fec`; `x_factor=extra_x/(extra2_x*global_scale)` at `+0x2ff0`; `y_factor=extra_y/(extra2_y*global_scale)` at `+0x2ff4`; aspect `(cfg[0x1f9c]-cfg[0x1fa0])/(cfg[0x1fa4]-cfg[0x1fa8])` at `+0x2ff8`. `global_scale=0.00999999978f`. | Device initialization: `ccl_consumers.c:4104-4111`. | Medium | No. Physical values are descriptor/config derived, and final screen mapping occurs outside this DLL's CCL output path. |
| Fixed grid-like constants | `DAT_180765c70=10.0f`; nearby `.rdata` values include `11, 15, 15.9997559, 16, 20`. Only `10.0f` is directly consumed in the selected heat initializer as a numerator for `+0x19484/+0x1948c`. | `FUN_1805ff7e0`: `ccl_consumers.c:3624-3638`. | High for 10.0 use; low for nearby values' semantics | No direct connection. |
| CCL capacity/layout | Up to 47 blob slots, stride `0x254`; pixel records stride `0xb0`; per-pixel label at `+0x41`, antenna/LUT selector at `+0x4b`. | CCL consumers and dispatcher decompilation in `ccl_consumers.c`. | High | No direct byte layout connection. |
| Eigen/orientation formula | Weighted moments feed `lambda=(trace +/- sqrt(trace^2-4*det))/2`; angle uses `atanf((2*Sxy)/(Syy-Sxx))`. | `FUN_180600c40` selected decompilation; see existing derived analysis in `docs/GROUND_TRUTH.md` section 22.6. | Medium | Conditional only after conversion; not a V0 field map. |
| Touch detection percentages | `15%, 22.5%, 25%, 37.5%` appear in the processor's diagnostic/config vocabulary, but this pass did not recover an initializer plus CCL comparison that proves their table location or selection rule. | Keyword-driven consumer sweep; no value table recovered. | Low | No. Do not use as V0 thresholds. |
| Tracking/Kalman configuration | Schema fields exist: `IsNeedToRunTrackLib`, `IsTrackKalmanCorrectInitialized`, `KalmanLocationConstant[0..1]`, `KalmanVelocityConstant[0..1]`. This sweep reached TraceLogging schema builders, not the tracking configuration producer/consumer; numeric values were not recovered. | `keyword_xrefs.c` header and schema-builder references. | Low | No. |
| `RectAnt.*`, `ccpSection*`, `PenDft*`, per-region/edge calibration | Raw strings are present but this two-hop xref scan found no code/data pointer referrer. No numeric RectAnt, region, or edge-calibration table can be attributed to a heat-processing consumer in this build. | `keyword_xrefs.c`; Ghidra run: 137 keyword hits, 47 referenced strings, 0 pointer slots. | None | No. |
| Antenna magnitudes | Code-referenced ETW schema names establish 16 short and 16 long antenna magnitude fields. They establish telemetry field count, not an antenna geometry/factor table. | Keyword consumer sweep and existing static result in `docs/GROUND_TRUTH.md` section 21.2. | Medium for count; none for geometry/factors | No. |

## Important Limits

1. The DLL receives frames through an `IHeatDevice` callback. The host callback implementation and controller descriptor are not in this DLL.
2. The captured V0 payload is approximately 4,297 bytes and is DFT-oriented. The recovered CCL scan buffer is 288 columns by runtime-selected rows. Static analysis did not recover the converter's complete V0-to-grid/antenna mapping.
3. Device-specific antenna factors, RectAnt geometry, panel-region offsets, and Kalman numerical coefficients are runtime descriptor/config data or have no attributable consumer in this binary. They are deliberately recorded as unrecovered, rather than inferred from strings.
4. `c590` is a processor lookup function, not proof that a captured raw V0 byte at the same numeric offset is a CCL intensity index.

## Evidence Files

- `ccl_consumers.c`: Ghidra decompilation of the callback dispatch, CCL orchestrator/helpers, centroid, calibration initializer, and device initialization.
- `keyword_xrefs.c`: Ghidra two-hop keyword/xref decompilation for RectAnt, ccpSection, PenDft, antenna, threshold/filter, and TrackLib/Kalman terms.
- `MANIFEST.md`: hashes, commands, scope, and reproducibility record.
