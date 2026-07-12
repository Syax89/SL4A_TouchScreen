# Next Steps — Post-Standard-HID-Mode Roadmap (2026-07-12)

> Standard HID mode (Report ID 0x40) is functional: single-touch + pen work.
> **CCL pipeline fully mapped** — see `GROUND_TRUTH.md` §22.

---

## What's still open (quick summary)

**2026-07-12 — major static analysis breakthrough.** The entire touch-processing pipeline
inside `TouchPenProcessor0C19.dll` has been reverse-engineered statically on Linux (zero
Windows boots). Every function from the IHeatDevice callback entry point through CCL, blob
property extraction, eigenvalue decomposition, and coordinate computation is now mapped
(GROUND_TRUTH.md §22).

Remaining work:

1. **Multi-touch handshake reliability** (§D.0) — `SET_FEATURE` write intermittency remains unresolved.
   The CCL pipeline works once the device is in raw mode; the bottleneck is getting the device
   INTO raw mode reliably.
2. **Raw → screen coordinate mapping** — the raw frame format is now known (288-column, index→float
   lookup table). The pipeline computes blob centroids internally, but the final mapping to
   screen pixels depends on calibration coefficients in the per-device context tables that
    are initialized at setup time (not yet extracted; see GROUND_TRUTH.md §22.8).
3. **Calibration data extraction** — the antenna geometry, coordinate scaling factors, and
   calibration matrices are embedded in the per-device context (0x109fc stride) populated by
   `FUN_1805ff7e0` and `FUN_18068bfc0`. Extracting these tables would complete the Linux
   multi-touch driver without needing a Windows trace.
4. **Linux CCL calibration** — the driver contains an experimental CCL approximation
   (slot tracking, eigenvalue axes/orientation, and distance-based ghost rejection), but it
   still needs validated gain adaptation and calibration data from the Windows pipeline.
5. **Windows side** — an ETW trace with `TouchAndPen.Prod` provider or the host binary
   (found via `collect_touch_host.ps1`) remains the fastest path to empirical validation.
6. **Logging/polish** (§E) — lower-priority cleanup remains.

Everything else (device init, report descriptor, standard-mode single-touch + pen) is stable
and done — see DONE below.

---

## DONE

- [x] Device init: DESCREQ → DEVICE_DESC → DESCREQ2 → RPT_DESC
- [x] HID report descriptor: 936B, 98.5% wire + 14B targeted patch
- [x] Standard HID mode: Report ID 0x40 (TouchScreen) + 0x01 (Pen)
- [x] Small report buffer overflow fix (rl - 2)
- [x] BTN_TOUCH tap/lift detection
- [x] Stylus/Pen input (Report ID 0x01, event21 + mouse2)

---

## §D — Multi-touch via raw heatmap blob detection

The device supports multi-touch in **raw heatmap mode**. The captured Windows trace uses
`SET_FEATURE(selector=4, val=1)`, while the current Linux test device has empirically
streamed with selector 5. This selector difference is unresolved; it must not be treated
as a universal protocol constant. Raw mode sends content_id=0x0C frames (~4302 bytes).

### §D.0 — raw_mode handshake reliability (blocking further calibration work)

`SET_FEATURE` succeeds only ~1 in 4-6 attempts; the rest go completely silent (zero further
GPIO interrupts — not even a `RESET_RSP`). Found that real Windows waits ~4.6ms between
reading `GET_FEAT_RESP` and sending `SET_FEATURE` (we sent it instantly); added a matching
`usleep_range(4500, 5500)` in `driver/spi-hid-core.c` `case 5`, but live testing across
several attempts (including a fresh power-cycle via the new `tools/reset_touch.sh`) showed no
clear improvement (see GROUND_TRUTH.md §18.6). Blob detection / CCL / slot-tracking logic
fixes are confirmed working live in the one session where the handshake did succeed, but
further calibration work (mapping grid position to physical screen position) needs a
reliably-connectable device to do efficiently.

The driver implements a bounded watchdog, but it is mitigation rather than discovery: it
cannot establish a safe selector or explain why a SET_FEATURE attempt silences the controller.

**Update — decompiled the real answer from `HidSpiCx.sys` (GROUND_TRUTH.md §18.7)**: Windows
does exactly this. `HidSpiCx.sys`'s SmFx state machine arms a **2000ms response timer** for
GET_FEATURE/SET_FEATURE (and report descriptor/write requests), and on timeout runs
`CheckingResetRetryCountEntry`, which retries up to a hardcoded **3 times** before giving up.
Windows is not more reliable than us at the protocol level — it just automatically retries
invisibly. Concrete parameters to match: **2000ms per-attempt timeout, 3 retries**.

