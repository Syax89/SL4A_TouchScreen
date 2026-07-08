# Next Steps — Post-Breakthrough Roadmap (2026-07-08)

> Written cold, after consolidating the 2026-07-08 breakthrough (see `GROUND_TRUTH.md` §16).
> This is a working plan, not a status report — update it as items get resolved instead of
> appending a new document each session.

> **2026-07-08, later same day: Priority 1 (all 4 items) and Priority 3 (items 1-2) done.**
> See `GROUND_TRUTH.md` §17 for the full writeup. Short version: none of the four bundled
> changes matter, the reset timer doesn't drift beforehand, and it doesn't scale with SPI
> clock speed either — it's a fixed device-internal timer, not something the driver/bus can
> influence. Sections below are kept for history; checkboxes reflect what's now closed.
>
> Also found and fixed a real byte-level bug (§17.5): `DESCREQ2`'s Content ID byte was
> `0x02` instead of `0x00` (didn't match Windows's actual wire frame). Fixed, verified
> byte-identical now, but it changed nothing about the reset.
>
> **2026-07-08, evening — BREAKTHROUGH #2, see `GROUND_TRUTH.md` §18.** The "next candidate
> lead" mentioned above (full RPT_DESC byte diff) turned up a major bug: RPT_DESC's 940-byte
> body was being silently truncated to its first ~70 bytes by a single-segment PIO read that
> can't hold more (the 70-byte hardware FIFO), while still reporting success. Windows never
> hits this because `amdspi.sys` uses a separate DMA path for bulk reads >64B that we don't
> have. Fixed with chunked PIO reads instead (see §18.3) — not byte-perfect yet (~78% of
> content now matches Windows, up from ~7%), but enough to change the game: **GET_FEATURE now
> gets a real response for the first time ever, SET_FEATURE succeeds, and the device
> progresses further than ever before** — reaching a *new*, shorter ~109ms reset instead of
> the old ~508ms one. Priority 1-3 below are about the *old* ~508ms wall and should be
> considered historical/superseded until it's confirmed whether they still apply post-fix.
> The active priorities now are listed in §A below.

## §A — Active priorities (post-Breakthrough #2, 2026-07-08 evening)

1. [x] **DONE (2026-07-08, night): RPT_DESC's residual 14/940-byte corruption characterized
   and patched — wire descriptor now parses successfully on the first try.** A follow-up
   experiment (deliberately mis-sizing the first fetch chunk to 60 bytes) proved the defect
   is tied to the *device's own* 64-byte page structure (corruption widened but stayed
   anchored to the same absolute offsets), not to our chunk boundaries — so 64-byte-aligned
   fetches were already the right call, just not sufficient alone. Since the 14 bad positions
   are exactly known, always `0xFF`, and clock/timing-independent, `spi_hid_seq_thread()` now
   applies a minimal, targeted patch at just those 14 byte positions using
   `hardcoded_report_descriptor` as ground truth (values confirmed to match the real Windows
   capture independently found earlier). 922/936 bytes (98.5%) still come from the live wire
   read every boot. See `GROUND_TRUTH.md` §18.10. This unblocked §C (below): the wire
   descriptor now passes `hid_parse_report()` and `hid-generic`'s probe on the first attempt,
   every time, with the hardcoded-fallback retry mechanism (§18.9) now sitting unused as a
   pure safety net rather than the thing actually firing every boot.
   **Still genuinely unexplained** (not blocking, just an open curiosity): *why* this specific
   page-relative byte is unreliable on the AMD FCH SPI V2 controller/this touch chip — would
   need a logic analyzer to actually see the physical signal at that point in the transfer.
2. [x] **Characterize the new ~109ms reset — done, see `GROUND_TRUTH.md` §18.5.** It's not a
   passive timer: skipping `SET_FEATURE` entirely reverts the timing back to the old
   ~508ms figure, so *sending* SET_FEATURE actively triggers the fast reset. SET_FEATURE's
   own content was checked byte-for-byte against Windows and is exactly correct — so
   something else about our state at that point is what the device is rejecting.
3. [x] **Reframed by later findings, no longer an open mystery.** Item 1's fix (RPT_DESC
   now 98.5%+patched) shipped and the device still resets when idle and streams normally
   when touched — the reset isn't caused by RPT_DESC/GET_FEAT_RESP content after all. §18.8's
   finding explains it more simply: the device only sustains activity while something is
   actually happening (touch contact); with no contact and no further host command, it times
   out and resets, then comes back to life the moment real touch activity resumes. This
   matches ordinary idle/power-management behavior rather than a content-rejection bug.
