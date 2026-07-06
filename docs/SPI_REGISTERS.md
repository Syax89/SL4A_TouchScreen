# AMD FCH SPI V2 — Registers and Operation

## MMIO Register Map (base = 0xFEC10000)

| Offset | Name | Size | R/W | Description |
|--------|------|------|-----|-------------|
| 0x00 | CTRL0 | 32bit | R/W | Main control |
| 0x1D | ALT_CS | 8bit | R/W | Alternate chip select |
| 0x20 | ENA_REG | 32bit | R/W | Enable (SPI100, ALT_SPD at bits 20-23) |
| 0x22 | SPI100_SPEED_CONFIG | 16bit | R/W | Speed tiers: NORM(bits 2-0), FAST(5-3), ALT(8-6), TPM(11-9). Windows saves/restores. |
| 0x2C | SPI100_HOST_PREF | 32bit | R/W | Host prefetch config (bit15 = SPI_RD4DW_EN_HOST) |
| 0x44 | SPEED_OPCODE | 16bit | R/W | Windows-only speed config (high byte speed + low byte opcode) |
| 0x45 | OPCODE_REG | 8bit | R/W | Opcode for the V2 path |
| 0x47 | CMD_TRIGGER | 8bit | R/W | V2 trigger (bit7 = start) |
| 0x48 | TX_COUNT | 8bit | R/W | Bytes to transmit. After transfer, bits[11:8] of CTRL0 reflect this value. |
| 0x49 | — | 8bit | — | NOT used by Windows (zero occurrences in amdspi.sys). REMOVED from Linux driver. |
| 0x4A | — | 8bit | — | NOT used by Windows (zero occurrences in amdspi.sys). REMOVED from Linux driver. |
| 0x4B | RX_COUNT | 8bit | R/W | Bytes to receive. After transfer, bits[15:12] of CTRL0 reflect this value. |
| 0x4C | STATUS | 8bit | R | FIFO pointers (RD_PTR bits 23-16, WR_PTR bits 15-8, DONE_BYTE_COUNT bits 7-0) |
| 0x6C | SPEED_REG | 32bit | R/W | V2 speed (spd7 at bits 8-13) |
| 0x80 | FIFO_BASE | 71B | R/W | Data FIFO (71 bytes, 0x80-0xC6 for Cezanne) |
| 0xFC | SPI_MISC_CNTRL | 16bit | R/W | Semaphore: PSP_OWNS(bit10), HFP_OWNS(bit9), HBIOS_OWNS(bit5) |

---

## CTRL0 (0x00) — Full Bit Map

```
Bit 31    : BUSY (read-only)
Bit 30    : SPI_READ_MODE[2] — MSB of 3-bit READ_MODE
Bit 29    : SPI_READ_MODE[1] — middle bit
Bit 28-24 : Reserved / undocumented
Bit 23    : TXMODE — undocumented, not in coreboot headers
Bit 22    : SPI_ACCESS_MAC_ROM_EN
Bit 21    : Function unknown. Survives |= 0x60040000 by omission.
Bit 20    : FIFO_CLEAR — single set in Windows, toggle in Linux driver
Bit 19    : Reserved / auto-set by hardware during transfers
Bit 18    : SPI_READ_MODE[0] — LSB of 3-bit READ_MODE
Bit 17    : Reserved
Bit 16    : EXEC_CMD — V1 trigger
Bit 15-12 : Auto-set by hardware (reflects last RX_COUNT)
Bit 11-8  : Auto-set by hardware (reflects last TX_COUNT)
Bit 7-0   : OPCODE_V1 — opcode for the V1 path
```

SPI_READ_MODE = {bit30, bit29, bit18} → 3-bit value:
- 0 = Normal33M, 2 = Dual112, 3 = Quad114, 4 = Dual122, 5 = Quad144, 6 = Normal66M, 7 = FAST_READ
Windows always uses value 7 (FAST_READ) = 0x60040000.

### Observed CTRL0 Values

| Scenario | CTRL0 Value | Significant Bits |
|----------|-------------|-------------------|
| TX write (0x0B) | 0x6fac100b | 30+29+21+20+opcode=0x0B |
| RX read (0x0B) | 0x6f8c080b | 30+29+opcode=0x0B, auto-set bits 15-12 |
| After setup_v2_regs | 0x6fac0400 | 30+29+21+18 (base) |

### Bit Detail

**SPI_READ_MODE (bits 30+29+18):** Documented in coreboot `amdblocks/spi.h`.
3-bit field selecting the SPI read mode. Windows always sets value 7 (FAST_READ = 0x60040000).
Not "secret" — standard AMD FCH SPI controller feature.