**[x] DONE (2026-07-08)** — implemented `spi_hid_raw_handshake_watchdog()` in
`driver/spi-hid-core.c`, matching Windows's parameters exactly (`RAW_HANDSHAKE_TIMEOUT_MS`
2000, `RAW_HANDSHAKE_MAX_RETRIES` 3). Armed via `schedule_delayed_work()` right after sending
GET_FEATURE; cancelled/confirmed on the first real heatmap frame (`raw_handshake_confirmed`);
on timeout, resends DESCREQ and retries, decrementing a counter initialized once in `probe()`
(a first version reset the counter every retry inside the RPT_DESC branch itself, which is
re-entered by the watchdog's own DESCREQ retry — found live: the same "(2 left)" message
repeated forever instead of counting down; fixed by initializing the counter only in
`probe()`). Verified live: counter correctly counts down 2→1→0 and gives up cleanly when the
device really won't cooperate; confirms and cancels itself cleanly on first-try success with
no spurious re-firing; zero crashes across several load/unload cycles; standard mode
(`raw_mode=0`, default) confirmed completely unaffected (new fields are simply never touched
when raw_mode is off).

**Do not use `tools/reset_touch.sh` as a general recovery path.** It invokes direct ACPI
`\M010` GPIO methods. The canonical investigation log records such direct calls as capable
of leaving the controller silent until cold boot. Recovery experiments must use the ACPI power
state machine and collect lifecycle trace data.

### What we know

- The raw payload is 4297 bytes of "constant" data per the HID descriptor
- This is NOT a simple capacitance grid — it's dual-frequency DFT antenna data
  with real and imaginary components. **Corrected 2026-07-12** (`GROUND_TRUTH.md` §21.2,
  found via decompiled `shortAntMaxMag0..15`/`longAntMaxMag0..15` field-logging code): **16
  Short + 16 Long antennas**, not the previously-recorded 9+9 (that figure was never
  independently confirmed).
- 4297 is a prime number → cannot be a rectangular grid
- Windows processes this via `TouchPenProcessor0C19.dll` (9.7 MB) — **decompiled 2026-07-12**,
  see `GROUND_TRUTH.md` §21 for the full write-up:
  - DFT processing (PenMagToBits, ShortDftRefAntAllTouchedReal/Image)
  - Connected Component Labeling (TouchDetectionCclLogic)
  - Kalman tracking (TrackLibRunTrackingLogicEntry)
  - 3 gain sets, multi-level thresholds (15%-37.5%)
  - Already computes and can emit `TouchBlobCoMX`/`TouchBlobCoMY` per finger via a
    TraceLogging ETW provider (`Microsoft.Surface.TouchAndPen.Prod`,
    GUID `{3FA102E9-1A62-5490-7AF8-6088C2F9E6BE}` — recovered directly from the binary's raw
    bytes, see §21.4). Not present in the existing `traces/*.csv` captures (those only cover
    the GPIO/HIDCLASS/SPB providers) — would need a fresh Windows-side capture.

### What we tried

- Blob detection on assumed 86x50 grid: produces stable single blob but coordinates
  don't match physical position
- Attempts at 87x49, 64x67 grids: similar results
- Dilation + EMA smoothing: improves stability but doesn't fix grid alignment
- Axis inversion (invert_x, invert_y, swap_xy): no combination matches correctly
- (worth retrying with the corrected 16+16 antenna count above)

### What's needed — three alternative paths