4. **Huge find while investigating item 2**: the real Windows capture shows a continuous
   polling loop starting ~1.8ms after SET_FEATURE — a 9-byte `type=1` (DATA) header read
   immediately followed by a 221-byte content read, repeating every ~10ms, for as long as the
   trace runs. This is almost certainly the live touch-data stream itself (or periodic
   idle/empty frames). `spi_hid_seq_thread()`'s `case 4` already has full, working code to
   receive and forward this (`hid_input_report()`) — it has simply never been reached. Once
   item 1/3 are fixed, this loop should start appearing in our own captures; watching for it
   is the clearest signal of final success.
5. [x] **DONE (2026-07-08, late): the device bound to `hid-generic` for real, twice in a
   row, and a physical touch reached `case 4`'s `type == 1` handler for the first time ever
   — which crashed the kernel.** Root cause found and fixed: a stack buffer over-read in that
   handler (a wire-controlled length field wasn't bounded against the real 512-byte buffer
   capacity). See `GROUND_TRUTH.md` §18.7.
6. [x] **DONE (2026-07-08, night): MILESTONE — byte-identical, sustained, crash-free
   real-time protocol replication, confirmed against the real Windows capture.** After
   fixing item 5 properly (heap-allocated buffer sized to the report's real claimed length,
   capped at `max_input_length`), the device streams real `type=1` reports continuously at
   ~10ms intervals indefinitely with zero crashes and zero resets. Byte-for-byte identical in
   structure to Windows's own `surface_touch.csv` capture (same length, same content ID, same
   sensor baseline bytes). **Why no touch coordinates show up is now understood and is not a
   bug**: the steady-state report (`content_id=0x0C`, ~4302 bytes) is raw capacitive
   heat-map/sensor-image data, not a conventional numbered HID report — even Windows only
   gets small per-point reports at contact start/end (2 at the start, 10 at the end of the
   whole `surface_touch.csv` capture; continuous touch is 537 large frames with zero small
   ones interspersed). See `GROUND_TRUTH.md` §18.8. **This closes out the protocol/wire-level
   phase of this project.** What's left is a different kind of work — see §C below.

## §C — DONE: prefer the real device descriptor over the hardcoded fallback

Raised explicitly (2026-07-08 night), implemented and verified live the same night. See
`GROUND_TRUTH.md` §18.9 for the full writeup. Summary:

- `struct spi_hid` now has `wire_report_descriptor[1024]` / `_len` / `_rejected`, populated
  in `spi_hid_seq_thread()`'s RPT_DESC handling (`case 2`, `type == 8`).
- `spi_hid_ll_parse()` tries the wire-read descriptor first, hardcoded only as fallback.
- The interesting finding: `hid_parse_report()`'s return code alone is **not** a reliable
  enough signal that the wire descriptor is good — it can return success while logging a
  lenient "unexpected long global item" warning for a structurally-corrupted descriptor, and
  the real failure only shows up later (and outside our function) when the driver core
  synchronously probes `hid-generic` against the broken parsed structure and *that* fails,
  without `hid_add_device()` itself returning an error. Fixed by checking `hid->driver ==
  NULL` after a "successful" `hid_add_device()` as the real failure signal, and retrying once
  with the hardcoded descriptor when that happens.
- **Update (same night, later): §A item 1 landed the 14-byte targeted patch, and the wire
  descriptor now parses and binds successfully on the first try, every time.** The
  hardcoded-fallback/retry path described above is confirmed working (it's what caught the
  failure *before* the patch existed) but now sits unused in normal operation — a pure safety
  net rather than the thing actually firing. 922/936 bytes come from the live wire read every
  boot; only the 14 known page-relative positions are patched from `hardcoded_rd.h`.

## §D — Future phase (separate, substantial): heat-map → touch-point extraction

Not started. Real touch coordinates during continuous contact require parsing the raw
`content_id=0x0C` capacitive sensor frame (§18.8) into touch points — this is an image
processing / blob-detection task (find contiguous regions deviating from the baseline
~`0xB4` cell value, compute centroids), not a protocol fix. Treat as its own project phase.

## §B — Historical (pre-Breakthrough #2), kept for reference below

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

- [x] Toggle `spi-amd.c`'s PCI 0xB8/0xB4 FIFO-mode forcing (8-bit + Windows B4 layout) off,
      keep everything else — does RPT_DESC still happen? **YES, no effect at all.** RPT_DESC
      still arrives every cycle, reset still fires at 507.5-508.3µs (30+ cycles sampled).
      This register pair is not required for anything we've observed.
- [x] Toggle the `readb`-vs-`readw` FIFO read change back to `readw` alone — same test.
      **Same result: no effect.** RPT_DESC arrives, reset timing unchanged.
- [x] Toggle the ACPI `_PS3`→`_PS0` probe-time power-cycle off — same test. **Same result: no
      effect**, tested via warm module reload (rmmod/insmod without a full cold reboot) — see
      the caveat in §17.1 about why this specific one should eventually be retested from a
      genuinely cold boot before fully ruling it out.
