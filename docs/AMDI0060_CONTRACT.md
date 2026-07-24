# AMDI0060 Controller Contract

## Scope And Boundary

This is the current implementation contract for the Surface Laptop 4 AMD FCH
SPI controller: ACPI `AMDI0060`, V2 register path, and its `0xFEC10000` MMIO
resource. ACPI evidence assigns the MSHW0231 child logical CS 0, SPI mode 0,
33.33 MHz, and GPIO `0x55` (`docs/acpi/dsdt.dsl`; `docs/EVIDENCE.md`).

The controller owns PIO/MMIO setup, chip selection, speed selection, FIFO
transfer execution, and controller error returns. `MSHW0231` HID-over-SPI V0
sequencing, descriptor negotiation, IRQ use, feature activation, report
interpretation, and raw-mode behavior belong to `sl4a-spi-hid`; see
`docs/HIDSPI_PROTOCOL.md` and `docs/ACTIVATION.md`.

## Activation And Binding

- The controller intentionally exports no ACPI modalias. It is loaded only by
  an explicit, post-login `modprobe sl4a-spi-amd` (`driver/spi-amd.c`).
- The supplied activation path is `tools/sl4a-touch.sh activate`. It checks for
  exactly one controller and touch ACPI node, refuses to displace an existing
  driver, loads controller then transport, verifies both bindings, and removes
  modules it loaded if activation fails.
- Neither SL4A module exports a device-table modalias or installs a boot-time
  binding rule. This prevents automatic module loading/binding; once a module
  is explicitly loaded, its registered match table can bind a matching device.

## Ownership And MMIO Rules

- Before changing speed, selecting or clearing CS, executing an opcode, and at
  segment entry, the controller checks `SPI_MISC_CNTRL` (`MMIO+0xfc`) bit 10.
  PSP ownership returns `-EBUSY` instead of performing the guarded operation.
- `host->setup` performs no frequency or MMIO programming. Frequency selection
  is deferred to the guarded message transfer path, after the ownership check.
- A segment saves the 16-bit `SPI100_SPEED_CONFIG` prefix at `MMIO+0x22` and
  restores it after execution. It restores it on an execution error only if
  PSP ownership has not appeared. This mirrors the recorded Windows decomp
  behavior (`docs/AMDSPI_DECOMP.md`).
- Frequency changes preserve firmware state: SPI100 is set only for 100 MHz,
  and SPD7 is written only for selected SPD7 speeds. Lower-speed requests do
  not clear either field. The source records prior FCH stalls when they were
  cleared (`driver/spi-amd.c`; `tests/spi_amd_ownership_safety_test.py`).

## Chip Select, Transfers, And Timeouts

- The controller exposes one logical chip select and currently forces ALT_CS
  bits to physical CS1 before each message. This matches the Windows `0x1d`
  strobe shape, but target-machine evidence only proves ACPI logical CS0.
  Physical CS1 is an implementation/board quirk, not portable AMDI0060
  evidence (`docs/EVIDENCE.md`).
- The PIO TX FIFO limit enforced by the driver is 70 bytes per segment. Reads
  are drained in chunks of at most 64 bytes; advertised maximum transfer and
  message sizes are 65536 bytes because the host chunks internally.
- V2 uses opcode `0x02` for TX-only writes and `0x0b` for reads. Reads program
  `RX_COUNT` as requested bytes plus one; the driver strips a trailing `0xff`
  read clock from the submitted TX shape. The `0x44` 16-bit speed write can
  overwrite opcode `0x45`, so the opcode is written again before trigger.
- Before and after trigger, the controller polls CTRL0 busy bit 31 every
  20 microseconds for up to 2 seconds. A timeout is returned and logged.
  PSP acquisition during cleanup avoids a CS-register write and reports
  `-EBUSY` when no earlier error exists.

## Lifetime And Recovery

- Controller removal unregisters its SPI controller. It does not promise a
  bus reset, state rollback beyond per-segment `0x22` restoration, or recovery
  from a timeout.
- The supported experiment recovery path is unloading `sl4a-spi-hid` before
  `sl4a-spi-amd`, then rebooting. Keep local or remote recovery access before
  activation (`tools/sl4a-touch.sh activate`; `README.md`).

## Not Proven

- This document is not functional qualification. No release-qualified AMDI0060
  hardware matrix result is recorded.
- Physical CS1, the selected speed behavior on target hardware, timeout
  recovery, suspend/resume, repeated reload, and interoperability with other
  FCH clients remain unproven by an E1 result.
- The controller contract does not establish that MSHW0231 activation, HID
  transport, raw streaming, or touch behavior is reliable. Open transport and
  raw-mode discrepancies remain in `docs/EVIDENCE.md`.
