# SL4A_TouchScreen

> Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen

[![Status](https://img.shields.io/badge/status-software%20exhausted-red)](https://github.com/Syax89/SL4A_TouchScreen)
[![Hardware](https://img.shields.io/badge/next%20step-logic%20analyzer-blue)](#next-step)
[![License](https://img.shields.io/badge/license-GPL--2.0%20%7C%20BSD--3-orange)](LICENSE)

---

## What is this?

A **reverse-engineered Linux kernel driver** for the `MSHW0231` touchscreen controller used in the Surface Laptop 4 (AMD). Windows ships with proprietary drivers (`amdspi.sys` + `hidspi.sys`) downloaded via Windows Update. This project reverse-engineers their behavior to produce a native Linux driver.

---

## Status

**Reads work.** Writes don't. Every software avenue has been exhausted.

| Path | Status |
|------|--------|
| Reads (opcode `0x0B`) | Working — bit-identical to Windows traces |
| Writes (opcode `0x02`) | **Blocked** — device ignores all writes regardless of config |

The prime suspect is **CTRL0 bits [15:8]**: Windows = `0x0E`, Linux = `0xA9` (immutable from software). These bits control chip-select timing — a wrong value could invalidate the electrical framing of writes. A **logic analyzer** on SCK/MOSI/MISO/CS is the only remaining path forward.

Full details: [`docs/DRIVER_STATE.md`](docs/DRIVER_STATE.md) and [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) §15.

---

## Hardware

| Component | Detail |
|-----------|--------|
| Device | Surface Laptop 4 (AMD) |
| Touchscreen ACPI HID | `MSHW0231` (falls back to `PNP0C51`) |
| SPI Controller | `AMDI0060` (AMD FCH Cezanne) |
| MMIO Base | `0xFEC10000` |
| FIFO | 70 bytes at `0x80` |
| GPIO IRQ | Pin `0x55` (DSDT), Power=`0x5B`, Reset=`0x0103` |
| SPI Mode | MODE 0 (CPOL=0, CPHA=0) at 33.33 MHz |
| Protocol | HID-over-SPI **V0** (pre-release, "version 2" wire format) |

**Key finding**: the system has **two SPB devices**:

| Device | Connection | Opcodes | Purpose |
|--------|-----------|---------|---------|
| **A** — Touchscreen | `0x0B` | `0x0B`, `0x02` | HID input reports, descriptor discovery |
| **B** — Companion chip | `0x18`, `0x19`, `0x1A` | `0x00`, `0x70`, `0xB0`, `0xB1`, `0x22`, ... | Activation, firmware upload |

The touchscreen receives **only** reads (`0x0B`) and writes (`0x02`) — no firmware, no vendor activation.

---

## Architecture

```
┌──────────────────────────────────────┐
│  spi-hid.ko (driver/spi-hid-core.c)  │
│  HID-over-SPI protocol state machine │
│  · Device discovery (DESCREQ/2)      │
│  · Reset Response drain              │
│  · Runtime HID input reports (type 1)│
└──────────────┬───────────────────────┘
               │ Linux SPI framework
┌──────────────┴───────────────────────┐
│  spi-amd.ko (driver/spi-amd.c)       │
│  AMD FCH SPI controller driver (V2)  │
│  · TX-only writes + GPIO-gated reads │
│  · Secret bits (CTRL0: 30,29,18)     │
│  · 16-bit FIFO mode (PCI 0xB8 bit7)  │
└──────────────┬───────────────────────┘
               │ MMIO
┌──────────────┴───────────────────────┐
│  AMD FCH SPI Controller @ 0xFEC10000 │
└──────────────────────────────────────┘
```

---

## Protocol

The device uses the **HidSpiDeviceV0** path (pre-release protocol), not the public v1.0 spec.

### Wire format (V0, "version 2")

```
Header: [TYPE:4|VERSION:4] [u16 LE] [0x5A]
        version = 2, low nibble of u16 must be 0
Body:   [len16 LE = content+3] [ContentID] [content...]
```

### Report types

| Type | Name | When |
|------|------|------|
| `1` | DATA | Runtime input reports |
| `3` | RESET_RSP | Device reset signal |
| `7` | DEVICE_DESC | 28-byte device descriptor |
| `8` | RPT_DESC | 936-byte HID report descriptor |

### Touchscreen init sequence (from Windows ETW traces)

```
GPIO IRQ → 0x0B read 9B → RESET_RSP header (32 10 00 5A)
         → 0x0B read 9B → RESET_RSP body   (03 00 00 00)   ← same report, not "ACK"
         → 0x02 write 10B → DESCREQ (register 0x000001)      ← TX-only
         → GPIO IRQ (~58 µs)
         → 0x0B read 9B → DEVICE_DESC header (72 80 00 5A)
         → 0x0B read 37B → device descriptor body
            VID=0x045E, PID=0x0C19, Ver=0x0100
         → 0x02 write 10B → DESCREQ2 (register 0x000002)     ← TX-only
         → GPIO IRQ (~727 µs)
         → 0x0B read 9B → RPT_DESC header (82 B0 0E 5A)
         → 0x0B read 945B → 936-byte HID report descriptor
         ~ runtime HID input reports (type=1) ~
```

---

## Building

```bash
# SPI controller driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver modules

# Or for HID-only:
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/driver -f Makefile.hid modules
```

### Load

```bash
sudo rmmod spi_hid spi_amd 2>/dev/null
sudo insmod driver/spi-amd.ko
sudo insmod driver/spi-hid.ko
```

### Debug

```bash
sudo dmesg -w | grep -E "SEQ|spi-amd|spi-hid"
```

---

## PCI Configuration

The FCH LPC bridge (`1022:790e`, device `00:14.3`) needs two registers set to match Windows:

```bash
# 16-bit FIFO access mode (fixes byte ordering on reads)
sudo setpci -s 00:14.3 B8.L=0x33ED0084

# FIFO data layout
sudo setpci -s 00:14.3 B4.L=0x7DFFE000
```

With `0xB8` bit7=1, data must be read as 16-bit words:
```c
for (i = 0; i < rx_len; i++) {
    u16 w = readw(base + read_off + (i/2)*2);
    dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
}
```

---

## What's been ruled out (exhaustive)

| Category | Tests |
|----------|-------|
| SPI modes | 0, 1, 2, 3 |
| Speeds | 800 KHz through 33 MHz |
| Triggers | V1 (CTRL0 bit16), V2 (0x47 bit7), hardcoded 0x80 |
| PCI config | 0xB4, 0xB8 (full 32-bit values), extended config space of 11 devices |
| MMIO | CTRL1, 0x44 dance, TXMODE, CS selection, opcode prepend, FIFO variants |
| ACPI | Full power cycle (_PS3→_PS0→_RST), GPIO M009/M010 toggle |
| Protocol | Vendor init @0x04, synchronous/async DESCREQ, IRQ timing |
| Kernel | `iommu=off`, `amd_iommu=off`, SME, clean boot (no stale module) |
| PCI lock | RomProtect, SpiProtectEn, SPIRestrictedCmd — identical Windows/Linux |
| WPP tracing | Confirmed: Windows `hidspi.sys` succeeds with opcode 0x02 on this hardware |

Full test matrix: [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) §3 and §15.

---

## Tools

| Tool | Path | Description |
|------|------|-------------|
| `parse_spi.py` | `tools/parse_spi.py` | Full ETW CSV parser (transactions, timing, GPIO) |
| `parse_spb_csv.py` | `tools/parse_spb_csv.py` | SPB payload extraction |
| `decode_hidspi.py` | `tools/decode_hidspi.py` | HID-over-SPI protocol decoder |
| `mmio_write.c` | `tools/diagnostics/mmio_write.c` | Raw MMIO test module |
| `gpio_test.c` | `tools/gpio_test.c` | GPIO power/reset via ACPI |
| `test_coldboot.sh` | `tools/test_coldboot.sh` | Virgin-boot test harness |

---

## Documentation

| File | Contents |
|------|----------|
| [`docs/GROUND_TRUTH.md`](docs/GROUND_TRUTH.md) | **Single source of truth** — cross-verified model (CSV × decomp × ACPI × PCI × tests) |
| [`docs/DRIVER_STATE.md`](docs/DRIVER_STATE.md) | Status, test matrix, PCI findings, next steps |
| [`docs/SPI_REGISTERS.md`](docs/SPI_REGISTERS.md) | AMD FCH SPI + PCI config register map |
| [`docs/HIDSPI_PROTOCOL.md`](docs/HIDSPI_PROTOCOL.md) | HID-over-SPI V0 protocol and touchscreen behavior |
| [`docs/CSV_SEQUENCE.md`](docs/CSV_SEQUENCE.md) | Annotated Windows boot SPI trace (2384 transactions) |
| [`docs/AMDSPI_DECOMP.md`](docs/AMDSPI_DECOMP.md) | `amdspi.sys` decompilation with function index |
| [`docs/VERIFICATION_FINDINGS.md`](docs/VERIFICATION_FINDINGS.md) | Independent code audit — 8 bugs found and fixed |
| [`docs/VERIFICATION_PLAN.md`](docs/VERIFICATION_PLAN.md) | Phased verification methodology |
| [`docs/SESSION_2026-07-06.md`](docs/SESSION_2026-07-06.md) | Final debugging session log |
| [`docs/analisi_MSHW0231.md`](docs/analisi_MSHW0231.md) | Touchscreen register/feature analysis |
| [`docs/decomp/amdspi/DECOMP-INDEX.md`](docs/decomp/amdspi/DECOMP-INDEX.md) | Authoritative function index for amdspi.sys |

### Verification reports

| File | Verdict |
|------|---------|
| [`docs/verification/amdspi-decomp-report.md`](docs/verification/amdspi-decomp-report.md) | SPI controller register-level verification |
| [`docs/verification/protocol-verification-report.md`](docs/verification/protocol-verification-report.md) | HID protocol verification (spec + decomp + CSV) |
| [`docs/verification/csv-verification-report.md`](docs/verification/csv-verification-report.md) | CSV trace ground-truth verification |

---

## Next step

**Logic analyzer** on SCK/MOSI/MISO/CS between Windows and Linux.

Without a physical measurement of the SPI bus electrical signals, no software fix is possible. The CTRL0[15:8] timing bits are the prime suspect; a logic analyzer will confirm whether the write framing is electrically valid on Linux.

---

## License

Dual **GPL-2.0** / **BSD-3-Clause**. See source file headers.

