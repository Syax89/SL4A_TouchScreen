# SL4A_TouchScreen — Linux Driver for Surface Laptop 4 AMD Touchscreen

Linux HID-over-SPI driver for the Microsoft Surface Laptop 4 (AMD) touchscreen
(ACPI HID `MSHW0231`, SPI controller `AMDI0060`).

Windows supports this hardware out of the box via `amdspi.sys` + `hidspicx.sys`.
This project reverse-engineers the Windows driver protocol to implement a native
Linux kernel driver.

## Status

**Work in progress.** The SPI bus communication works (V2 path, FIFO reads/writes
verified), but the device initialization handshake is not yet complete. The
driver can talk to the touchscreen controller and receives `RESET_RSP` from the
device, but the device descriptor discovery (`DESCREQ`) is still being debugged.

## Architecture

```
┌─────────────────────────────────────────────┐
│ spi-hid.ko (spi-hid-core.c)                 │
│  - HID-over-SPI protocol state machine      │
│  - Device discovery (DESCREQ/DESCREQ2)       │
│  - Firmware upload (B0 blocks)              │
│  - HID report forwarding                    │
└───────────────┬─────────────────────────────┘
                │ Linux SPI framework
┌───────────────┴─────────────────────────────┐
│ spi-amd.ko (spi-amd.c)                      │
│  - AMD FCH SPI controller V2                │
│  - FIFO management (70-byte FIFO at 0x80)   │
│  - Combined TX+RX transfers                 │
└───────────────┬─────────────────────────────┘
                │ MMIO
┌───────────────┴─────────────────────────────┐
│ AMD FCH SPI controller @ 0xFEC10000         │
│  - V2 path with registers 0x45-0x4B         │
│  - 16-bit FIFO access                       │
└─────────────────────────────────────────────┘
```

## Hardware

| Component | Detail |
|-----------|--------|
| Device | Surface Laptop 4 (AMD) |
| Touchscreen ACPI HID | `MSHW0231` (falls back to `PNP0C51`) |
| SPI Controller | `AMDI0060` (AMD FCH V2) |
| MMIO Base | `0xFEC10000` |
| FIFO Size | 70 bytes |
| GPIO IRQ | Pin `0x55`, Edge-triggered Active-Low |
| SPI Speed | 33.33 MHz |
| Wintouch Vendor/Product | `0x045E` / `0x0C19` |

## Key Source Files

| File | Contents |
|------|----------|
| `driver/spi-hid-core.c` | HID-over-SPI protocol implementation |
| `driver/spi-hid-core.h` | Protocol constants and data structures |
| `driver/spi-amd.c` | AMD SPI controller V2 driver |
| `driver/spi-amd.h` | SPI controller definitions |

## HID-over-SPI Protocol

The Microsoft HID-over-SPI protocol is documented in the public specification
(`docs/HidSpiProtocolSpec.pdf`). Key protocol elements:

- **Read Approval (0x0B)**: Read device data with register addressing
- **Output Write (0x02)**: Send commands (DESCREQ, DESCREQ2)
- **Activation (0x00, 0x70)**: Vendor-specific init commands
- **Firmware Upload (0xB0)**: 120 blocks × 241 bytes

### Device Initialization Sequence (from Windows CSV traces)

1. Device resets → sends `RESET_RSP` (type=3) via 0x0B read
2. After ~168 µs, device auto-advances → sends `ACK` (type=0)
3. Host sends `DESCREQ` (opcode 0x02) → device acknowledges inline
4. GPIO IRQ fires → host does 0x0B read → receives `DEVICE_DESC` (type=7)
5. Host parses device descriptor → sends `DESCREQ2` → receives `RPT_DESC` (type=8)
6. Activation commands (`cmd1`/`cmd2`/`cmd3`)
7. Firmware upload (120 B0 blocks + tail)
8. ~41s initialization → runtime HID reports

## Building

```bash
# Build SPI controller driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$HOME/spi-amd-v2-multi modules

# Build HID driver
make LLVM=1 -C /lib/modules/$(uname -r)/build M=$HOME/spi-hid/module modules
```

## Loading

```bash
# Unload old modules
sudo rmmod spi_hid spi_amd 2>/dev/null

# Load new modules
sudo insmod ~/spi-amd-v2-multi/spi-amd.ko
sudo insmod ~/spi-hid/module/spi-hid.ko

# Or install to DKMS
sudo cp ~/spi-amd-v2-multi/spi-amd.ko ~/spi-hid/module/spi-hid.ko \
        /lib/modules/$(uname -r)/updates/dkms/
sudo depmod -a
sudo modprobe spi_hid
```

## Debugging

```bash
# Monitor driver activity
sudo dmesg -w | grep -E "SEQ|spi-amd"

# Trigger module reload
sudo rmmod spi_hid; sudo rmmod spi_amd
sudo insmod ~/spi-amd-v2-multi/spi-amd.ko
sudo insmod ~/spi-hid/module/spi-hid.ko
```

## Fixes Applied

Verified against Windows decomp (`amdspi.sys`, `hidspicx.sys`) and ETW CSV traces:

| Fix | Description |
|-----|-------------|
| C1 | ALT_CS encoding: use `AND 0xFC, OR 0x01` like Windows |
| C2 | Secret bits applied per-segment in `exec_segment` |
| C4 | `memcpy` offset `+4` in descriptor parse |
| C5 | `input_register` default `0x0000` (was `0x1000`) |
| C6 | `approval7 = 0x0A` at runtime |
| C7 | `approval8 = 0x00` always |
| FIFO | 16-bit reads via `readw`, offset `+tx_len` |

## Documentation

- `docs/SPI_REGISTERS.md` — AMD FCH SPI register map, CTRL0 bits, V1/V2 paths
- `docs/AMDSPI_DECOMP.md` — `amdspi.sys` decompilation analysis
- `docs/HIDSPI_PROTOCOL.md` — HID-over-SPI protocol and state machine
- `docs/CSV_SEQUENCE.md` — Windows boot sequence from ETW traces
- `docs/DRIVER_STATE.md` — Current driver status and bug tracker

## License

Dual GPL-2.0 / BSD-3-Clause. See source file headers.
