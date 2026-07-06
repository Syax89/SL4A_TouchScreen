# Full Verification Results — 2026-07-04

## Executive Summary

Three sub-agents independently verified the SPI controller, HID state machine, and ACPI/init flow.
Below are the **8 critical bugs** to fix BEFORE any other change.

---

## Critical Bug Summary

| # | Component | Bug | File:Line | Impact |
|---|-----------|-----|-----------|---------|
| C1 | **SPI** | `amd_spi_select_chip()`: uses `cs & 0x03`, Windows forces `OR 0x01` | spi-amd.c:106-108 | CRITICAL — transfers with CS=0 fail |
| C2 | **SPI** | `amd_spi_setup_v2_regs()` called from `host_transfer`, NOT from `exec_segment` | spi-amd.c:426-427 | HIGH — secret bits not applied per segment |
| C3 | **SPI** | Strobe 0x49/0x4A absent in Windows, potentially harmful | spi-amd.c:269-270 | MEDIUM |
| C4 | **HID** | `memcpy(&raw, hdr+off, s)` instead of `hdr+off+4` — corrupted descriptor | spi-hid-core.c:1237 | **CRITICAL** — all descriptor fields shifted by 4 bytes |
| C5 | **HID** | `input_register = 0x1000` for pre-descriptor reads — should be `0x0000` | spi-hid-core.h:76, spi-hid-core.c:1814 | **CRITICAL** — full state machine desync |
| C6 | **HID** | `approval7 = 0x03` at runtime — should be `0x0A` | spi-hid-core.c:1129 | **CRITICAL** — device rejects reports |
| C7 | **HID** | `approval8 = 0x0A` at runtime — should be `0x00` | spi-hid-core.c:1131 | **CRITICAL** — device rejects reports |
| C8 | **ACPI** | GPIO pin documented as `0x15` but the DSDT says `0x55` (85 dec, Edge/ActiveLow trigger) | DSDT / DRIVER_STATE.md | MEDIUM — the driver reads it from ACPI at runtime, but the documentation was wrong |

---

## Critical Bug Detail

### C1 — ALT_CS Encoding (SPI)

```c
// Current code (spi-amd.c:106-108)
static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs) {
    amd_spi_setclear_reg8(amd_spi, AMD_SPI_ALT_CS_REG,
                          cs, AMD_SPI_ALT_CS_MASK);
}

// Windows behavior (from fcn.0x4bac decomp)
// read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
```

**Problem**: bit 0 of 0x1D might be a required "strobe enable", not a real CS select.
Windows always forces `0x01`. Our driver with cs=0 produces `0x00` (strobe disabled).

**Fix**:
```c
static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs) {
    u8 tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
    tmp = (tmp & 0xFC) | 0x01;  // Windows pattern
    amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);
}
```

### C2 — Secret Bits Not in exec_segment (SPI)

```c
// Current code: setup_v2_regs is only called from host_transfer (line 427)
// NEVER from amd_spi_exec_segment (lines 234-292)

// Fix: call amd_spi_setup_v2_regs at the start of amd_spi_exec_segment
// Windows sets the secret bits INSIDE fcn.0x2be4 (transfer_data) for every segment
```

### C4 — memcpy Offset (HID)

```c
// Current code (spi-hid-core.c:1237):
memcpy(&raw, hdr + off, s);  // copies from the HID header!

// Fix:
memcpy(&raw, hdr + off + 4, min(s - 4, sizeof(raw)));
// +4 skips the header [type][len_low][len_high][0x5A]
```

### C5 — Initial input_register (HID)

```c
// spi-hid-core.h:76:
#define SPI_HID_DEFAULT_INPUT_REGISTER  0x1000

// spi-hid-core.c:1814:
shid->desc.input_register = SPI_HID_DEFAULT_INPUT_REGISTER;

// Fix: use 0x0000 in state 0 (pre-descriptor)
// After the descriptor parse, the real input_register comes from the device
```

