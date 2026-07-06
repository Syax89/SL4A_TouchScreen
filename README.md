# SL4A_TouchScreen — Linux Driver for Surface Laptop 4 AMD Touchscreen

Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen
(ACPI HID `MSHW0231`, SPI controller `AMDI0060`).

Windows supports this hardware via `amdspi.sys` + `hidspicx.sys` (drivers downloaded
through Windows Update, not included in the base OS image).
This project reverse-engineers the Windows driver protocol to implement a native
Linux kernel driver.

## Status

**Software exhausted — logic analyzer (or a live JTAG/WinDbg kernel debug rig on a
second machine) is required to go further.**

The SPI bus communication works: reads (opcode 0x0B) produce correct HID data
(verified bit-identical to Windows traces). The device ignores all write operations
(opcode 0x02) — DESCREQ, vendor init, all write transactions fail regardless of
SPI mode, speed, trigger method, or register configuration. This has been verified
on a genuinely pristine boot (no prior driver ever touched the device this boot —
see [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) §15.9), ruling out any
possibility of leftover/corrupted controller state from an earlier probe.

### PCI 0xB8 bit7 (16-bit FIFO mode) — fixes read layout, not the write block

The FCH LPC bridge (1022:790e, device 00:14.3) has a PCI config register at
offset 0xB8 that controls FIFO access mode:

```
# Linux default (8-bit FIFO — byte-interleaved reads)
setpci -s 00:14.3 B8.L
# → 0x33ED0004

# Windows value (16-bit FIFO — correct byte ordering)
setpci -s 00:14.3 B8.L=0x33ED0084
```

With bit7=1, the FIFO returns 16-bit words. The correct byte extraction formula:
```c
for (i = 0; i < rx_len; i++) {
    u16 w = readw(base + read_off + (i/2)*2);
    dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
}
```

This produces the expected HID response: `FF FF FF FF FF FF 32 10 00 5A`.

PCI offset 0xB4 (Windows: `0x7DFFE000`, Linux: `0x00000000`) controls FIFO data
layout and sync byte count. Both 0xB4 and 0xB8 are writable via setpci and were
retested with the correct full 32-bit values — no effect on the write path.

**Write operations (opcode 0x02) still fail** even with 16-bit FIFO mode, correct
PCI 0xB4/0xB8 values, and every other software knob tried (see the exhaustive test
matrix in `docs/GROUND_TRUTH.md` §3 and §15). The AMD FCH Cezanne SPI controller
under Linux cannot produce a valid write signal that the MSHW0231 touchscreen
recognizes. A logic analyzer on SCK/MOSI/MISO/CS is the only remaining path.

### Everything else ruled out (2026-07-06/07 session)

Beyond the original register/protocol test matrix, the following were investigated
and empirically excluded as the cause (all with real hardware tests, not just
theory — details in `docs/GROUND_TRUTH.md` §15):

- **AMD "SPI Lock" mechanism** (`RomProtect`, `SpiProtectEn0/Lock`, `SPIRestrictedCmd`/`2`)
  — the exact same mechanism `fwupd` uses to detect BIOS flash write-protection.
  All four registers are byte-identical between Windows and Linux on this machine.
- **Extended PCI config space** of Root Complex, IOMMU, SMBus, and all 8 Data Fabric
  functions — only one Data Fabric register differed, tested live, no effect.
- **`iommu=off`/`amd_iommu=off`** — no effect.
- **WPP-level driver tracing** of `hidspi.sys` (real Windows internals, not just
  MMIO snapshots) confirms opcode 0x02 writes genuinely succeed on Windows on this
  exact hardware — closing any doubt that the "Windows works" assumption itself
  might be wrong.
- **Protocol version**: confirmed (independently, by a linux-surface maintainer on
  the LKML thread proposing upstream `spi-hid`) that this device uses the legacy
  `SPI_HID_SUPPORTED_VERSION = 0x0100`, not the publicly documented v1.0
  (`0x0300`) spec — explains why the public Microsoft PDF spec doesn't fully match
  this device's behavior, but the difference is purely at the protocol/addressing
  level, not something that explains the write block itself.
- **A stray unsigned copy of an old build of this very driver baked into the boot
  initramfs**, auto-loading before any other software could touch the device —
  found and fixed (see §15.9), then re-tested on a provably virgin boot: same
  failure. Ruled out.

## Architecture

```
┌─────────────────────────────────────────────┐
│ spi-hid.ko (spi-hid-core.c)                 │
│  - HID-over-SPI protocol state machine      │
│  - Device discovery (DESCREQ/DESCREQ2)       │
│  - Firmware upload (B0 blocks)              │
│  - HID report forwarding                    │
└───────────────┬─────────────────────────────┘
                │ Linux SPI framework
┌───────────────┴─────────────────────────────┐
│ spi-amd.ko (spi-amd.c)                      │
│  - AMD FCH SPI controller V2                │
│  - FIFO management (70-byte FIFO at 0x80)   │
│  - Combined TX+RX transfers                 │
└───────────────┬─────────────────────────────┘
                │ MMIO
┌───────────────┴─────────────────────────────┐
│ AMD FCH SPI controller @ 0xFEC10000         │
│  - V2 path with registers 0x45-0x4B         │
│  - 16-bit FIFO access                       │
└─────────────────────────────────────────────┘
```

## Hardware

