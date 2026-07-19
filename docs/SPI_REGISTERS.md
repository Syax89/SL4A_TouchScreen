# AMD Cezanne FCH SPI Controller V2

Register map and operational model for the AMD FCH SPI controller used
by the Surface Laptop 4 (AMD) touchscreen. The Linux driver uses PIO V2
mode with the register layout documented below.

## MMIO Register Map

Base address: `0xFEC10000` (SPI bus 1, chip select 0).

### Core Registers

| Offset | Name | Size | Access | Description |
|--------|------|------|--------|-------------|
| 0x00 | CTRL0 | 32 | R/W | Main control register (mode, TX/RX counts, enable) |
| 0x04 | CTRL1 | 32 | R/W | Control register 1 (chip select, speed index) |
| 0x0C | ADDR | 32 | R/W | Address extension |
| 0x20 | ENA_REG | 32 | R/W | Enable register (SPI100, ALT_SPD at bits 20-23) |
| 0x22 | SPI100_SPEED_CONFIG | 16 | R/W | Speed tier selection |
| 0x2C | SPI100_HOST_PREF | 32 | R/W | Host prefetch config |

### Command and Transfer Registers

| Offset | Name | Size | Access | Description |
|--------|------|------|--------|-------------|
| 0x44 | SPEED_OPCODE | 16 | R/W | High byte: speed sel, low byte: opcode |
| 0x45 | OPCODE_REG | 8 | R/W | V2 transfer opcode |
| 0x47 | CMD_TRIGGER | 8 | R/W | Bit 7 = start V2 transfer |
| 0x48 | TX_COUNT | 8 | R/W | PIO bytes to transmit |
| 0x4B | RX_COUNT | 8 | R/W | PIO bytes to receive |
| 0x4C | STATUS | 8 | R | FIFO state (pointers, done count) |
| 0x6C | SPEED_REG | 32 | R/W | V2 speed (spd7 at bits 8-13) |
| 0xFC | SPI_MISC_CNTRL | 16 | R/W | Bus ownership semaphore |

### FIFOs

| Offset | Name | Capacity | Access |
|--------|------|----------|--------|
| 0x80-0xC6 | Data FIFO | 71 bytes | R/W |
| 0x04 (within FIFO range) | Opcode FIFO | 8 slots × 8-bit | W |

## CTRL0 Register Bit Map (0x00)

```
Bit 31    : BUSY (read-only)
Bit 30-28 : SPI_READ_MODE (3-bit)
Bit 27    : SPI_ARB_ENABLE
Bit 26    : SPI_FIFO_PTR_CLR
Bit 25    : SPI_FIFO_AUTO_CLR
Bit 24    : SPI_RX_FIFO_RESET
Bit 23    : SPI_TX_FIFO_RESET
Bit 22    : SPI_BUSY_WAIT_EN        ← Must be 1 for V2
Bit 21    : SPI_START
Bit 20    : reserved
Bit 19-16 : SPI_READ_BYTE_COUNT (4-bit, RX_COUNT post-transfer)
Bit 15-12 : SPI_WRITE_BYTE_COUNT (4-bit, TX_COUNT post-transfer)
Bit 11-10 : SPI_IO_MODE (2-bit: 0=PIO, 1=DMA)
Bit 9-8   : SPI_OPCODE_BYTE_COUNT (2-bit: 0-3 opcodes)
Bit 7-6   : SPI_CS_SELECT (2-bit)
Bit 5     : SPI_MODE (0=SPI, 1=I2C)
Bit 4     : SPI_MASTER (1=controller mode)
Bit 3     : SPI_MODE_SEL (CPOL/CPHA)
Bit 2     : SPI_CS_ACTIVE
Bit 1     : SPI_ENABLE
Bit 0     : reserved
```

## V2 Transfer Sequence

### Read Transfer (PIO)

```
1. Select chip (CS#0, configure CTRL1 speed index)
2. Program opcode FIFO (read command + register address)
3. Set TX_COUNT = 3                     ← PIO quirk: must be 3, not 0
4. Write TX FIFO (3 dummy bytes)
5. Trigger EXEC_OPCODE (CMD_TRIGGER bit 7)
6. Poll: wait for bus idle + FIFO ready
7. Set RX_COUNT = n_bytes_to_read
8. Wait for RX_COUNT done (STATUS register)
9. Read RX FIFO (n bytes)
10. Check STATUS for bus errors
```

### Write Transfer (PIO)

```
1. Select chip, program opcode FIFO
2. Set TX_COUNT = n_bytes (leave ≥4 bytes headroom for FIFO status)
3. Write opcode + payload to TX FIFO
4. Trigger EXEC_OPCODE
5. Poll: wait for bus idle + TX FIFO empty
6. Check STATUS for bus errors
```

## TX_COUNT Constraints

- **PIO read**: TX_COUNT must be 3 (not 0) to correctly trigger the
  full-duplex read phase. Matches Windows `amdspi.sys` decomp (0x4bac).
  Without this, the first 3 RX bytes are corrupted.

- **FIFO headroom**: TX_COUNT + reserved_status_bytes ≤ FIFO_DEPTH.
  On Cezanne the FIFO is 71 bytes with a 4-byte status word, so
  TX_COUNT ≤ 67.

- **Chunking**: Large reads (>64 bytes) must be split into 64-byte chunks
  with separate transfers.

## Speed Control

| Speed Index | Nominal Clock | SPI100 Register Bits |
|-------------|---------------|---------------------|
| 0 | 12 MHz | NORM[2:0] = 0 |
| 1 | 25 MHz | NORM[2:0] = 4 |
| 2 | 33 MHz | NORM[2:0] = 1 |
| 3 | 50 MHz | NORM[2:0] = 5 |
| 4 | 66 MHz | NORM[2:0] = 2 |
| 5 | 100 MHz | NORM[2:0] = 6 |

The driver uses speed index 0 (12 MHz) for normal operation. The
Windows driver uses the same speed.

## SPI_MISC_CNTRL (0xFC) Semaphore

```
Bit 10 : PSP_OWNS (Platform Security Processor)
Bit 9  : HFP_OWNS (Host Firmware Processor)
Bit 5  : HBIOS_OWNS (Host BIOS)
```

The driver checks bit 10 before accessing the SPI controller. If the
PSP owns the bus, the driver cannot safely touch it.

## References

- AMD Cezanne FCH BIOS Specification (NDA, register definitions)
Reverse-engineering source material is retained locally but excluded from
this repository. See `docs/decomp/` for decompilation notes.
