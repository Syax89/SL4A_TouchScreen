# Complete Verification Plan — MSHW0231 Linux Driver
>
> ## Summary
>
> Systematic verification of **every** component of the Linux driver for the Surface Laptop 4 AMD
> touchscreen, cross-checked against the decompilation of the Windows drivers (`amdspi.sys` + `hidspi.sys`).
>
> **Principle**: Measure a thousand times, cut once. Every change must be verified by
> a sub-agent that cross-checks it against the Windows decomp.

---

## 0. Pre-verification: Hardware Data Collection

### 0.1 ACPI DSDT
- [ ] File: `~/dsdt.dsl` — search for `MSHW0231` to extract:
  - SPI register addresses: `_DSM` functions 1/2/3
  - GPIO pin and IRQ configuration (pin 0x15 in ETW traces, 0x55=85 in DSDT — discrepancy documented)
  - SPI controller MMIO base: `0xFEC10000`
- [ ] File: `~/ssdt*.dsl` — search for `AMDI0060` for SPI controller configuration

### 0.2 Report Descriptor
- [ ] File: `~/Desktop/wintrace/mshw0231_report_descriptor.txt`
  - Verify length (936 bytes)
  - Verify Usage Pages and Report IDs

---

## 1. SPI Controller (spi-amd.c) — Full Verification

### 1.1 Register Map (spi-amd.c vs decomp comparison)

| Register | Offset | Linux (spi-amd.c) | Windows (amdspi.sys) | Match? |
|----------|--------|-------------------|----------------------|--------|
| CTRL0 | 0x00 | AMD_SPI_CTRL0_REG | read32/write32 | [ ] |
| ALT_CS | 0x1D | AMD_SPI_ALT_CS_REG | read8/write8 | [ ] |
| ENA_REG | 0x20 | AMD_SPI_ENA_REG | present | [ ] |
| REG_PREFIX | 0x22 | read16 in probe | fcn.0x6fc0 (16-bit!) | [ ] |
| SPEED_OPCODE | 0x44 | **NOT USED** | USED (16-bit write) | [ ] |
| OPCODE_REG | 0x45 | AMD_SPI_OPCODE_REG | single/double write8 | [ ] |
| CMD_TRIGGER | 0x47 | AMD_SPI_CMD_TRIGGER_REG | toggle bit7 | [ ] |
| TX_COUNT | 0x48 | AMD_SPI_TX_COUNT_REG | write8 | [ ] |
| — | 0x49/0x4A | **REMOVED** (zero occurrences in amdspi.sys) | NOT present | [ ] |
| RX_COUNT | 0x4B | AMD_SPI_RX_COUNT_REG | write8 | [ ] |
| STATUS | 0x4C | AMD_SPI_STATUS_REG | read8 in fcn.0x3c20 (byte!) | [ ] |
| SPEED_REG | 0x6C | AMD_SPI_SPEED_REG | present | [ ] |
| FIFO_BASE | 0x80 | AMD_SPI_FIFO_BASE | 70 bytes | [ ] |

### 1.2 CTRL0 Bit Map

| Bit | Linux Name | Value | Windows Decomp | Correct? |
|-----|-----------|--------|----------------|-----------|
| 31 | BUSY | poll to wait | BIT(31) wait | [ ] |
| 30 | SECRET_30 | 0x40000000 | part of SECRET_BITS | [ ] |
| 29 | SECRET_29 | 0x20000000 | part of SECRET_BITS | [ ] |
| 23 | TXMODE | 0x00800000 | Windows does **NOT** set it | [ ] |
| 21 | PRESERVED | 0x00200000 | Windows preserves it | [ ] |
| 20 | FIFO_CLEAR | clear→set toggle | fcn.0x4bac: clear→set | [ ] |
| 18 | SECRET_18 | 0x00040000 | part of SECRET_BITS | [ ] |
| 16 | EXEC_CMD | clear→set toggle | V1 path only | [ ] |

### 1.3 Segment Execution Sequence (fcn.0x4bac decomp)

**Step-by-step comparison with `amd_spi_exec_segment()` in spi-amd.c:**

