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

## 10. GPIO (from SSDT5 M009/M010)

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

Calling `M010(0x5B,0)` (power off) or any M010 sequence **permanently breaks
the touchscreen until a full system reboot**. After M010, the device stops
sending RESET_RSP and MISO returns only `00 03 00 00 00...`. Verified across
8 different power-cycle sequences. The driver must NEVER call M009/M010.

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
