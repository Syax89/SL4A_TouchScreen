# Reverse Engineering

This driver was developed by reverse-engineering the Windows driver stack
for the Surface Laptop 4 (AMD). This page documents the methodology, tools,
and known gaps.

## Windows Driver Stack

The Windows touch pipeline consists of these components:

```
User mode
  SurfaceTouchPenProcessor.exe
  TouchPenProcessor0C19.dll     ← Core touch pipeline (config table, CCL, tracking)
    │
Kernel mode
  SurfaceSystemTelemetryDriver.sys  ← Device firmware communication
  hidspi.sys (PDB: b00863b3...)     ← HID over SPI protocol
  HidSpiCx.sys                      ← Class extension framework
    │
  amdspi.sys                        ← AMD FCH SPI controller driver
```

## Driver Components Decompiled

| Component | Status | Documentation |
|-----------|--------|---------------|
| `amdspi.sys` | Fully decompiled | `docs/decomp/clean/0x4bac.txt` (read), `0x3c20_transfer.txt` (transfer), `0x54d0_submit_handler.txt` (write) |
| `hidspi.sys` | Protocol layer decompiled | `docs/decomp/clean/hidspi_*.txt` (V0 protocol, descriptors) |
| `HidSpiCx.sys` | Orchestrator decompiled | `docs/decomp/clean/hidspicx_*.txt` (descriptor handling, reset) |
| `TouchPenProcessor0C19.dll` | Partially decompiled | `docs/decomp/SURFACE_TRACKER_DECOMP.md`, `docs/decomp/MULTITOUCH_STATIC_DECOMP.md` |
| UEFI DXE drivers | Fully decompiled | `docs/decomp/uefi/` (7 files: AmdSpiHcProtocolDxe, SurfaceTouchHidDxe, et al.) |

## Methodology

### 1. Windows Trace Capture

- **RWEverything**: Dumped MMIO register state and PCI config space on Windows boot
- **ETW (Event Tracing for Windows)**: Captured HID-over-SPI bus traces via
  `Microsoft-Windows-HIDSPI` provider at 0xFFFF verbosity level
- **MSI analysis**: Extracted driver MSI package to find binaries and INF files
  (`docs/msi-tables/`)

### 2. Binary Analysis

- **Ghidra**: Primary tool for decompiling Windows drivers and UEFI modules:
  - `amdspi.sys` (no PDB): Analyzed via function boundary recovery and register
    pattern matching against known AMD FCH SPI controller documentation
  - `hidspi.sys` (PDB available): Full symbol recovery via
    `b00863b38d904fcbbd2bf2db07f88cff2` PDB
  - `HidSpiCx.sys`: Class extension with full PDB symbols
  - `TouchPenProcessor0C19.dll`: Targeted decomp of specific pipeline functions
  - UEFI DXE drivers (7 modules): Full decomp with register definitions

- **Ghidra Scripts**: Custom scripts exported cross-references and consumer
  chains for the CCL pipeline functions (`decomp/touchpen0c19_tables_20260716/`)

### 3. Config Table Extraction

The `TouchPenProcessor0C19.dll` contains a static configuration table at
virtual address `0x1808E0460` (file offset `0x8DF060`, section `.rdata`).
This table was extracted by:

1. Identifying the table base address via cross-reference to the pipeline
   initialization function (FUN_180600c40)
2. Reading float32 values at known offsets (+0x8DC through +0x8EC for
   association radii, +0x8D0/+0x8D4 for edge weights, etc.)
3. Validating values against ETW CSV traces showing actual association
   behavior

### 4. Protocol Validation

The HID-over-SPI V0 protocol was validated against:
- Windows EHCI trace dump (`surface_init.csv`, `surface_touch.csv`)
- UEFI DXE drivers showing register-level interactions
- The Microsoft HIDSPI specification (not public, but inferred from
  multi-vendor implementations)

### 5. Pipeline Validation

The CCL pipeline was validated by comparing kernel driver output against:
- ETW CSV traces showing Windows blob detection and tracking
- Python oracle (`tools/surface_tracker.py`) implementing the Windows
  algorithm on captured raw frames
- Mathematical validation of eigenvalue formulas

## Known Gaps

### Mahalanobis Contact Classifier (FUN_180601690)

The Windows driver classifies each CCL blob as `finger`, `palm`, or `noise`
using a 10×11 Mahalanobis distance matrix. This matrix is populated at
runtime from device firmware via `SurfaceSystemTelemetryDriver.sys` and
is not present in the static DLL binary.

**Impact**: Without classification, all blobs are treated as potential
finger contacts. Noise rejection relies on peak gate + velocity rejection
instead.

### Per-Cycle Gain Adaptation (FUN_180600820)

The Windows pipeline adjusts per-cell gain values after each frame cycle
based on noise floor measurements and signal distribution. These gains
are runtime-populated via the same telemetry channel.

**Impact**: The Linux driver uses static c590 LUT values and a fixed
noise floor threshold. Accuracy may degrade over long sessions.

### Hold/Lift Policy (FUN_180606370)

The Windows hold policy is a complex state machine that evaluates track
history quality (duration, pixel count, eigenratio, signal level).
Without this, the Linux driver uses simple frame-count-based lift.

**Impact**: Temporary contact loss during fast movement may be handled
less gracefully than on Windows.

## References

- `docs/decomp/` — Complete decompilation notes and function analyses
- `docs/HIDSPI_PROTOCOL.md` — HID-over-SPI V0 protocol specification
- `docs/CSV_SEQUENCE.md` — ETW CSV trace format
- `docs/SPI_REGISTERS.md` — AMD FCH SPI controller register map
- `docs/decomp/clean/` — Cleaned decompilation outputs (42 files)
- `docs/decomp/uefi/` — UEFI DXE driver decompilation (7 files)
- `docs/acpi/` — DSDT and SSDT ACPI tables (ASL source)
