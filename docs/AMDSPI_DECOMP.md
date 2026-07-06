# amdspi.sys — Windows AMD SPI Driver (Decompiled)

Windows driver for the AMD FCH SPI controller. Responsible for low-level SPI communication.

## Source Files

All files in `docs/decomp/amdspi/` are functions decompiled from `amdspi.sys` (x64).

---

## Function Index

| Address | File Name | Description |
|-----------|-----------|-------------|
| 0x1000 | 0x1000_get_context.txt | Gets the device context |
| 0x19c0 | 0x19c0_read8.txt | 8-bit MMIO read |
| 0x19d0 | 0x19d0_read32.txt | 32-bit MMIO read |
| 0x19e0 | 0x19e0_read8b.txt | 8-bit MMIO read (variant) |
| 0x1e34 | 0x1e34.txt | (utility) |
| 0x1fbc | 0x1fbc_write_mmio.txt | Generic MMIO write |
| 0x1fd8 | 0x1fd8_write_mmio2.txt | Generic MMIO write 2 |
| 0x21c0 | 0x21c0.txt | (utility) |
| 0x2ad4 | 0x2ad4.txt | (transfer setup) |
| 0x2be4 | 0x2be4_transfer_data.txt | Fills the FIFO with TX data |
| 0x2d28 | 0x2d28.txt | (buffer management) |
| 0x2ff4 | 0x2ff4.txt | (completion handling) |
| 0x3528 | 0x3528_sub_transfer.txt | Sub-transfer: handles TX+RX under the same CS |
| 0x3994 | 0x3994.txt | (error handling) |
| **0x3c20** | **0x3c20_transfer.txt** | **Full SPI transfer execution** |
| 0x4684 | 0x4684.txt | (cleanup) |
| **0x4bac** | **0x4bac.txt** | **Segment execution (TX+RX)** |
| **0x54d0** | **0x54d0_submit_handler.txt** | **Entry point: submit I/O request** |
| 0x6d50 | 0x6d50.txt | (timer/cleanup) |
| 0x6f84 | 0x6f84_restore_register_prefix.txt | Restores 0x22 after the transfer |
| 0x6fc0 | 0x6fc0_read_register_prefix.txt | Reads the register prefix from 0x22 |
| 0x7714 | 0x7714.txt | (I/O completion) |
| 0x7ba0 | 0x7ba0.txt | (utility) |

---

## Key Functions

### fcn.0x3c20 — Transfer (Full Execution)

**Called from**: 0x5c9c (dispatch)

**Logic** (from the assembly):
1. Allocates a DMA buffer (512 bytes, tag 'SPI2')
2. Calls `fcn.0x3528` (sub_transfer) to configure the transfer
3. If sub_transfer returns 0, error
4. Calls `fcn.0x6fc0` — **reads the register prefix from MMIO+0x22** (16-bit)
5. Calls `fcn.0x19c0` — 8-bit read of the status register at context+0x4C (MMIO+0x4C), saves it to a global variable
6. Saves the read value in the global variable `[0x14001e814]`
7. Saves `[context+0x20]` — looks like a segment counter
8. Divides by 64 (0x40) — computing the number of segments?
9. If the remainder != 0 → conditional branch
10. Calls `fcn.0x2be4` — **transfer_data**: fills the FIFO and executes

### fcn.0x4bac — Execute Segment

**Operation order** (confirmed against our own code):

```
1. read8(0x1D)                         // read ALT_CS
2. AND 0xFC, OR 0x01                   // preserve bits 7-2, zero the CS bits, set bit 0
3. write8(0x1D)                         // write ALT_CS (enable strobe)

4. Clear FIFO:
   ctrl0 = read32(CTRL0)
   ctrl0 &= ~BIT(20)                   // clear FIFO_CLEAR
   write32(CTRL0, ctrl0)
   ctrl0 |= BIT(20)                    // set FIFO_CLEAR (rising edge)
   write32(CTRL0, ctrl0)

5. write8(opcode, 0x45)                // opcode into OPCODE_REG

6. Secret bits:
   ctrl0 = read32(CTRL0)
   ctrl0 |= 0x60040000                 // bits 30, 29, 18
   // Does NOT set bit 23 (TXMODE)!
   // Does NOT touch bit 21 (PRESERVED)!
   write32(CTRL0, ctrl0)

7. Speed config:
   r44 = read16(0x44)                  // read speed+opcode
   r44 = (r44 & 0xF0FF) | (nibble << 8) // set the speed nibble
   r44 = (r44 & 0x0FFF) | (nibble << 12)
   write16(0x44, r44)                  // write — OVERWRITES 0x45!

8. write8(TX_COUNT, 0x48)             // bytes to transmit

9. Fill FIFO (0x80+):
   for i in 0..tx_len-1:
     write8(data[i], 0x80 + i)

10. write8(RX_COUNT, 0x4B)            // bytes to receive
    // For a write: RX_COUNT = 0
    // For a read: RX_COUNT = rx_len

11. write8(opcode, 0x45) AGAIN         // re-write after the speed config clobbered it!

12. Secret bits AGAIN:
    ctrl0 = read32(CTRL0)
    ctrl0 |= 0x60040000
    write32(CTRL0, ctrl0)

13. Trigger:
    write8(0x80, 0x47)                // CMD_TRIGGER bit7

14. Wait busy (lfence first):
    while (read32(CTRL0) & BIT(31))
      pause/spin

15. Restore (fcn.0x6f84):
    write16(0x22, saved_prefix)       // restores the register prefix (0x22 ONLY)
```

