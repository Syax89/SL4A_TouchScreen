# HID-over-SPI Protocol — MSHW0231 Touchscreen

## Overview

The Surface Laptop 4 AMD touchscreen uses the HID-over-SPI protocol (section 7.2.5.7 specifies HID SPI).
The device is a touchscreen controller that requires:
1. A discovery sequence (reset → descriptors → report descriptor)
2. Vendor-specific activation (cmd1/cmd2/cmd3)
3. B0 firmware upload (120 blocks of 241 bytes)
4. Initialization (~41 seconds)
5. Normal operation (HID reports)

## HID Header Format

Every response from the device starts with sync bytes (0xFF) followed by the header:

```
Byte 0: [TYPE_4bit][VERSION_4bit]
        version = 2 (HID-over-SPI)
        report type:
           0 = ACK/Ready
           1 = DATA
           3 = RESET_RSP
           4 = COMMAND_RESP
           5 = GET_FEATURE_RESP
           7 = DEVICE_DESC
           8 = RPT_DESC

Byte 1: [LENGTH_LOW_4bit][FRAGMENT_ID_4bit]
Byte 2: [LENGTH_HIGH_8bit]
Byte 3: 0x5A (marker)

Body length = (((Byte1 >> 4) << 0) | (Byte2 << 4)) * 4
```

### Examples

```
RESET_RSP:  32 10 00 5A    type=3, body_len = ((0x10>>4) | 0x00<<4)*4 = 4
DEVICE_DESC: 72 80 00 5A    type=7, body_len = ((0x80>>4) | 0x00<<4)*4 = 32
RPT_DESC:   82 B0 0E 5A    type=8, body_len = ((0xB0>>4) | 0x0E<<4)*4 = 0xEB*4 = 940

ACK pattern: 03 00 00 00    (no 0x5A! Special pattern)
```

---

## SPI Commands

### 0x0B — Read Register (Approval Read)

```
TX: [0x0B] [ADDR_H] [ADDR_M] [ADDR_L] [0xFF] [0x00] [0x00] [APPR7] [APPR8]
      opcode  └──── register address (24bit) ────┘  └pad─┘ └─ approval ─┘

9 bytes total. The device responds with the data from the specified register.

Approval bytes:
  APPR7: 0x00 for state==0, 0x03 for state!=0 (spi_hid_approval_byte7: changes on the 0→1
         transition, not after the descriptor parse)
  APPR8: 0x00 by default

Register addresses:
  0x000000: input register (default)
  0x001000: input_register (after device descriptor parse)
```

### 0x02 — DESCREQ (Device Descriptor Request)

```
TX: [0x02] [0x00] [0x00] [0x01] [0x42] [0x00] [0x00] [0x03] [0x00] [0x00]
      opcode  └───???────┘ └─??─┘ └─???──────────────┘ └─??─┘

10 bytes total.

DESCREQ2 (for the report descriptor):
TX: [0x02] [0x00] [0x00] [0x02] [0x42] [0x00] [0x00] [0x03] [0x00] [0x00]
                      ↑ register 0x02 instead of 0x01

The device responds with ACK (03 00 00 00), then raises an IRQ.
After the IRQ, the next 0x0B read returns the descriptor.
```

### 0x00 — Activation (cmd1, cmd2)

```
cmd1: [0x00] [0x0E] [0x00] [0x00] [0x00]    5 bytes
cmd2: [0x00]                                   1 byte (NOP/padding)

TX only, no inline response.
```

### 0x70 — Read Status (cmd3)

```
TX: [0x70]                                     1 byte
RX: 14 bytes of firmware status

TX+RX in the same transaction.
```

### 0xB0 — Firmware Block Write

```
TX: [0xB0] [240 bytes of firmware data]         241 bytes total

TX only, no inline response.
120 blocks + 125-byte tail = 29,045 bytes total.
```

---

## Complete Boot Sequence

### Phase 1: Reset (0-200 µs)
```
┌─ 0x0B read → RESET_RSP (32 10 00 5A)
│  The device is in reset, it sends the reset report.
├─ 0x0B read → ACK (03 00 00 00)
│  After ~168 µs, the device is ready.
│  NO command sent between the two reads!
└─ The device changes state on its own (internal timeout)
```

### Phase 2: Device Discovery (~200-2000 µs)
```
┌─ DESCREQ (0x02) → ACK (03 00 00 00 00)
├─ GPIO IRQ (~58 µs later)
├─ 0x0B read → DEVICE_DESC type=7 (72 80 00 5A)
│  approval byte7 changes to 0x03
├─ 0x0B read 37B → device descriptor data
│  vendor=0x03A8, product=0x0002, ver=0x0320
├─ DESCREQ2 (0x02) → all zeros (device busy)
├─ GPIO IRQ (~727 µs later)
├─ 0x0B read → RPT_DESC type=8 (82 B0 0E 5A)
├─ 0x0B read 945B → full HID report descriptor
└─ ~962ms gap (internal driver processing)
```

### Phase 3: Activation (~962ms)
```
┌─ cmd1 (0x00, 5 bytes)
├─ cmd2 (0x00, 1 byte)
├─ 0x0B read → checksum response
├─ cmd3 (0x70, TX+RX 14B) → FW status
└─ (possible other vendor commands: 0x28, 0x24, 0x29, 0x22, 0x25, 0x26)
```

### Phase 4: Firmware Upload (~410ms)
```
┌─ B0 block #1 (241 bytes)
├─ B0 block #2
├─ ... (120 blocks total)
├─ B0 tail (125 bytes)
└─ 41-second wait for device initialization
```

