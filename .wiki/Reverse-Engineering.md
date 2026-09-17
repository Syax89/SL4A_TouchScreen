# Reverse Engineering

This driver was developed by reverse-engineering the Windows driver stack
for the Surface Laptop 4 (AMD) touchscreen. This page documents the
methodology, tools, evidence and known gaps. The SL3 AMD panel (`MSHW0162`)
was added later from the same protocol with geometry values contributed and
tested on real hardware.

## Windows driver stack

```
User mode
  SurfaceTouchPenProcessor.exe
  TouchPenProcessor0C19.dll     ← Core touch pipeline (config table, CCL, tracking)
    │
Kernel mode
  SurfaceSystemTelemetryDriver.sys  ← Device firmware communication
  hidspi.sys (PDB: b00863b3...)     ← HID-over-SPI protocol
  HidSpiCx.sys                      ← Class extension framework
    │
  amdspi.sys                        ← AMD FCH SPI controller driver
```

## What was decompiled

| Component | Status | Evidence in repo |
|---|---|---|
| `amdspi.sys` | Fully decompiled | read/transfer/write handlers documented in `docs/SPI_REGISTERS.md`, `docs/AMDI0060_CONTRACT.md` |
| `hidspi.sys` | Protocol layer decompiled | V0 protocol, descriptors — summarized in `docs/HIDSPI_PROTOCOL.md` |
| `HidSpiCx.sys` | Orchestrator decompiled | descriptor handling, reset |
| `TouchPenProcessor0C19.dll` | Partially decompiled | `docs/decomp/SURFACE_TRACKER_DECOMP.md`, `docs/decomp/MULTITOUCH_STATIC_DECOMP.md`, `docs/decomp/HEATCORE_REPORT_TABLE.md` |
| UEFI DXE drivers | Fully decompiled | `AmdSpiHcProtocolDxe`, `SurfaceTouchHidDxe` and more (Ghidra projects/scripts in `tools/ghidra/`) |

## Methodology

### 1. Windows trace capture

- **RWEverything**: dumped MMIO register state and PCI config space on
  Windows boot
- **ETW**: captured HID-over-SPI bus traces via the
  `Microsoft-Windows-HIDSPI` provider at 0xFFFF verbosity; CSV conversion
  documented in `docs/ETW_CSV_FORMAT.md`
- **MSI analysis**: extracted driver packages to find binaries and INF files
  (`tools/windows_capture/`)

### 2. Binary analysis

- **Ghidra** — primary decompiler (`tools/ghidra/` contains the scripts:
  `DecompileAll.java`, `DecompileByAddress.java`, `DecompileByKeyword.java`,
  `DumpPointerTable.java`, …):
  - `amdspi.sys` (no PDB): function-boundary recovery + register-pattern
    matching against the AMD FCH SPI controller documentation
  - `hidspi.sys` (PDB available): full symbol recovery
  - `HidSpiCx.sys`: class extension with full PDB symbols
  - `TouchPenProcessor0C19.dll`: targeted decomp of the pipeline functions
  - UEFI DXE drivers: full decomp with register definitions

### 3. Config table extraction

The `TouchPenProcessor0C19.dll` static configuration table lives at virtual
address `0x1808E0460` (file offset `0x8DF060`, `.data`). Extraction:

1. locate the table base via cross-reference to the pipeline init function
   (FUN_180600c40),
2. read float32 values at known offsets (+0x8DC…+0x8EC association radii,
   +0x8D0/+0x8D4 edge weights, …),
3. validate against ETW CSV traces showing actual association behavior.

See [Config Table](Config-Table) for the values and their Linux mapping.

### 4. Protocol validation

The V0 protocol was validated against:

- Windows trace dumps (`surface_init.csv`, `surface_touch.csv`),
- UEFI DXE drivers showing register-level interactions,
- the Microsoft HIDSPI specification (inferred from multi-vendor
  implementations).

### 5. Pipeline validation

The CCL pipeline was validated by comparing driver output against:

- ETW CSV traces of Windows blob detection and tracking,
- a Python oracle (`tools/surface_tracker.py`) implementing the Windows
  algorithm on captured raw frames,
- mathematical validation of eigenvalue formulas,
- host replay tests: `raw_pipeline_replay_test` runs synthetic 1–5-finger
  heatmap fixtures through the real pipeline code (`tests/fixtures/raw-replay/`).

## Known gaps

### Detector input plane

The per-frame detector reads a byte plane whose row stride is 288 bytes while
the image occupies only the first 72 bytes of each row — the plane is filled by
the transport side and its pre-processing (accumulation, baseline handling) is
not established. Scoring the raw CapImg raster with the detector's kernel and
peak threshold does not reproduce the published contacts.

**Impact**: the Linux peak criterion is the driver's own (rise threshold, noise
floor, weight gate); it cannot be aligned with the reference until the plane's
contents are understood.

### Per-frame list provenance

The frame carries two lists the detector consumes: a blob list (count plus
4-byte `{x, y}` records) and a 16-slot fixed-point list (count plus u16
entries). No writer for either exists in the DLL, and their values are not
found in the raw device payload region.

**Impact**: the Linux pipeline detects its own peaks and blobs; the reference
candidate source cannot be replicated directly.

### Centroid refinement (5×5)

The published peak position is refined by a 5×5 centroid step whose moment
accumulators and clamping are not fully modelled.

**Impact**: reference positions can be compared only to the nearest cell.

### Mahalanobis contact classifier (FUN_180601690)

Windows classifies each blob as `finger` / `palm` / `noise` with a 10×11
Mahalanobis matrix populated **at runtime** from device firmware via
`SurfaceSystemTelemetryDriver.sys` — absent from the static DLL.

**Impact**: the Linux driver treats all blobs as potential contacts; noise
rejection relies on the peak gate + velocity rejection instead.

### Per-cycle gain adaptation (FUN_180600820)

Windows adjusts per-cell gain each frame from noise-floor measurements and
signal distribution, populated via the same telemetry channel.

**Impact**: the Linux driver uses the static c590 LUT and a fixed noise
floor; accuracy may degrade over long sessions.

### Hold/lift policy (FUN_180606370)

Windows hold is a quality-gated state machine (duration, pixel count,
eigenratio, signal level). The Linux driver uses frame-count-based lift
(`blob_lift_frames`) with hold disabled by default.

**Impact**: temporary contact loss during fast movement may be handled less
gracefully than on Windows.

## Evidence and references

- `docs/decomp/` — decompilation notes (tracker, multitouch static, heatcore)
- `docs/HIDSPI_PROTOCOL.md` — HID-over-SPI V0 protocol reference
- `docs/ETW_CSV_FORMAT.md` — ETW CSV trace format
- `docs/SPI_REGISTERS.md` — AMD FCH SPI controller register map
- `docs/AMDI0060_CONTRACT.md` — SPI controller contract
- `docs/acpi/` — DSDT/SSDT ACPI tables (`dsdt.dsl`, `ssdt*.dsl`)
- `tools/ghidra/` — decompilation scripts
- `tools/windows_capture/` — Windows-side capture tooling
- `docs/EVIDENCE.md` — chronological investigation record
