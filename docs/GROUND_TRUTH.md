# GROUND TRUTH — Model verified by cross-referencing ETW CSV × amdspi decomp × hidspi V0 decomp × spec PDF × HW tests

> **Last updated: 2026-07-06 evening/night (final + software avenues closed out).**
> Sources: hidspi.sys decomp (PDB), amdspi.sys (no PDB, objdump), ETW CSV,
> DSDT/SSDT ACPI, Linux tests on a Surface Laptop 4 AMD (Cezanne), kernel 7.1.2-3-cachyos,
> Windows MMIO dump via RWEverything, Windows PCI config space dump via RWEverything
> (11 devices), WPP tracing internal to hidspi.sys (tracepdb/traceview/tracefmt).

---

## 0. PCI Configuration Discovery (CRITICAL — from the Windows dump LPC_00_14_3.BIN)

The FCH LPC bridge (1022:790e, device 00:14.3) has differences in PCI configuration
space between Windows and Linux that affect the SPI controller:

| PCI Offset | Windows Value | Linux Value | Effect |
|-----------|---------------|-------------|--------|
| 0xB4 | 0x7DFFE000 | 0x00000000 | FIFO data layout / sync byte count |
| 0xB8 | 0x33ED0084 | 0x33ED0004 | bit7 (0x80) = 16-bit FIFO access mode |

Both registers are **WRITABLE** via setpci on Linux. Setting 0xB8 to the
Windows value enables **16-bit FIFO mode**, which fixes the read data
layout problem (see section 0.1).

---

## 0.1. 16-bit FIFO Read Formula

With PCI 0xB8 bit7=1, the FIFO returns data as 16-bit words. The correct
byte extraction is:

```c
for (i = 0; i < rx_len; i++) {
    u16 w = readw(base + read_off + (i/2)*2);
    dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
}
```

This produces the correct byte ordering: `FF FF FF FF FF FF 32 10 00 5A`
(matching the expected HID response). Without this formula, `readb` produces
interleaved high/low bytes: `FF 10 FF 5A FF 00...`.

---

## 0.2. Header Parser Fix

Modified `spi_hid_seq_hdr_type()` to search for 0x5A dynamically instead of at a
fixed offset. This supports both the 3-sync-byte and 6-sync-byte layouts.

---

## 0.3. ACPI Verification: Tables IDENTICAL Between Windows/Linux

The DSDT dumped from Windows and from Linux has the same md5sum: `78046fa74c0282ee59db8b04a5204d88`.
The ACPI tables (DSDT, SSDT1-7) are **bit-identical**. There is no hardware configuration
difference between the two OSes — the BIOS exposes exactly the same resources, GPIOs, MMIO addresses, and _DSM.

SPI mode confirmed: **MODE 0** (ClockPolarityLow, ClockPhaseFirst) at 33.33 MHz.
GPIO: POWER=0x5B, RESET=0x0103, IRQ=0x55.

**Conclusion**: the problem is NOT in the ACPI tables. Any behavioral difference is at the
driver/controller level, or in the LPC bridge's PCI configuration.

---

## 1. Windows Driver Architecture

```
┌─────────────────────────────────────────┐
│ hidspi.sys (143KB) — HID protocol layer │
│ Contains ALL the V0 MMIO code           │
│ ✅ DECOMPILED (with PDB)                │
│ Functions: 0x3c20 (transfer), 0x4bac,   │
│ 0x54d0 (submit_handler), 0x3528 (sub)   │
│ V0 path: DIRECT MMIO, no SpbCx          │
└────────────┬────────────────────────────┘
             │ (V0 path: direct MMIO)
             ▼
┌─────────────────────────────────────────┐
│ amdspi.sys (232KB) — AMD FCH SPI driver │
│ Contains the same MMIO code + WDF init  │
│ ❌ NEVER DECOMPILED (no PDB)            │
│ BUT: init = pure WDF boilerplate        │
│ NO SPI register touched during init     │
│ Only used for the V1/SpbCx path         │
└────────────┬────────────────────────────┘
             ▼
    AMD FCH SPI Controller HW
         (MMIO 0xFEC10000)
```