```
Windows decomp (fcn.0x4bac):          vs    Linux (amd_spi_exec_segment):

1. read8(0x1D)                              ❌ MISSING in exec_segment
   AND 0xFC, OR 0x01                        (only in select_chip: cs & 0x3, does NOT force 0x01)
   write8(0x1D)

2. Clear FIFO (clear→set bit20)          ✅ amd_spi_clear_fifo_ptr (toggle)

3. write8(opcode, 0x45)                   ✅ amd_spi_set_opcode (single)

4. Secret bits:                           ⚠️  amd_spi_setup_v2_regs (only in host_transfer,
   ctrl0 |= 0x60040000                       NOT in exec_segment!)
   write32(CTRL0)

5. Speed config:                          ❌ MISSING in exec_segment
   write16(0x44, config)                  (done only in host_setup via amd_set_spi_freq
                                            using ENA_REG + SPEED_REG, never 0x44)

6. write8(TX_COUNT, 0x48)                 ✅ present

7. Fill FIFO (0x80+)                      ✅ present

8. write8(RX_COUNT, 0x4B)                 ✅ present

9. write8(opcode, 0x45) AGAIN             ❌ MISSING (double write after 0x44 overwrite)

10. Secret bits AGAIN:                    ❌ MISSING
    ctrl0 |= 0x60040000

11. write8(0x80, 0x47) Trigger            ✅ toggle via amd_spi_execute_opcode

12. Wait busy (CTRL0 bit31)               ✅ amd_spi_busy_wait

13. Restore 0x22 (fcn.0x6f84)            ⚠️  read in probe but NEVER restored
```

### 1.4 Function `amd_spi_setup_v2_regs()`

**Current code** (spi-amd.c:170-176):
```c
static void amd_spi_setup_v2_regs(struct amd_spi *amd_spi) {
    amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG,
                           AMD_SPI_SECRET_BITS, 0);
}
```

**Problems:**
- Only called from `host_transfer` (line 427), NOT from `exec_segment`
- `setclear` with clear=0 means OR, doesn't preserve bit 21
- **Should preserve bit 21** (like Windows does, by reading ctrl0 first)

**Proposed fix:**
```c
static void amd_spi_setup_v2_regs(struct amd_spi *amd_spi) {
    u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
    ctrl0 |= 0x60040000;  // bits 30+29+18
    // Does NOT touch bit 21 (preserved), does NOT set bit 23 (TXMODE)
    amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
}
```

### 1.5 ALT_CS (0x1D) — Behavior

**Windows** (fcn.0x3c20 decomp):
```
read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
```
**Always** forces cs=1 with bit0=1. Bit0 might be an enable strobe.

**Linux** (spi-amd.c:106-109):
```c
static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs) {
    amd_spi_setclear_reg8(amd_spi, AMD_SPI_ALT_CS_REG,
                          cs, AMD_SPI_ALT_CS_MASK);
}
```
Uses `cs & 0x3` as a pure CS select. Supports cs=0,1,2,3.

**Potential issue**: If the touchscreen is on cs=0, the value would be `OR 0x00` = no bit set, whereas Windows writes `OR 0x01`.

### 1.6 RX FIFO Offset

**Linux**: `fifo_pos + 4` (fixed +4 offset)
**Needs verification**. The controller overwrites the first FIFO positions with RX data. With TX=0, RX data appears at offset 0. With TX>0, should RX data appear after the TX data? The +4 fix pins a position after the header. **Needs real hardware testing**.

### 1.7 Function `amd_spi_host_transfer` — Multi-segment

The current function walks every transfer in the list and:
- For TX: extracts the opcode from the first byte, performs FIFO chunking
- For standalone RX: always uses opcode 0x0B

**Problem**: TX and RX are **separate** operations even when they belong to the same logical transaction (e.g. DESCREQ where TX=0x02 and RX=0x02 should be combined).

---

## 2. HID Protocol (spi-hid-core.c) — Full Verification

### 2.1 State Machine

