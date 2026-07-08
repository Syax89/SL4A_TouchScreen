# Diagnostic kernel modules

Standalone kernel modules used during the Surface Laptop 4 touchscreen
(MSHW0231, HID-over-SPI) reverse-engineering project to poke the AMD FCH SPI
controller (0xFEC10000), FCH GPIO block (0xFED81500), and ACPI GPIO methods
directly, bypassing the Linux SPI/HID framework. Findings are logged in
`/home/simone/spi-hid/docs/GROUND_TRUTH.md` ("GROUND_TRUTH" below).

Build each standalone: `make LLVM=1 -C /lib/modules/$(uname -r)/build M=$PWD modules`
(the `Kbuild` in this directory lists the modules below as `obj-m` targets),
or copy a single `.c` file into a scratch directory with a minimal
`obj-m += <name>.o` Kbuild.

**2026-07-08 breakthrough note**: the actual DEVICE_DESC/RPT_DESC breakthrough (GROUND_TRUTH.md
§16) was found in the real driver (`driver/spi-hid-core.c`'s `spi_hid_seq_thread()`), not in
any tool in this directory — the raw-MMIO tools below reproduce the byte-identical DESCREQ
wire format correctly, but the fix that actually unlocked it (a doubled leading opcode byte
in the buffer passed to the driver's own `spi_hid_seq_write()`) is specific to that function,
not reproduced here. See `docs/NEXT_STEPS.md` for the current priority list.

## Active tools

These are the tools whose output is cited in GROUND_TRUTH.md and that still
back a live conclusion.

- `irq_oracle.c` — watches GPIO pin 85 (AMDI0031 interrupt line) directly
  instead of inferring write success from MISO content, timing the edge
  after a DESCREQ; later extended with `set_spi_freq()` for the clock-speed
  sweep. Supports GROUND_TRUTH.md §15.9 (IRQ-Line Oracle) and §15.10 (SPI
  clock speed sweep) — the tool that established the device always answers
  DESCREQ with `type=3` (RESET_RSP), never `type=7` (DEVICE_DESC), at every
  speed. Also used in §15.20 to independently confirm the operational
  incident (0 spontaneous edges) during the UEFI-replica testing.
- `ps3_ps0_cycle.c` — a real ACPI `_PS3`→`_PS0` power cycle (distinct from
  the destructive raw GPIO/M010 power cycle). Supports GROUND_TRUTH.md
  §15.20: used as the clean, non-destructive recovery method when the
  device was found held in reset mid-session (pin103 read 0); recovery was
  confirmed by the idle cadence resuming immediately.
- `uefi_exact_replica_test.c` — four variants layered onto the proven
  DESCREQ path, replicating `AmdSpiHcProtocolDxe.efi`'s real `Transfer()`
  path byte-for-byte (RMW trigger write, STATUS-register busy-wait,
  skipping the FAST_READ mode bits, and all three combined). Supports
  GROUND_TRUTH.md §15.20 — exhaustively closed every remaining
  Windows/UEFI/Linux structural delta with no change in outcome.
- `uefi_gpio_replay_test.c` — replays the UEFI GPIO-toggle loop (set/clear
  bit22) verbatim around the driver's own proven `do_read`/`do_write`
  helpers, edge-synced. Supports GROUND_TRUTH.md §15.18 — ruled out the
  UEFI GPIO sequencing as the missing piece.
- `fifo_clear_singleset_test.c` — tests the Windows/UEFI-style FIFO_CLEAR
  single-set (vs Linux's toggle) in isolation. Supports GROUND_TRUTH.md
  §15.10/§15.18 — confirmed no effect, closing that structural delta.
- `mmio_write.c` — the original Windows-exact write sequence (opcode 0x02)
  with a full 70-byte FIFO dump before/after. Supports GROUND_TRUTH.md §4
  (FIFO Diagnostics) and is the module in the §11/§14 tool reference
  tables — established that the device's MISO stays at zero/undriven for
  the entire write, with no ACK or error.
- `m009_read.c` — read-only ACPI evaluate of `\M009` for GPIO pins 0x5B
  (power gate) and 0x103 (reset line); never writes. Matches the
  "read-only ACPI GPIO-value tool" used in GROUND_TRUTH.md §15.20 to
  discover the reset line was held low (pin103 = 0) during the mid-session
  incident, ahead of the `ps3_ps0_cycle` recovery. (Cited there by
  description, not filename — the register reads it performs are an exact
  match.)
- `health_check.c` — one-shot dump of CTRL0, STATUS, ENA_REG and the raw
  pin85 GPIO value. Matches the register set GROUND_TRUTH.md §15.20 reports
  checking during the same incident ("CTRL0/STATUS/ENA_REG all read back as
  ordinary, non-corrupted values") to rule out the earlier "corrupted
  CTRL0" pattern. (Cited there by description, not filename.)
- `baseline_check.c` — passive 2000ms watch of pin85 with zero writes,
  counting spontaneous edges. Matches the "0 spontaneous edges in a full
  2000ms passive observation window" measurement GROUND_TRUTH.md §15.20
  reports taking when the device went silent mid-session. (Cited there by
  description, not filename.)
- `reactive_descreq_test.c` — reacts to a freshly-observed RESET_RSP edge
  with zero artificial delay (read header, read body, send DESCREQ),
  replicating Windows's ~300us reaction profile from a real boot ETW trace
  instead of firing DESCREQ blind like every other tool here. Supports
  GROUND_TRUTH.md §15.21 — falsified the "reaction-window" hypothesis (we
  react faster than Windows, 95-157us, and still never get DEVICE_DESC).

## Unclear

- `misc_cntrl_read.c` — read-only dump of `SPI_MISC_CNTRL` (MMIO 0xFC),
  checking the PSP/HFP/HBIOS ownership semaphore bits. Built and safe
  (read-only, per its own header comment), but no result attributed to it
  — by name or by content — appears anywhere in GROUND_TRUTH.md; the
  closest related material (§15.5, the "AMD SPI Lock" mechanism) covers a
  different set of registers (RomProtect, ALT_CS, SPIRestrictedCmd) and
  never mentions MISC_CNTRL/0xFC or the ownership bits. Left in place
  (not archived) pending a decision on whether it was actually run.

## Archive

`archive/` holds early one-off exploratory probes (mostly from the first
sessions of this project) whose results are either never mentioned in
GROUND_TRUTH.md at all, or whose specific hypothesis is only covered
implicitly by the generic "Complete Matrix of Failed Tests" summary table
in §3 — not attributed to any one tool by name. They are kept for history,
not deleted, but none of them should be treated as authoritative: the
current understanding superseded them, mainly via `irq_oracle.c`'s
edge-synced methodology (§15.9), which found and fixed correctness bugs
(9-vs-10-byte header reads, unprimed "baseline" state) that these earlier,
simpler probes did not account for.

Contents: `bytebang.c`, `clone_win.c`, `companion_probe.c`, `ctrl1_test.c`,
`descreq_test.c`, `diff_fifo.c`, `mmio_trace.c`, `more_ideas.c`,
`oneshot.c`, `pci_smn_diag.c`, `power_cycle.c`, `radical3.c`,
`rapid_fire.c`, `regdump.c`, `reset_test.c`, `skip_descreq.c`,
`smn_write_test.c`, `step_trace.c`, `test_raw.c`, `tx16_test.c`,
`ultimate.c`.

`oneshot.sh` and `oneshot_boot.sh` (still in this directory, not archived)
have been updated to invoke `archive/oneshot.ko` at its new path.

**2026-07-08 addition**: `cs0_test.c`, `fifo_mode_test.c`,
`final_solution_test.c`, `read_298.c` — found untracked in the working tree
(with matching `obj-m` lines already added to `Kbuild`, but never committed
and no result ever recorded in GROUND_TRUTH.md). These are more substantial
than the other archived probes and test two genuinely new, previously
undocumented hypotheses: **chip-select 0 vs 1** (`cs0_test.c`, `ALT_CS` bits
1:0 = 00 instead of the always-used 01) and **duplicating the opcode byte
inside the FIFO payload itself** (`fifo_mode_test.c`/`final_solution_test.c`,
sending an 11-byte transaction where the FIFO also contains a leading 0x02
in addition to the OPCODE register write) — the same "opcode duplicated"
idea was also found, uncommitted, in `driver/spi-hid-core.c` at the same
time and was reverted there as an incomplete/broken refactor (extra byte
shifts the whole DESCREQ address field). Archived here rather than deleted
since neither hypothesis has actually been run to a documented conclusion —
if you want to test chip-select or opcode-duplication-in-FIFO, these are a
starting point, not a closed line of investigation. `read_298.c` is a
one-shot read-only probe of `0xFED80298` (checking whether UEFI overrides
chip-select) with no result ever recorded either.
