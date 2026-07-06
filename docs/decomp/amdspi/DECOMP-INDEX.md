# AMD SPI Driver (amdspi.sys) — Decomp Index

> **Updated: 2026-07-06 (final)** — software exhausted. The MMIO sequence is identical to Linux.

## MMIO Register Access Functions
| Address | Name | Description |
|---------|------|-------------|
| 0x1400019c0 | write8_read? | `mov al, [rax]` — reads BYTE from MMIO address |
| 0x1400019d0 | read_dword | `mov eax, [rax]` — reads DWORD from MMIO address |
| 0x1400019e0 | read_byte? | reads byte from MMIO |
| 0x140001fbc | write_byte_mmio | `mov [rax], cl` — writes BYTE to MMIO, with memory barrier |
| 0x140001fd8 | write_dword_mmio | `mov [rax], ecx` — writes DWORD to MMIO, with barrier |

## Key Transfer Functions
| Address | Size | Name | Description |
|---------|------|------|-------------|
| 0x140003c20 | 2659B | transfer_execute | MAIN transfer function. TX_COUNT=3, opcode=0x0B, secret bits, FIFO data. Reads RX at 0x83. |
| 0x140003528 | 1130B | sub_transfer | Sub-transfer setup, DMA descriptors ('AeiC'/'ALDT'). NO MMIO access. |
| 0x1400054d0 | 1996B | submit_handler | WRITE handler (opcode 0x02). RX_COUNT=0 — TX-only. |
| 0x140004bac | 2337B | read_with_speed | READ handler with speed config. 0x44 dance. RX_COUNT=rx_len+1. Reads at 0x84. |
| 0x140004684 | 1319B | write_sequence | WRITE handler inside SPB sequence. RX_COUNT=0. |
| 0x140002be4 | 323B | transfer_data | Data copy/setup for transfer |
| 0x140006fc0 | 58B | read_register_prefix | Reads 0x22, stores word at extension+0x58 |
| 0x140006f84 | 58B | restore_register_prefix | Restores 0x22 from extension+0x58 |

## Register Values — authoritative

Verified values (correcting previous wrong conclusions):
- **0x45** = literal SPI opcode: 0x0B (read) / 0x02 (write)
- **0x48** = TX byte count (read: 3 · write: tx_len)
- **0x4B** = RX byte count (read: rx_len · write: 0)
- **0x47** = trigger V2 (bit 7 = 0x80)
- **CTRL0 secret bits** = 0x60040000 (bit 30+29+18)
- **CTRL0 bit 23 (0x00800000)**: NOT written by amdspi.sys. The string `0x800000` doesn't appear in ANY function.
- **FIFO base = 0x80**, FIFO size = 70 (0x46), chunk 64
- **0x1D** = ALT_CS, **0x49/0x4A** = NOT USED in amdspi.sys (zero occurrences in the binary)
- **0x44** = **NOT read-only**: the write path writes a 16-bit WORD. Formula: `(read16(0x44) & 0x00FF) | (n<<8) | (n<<12)`, n = extension[0x20] & 0xF. The write16 overwrites 0x45 → Windows RE-writes 0x45 right after.

## Key findings from independent verification (amdspi-decomp-report.md)

1. **Write path (0x54d0) does NOT set RX_COUNT>0** and does NOT read FIFO. Write is TX-only.
2. **TX and RX are separate WDF requests** — two triggers. DESCREQ = write 0x02 then read 0x0B.
3. **RX offset = 0x80 + TX_COUNT** (0x83 for 0x3c20, 0x84 for 0x4bac with RX_COUNT+1).
4. **0x49/0x4A strobes DO NOT EXIST** in amdspi.sys. Zero byte hits in entire binary.
5. **Secret bits applied ONCE per segment**, not twice.
6. **STATUS (0x4C) poll is a no-op** — reads byte, tests bit31 of a byte. The real busy is CTRL0 bit31.
7. **FIFO clear is a single set** (no toggle clear→set). If HW needs toggle, that's a Linux-side discovery.
8. **ALT_CS: |0x01 on entry, only &0xFC on exit.**
