# Raw MMIO diagnostic modules

Standalone kernel modules used during the investigation to poke the AMD FCH
SPI controller (0xFEC10000) directly, bypassing the Linux SPI framework.
Build each with `make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD/<file without .c>.o modules`
or copy into a scratch directory with a minimal Kbuild (`obj-m += <name>.o`).

- `mmio_write.c` — Windows-exact write sequence (opcode 0x02) + full 70-byte FIFO
  dump before/after, used for the WREN (0x06) and SET_POWER (0x07) experiments.
- `regdump.c` — one-shot dump of all named SPI controller registers.
- `test_raw.c` — bare reads at multiple register offsets, no HID framing.
