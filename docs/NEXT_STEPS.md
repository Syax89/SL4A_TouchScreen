# Next Steps — Post-Breakthrough Roadmap (2026-07-08)

> Written cold, after consolidating the 2026-07-08 breakthrough (see `GROUND_TRUTH.md` §16).
> This is a working plan, not a status report — update it as items get resolved instead of
> appending a new document each session.

## Where we actually are

For the first time in this project, real hardware returns `DEVICE_DESC` and `RPT_DESC`
instead of looping `RESET_RSP` forever. The fix is a one-byte change (`spi_hid_seq_write()`
needs its TX buffer's opcode doubled) inside `driver/spi-hid-core.c`'s experimental `SEQ`
sequencer. It is **not** a finished driver: the device reliably reaches the fully-parsed
descriptor state, creates real Linux HID input devices, and then resets on its own
~507-508ms later — every single time, regardless of what we do in that window. Zero real
touch (`type=1`) reports have ever been observed.

The mystery has *changed shape*, not disappeared: it's no longer "why does every write get
ignored" (closed, §15.20), it's "why does our device reset ~508ms after RPT_DESC when
Windows's doesn't reset for at least 5.9 seconds under the same apparent conditions."

## Priority 1 — Bisect the restored diff (mechanical, no new hypotheses needed)

The uncommitted diff that produced the breakthrough bundled several unrelated changes.
Only one of them (the doubled opcode) is understood to matter; the rest were restored
wholesale because untangling them wasn't the point in the moment. Now it is:

- [ ] Toggle `spi-amd.c`'s PCI 0xB8/0xB4 FIFO-mode forcing (8-bit + Windows B4 layout) off,
      keep everything else — does RPT_DESC still happen?
- [ ] Toggle the `readb`-vs-`readw` FIFO read change back to `readw` alone — same test.
- [ ] Toggle the ACPI `_PS3`→`_PS0` probe-time power-cycle off — same test.
- [ ] Toggle `spi_hid_ll_open()`/`spi_hid_ll_close()` back to their full (non-gutted)
      implementation, now that we know `spi_hid_power_up()`/`assert_reset`/`deassert_reset`
      are otherwise dead code — does that change anything about the ~508ms reset?

Do these one at a time, not all at once — the goal is a clean isolation, not a faster path
back to "it works," since we already have "it works (partially)" as a baseline.

## Priority 2 — Understand *why* the doubled opcode is needed

Read `amd_spi_exec_segment()` in `driver/spi-amd.c` side by side with what
`spi_hid_seq_write()` sends. The raw-MMIO diagnostic tools (`tools/diagnostics/irq_oracle.c`
etc.) build the same 10-byte wire frame without any doubling and it works fine at that
level — so the extra byte is compensating for something specific to how
`amd_spi_exec_segment()` counts/consumes the opcode when called via the normal
`spi_sync()`/`spi_message` path, not a protocol requirement. This is a code-reading task,
not an experiment. Understanding it properly might also explain the ~508ms reset for free
(e.g. if `amd_spi_exec_segment()` is quietly doing something extra on every call that a raw
MMIO poke doesn't).

## Priority 3 — The ~508ms reset itself

In order of cheapness to test, once Priority 1 has produced a minimal reproducer:

1. Read CTRL0/STATUS/ENA_REG on a timer *during* the ~508ms idle window (a
   `health_check.c`-style read, called repeatedly, not once) — do any of them drift or
   change value before the reset fires? If something changes, that's the smoking gun.
2. Re-run the SPI clock-speed sweep (§15.10) but *after* reaching RPT_DESC instead of
   before DESCREQ — does the ~508ms figure move with clock speed? If yes, it's bus-timing
   related; if it stays fixed in wall-clock terms regardless of SPI clock, it's a
   device-internal timer unrelated to our bus activity.
3. Check chip-select behavior specifically: does `amd_spi_exec_segment()` release CS
   between transactions in a way that differs from what the raw-MMIO tools do? A
   continuously-asserted or oddly-toggled CS line during idle could plausibly cause a
   device-side timeout distinct from anything content-related.
4. Only after 1-3: consider whether this needs a logic analyzer. Given how tight and
   reproducible the ~508ms figure is (sub-millisecond variance across 60+ cycles), it is
   very likely explainable from software once the right register/line is watched — this
   is a stronger position than the "needs physical instrumentation" conclusion §15.20
   reached, precisely because we now have a live, repeatable, fast (~1-2s per cycle)
   failure to instrument against, instead of an eternal unbroken RESET_RSP loop.

## Explicitly ruled out (don't retest without new evidence)

- Reaction speed to a freshly-observed RESET_RSP (§15.21) — we react faster than Windows
  and it doesn't matter.
- `spi_hid_set_power(ACTIVE)` immediately after RPT_DESC — no effect on the ~508ms timer.
- Exact byte-for-byte GET_FEATURE/SET_FEATURE replica from `surface_boot_auto.csv` sent
  immediately after RPT_DESC — no effect, and the response never even arrives.
- Physical touch during the live cycle — no effect, 12/12 cycles identical whether touched
  or not.
- Every register-level Windows/UEFI/Linux structural delta enumerated in §15.17-§15.20 —
  closed before the doubled-opcode discovery, still valid: none of them explain content
  differences, because content was never actually the problem for the specific 4 core
  commands (DESCREQ/DESCREQ2) — see §16.2.

## Operational notes for whoever picks this up

- The winning driver state is **not yet committed as "the" driver** — it's restored in
  `driver/spi-hid-core.c`/`spi-amd.c`/`spi-hid-core.h` with heavy `SEQ:` diagnostic logging
  intact, still clearly experimental (see the `spi_hid_seq_thread()` state machine,
  states 0-5). Don't mistake this for a finished, mergeable driver.
- Recovering from a "device went silent" incident: use `tools/diagnostics/ps3_ps0_cycle.c`
  (real ACPI `_PS3`→`_PS0`, non-destructive, proven in §15.20) — never raw M009/M010 pokes
  (§10, permanently breaks the device until a full reboot).
- `tools/parse_spi.py`'s `TXN#N` numbering interleaves Device A (touchscreen) and Device B
  (companion chip) by wall-clock time — the "+Xus" gap column is relative to whichever
  transaction (either device) immediately precedes it, **not** necessarily the same
  device's previous transaction. For real elapsed-time-on-one-device questions, diff the
  raw `clock_100ns` values directly (see §16.4's worked example) instead of trusting the
  printed gap column.
