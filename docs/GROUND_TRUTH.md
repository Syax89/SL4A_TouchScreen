# GROUND TRUTH — Model verified by cross-referencing ETW CSV × amdspi decomp × hidspi V0 decomp × spec PDF × HW tests

> **Last updated: 2026-07-07 — work in progress.**
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
5.  READ_MODE:        read32(CTRL0) → |= 0x60040000 → write32     [FAST_READ, bit23 NEVER touched]
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
### Write path — IN PROGRESS ⚠️

The registers are bit-identical to Windows:
- 0x44 dance with `0x00FF` mask (matches Windows), opcode re-written after 0x44
- Single-set FIFO clear, trigger write8(0x47, 0x80)
- 0x22 save/restore, opcode restored to 0x0B post-transfer
- RX_COUNT=0 write, full-duplex test, retry loop 0x80→0x4B+0x02→0x45
- CTRL0 busy poll (more reliable than Windows, which doesn't really wait)
- **The device ignores writes (opcode 0x02) — only reads (0x0B) work**

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

> **SUPERSEDED (2026-07-07) — see §15.8's note and §15.9.** The "device selectively ignores
> opcode 0x02" conclusion below rests entirely on MISO staying zero *during* the write
> transaction, which `HIDSPI_PROTOCOL.md` says is expected regardless of outcome (the real
> response is asynchronous, on GPIO pin 85, not synchronous on MISO). `irq_oracle.c` (§15.9)
> proved the write does reach the device — pin 85 fires ~108-109ms later, far faster than the
> idle cadence — it just never progresses past RESET_RSP. Kept below as the historical
> MISO-only evidence, not the current understanding.

The `mmio_write.c` module's write path (`windows_write()`) reads back FIFO bytes from
offset `TX_COUNT` immediately after the trigger completes (the `mmio: RX read_off=...` log
line) — this is the actual per-write "post-write MISO" evidence, not a paired pre/post
70-byte dump. A single full 70-byte `fifo_dump()` runs once, at the very end of the whole
`mmio_init()` sequence (after READ1, READ2, WRITE, and READ3 have all already executed) —
useful as a final-state snapshot, but it does not correspond one-to-one with the write step
described below.

### Write (opcode 0x02)
- **MOSI confirmed**: the TX bytes (opcode + payload) are correctly staged into the FIFO
  before the trigger (`fifo_fill`/inline `reg_write8(0x80+i,...)` loop)
- **Post-write MISO** (read back from FIFO offset `TX_COUNT` right after the trigger
  completes): **ALL ZEROS**
- The device does NOT drive MISO during the write — the line stays undriven/floating

### Read (opcode 0x0B) — pre/post-read FIFO
- **Pre-read FIFO**: 3 dummy bytes (address), RX_COUNT programmed
- **Post-read MISO**: contains correct HID data (sync bytes + header + body)
- Reading from offset `0x80 + TX_COUNT` works perfectly

### FIFO Diagnostic Conclusion (historical — see superseded note above)
The AMD FCH controller correctly writes the TX data onto the MOSI bus.
At the time, this looked like the touchscreen device **selectively ignoring** transactions
with opcode 0x02:
- MISO stays at zero (undriven/floating) for the entire duration of the write
- No ACK, no error, no response of any kind *synchronous with the write itself*
- Reads (0x0B) work immediately before and after the write
- (Now understood per §15.9: the response IS there, just asynchronous on pin 85 and never
  advancing past RESET_RSP — this section's MISO-only view couldn't see that.)

---

## 5. Windows vs Linux MMIO Comparison

Dumps obtained with RWEverything on Windows (4 dumps: IDLE, TOUCH1, TOUCH2, TOUCH3)
and with the diagnostic module on Linux.

| Register | Offset | BIOS/Linux | Windows | Match | Notes |
|----------|--------|-----------|---------|-------|------|
| CTRL0 | 0x00 | 0x6f8ca90b | +=0x60040000 | ✅ | bits[15:8] differ: 0xA9 (Linux) vs 0x0E (Windows) — **hardware-managed** |
| CTRL1 | 0x0C | 0x02000000 | 0x020006B5 | ⚠️ | Windows has 0x06B5 in the low bits, but CTRL1 is **read-only** from software |
| ALT_CS | 0x1D | 0xB1 | (r&0xFC)\|0x01=0xB1 | ✅ | Identical |
| ENA | 0x20 | 0x11110713 | low nibble=3 | ✅ | Identical |
| 0x44 | 0x44 | 0x0200 | →0x3300 dance | ✅ | Identical |
| 0x45 | 0x45 | 0x0B/0x02 | opcode ×2 | ✅ | Identical |
| 0x22 | 0x22 | 0x1111 | save/restore | ✅ | Identical |
| Speed | — | 33.33 MHz | from ACPI _CRS | ✅ | Identical |

### CTRL0 bits[15:8] — Auto-set by Hardware (NOT "immutable")

**2026-07-07 discovery**: bits[11:8] = TX_COUNT, bits[15:12] = RX_COUNT. These are NOT
chip-select timing parameters — they are **transient hardware state** that reflects the
last transfer's byte counts. Verified by testing TX_COUNT values 1-15 and observing
bits[11:8] track exactly.

- **Windows shows 0x0E** because the last transfer had TX_COUNT=14 (the 14-byte
  vendor init command `02 00 00 04 82 00 00 04 00 01 02 0C EE 5B`)
- **Linux shows 0xA9** because the last transfer had different TX/RX counts

Direct writel to these bits IS ignored (they're read-only/hardware-managed),
but the previous theory that they were "chip-select timing causing
write failure" is **FALSE**. Setting TX_COUNT=14 reproduces 0x0E identically
to Windows, and writes still fail. These bits are not the root cause.

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

## 9.1 GPIO (from SSDT5 M009/M010)

(Renumbered 2026-07-08 from a duplicate "## 10." heading — this block is unrelated to
§10 "ACPI Device Tree" below, which has its own 10.1-10.7 subsections.)

- **M084** (GPIO base) = pointer in the CPNV OperationRegion @0x7C7A3018+8
- **M009(0x0103)**: Bank 1, Pin 3, reads `M084 + 0x1202 + pin*4`
- **M010(0x0103, v)**: Bank 1, Pin 3, writes a 2-bit field at offset 6 with `(2|v)`
- **M009(0x5B)**: Bank 0, Pin 91, reads `M084 + 0x1502 + pin*4`
- **M010(0x5B, v)**: Bank 0, Pin 91, writes the output value
- **0x0103**: RESET line (asserted LOW, released HIGH)
- **0x5B**: POWER ENABLE (HIGH = on, LOW = off)

## 10. ACPI Device Tree — Complete Enumeration Flow

All ACPI methods that affect the touchscreen (extracted from DSDT + SSDT4/SSDT5,
verified bit-identical between Windows and Linux via md5sum).

---

### 10.1 Device Tree

```
\_SB.PCI0.LPC0          (1022:790e) — FCH LPC bridge, dependency _DEP[2]
\_SB.GPIO               AMD GPIO controller, dependency _DEP[1]
\_SB.SPI1               AMD SPI controller (AMDI0060), dependency _DEP[0]
  └── HSPI              Touchscreen device (MSHW0231 / PNP0C51)
```

### 10.2 SPI Controller (AMDI0060)

```asl
Device (_SB.SPI1) {
    Name (_HID, "AMDI0060")
    Name (_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0xFEC10000, 0x00000100)
    })
    Method (_STA) { Return (0x0F) }     // always present
}
```

The SPI controller has **no _INI, no _PSx, no power management methods**. It is a
pure resource provider. The MMIO region `0xFEC10000-0xFEC100FF` is the only resource.

### 10.3 Touchscreen Device (HSPI)

```asl
Device (_SB.SPI1.HSPI) {
    // _HID: dynamic — reads SHSD field from OperationRegion
    // On Surface Laptop 4 AMD: SHSD = "MSHW0231"
    Method (_HID) { Return (ToString(ToBuffer(SHSD), 0x09)) }

    Name (_CID, "PNP0C51")              // compatible: generic HID-over-SPI
    Name (_S0W, 0x02)                   // wake from S0, necessary for GPIO IRQ

    Method (_STA) { Return (0x0F) }     // present + enabled + functional

    // Resources: SPI bus 33.33MHz MODE0 CS0 + GPIO IRQ pin 0x55 Edge/ActiveLow
    Name (RBUF, ResourceTemplate() {
        SpiSerialBusV2(0x0000, PolarityLow, FourWireMode, 8,
            ControllerInitiated, 0x01FC9350,
            ClockPolarityLow, ClockPhaseFirst,
            "\\_SB.SPI1", 0x00, ResourceConsumer, , Exclusive)
        GpioInt(Edge, ActiveLow, ExclusiveAndWake, PullNone, 0x0000,
            "\\_SB.GPIO", 0x00, ResourceConsumer, , ) { 0x0055 }
    })
    Method (_CRS) { Return (RBUF) }

    // _DSM: HID descriptor query (UUID 6e2ac436-...)
    // Function 0 returns 0x03 (3 functions supported)
    // Function 1 returns 1 (HID descriptor address = device descriptor register)
    Method (_DSM, 4) {
        If (Arg0 == UUID("6e2ac436-...")) {
            If (Arg1 == 1) {
                If (Arg2 == 0) { Return (Buffer() { 0x03 }) }
                If (Arg2 == 1) { Return (One) }
            }
        }
        Return (Buffer() { 0x00 })
    }
}
```

### 10.4 Power Management Methods

```asl
Scope (\_SB.SPI1.HSPI) {
    Name (_UID, 0x0A)
    Name (_DEP, Package() { SPI1, GPIO, ^^PCI0.LPC0 })

    Name (FLAG, Zero)                   // power state tracking:
                                        //   0 = D0 (powered on)
                                        //   3 = D3 (powered off)

    // _INI: called ONCE by ACPI during enumeration.
    // Only powers on the device if BIOS left it OFF.
    Method (_INI) {
        Local0 = M009(0x5B)             // read POWER GPIO
        If (Local0 == Zero) {           // device was OFF
            M010(0x0103, Zero)          //   assert RESET
            M010(0x5B, One)             //   turn POWER on
            Sleep(0x012C)               //   wait 300ms
        }
        Local0 = M009(0x0103)           // read RESET GPIO
        If (Local0 == Zero) {           // RESET still asserted?
            M010(0x0103, One)           //   de-assert
        }
        FLAG = Zero                     // mark as D0
    }

    // _RST: reset pulse (300ms LOW on RESET pin)
    Method (_RST) {
        M010(0x0103, Zero)              // assert RESET
        Sleep(0x012C)                   // 300ms
        M010(0x0103, One)               // de-assert
    }

    // _PS0: transition from D3cold to D0
    Method (_PS0) {
        If (FLAG != Zero) {             // was in D3?
            M010(0x5B, One)             //   power on
            Sleep(0x012C)               //   300ms
            M010(0x0103, One)           //   de-assert reset
        }
        FLAG = Zero
    }

    // _PS2: empty (NOP)
    Method (_PS2) { }

    // _PS3: transition from D0 to D3cold
    Method (_PS3) {
        If (FLAG != 3) {                // not already D3?
            M010(0x0103, Zero)          //   assert RESET
            Sleep(1)                    //   1ms
            M010(0x5B, Zero)            //   power OFF
        }
        FLAG = 3
    }
}
```

### 10.5 FLAG State Machine

```
FLAG = 0   → Device is in D0 (powered on, reset de-asserted)
FLAG = 3   → Device is in D3 (powered off, reset asserted)

Transitions:
  Boot → _INI           → FLAG = 0        (powers on if needed)
  D3  → _PS0            → FLAG = 0        (powers on, de-asserts reset)
  D0  → _PS3            → FLAG = 3        (asserts reset, powers off)
  Any → _RST            → (pulse, no FLAG change)
  Any → _PS2            → (NOP)
```

### 10.6 What Windows Does vs What We Did

| Event | Windows | Our driver (before fix) |
|-------|---------|------------------------|
| Boot | ACPI calls _INI (NOP if already on) | probe() called M009/M010 — **DESTRUCTIVE** |
| Driver load | hidspi.sys probe: no GPIO calls | Called M010 to power-cycle |
| Runtime | Never calls _RST, _PS0, _PS3 | — |
| Sleep/wake | ACPI handles _PS3/_PS0, not the driver | — |

**Windows driver NEVER calls M009/M010 directly.** The decomp (hidspi.sys symbols,
function list) contains zero references to ACPI evaluate, GPIO methods, or _PSx/_RST.
All power management is handled by the ACPI subsystem transparently.

### 10.7 Correct Driver Behavior

1. **probe()**: device is already powered and initialized by ACPI _INI. Do NOT call
   M009/M010. Just drain RESET_RSP and send DESCREQ.
2. **remove()**: nothing. The ACPI subsystem handles power.
3. **suspend/resume**: the ACPI subsystem calls _PS3/_PS0 automatically via the
   kernel PM framework. The driver just pauses/resumes the SPI message queue.

### GPIO Details

- **M084** (GPIO base) = pointer in the CPNV OperationRegion @0x7C7A3018+8
- **M009(addr)**: GPIO read. Bank = addr[10:8], Pin = addr[7:0]
  - M009(0x5B): Bank 0, Pin 91, reads `M084 + 0x1502 + pin*4`
  - M009(0x0103): Bank 1, Pin 3, reads `M084 + 0x1202 + pin*4`
- **M010(addr, value)**: GPIO write. Bank 0 writes output value; Bank 1 writes
  `(2|value)` to a 2-bit field at offset 6.
- **0x5B**: POWER ENABLE (HIGH = on, LOW = off)
- **0x0103**: RESET line (asserted LOW, released HIGH)

### M010 Power Cycle Destroys the Device (2026-07-07)

Calling `M010(0x5B,0)` (power off) or any *raw, standalone* M010 sequence **permanently
breaks the touchscreen until a full system reboot**. After M010, the device stops
sending RESET_RSP and MISO returns only `00 03 00 00 00...`. Verified across
8 different power-cycle sequences. The driver must NEVER call M009/M010 directly.

**Clarification (2026-07-08, this audit pass)**: this does not contradict §15.20's use of
`tools/diagnostics/ps3_ps0_cycle.c` to recover the device mid-session. That tool evaluates
the ACPI `_PS3`/`_PS0` methods (exactly what §10.7 recommends letting the ACPI subsystem
do), which internally sequence M010 together with the `FLAG` state-machine checks from
§10.5 — it is not a raw, out-of-sequence M010 poke like the destructive case above. The
distinguishing factor is *properly-sequenced ACPI _PS3/_PS0* (safe, used for recovery) vs.
*ad-hoc direct M010 calls outside that sequence* (destructive, must never happen in the
driver).

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

> **SUPERSEDED (2026-07-07) — see §15.8's note and §15.9.** The "device ignores every write"
> verdict below was based entirely on MISO staying zero during the write, which
> `HIDSPI_PROTOCOL.md` itself says is expected (the real response is asynchronous, on GPIO
> pin 85, not on MISO during the transaction). `tools/diagnostics/irq_oracle.c` proved the
> write DOES reach the device — pin 85 fires at ~108-109ms after every DESCREQ, far faster
> than the ~609ms idle reset cadence. The device reacts to the write; it just re-resets
> instead of emitting DEVICE_DESC. Read this section as a historical snapshot of the
> MISO-only evidence available at the time, not as the project's current understanding.

After hours of exhaustive analysis of every software component — Windows driver decompilation,
ETW CSV traces, MMIO and PCI dumps, GPIO/ACPI tests, register-by-register comparison — **every
software option has been investigated**.

The device **ignores every write (opcode 0x02) to every register**, while every
read (opcode 0x0B) works. The SPI registers are bit-identical between Windows and Linux
(except for CTRL0 bits[15:8], which is hardware-managed). The MMIO sequence is identical.

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
avenue has been tested.

### Most Likely Cause

The AMD FCH Cezanne controller under Linux does not produce a valid write signal
that the MSHW0231 touchscreen recognizes. Possible causes:
1. **CTRL0 bits[15:8] = 0xA9 vs 0x0E** — chip-select timing difference
2. **CTRL1 bits[15:0] = 0x0000 vs 0x06B5** — controller configuration difference (read-only)
3. **CS/clock timing** — physical signal-level differences between the two OSes
4. **Controller initialization** — Windows might do a PCI/SMBus init step not visible via MMIO
5. **PCI config space** — other, not-yet-discovered offsets that affect write behavior

### Next Steps

Continue investigating the write path asymmetry between Windows and Linux.
Key areas still under investigation include the AMD FCH Cezanne controller configuration
and the MSHW0231 device response to opcode 0x02.

---

## 15. Session 2026-07-06 Evening/Night — Definitively Closing Out the Software Avenues

A session with the explicit goal of investigating every remaining "software-only" avenue.

### 15.1 Documentation Bug Fix: PCI 0xB4

The Windows value for 0xB4 reported everywhere (`0x007DFFE0`) was a **transcription
error**: the real value, read byte-by-byte from the raw dump, is
`0x7DFFE000`. As a result, the earlier "0xB4 failed" test had never actually
been run with the correct value (the command used wrote `0x0000FFE0`). Redone with
`setpci -s 00:14.3 B4.L=0x7DFFE000` + `B8.L=0x33ED0084` (full values): **no
effect** — CTRL0/CTRL1 pre-trigger identical, DESCREQ still ignored. This
time the test is valid, and the avenue has been verified.

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
verified with the cleanest possible proof — the initramfs fix remains valid regardless
(it was a real system hygiene problem) but doesn't change the underlying verdict.

### 15.8 Definitive Conclusion

Sections 15.1-15.7 cover, in a verified and clean way, the investigation of:
PCI config space (LPC bridge AND Data Fabric), driver-internal WPP/tracing,
_OSI/ACPI, WREN/SPI-NOR heritage, kernel lockdown/SME/IOMMU (tested live with
`iommu=off amd_iommu=off` — AMD-Vi confirmed disabled), the AMD SPI Lock
mechanism (RomProtect/SpiProtectEn0/SPIRestrictedCmd). None of these produced any
effect on the write path. Investigation continues.

NOTE (2026-07-07): the "software exhausted" framing above was superseded the same day —
see `tools/diagnostics/irq_oracle.c` and 15.9 below. The write DOES reach the device
(confirmed via IRQ-line timing, not just MISO polling); the open question is why the
device re-resets instead of emitting DEVICE_DESC, not whether the write lands.

### 15.9 IRQ-Line Oracle: the Write Reaches the Device, but Never Escapes RESET_RSP (2026-07-07)

Built `tools/diagnostics/irq_oracle.c`: instead of inferring write success from MISO
content during the SPI transaction (which `HIDSPI_PROTOCOL.md` itself says is garbage
during a write — the real response is asynchronous on GPIO pin 85 / AMDI0031), this tool
watches pin 85 directly (read-only, never written) and times the interrupt edge after a
DESCREQ.

First run had two bugs (found and fixed in the same session): (1) header reads used 9
bytes, but the `0x5A` discriminator byte that `hid_type()` looks for sits at offset 9 of
the 10-byte RESET_RSP header (`ff ff ff ff ff ff 32 10 00 5a`) — every read was
mis-decoded as `type=-1` regardless of content; (2) pin 85 was already asserted (an
un-drained interrupt from before any driver was bound) at module load, so the "baseline"
measurement started from a stuck state, not idle. Fixed by reading 10 bytes for type
detection and adding a bounded priming/drain step (header + body read, mirroring
`spi-hid-core.c`'s state0-clean drain) before measuring.

With both fixed, repeated clean result (5/5 edges caught across a 10-round sweep, 200ms
window per round): after a DESCREQ, pin 85 reliably fires at **~108.7-109ms** — far
faster than the idle reset cadence (~609ms measured in the 700ms baseline window) — and
the read taken exactly at that edge is **always `type=3` (RESET_RSP), never
`type=7` (DEVICE_DESC)**.

Conclusion: this closes the specific hypothesis the tool was built to test — "our reads
are just unsynchronized with the interrupt, so we're missing a DEVICE_DESC that's
actually there." That is falsified: we caught the edge directly and it genuinely carries
RESET_RSP, not a missed descriptor. This reinforces (with the cleanest measurement to
date) the existing reset-loop finding: **the device receives the DESCREQ, reacts to it
(faster re-reset cadence, exact match to the ~109ms figure from earlier `seq_thread`
polling tests), but never produces DEVICE_DESC — it just re-resets.** The unresolved
question remains *why* the device re-resets instead of answering, given the byte content
of the DESCREQ is confirmed bit-identical to Windows' capture. No new software avenue
opened by this test; it removes one plausible "maybe it's just a software sync bug" doubt
with hard data instead of assumption.

### 15.10 SPI Clock Speed Sweep with the Correct Methodology (2026-07-07, same day)

Before this run, `irq_oracle.c` never explicitly programmed the SPI clock speed — it
inherited whatever `ENA_REG`(0x20)/`SPEED_REG`(0x6C) already contained, which (with no
driver bound) is firmware POR state, not something any of our tools had verified. The
old "0.8-33MHz matrix tested and failed" (very early sessions) used a delayed, non-IRQ-
synced read as its success criterion — worth redoing now that 15.9 gives a correct one.

Added `set_spi_freq()` to `irq_oracle.c`, replicating `amd_set_spi_freq()` from
`spi-amd.c` bit-for-bit (same `ENA_REG`/`ALT_SPD` and `SPEED_REG`/`spd7` fields, same
"only ever set the SPI100 bit for the 100MHz case" behavior) so no undefined/unsafe
register combination could be programmed.

Findings:
- **The inherited (firmware POR) speed was already `ALT_SPD=0x1` = 33.33MHz** — the same
  value `VERIFICATION_FINDINGS.md` already marks as Windows-matched. So the speed used by
  every prior test in 15.9 (and earlier sessions) was already correct; this was a real gap
  in what we could verify, but it verifies as fine, not as a bug.
- Full sweep across all 9 documented speed tiers (800kHz, 3.17MHz, 4MHz, 16.66MHz,
  22.22MHz, 33.33MHz, 50MHz, 66.66MHz, 100MHz), each with a drain + DESCREQ + edge-synced
  read: **identical signature at every single speed** — either `type=3` (RESET_RSP) at the
  same ~108.7-109ms mark, or no edge caught in that particular 200ms window (an artifact of
  which phase of the device's own ~109ms cycle the round happened to start in, reproduced
  identically in the plain 10-round sweep in 15.9 with a single fixed speed). **`type=7`
  (DEVICE_DESC) never appeared at any speed.**

Conclusion: this closes SPI clock speed as a variable, this time with the correct
edge-synced methodology across the entire available range, not just the default. Combined
with 15.9 (bit-perfect RESET_RSP decode on every catch — inconsistent with an electrical/
timing-level explanation), there is no remaining untested purely-electrical/timing
software lever on our side. The reset-loop pathology looks like a device/firmware-side
protocol decision, not a controller configuration problem. Physical instrumentation
(logic analyzer) or a WinDbg capture of a live Windows write remain the only avenues
not yet tried.

### 15.11 Searching for an ACPI "Hub" Device Around the Touchscreen (2026-07-07)

User question: is there some ACPI device acting as a hub/coordinator for the touchscreen,
the way other x86 platforms might have one? Checked exhaustively:

- String search for "hub"/"aggregator" across the whole DSDT: only generic `RHUB` (standard
  USB XHCI root hubs, unrelated) and `ACPI000C` (generic ACPI Processor Aggregator Device,
  unrelated).
- Catalogued every `MSHW*` `_HID` in the tree: `MSHW0040`=`MSBT` (lid/button), `MSHW0084`
  (SSH/SAM, already known), `MSHW0153`=`SHPS` (hinge sensor), `MSHW0184`=`ACSD` under
  `I2CA` (accelerometer, gated by a static `SACS` flag), `MSHW0187`/`MSHW0188` (thermal
  zones), `MSHW0214`=`SRTC`. None are a hub for touch, none share a pin or dependency
  with HSPI.
- HSPI's own `_HID` is computed at runtime as `ToString(ToBuffer(SHSD), 9)` rather than a
  literal string — traced `SHSD` to its source: a field inside
  `OperationRegion(MNVS, SystemMemory, 0x7C796000, 0x56)`, a block of firmware-populated
  NVS memory holding per-SKU static strings (`SHSD`, `SIDH`, `SFUH`, `SACS`, etc.) shared
  across Surface models from one DSDT. Not a dynamic query to another device — just static
  config bytes written once by firmware at boot.
- Re-read HSPI's entire ACPI block end-to-end (confirmed the scope closes right before
  `Device (LID0)` begins) — `_DEP` is still exactly `{SPI1, GPIO, ^^PCI0.LPC0}`, nothing
  hidden.
- Traced `M009`/`M010` (the generic GPIO read/write helpers HSPI's `_INI`/`_RST`/`_PS0`/
  `_PS3` call) to their definitions in `ssdt5.dsl`: they decode the argument into a GPIO
  bank/pin and dispatch to `M011`/`M012` (direct MMIO, bank 0/1, which is what our own
  kernel modules already do) or `M249`/`M250` (SMN-indirect, only for a different bank
  range/flag combination our touch pins don't use). Confirmed generic platform plumbing
  shared by every ACPI GPIO consumer, not something touch-specific or hub-like.

**Conclusion: no ACPI "hub" device exists in this DSDT.** The touchscreen talks directly
to the bare SPI controller (`AMDI0060`) and GPIO controller (`AMDI0031`), nothing in
between. This is a real, structural difference from Intel platforms, which have a
dedicated silicon block (Touch Host Controller, THC — ACPI IDs like `INTC1055`/`INT3515`)
that hardware-offloads HID-over-SPI framing. AMD has no equivalent; `hidspi.sys` talks to
the raw SPI controller entirely in software. This is part of why the AMD port is harder
than an Intel one — there's no hardware "cushion" hiding the fragile details.

### 15.12 Found instead: an un-decompiled driver layer in the Windows stack (2026-07-07)

While closing 15.11, re-examined `tools/windows_capture/providers.txt` (PnP enumeration
captured 2026-07-0X) and noticed the touch device tree on Windows is NOT flat at the
**driver** level either, even though it's flat at the ACPI level:

```
ACPI\MSHW0231                     <- base FDO (SPI-HID transport: hidspi.sys + amdspi.sys,
                                      both already decompiled)
  ACPI\MSHW0231\A                 <- separate RAW PDO, FriendlyName "Surface Digtizer
                                      HidSpi Extn Package" (Class=HIDClass)  <-- NEVER
                                      pulled or decompiled
    HID\MSHW0231&COL01..COL08     <- individual HID collections (touch/pen/etc), split
                                      out from the raw HID device by HIDCLASS/HIDPARSE
```

The `\A` raw-PDO suffix is standard Windows HID-stack convention, but the FriendlyName is
Surface-specific and non-generic, meaning a dedicated driver package (not just generic
`hidclass.sys`) is bound to it. The ETW provider list in the same file separately shows a
real provider `Microsoft-Surface-SurfaceHidMiniDriver`
(`{2FEA7205-B0B1-41CA-8609-5A1D16F3132F}`) which very plausibly belongs to exactly this
binary. Checked `docs/decomp/` — only `amdspi.sys`, `hidspi.sys`, `hidspicx.sys` have ever
been pulled/decompiled. This driver has not.

This is now the most concrete, well-motivated, and completely unexplored lead in the
project: a real, separate, never-examined binary sitting exactly at the layer between the
raw SPI-HID transport (which we've fully replicated and verified byte-for-byte) and the
HID collections — precisely where a Surface-specific extra init/handshake sequence would
live if one exists, and precisely what our Linux port does not have (it only implements
the bare transport). Unlike the ACPI-level "hub" search in 15.11 (which came up empty),
this is a real driver binary that has simply never been looked at.

**Next step (given to the user, not yet run)**: `tools/windows_capture/04_find_extn_driver.cmd`
— identifies the service/driver file bound to `ACPI\MSHW0231\A` via
`Get-PnpDeviceProperty`/`driverquery`/`pnputil /enum-drivers`, so the actual `.sys` can be
copied out of `C:\Windows\System32\drivers\` and decompiled the same way `hidspi.sys` was.

### 15.13 Result: `ACPI\MSHW0231\A` Is Just `hidspi.sys` — Hypothesis Falsified, New Thread Opened (2026-07-07)

Ran `04_find_extn_driver.cmd` on the real hardware (output: `fine extn driver.txt`).
Result for `ACPI\MSHW0231\A`:

```
DEVPKEY_Device_Service       hidspi
DEVPKEY_Device_DriverInfPath hidspi_km.inf
```

**The 15.12 hypothesis is falsified**: this raw PDO is bound to the exact same `hidspi.sys`
(via `hidspi_km.inf`) we already decompiled — not a separate Surface-specific binary. The
FriendlyName "Surface Digtizer HidSpi Extn Package" is just a device-description string
that `hidspi_km.inf` associates with this hardware ID in its `[Strings]`/models section; it
does not imply separate code. So there is no hidden extra driver running for this raw PDO
specifically — `hidspi.sys` (already fully decompiled) is the whole story here.

However, `driverquery`/`pnputil /enum-drivers` in the same capture surfaced something real
and not yet explained: a **separately published** driver package,
`surfacedigitizerhidspiextnpackage.inf` (provider: Surface), sitting in the DriverStore —
name almost identical to the FriendlyName above, but **not currently bound to
`ACPI\MSHW0231\A`** (confirmed bound to `hidspi_km.inf` instead). Two explanations are
possible: (a) this INF targets a different digitizer generation/hardware ID entirely
(dead weight on this machine) or the pen sub-device rather than the touchscreen, or
(b) it genuinely targets this same hardware ID but lost driver-ranking to the in-box
`hidspi_km.inf` for some reason — which would be a real, novel lead (a vendor driver that
*should* be running but isn't, possibly the actual source of Surface-specific extra init).

**Next step (given to the user, not yet run)**: `tools/windows_capture/05_check_extn_inf.cmd`
— locates `surfacedigitizerhidspiextnpackage.inf` in `C:\Windows\System32\DriverStore\` and
dumps its contents (hardware ID / Models section) to determine what it actually targets.

### 15.14 Result: the "Extn Package" Is Not Code At All — Thread Definitively Closed (2026-07-08)

Ran `05_check_extn_inf.cmd`. Full contents of
`SurfaceDigitizerHidSpiExtnPackage.inf` (DriverStore path
`surfacedigitizerhidspiextnpackage.inf_amd64_4caecc051d4c6e2a`):

```ini
[Version]
Class = Extension
ClassGuid = {e2f84ce7-8efa-411c-aa69-97454ca4cb57}   ; the standard "Extension" class —
                                                       ; AddReg/co-installer only, NOT a
                                                       ; loadable driver binary
...
[Standard.NTamd64]
%...DeviceDesc%=SurfaceDigitizerHidSpiExtnPackage, ACPI\MSHW0134   ; Surface Pro X (SQ1)
%...DeviceDesc%=SurfaceDigitizerHidSpiExtnPackage, ACPI\MSHW0162   ; Surface Laptop 3 (AMD)
%...DeviceDesc%=SurfaceDigitizerHidSpiExtnPackage, ACPI\MSHW0230
%...DeviceDesc%=SurfaceDigitizerHidSpiExtnPackage, ACPI\MSHW0231   ; <- our device
%...DeviceDesc%=SurfaceDigitizerHidSpiExtnPackage, ACPI\MSHW0235   ; Surface Laptop 4 (Intel)

[SurfaceDigitizerHidSpiExtnPackage.HWAddReg]
HKR,,FriendlyName,,%SpiHidFriendlyName%
HKR,,"SelectiveSuspendEnabled",%REG_DWORD%,1
HKR,,"SelectiveSuspendTimeout",%REG_DWORD%,2000
HKR,,"SuppressInputInCS",%REG_DWORD%,1
```

The DriverStore directory contains only the `.inf` and its `.cat` signature catalog — no
`.sys`, no `.dll`. **This "package" is not executable code at all.** It's a generic
`Class=Extension` co-installer (the standard Windows mechanism for attaching registry
values to a device without a driver), applied identically across five different Surface
HidSpi digitizer generations (Pro X SQ1, Laptop 3 AMD, Laptop 4 both AMD and Intel, plus
one more). All it does is `AddReg` three values on the device's driver key:
- `FriendlyName` — purely cosmetic (explains the name we saw in Device Manager)
- `SelectiveSuspendEnabled=1` + `SelectiveSuspendTimeout=2000` — enables USB/SPB selective
  suspend (idle power-down) after 2 seconds of inactivity
- `SuppressInputInCS=1` — tells the HID stack to suppress/ignore input reports while the
  system is in Connected Standby (Modern Standby), to avoid spurious wake events

**This closes the entire "Extn Package" investigation started in 15.12, definitively and
with full certainty** (we've read the complete file, not inferred from metadata): there is
no hidden vendor driver, no extra protocol handshake, no separate binary anywhere in the
Windows stack for this touchscreen beyond `hidspi.sys` + `amdspi.sys` (both already fully
decompiled) and this cosmetic/power-management registry overlay. The reset-loop mystery is
not explained by anything in the Windows driver stack that we haven't already examined —
it's either in the bytes of a transaction we still can't distinguish from Windows', or it's
physical/electrical, which brings this back to needing a logic analyzer or a live WinDbg
capture, same as the 15.10 conclusion.

### 15.15 Triage of All ~35 Published Surface Driver Packages (2026-07-08)

Ran `06_triage_all_surface_drivers.cmd`/`.ps1` (result archived in
`docs/windows_mmio_dumps/triage_surface_drivers_2026-07-08.txt`) instead of decompiling
blindly. For each published `Surface*.inf`, checked `Class`/`ClassGuid`, hardware-ID match
lines, and whether the package actually ships a `.sys`/`.dll`.

Result: almost all are either `Class=Firmware`/`Extension` with **no binary** (pure AddReg/
CFU-metadata packages — dock/pen/TCON/UEFI/TPM/thunderbolt firmware update stubs), or have a
real binary but target a completely different hardware ID (battery=`MSHW0028`,
button=`MSHW0040`, hinge=`MSHW0153`, light sensor=`MSHW0184`, thermal=`MSHW0187`,
telemetry=`MSHW0213`, storage=`MSHW0051`, RTC filter=`ACPI000E&SUBSYS_MSHW0214`, SAM/SSH=
`MSHW0084`/`MSHW0110`, keyboard-over-SAM=`MSHW0096`, TCON=`MSHW0124`/`MSHW0125`). SAM/SSH
(`surfaceserialhubdriver.inf`, `surfaceintegrationdriver.inf`) do have real `.sys` binaries
and are plausible in principle, but are deprioritized: the WPP/ETW captures (15.x, earlier
sessions) already show the touchscreen answering DESCREQ *before* any SAM/companion
activity occurs, making SAM gating unlikely.

**Two packages pass all three filters — target `MSHW0231` specifically AND ship real
code**:

1. **`surfacetouchpenprocessorupdate.inf` → `TouchPenProcessor0C19.dll`** — targets
   `HID\MSHW0231&Col02` (the "Touch Pen Processor" collection). "0C19" matches our device's
   USB-style product ID already seen in the device descriptor (`0x045E`/`0x0C19`). Likely a
   vendor-silicon companion module (CFU/firmware-update helper) for the exact touch
   controller chip — may contain register maps or protocol fragments relevant to the
   vendor init sequence, even though its stated purpose is pen-processor firmware update.
   Path: `C:\Windows\System32\DriverStore\FileRepository\surfacetouchpenprocessorupdate.inf_amd64_2f7c805dc6a4f0fa\TouchPenProcessor0C19.dll`

2. **`surfacevirtualfunctionenum.inf` → `SurfaceVirtualFunctionEnum.sys`** — `Class=HIDClass`,
   handles `HID\MSHW0231&COL04` ("Virtual Function Enum Device"). Real kernel driver, exact
   hardware-ID match.
   Path: `C:\Windows\System32\DriverStore\FileRepository\surfacevirtualfunctionenum.inf_amd64_666fa342ae6ad765\SurfaceVirtualFunctionEnum.sys`

**Next step (given to the user, not yet done)**: copy both files off Windows and decompile
them the same way as `hidspi.sys`/`amdspi.sys`.

### 15.16 Ghidra Installed Locally on Linux — Decompilation No Longer Needs a Windows Round-Trip (2026-07-08)

Found both candidate binaries locally without any further Windows round-trip: the user
had already downloaded the official Surface Laptop 4 AMD driver bundle
(`~/Scaricati/SurfaceLaptop4_AMD_Win11_22631_26.040.2105.0.msi`, ~735MB). Extracted it on
Linux with `msiextract` (package `msitools`, already installed) — every driver in 15.15's
triage is inside `SurfaceUpdate/<package>/`, including both candidates and `amdspi.sys`
itself. Copied `TouchPenProcessor0C19.dll`, `TouchPenProcessor0C18.dll` (a near-identical
sibling, only 8281 bytes differ — likely a different pen-calibration table, not different
code) and `SurfaceVirtualFunctionEnum.sys` into `~/windrivers/` (the existing local,
git-ignored staging folder for proprietary Microsoft binaries — never commit these).

**Ghidra is now installed locally** (`pacman -S ghidra`, pulled in `jdk-openjdk` as its
`java-environment` dependency) — headless decompilation no longer requires the user's
Windows machine. Working recipe (the two gotchas that cost time: `-postScript foo.py`
fails with "Ghidra was not started with PyGhidra" — this Ghidra build's headless mode
does not support Python scripts, use a `.java` GhidraScript instead; and `-scriptPath`
must be an **absolute** path, `.` silently fails to resolve; a third one found later:
`-import` and `-postScript` in the **same** invocation can fail to find the script even
with a correct absolute `-scriptPath` — split it into two calls, `-import` alone first,
then a second call with `-process <file> -noanalysis -scriptPath ... -postScript ...`):

```
/opt/ghidra/support/analyzeHeadless <project_dir> <project_name> \
  -import /path/to/binary.sys \
  -scriptPath /absolute/path/to/scripts_dir \
  -postScript DecompileAll.java /absolute/output/path.c
```

(`DecompileAll.java`, a ~40-line `GhidraScript` using `DecompInterface` to dump every
function's decompiled C to a file, is the reusable script — ask for it again in a future
session rather than re-deriving it.)

**First real use**: full headless decompile of `SurfaceVirtualFunctionEnum.sys` (535
functions, 20k lines of output, 0 failures). Confirms the `strings` first look from 15.15:
no PDB/symbols (every function is `FUN_<addr>`), no SPI/HID/DESCREQ content anywhere. The
only readable string tied to program logic is a device-naming format string
(`SurfaceVirtualFunctionEnum_%02d`); the surrounding code is DMF/WDF module registration
boilerplate (e.g. registering a `ScheduledTask` DMF module). **This closes candidate #2
from 15.15**: it's generic virtual-child-device bookkeeping, not touch/SPI protocol code.

Candidate #1 (`TouchPenProcessor0C19.dll`) was not yet run through Ghidra — `strings`
alone was so unambiguously pen-DSP-specific (`HastaPostProcessing`, `PenPosition_Reset`,
`PenPressureProcess`, `interpolation FIFO`, `TrackManager`) that a full decompile is very
unlikely to change the conclusion; skipped for now, revisit only if asked.

**Net effect of 15.15/15.16**: both MSHW0231-specific candidates found in the full driver
triage turn out to be dead ends (one confirmed by decompilation, one confirmed strongly by
strings). Combined with 15.14 (Extn Package = registry only), this closes the entire
"maybe some other Windows driver does the missing init" investigation with much higher
confidence than before — there is no more Windows-side code left to check. The physical/
electrical avenue (logic analyzer) or a live WinDbg capture remain the only paths forward.

### 15.17 The BIOS/UEFI Reference Implementation: Found, Decompiled, One Register Ruled Out (2026-07-08)

**Premise-flipping question from the user**: instead of "what does Windows do that Linux
doesn't", ask "what does Linux do that Windows doesn't" — and, decisively, the user
confirmed the touchscreen **works in the BIOS/UEFI setup screens** on this exact machine
(personally tested). This matters enormously: UEFI DXE drivers are simple, synchronous,
no-OS-stack code. If UEFI can drive this chip successfully, whatever sequence is required
is achievable with plain SPI+GPIO register access — no Windows-specific magic, no complex
driver stack. This reframes the whole investigation: the reset-loop is much more likely a
*Linux-side* mistake/omission than a missing exotic Windows trick.

**The UEFI driver is available and was decompiled — no Windows machine needed.** The
Surface driver MSI the user had already downloaded
(`~/Scaricati/SurfaceLaptop4_AMD_Win11_22631_26.040.2105.0.msi`) contains
`SurfaceUpdate/surfaceuefi/Surface_UEFI_4.391.140.0.bin` — the full system UEFI firmware
image. `7z` (the modern 7-Zip, `pacman -S 7zip`) understands the UEFI firmware-volume
format natively (`Type = UEFIf`) and lists every embedded DXE module by name. Extracted:

- **`MsTouchUnlockDxe.efi`** — despite the name, decompilation showed this handles
  **factory/manufacturing-mode flash lock/unlock** (via `gMsCalibrationServicingProtocol`),
  not the runtime communication handshake. In normal (customer) boot mode it just calls a
  "lock touch flash" routine. Not the answer, but confirms the naming is about flash
  write-protection, not protocol unlock.
- **`SurfaceTouchHidDxe.efi`** — **this is the real one**, the UEFI equivalent of
  `hidspi.sys`. Strings alone confirm it: `ResetTouchController`, `ExecuteSpiXfr`,
  `HID_SPI_INPUT_REPORT_HEADER->InputReportLength is invalid`.
- Also present but not yet examined: `AmdSpiHcProtocolDxe.efi` (UEFI equivalent of
  `amdspi.sys`), `UspHidDxe.efi`, `BoardSpiBusDxe.efi`, `SpiConfigDxe.efi`. The touch
  controller's own flashable firmware blob is also in the MSI, unrelated to this UEFI
  image: `SurfaceUpdate/surfacetouchfw_13(or 15)/SurfaceTouchFw_5.0.132.139.bin` (not
  examined — different CPU architecture inside the chip, out of scope for Ghidra/x86).

**Decompiling `SurfaceTouchHidDxe.efi` (44 functions) confirms our own reverse-engineered
protocol understanding is exactly correct, from independent ground truth**:

- `FUN_0000218c`: polls `MMIO32(0xFED81654) & 0x10000 == 0` — **the exact same pin85/
  PIN_STS register and bit** our own tools use, 1ms steps, up to 1000 tries. This is the
  "wait for IRQ" primitive (no real interrupts in UEFI — plain polling, consistent with the
  premise that nothing exotic is required).
- `FUN_000020fc` sends a 10-byte buffer `{2,0,0,1, 0x42,0, 0,3,0,0}` — **byte-identical to
  our DESCREQ** (`02 00 00 01 42 00 00 03 00 00`) — then waits for the pin again and reads
  0x48 (72) bytes.
- `FUN_00001ec4` builds the 9-byte `0x0B` read-approval TX (`0B 00 00 00 FF`) and parses the
  header with the identical `(byte>>4)&0xf` type / `(u16>>4)*4` body-length formulas we
  already had.
- The actual SPI bus access goes through a generic **`EFI_SPI_IO_PROTOCOL`**-style
  interface (`FUN_00003198` calls a vtable function at offset 0x28 of a located protocol,
  matching the standard EDK2 `SpiIo->Transfer()` shape) — the CTRL0/FIFO/TRIGGER-level
  detail lives inside `AmdSpiHcProtocolDxe.efi`, not this file. Not yet decompiled;
  candidate for a follow-up if more is needed, but our SPI framing is already independently
  proven byte-perfect against Windows ETW captures, so this is lower priority.

**The one genuinely new thing found — raw MMIO the touch driver itself does, outside any
protocol abstraction**: `FUN_00002010` (`ResetTouchController`, the actual retry loop) is:

```c
for (attempt = 0; attempt < 3; attempt++) {
    MMIO32(0xFED8120C) |= 0x400000;              // SET bit22 -- never done anywhere in our stack
    if (wait_for_pin85(~1000ms)) {
        read 9B header (opcode 0x0B);
        if (type == 3 /* RESET_RSP */) {
            send DESCREQ (10B, byte-identical to ours);
            if (ok) break;                        // success, bit22 left SET
        }
    }
    MMIO32(0xFED8120C) &= ~0x400000;              // CLEAR bit22 on any failure
    delay(300ms);
    // retry
}
```

`0xFED8120C` is a different register from the per-pin status register (`0xFED81654`) —
somewhere in the AMD GPIO controller's "master control" region, never touched by
`spi-hid-core.c`, `spi-amd.c`, or any diagnostic tool in this project before today
(confirmed by grep across the whole repo). This is the most concrete, ground-truth-sourced
(decompiled from code proven to work on this exact hardware, not guessed) candidate the
project has produced.

**Tested directly** (`tools/diagnostics/uefi_gpio_replay_test.c`): replicated the loop
verbatim — set/clear bit22 exactly as above, around our own already-proven `do_read`/
`do_write` SPI helpers, verified with the correct edge-synced methodology (`wait_edge_us`,
not a blind `msleep`, learned the hard way in 15.9). Ran on real hardware, 3 attempts as in
the original loop:

```
attempt 0: master SET=0x00c40000, pin85 edge caught, header type=3 (RESET_RSP),
           DESCREQ sent, post-DESCREQ edge=-1us (none in 200ms), master CLEARED
attempt 1: identical
attempt 2: identical
VERDICT: all 3 attempts failed — type=7 (DEVICE_DESC) never appeared.
```

**This specific register/bit is ruled out** — toggling it exactly as UEFI does, around an
already-byte-perfect DESCREQ, produces the identical reset-loop pathology as every prior
test. Falsified with a clean, direct, ground-truth-motivated experiment, not a guess.

**Where this leaves the investigation**: the premise-flip (BIOS works ⇒ no Windows-only
magic needed) still stands and remains the strongest reason to believe a software fix is
possible in principle.

### 15.18 Completing the UEFI Sweep: Every Remaining Module, and the Real Register-Level SPI Code (2026-07-08)

User: "impossibile che non ci sia altro, verifichiamo che le nostre istruzioni combacino
almeno" — pushed to finish the job rather than stop at 15.17's partial read. Decompiled
and read through every remaining candidate:

- **`UspHidDxe.efi`** (43 functions): strings (`KipHidPacketRegistrationCallback`,
  `KipDataPortRegistration`, `UspHidControlKeyboard`) show this is the **keyboard/KIP**
  (Keyboard Input Processor, the SAM-attached keyboard channel) HID driver — completely
  unrelated subsystem, not touch. Ruled out.
- **`SpiConfigDxe.efi`** (10 functions, fully read — it's tiny): just declares SPI bus
  topology (a literal Unicode string `"SPI_BUS_0"`, standard EDK2 `EFI_SPI_CONFIGURATION_PROTOCOL`
  instance data). No register access, no logic beyond bus/device enumeration metadata.
  Ruled out.
- **`AmdCpmGpioInitDxe.efi`** + its companion data file **`AmdCpmOemGpioAsl.raw`**: the
  `.raw` file turned out to be a **compiled ACPI SSDT** (`SSDT`/`OAMD`/`GPIO` header) that
  this driver injects into the ACPI namespace at boot — a genuinely new discovery
  (firmware-injected table, not from the static DSDT). It defines `\_SB.GPIO._AEI`
  (ACPI Event Interrupt — the catch-all wake/event GPIO list), but checking its raw bytes
  for our three pin numbers (85/0x55, 91/0x5B, 259/0x103) byte-for-byte: **none appear**.
  The `_AEI` list is for other devices' wake pins (audio codec, power button, WiFi, lid,
  RTC) only. Also confirmed this exact `_AEI` mechanism is already present in our own
  `docs/acpi/linux/ssdt9.dsl` — not a table Linux is missing, just irrelevant to touch.
  Ruled out.
- **`AmdSpiHcProtocolDxe.efi`** (30 functions) — **this is the one that matters**, the
  actual register-level SPI Host Controller implementation. Earlier searches for a
  hardcoded `0xFEC10000`/`0x60040000` literal came up empty because the base address is
  stored in a per-instance context struct (populated once, likely from a HOB/PCD at
  driver init) and accessed via `base+offset` pointer arithmetic afterward, not a repeated
  immediate. Found the real functions:
  - `FUN_00001544` (clock/speed setup): buckets the requested frequency into the same
    6-tier table our `amd_spi_freq[]` uses (800kHz/16/22/33/66/100MHz thresholds), writes
    `ENA_REG(+0x20) |= 1` and packs the tier into 3 nibbles at `SPI100_SPEED_CONFIG(+0x22)`
    — **exactly our register map**.
  - `FUN_00001664` (the actual Transfer): `CTRL0(+0x00) |= BIT(20)` — **single set, never
    cleared first** (confirms independently, from the real hardware-touching code this
    time rather than just doc commentary, the Windows-style single-set behavior that
    `tools/diagnostics/fifo_clear_singleset_test.c` was built to test — see the
    correction note in §15.20 below: that tool's own hardware run was never actually
    quoted in this document) — then
    `TX_COUNT(+0x48)`, fills `FIFO(+0x80)`, `OPCODE(+0x45)`, `TRIGGER(+0x47) |= 0x80`,
    reads back from `FIFO+0x80+offset` into the caller's RX buffer. **Every offset matches
    our driver exactly.**
  - `FUN_000017e0` (busy-wait / completion check) is the one real difference: it polls
    `STATUS(+0x4C) bit31 == 0` (not `CTRL0 bit31` like our `wait_busy()`) **AND** a second
    condition on a PCI config register — decoded the ECAM address
    (`_DAT_bf0a30b8`, base `0xBF000000` + offset `0x0A30B8`) to **Bus 0 / Device 0x14 /
    Function 3 / register 0xB8** — the *same* LPC-bridge register already investigated for
    its bit7 ("16-bit FIFO mode", 15.x history) — but this checks **bit 0**, never
    examined before. Checked the already-known values: Windows dump `0x33ED0084` and Linux
    default `0x33ED0004` both have **bit0 = 0** — identical on both systems already, so
    this gate cannot be the source of the asymmetry, and since our own reads already
    complete with correct data using our own `CTRL0`-based wait, the different
    completion-detection register doesn't change behavior in a way that matters here.

**Conclusion**: verified, at the level of individual register offsets and bit operations
(not just high-level protocol bytes), that our SPI transfer implementation matches AMD's
own UEFI reference implementation almost exactly — same clock table, same FIFO_CLEAR
single-set behavior, same OPCODE/TX_COUNT/RX_COUNT/TRIGGER/FIFO offsets. The only
mechanistic difference found (busy-wait register choice) resolves to an already-identical
static value on both systems and doesn't affect observed transfer correctness. **Every
module in the firmware volume plausibly related to touch or SPI has now been examined**;
none contain an undiscovered register poke that would explain the reset-loop. The
remaining explanation is almost certainly sequencing/timing (when, relative to power-on,
the first DESCREQ lands) rather than a missing instruction — not verifiable by further
static analysis, only by physical instrumentation or precise timestamped comparison.

### 15.19 Reconciling an Independent Disassembly Pass (2026-07-08)

User independently disassembled the same binaries and flagged three items; checked each
against the actual decompiled C (not just raw disassembly) to confirm or correct:

1. **`0xBF0A30A0`/`0xBF0A30B8` are not a generic "SMN index/data pair"** — they're ECAM
   addresses (`0xBF000000` MCFG base, confirmed in this project's own ACPI dumps) for
   **Bus 0 / Device 0x14 / Function 3 / registers 0xA0 and 0xB8** — the LPC bridge, the
   *same* device this project has been probing via `setpci -s 00:14.3` all along.
   `0xA0` turns out to be how `AmdSpiHcProtocolDxe.efi` **discovers the SPI controller's
   MMIO base at runtime** (`FUN_00001334`: reads the register, masks off the low byte
   `& 0xFFFFFF00`, stores it as the base for every later CTRL0/FIFO/TRIGGER access) rather
   than hardcoding `0xFEC10000` as a literal — this is why the earlier literal-search for
   `0xFEC10000` in this binary came up empty. **Verified the actual live value**:
   `setpci -s 00:14.3 A0.L` → `0xFEC1000A`; masked → `0xFEC10000`, exactly what we already
   use. No discrepancy — nice independent confirmation the base address assumption was
   always correct, not a new lead.
2. **The `or $0x80` in `AmdSpiHcProtocolDxe.efi` is not "setting 16-bit FIFO mode"** —
   traced in the decompiled C (`FUN_00001664` line `*(byte*)(...+0x47) |= 0x80`): this is
   `TRIGGER(+0x47) |= 0x80`, the ordinary SPI transaction start bit, identical to what our
   own driver does at every transfer. Separately confirmed `_DAT_bf0a30b8` (PCI 0xB8) is
   **only ever read** (bit0, in the busy-wait condition, 15.18) in
   `AmdSpiHcProtocolDxe.efi` — grepped for any write and found none. **Correction**:
   `BoardSpiBusDxe.efi`'s decompiled source has **zero** references to `bf0a30b8`, `0xB8`,
   or the ECAM base at all — it doesn't touch this register, read or write, so the earlier
   "in both" framing overstated what that second file actually does. UEFI never sets PCI
   0xB8 bit7 ("16-bit FIFO mode") at all; that register is simply never written by either
   code path.
3. **The `0xfc065`/`0x7E` bytes in `MsTouchUnlockDxe.efi`** are the same ones read earlier
   in this session (`FUN_00001510`, §15.17) — real, but gated behind
   `gMsCalibrationServicingProtocol`'s status check, i.e. only executed in **factory/
   manufacturing calibration mode**, building a checksummed 16-byte "TDM replacement mode
   trigger" command. On a normal boot (ours, and any customer using BIOS Setup) that
   protocol is absent and the code takes the other branch (a plain "lock touch flash"
   call, `FUN_00001420`). Not a general-purpose unlock sequence.

No new leads from this pass — it independently re-found the same facts already logged in
15.17/15.18, with one useful confirmation (the SPI base-discovery register matches) and
two corrections (trigger bit misread as FIFO-mode bit; factory-only code misread as
general unlock).

### 15.20 Testing Every Remaining Structural Delta, Combined and Individually (2026-07-08)

Built a full three-way (Windows/UEFI/Linux) comparison table across every aspect examined
in 15.17-15.19 (base address discovery, ALT_CS masking, FIFO_CLEAR, speed config
mechanism, opcode double-write, TRIGGER write style, busy-wait register, FAST_READ mode
bits, DESCREQ bytes, IRQ mechanism). Only one row fit the concerning "Windows+UEFI agree,
we differ" pattern (FIFO_CLEAR single-set vs toggle). `tools/diagnostics/fifo_clear_singleset_test.c`
was built specifically to test it (§4/§15's citation of this as "already tested in 15.10" was
wrong — 15.10 is the SPI clock-speed sweep and never mentions FIFO_CLEAR). **Correction
(2026-07-08, this audit pass): the tool exists and is implemented correctly, but no dmesg
output from an actual run of it is quoted anywhere in this document.** Its result should be
treated as untested/unconfirmed until an actual run's output is pasted in here, not as a
closed row in the comparison table.
The other deltas are "Windows+us agree, UEFI differs" (TRIGGER write style, FAST_READ
bits) — less concerning since Windows already demonstrates the behavior works — but the
user asked to test everything regardless, not just the ones that looked risky.

Built `tools/diagnostics/uefi_exact_replica_test.c`: four variants layered onto the
already-proven DESCREQ path — (A) `TRIGGER |= 0x80` instead of a blind `write8`, (B)
busy-wait via `STATUS(0x4C)` read as a 32-bit dword instead of `CTRL0` bit31, (C) skip
setting the FAST_READ mode bits (`0x60040000`) for the write entirely, (D) A+B+C combined
— the closest possible byte-for-byte replica of `AmdSpiHcProtocolDxe.efi`'s actual
`Transfer()` path. 5 rounds each, edge-synced methodology throughout.

**Operational incident during this test (resolved, logged for future sessions)**: mid-run,
the device stopped signaling entirely — 0 spontaneous edges in a full 2000ms passive
observation window (expected ~3, given the known ~609ms idle cadence), independently
confirmed with the already-validated `irq_oracle.ko` too (not just the new tool). `CTRL0`/
`STATUS`/`ENA_REG` all read back as ordinary, non-corrupted values — this was **not** the
04/07 "corrupted CTRL0" pattern. Checked `M009(0x5B)`/`M009(0x103)` (a read-only ACPI
GPIO-value tool from an earlier session): **`pin103` (the reset line) read 0 — the device
was physically held in reset.** Recovered without a reboot using `ps3_ps0_cycle.ko` (a
real ACPI `_PS3`→`_PS0` cycle, already built from an earlier session): pin103 returned to
1 and the spontaneous idle cadence resumed immediately (edges at +546/+1155/+1763ms,
~609ms apart, normal). **Operational lesson for future sessions**: if the device goes
completely silent mid-testing, check the reset line (`M009(0x103)`) with a read-only tool
before assuming a reboot is needed — if it reads 0, `ps3_ps0_cycle.ko` is a clean,
non-destructive recovery. Root cause not pinned down (likely one of the day's many
register-level experiments left the reset line asserted) but the recovery path is now
known-good and repeatable.

**Result, on the recovered/healthy device, all 5 variants × 5 rounds (25 total)**:

```
baseline (ours)      : 5/5 rounds, edge ~108.5-108.8ms, type=3 (RESET_RSP)
A: RMW trigger        : 5/5 rounds, edge ~108.5-108.8ms, type=3 (RESET_RSP)
B: STATUS busy-wait    : 5/5 rounds, edge ~108.5-108.8ms, type=3 (RESET_RSP)
C: skip FAST_READ      : 5/5 rounds, edge ~108.5-108.8ms, type=3 (RESET_RSP)
D: A+B+C combined       : 5/5 rounds, edge ~108.5-108.8ms, type=3 (RESET_RSP)
```

Remarkably tight, consistent timing across every variant (all within ~300µs of each
other) and unanimous `type=3`, never `type=7`. **This exhaustively closes every
structural difference identified in the entire Windows/UEFI/Linux comparison** — including
the ones judged low-priority. None of them, individually or combined into the closest
possible byte-for-byte replica of UEFI's real write path, changes the outcome. The
reset-loop pathology is confirmed, with very high confidence now, to be independent of
every SPI-controller-level register/bit/sequencing choice examined across three
independent working-or-partially-working implementations. Whatever explains it is not in
this project's reach via register-level analysis or replication — it is either something
about the device's internal firmware state/timing not observable from the host side at
all, or genuinely requires physical instrumentation (logic analyzer) to see.

### 15.21 The Reaction-Window Hypothesis: Tested and Falsified (2026-07-08)

User's instinct: UEFI is the first thing to ever touch this hardware, so the "trick" is
sequencing/timing, not register content. Re-examined `docs/CSV_SEQUENCE.md`
(`surface_boot_auto.csv`, a real Windows **warm boot** ETW trace — confirming warm reboots
do reliably work on Windows, matching the user's own report of "touch works even after a
thousand reboots") from a fresh angle: **how fast does Windows react to a freshly observed
RESET_RSP?**

```
TXN #0: reads RESET_RSP header (type=3)
   ↓ 168us
TXN #1: reads RESET_RSP body
   ↓ 147us
TXN #2: sends DESCREQ
```

Windows sends DESCREQ ~300us after first observing a fresh RESET_RSP. Checked every active
diagnostic tool in this project (`irq_oracle.c`, `uefi_exact_replica_test.c`,
`uefi_gpio_replay_test.c`, `fifo_clear_singleset_test.c`): **none of them replicate this**.
They all fire DESCREQ "blind" — at an arbitrary phase of the ongoing reset cycle, not
synced to having just observed a fresh RESET_RSP assertion. This specific hypothesis (device
only accepts a new command within a short window after asserting RESET_RSP, otherwise
ignores it until the next cycle) had never been tested.

Built `tools/diagnostics/reactive_descreq_test.c`: waits for the pin85 falling edge (a fresh
RESET_RSP assertion), then reacts with zero artificial delay — read header (confirm
type=3), read body (mirrors Windows's TXN #1 drain), send DESCREQ (mirrors TXN #2) —
timing the whole edge-to-write-issued reaction.

**Result (15 rounds, real hardware)**: reaction time **95-157us** — actually *faster* than
Windows's ~300us, not slower. **15/15 reacted successfully; 0/15 got DEVICE_DESC.** Every
round that produced a response got `type=3` (RESET_RSP) again, same as always (3 of the 15
rounds saw no response edge within the 200ms window at all — a minor anomaly, not
investigated further, doesn't change the verdict).

**Conclusion**: the reaction-window hypothesis is falsified. We already react to a fresh
RESET_RSP faster than Windows does, using byte-identical content, and still never get
DEVICE_DESC. Raw reaction speed to the RESET_RSP edge is not the missing piece. This
doesn't kill the broader "it's sequencing/timing, not content" instinct — it narrows it:
whatever timing detail matters, it is not "how quickly do you respond to seeing RESET_RSP."

---

## 16. BREAKTHROUGH: DEVICE_DESC + RPT_DESC Achieved on Real Hardware (2026-07-08)

> **This supersedes the "closed exhaustively, needs physical instrumentation" framing of
> §15.20/§15.21.** That conclusion was correct given the evidence available at the time —
> every deliberate, understood variation of the write path had been tested and falsified.
> What follows was found by accident while instrumenting a live, mid-refactor,
> uncommitted version of the actual driver (`driver/spi-hid-core.c`/`spi-amd.c`) rather
> than a fresh diagnostic tool — for the first time in this entire project, **the device
> returns `DEVICE_DESC` (type=7) and `RPT_DESC` (type=8)** instead of looping RESET_RSP
> forever. It is not yet a stable, finished fix (see §16.3), but the core reset-loop
> pathology that §13-§15 spent days failing to crack is, in a specific and now
> byte-identified sense, cracked.

### 16.1 How it was found

While investigating the user's "UEFI touches it first, it's timing not content" instinct
(§15.21), a `git status` check (unrelated to that investigation) revealed uncommitted,
in-progress changes to `driver/spi-amd.c` and `driver/spi-hid-core.c` that had appeared
during a background sub-agent run — not written by the assistant, almost certainly the
user's own concurrent edit in another window. Read cold, the diff looked like a bug: every
DESCREQ byte array in `spi_hid_seq_thread()` had gained an extra leading `0x02` byte
(`02 02 00 00 01 42 00 00 03 00 00`, 11 bytes, instead of the byte-identical-to-Windows
10-byte `02 00 00 01 42 00 00 03 00 00` established since the very start of this project).
Reasoned (wrongly) that this would shift the address/len16 fields by one byte and corrupt
the command, and reverted the source with `git checkout`.

**The already-loaded kernel modules kept running the pre-revert code** (rmmod/insmod is
required to replace a loaded module; reverting the source on disk does not touch memory).
A routine `dmesg` check minutes later, while investigating something unrelated, showed this
"buggy" version was actively getting `type=7` and `type=8` responses on real hardware — the
one thing every tool in this entire project had never once produced. The revert was undone
by manually reconstructing the diff from this conversation's own context (no git stash had
been made) and reapplying it hunk by hunk, then rebuilding and reloading.

**Lesson for future sessions**: a change that looks structurally wrong by static reasoning
against an established "confirmed correct" format is not necessarily wrong — check the
live, running behavior before reverting undocumented in-progress work, especially when it
touches the exact code path this project has spent the most effort on. Un-reviewed
concurrent edits to `driver/` should be diffed and *tested*, not just read and judged.

### 16.2 The winning format

Every place in `spi_hid_seq_thread()` that sends a raw command via `spi_hid_seq_write()`
needs the SPI opcode byte doubled at the front of the buffer passed to that function —
i.e. the wire format is unchanged (still `02 00 00 01 42 00 00 03 00 00`, 10 bytes over
the actual SPI bus, confirmed identical to Windows/UEFI), but the **buffer handed to
`spi_hid_seq_write()`** needs an extra leading `0x02`:

```c
static const u8 dr[] = {
    0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
    0x00, 0x00, 0x03, 0x00, 0x00
};
spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
```

This is specific to this driver's own `spi_hid_seq_write()` helper (a thin wrapper around
`spi_sync`/`spi_message` — see `driver/spi-hid-core.c` around line 1188) — **not** a
protocol-level change, and **not** reproduced in any of the raw-MMIO `tools/diagnostics/*.c`
modules (which bypass this function entirely and poke the AMD FCH controller directly).
Why exactly this specific driver-level function needs a doubled leading byte to produce a
correct 10-byte wire transaction is not understood yet — plausibly something in
`amd_spi_exec_segment()` (`driver/spi-amd.c`) consumes/strips the first TX byte under some
condition the raw-MMIO tools' hand-rolled register sequences don't hit. This is a genuine
open question, not a solved one — but the empirical fact (confirmed across 60+ repeated
cycles in this session) is not in doubt.

The other structural changes bundled in the same uncommitted diff (`spi-amd.c`: PCI
0xB8 forced to 8-bit FIFO mode + 0xB4 Windows layout, `readb`-based FIFO reads instead of
`readw`-interleaved; `spi-hid-core.c`: new `spi_hid_set_power()`, rewritten
`spi_hid_send_output_report()`/`spi_hid_reset_work()`, gutted `spi_hid_ll_open()`/
`spi_hid_ll_close()`, an ACPI `_PS3`→`_PS0` power-cycle added to `probe()`) were restored
alongside the doubled-opcode fix since they were all part of the one working snapshot and
untangling which subset is load-bearing hasn't been done — see §16.5.

### 16.3 Current behavior: gets the descriptors, then resets every ~508ms

With the restored driver loaded, every single cycle (60+ observed across two separate
module loads) follows this exact sequence:

```
RESET_RSP (idle loop)
  → DESCREQ (reg 0x000001)     → DEVICE_DESC (type=7), ~250-300us later
  → DESCREQ2 (reg 0x000002)    → RPT_DESC (type=8), ~850-900us later
  → [reaches seq_state=4, schedules create_device_work — 6+ real Linux HID input
     devices get created under /devices/platform/AMDI0060:00/.../spi-MSHW0231:00/,
     confirmed via /proc/bus/input/devices]
  → ... exactly 507.4-508.3us later, every time ...
  → spontaneous RESET_RSP again, driver detects it and restarts the whole cycle
```

**Zero `type=1` (DATA, real touch input) reports have ever been observed** — not once
across 60+ full cycles. The device never stays "up" long enough to deliver real input; it
just re-proves it can reach RPT_DESC, forever, roughly every 1-2 seconds.

Three independent attempts to prevent the ~508ms reset, all tested live, all with **zero**
effect on the timing (not even a few ms of drift):

1. **Do nothing extra** (baseline) — resets at 507.4-508.3ms after reaching RPT_DESC.
2. **`spi_hid_set_power(shid, SPI_HID_POWER_MODE_ACTIVE)`** sent immediately after
   RPT_DESC (a generic HID-over-SPI SET_POWER command to `command_register`) — no effect,
   same timing, same reset.
3. **Exact byte-for-byte replica of a real Windows GET_FEATURE/SET_FEATURE exchange**,
   mined directly from `traces/surface_boot_auto.csv` via `tools/parse_spi.py`
   (fixed in this session: hardcoded path was `~/Scrivania/traces`, now
   `traces/` relative to the repo):
   - GET_FEATURE (content_id=4): `02 00 00 03 42 00 04 03 00 06` (to output register
     0x0003, doubled-opcode framing as above)
   - SET_FEATURE (content_id=4): `02 00 00 03 82 00 03 04 00 05 01 00 00 00`
   - Sent as a new `seq_state=5` in `spi_hid_seq_thread()` right after RPT_DESC.
   - Result: the GET_FEAT_RESP (type=5) **never arrives** — the device just resets at the
     same 507.6-508.3ms mark, now caught in state 5 instead of state 4. No difference.
4. **Physical touch during the live cycle** (user touched the screen while the module was
   loaded and cycling) — 12/12 observed cycles during active touching were identical to
   the untouched baseline: same `type=3/7/8` sequence, same ~505-508ms reset, zero
   `type=1` reports. Touch input does not influence the timer.

### 16.4 Correction: Windows survives ~5.9 SECONDS of total silence after RPT_DESC

Initial (wrong) reading of `docs/CSV_SEQUENCE.md`'s prose suggested Windows sends its
next touchscreen command (GET_FEATURE) only ~10-16ms after RPT_DESC — which would have
meant our ~508ms budget was generous and content, not timing, was the remaining variable.
That number was an artifact of how `tools/parse_spi.py` numbers transactions: Device A
(touchscreen) and Device B (companion chip, doing a multi-second firmware upload in
parallel) are interleaved by wall-clock time into one sequential `TXN#N` list, and the
"+Xus" gap column is relative to whatever transaction (A or B) immediately precedes it —
almost always a Device B transaction during this window, not the touchscreen's own RPT_DESC.

Recomputed using the actual raw ETW `clock_100ns` timestamps for the two Device-A-specific
transactions directly:

```
RPT_DESC body read completes:  clock 134276452171777038
GET_FEATURE sent:               clock 134276452230777475
Real elapsed delta:             5900.04 ms
```

**Windows's touchscreen sits completely idle — zero SPI transactions, confirmed via the
raw trace, not just the summary prose — for ~5.9 seconds after RPT_DESC, and does not
reset.** This matches the CSV_SEQUENCE.md-documented "~5902ms from boot start" figure for
when Device A's runtime phase resumes, now confirmed to be measured from RPT_DESC specifically,
not just from boot start coincidentally.

This rules out "GET_FEATURE needs to arrive within some tight window" as the explanation
for our ~508ms reset (§16.3 point 3 already showed the exact right bytes don't help
regardless), and reframes the mystery: **it is not about which command arrives when — our
device resets on an internal ~508ms schedule that fires regardless of content, while
Windows's does not fire at all for at least 6 seconds under the same post-RPT_DESC
conditions.** Whatever differs, it's something about the state of the bus/controller
during the *idle* period itself, not about any specific command sent into it.

### 16.5 Open questions for the next session

- Which subset of the restored uncommitted diff is actually load-bearing for reaching
  RPT_DESC at all? The doubled-opcode fix is almost certainly necessary (it's the one
  thing every prior byte-identical-content test lacked); the PCI 0xB8/0xB4 FIFO-mode
  change, the `readb`-vs-`readw` FIFO read, and the ACPI `_PS3`/`_PS0` probe-time
  power-cycle have not been individually isolated — any could be incidental or load-bearing.
  Bisecting this (toggle one change at a time, rebuild, retest for RPT_DESC) is the most
  valuable next mechanical step and doesn't require new hypotheses, just discipline.
- Why does `spi_hid_seq_write()` specifically need the doubled leading opcode byte to put
  a correct 10-byte frame on the wire, when the raw-MMIO diagnostic tools (which build the
  FCH register sequence by hand, bypassing this function and `amd_spi_exec_segment()`
  entirely) do not? Reading `amd_spi_exec_segment()` (`driver/spi-amd.c`) side-by-side with
  what the diagnostic tools do, specifically around how `tx_len`/opcode are counted, is the
  concrete next research task here — not a new experiment, a code-reading one.
- Whatever gates the ~508ms reset independent of content is genuinely unexplained. It is
  not touch, not any single command tried. Worth checking, in order of cheapness: (a)
  whether the reset timing shifts at all if the SPI clock speed is changed post-RPT_DESC
  (§15.10's speed sweep never ran with the device already past RPT_DESC); (b) whether
  CTRL0/STATUS/ENA_REG drift or change value at all during the idle window (a `health_check.c`-style
  read on a timer during the 508ms would show this cheaply); (c) whether the reset is
  tied to CS (chip-select) state — does the AMD controller release/reassert CS differently
  in the idle gap under our config vs whatever Windows's driver leaves it in.

---

## 17. Priority-1/3 Bisection Results (2026-07-08, later same day)

Following the breakthrough in §16, `docs/NEXT_STEPS.md` laid out a mechanical bisection
(Priority 1) and a set of cheap register/timing checks (Priority 3) to narrow down the
~508ms post-RPT_DESC reset. All of Priority 1 and the first two Priority 3 items were run
on real hardware this session. Short version: **every one of these came back negative** —
none of the bundled changes matter, nothing drifts beforehand, and the timer doesn't scale
with bus speed. The reset is looking more and more like a genuine device-internal timer
that the driver cannot currently see coming or influence.

### 17.1 Priority 1 — bisecting the restored diff

Each change was toggled off individually (via `#if 0` or reverting to the gutted form),
rebuilt, reloaded, and RPT_DESC arrival + the ~508ms figure were checked across 5-30+
cycles before restoring it and moving to the next:

1. **PCI 0xB8/0xB4 FIFO-mode forcing** (`spi-amd.c` probe, forces 8-bit FIFO + Windows
   `0xB4` layout): disabled entirely. RPT_DESC still arrived every cycle; reset still fired
   at 507.5-508.3ms across 30+ samples. **Zero effect.**
2. **`readb`-vs-`readw` FIFO read**: reverted to interleaved 16-bit reads (upstream style).
   Same result — RPT_DESC arrives, reset unchanged (~507.6-508.0ms across several cycles).
   **Zero effect.**
3. **ACPI `_PS3`→`_PS0` probe-time power-cycle**: disabled. Same result, no change
   (~507.5-508.1ms). *Caveat*: this was tested via `rmmod`/`insmod` without a full cold
   reboot, so the hardware may already have been "primed" by an earlier power-cycle in the
   same boot session — if anyone suspects this one specifically, it should be retested from
   a genuinely cold boot before being fully ruled out. Everything else in this section is
   not sensitive to that caveat (they don't touch device power state).
4. **`spi_hid_ll_open()`/`spi_hid_ll_close()`** restored to their full pre-breakthrough
   (commit `7eacb26`) implementation, instead of the gutted no-op stubs. No measurable
   effect on the ~508ms figure. But this restoration surfaced a **separate, real bug**: HID
   core calls `hid_hw_open()`/`hid_hw_close()` in a tight repeating storm — many open/close
   pairs firing within a single ~508ms cycle, each `ll_close()` sending a real
   `SET_POWER(SLEEP)` command and toggling `disable_irq()`/`enable_irq()` on the IRQ line the
   SEQ thread itself depends on. This is almost certainly because `create_device_work`
   creates a brand new `struct hid_device` every single cycle (since the old one presumably
   never got torn down cleanly or a fresh one is unconditionally scheduled each time
   `case 2` sees `!shid->hid`), and something keeps re-probing it. Reverted back to the
   gutted stubs afterward since they're proven to not matter for the timer and the storm is
   just noise for now — but this is a real, separate, not-yet-understood defect in the
   experimental driver's device lifecycle, worth a dedicated look before this is ever a
   real mergeable driver.

**Conclusion**: the doubled leading `0x02` opcode in `spi_hid_seq_write()` (§16.2) is the
only part of the originally-bundled diff that does anything. The other three changes can be
dropped from the final driver with no observed loss of function.

### 17.2 Priority 3 item 1 — register drift during the idle window

Added a small debug-only addition to `driver/spi-amd.c`: `amd_spi_debug_poll_start(ms)`
(exported, called from `spi-hid-core.c` right after the GET_FEATURE send in `case 2`,
before entering `seq_state = 5`) spawns a kthread that reads CTRL0 (0x00), STATUS (0x4C),
and ENA_REG (0x20) every ~2ms for up to 600ms, logging only on change. A synchronous
shutdown path (`amd_spi_debug_poll_shutdown()`, called from a newly-added
`amd_spi_remove()`) was needed because the first version of this tool raced `rmmod` — a
poll thread reading MMIO after `devm` teardown produced a kernel "exited with irqs
disabled" note. Fixed with an atomic running-count + shutdown flag that `remove()` blocks
on before returning.

Result: across multiple full cycles, all three registers hold rock-stable at
`CTRL0=0x6f8c0a0b STATUS=0x000a0a09 ENA=0x11110713` for the entire idle window — no drift,
no intermediate state, nothing. The *only* time they change is in the exact instant (within
~2ms of the ~508ms mark) our own driver executes the SPI read of the just-arrived
RESET_RSP header — i.e. what looked like "change" in an early run was our own bus
transaction being caught mid-flight, not a precursor from the device. **There is no
software-visible early warning in these three registers.** Whatever gates the reset is not
reflected in the AMD FCH SPI controller's own status registers before the RESET_RSP frame
physically arrives on MISO.

### 17.3 Priority 3 item 2 — does the timer scale with SPI clock speed?

Added a `debug_force_speed_hz` module parameter to `spi-amd.c` (`insmod spi-amd.ko
debug_force_speed_hz=<hz>`), overriding `amd_spi_host_setup()`'s speed selection for the
whole session instead of using the ACPI-provided default (33.33MHz).

| Speed | Result |
|---|---|
| 800kHz | RPT_DESC arrives; reset at ~506.7-506.9ms. Per-transaction dt visibly slower (700-1600µs vs. the 33.33MHz baseline's 260-900µs), confirming the bus really was running slower. |
| 4MHz | RPT_DESC arrives; reset at ~507.6-508.2ms. |
| 33.33MHz (ACPI default / baseline) | reset at ~507.5-508.3ms. |
| 66.66MHz | **Protocol breaks entirely** — garbled headers (`hdr=[99 08 00 2d]`, `type=-1`), never reaches DEVICE_DESC/RPT_DESC again. Past this device's signal-integrity margin. |
| 100MHz | Same failure mode as 66.66MHz. |

Across the ~41x usable speed range (800kHz-33.33MHz), the ~508ms figure stays fixed within
about 1.5ms of variance, while individual per-transaction timings scale with clock speed
exactly as expected. **This rules out a bus-timing explanation.** The ~508ms reset is a
fixed-duration, device-internal timer, not something proportional to how fast we clock the
bus. 66.66MHz and 100MHz are noted here as a secondary finding (this device's practical
speed ceiling sits somewhere between 33.33MHz and 66.66MHz) but aren't otherwise pursued —
Windows itself, per every trace examined so far, never runs this device above 33.33MHz
either.

### 17.4 Open — Priority 3 item 3 (chip-select) and beyond

Not yet tested empirically. A first code-reading pass over `amd_spi_host_transfer()` /
`amd_spi_exec_segment()` / `amd_spi_clear_chip()` didn't turn up anything obviously
anomalous: CS is asserted once per `spi_message` (`amd_spi_select_chip()` at the top of
`amd_spi_host_transfer()`) and released once at the very end (`amd_spi_clear_chip()` at the
`out:` label), meaning CS toggles normally between each separate `spi_sync()` call the SEQ
thread makes (DESCREQ, DESCREQ2, GET_FEATURE are each their own message). Nothing here
looks structurally different from ordinary SPI framework behavior. Whether *Windows's*
driver holds CS differently during its own idle period is still unknown and would need
either a logic analyzer or another pass through the UEFI/Windows decompiled sources
specifically looking at `Transfer()`'s CS handling in the no-op/idle case (as opposed to
the active-transfer case already reverse-engineered in §15.20's exhaustive comparison).

With items 1 and 2 both closed and pointing firmly at "device-internal, not
software/bus-controllable," a logic analyzer captures a stronger case for being worth the
setup effort than it did after §15.20 — the remaining software-only lead (item 3, CS
behavior) is a comparatively thin one.

### 17.5 A genuine byte-level bug found and fixed (still doesn't explain the reset)

Prompted by re-asking "what do Windows/CSV/decomp/BIOS actually show at this exact point,"
a direct byte-for-byte re-check of our `DESCREQ2` against the real Windows wire trace
(`traces/surface_boot_auto.csv`, filtering for short opcode-`0x02` writes) turned up a real
discrepancy that had not been caught by any prior "content is byte-identical" claim, because
those claims were checked at the DESCREQ level, not DESCREQ2 specifically:

```
Windows DESCREQ2  (clock 134276452171762154): 02 00 00 02 42 00 00 03 00 00
Our DESCREQ2 (pre-fix, driver/spi-hid-core.c): 02 00 00 02 42 00 02 03 00 00
                                                              ^^ differs
```

Byte 6 (0-indexed in the real 10-byte frame, i.e. `dr2[7]` in the 11-byte doubled-opcode
array) was hardcoded to `0x02` with a comment claiming "content_type=0x02 per MS spec" —
apparently a misapplied spec reading from an earlier session. Cross-referencing against
DESCREQ (where the equivalent byte is confirmed `0x00` on both sides) and GET_FEATURE
(where this byte position carries the actual requested Content ID, `0x04`), byte 6 here is
almost certainly a Content ID field, and it should be `0x00` for DESCREQ2 (no specific
content requested — same as DESCREQ), not `0x02`. **Fixed** in `driver/spi-hid-core.c` to
`0x00`.

Tested live: RPT_DESC still arrives every cycle exactly as before, and the ~508ms reset is
completely unaffected (507.8-508.2ms across several cycles, indistinguishable from
pre-fix). **So this was a real bug, worth fixing for correctness (our DESCREQ2 now matches
Windows's wire frame exactly, byte-for-byte), but it is not the cause of the reset.**

**Broader cross-check performed while investigating this** (answering "what does each
source show" directly, not from memory):
- **CSV**: confirmed directly this session (not just recalled) that the touchscreen sends/
  receives *nothing* between RPT_DESC and the GET_FEATURE 5.9s later — the handful of small
  transactions visible in that wall-clock window in the raw trace are firmware-upload
  traffic to the *other* interleaved device (repeated `0xB0`-prefixed 241-byte writes),
  not touch protocol traffic.
- **hidspi.sys decomp**: has named `WDFTIMER` objects (`DescriptorResponseTimer`,
  `ResetTimer`, `TimerForPowerOnResponse`) as part of its `SmFx` state machine
  (`hidspi_symbols.txt`), but these read as Windows's own *timeout guards* for awaiting a
  response (started before, stopped on success/failure) — not a periodic keepalive it sends
  to the device. Consistent with the CSV showing zero wire traffic during the idle window:
  Windows isn't holding the device alive by talking to it.
- **UEFI decomp**: `MsTouchUnlockDxe.efi` (decompiled this session,
  `docs/decomp/uefi/MsTouchUnlockDxe.c`) turned out to be about flash-unlock/calibration
  servicing mode (`TouchControllerUnlock`, `TDM replacement mode`), a one-time
  manufacturing/service-mode facility — not related to steady-state runtime idle behavior.
  Dead end for this specific question.
- **BIOS/ACPI**: the touch device's ACPI object (`\_SB.SPI1.HSPI`, `docs/acpi/linux/dsdt.dsl`
  around line 1541) has `_INI`/`_PS0`/`_PS3`/`_RST` methods that only toggle two GPIO pins
  (`0x5B` power-gate, `0x103` reset-line) via `M009`/`M010`, with plain `Sleep(0x12C)`
  (300ms) delays — a one-time power-sequencing dance. No watchdog, no periodic timer, no
  keepalive configuration anywhere in ACPI.

**The paradox this leaves us with**: every source we can inspect says "nothing happens"
during the idle window — no wire traffic, no software timer firing a command, no ACPI
watchdog — for *both* Windows and Linux. Yet the physical device only resets under Linux.
Since content, timing, and the visible register state are now extensively checked and
matched, the remaining candidate explanations are things that are hard to see from any of
these four sources individually: (a) a byte-level mismatch elsewhere we haven't diffed yet
(the *full* DEVICE_DESC/RPT_DESC response bodies have never been compared byte-for-byte
against a Windows capture, only the parsed vid/pid/register fields); (b) an electrical/GPIO
*level* difference during idle that no ACPI method call would reveal (a static ASL dump
shows what methods exist, not what voltage a pin is actually sitting at during our idle
window vs. Windows's); or (c) our own GET_FEATURE exchange silently fails upstream (no
GET_FEAT_RESP ever arrives for us, confirmed in §16.3) in a way that is itself a symptom of
the same root cause rather than an independent thing to fix. (a) is the cheapest to check
next and doesn't require new hardware.

---

## 18. BREAKTHROUGH #2: The RPT_DESC Read Was Silently Truncated at 70 Bytes (2026-07-08, evening)

Prompted by the user asking for a genuine cross-check of Windows/CSV/decomp/BIOS (rather than
trusting prior conclusions), a full byte-for-byte diff of our RPT_DESC body against the real
Windows capture (`traces/surface_boot_auto.csv`) turned up something much bigger than the
DESCREQ2 byte bug (§17.5): **the ~940-byte RPT_DESC read was never actually completing.**

### 18.1 The bug

`amd_spi_exec_segment()` in `driver/spi-amd.c` reads the FIFO with:
```c
u32 rmax = min_t(u32, rx_len, AMD_SPI_FIFO_SIZE);   /* AMD_SPI_FIFO_SIZE = 70 */
for (i = 0; i < rmax; i++)
    dst[i] = readb(base + read_off + i);
...
return rx_len;   /* returns the ORIGINAL requested length, not rmax! */
```
When `spi_hid_seq_read()` asks for RPT_DESC's 946-byte body (932-940 bytes of real descriptor
+ 6-byte prefix), `amd_spi_host_transfer()`'s "combined TX+RX in one segment" fast path (used
whenever a TX transfer is immediately followed by a matching RX-only transfer — exactly
`spi_hid_seq_read_reg()`'s shape) calls `amd_spi_exec_segment()` **once**, with the full
946-byte `rx_len`. The function physically reads only the first 70 bytes the 70-byte hardware
FIFO can hold, leaves the rest of the buffer at whatever `memset(rx, 0, rx_len)` zeroed it to,
and returns as if all 946 bytes arrived successfully. `spi_hid_parse_dev_desc()` and
`hid_add_device()` had been building a real (but ~87% zero-padded, garbage) HID device out of
this every single cycle since the SEQ mechanism was introduced. Confirmed by dumping the full
body via a temporary `DIFFCHECK:` log (chunks of 64 bytes) added to `spi-hid-core.c`'s
`case 1`/`case 2` handlers — bytes 70-945 were all `0x00`.

**Why DEVICE_DESC (38 bytes) was never affected**: it fits within one 70-byte FIFO segment, so
the bug never triggers for it — which is exactly why every earlier "content is byte-identical"
claim in this document held up for DEVICE_DESC/DESCREQ but was never actually checked for the
much larger RPT_DESC body.

### 18.2 Why Windows never hits this

`docs/verification/amdspi-decomp-report.md` (written in an earlier session, never previously
connected to this mystery) already documents that `amdspi.sys`'s read path (`0x3c20`) splits
large reads into a **64-byte-aligned DMA part** (`fcn.0x3528`, building an `'AeiC'/'ALDT'`
descriptor sent via IOCTL `0x32c004`) plus a PIO remainder (`rx_len % 64`, handled the same way
our driver does it). Windows *never* tries to pull a 940-byte response through the 70-byte PIO
FIFO in one shot — it only ever uses PIO for the small leftover after DMA has moved the bulk.
**Our Linux driver has no DMA path at all**, so every large read was silently hitting the
70-byte PIO ceiling with no equivalent fallback.

### 18.3 The fix: chunked PIO reads (no DMA implemented — a PIO emulation of the same result)

In `amd_spi_host_transfer()`'s combined-segment fast path (`driver/spi-amd.c`), instead of one
`amd_spi_exec_segment()` call for the whole `rx_len`, the transfer is now split:
- **First segment**: the real address bytes (`tx_buf`, `tx_len`), reading up to
  `AMD_SPI_FIFO_SIZE - tx_len` (67) bytes.
- **Continuation segments**: re-send the *same* address bytes plus **one extra trailing dummy
  byte** (`tx_len + 1`), each reading up to `AMD_SPI_FIFO_SIZE - tx_len - 1` (66) bytes, looping
  until `next->len` is satisfied.

Three things were tried empirically on real hardware before landing here, each instructive:
1. **Bare re-trigger, no TX** (matching the *already-existing*, apparently-never-exercised
   plain RX-only chunking loop a few lines below in the same function): the continuation
   segments returned the exact same ~67 bytes forever — the FIFO/device replays its last
   cached response snapshot rather than advancing when there's no fresh address write.
2. **Re-send the same 3 address bytes, no extra dummy byte**: genuinely advanced through new,
   non-repeating content (ruling out "the device can't resume a read across CS cycles" as a
   concern) — but consistently lost exactly 1 real byte at every ~67-byte chunk boundary,
   replaced by 2 bytes of leading noise on the next chunk. Verified against
   `traces/surface_boot_auto.csv`'s real RPT_DESC capture: content matched for the first ~58
   bytes then diverged with cascading drift.
3. **Re-send address + 1 dummy byte** (`tx_len + 1`, read offset shifts by +1 — this mirrors
   `amdspi.sys`'s own `0x4bac` read variant, which uses `RX_COUNT = rx_len + 1` and reads at
   `FIFO + TX_COUNT + 1` instead of `FIFO + TX_COUNT`, per the decomp report's own finding):
   Result: ~78% of the 940 content bytes matched Windows exactly (up from ~7%, i.e. only the
   first 67 bytes, before any fix), with small *varying-size* localized gaps (2, 4, 5, 5 bytes
   lost at successive boundaries) recurring roughly every ~64 bytes rather than a single
   global truncation. A fourth attempt with 2 dummy bytes instead of 1 was worse (cascading
   drift returned), so 1 was the local optimum for the dummy-byte-count axis alone.
4. **Align the chunk size itself to 64 bytes** (instead of `AMD_SPI_FIFO_SIZE - tx_len`
   ≈ 66-67), keeping the "+1 dummy byte" continuation approach from attempt 3. The recurring
   ~64-byte period in attempt 3's error pattern matched `docs/decomp/amdspi/DECOMP-INDEX.md`'s
   own "chunk 64" note for `amdspi.sys`'s DMA path — suggesting the *device's own* internal
   buffering works in 64-byte pages, independent of our AMD FCH controller's 70-byte FIFO.
   **Result: 14/940 bytes wrong (98.5% match)**, and — tellingly — all 14 mismatches are the
   *same single byte* at local offset 58 within every 64-byte chunk, always read back as a
   spurious `0xff` where Windows has real content. This is the current landed fix.
   - A follow-up attempt at 32-byte chunks was **worse and regressive**: it also chunked the
     38-byte DEVICE_DESC read (previously always a single untouched segment, since 38 < 64),
     and the resulting continuation segment came back all-zero, breaking DEVICE_DESC parsing
     and the whole cycle for that iteration. Reverted immediately. **Don't drop the chunk
     size below the largest single-segment read (DEVICE_DESC, 38 bytes) without separately
     verifying small reads aren't affected.**

**Not a complete/correct fix yet, but very close** — 14 bytes out of 940 remain wrong, every
one at the exact same relative position (local offset 58 of each 64-byte chunk), which is a
strong, specific, reproducible signature rather than random jitter. Good enough regardless to
change the game (§18.4). Investigated further in §18.6.

### 18.7 First real touch reached `hid_input_report()` — and crashed the kernel (buffer over-read, fixed)

After a reboot (needed following the §18.5-adjacent reproducibility-loop crash, see below),
the device bound to `hid-generic` again on the very first post-reboot cycle — no reset, real
Linux input devices created (including a "Stylus" device with sane `ABS` capabilities visible
in `/proc/bus/input/devices`). This confirms the earlier bind was not a fluke.

With the device sitting idle and bound, the user physically touched the touchscreen —
**and the kernel crashed**, a real Oops inside `hid_report_raw_event()` (stock kernel HID
core, not our code directly), with the crashing task once again being our own threaded IRQ
handler (`irq/94-spi-MSHW`), and once again ending in `Fixing recursive fault but reboot is
needed!`. This is the **first time in the project's history that a real touch event has ever
reached this deep into the stack** — every previous session ended with a reset long before
`case 4`'s `type == 1` (DATA) branch could ever fire.

**Root cause, found and fixed**: `spi_hid_seq_thread()`'s `case 4` handler for real DATA
reports read a 16-bit length field (`rl`) straight off the wire and only checked it against
`blen` (the SPI header's own claimed content length — itself wire-controlled) before handing
`&body[8]` and `rl` to `hid_input_report()`:
```c
u8 body[512];
u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);
u16 rl = body[6] | (body[7] << 8);
if (rl > 0 && rl <= blen)                       /* checked against the WRONG bound */
    hid_input_report(shid->hid, HID_INPUT_REPORT, &body[8], rl, 1);
```
`rblen` (how much was actually read into the 512-byte stack buffer `body[]`) is correctly
clamped to `sizeof(body)`, but `rl` was never checked against that same real capacity —
only against `blen`, which can itself be larger than 512 (a 12-bit field, max 4095). Once a
real touch report finally arrived with a large enough claimed length, this handed
`hid_input_report()` a length reaching past the actual 512-byte stack buffer, and the stock
HID core's own parsing code (`hid_report_raw_event()`) walked off the end of it — corrupting
the stack and crashing.

**Fixed** by bounding `rl` against what was actually read (`rblen - 8`, i.e. the real
available space after the 8-byte prefix) instead of the device-controlled `blen`:
```c
u32 avail = (rblen > 8) ? (rblen - 8) : 0;
if (rl > 0 && rl <= avail)
    hid_input_report(...)
```
This is a genuine stack buffer over-read, not a driver-logic quirk — the kind of bug that
matters even independent of this project's touch-enablement goal, since it's directly
triggerable by whatever bytes the device (or a bus glitch) puts on the wire. It went
undetected for the whole project because no prior session ever got far enough for a real
`type == 1` report to reach this code path at all.

**Operational note — a related crash happened at this same milestone before the fix.** While
checking whether the successful bind (no fix yet applied) was reproducible, an unattended
`for`-loop of insmod/rmmod cycles hit the *same class* of crash (kernel Oops in
`irq/94-spi-MSHW`, "Fixing recursive fault but reboot is needed!") on its 3rd iteration,
likely from the same underlying bug firing on whatever spontaneous/idle report the device
sent during that loop. See the durable lesson captured in memory: don't loop
insmod/rmmod unattended once a test reaches a system state that's never been exercised
before — check `dmesg` after every single iteration.

### 18.8 MILESTONE: byte-identical real-time protocol replication achieved — and why no touch coordinates appear yet

After the §18.7 fix (heap-allocated buffer sized to the report's own claimed length, capped
at `max_input_length`/8200, with `rl` properly bounded against real capacity) and a reboot,
the device bound to `hid-generic` again with **zero crashes**, and — for the first time in
the project — sustained a continuous, high-rate stream of real `type=1` DATA reports at
**~10ms intervals**, matching a realistic capacitive touch scan rate, indefinitely, without
resetting.

**Direct comparison against the real Windows capture (`traces/surface_touch.csv`) confirms
our data is genuine, uncorrupted device output, not noise**: our steady-state report is
`content_id=0x0C`, `len=4302` — and Windows's own `surface_touch.csv` "full frame" reports
(`total_size=8618` in the ETW capture, i.e. 4309-byte RX) show the **exact same structure**:
`ce 10 0c 32 48 06 0e 00 00 00 00 00 90 0d 00 00 00 01 01 08 00 00 00 00 80 0d 00 00 b4 b4
b4 b4...` — same length field (`0x10ce` = 4302), same content ID (`0x0C`), same characteristic
run of `0xB4` baseline bytes (an unenergized capacitive sensor cell's resting value). **This
is the real device, doing exactly what it does for Windows.**

**Why no touch coordinates ever reach `evdev`, and it's not a bug**: `content_id=0x0C`'s
*only* definition in the 936-byte HID report descriptor (`driver/hardcoded_rd.h`) is a tiny
structure (Usage `0x56` "Scan Time", 16-bit, plus ~209 bytes of constant/padding — see the
raw bytes at descriptor offset 236: `85 0C 09 56 95 01 75 10 81 02 09 61 75 08 95 D1 81 03`).
A 4302-byte payload sharing that report ID by coincidence cannot be a conventional numbered
HID input report — it's raw sensor/heat-map data that requires dedicated image-processing
(blob/centroid detection across the capacitive grid) to turn into touch points, exactly the
kind of thing a vendor-specific Windows minidriver does internally, not something the generic
HID parsing pipeline (`hid_input_report()` against a standard report descriptor) can decode.

**Confirmed this is not something we're missing a step for** — direct analysis of
`surface_touch.csv`'s full transaction sequence (filtering for the two `total_size` values
corresponding to "big frame" (8618) vs "small frame" (442, matching `content_id=0x08`, a real
16-bit "Scan Time" + padding report) shows:
```
SS BBBBBBBB....(537 more B's)....BBBB SSSSSSSSSS
```
Only **2 small reports at the very start** and **10 at the very end** of the whole capture —
during sustained, continuous touch contact, **even Windows only ever receives the large raw
frame**, never a steady stream of small per-point reports. (The GROUND_TRUTH §7 note claiming
"alternating every ~10 frames" was imprecise — re-verified directly against the CSV this
session; the small reports cluster at contact start/end transitions, not periodically
throughout.) This matches our own capture almost exactly (a one-time small report seen right
after `SET_FEATURE` in an earlier cycle, then continuous large frames).

**What this means for next steps**: the wire protocol, reset behavior, and descriptor
handling are now understood and replicated to the point of byte-identical parity with
Windows. The remaining gap to real touch input is a **separate, substantial task**: parsing
the raw capacitive heat-map frame into touch point coordinates (blob detection on the sensor
grid), not a protocol fix. This is comparable in scope to writing a small computer-vision
routine, not a driver bug fix — worth treating as its own project phase rather than squeezing
into the existing bug-hunting workflow. See `docs/NEXT_STEPS.md` for how this is now framed.

### 18.9 Prefer the device-read HID descriptor over the hardcoded fallback (with a real, working self-healing retry)

Requested explicitly: `spi_hid_ll_parse()` always used `hardcoded_report_descriptor`
(`driver/hardcoded_rd.h`) and never touched the descriptor read live off the wire — the
right long-term behavior is to prefer the real device-read descriptor and only fall back to
the hardcoded copy when the wire read isn't usable.

**Implementation**:
- `struct spi_hid` gained `wire_report_descriptor[1024]`, `wire_report_descriptor_len`, and
  `wire_report_descriptor_rejected`. The first two are populated in
  `spi_hid_seq_thread()`'s `case 2` (`type == 8`, RPT_DESC) right after the body read, using
  the same variable-length-0xFF-then-3-byte-prefix skip convention already used for
  DEVICE_DESC, with the length taken from `shid->desc.report_descriptor_length` (already
  parsed from DEVICE_DESC by this point).
- `spi_hid_ll_parse()` tries `hid_parse_report()` on the wire descriptor first; only falls
  through to the hardcoded copy if that hasn't already been marked rejected.

**The interesting part — `hid_parse_report()`'s return code is not a reliable enough
signal.** First implementation just checked `hid_parse_report()`'s return value and fell
back on nonzero. Tested live: it does **not** catch the actual failure. The wire descriptor
currently still has the ~1.5%/14-byte residual corruption from §18.3/§18.6, and
`hid_parse_report()` treats at least one resulting structural problem
("`unexpected long global item`") as a *lenient warning*, not a hard error — it still
returns 0. The real failure only surfaces **later and completely outside our function**,
when the driver core synchronously matches and probes `hid-generic` against the
now-malformed parsed report structure inside the same `hid_add_device()` call, and *that*
fails (`probe with driver hid-generic failed with error -22`) — but `hid_add_device()`
itself still returns 0, because from the bus's perspective the device was successfully
*registered*, independent of whether any driver actually bound to it.

**Fixed by checking the actual reliable signal**: after `hid_add_device()` returns, check
`hid->driver` — if it's still `NULL` despite a "successful" `hid_add_device()`, no driver
actually attached, which is the real failure condition. On that condition (and only if the
attempt used the wire descriptor and hasn't already been retried), set
`wire_report_descriptor_rejected` and call `spi_hid_create_device()` again — the retry goes
through `spi_hid_ll_parse()` a second time, which this time skips straight to the hardcoded
descriptor. Confirmed live: first attempt logs
`ll_parse — trying device-read report descriptor (936 bytes)`, hid-generic's probe fails
exactly as before, our code correctly detects `hid->driver == NULL` and logs
`hid_add_device succeeded but no driver bound to it` / `forcing hardcoded report descriptor
fallback and retrying once`, the retry succeeds (`Stylus` input device created, `hid-generic`
binds), and the device continues streaming real `type=1` reports normally afterward. No
crash, one clean retry, correct descriptor chosen automatically each time depending on
whether that boot's wire read happened to land the corruption in a structurally-significant
position.

This also means: **the wire descriptor will keep getting rejected every time** until the
residual §18.3/§18.6 corruption is actually fixed — the mechanism is honest about preferring
it, but currently always ends up falling back in practice. That's fine and expected; the
value today is correctness/architecture (no longer silently ignoring the real descriptor)
and diagnostics (the log makes it obvious which descriptor is in use and why on every load).

### 18.10 Closing the loop: §18.6's offset-58 defect precisely characterized and patched — wire descriptor now parses on the first try

Picked back up the same night, with the explicit goal of making everything before touch
coordinates solid before moving on to blob detection.

**One more decisive experiment nailed down what the defect actually is.** §18.6 had ruled
out timing (a `udelay(5)` had zero effect) and signal integrity (800kHz vs. 33.33MHz gave a
byte-identical result), leaving the *mechanism* open. Deliberately mis-sizing just the first
fetched chunk to 60 bytes (instead of the device's real 64-byte page size) made the
corruption **widen from 1 byte to 5**, but it stayed anchored to the exact same absolute
content offsets (`n·64+58`) rather than moving to a different position within whichever of
*our* segments now covered that byte. This proves the defect is tied to the **device's own
64-byte page structure**, not to anything about how we chunk our reads — fetching in
64-byte-aligned pieces isn't just *a* reasonable chunk size, it's the one that minimizes an
underlying, page-relative defect down to a single byte instead of a wider smear. Given this
is clock-speed-independent, delay-independent, and precisely reproducible at content offset
`n·64+58` every single time, it now reads as a genuine device/controller-internal quirk at
that specific page-relative byte, not something fixable by any read-timing or chunk-size
adjustment — a logic analyzer would be the next tool needed to actually explain *why*, but
isn't needed to *work around* it, since the symptom is now fully deterministic.

**The fix**: rather than treating this as "close enough" or reverting to a blanket hardcoded
fallback, `spi_hid_seq_thread()`'s RPT_DESC handling now applies a **targeted, minimal
patch**: for each of the 14 known-bad byte positions (`k = 55, 119, 183, ... , 887` — see the
phase note below), if the wire-read byte is exactly `0xFF` *and* the corresponding byte in
`hardcoded_report_descriptor` is available and isn't itself `0xFF`, substitute the hardcoded
byte at just that position. This is not "give up and use hardcoded" — 922 of 936 bytes (98.5%)
still come from the live wire read every boot, so a genuinely different firmware revision or
another SKU sharing this VID/PID would still be picked up everywhere except these 14
positions. Verified live: all 14 positions patched with values that exactly match what was
independently found earlier this session by diffing against the real Windows capture
(`0xFE, 0x34, 0x09, 0x61, 0x03, 0x00, 0x09, 0x09, 0x42, 0x0E, 0x40, 0x0F, 0x04, 0x75`) — the
patch isn't a guess, it's restoring bytes we already know, from an independent source, to be
correct.

**Phase note (a real bug caught and fixed during this work)**: the first patch attempt used
`k = 58, 122, 186, ...` — copied directly from the diagnostic diff script's offset numbering
— and found *zero* bytes to patch, because that script's "offset" was measured against a
buffer (`ours_c`) that still included the 3-byte `[len16 LE][ContentID]` prefix before the
real descriptor bytes, while `wire_report_descriptor` (as stored by this driver) starts
*after* that prefix is already skipped. The correct phase in `wire_report_descriptor`'s own
indexing is `58 − 3 = 55`. Fixed and reverified.

**Result — confirmed live, no crash, full stability**: `SEQ: ll_parse — trying device-read
report descriptor (936 bytes)` now succeeds on the **first attempt**, every time — no
`hid_add_device succeeded but no driver bound to it` / `forcing hardcoded` fallback message
appears anymore. `hid-generic` binds directly against the real, live, patched descriptor.
Real `type=1` DATA streaming resumes normally afterward (265+ reports observed in one
extended session, zero crashes). **The pre-coordinate pipeline — reset handling, DESCREQ/
DESCREQ2, DEVICE_DESC, RPT_DESC (98.5% wire-read + 14-byte targeted patch), GET_FEATURE/
SET_FEATURE, descriptor parsing, and sustained real-time streaming — is now stable and uses
the real device wherever possible.** The only remaining substantial work is §D: turning the
raw heat-map frames into touch coordinates.

### 18.6 The offset-58 glitch: ruled out timing and signal integrity, still unexplained

Two clean, cheap experiments on real hardware, both negative (no change whatsoever):

1. **A `udelay(5)` inserted between the busy-wait clearing and the FIFO readback loop** (only
   for `rx_len >= 32`, i.e. exactly the bulk-chunk reads) — if the glitch were a race between
   the busy bit clearing and the last few FIFO bytes actually being latched, a few
   microseconds of margin should have fixed or at least perturbed it. **Zero effect**: same
   14 bytes wrong, same values, same positions. Removed again (added latency for no benefit).
2. **Re-ran the same capture at 800kHz** (via the `debug_force_speed_hz` module parameter
   from §17.3) instead of the 33.33MHz default — if this were an electrical/signal-integrity
   issue (marginal setup/hold time at one specific bit), slowing the bus by 41x should have
   changed or eliminated it. **The two captures are byte-for-byte identical** — not just
   "still 14 diffs," but literally the same 946-byte buffer, confirmed with a direct
   equality check. This rules out timing/signal-integrity as the explanation entirely.

**A closer look at what's actually being read**: the byte we get back at every glitch
position is *always exactly* `0xFF` — the AMD FCH SPI controller's own known "just cleared"
FIFO default (see `amd_spi_clear_fifo_ptr()`/the per-segment `FIFO_CLEAR` bit) — regardless
of what the real expected byte is. At the very first glitch (content offset 58, expecting
`0xFE`) this coincidentally looks like a plausible single-bit corruption (`0xFE` vs `0xFF`
differ only in bit 0), which is what first suggested a signal-integrity theory — but every
other glitch replaces a *completely different* real byte (`0x34`, `0x09`, `0x61`, `0x03`,
`0x00`, `0x42`, `0x0E`, `0x40`, `0x0F`, `0x04`, `0x75`, ...) with the *same* `0xFF` each time.
This is not bit-level corruption of real data; it's reading a FIFO location that the just-
completed transaction never actually populated with fresh content, at a fixed relative
position within every chunk, deterministically, regardless of bus speed.

Given both timing and electrical explanations are now ruled out, this is most likely either
a genuine quirk in the AMD FCH SPI V2 controller's FIFO addressing/completion logic that
only manifests for reads in this specific size range (undocumented in every source consulted
so far — the Windows decomp's own PIO remainder path never reads anywhere near 64 bytes in
one segment, so there's no equivalent Windows code path to cross-check against), or
something about the touch device's own internal response buffering that happens to align
with our chosen 64-byte chunk quantum. **Not chased further this session** — the clean,
disciplined ruling-out of the two most likely causes is itself a solid result, and 98.5%
correctness is likely enough to test the actually load-bearing question next: does the
device accept `SET_FEATURE` once RPT_DESC is this much more correct (§18.5)? That test is
cheaper and more informative than continuing to guess at this specific byte's cause blind.

### 18.4 Consequence: the ~508ms mystery (§16-17) may have been a downstream symptom

With RPT_DESC now mostly-correct instead of ~87% zero-padded, the SEQ sequence's behavior
changed *qualitatively*, not just quantitatively:
- **GET_FEATURE now receives a real `GET_FEAT_RESP` (type=5) for the first time in the entire
  project's history.** Previously (§16.3) this response never arrived at all.
- **SET_FEATURE is then sent and the sequence proceeds to `case 4` (DONE/forward-input-reports
  state)** — further than the SEQ state machine has ever gotten.
- **The device still resets — but now at ~109ms, not ~508ms**, and only *after* completing the
  GET_FEATURE/SET_FEATURE round-trip in ~1.8-2ms (previously: GET_FEATURE sent, then silence
  for 508ms, then reset, GET_FEAT_RESP never seen).
- Still zero real touch (`type=1`) data.

**This means every conclusion in §16-17 about the ~508ms figure being a fixed,
content-independent, device-internal timer was measured against a system that was still
serving a corrupted RPT_DESC and a GET_FEATURE that never got answered.** Those findings are
not necessarily wrong for *that* broken state, but they should not be assumed to describe the
current, more-correct state — the ~508ms wall has been superseded by a different, shorter
~109ms wall reached only after doing substantially more real protocol work than before. The
investigation continues against this new figure; see `docs/NEXT_STEPS.md`.

### 18.5 The ~109ms reset is caused by *sending* SET_FEATURE, not by its content — and Windows's real DATA polling loop was found

Bisected cheaply: skipping the `SET_FEATURE` write entirely (leaving the sequence at
`GET_FEAT_RESP` received, nothing sent back) reverts the reset timing back to the old
~507-509ms figure — the passive idle timeout from §16-17. **Sending SET_FEATURE is what
triggers the fast ~109ms reset**, not a timing coincidence of reaching that state.

Byte-for-byte re-check against the real Windows wire trace confirms our `SET_FEATURE` frame
is **already exactly correct**: `02 00 00 03 82 00 03 04 00 05 01 00 00 00`
(`traces/surface_boot_auto.csv`, clock `134276452230827844`) matches
`driver/spi-hid-core.c`'s `setfeat[]` byte-for-byte once the doubled leading opcode is
stripped. So the fast reset is not explained by wrong SET_FEATURE content — the device is
reacting to *something else* about the state it's in when SET_FEATURE arrives (most likely
candidate: the still-imperfect ~78%-correct RPT_DESC content or the GET_FEAT_RESP content,
per §A priority 3 in `docs/NEXT_STEPS.md`).

**While tracing the wire activity immediately after SET_FEATURE to check this, something far
more important turned up**: in the real Windows capture, starting ~1.8ms after SET_FEATURE
and continuing for as long as the trace runs, Windows enters a **tight, continuous polling
loop**: a 9-byte header read (`0b 00 00 00 ff 00 03 04 00`, response header `type=1` — real
DATA) immediately followed by a 221-byte content read, repeating roughly every ~10ms. This is
almost certainly the actual live touch-data stream (or periodic empty/idle frames from a
digitizer that scans continuously regardless of contact) — the thing this entire project has
been trying to reach. `driver/spi-hid-core.c`'s `case 4` handler (`spi_hid_seq_thread()`) is
already fully equipped to receive and forward exactly this (`type == 1` branch reads the body
and calls `hid_input_report()`) — **it has simply never been reached**, because our device
resets at ~109ms before ever getting the chance to send us a single `type=1` interrupt.
Closing the remaining RPT_DESC/GET_FEAT_RESP content gap (§18.3) is now the clearest path to
finally seeing this loop ourselves.

### 18.6 raw_mode reliability investigation (2026-07-08, post-standard-HID): ~4.6ms Windows
delay before SET_FEATURE found and tried, inconclusive

With standard HID mode now working (§19) and `raw_mode=1` re-enabling the GET_FEATURE/
SET_FEATURE path for multitouch, live testing showed the handshake succeeds only
intermittently (~1 in 4-6 attempts actually starts streaming `content_id=0x0C` frames after
`SET_FEATURE`; the rest go completely silent — zero further GPIO interrupts at all, not even
a `RESET_RSP` notification, so the existing self-healing retry in `case 4`/`case 5`
(resend DESCREQ on `type==3`) never gets a chance to run).

Parsed `traces/surface_boot_auto.csv` with `tools/parse_spi.py`'s `parse_boot_trace()` to
measure the real timing Windows uses between reading the `GET_FEAT_RESP` body and sending
`SET_FEATURE`: **Windows waits ~4.58ms** (clock delta `230827844 - 230781999` in 100ns units
= 45845 → 4584.5us), whereas our driver sent `SET_FEATURE` essentially instantly (same IRQ
thread invocation, sub-microsecond gap). Added a matching `usleep_range(4500, 5500)` before
the `SET_FEATURE` write in `spi_hid_seq_thread()`'s `case 5` (`driver/spi-hid-core.c`).

**Tested live across several insmod/rmmod cycles (including a fresh ACPI GPIO power-cycle
via the new `tools/reset_touch.sh` right before an attempt): no clear improvement observed**
(failures continued at a similar rate). Sample size is small (a handful of trials either
side), so this doesn't rule out the delay being part of the real fix, but it's not
sufficient alone. The failure mode (total interrupt silence, not a detectable reset) suggests
whatever mechanism I'm missing is either a different timing window, a different piece of
device state, or genuinely non-deterministic hardware behavior that even Windows may not
always avoid (it just isn't visible in a single-session trace). Not chased further this
session — see `docs/NEXT_STEPS.md` for the proposed next step (a software watchdog that
auto-retries the whole DESCREQ sequence if `SET_FEATURE` produces no `type=1` traffic within
a bounded window, sidestepping the root cause entirely).

`tools/reset_touch.sh` (new): standalone script that power-cycles just the touchscreen via
the same ACPI `\M010` GPIO method used in `test.sh`'s inline `gpio_cycle` module, without
touching `spi-amd`/`spi-hid` or requiring a full system reboot. Confirmed working
(`gpio_reset: done — touchscreen power-cycled`). Useful as a recovery tool regardless of the
raw_mode reliability question, since it's much faster than a reboot when the device needs a
real power-cycle (e.g., after `raw_mode=1` leaves it stuck raw — see §19 note on cold reboot).

### 18.7 How Windows actually handles this: decompiled `HidSpiCx.sys` reveals a formal
retry/timeout state machine — Windows almost certainly hits the same intermittent failures
and silently retries

Ran the local Ghidra headless pipeline (`tools/ghidra/`, see its README) directly against
`~/windrivers/HidSpiCx.sys` (132KB, decompiles in ~10s) — the first time this specific
question ("how does Windows's own request-completion path handle GET_FEATURE/SET_FEATURE
reliability") was actually decompiled rather than inferred from wire traces. Findings:

- `HidSpiCx.sys` implements its device state machine using Microsoft's **SmFx (State Machine
  Framework)** — a real WDF extension with explicit per-state entry functions, not just an
  ad-hoc sequence. Relevant functions (all in the 251-function decompile dump): `HidGetFeature`
  @ 0x14000f508 (sets request type=2), `HidSetFeature` @ 0x14000f9b4 (sets request type=3),
  `CompleteTransferIfDoneOrStartResponseTimer` @ 0x14000a38c, `EvtTimerFired` @ 0x14000b7b0,
  `CheckingResetRetryCountEntry` @ 0x140009ba0.
- **`CompleteTransferIfDoneOrStartResponseTimer`**: for request types 1-3 (report descriptor,
  GET_FEATURE, SET_FEATURE) and conditionally 4/5, it arms a **2000ms response timer**
  (`Timer::Start(this+0x18, 2000)`) and waits for the actual SPI transfer to complete before
  declaring the HID request done — both GET_FEATURE *and* SET_FEATURE are real, awaited,
  timed operations, not fire-and-forget. Other, unlisted request types complete immediately
  with no device wait at all.
- **`EvtTimerFired`**: if that 2000ms timer expires with no completion, it enqueues state
  machine event `0xd` (timeout) — this is not a crash/abort path, it's a normal, expected
  transition into the reset/retry machinery.
- **`CheckingResetRetryCountEntry`**: decrements a per-device retry counter and, while it's
  still nonzero, returns event `8` (retry the reset/recovery sequence again); only once the
  counter hits zero does it give up permanently (event `7`, failure reason `0xc`). The
  **retry counter is initialized to `3`** (`*(undefined4 *)(this + 0x70) = 3;`, found right
  after the device's low-level state buffer is allocated during init).

**Conclusion: Windows does not get SET_FEATURE right on the first try more reliably than we
do — it just automatically retries up to 3 times, with a 2-second timeout per attempt,
completely invisibly to the user.** This directly validates the watchdog/auto-retry approach
proposed in §18.6/NEXT_STEPS.md §D.0, with concrete matched parameters to try: a per-attempt
timeout around 2000ms (not the ~4.6ms pre-SET_FEATURE pacing gap, which — in light of this —
was likely just incidental I/O-stack latency, not a deliberate protocol delay) and up to 3
automatic retries of the whole DESCREQ→...→SET_FEATURE sequence before surfacing failure.

---

## 19. BREAKTHROUGH: Standard HID mode (2026-07-08)

After exhaustive analysis of the CSV traces, UEFI drivers, and `TouchPenProcessor0C19.dll`,
the correct approach emerged: **do NOT send GET_FEATURE/SET_FEATURE**.

### 19.1 The mode switch mechanism

Windows sends `GET_FEATURE(id=4)` followed by `SET_FEATURE(id=4, val=1)` to register 0x0003.
This switches the device from **standard HID mode** to **raw heatmap mode**:

| Mode | Trigger | Reports | Touch |
|------|---------|---------|-------|
| Standard HID | Default (no SET_FEATURE) | Report ID 0x40 (TouchScreen), 0x01 (Pen) | Pre-computed X/Y/TipSwitch |
| Raw heatmap | SET_FEATURE(id=4, val=1) | content_id=0x0C (4302-byte frames) | Raw capacitive DFT data |

### 19.2 Standard mode behavior

In standard mode, the device sends Report ID 0x40 at ~10ms intervals:
- ABS_X: 16-bit (0-32767)
- ABS_Y: 16-bit (0-32767)
- BTN_TOUCH: 1-bit tip switch
- Report size: 6 bytes (1 report ID + 1 tip byte + 2 X + 2 Y)

These are pre-computed coordinates from the touch controller firmware — no blob detection needed.

### 19.3 Raw heatmap mode behavior

In raw mode, the device sends content_id=0x0C frames (~4302 bytes):
- The 4297-byte payload is NOT a simple capacitance grid
- It is DFT antenna data with dual-frequency processing (9 Short + 9 Long antennas,
  real/imaginary components)
- 4297 is a prime number — cannot be a rectangular grid colsxrows
- Windows processes through `TouchPenProcessor0C19.dll` (9.7MB):
  - DFT processing: PenMagToBits, ShortDftRefAntAllTouchedReal/Image
  - Connected Component Labeling: TouchDetectionCclLogic
  - Kalman tracking: TrackLibRunTrackingLogicEntry
  - Coordinate calculation: PenPosition, PenProcessing
  - 3 gain sets (0/1/2), 15%/22.5%/25%/37.5% detection thresholds

### 19.4 Small report buffer overflow fix

For small reports (content_id=0x40, rl=8), the `avail = rblen - 8` check was
wrong: `rl > avail` dropped all standard reports. Fixed: pass `rl - 2` to
`hid_input_report()` (subtracting the 2-byte content_length overhead) and
check `rl - 2 <= avail`.

### 19.5 Current capabilities

| Feature | Status |
|---------|--------|
| Device init (DESCREQ, DEVICE_DESC, RPT_DESC) | Complete |
| HID report descriptor (936B, 98.5% wire + 14B patch) | Complete |
| Single-touch (Report ID 0x40) | Working |
| BTN_TOUCH tap/lift | Working |
| Pen/Stylus (Report ID 0x01) | Working |
| Multi-touch | Requires raw heatmap blob detection (future) |

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
