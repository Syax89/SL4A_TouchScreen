# Changelog

## 1.4.0 — Raw Multitouch Fixes, Unified Installer (2026-07-24)

### Raw multitouch pipeline

- Replaced the greedy zero-assignment blob-to-slot matcher with a correct
  Kuhn-Munkres augmenting-path Hungarian solver; added a track-continuity
  cost bias so two actively-tracked fingers aren't swapped for a marginal
  cost improvement during a pinch/rotate.
- Scaled the ghost-merge radius down (not up) as finger count rises, so
  genuinely distinct close fingers stop being merged into one at higher
  density.
- Fixed a CCL label-reuse bug that could corrupt a nearby blob's ellipse
  data in dense multi-finger frames.
- Fixed stale `TOUCH_MAJOR/MINOR/ORIENTATION` being reported after a blob
  split.
- Fixed `raw_detect_peaks()`: it compared each cell against only 4 fixed
  points at one exact radius instead of scanning its full neighborhood,
  so every touched cell of a blob (not just its center) counted as a
  "peak" — exhausting the shared 16-peak-per-frame budget after 1-2
  blobs and silently dropping any 3rd+ simultaneous finger. Replaced
  with a true local-maximum neighborhood scan.
- Closed a handshake-watchdog gap where a cold-boot retry timer could
  tear down an already-confirmed-working raw stream.
- Added `tests/raw_pipeline_replay_test.c`: links the real
  `driver/mshw0231-raw.c` pipeline (not hand-written mirrors) against
  saved synthetic heatmap fixtures (`tests/fixtures/raw-replay/`),
  replaying 1-5 simultaneous synthetic touches through the actual code.
  All two independently-verified bugs above (and the peak-detection fix)
  were found and confirmed via this harness plus a double-blind
  post-merge code review.
- Verified on real Surface Laptop 4 hardware: single-touch confirmed
  working end-to-end (raw `/dev/input` capture); simultaneous multi-touch
  not yet re-verified on hardware after the peak-detection fix.

### Installer consolidation

- Replaced `tools/install.sh`, `tools/uninstall.sh`, `tools/activate-fch.sh`,
  and `tools/rebuild_and_install.sh` with a single `tools/sl4a-touch.sh`
  covering `install` / `uninstall` / `activate` / `status` / `logs` /
  `rebuild`. `install` now prompts interactively for a profile (standard
  HID, marked stable/recommended, vs. raw multitouch, marked
  experimental) when run on a terminal without `--standard`/`--raw`;
  non-interactive runs still default to standard.
- Added `status` (installed vs. checkout version, active profile, loaded/
  bound state — read-only, no root) and `logs` (single-file diagnostic
  bundle: versions, DKMS/modprobe state, driver sysfs stats, filtered
  dmesg — for bug reports).
- Fixed a pre-existing bug in `install.sh` that made it fail to parse at
  all under this system's bash (`*[|/\\]*` inside a `[[ ]]` bracket
  expression needs the `|` escaped).
- `install` now activates immediately at the end — no separate manual
  step for the current session.
- `install` also writes and enables a systemd oneshot unit
  (`sl4a-touch-activate.service`, gated on `multi-user.target`) so the
  driver activates automatically on every future boot too. This runs
  after the base system is already up, not during early kernel/initrd
  boot — the actual failure mode the original "Boot safety (black-screen
  fix)" work (see the `refactor/sl4a-distribution (2026-07-23)` entry
  below) was protecting against came from kernel-level ACPI/SPI module
  aliases resolving during early boot with no shell and no recovery
  path. A unit gated on `multi-user.target` runs well past that point:
  a hung or failed activation here still leaves a working login.
  `uninstall` disables and removes the unit (ownership-marker gated,
  same pattern as the modprobe.d config).
- Running the script with no arguments now shows an arrow-key menu
  (Install/Uninstall/Activate/Status/Logs/Quit) instead of a usage/error
  message — no subcommand needs to be typed or remembered. Explicit
  subcommands still work unchanged for scripting/docs.
