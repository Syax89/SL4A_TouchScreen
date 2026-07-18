# Changelog

## v1.1.0 (2026-07-19)

> [!WARNING]
> **Beta software.** This is an experimental, reverse-engineered kernel driver.
> It may cause touchscreen failure, system instability, or data loss.
> Use entirely at your own risk. No warranty of any kind.

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