**Path C (Linux-only, requires a controlled active experiment)** — found 2026-07-12,
see `GROUND_TRUTH.md` §21.5: `TouchPenProcessor0C19.dll` embeds a ZIP archive containing
`docs/decomp/GenericCliDescriptor.xml`, a full diagnostic CLI protocol (49 commands) including
`GetDescriptorBuffer`/`GetDescriptorPointer` (read a calibration descriptor **stored on the
touch controller itself**) and `GetPanelAlignmentOffsetByRegion` ("panel offset by region in
mm"). Our own HID report descriptor has a matching, currently-unused transport: **Report ID 31
(0x1f)**, a 60-byte Input + 60-byte Output pair on vendor usage page `0xff0f`/`0x50`. Exact
wire framing not yet confirmed (no dispatcher code found in the DLL — it's a generic/reflection
dispatcher, not per-command functions). Next step: try sending `GetVersion` (id 0, simplest
command, no params) as a 60-byte Output report to Report ID 31 and see what comes back on
Input. It is not assumed safe: other nominally read-only vendor commands have reset this
controller, so this requires a defined recovery boundary and immutable transaction logging.

**Path A (empirical via Windows ETW, not yet attempted)**: capture a fresh ETW/TraceLogging trace
on Windows with provider `{3FA102E9-1A62-5490-7AF8-6088C2F9E6BE}` enabled (e.g. `logman start
touchcap -p "{3FA102E9-1A62-5490-7AF8-6088C2F9E6BE}" 0xffffffffffffffff 0xff -ets`), touch known
physical screen positions during capture, then correlate the logged `TouchBlobCoMX`/`CoMY`
values against those known positions to empirically fit a grid→screen transform. Avoids fully
reversing the DFT math — same "trust the real trace" method that solved the SPI protocol.

**Path B (full static reverse-engineering, partially done)**: extract from
`TouchPenProcessor0C19.dll`:

1. The antenna-to-position mapping matrix (the `RectAnt.ShortAntsStart/End`/`LongAntsStart/End`
   strings exist in the binary but aren't yet reached from the ~25 functions found so far —
   likely accessed through a pointer table, needs one more hop, see `GROUND_TRUTH.md` §21.2)
2. The DFT window parameters (Short vs Long window sizes)
3. The coordinate calculation formulas (PenPosition, PenCenterOfMass)
4. The noise rejection and gain control logic

### Files to investigate

| File | Size | Contents |
|------|------|----------|
| `windrivers/TouchPenProcessor0C19.dll` | 9.8 MB | Main touch/pen processing DLL — decompiled 2026-07-12, see `GROUND_TRUTH.md` §21 |
| `docs/decomp/uefi/SurfaceTouchHidDxe.c` | 1944 lines | UEFI touch driver (`>>5` in `FUN_00001484` is a fixed-point scale on the *standard-mode* Report 0x40 X/Y, unrelated to the raw multi-touch blob — checked 2026-07-12) |
| `docs/decomp/uefi/MsTouchUnlockDxe.c` | 2220 lines | Touch unlock/calibration sequences |

---

## §E — Cleanup and polish

- [x] Remove unused heatmap blob detection code — done 2026-07-08: guarded `touch_input`/blob
      pipeline behind `raw_mode`, deleted 6 dead functions the compiler flagged as unused
      (`spi_hid_output_body`, `spi_hid_bus_input_report`, `spi_hid_assert_reset`,
      `spi_hid_deassert_reset`, `spi_hid_power_up`, `heatmap_report_touch`) — build now has
      zero warnings.
- [x] Repo-wide cleanup 2026-07-08 — removed the entire bring-up-phase scaffolding now that
      the driver works end-to-end: `tools/diagnostics/` (dozens of one-off raw-MMIO kernel
      modules answering "does DESCREQ even reach the device" — long since answered yes),
      `tools/raw-mmio-test/`, root-level `test_raw.sh`/`test_seq.sh`, `tools/gpio_test.c`
      (superseded by `tools/reset_touch.sh`), unused companion-chip firmware blob data
      (`module/b0_blocks.h`, confirmed unreferenced anywhere in `driver/`), stale/superseded
      docs (`docs/analisi_MSHW0231.md` — pre-breakthrough analysis whose "no mode-change
      command exists" conclusion is now known wrong; `docs/SESSION_2026-07-06.md` — explicitly
      says "write path not yet functional"; `docs/VERIFICATION_FINDINGS.md`/`VERIFICATION_PLAN.md`
      — an earlier, unrelated SPI-controller bug audit from 2026-07-04, long since fixed;
      `docs/DRIVER_STATE.md` — thin duplicate of this README). All recoverable from git
      history. `docs/GROUND_TRUTH.md`, `docs/NEXT_STEPS.md`, and the protocol/register
      reference docs (`SPI_REGISTERS.md`, `HIDSPI_PROTOCOL.md`, `AMDSPI_DECOMP.md`,
      `CSV_SEQUENCE.md`) remain as the current source of truth.
- [ ] Reduce `dev_info` logging noise on the stable standard-mode path (the RPT_DESC/
      GET_FEAT_RESP hex-dump-style `DIFFCHECK:` logs are harmless but verbose; not gated
      behind a debug flag yet)
- [ ] Proper sysfs interface for diagnostics
- [x] Systemd service for auto-load at boot — done 2026-07-08: cleaned up
      `driver/sl4a-touch.service` (removed two stale `setpci` lines left over from the
      long-closed PCI 0xB4/0xB8 investigation, fixed `ExecStop` module names), installed to
      `/etc/systemd/system/`, enabled + started. Loads `raw_mode=0` (standard, stable mode)
      on every boot.
- [ ] Kernel module signing for Secure Boot

---

## §F — Upstreaming

Split into Linux kernel patches:
1. `spi-amd.c` — AMD FCH Cezanne SPI controller V2 driver
2. `spi-hid-core.c` — HID-over-SPI transport driver (V0 protocol)
3. Device tree / ACPI binding for MSHW0231
4. Documentation