| Component | Detail |
|-----------|--------|
| Device | Surface Laptop 4 (AMD) |
| Touchscreen ACPI HID | `MSHW0231` (falls back to `PNP0C51`) |
| SPI Controller | `AMDI0060` (AMD FCH V2) |
| MMIO Base | `0xFEC10000` |
| FIFO Size | 70 bytes |
| GPIO IRQ | Pin `0x55` (da ACPI), Power=`0x5B`, Reset=`0x0103` |
| SPI Speed | 33.33 MHz, MODE 0 (CPOL=0, CPHA=0) |
| SPI Mode | MODE 0 (ClockPolarityLow, ClockPhaseFirst) |

## Key Source Files

| File | Contents |
|------|----------|
| `driver/spi-hid-core.c` | HID-over-SPI protocol implementation |
| `driver/spi-hid-core.h` | Protocol constants and data structures |
| `driver/spi-amd.c` | AMD SPI controller V2 driver |
| `driver/spi-amd.h` | SPI controller definitions |

## HID-over-SPI Protocol

The Microsoft HID-over-SPI protocol is documented in the public specification
(`docs/HidSpiProtocolSpec.pdf`). Key protocol elements:

- **Read Approval (0x0B)**: Read device data with register addressing
- **Output Write (0x02)**: Send commands (DESCREQ, DESCREQ2)
- **Activation (0x00, 0x70)**: Vendor-specific init commands
- **Firmware Upload (0xB0)**: 120 blocks × 241 bytes

### Device Initialization Sequence (from Windows CSV traces, 2384 transactions, ~88s total)

1. Device resets → sends `RESET_RSP` (type=3) via 0x0B read
2. After ~168 µs, device auto-advances → sends `ACK` (type=0)
3. Host sends `DESCREQ` (opcode 0x02) → device acknowledges inline; GPIO IRQ fires ~62µs later
4. Host does 0x0B read → receives `DEVICE_DESC` (type=7, Vendor=0x045E, Product=0x0C19)
5. Host parses device descriptor → sends `DESCREQ2` → receives `RPT_DESC` (type=8, 936 bytes)
6. Activation commands (`cmd1`/`cmd2`/`cmd3`)
7. Firmware upload (120 B0 blocks, 241 bytes each — for companion device, NOT touchscreen)
8. Second binary upload (0x22) for companion device
9. Runtime HID reports: 4304B full frames, 216B heatmap frames, alternating ~10 frames

## Building

```bash
# Build SPI controller driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$HOME/spi-amd-v2-multi modules

# Build HID driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$HOME/spi-hid/driver modules
```

## Loading

```bash
# Unload old modules
sudo rmmod spi_hid spi_amd 2>/dev/null

# Load new modules
sudo insmod ~/spi-amd-v2-multi/spi-amd.ko
sudo insmod ~/spi-hid/driver/spi-hid.ko

# Or install to DKMS
sudo cp ~/spi-amd-v2-multi/spi-amd.ko ~/spi-hid/driver/spi-hid.ko \
        /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo modprobe spi_hid
```

## Debugging

```bash
# Monitor driver activity
sudo dmesg -w | grep -E "SEQ|spi-amd"

# Trigger module reload
sudo rmmod spi_hid; sudo rmmod spi_amd
sudo insmod ~/spi-amd-v2-multi/spi-amd.ko
sudo insmod ~/spi-hid/driver/spi-hid.ko
```

## Fixes Applied

Verified against Windows decomp (`amdspi.sys`, `hidspi.sys`) and ETW CSV traces:

| Fix | Description |
|-----|-------------|
| C1 | ALT_CS encoding: use `AND 0xFC, OR 0x01` like Windows |
| C2 | Secret bits applied per-segment in `exec_segment` |
| C4 | `memcpy` offset `+4` in descriptor parse |
| C5 | `input_register` default `0x0000` (was `0x1000`) |
| C6 | `approval7 = 0x03` post-DESCREQ, `0x0A` at runtime |
| C7 | `approval8 = 0x00` always |
| FIFO | PCI 0xB8 bit7 16-bit mode: `readw` with byte extraction |
| HDR | Dynamic 0x5A search in header parser (3 or 6 sync bytes) |

## Documentation

- `docs/GROUND_TRUTH.md` — Cross-verified ground truth model (CSV × decomp × ACPI × PCI × tests × WPP tracing); the single source of truth, see especially §15 for the final exhaustive close-out
- `docs/SPI_REGISTERS.md` — AMD FCH SPI + PCI config register map, CTRL0 bits, V1/V2 paths
- `docs/DRIVER_STATE.md` — Current driver status, test matrix, PCI findings
- `docs/VERIFICATION_FINDINGS.md` — Independent sub-agent verification of SPI/HID/ACPI code, 8 critical bugs found and fixed
- `docs/VERIFICATION_PLAN.md` — Phased verification methodology used throughout the project
- `docs/AMDSPI_DECOMP.md` — `amdspi.sys` decompilation analysis
- `docs/HIDSPI_PROTOCOL.md` — HID-over-SPI protocol and state machine
- `docs/CSV_SEQUENCE.md` — Windows boot sequence from ETW traces
- `docs/SESSION_2026-07-06.md` — Session log of the final exhaustive debugging pass

## License

Dual GPL-2.0 / BSD-3-Clause. See source file headers.