The **MSHW0231** device uses the **HidSpiDeviceV0** (pre-release) path.
The decomp we have is from the correct driver. amdspi.sys has WDF init code
that never touches SPI registers — verified via objdump: no `add rax, 0x1D`
or similar in the INIT/PAGE sections.

---

## 2. Exact Windows Write Path Sequence (0x54d0/0x4bac) — MMIO Trace

```
1.  save 0x22:       readw(base+0x22), store context
2.  ALT_CS:          read8(0x1D) → &0xFC → |0x01 → write8(0x1D)
3.  FIFO clear:      read32(CTRL0) → |= BIT20 → write32(CTRL0)   [single-set, no toggle]
4.  opcode #1:       write8(0x45, opcode)
5.  secret bits:     read32(CTRL0) → |= 0x60040000 → write32     [bit23 NEVER touched]
6.  0x44 dance:      read16(0x44); n = read8(0x20) & 0xF;
                    w = (w & 0x00FF) | (n<<8) | (n<<12);
                    write16(0x44, w)                              [clobbers 0x45!]
7.  opcode #2:       write8(0x45, opcode)                          [re-written after 0x44]
8.  TX_COUNT:        write8(0x48, tx_len)
9.  FIFO fill:       write8(0x80+i, data[i]) for i in 0..tx_len-1
10. RX_COUNT:        write8(0x4B, 0)                               [TX-only for a write]
11. trigger:         write8(0x47, 0x80)
12. poll:            read8(0x4C), check bit31 busy [Windows: buggy/no-op]
13. restore 0x22:    writew(base+0x22, saved)
14. restore opcode:  write8(0x45, original_opcode)                 [0x4684 teardown]
15. ALT_CS exit:     read8(0x1D) → &0xFC → write8                  [deselect CS]
```

**Read path (0x3c20):** identical except:
- TX_COUNT=3 (3 address bytes), NO 0x44 dance
- RX_COUNT=rx_len, response at FIFO+TX_COUNT (0x83)

---

## 3. Linux Driver State — 2026-07-06 (FINAL)

### Read path — WORKING ✓
- `readb` (not `readw & 0xFF`), TX_COUNT=3, offset 0x83
- 6 sync bytes (not 5), 10B header read, parser at `&hdr[6]`
- Body read: `blen + 6` bytes, `0x5A` header validation
- CTRL0 = 0x60040000, no TXMODE bit23, ALT_CS CS1 (0x01)
- **RESET_RSP header+body bit-identical to Windows**

