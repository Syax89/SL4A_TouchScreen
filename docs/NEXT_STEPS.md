# Next Steps — Post-Standard-HID-Mode Roadmap (2026-07-08)

> Standard HID mode (Report ID 0x40) is functional: single-touch + pen work.
> This doc tracks remaining work.

---

## What's still open (quick summary)

1. **Multi-touch handshake reliability** (§D.0) — `raw_mode=1`'s `SET_FEATURE` write only
   succeeds intermittently; root cause not found (ruled out: clock speed, opcode-doubling
   framing — see `GROUND_TRUTH.md` §18.8). A watchdog auto-retries (matching Windows's own
   2000ms-timeout/3-retry behavior) but doesn't fix the underlying cause. Blocks efficient
   multi-touch calibration work since the device doesn't reliably connect in raw mode.
2. **Multi-touch coordinate mapping** (§D) — once raw mode connects, the DFT antenna
   layout → screen position mapping is still unknown (4297 bytes isn't a clean rectangular
   grid); needs `TouchPenProcessor0C19.dll` reverse-engineering or empirical calibration once
   the device connects reliably enough to do it.
3. **Logging/polish** (§E) — a few lower-priority cleanup items remain (see below).

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

The device supports multi-touch, but only in **raw heatmap mode** (enabled by
SET_FEATURE(id=4, val=1)). In this mode the device sends content_id=0x0C
frames (~4302 bytes) containing DFT antenna data.

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

**Proposed next step (not yet implemented)**: a software watchdog — a delayed_work armed
right after sending `SET_FEATURE`, that if no `type=1` DATA report arrives within a bounded
window, treats it exactly like a detected `RESET_RSP` (re-send DESCREQ, reset state to 1)
instead of waiting forever for an IRQ that in the failure case never comes. This wouldn't fix
the root cause but would turn a ~20% per-attempt success rate into an automatic retry loop
that converges within a few seconds most of the time, without requiring manual insmod/rmmod
cycles.

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

**`tools/reset_touch.sh`** (new): power-cycles just the touchscreen via ACPI `\M010`
(same GPIO method as `test.sh`'s inline module), independent of whether spi-amd/spi-hid are
loaded. Confirmed working. Useful as a general recovery tool (e.g. after `raw_mode=1` leaves
the device stuck in raw mode, which otherwise needs a full reboot).

### What we know

- The raw payload is 4297 bytes of "constant" data per the HID descriptor
- This is NOT a simple capacitance grid — it's dual-frequency DFT antenna data
  with real and imaginary components (9 Short + 9 Long antennas in each direction)
- 4297 is a prime number → cannot be a rectangular grid
- Windows processes this via `TouchPenProcessor0C19.dll` (9.7 MB):
  - DFT processing (PenMagToBits, ShortDftRefAntAllTouchedReal/Image)
  - Connected Component Labeling (TouchDetectionCclLogic)
  - Kalman tracking (TrackLibRunTrackingLogicEntry)
  - 3 gain sets, multi-level thresholds (15%-37.5%)

### What we tried

- Blob detection on assumed 86x50 grid: produces stable single blob but coordinates
  don't match physical position
- Attempts at 87x49, 64x67 grids: similar results
- Dilation + EMA smoothing: improves stability but doesn't fix grid alignment
- Axis inversion (invert_x, invert_y, swap_xy): no combination matches correctly

### What's needed

The fundamental challenge is understanding the **DFT antenna layout** — how the
4297 frequency-domain samples map to physical positions on the screen.
This requires reverse-engineering the `TouchPenProcessor0C19.dll` to extract:

1. The antenna-to-position mapping matrix
2. The DFT window parameters (Short vs Long window sizes)
3. The coordinate calculation formulas (PenPosition, PenCenterOfMass)
4. The noise rejection and gain control logic

### Files to investigate

| File | Size | Contents |
|------|------|----------|
| `windrivers/TouchPenProcessor0C19.dll` | 9.8 MB | Main touch/pen processing DLL |
| `docs/decomp/uefi/SurfaceTouchHidDxe.c` | 1944 lines | UEFI touch driver (>>5 coordinate transform) |
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
- [ ] Systemd service for auto-load at boot (a `driver/sl4a-touch.service` unit already
      exists — needs wiring up/testing as an actual boot-time default)
- [ ] Kernel module signing for Secure Boot

---

## §F — Upstreaming

Split into Linux kernel patches:
1. `spi-amd.c` — AMD FCH Cezanne SPI controller V2 driver
2. `spi-hid-core.c` — HID-over-SPI transport driver (V0 protocol)
3. Device tree / ACPI binding for MSHW0231
4. Documentation
