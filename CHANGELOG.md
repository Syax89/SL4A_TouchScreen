# Changelog

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
