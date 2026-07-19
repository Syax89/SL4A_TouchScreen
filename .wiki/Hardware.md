# Hardware Reference

## Device: MSHW0231 (045E:0C19)

Surface Laptop 4 (AMD) touchscreen controller, connected via SPI bus
on the AMD Cezanne (Ryzen 4000/5000 Mobile) FCH.

### Specifications

| Property | Value |
|----------|-------|
| HID Vendor ID | 0x045E (Microsoft) |
| HID Product ID | 0x0C19 |
| ACPI ID | MSHW0231 |
| Touch grid | 72 columns × 48 rows (3456 cells) |
| Technology | Mutual-capacitance projective touch |
| Report rate | ~100 Hz (raw mode) |
| SPI clock | 12 MHz (initial), configurable |
| SPI mode | 0 (CPOL=0, CPHA=0) |
| Chip select | 0 |

### SPI Wiring

The touch controller sits on the AMD FCH SPI bus 1:

```
AMD Cezanne FCH (SPI1)               MSHW0231
───────────────────────               ────────
CS#0        ─────────────────────── → CS
SCLK        ─────────────────────── → SCLK
MOSI (SDO)  ─────────────────────── → MOSI
MISO (SDI)  ─────────────────────── → MISO
GPIO (IRQ)  ←───────────────────────  INT (data-ready)
```

### GPIO

The device uses a GPIO for data-ready interrupts:
- GPIO pin: declared in ACPI `_CRS` as `GpioInt`
- Edge-triggered, active-low
- Signals that a new input report is available for reading

### ACPI Power Resources

```
Device (TPD0) {
    Name (_PR0) { P0A0 }  // D0 requires power resource P0A0
    Name (_PR3) { P0A0 }  // D3 requires power resource P0A0
    Method (_PS0) { ... }  // Power on (D0)
    Method (_PS3) { ... }  // Power off (D3)
}
```

The driver uses `_PS0` / `_PS3` for power transitions but NEVER calls `_RST`
(M010), which physically destroys the device on this hardware.

## AMD FCH SPI Controller

The Cezanne FCH integrates a multi-mode SPI controller supporting
PIO (Programmed I/O) and DMA modes. The Linux driver uses V2 PIO mode.

### Register Map (Base: MMIO, offset from PCI BAR2)

| Register | Offset | Width | Description |
|----------|--------|-------|-------------|
| CTRL0 | 0x00 | 32 | Control register 0 (TX/RX counts, mode, enable) |
| CTRL1 | 0x04 | 32 | Control register 1 (chip select, speed index) |
| CMD | 0x08 | 32 | Command (opcode) |
| ADDR | 0x0C | 32 | Address extension |
| TX | 0x48 | 32×71 | TX FIFO (71 slots, 32-bit each) |
| RX | 0xB0 | 32×71 | RX FIFO (71 slots, 32-bit each) |
| OPCODE | 0x04 in FIFO | 8×8 | Opcode FIFO (8 slots, 8-bit each) |
| STATUS | (varies) | 32 | Busy/error status |
| SPEED_CONFIG | (varies) | 32 | SPI100 speed config |

### PIO Mode Transfer Sequence (V2)

1. **Select chip** — set CS#0, configure CTRL1 with speed index
2. **Write opcodes** — program opcode FIFO with read/write commands
3. **Set TX_COUNT** — program PIO byte count for write phase
4. **Write TX FIFO** — push data to transmit
5. **Trigger execute** — set EXEC_OPCODE bit in CMD register
6. **Wait for FIFO ready** — poll TX/RX FIFO ready bits
7. **Set RX_COUNT** — program PIO byte count for read phase
8. **Read RX FIFO** — pop received data
9. **Check STATUS** — verify no bus errors

### Speed Configuration

| Speed Index | Clock | Notes |
|-------------|-------|-------|
| 0 | 12.5 MHz | Default / safe speed |
| 1 | 25 MHz | Requires SPI100 mode |
| Higher | Up to 100 MHz | Requires evaluation |

The Windows driver uses speed index 0 (12 MHz nominal) for normal
operation. The Linux driver follows this.

### TX_COUNT Quirk (PIO)

For PIO reads, the AMD SPI V2 controller requires TX_COUNT=3
(not 0) to correctly trigger the full-duplex read phase.
This matches Windows `amdspi.sys` decompilation at function 0x4bac.
Without this quirk, the first 3 bytes of received data are corrupted.

TX_COUNT must always leave at least 4 bytes of FIFO space for the
controller's internal status word (`TX_COUNT ≤ FIFO_DEPTH - 4`).
Larger reads must be split into 64-byte chunks.

## References

- `docs/SPI_REGISTERS.md` — Complete register map and bit definitions
- `docs/decomp/clean/0x4bac.txt` — AMD SPI read handler decomp
- `docs/decomp/clean/0x3c20_transfer.txt` — Main transfer function decomp
- `docs/decomp/uefi/AmdSpiHcProtocolDxe.c` — UEFI AMD SPI driver
- AMD Cezanne FCH BIOS Specification (NDA)
