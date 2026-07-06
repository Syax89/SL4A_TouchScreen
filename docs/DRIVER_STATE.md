# DRIVER STATE — 2026-07-06 (FINAL + software avenues closed out)

> **Repository**: https://github.com/Syax89/SL4A_TouchScreen
> **Source of truth**: [GROUND_TRUTH.md](GROUND_TRUTH.md)

---

## Overall status: SOFTWARE EXHAUSTED

After exhaustive analysis of **every** software component (Windows driver decompilation,
ETW CSV traces, MMIO and PCI dumps, GPIO/ACPI tests, register-by-register comparison),
**every software option has been exhausted**.

---

## Read path

**WORKING** — RESET_RSP identical to Windows (`32 10 00 5A` + `03 00 00 00`), bit-identical.
Every read (opcode 0x0B) works correctly. The device responds with valid HID data.

---

## Write path

**EXHAUSTED** — the device ignores **every write** (opcode 0x02) to every register.
The AMD FCH Cezanne controller under Linux does not produce a write signal that the
MSHW0231 touchscreen recognizes.

### Failed tests (complete matrix)
- SPI modes 0, 1, 2, 3
- Speeds 800 KHz - 33 MHz
- Trigger V1 (CTRL0 bit16)
- Trigger V2 (0x47 bit7)
- Hardcoded trigger V2 0x80
- CTRL1 write, Windows value 0x020006B5 (read-only register, ignored)
- 0x44 dance with masks 0xF0FF, 0x0FFF
- TXMODE CTRL0 bit23
- Hardcoded CS1
- Opcode prepended to the FIFO
- Full power cycle: _PS3→_PS0→_RST via ACPI
- GPIO power cycle via M010 — **DESTRUCTIVE (kills device until reboot, 2026-07-07)**
- Vendor init @0x04 (14-byte cold boot)
- Synchronous DESCREQ, IRQ, and workqueue
- PCI 0xB8 bit7 (16-bit FIFO mode — fixes the read data layout, NOT the write)
- **Every combination of the categories above**

---

## PCI Configuration Discovery (CRITICAL)

Dump of the FCH LPC bridge (1022:790e, device 00:14.3) from Windows:

### PCI 0xB4 — FIFO Data Layout
| | Value | Description |
|--|-------|-------------|
| Windows | 0x7DFFE000 | FIFO data layout / sync byte count (**corrected on the night of 2026-07-06**, was previously mis-transcribed as 0x007DFFE0) |
| Linux | 0x00000000 | BIOS default |
| Writable | YES | Via setpci — retested with the correct value: no effect on writes |

### PCI 0xB8 — 16-bit FIFO Access Mode
| | Value | Description |
|--|-------|-------------|
| Windows | 0x33ED0084 | bit7=1: 16-bit FIFO access |
| Linux | 0x33ED0004 | bit7=0: 8-bit FIFO access (default) |
| Writable | YES | Via `setpci -s 00:14.3 B8.L=0x33ED0084` (full value — a partial write like `B8.L=0x0084` clobbers the whole dword instead of only setting bit7) |

Setting 0xB8 to the Windows value enables 16-bit FIFO mode. The data read out
is then correct using the word-extraction formula (readw + odd/even byte extraction).
**However, writes (opcode 0x02) still fail.**

---

## CTRL0 bits[15:8] — Prime Suspect

- **Windows** = 0x0E
- **Linux** = 0xA9 (hardwired, **immutable from software** — every writel is ignored by the controller)

These bits control chip-select timing parameters. A wrong value could
invalidate the electrical-level framing of writes.

---

## CTRL1 — Read-only

- **Windows** = 0x020006B5
- **Linux** = 0x02000000
- The difference in the low bits (0x06B5 vs 0x0000) can't be reproduced: CTRL1 is read-only.

---

## FIFO diagnostics

The `mmio_write.c` module confirms:
- Write: the TX data is correct in the FIFO, but MISO stays **all zeros** (the device doesn't drive the line)
- Read: MISO contains valid HID data (sync bytes + header + body) — with PCI 0xB8 bit7=1, the layout is correct
- The device **selectively ignores** writes, not reads

---

## ACPI verification

- DSDT md5sum identical between Windows/Linux: `78046fa74c0282ee59db8b04a5204d88`
- The ACPI tables are **bit-identical** — no hardware configuration difference
- SPI mode: MODE 0 (ClockPolarityLow, ClockPhaseFirst) at 33.33 MHz

---

## Session 2026-07-06 evening/night — closing out the remaining software avenues

Without a logic analyzer or a second PC (no breakpoint-based WinDbg), every remaining
plausible software idea was tested and closed out:

- **WPP tracing of hidspi.sys** (tracepdb+traceview+tracefmt): captured a real write
  (`HidSetFeature`, 14B) completing with STATUS_SUCCESS, confirmed from inside the
  driver itself — independent confirmation that opcode 0x02 works on
  Windows. Cross-checked against "Vendor init @0x04" (already in the FAILED test
  matrix on Linux, same register): the block remains uniform Windows-vs-Linux, not
  specific to a single register.
- **Extended PCI config space**: compared the Root Complex, IOMMU, SMBus, and all 8
  Data Fabric functions in addition to the LPC bridge. Only Data Fabric Fn4 (00:18.4)
  showed real differences (0x5C, 0x98, 0x9C) — tested on real hardware: 0x98/0x9C are
  read-only, 0x5C is writable but has **no effect** on the write path once applied.
- **PCI 0xB4**: fixed a transcription bug (real value 0x7DFFE000, not
  0x007DFFE0) and retested — no effect, this avenue is genuinely closed this time.
- Already ruled out earlier: SMN/PCI-config access inside the Windows driver (decomp), _OSI
  ACPI gating, WREN/SPI-NOR heritage, kernel lockdown, SME, IOMMU (likely
  irrelevant).

**No plausible software avenue remains to be tested.**

---

## MANDATORY next step

**Logic analyzer** on the SPI bus — compare SCK/MOSI/MISO/CS signals between Windows and Linux.
Without this physical measurement, **no software fix is possible**.

---

## Diagnostic Tools

| Tool | Path | Description |
|------|------|-------------|
| `parse_spi.py` | `tools/parse_spi.py` | Full ETW CSV parser (transactions, timing, IRQ, init) |
| `mmio_write.c` | `tools/diagnostics/mmio_write.c` | Raw MMIO test: Windows-exact write + FIFO dump |
| `gpio_test.c` | `tools/gpio_test.c` | GPIO M009/M010 test via ACPI evaluate |
| PCI dump | Windows-side, `Desktop\windrivers\` | PCI config space (bridge 1022:790e) and 11-device comparison |

---

## Modules

Built from `driver/` (single source of truth — `spi-amd.c` and `spi-hid-core.c` live and
build together from this one directory; there is no longer a separate working copy).

---

## Key Files

| File | Contents |
|------|-----------|
| `driver/spi-hid-core.c` | WINSEQ + GPIO in probe |
| `driver/spi-amd.c` | Pre-trigger CTRL0 fix |
| `tools/diagnostics/mmio_write.c` | Raw MMIO test module |
| `tools/parse_spi.py` | ETW CSV parser |
| `traces/surface_*.csv` | Windows ETW traces |
| `docs/decomp/` | Windows driver decompilation |
| `docs/windows_mmio_dumps/` | Windows MMIO dump (RWEverything) |
| Windows-side, `Desktop\windrivers\` | PCI config space dump (LPC bridge + 11-device comparison), hidspi.pdb/.sys, WPP TMF files, decoded logs |