### C6/C7 — Wrong Approval Bytes (HID)

```c
// Current code (spi-hid-core.c:1129-1131):
static u8 spi_hid_approval_byte7(struct spi_hid *shid)
{ return (shid->seq_state == 0) ? 0x00 : 0x03; }
static u8 spi_hid_approval_byte8(struct spi_hid *shid)
{ return (shid->seq_state == 4) ? 0x0A : 0x00; }

// Windows CSV at runtime: approval7=0x0A, approval8=0x00
// Fix:
static u8 spi_hid_approval_byte7(struct spi_hid *shid)
{ return (shid->seq_state == 0) ? 0x00 : ((shid->seq_state >= 4) ? 0x0A : 0x03); }
static u8 spi_hid_approval_byte8(struct spi_hid *shid)
{ return 0x00; }
```

---

## What Was CONFIRMED OK

| Area | Confirmation |
|------|---------|
| SPI register offsets (all) | Correct |
| MMIO access widths (8/16/32 bit) | Correct |
| FIFO_CLEAR toggle (bit20 clear→set) | Correct |
| CMD_TRIGGER toggle (0x47 bit7 clear→set) | Correct, more robust than Windows |
| RX FIFO offset +4 | Correct |
| Busy wait (CTRL0 bit31, 2s timeout) | Correct |
| Secret bits (30+29+18 = 0x60040000) | Identical to Windows |
| Bit 21 preserved (read-modify-write) | OK (clear mask = 0 in setclear) |
| DESCREQ payload format (10 bytes) | Exact |
| DESCREQ2 payload format | Exact |
| cmd1/cmd2/cmd3 payload | Exact |
| Double drain (2x 0x0B read) before DESCREQ | Correct |
| State machine transition logic | Correct |
| Report descriptor buffer size (8K) | Sufficient (936 bytes) |
| ACPI match table (AMDI0060 → V2) | Correct |
| MMIO base 0xFEC10000 | Correct |
| SPI speed 33.33 MHz | Correct |
| reg_prefix (ioread16 from 0x22) | Correct |
| GPIO IRQ thread registration | Correct |
| DSDT _DSM UUID match | Correct |

---

## Fix Status (2026-07-06 — FINAL)

| # | Bug | Status | File |
|---|-----|--------|------|
| C4 | memcpy offset +4 | **FIXED** | spi-hid-core.c:1237 |
| C5 | input_register=0x1000 | **FIXED** | spi-hid-core.h:76 |
| C6 | approval7 runtime | **FIXED** | spi-hid-core.c:1129 |
| C7 | approval8 runtime | **FIXED** | spi-hid-core.c:1131 |
| C1 | ALT_CS encoding | **FIXED** | spi-amd.c:106-108 |
| C2 | secret bits in exec_segment | **FIXED** | spi-amd.c:250-251, removed from host_transfer |
| C3 | Strobe 0x49/0x4A | **REMOVED** (not used by Windows) | spi-amd.c:272-273 |
| C4-C7,C1-C3 | Build | **COMPILES** | Both modules |
| DESCREQ | Write path | **EXHAUSTED** — software fix insufficient. Logic analyzer needed. | — |

---

## Final Test Outcome (2026-07-06)

All software fixes have been applied. However, **the device ignores every write (opcode 0x02)**.
The block is at the physical level (CTRL0 bits[15:8] hardwired to 0xA9 vs 0x0E on Windows).

**Next step**: logic analyzer on SCK/MOSI/MISO/CS between Windows and Linux.

---

## Operating Principle

```
MEASURE A THOUSAND TIMES, CUT ONCE.
```

Every code change must:
1. Be compared against the corresponding Windows decomp
2. Be verified by an independent sub-agent
3. Be documented in this file and in DRIVER_STATE.md
4. Be tested with `make && insmod` BEFORE moving on to the next fix