### fcn.0x54d0 — Submit Handler

**Entry point** for every SPI I/O request.

Calls:
1. `fcn.0x6f84` (restore) — register restore

### fcn.0x2be4 — Transfer Data

**Fills the FIFO** with the TX data for the transfer.
Called by `fcn.0x3c20` for every segment.

### fcn.0x6fc0 — Read Register Prefix

Reads 16 bits from MMIO+0x22 and stores it in the context.
This value is later used by `fcn.0x6f84` for the restore.

### fcn.0x6f84 — Restore Register Prefix

Restores ONLY 0x22:
- `write16(0x22, saved_value_from_0x6fc0)`

This ensures the register prefix state stays consistent between transfers. It does not restore 0x44.

---

## hidspicx.sys — HID Protocol Driver

The files in `docs/decomp/hidspicx_*.txt` contain the HID-over-SPI driver:

| File | Function |
|------|----------|
| hidspicx_dd64_VerifyResetResponse | Verifies the RESET_RSP header (type=3) |
| hidspicx_f458_HidGetDeviceDescriptor | Sends DESCREQ, receives the device descriptor |
| hidspicx_d2f8_VerifyDescriptorCompletion | Verifies the descriptor, sends DESCREQ2 |
| hidspicx_a664_ConfigureTransfer | Configures the transfer buffer |
| hidspicx_a900_ConfiguringDescriptorTransfer | Configures the descriptor transfer |
| hidspicx_orchestrator | Main loop — dispatch by report type |
| hidspicx_12bc0_PrepareDescriptor | Allocates the buffer and prepares the descriptor |
| hidspicx_1ef0_InitTransfer | Initializes the transfer |
| hidspicx_da9c_State2 | State 2 handling |

### Orchestrator (fcn.0xc8d8)

The hidspi driver's main loop. Dispatches based on the report type:

```
1. Reads the type byte from the received buffer
2. r13d = 3 (constant for RESET_RSP)
3. cmp type, 3 → if type==3, RESET_RSP handler
4. cmp type, 7 → if type==7, DEVICE_DESC handler
5. cmp type, 8 → if type==8, RPT_DESC handler
6. For each type, calls the appropriate function
7. Loops on subsequent IRQs
```

Assembly details:
```
mov r13d, 3           ; constant for type==3
movzx edi, byte [rcx] ; read the type byte
cmp dil, r13b          ; type == 3?
jne not_reset_rsp      ; if not, jump away
; ... RESET_RSP handler ...
```

The `r14b` flag tracks whether the descriptor has been received:
- `r14b = 0` → descriptor not yet received
- `r14b = 1` → descriptor received

The `dil` flag tracks whether the buffer is valid:
- `setne dil` after `cmp` — dil=1 if the context pointer is valid

---

## Summary

### What Windows does that we don't

1. **Two 0x0B reads before DESCREQ** — RESET_RSP drain
2. **TX+RX with the same opcode** in a single operation (0x02 TX + 0x02 RX)
3. **Speed config** at 0x44 which overwrites 0x45, then re-writes the opcode
4. **Restore of 0x22** after every transfer (NOT 0x44)
5. **0x1D strobe** (AND 0xFC, OR 0x01) before every transfer
6. **Secret bits** (30, 29, 18) in CTRL0 — always set
7. **Does NOT set TXMODE** (bit 23) — Windows doesn't need it
8. **Waits for the GPIO IRQ** before reading DESCREQ responses
9. **DESCREQ2** to request the report descriptor after the device descriptor
10. **~962ms gap** between receiving the descriptors and the activation commands

### What we do that Windows doesn't

1. **Separate TX+RX** with different opcodes (0x02 TX, 0x0B RX)
2. **Single drain read** instead of double
3. **No speed config** (removed for simplicity)
4. **No 0x22/0x44 restore** (not needed without the speed config)
5. **No 0x1D strobe** (removed for debugging)
6. **Extensive debug logging** on every SPI operation