- Found and fixed four real bugs by actually exercising `activate`
  end-to-end against this machine's live installed state (DKMS,
  Secure Boot enabled, real Surface ACPI topology) rather than only
  `--check`/`--dry-run`, which none of the earlier testing had
  exercised:
  - The Secure Boot / MOK enrollment check trusted `mokutil
    --test-key`'s exit code, which is 1 on this system even when the
    key IS in the enrolled MOK database (mokutil separately checks the
    *running* kernel's live trusted keyring, which only reflects an
    enrollment after the next reboot, and still prints "already
    enrolled" while exiting 1). Combined with `set -o pipefail`, piping
    mokutil's output straight into `grep` made the whole pipeline
    report failure regardless of what grep found. Fixed by capturing
    mokutil's output separately from its exit code.
  - A "no other MSHW\* ACPI device may exist" check unconditionally
    refused to activate on real Surface hardware, which always exposes
    several unrelated MSHW\* nodes (keyboard, sensors, battery, ...)
    with their own drivers already bound. Removed it — the actual
    identifying check (exactly one MSHW0231) was already separate and
    sufficient.
  - The arrow-key menu leaked raw `tput` cursor/erase escape sequences
    onto stdout instead of stderr, corrupting the captured command name.
  - The menu's redraw logic moved the cursor up one line short of what
    it actually printed each frame, so every keypress scrolled the
    terminal instead of redrawing in place.
- Dropped a confusing "The git repo itself was not touched" line from
  `uninstall`'s output; replaced with how to reinstall.

### CI

All three GitHub Actions jobs (`whitespace`, `host-tests`,
`kernel-build`) had been failing since at least the `v1.3.0` tag:

- `kernel-build`: `driver/spi-amd.c`'s `platform_driver.remove` is
  void-returning, matching Linux 6.11+ (this driver targets upstream
  v6.15). Ubuntu 24.04 LTS's 6.8 kernel headers — CI's target, and what
  many real users on non-rolling distros still run — predate that
  signature change and reject it outright under `-Werror`. Fixed with a
  `LINUX_VERSION_CODE` conditional.
- `kernel-build`: `driver/spi-hid-capimg.c` unconditionally included
  `<linux/unaligned.h>`, which doesn't exist on that same 6.8 kernel
  (only the older `<asm/unaligned.h>`). Fixed with `__has_include`
  instead of another version check, since the exact boundary isn't a
  clean single version across distro backports.
- `host-tests`: `raw_pipeline_math_test` and `raw_pipeline_replay_test`
  linked with `-lm` placed *before* the object/source files needing it —
  works with this dev machine's linker defaults, fails under `-Wl,--as-needed`
  (the Ubuntu CI runner's default) with "undefined reference to `sqrt`".
  Fixed the link order.
- `whitespace`: `git diff --check` was flagging ~34 files, almost all
  verbatim forensic/reference data (`captures/`, `docs/acpi/`
  iasl-decompiled tables, `docs/decomp/` pasted decompiler output,
  `tools/windows_capture/`, `traces/`) where "fixing" the whitespace
  would mean editing evidence data. Excluded those paths from the check.
  Fixed the small number of remaining real hits directly
  (`tools/parse_spi.py`, one incidental blank-with-whitespace line, one
  inconsistent markdown hard-break, one stray blank line at EOF in
  `driver/hardcoded_rd.h`).
- Fixed a same-day regression: `tests/boot_binding_safety_test.py` still
  read the three now-deleted `tools/{install,uninstall,activate-fch}.sh`
  files after the installer consolidation above; updated it to read the
  consolidated `tools/sl4a-touch.sh`.
- Bumped `actions/checkout` to v5 (clears a Node.js 20 deprecation
  warning on every run).

## 1.3.0 — Production Hardening (2026-07-23)

### Double-Blind Audit
- 12 independent agents audited 6 domains (controller, HID transport, raw consumer,
  headers/protocol, tests, scripts/DKMS) in parallel.
- 3 findings confirmed as false positives by cross-verification against source.
- 45 confirmed findings: 6 critical, 21 high, 18 medium.

### Critical Fixes
- `spi_hid_latency_show`: fix snprintf buffer overflow (PAGE_SIZE not reduced).
- `hid_allocate_device`: fix NULL-vs-ERR_PTR check causing NULL dereference.
- `spi-amd` combined TX+RX: add tx_len chunking when exceeding FIFO (70 bytes).
- `spi-amd`: check `amd_spi_set_opcode` return value at all 3 call sites.
- `install.sh`: fix sed injection via PKG_VERSION metacharacters.
- `uninstall.sh`: add missing `-o pipefail`.

### High Fixes
- `debug_level` renamed to `sl4a_debug_level` (was non-static global).
- Remove no-op lock/unlock in suspend; stop bypassing HID core `claimed` field.
- Fix poll_work off-by-one byte offsets (data_buf[6]->[5], [8]->[7]).
- `spi-amd`: FIFO combined-length validation (tx+rx+echo <= 70).
- `spi-amd`: data preserved after PSP post-execute takeover.
- `mshw0231-raw`: clamp overflow guards on ghost_dist and blob_max_distance.
- `mshw0231-raw`: fix u32->u16 truncation in screen coordinates.
- `mshw0231-raw`: fix Hungarian row_match stale assignments per round.
- `mshw0231-raw`: `input_register_device` failures now return -ENODEV.
- `spi-hid-core.h`: remove __packed from isolated_set_frame (misaligned __u64).
- `spi-hid_trace.h`: guard memcpy against negative tx_len.
- `spi-hid-core.h`: unify device_descriptor_register/hid_desc_addr to u32.
- `activate-fch.sh`: wait timeout increased 2s -> 15s.

### Medium Cleanup (-177 lines)
- Removed ~30 unused constants, isolated_set subsystem, raw_transition fields,
  raw_capture_lock, latencies ring buffer, DEV_EVENT/BUS_ERROR/POWER_SUPPORT.
- Deduplicated vendor_init/sf_cmd/gf_cmd byte arrays (4->1 static const each).
- Removed V1 code paths and DBG_VERBOSE blocks from spi-amd.
- Fixed c590 LUT precision (22 -> 22204/1000).
- Removed touch_threshold_pct (dead param), GRID_CELLS (dead define).
- Moved param clamp from hot path to init (one-time instead of per-frame).
- Activated descreq_work polling (scheduled on WAIT_DESC transition).
- Added 6 orphaned test files to Makefile.

### New Tests (+30 assertions)
- `c590_atan2_host_test.c`: 33 assertions for LUT values and atan2 fixed-point math.
- `capimg_decoder_host_test.c`: extended from 41 to 71 assertions (null pointers,
  magic bytes, content_id, container length, body length boundaries, reserved
  fields, raster sample count, section types, missing sections, zero-length).
- `protocol_test.c`: extended from 92399 to 92409 assertions (parse_content
  boundaries, output_length_valid range).
- **Total gate**: 92,513 assertions, 0 failures, ASan/UBSan clean, LLVM=1 clean.

---

## refactor/sl4a-distribution (2026-07-23)

### Boot safety (black-screen fix)

- Renamed modules to `sl4a-spi-amd`/`sl4a-spi-hid`; no longer collide with
  in-tree `spi-amd` or auto-bind at boot.
- Removed all ACPI/OF/SPI module aliases from both modules.
- Added `tools/activate-fch.sh` for explicit post-login controller + HID
  transport binding with rollback on failure.
- Added PSP ownership gate (`SPI_MISC_CNTRL` bit 10) before every MMIO write;
  PSP-owned controller returns `-EBUSY`.
- Deferred speed programming to transfer path; setup performs no MMIO writes.
- SPI100/SPD7 bits are now set-only, never cleared — preserves firmware state
  and avoids documented FCH freeze condition.
- Added regression tests for alias absence, ownership gating, and speed safety.

### Phase 3 — Internal boundaries

- Extracted raw CapImg decoder, heatmap, blob tracker, and input publication
  into `driver/mshw0231-raw.c`. Core retains SPI, IRQ, V0 sequencing,
  activation timing, and watchdogs.
- Added `docs/AMDI0060_CONTRACT.md` — controller-specific invariants.
- Added `docs/PARAMETERS.md` — full release/diagnostic/experimental map.
- All raw pipeline and calibration controls are now load-time-only (`0444`).
- `raw_input_beta` defaults to `false`; `install.sh --raw` sets it explicitly.
- `raw_mode` defaults to `0` (standard HID), unlike v1.1.0 which defaulted to `1`.
  Standard profile provides single-touch only; opt-in to raw mode for multi-touch.
- Corrected misleading parameter descriptions (getfeat_delay_ms, touch_signal_mode,
  touch_threshold_pct, acpi_probe_power_cycle, grid geometry).

### Phase 4 — Replay and evidence

- Versioned replay corpus `tests/fixtures/replay/v1` with eight real V0 frames,
  SHA-256, provenance, three deterministic malformed inputs.
- Required `tests/replay_fixture_test.py` — no skip path.
- Decoder C test replays eight tracked raw-slot frames (41 assertions).
- Hardware evidence collection suite:
  - `tools/hardware_evidence/collect.sh` — dynamic ACPI/SPI/input discovery.
  - `tools/hardware_evidence/capture_direct_touch.sh` — bounded touch evtest.
  - `tools/hardware_evidence/capture_stylus.sh` — bounded stylus evtest.
  - `tools/hardware_evidence/capture_linux_trace_bundle.sh` — provenance,
    sysfs state, journal window, optional input capture, SHA-256 manifest.
  - `tools/hardware_evidence/run_blinded_session.sh` — opaque-session runner
    with role-separation worksheet and checksum verification.

### Phase 5 — CI

- `.github/workflows/ci.yml`: whitespace, host tests, sanitizers, out-of-tree
  Ubuntu generic-header build smoke test.
- `driver/Makefile` supports externally-set `KDIR` and `M`.
- Added `docs/ROLLBACK.md` with uninstall, upgrade, kernel-update, and
  Secure Boot / MOK enrollment procedures.

### Hardware evidence (standard HID)

Cold boot, warm boot, 30-minute stress, and suspend/resume observed
on CachyOS 7.1.3-2 with BIOS 4.391.140. Pen and raw multi-touch remain
unobserved/unqualified. See `docs/COMPATIBILITY.md` and
`docs/BLINDED_SESSION_LOG.md`.

### Installer and DKMS

- Default modprobe config: `/etc/modprobe.d/sl4a-spi-hid.conf`.
- Installer upgrades legacy artifacts to the opt-in controller format.
- Uninstaller verifies package ownership before touching config or DKMS tree.

---

## v1.2.0 (2026-07-19)

> [!WARNING]
> **Beta software.** This is an experimental, reverse-engineered kernel driver.
> It may cause touchscreen failure, system instability, or data loss.
> Use entirely at your own risk. No warranty of any kind.

### Windows pipeline alignment (~85%)

CCL flood-fill pipeline replacing the old 5×5 local centroid approach,
matching the `TouchPenProcessor0C19.dll` chain:

- **c590 LUT** — raw 16-bit → fixed-point signal conversion
- **Peak detection gate** — cross-shaped ±5 cells, min_rise=200, collects peaks for velocity rejection
- **CCL flood-fill** — 4-connected BFS component labeling on touched cells
- **Velocity rejection** — blob centroid must be within 6 cells of peak (dist² ≤ 36.0)
- **Edge penalty** — bottom edge ×0.23, other edges ×0.97 (DLL config+0x8D0/0x8D4)
- **Blob splitting** — multi-peak blobs (≥4 cells apart, pixel_count≥8) split by internal peak positions
- **Full-blob centroid** — signal-weighted ×100 fixed-point over blob extent
- **Eigenvalues** — second moments on blob bounding box → touch major/minor/orientation
- **Pre-merge** — ghost_dist=6 cells, keep strongest
- **Hungarian assignment** — multi-finger association radii from DLL config:
  1 finger ×2.2, 2×1.0, 3×2.8, 4×3.4, 5+×4.0
- **EMA + deadband + stationary lock** — alpha=7, ±0.8-cell deadband, 6-frame lock
- **Lift lookback** — 2-frame history position on lift

### DLL config table integration

20+ values extracted from `TouchPenProcessor0C19.dll` data segment
(VA 0x1808E0460, file offset 0x8DF060): association radii, edge weights,
coalescing threshold, noise floor, pre-assoc ratio, hold policy, touch
detection threshold. See `docs/CONFIG_TABLE.md`.

### Parameter tuning

- `min_rise` 300→200 for weak fingers at 3+ density
- `hold_frames=0` (disabled — caused scroll brake)
- `pre_assoc_ratio=0` (disabled — too aggressive without classifier)
- `ghost_dist=6` (matches DLL +0xC98=36.0)

### Documentation overhaul

- **GitHub Wiki** — 7-page technical wiki: protocol, pipeline, config table,
  hardware, build & install, reverse engineering
- **Source code** — per-field comments on `struct spi_hid`, kerneldoc for
  major functions, removed 10+ date-stamped developer diary entries
- **docs/** — 556 stale files deleted (diary, evidence, snapshot directories),
  7 technical docs rewritten, 4 new references created
- **README.md** — rewritten as project page with architecture diagram,
  feature table, module parameters, troubleshooting

### Repository cleanup

Removed 556 diary/research/evidence files including GROUND_TRUTH (3006-line
research journal), all GHIDRA snapshot directories, MSI database exports,
SESSION_HANDOFF, and experimental configs. Kept decomp/clean/, decomp/uefi/,
and acpi/ as technical reference.

Touch quality: 2-finger excellent, 3-finger good, 4-finger improved.

---

## v1.1.0 (2026-07-19)

### Multi-touch (active by default)

Raw multi-touch mode is **enabled by default** (`raw_mode=1`). The device
streams 72×48 heatmap frames processed through a pipeline matching the
Windows `TouchPenProcessor0C19.dll` chain:

- **Peak detection** — cross-shaped ±5-cell N/S/E/W scan, min rise 300
- **5×5 local centroid** — signal-weighted average with ×100 fixed-point
  sub-cell precision, eliminating visible grid stepping
- **Hungarian assignment** — minimum-cost bipartite matching with
  Windows-matching cost matrix; jump rejection prevents noise-blob
  slot theft for 3+ fingers
- **EMA + deadband + stationary lock** — alpha=7 smoothing, ±0.8-cell
  deadband, position frozen after 6 still frames to eliminate
  pinch-to-zoom jitter
- **Lift lookback** — emits lift at history position from 2 frames ago
- **Per-blob eigenvalues** — `ABS_MT_TOUCH_MAJOR/MINOR/ORIENTATION` for
  every active finger

Touch quality: 2-finger very good, 3-finger mostly stable (95%),
4-finger unstable.

### Standard HID (fallback)

Single-touch + pen (Report IDs 0x40 and 0x01) remain functional.
If the raw mode handshake fails after 3 attempts, the driver falls back
to standard HID automatically.

### 100% wire-read report descriptor

The 936-byte HID report descriptor is read live from the device every
boot. A historical PIO TX_COUNT bug (1 extra byte per 64-byte
continuation, corrupting bytes at offsets n·64+55) has been fixed by
matching the Windows `0x4bac` TX_COUNT=3 convention. The hardcoded
descriptor remains as an emergency fallback only.

### Auto-retry on cold boot

The first raw mode handshake attempt fails ~30% of the time after cold
boot. The driver now retries up to 2 additional times (5-second delay,
D2→D0 power cycle, fresh DESCREQ) before falling back to standard HID.

### Licensing

GNU General Public License v2.0. See [LICENSE](LICENSE).

### Installer

DKMS-based multi-distro installer (Arch/CachyOS, Ubuntu/Debian, Fedora,
openSUSE). Udev auto-loads the driver via `/etc/modprobe.d/spi-hid.conf`
(`raw_mode=Y skip_getfeat=Y`). No systemd service required.

### Repository cleanup

Removed 42+ stale files (calibration stubs, one-shot raw-mode scripts,
old test modprobe configs, build artifacts, disused analysis tools).
Added `.gitignore` coverage for all build artifact patterns.

---

## v1.0.0-beta1 (2026-07-08)

- Initial release: standard HID single-touch + pen working
- IRQ-driven HID-over-SPI V0 sequencer
- AMD FCH Cezanne SPI controller driver (V2)
- DKMS installer with multi-distro support
- Hardcoded report descriptor with ~98.5% wire-read + 14-byte patch