**Bit 23 (TXMODE, 0x00800000):**
- When SET: the controller forces the MOSI line active for ALL opcodes
- Needed for opcodes like 0x00, 0x02, 0x70, 0xB0 (the controller doesn't know these are writes)
- For opcode 0x0B: the controller has special hardware that handles auto-read
- **PROBLEM:** with TXMODE=1, non-0x0B opcodes can't do RX (the controller stays in TX)
- Windows **NEVER** sets TXMODE (it uses a different mechanism)
- Only truly needed for the 0xB0 firmware opcode

**Bit 21 (0x00200000):** Function unknown. Windows does `ctrl0 |= 0x60040000`, so bit21 survives
purely by omission (is never cleared or set explicitly). It is not deliberately "preserved."

**Bit 20 (FIFO_CLEAR, 0x00100000):**
- Windows driver uses a **single set**: `ctrl0 = read32(CTRL0); ctrl0 |= BIT(20); write32(CTRL0, ctrl0)`
- The toggle procedure (clear→set) was a Linux-side discovery — if the hardware requires a
  0→1 transition, both approaches may work depending on whether the bit was already 0.
  ```
  // Windows style (single set only):
  ctrl0 = read32(CTRL0); ctrl0 |= BIT(20); write32(CTRL0, ctrl0);

  // Linux style (explicit toggle, belt-and-suspenders):
  ctrl0 = read32(CTRL0); ctrl0 &= ~BIT(20); write32(CTRL0, ctrl0);  // clear
  ctrl0 |= BIT(20); write32(CTRL0, ctrl0);  // set (rising edge)
  ```

**Bit 18 (0x00040000):** LSB of SPI_READ_MODE. Part of the 3-bit read mode selector. Always set by Windows (FAST_READ).

**Bit 16 (EXEC_CMD, 0x00010000):**
- Trigger for the V1 path
- **Requires a 0→1 transition** like FIFO_CLEAR
- Correct procedure: clear, then set

**Bits 15-12:** Auto-set by the hardware based on the value written to RX_COUNT (0x4B). Should not be modified manually.

---

## V1 vs V2 Path

### V1 Path (Original)
```
1. Write opcode into CTRL0[7:0]
2. Write TX_COUNT (0x48)
3. Fill FIFO (0x80+)
4. Write RX_COUNT (0x4B)
5. Trigger: toggle CTRL0 bit16 (EXEC_CMD)
6. Busy wait: poll CTRL0 bit31
7. Read RX from the FIFO
```

### V2 Path (Windows — amdspi.sys)
```
1. Write opcode into OPCODE_REG (0x45)
2. Write TX_COUNT (0x48)
3. Fill FIFO (0x80+)
4. Write RX_COUNT (0x4B)
5. Trigger: CMD_TRIGGER (0x47) bit7
6. Busy wait: reads STATUS (0x4C) as a byte, tests bit31 (no-op)
7. Read RX from the FIFO at offset 0x80 + TX_COUNT

NOTE: Windows does NOT write to 0x49/0x4A. Zero occurrences in amdspi.sys.
```

### V2 Path (Linux — spi-amd.c, our driver)
```
1. Write opcode into OPCODE_REG (0x45)
2. Write TX_COUNT (0x48)
3. Fill FIFO (0x80+)
4. Write RX_COUNT (0x4B)
5. Trigger: toggle CMD_TRIGGER (0x47) bit7
6. Busy wait: poll CTRL0 bit31 (ioread32)
7. Read RX from the FIFO (at offset 0x80 + TX_COUNT)

NOTE: The Linux driver no longer writes to 0x49/0x4A (removed — not used by Windows).
```

### Key Differences
| Aspect | V1 | V2 (Windows) |
|---------|----|--------------|
| Opcode register | CTRL0[7:0] | 0x45 |
| Trigger | CTRL0 bit16 | 0x47 bit7 |
| Speed config | none | ENA_REG (0x20) ALT_SPD + SPEED_REG (0x6C) spd7 |
| Secret bits | none | CTRL0 bits 30,29,18 |
| 0x1D strobe | none | AND 0xFC, OR 0x01

### V2 Path — Exact Order (our spi-amd.c driver)
```
1.  read8(0x1D) → AND 0xFC → OR (cs & 0x03) → write8(0x1D)     [select CS]
2.  Clear FIFO (CTRL0 bit20 toggle: clear→set)                    [Linux uses toggle, Windows uses single set]
3.  Speed config: ENA_REG (0x20) ALT_SPD + SPEED_REG (0x6C) spd7  [amd_set_spi_freq()]
4.  Write opcode to 0x45                                           [single write]
5.  Secret bits: CTRL0 |= bits 30,29,18 → write CTRL0              [ONCE per segment, in amd_spi_setup_v2_regs()]
6.  write8(TX_COUNT, 0x48)
7.  Fill FIFO (0x80+)
8.  write8(RX_COUNT, 0x4B) — 0 for a write, rx_len for a read
9.  Trigger: write8(0x80, 0x47)
10. Wait busy: poll CTRL0 bit31
11. Read RX from the FIFO at offset 0x80 + TX_COUNT
```

**Note:** the Linux driver does NOT write the opcode twice and does NOT save/restore 0x22 and 0x44. Saving
0x22 and the 0x44 speed config are Windows-only behaviors. The double opcode write (Windows step 9) is
only needed because Windows overwrites 0x45 by writing 0x44 as a 16-bit value.

---

## FIFO Layout

```
Offset 0x80 (FIFO_BASE):
  Positions 0-69: 70 bytes of FIFO

TX write:
  writeb(data[i], base + 0x80 + i)  for i = 0..tx_len-1

RX read (verified against Windows decomp):
  The RX data starts at offset 0x80 + TX_COUNT:
  - For 0x3c20 (TX_COUNT=3):     RX starts at 0x83
  - For 0x4bac (TX_COUNT=rx_len+1): RX starts at 0x84
  The offset is NOT a fixed +4 constant — it depends on the variant.

  Windows 0x4bac uses RX_COUNT = rx_len+1 (one extra byte in TX),
  so the RX reads start at 0x80 + (tx_len that includes the extra byte) = 0x84.
  Windows 0x3c20 uses exact RX_COUNT, so RX starts at 0x80 + 3 = 0x83.

IMPORTANT: with TX_COUNT > 0, the first TX_COUNT bytes are
  written into the FIFO. RX data appears AFTER the TX data in the FIFO
  (at offset 0x80 + TX_COUNT).

---

## ALT_CS (0x1D) — Chip Select

```
Bits 1-0: CS_MASK (0x3) — which chip select is active
Bits 7-2: Reserved / other uses

Windows operation (fcn.0x3c20):
  read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
  This preserves bits 7-2, zeroes CS (bits 1-0), sets bit 0.
  The AND 0xFC/OR 0x01 pattern always forces cs=1 regardless
  of which chip is selected. Bit 0 might be an "enable" strobe,
  not a real CS select.

Our driver (select_chip in spi-amd.c):
  read8(0x1D) → AND 0xFC → OR (cs & 0x03) → write8(0x1D)
  Bits 1-0 are treated as a pure CS select with a 0x3 mask,
  supporting cs=0,1,2,3 depending on the selected SPI chip.
```

---

## GPIO — Interrupt Device

```
Pin: 0x15 (GPIO bank 0)
IRQ type: level-triggered, active-high (flags 0x409)
ISR: invoked by the GPIO Class Extension

Typical sequence:
  1. Host sends an SPI command
  2. Device processes the command
  3. Device lowers (or raises) the IRQ line
  4. GPIO controller detects the interrupt
  5. ISR is invoked (duration ~1-5 µs)
  6. Host performs a 0x0B read to get the response
```

---

## Speed Config (Linux — amd_set_spi_freq())

The Linux driver uses two separate registers:

### ENA_REG (0x20) — ALT_SPD field (bits 20-23)
```
Actual speed set based on dev->speed_hz.
The ALT_SPD field determines the speed tier (4 bits).
```

### SPEED_REG (0x6C) — spd7 field (bits 8-13)
```
Fine speed adjustment (6 bits).
```

### Speed Table (from spi-amd.c lines 127-137)

| speed_hz (target) | enable_val (ENA_REG ALT_SPD) | spd7_val (SPEED_REG) |
|-------------------|------------------------------|----------------------|
| 12.5 MHz          | 0x0                          | 0x04                 |
| 25.0 MHz          | 0x1                          | 0x04                 |
| 33.0 MHz          | 0x2                          | 0x04                 |
| 50.0 MHz          | 0x3                          | 0x04                 |
| 66.0 MHz          | 0x4                          | 0x04                 |
| 80.0 MHz          | 0x5                          | 0x04                 |
| 100.0 MHz         | 0x6                          | 0x04                 |
| 133.0 MHz         | 0x7                          | 0x04                 |

### Register 0x44 (Windows-only)

**WARNING:** the 16-bit register 0x44 is used **only** by the Windows driver (amdspi.sys).
Writing it as 16 bits (writew) also overwrites the opcode at 0x45, which is why Windows
has to re-write the opcode after the speed config. The Linux driver does **NOT** use 0x44.

---

## Status and Busy Polling

```
Windows (amdspi.sys):
  BUSY POLL: read8(0x4C) — reads STATUS as a BYTE, tests bit31.
  Since bit31 of a byte is always 0, this poll is effectively a no-op.
  The real busy indicator is CTRL0 bit31 (not polled by Windows).
  A retry counter prevents infinite spin.

Linux (spi-amd.c):
  BUSY POLL: ioread32(CTRL0), check bit31
  V2 trigger: write8(0x80, 0x47), then poll CTRL0 bit31
  Timeout: 2 seconds (2,000,000 µs), polled every 20 µs
  After the trigger: udelay(20) before the first poll
```

---

## Summary of Fixes Applied to Our Driver

| Fix | Register | Before | After |
|-----|----------|-------|------|
| FIFO clear | CTRL0 bit20 | `setclear` (single write) | clear→set toggle (Linux-style; Windows uses single set) |
| V2 trigger | 0x47 bit7 | CTRL0 bit16 | 0x47 bit7 toggle |
| Busy poll | — | switched by version | unified on CTRL0 bit31 |
| RX offset | FIFO | `fifo_pos + tx_len` | `0x80 + TX_COUNT` |
| RX_COUNT | 0x4B | `+1` for 0x0B | exact `rx_len` |
| Strobe V2 | 0x49, 0x4A | `writeb(0x00)` to both | **REMOVED** (zero occurrences in amdspi.sys) |
| V1 trigger | CTRL0 bit16 | `setclear` | clear→set toggle |

---

## PCI Config Space — FCH LPC Bridge (1022:790e, device 00:14.3)

These registers live in the LPC bridge's PCI configuration space and affect
the behavior of the Cezanne SPI controller. **They are NOT SPI MMIO registers** — they are
accessed via PCI config space (setpci / RWEverything).

| Offset | Size | Name | Windows Value | Linux Default | Writable | Description |
|--------|------|------|---------------|---------------|------------|-------------|
| 0xB4 | 32bit | FIFO_LAYOUT | `0x7DFFE000` | 0x00000000 | YES | FIFO data layout / sync byte count |
| 0xB8 | 32bit | FIFO_ACCESS | `0x33ED0084` | 0x33ED0004 | YES | bit7=1: 16-bit FIFO access mode |

> **Correction (2026-07-06 night)**: the Windows value for 0xB4 was previously mis-transcribed
> as `0x007DFFE0`. The correct value, read byte-by-byte from the raw Windows dump
> (`b4-b7 = 00 e0 ff 7d`, little-endian), is `0x7DFFE000`. Retested with the correct
> full 32-bit values for both 0xB4 and 0xB8 — no effect on the write path (see
> `GROUND_TRUTH.md` §15.1 and §15).

### PCI 0xB8 bit7 — 16-bit FIFO Access Mode

When bit7=1 (the Windows value), the Cezanne SPI controller operates in 16-bit FIFO mode.
Data must be read as 16-bit words (`readw`) and the bytes extracted with this formula:

```c
for (i = 0; i < rx_len; i++) {
    u16 w = readw(base + read_off + (i/2)*2);
    dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
}
```

**Without this fix**, byte-wide reads (`readb`) produce interleaved (high/low) bytes.

### setpci commands to apply the Windows settings

Use the **full 32-bit value** for both registers — a partial value (e.g. `B8.L=0x0084`)
overwrites the entire dword instead of only setting bit7, and was the source of an earlier
invalid test (see `GROUND_TRUTH.md` §15.1):

```bash
# Enable 16-bit FIFO mode (full value, preserves the 0x33ED upper bits)
setpci -s 00:14.3 B8.L=0x33ED0084

# Write the full 0xB4 value
setpci -s 00:14.3 B4.L=0x7DFFE000
```

**Note:** 16-bit FIFO mode fixes the data layout on reads but NOT write
operations (opcode 0x02), which still fail even with these corrected values applied. See
`GROUND_TRUTH.md` §15 for the full, exhaustive close-out of this and every other software
avenue investigated.

### Checking the current value

```bash
# Read 0xB8
setpci -s 00:14.3 B8.L

# Read 0xB4
setpci -s 00:14.3 B4.L
```