| State | Our Driver | Windows Decomp |
|-------|---------------|----------------|
| 0 (WAIT_RESET) | type==3: drain x2 + DESCREQ | VerifyResetResponse: if type==3 → advance and send DESCREQ |
| 1 (WAIT_DESC) | type==7: parse desc → state=2 | type==7: receives desc → DESCREQ2 |
| 2 (WAIT_RPT) | type==8: drain → VENDOR_INIT | type==8: receives report desc (State2 compares against type==4!) |
| 3 (VENDOR_INIT) | cmd1,cmd2,cmd3 | Vendor activation after descriptors |
| 4 (DONE) | type==7 → hid_input_report | Runtime |

### 2.2 Bug #1: memcpy offset (DRIVER_STATE.md)

**File**: spi-hid-core.c:1237
**Bug**: `memcpy(&raw, hdr+off, s)` copies the header instead of the body
**Fix**: `memcpy(&raw, hdr+off+4, s)` (+4 to skip the HID header)

### 2.3 Bug #2: initial input_register

**File**: spi-hid-core.c
**Bug**: Always uses `SPI_HID_DEFAULT_INPUT_REGISTER = 0x1000`
**Windows**: Uses 0x0000 with approval byte7=0x00 before the descriptor parse

### 2.4 Bug #3: DESCREQ doesn't produce type=7

**Root cause**: TX opcode 0x02 + separate RX opcode 0x0B
- The 0x0B RX is a **new read command**, not the completion of the DESCREQ
- The device processes the DESCREQ during the 0x0B RX, not before

**Windows solution**: TX+RX with the same opcode (0x02) in a single SPI operation
- CSV TXN #3: opcode 0x02, TX 10 bytes + RX 10 bytes, response = ACK
- Then GPIO IRQ → 0x0B read → type=7

### 2.5 Initialization Sequence (from Windows CSV)

```
Phase 1 — Reset (Device A, touchscreen):
  TXN#0: 0x0B read 9B → RESET_RSP header (32 10 00 5A)
  TXN#1: 0x0B read 9B → RESET_RSP body (03 00 00 00)
  These are header+body of the SAME Reset Response input report.

Phase 2 — Discovery (Device A, touchscreen):
  TXN#2: DESCREQ 0x02 TX 10B (write-only, RX_COUNT=0)
  GPIO IRQ (58 µs after IoComplete)
  TXN#3: 0x0B read 9B → DEVICE_DESC header type=7
  TXN#4: 0x0B read 37B → device descriptor body (28 bytes at body+3)
  TXN#5: DESCREQ2 0x02 TX 10B (write-only) → MISO all zeros (device busy)
  GPIO IRQ (727 µs after IoComplete)
  TXN#6: 0x0B read 9B → RPT_DESC header type=8
  TXN#7: 0x0B read 945B → HID report descriptor body (936 bytes at body+3)
  (~962ms gap — driver processing)

Phase 3 — Device B Activity (companion chip, NOT touchscreen):
  TXN B1-B3: cmd1 (0x00, 5B), cmd2 (0x00, 1B), checksum read
  TXN B4: cmd3 (0x70 TX+RX 14B) — FW status on companion device
  TXN B5+: B0 firmware upload (120 blocks × 241B) on companion device
  IMPORTANT: This activity is on a DIFFERENT SPB device (handle 0x7F74AA5D7B88,
  connections 0x18/0x19/0x1A). The touchscreen receives NONE of these commands.
```

### 2.6 Previously Called "Approval Bytes" — NOW KNOWN TO BE BUFFER ARTIFACTS

The bytes at positions 6-8 of the 0x0B TX payload are **residual buffer data**
from the Windows driver's buffer reuse, NOT intentional protocol fields.
After a 10-byte DESCREQ write (`02 00 00 01 42 00 00 03 00 00`), the next
read only writes bytes 0-4, leaving bytes 5-8 with residual values.
Observed values (0x00, 0x03, 0x0A) are explained by this mechanism.
These are NOT "approval bytes" and do NOT need to be set by the driver.

### 2.7 GPIO IRQ

- Pin 0x15, level-triggered active-high (flags 0x409)
- The device raises the IRQ when it has data ready after DESCREQ/DESCREQ2
- The host must **wait** for the IRQ, then perform a 0x0B read

---

## 3. Firmware Upload

### 3.1 Firmware Files