### Phase 5: Operation (~41s after FW)
```
┌─ Periodic GPIO IRQ (~every 110ms)
├─ 0x0B read → HID report type=7
├─ hid_input_report() → touch events
└─ Continuous loop
```

---

## Our Driver's State Machine

### Current State (v5)

```
State 0 (WAIT_RESET):
  ├── type==3: drain #1 (blen bytes) + drain #2 (64 bytes)
  ├── Always: DESCREQ (0x02 TX+RX 10B) → state=1
  └── After state=1: extra spi_hid_seq_read(shid, shid->input.content, 64) to flush the buffer

State 1 (WAIT_DESC):
  ├── type==7: parse device descriptor → DESCREQ2 → state=2 + drain
  ├── type==3: "still RESET_RSP" (log)
  └── type==0: "ACK in state 1" (log, no action)

State 2 (WAIT_RPT):
  ├── type==8: drain → goto VENDOR_INIT
  ├── NOTE: State2's decomp (fcn.0xda9c) compares against type==4 (COMMAND_RESP), not type==8.
  │   Our code compares against type==8. Discrepancy to be verified.
  └── type==3/7: log and retry

State 3 (VENDOR_INIT):
  ├── cmd1 (0x00, 5B) → cmd2 (0x00, 1B) → ACK read → cmd3 (0x70)
  └── schedule fw_work + create_device_work → state=4

State 4 (DONE):
  └── type==7: forward to hid_input_report()
```

### Known Issues

1. **DESCREQ doesn't produce type=7**: TX uses opcode 0x02, the separate RX uses 0x0B
   - On Windows: TX+RX use the SAME opcode 0x02 in the same operation
   - From the CSV: inline DESCREQ TX+RX returns ACK, then IRQ, then type=7

2. **No DESCREQ2**: the second DESCREQ for the report descriptor is missing
   - The code in state 1 DOES have DESCREQ2 but it's never executed (state 1 never sees type=7)

3. **Missing multi-drain**: Windows performs 2 0x0B reads before DESCREQ
   - The two reads drain RESET_RSP and then receive ACK

4. **TX+RX opcode mismatch**: the 0x02 write and the 0x0B read are separate operations
   - The device interprets 0x0B as a new command, not as the completion of 0x02

### Proposed Fixes

**Option A**: Use a combined `exec_segment(opcode=0x02, tx=9, rx=10)`
  - Same opcode for TX and RX in a single operation
  - Requires understanding the FIFO layout for RX when TX>0

**Option B**: TX-only 0x02 write, wait for the GPIO IRQ, then 0x0B read
  - The device processes the DESCREQ and raises the IRQ when ready
  - The following 0x0B read picks up the response

**Option C**: Use the V1 path (CTRL0) for DESCREQ with combined TX+RX
  - The V1 path might handle RX for any opcode
  - Fewer restrictions than the V2 path with TXMODE

---

## Windows Driver — hidspicx.sys

From decompilation, the Windows HID-over-SPI driver implements:

### Main Functions

1. **VerifyResetResponse** (fcn.0xdd64):
   - Reads the RESET_RSP header (type=3)
   - Verifies the body contains the correct data
   - If valid, advances the state

2. **HidGetDeviceDescriptor** (fcn.0xf458):
   - Sends DESCREQ (opcode 0x02, register 0x01)
   - Waits for the GPIO IRQ
   - Reads the device descriptor via 0x0B

3. **VerifyDescriptorCompletion** (fcn.0xd2f8):
   - Verifies the descriptor is complete
   - Sends DESCREQ2 (opcode 0x02, register 0x02)
   - Waits for the GPIO IRQ
   - Reads the report descriptor

4. **ConfigureTransfer** (fcn.0xa664):
   - Configures the transfer buffers
   - Sets the approval bytes (0x00 → 0x03)

5. **PrepareDescriptor** (fcn.0x12bc0):
   - Allocates the descriptor buffer
   - Prepares the SPI_HID_DEVICE_DESC structure

### Orchestrator (fcn.0xc8d8) — Main Loop

```
1. Reads the report type from the buffer
2. Switches on the type:
   - type==3 (RESET_RSP): advances state and sends DESCREQ
   - type==7 (DEVICE_DESC): receives descriptor, sends DESCREQ2
   - type==8 (RPT_DESC): receives report descriptor
   - type==0 (ACK) with body_len>0: verifies completion
3. If the descriptor is complete, starts activation
4. Loops on subsequent IRQs
```

### Decomp Reading Key

```
r13d = 3       → constant used to compare against type==3
r14d = state flag (0/1)
rdi = type byte from the buffer
cmp dil, r13b  → compares type against 3
je 0x...       → if type==3, jump to the RESET_RSP handler
```

---

## Summary of Required Fixes

| Priority | Issue | File | Fix |
|----------|---------|------|-----|
| CRITICAL | DESCREQ doesn't produce type=7 | spi-hid-core.c | TX+RX same opcode, or wait for IRQ |
| CRITICAL | Device doesn't receive DESCREQ | spi-amd.c | Verify TXMODE for 0x02 |
| HIGH | Missing multi-drain | spi-hid-core.c | Two 0x0B reads before DESCREQ |
| HIGH | State 1 never sees type=7 | spi-hid-core.c | Add logic for type=7 |
| MEDIUM | Missing DESCREQ2 | spi-hid-core.c | Already present but unreachable |
| MEDIUM | Full VENDOR_INIT missing | spi-hid-core.c | Add vendor commands |
| LOW | Firmware untested | spi-hid-core.c | Re-enable fw_work after fixes |