- [x] Toggle `spi_hid_ll_open()`/`spi_hid_ll_close()` back to their full (non-gutted)
      implementation, now that we know `spi_hid_power_up()`/`assert_reset`/`deassert_reset`
      are otherwise dead code — does that change anything about the ~508ms reset? **No
      measurable effect on the reset timer**, but it surfaced a real, separate bug: the HID
      core calls `hid_hw_open()`/`hid_hw_close()` in a tight repeating storm (many pairs per
      ~508ms cycle), presumably because `create_device_work` recreates the `hid_device` every
      single cycle and something re-probes it each time. Not chased further this session —
      see §17.1.

**Conclusion: none of the four bundled changes explain or affect the ~508ms reset.** The
doubled-opcode fix in `spi_hid_seq_write()` remains the only change that matters for reaching
RPT_DESC at all.

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

1. [x] Read CTRL0/STATUS/ENA_REG on a timer *during* the ~508ms idle window (a
   `health_check.c`-style read, called repeatedly, not once) — do any of them drift or
   change value before the reset fires? **No.** All three registers are rock-stable
   (`CTRL0=0x6f8c0a0b STATUS=0x000a0a09 ENA=0x11110713`) for the entire idle window, every
   cycle. The only time they change is in the exact instant our own driver executes the SPI
   read of the just-arrived RESET_RSP header — i.e. the "change" is our own bus read, not a
   precursor. There is no software-visible early warning in these registers. See §17.2.
2. [x] Re-run the SPI clock-speed sweep (§15.10) but *after* reaching RPT_DESC instead of
   before DESCREQ — does the ~508ms figure move with clock speed? **No — confirmed
   device-internal timer.** Tested 800kHz, 4MHz, and 33.33MHz (the ACPI default); all three
   give ~506.7-508.2ms with sub-millisecond spread, while individual per-transaction dt
   values scale visibly with clock speed as expected (e.g. ~260-900µs at 33.33MHz vs.
   ~700-1600µs at 800kHz). 66.66MHz and 100MHz both break protocol sync entirely (garbled
   headers, `type=-1`) — those speeds are past this device's signal-integrity margin and
   were not usable data points. See §17.3.
3. [ ] Check chip-select behavior specifically: does `amd_spi_exec_segment()` release CS
   between transactions in a way that differs from what the raw-MMIO tools do? A
   continuously-asserted or oddly-toggled CS line during idle could plausibly cause a
   device-side timeout distinct from anything content-related. Not yet tested empirically;
   a first code-reading pass didn't show anything obviously anomalous (`amd_spi_clear_chip()`
   is called once per `spi_message`, so CS toggles normally between each `spi_sync()` call
   the SEQ thread makes) — still open, see §17.4 for what a next pass should look at.
4. Only after 1-3: consider whether this needs a logic analyzer. Given how tight and
   reproducible the ~508ms figure is (sub-millisecond variance across 60+ cycles), it is
   very likely explainable from software once the right register/line is watched — this
   is a stronger position than the "needs physical instrumentation" conclusion §15.20
   reached, precisely because we now have a live, repeatable, fast (~1-2s per cycle)
   failure to instrument against, instead of an eternal unbroken RESET_RSP loop. With items
   1-2 now closed and pointing firmly at "device-internal, not bus-controllable," a logic
   analyzer is looking more necessary than it did this morning — software-visible knobs are
   running out.

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
- As of Breakthrough #2 (§18), `amd_spi_host_transfer()`'s combined-segment fast path in
  `spi-amd.c` now chunks large reads instead of silently truncating at 70 bytes — this is
  new, load-bearing code, not just diagnostics. The `DIFFCHECK:` log lines in
  `spi-hid-core.c` (dumping full DEVICE_DESC/RPT_DESC bodies) are debug-only and safe to
  remove once the chunked-read alignment is finalized, but keep them until then — they're
  how the ~78% match figure was measured and how any further alignment fix should be
  verified.
- Recovering from a "device went silent" incident: use `tools/diagnostics/ps3_ps0_cycle.c`
  (real ACPI `_PS3`→`_PS0`, non-destructive, proven in §15.20) — never raw M009/M010 pokes
  (§10, permanently breaks the device until a full reboot).
- `tools/parse_spi.py`'s `TXN#N` numbering interleaves Device A (touchscreen) and Device B
  (companion chip) by wall-clock time — the "+Xus" gap column is relative to whichever
  transaction (either device) immediately precedes it, **not** necessarily the same
  device's previous transaction. For real elapsed-time-on-one-device questions, diff the
  raw `clock_100ns` values directly (see §16.4's worked example) instead of trusting the
  printed gap column.