- `~/b0_clean.bin` (29045 bytes)
- `~/b0_firmware.bin` (28920 bytes)
- `~/b0_firmware_v2.bin` (28920 bytes)

### 3.2 Upload Sequence (from CSV)

- 120 blocks of 241 bytes (0xB0 + 240 data bytes)
- 125-byte tail
- ~100-170 µs interval between blocks
- Total duration ~410ms
- After FW: 0xB1 execute command, then 0x70 status check

### 3.3 TXMODE for 0xB0

- Opcode 0xB0 requires TXMODE=1 because the controller doesn't recognize 0xB0 as a write
- Windows doesn't use TXMODE — how does it manage?
- Possibly the V2 path with secret bits handles direction automatically

---

## 4. Aspects to Verify with Real Hardware Tests

### 4.1 Register 0x44 (Windows Speed Config)

- Not implemented in the Linux driver
- Windows writes 16 bits to 0x44, which also overwrites 0x45
- The double opcode write (step 9) exists because of this
- Question: is it actually required for operation?

### 4.2 Strobe 0x49/0x4A

- Our driver writes 0x00 to both
- The Windows decomp doesn't explicitly show these writes
- Possibly handled by hardware or implicit in the trigger

### 4.3 Secret Bits

- 0x60040000 = bit 30 + 29 + 18
- Exact function unknown, but Windows always sets them
- Bits 30 and 29 might be "enable V2 path"
- Bit 18 might enable direction auto-detection (TX vs RX)

### 4.4 Restore 0x22

- fcn.0x6fc0 reads 16 bits from MMIO+0x22 at the start of every transfer
- fcn.0x6f84 restores 0x22 after the transfer
- Our driver reads 0x22 in probe (ioread16) but never restores it
- Might not be critical if we never modify 0x22

---

## 5. Action Plan (in priority order)

### Phase A: SPI Controller Fixes (CRITICAL)

1. **Fix `amd_spi_setup_v2_regs`**: read ctrl0 first, preserve bit 21, only set bits 30+29+18
2. **Move `setup_v2_regs` into `exec_segment`**: call it BEFORE every segment
3. **Add 0x22 restore**: save before, restore after in exec_segment
4. **Fix ALT_CS**: use the Windows pattern (AND 0xFC, OR 0x01) instead of a pure CS select
5. **Verify 0x44 speed config**: test whether it's actually necessary

Phase B: HID Protocol Fixes (REVISED 2026-07-06):

1. ~~Fix memcpy offset~~ **DONE** (spi-hid-core.c:1237 → hdr+off+4)
2. ~~Fix initial input_register~~ **DONE** (use 0x0000)
3. **Use separate write+read**: TX-only 0x02 write, wait for GPIO IRQ, then 0x0B read.
   This IS what Windows does — NOT "combined TX+RX same opcode."
4. **Add double drain**: two 0x0B reads before DESCREQ (header+body of RESET_RSP)
5. **Wait for GPIO IRQ**: after DESCREQ, wait for the IRQ before reading the response

NOTE: "Approval bytes" (C6/C7) and vendor activation (cmd1/cmd2/cmd3) are
NOT required — approval bytes are buffer artifacts, and activation commands
belong to the companion device (Device B), not the touchscreen.

### Phase C: Firmware and Runtime

1. **Re-enable fw_work** after the SPI fixes
2. **Test B0 upload** with the fixes applied
3. **Verify HID reports** at runtime

---

## 6. Final Verification Checklist

- [ ] The device responds with RESET_RSP header (type=3) on the first 0x0B read
- [ ] The second 0x0B read returns the RESET_RSP body (03 00 00 00 — NOT an "ACK")
- [ ] DESCREQ 0x02 TX (write-only) is sent to the device
- [ ] GPIO IRQ arrives after DESCREQ
- [ ] The post-IRQ 0x0B read returns DEVICE_DESC header type=7
- [ ] The 37B 0x0B read returns the correct descriptor (vendor=0x045E, product=0x0C19)
- [ ] DESCREQ2 produces RPT_DESC header type=8 after GPIO IRQ
- [ ] The 936-byte report descriptor is received correctly
- [ ] Type-1 (DATA) HID reports are received at runtime