### Write path — EXHAUSTED ✗
EVERY difference from Windows has been fixed — the registers are bit-identical:
- 0x44 dance with `0x00FF` mask (matches Windows), opcode re-written after 0x44
- Single-set FIFO clear, trigger write8(0x47, 0x80)
- 0x22 save/restore, opcode restored to 0x0B post-transfer
- RX_COUNT=0 write, full-duplex test, retry loop 0x80→0x4B+0x02→0x45
- CTRL0 busy poll (more reliable than Windows, which doesn't really wait)
- **The device ignores EVERY write (opcode 0x02) — only reads (0x0B) work**

### Complete Matrix of Failed Tests

| Category | Test | Result |
|-----------|------|-----------|
| **SPI Mode** | Mode 0, 1, 2, 3 | All failed |
| **SPI Speed** | 800 KHz, 12.5 MHz, 25 MHz, 33 MHz | All failed |
| **Trigger V1** | CTRL0 bit16 toggle | Failed |
| **Trigger V2** | 0x47 bit7 toggle | Failed |
| **Trigger V2 hardcoded** | fixed 0x80 | Failed |
| **CTRL1** | write 0x020006B5 (Windows value) | Ignored (read-only register) |
| **0x44 dance** | with masks 0xF0FF, 0x0FFF | Failed |
| **TXMODE** | CTRL0 bit23 | Failed |
| **CS1 hardcoded** | (reg & 0xFC) \| 0x01 | Failed |
| **FIFO opcode prepend** | opcode before data in the FIFO | Failed |
| **Power cycle** | _PS3→_PS0→_RST via ACPI | Failed |
| **GPIO power cycle** | M010 OFF/ON via SSDT | Failed |
| **Vendor init** | 0x02 @0x04 (14-byte cold boot) | Failed |
| **DESCREQ sync IRQ** | ~70us delay | Failed |
| **DESCREQ workqueue** | deferred execution | Failed |
| **PCI 0xB8 bit7** | 16-bit FIFO mode (Windows value) | Fixes the read data layout, NOT the write |
| **Combinations** | Every combination of the categories above | All failed |

---

## 4. FIFO Diagnostics

The `mmio_write.c` module dumps the full 70-byte FIFO after every operation.

### Write (opcode 0x02) — pre/post-write FIFO
- **Pre-write FIFO**: contains the correct TX data (first byte = opcode 0x02, payload data)
- **MOSI confirmed**: the bytes are correctly written into the FIFO register
- **Post-write MISO**: **ALL ZEROS** across the whole 70-byte dump
- The device does NOT drive MISO during the write — the line stays undriven/floating

### Read (opcode 0x0B) — pre/post-read FIFO
- **Pre-read FIFO**: 3 dummy bytes (address), RX_COUNT programmed
- **Post-read MISO**: contains correct HID data (sync bytes + header + body)
- Reading from offset `0x80 + TX_COUNT` works perfectly

### FIFO Diagnostic Conclusion
The AMD FCH controller correctly writes the TX data onto the MOSI bus.
The touchscreen device **selectively ignores** transactions with opcode 0x02:
- MISO stays at zero (undriven/floating) for the entire duration of the write
- No ACK, no error, no response of any kind
- Reads (0x0B) work immediately before and after the failed write

---

## 5. Windows vs Linux MMIO Comparison

Dumps obtained with RWEverything on Windows (4 dumps: IDLE, TOUCH1, TOUCH2, TOUCH3)
and with the diagnostic module on Linux.

| Register | Offset | BIOS/Linux | Windows | Match | Notes |
|----------|--------|-----------|---------|-------|------|
| CTRL0 | 0x00 | 0x6f8ca90b | +=0x60040000 | ✅ | bits[15:8] differ: 0xA9 (Linux) vs 0x0E (Windows) — **hardwired, immutable** |
| CTRL1 | 0x0C | 0x02000000 | 0x020006B5 | ⚠️ | Windows has 0x06B5 in the low bits, but CTRL1 is **read-only** from software |
| ALT_CS | 0x1D | 0xB1 | (r&0xFC)\|0x01=0xB1 | ✅ | Identical |
| ENA | 0x20 | 0x11110713 | low nibble=3 | ✅ | Identical |
| 0x44 | 0x44 | 0x0200 | →0x3300 dance | ✅ | Identical |
| 0x45 | 0x45 | 0x0B/0x02 | opcode ×2 | ✅ | Identical |
| 0x22 | 0x22 | 0x1111 | save/restore | ✅ | Identical |
| Speed | — | 33.33 MHz | from ACPI _CRS | ✅ | Identical |

### CTRL0 bits[15:8] — The Prime Suspect

- **Windows**: 0x0E
- **Linux**: 0xA9
- **Behavior**: every writel to CTRL0 that modifies bits[15:8] is **ignored** by the hardware controller. An immediate read after the write always shows 0xA9.
- **Impact**: these bits control chip-select timing parameters. A wrong value could invalidate the electrical-level framing of writes.
- **Cause**: the value is set by the BIOS at boot and the Cezanne FCH controller doesn't allow it to be modified via MMIO. It's unclear how Windows obtains 0x0E — the Windows driver might reconfigure the controller through a non-MMIO mechanism (PCI config space, SMBus, or something else).

### CTRL1 — Read-only

The Windows value 0x020006B5 (vs Linux's 0x02000000) cannot be reproduced:
writing to CTRL1 is ignored by the controller. The register is read-only from software.

---

## 6. Windows vs Linux FIFO Dump

### Windows (from the RWEverything dump)
The FIFO on Windows still contains residual data from the vendor init command
(`02 00 00 04 82 00 00 04 00 01 01 0C EE 5B`) after a full boot.
This confirms that:
- The Windows controller leaves the TX data in the FIFO
- The device doesn't modify the FIFO during writes
- The FIFO behavior is identical to Linux

---

## 7. Fully Analyzed Windows CSVs

### surface_boot_auto.csv (warm boot, 2384 transactions)
```
RESET_RSP @0x00 → drain ACK → DESCREQ @0x01 → IRQ → DEVICE_DESC type=7
→ DESCREQ2 @0x02 → IRQ → RPT_DESC type=8 → ~962ms gap
→ companion device activation (NOT the touchscreen)
→ runtime DATA (type=1)
```

### surface_init.csv (cold boot, 513 transactions)
```
Vendor init WRITE @0x04 FIRST → IRQ 73us → READ 0x04 → immediate DATA type=1
→ NO DESCREQ during cold boot!
→ The device goes straight into DATA mode after the vendor init
```

### surface_touch.csv (runtime touch)
- 4304-byte full frame reports
- 216-byte heatmap frame reports
- Alternating every ~10 frames
- **Approval bytes**: a8=0x00 (init), a8=0x03 (post-DESCREQ), a8=0x04 (runtime)
- The "approval bytes" are actually an echo of the last write, not protocol constants
- **Device descriptor**: Vendor=0x045E (MS), Product=0x0C19, Version=0x0100

---

## 8. V0 State Machine (62 states)

Standard enumeration flow:
```
resettingSync → _RST (300ms GPIO reset)
  → readResetResponse → header+body RESET_RSP (type=3)
  → writeDescriptorRequest → DESCREQ @0x01 (10 bytes, opcode 0x02)
  → readDescriptorResponse → DEVICE_DESC (type=7)
  → verifyDescriptor → DESCREQ2 @0x02
  → readDescriptorResponse → RPT_DESC (type=8)
  → ready → DATA (type=1)
```

On a repeated RESET_RSP: the device signals an error → the host RESTARTS FROM THE DESCREQ.

---

## 9. V0 Header Encoding (4 bytes)

```
byte[0] = (type << 4) | 2      type: 1=DATA, 3=RESET_RSP, 7=DEVICE_DESC, 8=RPT_DESC
byte[1-2] = body_len * 4 (LE)  e.g.: 0x0010/4=4 (RESET_RSP), 0x0EB0/4=940 (RPT_DESC)
byte[3] = 0x5A (V2 sync)
```

### Observed headers:
| Header | Type | Body len | Context |
|--------|------|----------|----------|
| `12 60 03 5A` | 1 (DATA) | 216 | Initial cold boot |
| `12 40 43 5A` | 1 (DATA) | 4304 | Full touch frame |
| `32 10 00 5A` | 3 (RESET_RSP) | 4 | Reset response |
| `72 80 00 5A` | 7 (DEVICE_DESC) | 32 | Device descriptor |
| `82 B0 0E 5A` | 8 (RPT_DESC) | 940 | Report descriptor |

---

## 10. GPIO (from SSDT5 M009/M010)

- **M084** (GPIO base) = pointer in the CPNV OperationRegion @0x7C7A3018+8
- **M009(0x0103)**: Bank 1, Pin 3, reads `M084 + 0x1202 + pin*4`
- **M010(0x0103, v)**: Bank 1, Pin 3, writes a 2-bit field at offset 6 with `(2|v)`
- **M009(0x5B)**: Bank 0, Pin 91, reads `M084 + 0x1502 + pin*4`
- **M010(0x5B, v)**: Bank 0, Pin 91, writes the output value
- **0x0103**: RESET line (asserted LOW, released HIGH)
- **0x5B**: POWER ENABLE (HIGH = on, LOW = off)

### ACPI Sequences:
- `_INI`: if off → turns power on (0x5B=1), sleeps 300ms, releases reset (0x0103=1)
- `_RST`: reset pulse (0x0103=0, sleep 300ms, 0x0103=1)
- `_PS0`: if it was D3 → turns power on, releases reset
- `_PS3`: if not D3 → asserts reset, turns power off
- WRST/NRST: empty stubs (NOP)

**M009/M010 verified working from Linux** — HW power cycle doesn't unblock writes.

---

## 11. Diagnostic Tools Created

| Tool | Path | Description |
|------|------|-------------|
| `parse_spi.py` | `tools/parse_spi.py` | Full ETW CSV parser: byte-by-byte SPI transactions, timing, IRQ, init sequences |
| `mmio_write.c` | `tools/diagnostics/mmio_write.c` | Raw MMIO kernel module: Windows-exact write bypassing the Linux SPI framework, with FIFO dump |
| `parse_spb_csv.py` | `tools/parse_spb_csv.py` | Alternative parser for SPB CSVs |
| `decode_hidspi.py` | `tools/decode_hidspi.py` | HID-over-SPI header decoder |
| `gpio_test.c` | `tools/gpio_test.c` | M009/M010 GPIO test via ACPI evaluate |

---

## 12. Verification of No Linux Interference

- **No Linux driver interferes** (no upstream spi-amd, no pinctrl, no IRQ conflict)
- **No pre-driver Windows init** (SpbCx doesn't touch registers, _INI only handles GPIO)
- **No firmware upload** to the touchscreen (only the companion device on a different SPB connection)
- **Clean DKMS** (fresh modules, no conflicts)

---

## 13. Final Conclusion

After hours of exhaustive analysis of every software component — Windows driver decompilation,
ETW CSV traces, MMIO and PCI dumps, GPIO/ACPI tests, register-by-register comparison — **every
software option has been exhausted**.

The device **ignores every write (opcode 0x02) to every register**, while every
read (opcode 0x0B) works. The SPI registers are bit-identical between Windows and Linux
(except for CTRL0 bits[15:8], which is hardwired). The MMIO sequence is identical.

### PCI 0xB8 bit7 (16-bit FIFO mode): PARTIAL progress

The PCI dump of the FCH LPC bridge (1022:790e, device 00:14.3) revealed that bit7
of offset 0xB8 controls the FIFO access mode:
- **Windows** (0xB8=0x33ED0084, bit7=1): 16-bit FIFO mode → data read as words
- **Linux default** (0xB8=0x33ED0004, bit7=0): 8-bit FIFO mode → interleaved data

Setting 0xB8 to the Windows value with setpci:
```
setpci -s 00:14.3 B8.L=0x33ED0084
```
...produces the correct read data layout. However, **writes (opcode 0x02)
still don't work**. 16-bit FIFO mode fixes the data layout problem
but NOT the write problem.

### PCI 0xB4

The PCI register 0xB4 (Windows: 0x7DFFE000 — **CORRECTED on the night of 2026-07-06**: the
previously reported value (0x007DFFE0) was a byte-order transcription error,
verified by reading the raw dump bytes with xxd: `b4-b7 = 00 e0 ff 7d` LE =
0x7DFFE000; Linux: 0x00000000) controls the FIFO data layout and the
sync byte count. It is writable via setpci. **Retested on the night of 2026-07-06 with the
CORRECT value** (`setpci -s 00:14.3 B4.L=0x7DFFE000`, together with B8 at the full
value `0x33ED0084`): no effect on writes — the earlier test (done with
`B4.L=0xFFE0`, which wrote 0x0000FFE0, neither the right value nor the
mis-documented one) was invalid; this retest is valid and confirms this
avenue is closed.

### Most Likely Cause

The AMD FCH Cezanne controller under Linux does not produce a valid write signal
that the MSHW0231 touchscreen recognizes. Possible causes:
1. **CTRL0 bits[15:8] = 0xA9 vs 0x0E** — chip-select timing difference
2. **CTRL1 bits[15:0] = 0x0000 vs 0x06B5** — controller configuration difference (read-only)
3. **CS/clock timing** — physical signal-level differences between the two OSes
4. **Controller initialization** — Windows might do a PCI/SMBus init step not visible via MMIO
5. **PCI config space** — other, not-yet-discovered offsets that affect write behavior

### MANDATORY Next Step

**Logic analyzer** to compare the SCK/MOSI/MISO/CS signals between Windows and Linux.
Without this physical measurement, it isn't possible to determine the signal difference,
and no software fix can solve the problem.

---

## 15. Session 2026-07-06 Evening/Night — Definitively Closing Out the Software Avenues

A session without a logic analyzer or a second PC (no breakpoint-based WinDbg), with the
explicit goal of exhausting every remaining "software-only" avenue.

### 15.1 Documentation Bug Fix: PCI 0xB4

The Windows value for 0xB4 reported everywhere (`0x007DFFE0`) was a **transcription
error**: the real value, read byte-by-byte from the raw dump, is
`0x7DFFE000`. As a result, the earlier "0xB4 failed" test had never actually
been run with the correct value (the command used wrote `0x0000FFE0`). Redone with
`setpci -s 00:14.3 B4.L=0x7DFFE000` + `B8.L=0x33ED0084` (full values): **no
effect** — CTRL0/CTRL1 pre-trigger identical, DESCREQ still ignored. This
time the test is valid, and the avenue is genuinely closed.

### 15.2 WPP Tracing of hidspi.sys (via tracepdb+traceview+tracefmt, WDK)

Extracted the TMF files from the PDB (`hidspi.pdb` contains full WPP info: 7 message
GUIDs, ~185 format strings, classes `HidSpiDeviceV0`, `SpbBusWrapper`,
`Fdo`, `CxClient`). Captured two real sessions (disable/enable cycle and
uninstall/rescan hardware in Device Manager) and decoded them with `tracefmt.exe`.

**Result**: both captures show only a D2→D0 resume (never a real
cold reset with RESET_RSP→DESCREQ — not even "Uninstall device" forces an
actual electrical power-down of the device). Observed sequence: `EvtD0Entry` → 14-byte
vendor-init (register 0x000004) → `HidGetFeature Report 6` (10B+129B, valid response) →
**`HidSetFeature Report 5` (14B) completes with STATUS_SUCCESS** — a real write
that succeeds, confirmed from inside the driver itself (clean SmFx state machine,
zero retries). Reproducible in both captures.

**Why it matters**: this is the first confirmation **independent of the
driver's own point of view** (not just external RWEverything MMIO dumps) that opcode 0x02
genuinely works on Windows on this exact hardware. Cross-checked against the
test matrix (which includes "Vendor init @0x04, 14 bytes" among the FAILED tests on
Linux, same register/packet): the block is NOT specific to register 0x000001/
DESCREQ, it is uniform on Linux (fails everywhere) and uniform on Windows
(works everywhere observed) — a Windows-vs-Linux asymmetry, not a
register-by-register one.

### 15.3 Extended PCI Config Space Comparison: 11 Devices Total

In addition to the LPC bridge (00:14.3, already done), compared byte-by-byte: Root
Complex (00:00.0), IOMMU (00:00.2), SMBus (00:14.0), and all 8 Data
Fabric functions (00:18.0-7). RWEverything commands via the Command console (`SAVE <file>.BIN
PCI <Bus#dec> <Dev#dec> <Fun#dec>` — Dev# in decimal, 0x14→20, 0x18→24).

| Device | Outcome |
|--------|-------|
| SMBus (00:14.0) | Identical |
| Data Fabric Fn 0,1,2,3,5,6,7 | Identical |
| Data Fabric **Fn4** (00:18.4, PCI ID 1022:144c) | 3 differing offsets: 0x5C, 0x98, 0x9C |
| Root Complex (00:00.0) | 31 differing bytes, but the pattern looks like volatile state/counters (the same value repeated at multiple offsets), discarded |
| IOMMU (00:00.2) | Windows reads all `0xFFFFFFFF` (the device doesn't respond at that BDF under Windows), irrelevant to SPI |

### 15.4 Data Fabric Fn4 — Tested on HW, No Effect

Searched public AMD documentation (OSRR) and the Linux kernel source: Fn4 is a
real function (used by Linux for L3 cache partitioning masks, but at offset 0x1D4,
outside the 256B compared) — no documentation for 0x5C/0x98/0x9C specifically.
Note that at 0x98/0x9C it is **Linux** that has the high bit set (`0x80000001`),
Windows has `0x00000000` — the opposite direction from the "Windows enables
something Linux is missing" narrative.

Tested on real HW (a test laptop, one register at a time with stability checks in between):
- 0x98, 0x9C: **read-only**, the write is ignored by the hardware (like CTRL0[15:8]/CTRL1)
- 0x5C: the write is **accepted** (set to the Windows value `0x000a0085`), system stable
- Reloaded spi-amd+spi-hid with 0x5C modified: **no effect**, DESCREQ
  still ignored, CTRL0 pre-trigger unchanged. Value restored at the end of the test.

### 15.5 External Research: the linux-surface Community and the "AMD SPI Lock" Mechanism

Checked the state of the `linux-surface` community: the SPI touchscreen on
Surface Laptop 3/4 AMD is a problem that is **known and never solved even by them**
(the `linux-surface/spi-hid` repo, a port of the Surface Duo 2 driver, was never
completed for ACPI-based devices due to lack of hardware — "We very briefly
tried to make that work on the AMD/ACPI-based devices, but it didn't work out").
Our project is further along than their official attempt. No hidden fix to
reuse. The only useful suggestion from their discussions: try
`iommu=off`/`amd_iommu=off` (matches a variable already isolated but not yet
tested in this session at the time).

Found and verified the **"AMD SPI Lock"** mechanism (documented by AMD,
used by fwupd to detect BIOS write-protection): three components —
`RomProtect` (LPC bridge PCI offset **0x50**), `SpiProtectEn0`/`SpiProtectLock`
(our own **ALT_CS, MMIO 0x1D**), `SPIRestrictedCmd`/`SPIRestrictedCmd2` (MMIO
**0x04/0x08**, a list of blocked SPI opcodes). It looked like a strong candidate
(a documented hardware write block, independent of the target register).
**Verified with real data** (dumps already in the project + fresh reads):

| Register | Windows | Linux | Outcome |
|----------|---------|-------|-------|
| RomProtect (LPC 0x50) | `05 05 fa ff 00 00 00 00` | `05 05 fa ff 00 00 00 00` | Identical |
| ALT_CS / SpiProtectEn0-Lock (0x1D) | `0xB1` | `0xB1` | Identical |
| SPIRestrictedCmd (MMIO 0x04) | `0x00000006` | `0x00000006` | Identical |
| SPIRestrictedCmd2 (MMIO 0x08) | `0x00000000` | `0x00000000` | Identical |

All four components of the mechanism are byte-identical between Windows and
Linux — real and well documented, but on this machine it isn't the cause. Avenue
closed with verified data, not just theory.

### 15.6 Protocol: External Confirmation of the "0x0100 Legacy" Version + Opcode 0x07 (SET_POWER) Retested

The public v1.0 spec PDF (page 16, sample ASL) explicitly says
"Revision ID – Will be 3 for HidSpi V1"; our `_DSM` responds with revision 1,
which per the spec itself is an "unsupported revision" — confirming that MSHW0231 does not
implement the public v1.0 but something earlier/undocumented.

Found a 2022 LKML thread (patchew.org, an upstream spi-hid proposal from a
Microsoft engineer) in which **Maximilian Luz (linux-surface maintainer)
explicitly confirms that AMD/x86 devices use
`SPI_HID_SUPPORTED_VERSION 0x0100`**, not 0x0300 (the version of the proposed
driver/public spec) — an independent external confirmation, not just our
own deduction from the _DSM. A real Microsoft repository was cited (Surface Duo,
Qualcomm) with code for 0x0100: the write there is a single TX-only
half-duplex transfer (framing already tested by us, and it failed), with a 6-byte header identical
to our own model — structural validation, no new avenue. No
AMD-specific quirk present (the code is Qualcomm/pinctrl-specific).

Also found, in an ABANDONED branch of our own project (DKMS package
`/usr/src/spi-hid-0.1`, never merged into the active driver), an opcode never
tested in this session: **0x07 (HidSpiCx-style SET_POWER)**, with a
comment claiming empirical success — but for a problem different from ours
(a device that was completely asleep, with every read returning 0xFF; in our case
reads already work fine from the start). Retested on HW
today: MISO responded `0xFF` (different from the `0x00` of 0x02 writes) but
**no real effect** followed — DESCREQ still fails identically.
0xFF is almost certainly a floating bus, not a device response. Avenue closed.

### 15.7 The "Ghost Pre-Boot Module" Hypothesis — Investigated and Closed

A valid suspicion raised by the user: a module was auto-loading on every boot (observed
throughout the whole session) — possibly corrupting the hardware before our clean tests
(a real precedent from 2026-07-04: poking CTRL0 put the controller into a corrupted state
that survived rmmod, requiring a reboot). Investigation: that module was NOT the abandoned
DKMS package (which was only installed for the LTS kernel) but an **old build of our own
project, frozen inside the ACTIVE INITRAMFS**, loaded 1.24s into boot (before the
real filesystem), invisible to rmmod/on-disk searches. Regenerated a clean initramfs and
synced the hash in `/boot/limine.conf` (via `limine-entry-tool`, required because Limine
verifies integrity via a hash). **Decisive test on a genuinely pristine boot** (confirmed via
`/sys/bus/acpi/devices/AMDI0060:00/` — no driver, no module, no SPI device,
dmesg silent): the result was IDENTICAL to always, DESCREQ ignored, MISO zero. The hypothesis is
ruled out with the cleanest possible proof — the initramfs fix remains valid regardless
(it was a real system hygiene problem) but doesn't change the underlying verdict.

### 15.8 Definitive Conclusion

Sections 15.1-15.7 close out, in a verified and clean way (no invalid test like
in the past), **every remaining software avenue**: PCI config space (LPC bridge AND
Data Fabric), driver-internal WPP/tracing, _OSI/ACPI, WREN/SPI-NOR heritage,
kernel lockdown/SME/IOMMU (tested live with a dedicated `iommu=off
amd_iommu=off` boot — AMD-Vi confirmed disabled, no effect), the AMD
SPI Lock mechanism (RomProtect/SpiProtectEn0/SPIRestrictedCmd). None of these produced any
effect on the write path. The verdict remains the one from section 13: **a physical
measurement is required (a logic analyzer on MOSI/CS/SCK)**, or a real breakpoint-based
WinDbg capture (which requires a second PC, not available). No further plausible
software ideas remain to be tested.

---

## 14. References

| Resource | Path |
|----------|------|
| Linux SPI driver | `driver/spi-amd.c` |
| Linux HID driver | `driver/spi-hid-core.c` |
| Windows ETW CSVs | `traces/surface_*.csv` |
| hidspi.sys decomp | `docs/decomp/clean/` |
| amdspi.sys decomp | `docs/decomp/amdspi/` |
| Windows MMIO dump | `docs/windows_mmio_dumps/` |
| Windows PCI dump | Windows-side, `Desktop\windrivers\` |
| ACPI Linux | `docs/acpi/linux/` |
| ACPI Windows | `docs/acpi/windows/` |
| CSV parser | `tools/parse_spi.py` |
| MMIO test module | `tools/diagnostics/mmio_write.c` |
| WPP TMF + PDB + decoded logs | Windows-side, `Desktop\windrivers\` (hidspi.pdb/.sys, *.tmf, logtouch*.etl/_decoded*.txt) |
| 11-device PCI dump (Windows .BIN + Linux reference) | Windows-side, `Desktop\windrivers\` |
| GitHub Repository | https://github.com/Syax89/SL4A_TouchScreen |
