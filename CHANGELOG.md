# Changelog

## Unreleased

### The read approval asked the wrong question

The descriptor never arrived because of the frame that asks for it, not because
of where the answer was looked for.

The traces are unambiguous about the shape of a read. Every read in
`traces/surface_boot_auto.csv` is a single message, two transfers, request
length equal to response length:

```
#0003 xfers=2 tx_len=9   tx=0B 00 00 00 FF 00 00 03 00
                         rx=FF FF FF FF FF 72 80 00 5A   v0 type=0x7 body=32B
#0004 xfers=2 tx_len=37  tx=0B 00 00 00 FF 00 00 03 00 00 …
                         rx=FF FF FF FF FF 1F 00 00 1C … (DEVICE_DESC body)
#0006 xfers=2 tx_len=9   tx=0B 00 00 00 FF 00 00 03 00
                         rx=FF FF FF FF FF 82 B0 0E 5A   v0 type=0x8 body=940B
#0007 xfers=2 tx_len=945 tx=0B 00 00 00 FF 00 00 03 00 00 …
                         rx=FF FF FF FF FF AB 03 00 75 …  (report descriptor)
```

The register is a **single byte at offset 7**, the address field (bytes 1..3) is
zero, and the request is clocked out **padded with zeros to the length of the
response** — nine bytes for a four-byte header, `5 + body` for a body.

`spi_hid_seq_read_reg()` built a five-byte frame with the register in the
address field instead. The device decodes the register from offset 7, so every
read it received was a request for register 0: it answered with its RESET_RSP,
which is exactly what the field bundles show (`reset_rsp == irq_count`, the
RESET_RSP loop in `WAIT_DESC`) and why the read of register 3 changed nothing.
The request buffers now come from `read_tx_buf` (zeroed once, first nine bytes
rewritten per read) and the frame is built like the reference.

Read requests are byte-compared against the traces for the first time in this
release; nothing in the host test suite can clock real SPI, so the pin in
`tests/driver_source_sanity_test.py` guards the offset and the zeroed address
field against a rewrite.

### The descriptor request was answered all along — on the other register

The field bundle showed the recovery chain working and the raw handshake still
failing, with `device_desc=0` and every IRQ carrying a `RESET_RSP`. The Windows
boot trace (`traces/surface_boot_auto.csv`, decoded with
`tools/parse_spb_csv.py`) has the answer, and it is four lines long:

```
#0002  write op=0x02 reg=0x000001  tx=02 00 00 01 42 00 00 03 00 00   ← the DESCREQ
#0003  read  op=0x0B reg=0x000003  rx=... 72 80 00 5A    v0 type=0x7 body=32B   ← DEVICE_DESC
#0004  read  op=0x0B reg=0x000003  rx=... 1F 00 00 1C ...                    ← its body
#0005  write op=0x02 reg=0x000002  tx=02 00 00 02 42 00 00 03 00 00   ← the report-descriptor request
#0006  read  op=0x0B reg=0x000003  rx=... 82 B0 0E 5A    v0 type=0x8 body=940B  ← RPT_DESC
#0007  read  op=0x0B reg=0x000003  rx=... AB 03 00 75 08 ...                  ← its body
```

Windows **writes the request and then reads the answer from register 3** — no
interrupt in between (the trace's GPIO events sit before and after, not inside
the exchange). The request frames this driver emits are already byte-identical
to those (`tx_len=10`, `02 00 00 01 42 00 00 03 00 00`); what differed is where
the answer was looked for. The driver read **everything** from
`desc.input_register` (register 0) and waited for the device to push responses
there. On the field unit register 0 only ever yields `RESET_RSP`, so the
request was answered into a register nobody read: discovery looped in
`WAIT_DESC` from boot until the retry budget ran out.

`spi_hid_seq_read_resp()` now reads a response from the output register (3 on
MSHW0231, the register Windows used) and falls back to the input register — so
a device, or a build older than this one, that answers on the input register
keeps working. The descriptor poller does the same at the header level: it
takes the first register whose read yields a frame header, and it now runs in
`WAIT_RPT` too, which makes the report descriptor findable with the same
mechanism. The register that produced each frame is logged at
`sl4a_debug_level=2`, so the next bundle says which side answered.

### The raw fallback could never have worked, and now it does

The bundle from the field test showed the recovery chain doing exactly what it
was built to do — the level-0 warnings, the three retry budgets, the two 5 s
restarts, and then `falling back to standard HID` — followed one line later by
`Unsupported device descriptor version 0` and an ACPI power cycle: the panel
ended up deader than before and nothing was armed anywhere.

`spi_hid_create_device_work()` refuses to publish a device whose
`desc.hid_version` is not `0x0100`, and the raw fallback runs precisely because
discovery never completed — so the descriptor was still zero, the version check
rejected it, and the error path powered the controller down. The two other
fallbacks in the file (the poll-work one and the vendor-init one) had always
carried the eleven hardcoded descriptor fields for exactly this reason; the raw
one did not.

Those fields now live in one helper, `spi_hid_use_hardcoded_desc()`, called by
all three. A failed raw handshake installs the Windows stack's own values
(vendor `045E`, product `0C19`, report-descriptor register `0x0002`, length 936
— the hardcoded report descriptor) before creating the device, so the fallback
now hands back a working single-touch HID touchscreen instead of a dead panel.

### The management tool: an upgrade that cannot take the driver away, and a bundle that answers the question

Ten findings from the same campaign about `tools/sl4a-touch.sh`.

**An upgrade used to remove the working driver before building the new one.**
`install` dropped every other DKMS registration of the package *first* — with
`dkms remove`, which also uninstalls the module objects the running system
boots from — and only then built. A build failure therefore left the machine
with no registered driver at all, while the message said the existing driver
state had been left unchanged. The removal now happens after the new version
has been built *and* installed (and the new version is installed once more
after it, because removing a version deletes the shared `/updates/dkms`
objects both versions record). The failure path only cleans up when this run
created the DKMS entry: with a version that was already registered — the usual
case, since VERSION does not move between commits — a failed attempt now leaves
the working module exactly where it was, and the message is true.

**`status` answered for the wrong kernel.** `dkms status` was consulted without
`-k`, so an entry installed for another kernel made the tool claim the checkout
matched. It is pinned to the running kernel now and says which one it means.

**The boot-activation promise was never checked.** The tool enabled the unit and
announced that every future boot would be covered, without asking whether the
unit can load at all — a checkout that has since moved or been deleted fails
that unit with `203/EXEC` at every boot. `install` now verifies it
(`systemd-analyze verify`) and fails loudly instead of promising; `status`
separates enabled from enabled-and-loadable.

**A completed activation could still be the previous build.** `modprobe` is a
no-op on a loaded module, so the freshly installed driver was not necessarily
the one answering. The tool compares the running module's `srcversion` with the
installed one and, when they differ, prints the way out (`modprobe -r … &&
activate`, or a reboot) instead of implying the new build is live.

**`logs -o` with a path starting with a dash** wrote the bundle and then failed
its own completion check, because the path reached `head`/`grep` as an option
and those read stdin. The path is normalised once, before anything uses it.

**The bundle now carries the identity of the module that is loaded**, not just
the one on disk: `/sys/module/*/srcversion` against `modinfo -F srcversion`, in
its own section labelled as the first thing to read — that comparison is the
stale-module answer, and `lsmod` alone never gave it. `build_info` is labelled
as the checkout/toolchain string it is rather than looking like a module
version.

**The bundle's git section** no longer swallows stderr, so "git refused" (the
usual case when run as root against a user-owned checkout) is distinguishable
from a clean tree and from local modifications.

**`VERSION` is validated before anything interpolates it**: `1.0.&` used to
stage `PACKAGE_VERSION="1.0.#VERSION#"` (sed expands `&`) and a space split the
DKMS build line.

### Recovery coverage, finished for raw mode, mapped for everything else

A recovery-matrix review (every timer × state × profile × knob, read back out of
the code) closed the last raw-mode gap and documented the rest:

- the raw watchdog now also covers `WAIT_RESET` at cold probe, so a controller
  that never sends a `RESET_RSP` at all is no longer the one silent case in raw
  mode (the resume path already armed it);
- the intended extra settling delay in the watchdog's own recovery path was
  dead code: `schedule_delayed_work()` on an already-pending item is a no-op, so
  the `getfeat_delay_ms + 1000 ms` re-arm silently became nothing. It uses
  `mod_delayed_work()` now, which is what the comment always said;
- `docs/RECOVERY-MATRIX.md` is the map: which timer covers which state, in which
  profile, what it logs at level 0 — and the eight (state × profile × knob)
  combinations that end in a stall with nothing watching, five of them in
  standard mode, where the shipped defaults (`wait_reset_kick_ms=0`,
  `std_liveness_ms=0`) leave every silent state uncovered. It also lists the
  exact lines a raw-mode bundle should show when the device stalls, so the next
  report can be checked against it without raising the debug level.

Standard-mode recovery is deliberately *not* changed here: that profile is the
one that works on the field machine, and widening its timers is a behaviour
change to make against hardware, not blind.

### Resume, counters and a diagnostic that no longer lies about the frame

The same review campaign's second batch, on the same rule (every finding
reproduced against the source, and against the kernel's own sources when the
finding was about the kernel).

**Raw mode had no timer after resume.** `spi_hid_resume()` assigns
`WAIT_RESET` directly rather than through `spi_hid_seq_set_state()`, and the
watchdog it arms there returns immediately in raw mode — so a controller that
came back from resume without a `RESET_RSP` had nothing watching it and the
panel stayed dead until the next suspend/resume cycle. It now arms the raw
handshake watchdog too.

**Two sysfs attributes that could only ever report zero.** `bus_error_count`
had no increment anywhere, and `device_initiated_reset_count` printed
`dir_count` — descriptor reads, a different thing, whose increment was removed
long ago. The first now counts failed protocol transfers (and keeps the last
error code), the second reports what its name says: the device-initiated reset
count the sequencer already tracks. The diagnostic bundle reads both, so a
report that showed a healthy zero was showing nothing at all.

**`ready` went down without a notification on suspend and resume** while every
other flip of that bit notifies, leaving a client blocked on the attribute
asleep until its timeout.

**`perf_mode` was writable and unread.** Nothing in the driver consumes
`shid->perf_mode`, so writing it changed nothing; the attribute is read-only
until something does.

**`heatmap_debug` printed `cells = len - 2`.** `heatmap_len` *is* the cell
count (both callers pass the raster length), so a 3456-cell frame advertised
3454 cells in the very line one reads when a frame looks wrong.

**A `_DSM` register mismatch now speaks up.** The `DESCREQ` that starts
discovery is built from a compile-time constant while the descriptor register
the firmware reports was only ever logged; if a machine reports a different
value, discovery would fail silently. It now warns with both values.

### Raw discovery can no longer stall in silence, and two field traps

Three fixes from a review campaign over the whole driver (several independent
reviewers, one copy of the tree each, findings verified one by one against the
source before being acted on).

**A device that stalls before `DONE` had no timer and said nothing.** In raw
mode the handshake watchdog was armed only at `DONE` and in the post-report
paths, so a device that answers the `DESCREQ` with nothing — or, as the field
unit does, with a `RESET_RSP` on every IRQ — left the sequencer parked in
`WAIT_DESC` forever: `descreq_work` only re-reads the input register (the IRQ
thread drains each frame first, so it always reads an empty one), the
unchanged-state early return in `spi_hid_seq_set_state()` re-armed nothing, and
the only trace of the loop sat behind `sl4a_debug_level`, which is 0 by default.
The symptom was a touchscreen that never produced a frame, counters frozen at
`device_desc=0 … reset_rsp == irq_count`, and a dmesg with nothing in it. The
watchdog is now armed on entering `WAIT_DESC` and `WAIT_RPT` in raw mode, before
the unchanged-state return, so the stall ends the way every other raw stall
does: a level-0 warning, the Windows-style `SET_POWER(D2→D0)` re-discovery, and
— when the retries are exhausted — the automatic fall back to standard HID,
which is what gives the panel its touch back instead of leaving it dead.

**`spi_hid_ll_parse()` returned with its mutex held.** A dangling `else` made the
`mutex_unlock()` conditional on the hardcoded report descriptor parsing
successfully; on the failure path the function returned with `shid->lock` held,
so the next lock taker (IRQ thread, any sysfs reader, removal) waited forever.
Reachable whenever the device-read descriptor fails to parse *and* the
hardcoded one does not parse either.

**`install` skipped the rebuild it was supposed to do.** After a pull, with the
DKMS version already registered, `install` took a branch that rewrote the
profile and skipped the build entirely — the `--force` added earlier to defeat
DKMS's "already built" cache sat on the branch that path bypasses. VERSION does
not change between commits, so "already installed" said nothing about whether
the module on the machine matched the checkout, which is exactly how a stale
module survived an install. The staging step now always re-copies the sources
and rebuilds; only `dkms add` is skipped when the entry already exists.

### Diagnostic bundle carries the last captured frame

`sudo ./tools/sl4a-touch.sh logs` now writes the last captured frame into the
bundle, so a problem report can be analysed from the bundle alone. What the
driver keeps is the frame's **cell field** — one byte per cell, 3456 bytes on
MSHW0231, 4056 on the 78x52 MSHW0162 panel — not the 4304-byte wire body, and
the bundle carries it as hex.

The frame could not come from `heatmap_debug`: a sysfs `show()` attribute is
limited to one page and the cell field of the 78x52 panel does not fit, so the
frame's tail — where the per-frame lists sit — was cut. The driver now exposes
the same buffer as the `heatmap_raw` binary attribute, which streams it whole,
in as many reads as the reader asks for, holding the same `seq_lock` as
`heatmap_debug` and advertising no size (the length is per-device). The
callback is version-guarded for kernels ≥ 6.16, where the bin_attribute
callback lost its non-const argument: the unguarded signature would turn into a
hard `-Werror=incompatible-pointer-types` build failure on current
Arch/CachyOS/Fedora kernels. CI builds against those headers too (the
`kernel-build-current` job), so the guard is pinned from both sides: 6.8 headers
on Ubuntu and whatever the Arch container ships today.

The bundle reads the attribute once and labels it with the bytes it just read,
so the count always describes the hex below it; an empty attribute says there is
no frame data instead of claiming "0 bytes, complete"; and on modules that
predate the attribute it falls back to the truncated `heatmap_debug` view,
labelled as such. It also picks up `build_info`, `ready` and the two error
counters. `sl4a_debug_level=2` continues to add the per-blob lines through
dmesg.

Three host tests pin it: `diagnostic_bundle_contract_test.py` (structure),
`diagnostic_bundle_frame_test.py` (the section run for real against a fake sysfs
tree, hex round-tripped) and `binattr_signature_test.py` (the guard compiles on
6.8/6.13/6.16, and the revert fails on 6.16).

### Windows alignment analysis, replay harness, and a refuted transplant

`docs/WINDOWS-ALIGNMENT.md` records, value by value, what the raw pipeline already
shares with the Windows detector of device 0x0C19 and what does not. Most of it is
already aligned: the c590 lookup table (zeroed at the resting byte 180), the 0.04
noise floor, the 0.02 low threshold, the candidate-count gate 2, the frame-age gate 3
(`blob_debounce`), extended search off, the 72x48 grid, a 30-frame baseline, the
6-cell ghost radius, the 23%/97% edge penalties and the 5x5 centroid.

`tests/real_frame_replay_test.c` replays a real Windows touch session (537 frames,
44 with three or more fingers) through the driver's own decoder and pipeline; it
takes the corpus path as an argument and skips when it is absent, so no Microsoft
data enters the repository.

Applying the remaining gap - the per-cell 0.1 gate (record +0x1fe = 135, = 1000 in
our units) - was implemented and then **reverted, because the corpus measured it as a
regression**: frames with a deficit 45 -> 63, contacts lost in frames with three or
more fingers 24 -> 42, cases fixed 0, and `raw_pipeline_replay_test` 58 passed/0
failed -> 43/26 because the synthetic fixtures do not clear 0.1 with a multi-cell
footprint. At 0.1 a real fingertip footprint collapses to one or two cells, which the
candidate gate (`+0x1fa` = 2, unchanged and not ours to change) then rejects.

Conclusion: the 135/1000 value is real, but transplanting it into a
mask -> connected-components -> pixel-count pipeline is the wrong mapping. In the DLL
it validates a candidate against its five-cell neighbourhood. The next attempt
belongs there, not in the mask.

### Not verified

No hardware run: every number above is a host replay of a recovered capture. The
second frame-age gate (record +0x1fc = 5) has no established semantics and stays an
open item in `docs/PARAMETERS.md`. The association radii, ghost radii, Hungarian
costs, split constants and EMA alphas still have no source in the binary and are
documented as invented rather than derived.

## 1.7.0 — host→device frames match the Windows stack; Report ID 6 read

Every command frame the sequencer puts on the bus now reproduces, byte for byte,
what the Windows stack sends. The reference is the SPB trace
(`captures/wintrace/surface_init.csv`, TXN 634377432 onwards), whose write frames
are SET_POWER D0, SET_FEATURE Report ID 0x56, DESCREQ register 1, DESCREQ
register 2, SET_FEATURE Report ID 0x56 again, GET_FEATURE Report ID 6 and
SET_FEATURE Report ID 5. The driver carried two differences from all of them:

- a doubled leading opcode (`02 02 ..` instead of `02 ..`) on SET_POWER,
  DESCREQ, SET_FEATURE and GET_FEATURE;
- zero padding where Windows sends the constant `0C EE 5B` trailer on the short
  command bodies (SET_POWER and SET_FEATURE Report ID 5).

Both are gone from the default path. The frames now live in
`driver/spi-hid-wire-frames.h`, the single source of truth for their bytes; the
legacy doubled form is still reachable through the new `wire_double_opcode`
parameter (default `0`), and `setfeat_no_double` keeps working as a deprecated
alias that opts the SET_FEATURE frame out of doubling, so existing `modprobe.d`
drop-ins still load. `tests/wire_frames_test.c` compiles that header and compares
every frame against the reference bytes; it runs in `make -C tests test` without
`|| true`. It also fails if a local frame table reappears in `spi-hid-core.c`.

The report-descriptor DESCREQ (`seq_handle_desc()`) is fixed the same way: the
trace shows `02 00 00 02 42 00 00 03 00 00`, and the register keeps coming from
the device descriptor.

The probe also reads GET_FEATURE Report ID 6 in the Windows order — after the
report descriptor, before the SET_FEATURE that enables the heatmap. It belongs to
the raw-mode init sequence, so it runs with `skip_getfeat=Y` as well (the
installer's raw profile sets it); `skip_getfeat` continues to mean "the
standard-mode handshake does not wait for a feature reply". The read is diagnostic
only: the reply is kept in `struct spi_hid_getfeat6` and logged as hex plus
IEEE-754 values at debug level 2, nothing acts on it, and a failed read never
delays the handshake. Evidence recorded for the mapping step: in the captured
reply the first 55 payload bytes are not binary32 values; the aligned array that
follows holds 16 of them (178.0, 182.0, 180.0, 1.0 twice, 90.0, 171.0, 100.0,
20.0, 172.0, 177.0, 175.0, 2.0).

### second dead-code pass (occurrence audit over the whole repo)

The first cleanup removed what round 4's inventory named; this one is a full
audit, so every item below is verified twice: word-boundary occurrence counts in
every file, the git history for readers that used to exist, and the live SPI write
path confirmed.

- `spi_hid_output`, `spi_hid_output_header` and `spi_hid_report_descriptor_request`
  have no call site anywhere. They were already unreachable before 1.6.3 (their
  only caller was the reset/refresh handler that release deleted); the live write
  path is `spi_hid_send_output_report` via `spi_hid_ll_output_report`, plus
  `spi_sync()` in five places.
- `spi_hid_protocol_validate_raw_capture`: unused `static inline` in the protocol
  header.
- Trace events with no producer: `spi_hid_dev_irq`, `spi_hid_reset_work`,
  `spi_hid_refresh_device_work`, `spi_hid_output_begin`, `spi_hid_output_end`.
- Fields written and never read: `report_descriptor_crc32` (its only reader lived
  in the refresh worker), `hid_desc_addr`, `device_power_support`,
  `power_response_delay`.
- `poll_missed` stays and is now reported by `protocol_stats`: the poller counts
  consecutive rejected reads and nothing ever showed the number, which is exactly
  the diagnostic the cold-boot investigation needs.

### installer: quote the `-o` path at the elevation call site

`logs -o` was passed to the elevated child as `${OUT:+-o "$OUT"}`, which bash
word-splits before the child sees it: a bundle path containing a space arrived as
three arguments and `cmd_logs` rejected the second as an unknown option. The
`logs` command is the one reporters are pointed at, so it is worth being exact.
The contract test now pins the quoted call and the absence of the expansion
(reverting it fails the suite).

### Not verified

The module cannot be compiled on the development host (no kernel headers): the
`kernel-build` CI job is the compile gate. No hardware run yet -- the wire
behaviour is asserted against the trace bytes only.

## 1.6.3 — housekeeping: dead code, build string, FSM model (2026-09-15)

Round 4's inventory listed the parts of the driver no code path can reach. They
are gone rather than annotated, so the next reader does not have to prove their
innocence again:

- `reset_work` / `spi_hid_reset_work()` and `reset_pending`: nothing ever
  scheduled the work, so the flag it set was never observed and its two readers
  were dead branches — the live device reset is the sequencer's own recovery.
- `refresh_device_work` / `spi_hid_refresh_device_work()` and
  `refresh_in_progress`: the report-descriptor refresh and HID re-creation worker
  was never queued.
- `keep_powered`: written in six places, read nowhere. Suspend-time power gating
  needs an implementation, not a field.
- The unreachable `VENDOR_INIT` handler stays, with its raw-mode gates: it
  documents a state of the decompiled protocol map, and those gates are correct
  if the state is ever wired up.

Also fixes the build string shipped in 1.6.2: `VERSION` moved to 1.6.2 while
`SL4A_DRIVER_VERSION` stayed at 1.6.1. The host-test version check caught it on
`main` (the release commit failed CI for exactly this), and bumping the define is
part of the release steps now.

Also aligns the per-slot state machine model in `raw_pipeline_math_test.c` with
the driver, which it had drifted from in four places: hold entry consulted the
blob weight (the driver holds the slot on the first missed frame whatever the
weight was), hold expiry fired one frame late, and both re-claim branches
restarted `duration` at 0 instead of 1. Reintroducing either mechanism now fails
the suite, so the expectations bite; the file also says out loud that it is
documentation, because the behavioural checks live in `raw_pipeline_replay_test.c`
where the real pipeline runs.

## 1.6.2 — Follow-up review of the 1.6.1 fixes (2026-09-15)

A fourth round of the same campaign reviewed the *corrections* made in 1.6.1,
which no round had looked at yet: four independent legs (raw pipeline, handshake
and backstop, installer and docs, whole-release coherence). Every finding below
was verified against the source first; two are behaviour bugs the 1.6.1
corrections introduced, and one is a much older bug found while trying to make a
check for one of those corrections actually fail.

### Raw pipeline

- The blob bounding-box **maximum was never tracked**: the bounds are `s32`
  starting at -1 while the cell indices are `u32`, so `r > max_r` was evaluated
  unsigned and stayed false forever. Two features were silently dead: the
  bottom/right **edge penalty** (the 23% bezel suppression inherited from the
  Windows DLL config) and the **second-moment ellipse**, so MAJOR/MINOR/
  ORIENTATION were always 0 for unsplit blobs.
- The `dfa_data_offset` geometry drop is latched per mismatch episode and the
  parameter override refuses a geometry that cannot fit the frame: with
  `grid_cols`/`grid_rows` set next to an impossible offset the override re-armed
  the cache every frame, so the pipeline was reset once per frame and every held
  contact was released at 100 Hz — the storm the 1.6.1 fix believed it removed.
- Split sub-blobs are penalised by their own extent, not by the peak's ±2
  sampling window (a superset): a peak two rows from the edge that touches nothing
  was penalised, and a bottom-touching component whose humps sit higher still
  escaped the penalty.
- The tracker's blob filter applies `blob_min_weight` to the pre-penalty weight
  (the same value the creation gate uses) instead of the penalised one: with the
  bottom-edge penalty now live, the second gate would otherwise drop a real
  bottom contact below the threshold before the tracker could ever see it — the
  penalty shapes the emitted weight, it does not delete contacts (review R18).
- Known residual, not changed: an explicit `grid_rows`/`grid_cols` that cannot fit
  a frame is trusted over the per-device config, so the parameters themselves can
  leave the raw pipeline idle until they are corrected — that is the documented
  meaning of "the parameters are an explicit override", and it is preferable to
  the per-frame reset storm this release removed.
- The failed re-derive path logs rate-limited, like its sibling message, instead
  of once per frame.

### Standard-mode recovery (issue #4)

- The WAIT_RESET backstop arms only once the IRQ is requested, and probe arms it
  after `request_threaded_irq()`: arming during probe's 300 ms settle let an
  opt-in interval shorter than that window kick a device that had no chance to
  answer — a spurious DESCREQ, and the hardcoded-descriptor fallback behind it.
- The arm snapshots the IRQ-edge counter with `READ_ONCE`, like the watchdog's own
  read.
- Every remaining `ready = true` site notifies pollers (the poll-work type-3
  fallback, both VENDOR_INIT paths and the normal report-descriptor path were the
  exceptions the 1.6.1 commit message claimed did not exist).

### Installer and docs

- The diagnostic bundle is verified by the redirect's own exit status and by its
  last section: a `> "$OUT"` that cannot be opened keeps the previous (non-empty)
  file, so a size check alone could announce a bundle that was never written.
- `-o` refuses an empty path, and "is this one of ours" looks at the first line
  only instead of grepping the whole file.
- `dkms status`'s `sl4a-touch/<version>: added` shape (no comma) parses as a
  version; without it `dkms remove -v` failed on the stray suffix and the stale
  registration survived the upgrade meant to clean it up. Only a plausible
  version reaches `dkms remove`/`rm -rf`, and a removal that fails is a `warn`
  naming the consequence instead of an `info` line.
- `cleanup_staged_install()` applies the same ownership marker as the other two
  removal paths, and the `PACKAGE_NAME` markers are anchored.
- The interactive menu survives a terminal without `cuu`/`ed`/`cnorm`, the
  `set -e` abort the earlier fix missed.
- "Install complete" prints after Step 7 rather than before it, and the two paths
  that skip activation (MOK key not enrolled, profile change) are described in
  the README and ROLLBACK instead of being contradicted by them.

### Checks

- `raw_pipeline_replay_test` gained a driver-level recovery-guard check (a
  bottom-edge blob, two dropped frames, the slot must come back) and pins the edge
  penalty and the ellipse; the constant-mirror assertion in
  `raw_pipeline_math_test` that could not fail is gone.
- New `installer_recovery_contract_test.py` pins the installer's failure-path
  strings and the DKMS version parse. Every check added here was verified to fail
  with its fix reverted.

## 1.6.1 — Review-campaign fixes, raw pipeline and installer (2026-09-15)

Fixes from a double-blind multi-agent review campaign over the 1.6.0 revision.
Every reported finding was verified against the source before being acted on;
three reports presented with high confidence turned out to be false positives and
are deliberately **not** "fixed" here: the installer module-name handling, the
synchronous-request lock ordering, and the diagnostic log-option quoting.

### Raw handshake paths, re-discovery and HID gating

A second review round covered what the first one had barely looked at. All of the
following were verified against the source before being changed:

- The delayed `raw_mode` handshake no longer drops the vendor init: with
  `skip_getfeat=0 getfeat_delay_ms>0` it sends the same
  `vendor init -> GET_FEATURE` sequence as the immediate path, instead of a bare
  `GET_FEATURE` that made the experiment disagree with itself.
- `setfeat_speed_hz` and `setfeat_no_double` are honoured on every SET_FEATURE
  path, not only on the `GET_FEAT_RESP` one: under the default `skip_getfeat=1`
  both switches used to be silently dead for the experiments they exist for.
- Stream-watchdog recovery goes through the same restart as the handshake
  watchdog (D2/D0 vendor init, fresh DESCREQ, timer re-armed) instead of a
  DESCREQ-only sequence with no timer and no counter reset.
- Every re-discovery clears `ready` (and notifies sysfs), so HID clients cannot
  interleave sync requests with the sequencer and steal its responses while the
  descriptor is being fetched again.
- The hardcoded-descriptor fallback no longer creates a standard HID device in
  raw mode: it follows the same `raw_mode_active` gate as the normal descriptor
  path, instead of giving userspace two publishers for one panel.
- `SET_REPORT` and `output_report` re-check `suspended`/`removing` on every call;
  those flags are published under `seq_lock` and read here with `READ_ONCE`, which
  narrows the window in which a client can block in `spi_sync` against a
  controller that has just been quiesced (it does not close it: a suspend landing
  after the check is bounded by the SPI core's own failure path); `ll_power` no
  longer pretends `shid->lock` protects the `hid` pointer;
  a negative `getfeat_delay_ms` is clamped at probe instead of wrapping the
  handshake watchdog into the far future.
- `SPI_HID_SEQ_VENDOR_INIT` is documented as unreachable: no path sets it, so the
  vendor handler belongs to the decompiled state map, not to live code.

### Raw pipeline (heatmap baseline)

- The heatmap baseline gains the slow downward decay its own comment promised:
  the resting value was only ever raised (acquisition kept the per-cell maximum,
  the per-frame update only recovered upward), so once the resting level drifted
  down the computed signal stayed positive across whole areas of the panel and
  the pipeline published phantom contacts until a module reload or a resume. The
  decay is one raw count per `HEATMAP_DRIFT_DIV` frames (about 2.5 s at 100 Hz),
  slow enough not to fade a held finger, and a cell resting on its own value
  stops decaying. Covered by a host-model assertion in `raw_pipeline_math_test` and by a
  real-pipeline check in `raw_pipeline_replay_test` (falsified: neutralising the
  branch makes that check fail).

### Raw pipeline (tracker and diagnostics)

- A one-frame gap no longer becomes a real release: re-acquisition while the lift
  is pending used to restart the debounce unconditionally (`case 3` → state 1),
  where the hold path requires a substantial blob before recovering the contact.
  Since a single peak-free frame puts *every* finger into lift, one dropped frame
  aborted pinches, scrolls and drags by releasing and re-pressing all contacts.
- The lift-lookback history is cleared whenever a slot starts or frees a contact.
  It was only ever appended to, so after a slot had been reused a fast tap could
  report the *previous* contact's position as its lift point, which libinput then
  classifies as a swipe instead of a tap.
- `grid_cols`/`grid_rows` are reachable again: the geometry cache was filled from
  the per-device config at probe (never NULL) and the parameter path only ran when
  the cache was empty, so both knobs were dead on every boot. The parameters are
  now an explicit override of the cached geometry.
- A `dfa_data_offset` that cannot fit the cached grid: the bullet below in this
  section (the pipeline used to reset ~100 times a second).
- Split sub-blobs are edge-penalised by their own window instead of skipping the
  penalty: the split path `continue`d over it, so a bezel artifact wide enough for
  two peaks was published at full weight. The tracker's recovery guard, on the
  other hand, now uses the *pre-penalty* weight: a real bottom-row finger keeps
  only 23% of its weight there, below the recovery threshold, so guarding on the
  penalised value made every bottom-edge contact unrecoverable after one dropped
  frame (found by the adversarial pass on this very change).
- A `dfa_data_offset` that cannot fit the cached grid no longer resets the whole
  pipeline 100 times a second: the mismatch is reported at a sane rate, the cached
  geometry is dropped so the auto-detect can re-derive it from the frames actually
  arriving, and any slot still held from a previous valid frame is released once on
  the way out.
- The ghost merge is strict (`<`, not `<=`), as in Windows and in the in-tree
  oracle test, so two blobs exactly at the coalescing radius no longer silently
  drop one.
- `mshw0231_raw_input_register()` returns `-ENOMEM` when the input device cannot
  be allocated. It reported success, which left the driver in "raw mode" with no
  input device: every frame discarded, nothing said why.
- The `CALIB:` trace printed coordinates 100× the ones the pipeline computes (it
  divided by 1000 where emission divides by 100000); it now shares the emission
  expression and says in the line that it prints the pre-offset value, because it
  logs pre-tracker blobs while emission logs post-tracker slots — the two are not
  the same quantity and reading it as if they were was the second half of the
  problem. The `CALIB_REF:` trace built `hx` and `hy` from the same byte; both now
  come from the consecutive little-endian pairs the report descriptor defines.
- Comment drift corrected in the tracker (the EMA/deadband/stationary numbers did
  not match the constants the code uses).

### Installer

- `logs` no longer truncates its own diagnostic bundle: `git` refuses to read a
  user-owned checkout as root, `systemctl status` exits non-zero for an inactive
  unit, and an empty `dmesg | grep` is exit 1 — under `set -e -o pipefail` each
  of those aborted collection, so the one command a reporter is asked to run
  failed in exactly the states worth diagnosing.
- Upgrades remove the previous DKMS registration instead of leaving both: two
  registered versions built the same module names, and `dkms autoinstall`
  installed whichever ran last on the next kernel update, so an older revision
  could silently become the one that loads. `uninstall` removes leftover versions
  too, instead of printing "Uninstall complete" while an old registration (and
  its `/usr/src` tree) survives and keeps being rebuilt.
- An interrupted staging run is recoverable: the next `install` cleans up the
  half-staged version and restages, rather than refusing to continue until the
  user deletes `/usr/src/sl4a-touch-<version>` by hand.
- `logs -o FILE` is validated before root puts it in a redirect: no symlinks, no
  non-regular files (`-o /dev/null` used to chmod the device node) and a missing
  argument is now an error instead of a silent `shift`.
- The boot-activation unit quotes its `ExecStart` path, so a checkout whose path
  contains a space no longer produces a unit that fails after every boot.
- `activate` honours `SL4A_SYSFS_ROOT` like the rest of the script, so preflight
  and activation agree on which tree they are inspecting.
- `install` refuses to replace a `sl4a-touch-activate.service` it does not own,
  the guard it already applied to the modprobe configuration.
- README/TESTING corrected: the install step does bind the experimental modules
  (Step 7) and enables the boot unit. The docs claimed it did not, which is
  exactly the sentence a careful user reads to decide when to arrange recovery
  access.

### Transport, power management and recovery

- The input IRQ-storm breaker no longer parks the sequencer silently: it logs,
  clears `ready` and schedules the recovery handler, and that handler
  re-enables the sequencer it needs, because the ACPI reset path bails out on a
  parked sequencer and the obvious fix alone would have been a no-op.
- Suspend clears `ready` and aborts an in-flight synchronous transaction, and a
  request that races the transition fails with `-ENODEV` instead of waiting out
  the whole `sync_timeout_ms` and, for descriptor reads, tearing the transport
  down and scheduling a recovery during the PM transition. The same abort and
  `ready` clear now cover removal, which used to leave a HID client waiting
  through the teardown.
- Resume runs the raw vendor init before re-enabling the IRQ, and reports a
  failed init instead of discarding its result.
- The IRQ enable/disable decision is a real test-and-set, so a suspend racing the
  terminal error path cannot disable the line twice and leave it masked.
- `input_unregister_device()` during the raw to standard fallback runs outside
  `seq_lock`, so the IRQ thread and poller are not serialized behind it.
- `std_liveness_recover` runs once per silent episode instead of once per
  discovery cycle — observed activity and resume restore the allowance, so a
  healthy but idle device cannot be power-cycled into
  the terminal-failure path.
- The descriptor poller handles the DEVICE_DESC it recovers instead of counting
  it and dropping the frame.
- Opt-in standard-mode backstop for a device that answers power-up or resume with
  nothing at all: `wait_reset_kick_ms` (0 = off, the default) sends one `DESCREQ`
  after that many milliseconds without a single IRQ edge — one kick per entry into
  `WAIT_RESET`, with a failed write retried up to three times — and the existing
  descriptor poller then keeps reading until the device answers or resets. Without
  the backstop the sequencer sat in `WAIT_RESET` with no timer armed, `ready` false
  and no touchscreen until a reload or a suspend/resume, which is the cold-boot
  shape of issue #4. It never reads the input buffer (so a frame the IRQ thread is
  about to handle cannot be stolen) and performs no power sequencing. The default
  is off because the safe interval is measured, not known: enabling it means a
  device that never answers is polled for its descriptor afterwards, and that a
  late `RESET_RSP` draws a second `DESCREQ` from the `WAIT_DESC` path (unmeasured,
  on the field-test list). Raw mode keeps its own cold-boot retries.

### Raw handshake and stream monitoring

- Only a raw stream frame (content id `0x0C`, length `>= 3`) confirms the raw
  handshake: confirming on any frame retired the watchdog and the poller while no
  heatmap data was flowing, which left raw mode parked with no input and no way
  back. The policy now lives in one inline predicate with host-test coverage.
- The stream watchdog keeps ticking through a recovery re-init instead of dying
  at the first tick that finds the sequencer outside `DONE`.
- The poller respects `raw_input_beta` exactly like the IRQ path, and the retry
  budgets are reset when a handshake is confirmed.
- A late synchronous response can no longer overwrite the response buffer of the
  transaction that superseded it.

### Diagnostics and installer

- The standard-mode liveness check counts controller activity (IRQs) in the
  window instead of parsed frames, so frames that arrived while the HID device
  node was still being created no longer look like silence.
- The installer fails with an explicit message when `openssl` is missing while
  re-encoding a legacy PEM signing key as DER, instead of a vague or absent
  error.

### Noted, deliberately not changed

- `spi_hid_reset_work` is never scheduled anywhere in the tree (only `INIT_WORK`
  and `cancel_work_sync` reference it), so `reset_pending` is never set, the
  clearing sites around it are unreachable, and the entry that used to describe
  them here was removed by the follow-up review for describing behaviour that
  cannot happen. The same review also lists the never-scheduled
  `refresh_device_work` (whose `ready = true` is dead) and the write-only
  `keep_powered` flag. Removing the machinery is left to a separate cleanup rather
  than folded into these fixes.

## 1.6.0 — Surface Laptop 3 (AMD) support, raw-mode streaming backstop, issue #4 diagnostics (2026-09-15)

### Standard-mode startup liveness, detection only (issue #4)

- Standard HID mode could reach `DONE`, report `ready` and create the HID device
  while the controller had never started streaming, and nothing in the driver
  would notice: the raw-mode watchdog never runs in standard mode, the poller is
  armed for raw mode only, and the input stream is event driven, so silence is
  also the normal idle state. A cold boot that ends this way is silent.
- New `std_liveness_ms` parameter (diagnostic class, default 0 = off). When set,
  the driver checks once, that many milliseconds after `DONE`, whether any input
  frame arrived, and logs the outcome. Detection only on purpose: an idle device
  is legitimately silent, so recovering on silence alone could power-cycle a
  healthy touchscreen. Field data before action, see issue #4.
- A failed feature `GET_REPORT` now names the report id, the report type and the
  calling process, so the cold-boot feature query in issue #4 can finally be
  attributed to a real client instead of "a HID client".
- Two opt-in switches for the issue #4 experiments, both off by default and never
  written by the installer: `std_liveness_recover` turns the liveness check into
  the existing ACPI recovery when no data arrives, and `skip_std_getfeat` answers
  feature reads with `-EOPNOTSUPP` in standard mode so nothing is written to SPI
  for a feature query (the A/B test requested in issue #4).
- No behavior change with default parameters: the check is off unless enabled.

### Raw-mode streaming backstop and installer fixes (contributed by cristinagp, PR #7)

- Raw mode: reaching `DONE` now arms the periodic poller and the handshake
  watchdog, so a lost edge-triggered IRQ (the data-ready line can fire while
  the driver is still inside the SET_FEATURE write path) no longer leaves the
  activation polling forever with zero data. The poller can now confirm the
  handshake itself instead of waiting for the IRQ path to set the flag.
- Raw mode: handshake confirmation accepts any data frame, not only heatmap
  frames with content id `0x0C`; `stream_watchdog_ms` default is now 2000
  (was 0, disabled) as the Windows-matching interval.
- Installer: the DKMS MOK certificate is now written DER-encoded (`openssl
  -outform DER`), which is what `mokutil --import` requires. An existing
  PEM-encoded certificate is re-encoded in place, at install and at activate,
  instead of failing the Secure Boot path.
- Installer: when the selected profile differs from the loaded `raw_mode`
  value (a load-time-only parameter), `install` now says a reboot is required
  and skips activation instead of activating with the wrong mode.
- Installer: `activate` refuses to displace a device only when it is bound to
  a different driver, so re-activating an already-correctly-bound device is
  idempotent again.
- No behavior change for the standard HID profile: every new driver path is
  gated on `raw_mode_active`, and the installer changes only affect Secure
  Boot key encoding, profile-change detection and the already-bound check.

### Support for Surface Laptop 3 (AMD) — MSHW0162

- The HID transport now probes the SL3 AMD touch controller (`MSHW0162`)
  in addition to the SL4 (`MSHW0231`). Device-specific tuning is selected
  by ACPI ID at probe time and logged in dmesg ("device config: ..."):
  - SL4 (MSHW0231): 72×48 grid, 3456 CapImg raster samples, 30-frame
    resting baseline — all unchanged. The baseline recovery EMA alpha is
    now 7 on SL4 as well (was 2): alpha 2 made the baseline converge to
    raw/6 instead of resting raw, while 7 gives the Windows-documented
    12.5% recovery rate (fix contributed by guskog).
  - SL3 (MSHW0162): native 78×52 grid (4056 samples), 33-frame baseline,
    EMA alpha 7 — values contributed and tested on real hardware by
    guskog (issue #6).
- `spi_hid_capimg_decode_v0()` now takes the expected raster sample count
  and rejects frames carrying a different count (per-device validation).
- Installer: hardware/DMI checks, activation, status, diagnostics and
  logs all accept `MSHW0162` / Surface Laptop 3; exactly one of
  `MSHW0231`/`MSHW0162` is required (plus `AMDI0060`).
- Regression tests: capimg decoder covers the 4056-cell SL3 frame
  (accepted with the SL3 count, rejected with the SL4 count).

## 1.5.0 — GET_REPORT Timeout Fix, Recovery Power-Cycle (2026-08-24)

### Fix: touchscreen dies after connect-time GET_REPORT timeout (issue #4)

- A HID client (hidraw `HIDIOCGFEATURE`) issues a feature GET_REPORT ~1 s
  after connect. The MSHW0231 does not answer feature queries until it has
  settled for ~3.6 s (measured from Windows traces; the original protocol
  doc cited ~5.9 s), so the hardcoded 1000 ms sync timeout fired and the
  driver treated a benign feature-query timeout as fatal: `ready=false` →
  ACPI power-cycle recovery → dead touchscreen (Zorin/Fedora).
- `spi_hid_sync_request()` now classifies requests (`SPI_HID_SYNC_FEATURE`
  vs `SPI_HID_SYNC_DESCRIPTOR`): a feature-query timeout returns
  `-ETIMEDOUT`/`-EPROTO` to the caller without tearing the transport down;
  descriptor-request timeouts keep the fatal recovery path.
- New module parameter `sync_timeout_ms` (default 6000, int, 0444) bounds
  every synchronous request, replacing the hardcoded 1000 ms; 6000 covers
  the measured 3.6 s settle plus the ~5.9 s documented worst case.
- Recovery is now a real ACPI `_PS3`→`_PS0` power cycle (never `_RST`,
  which calls M010 and destroys the device) combined with a deterministic
  re-arm of the sequencer to `WAIT_RESET` (performed before the cycle), so
  the device's power-on `RESET_RSP` restarts descriptor discovery. If the
  ACPI evaluation fails, the driver warns and re-arms discovery anyway
  instead of leaving `ready=false` forever.
- Docs corrected: the ~5900 ms device-settle figure is not reproducible
  from the trace rows (measured ~3.6 s; 3.623 s gap in `surface_init.csv`).
  `docs/PARAMETERS.md`, `docs/HIDSPI_PROTOCOL.md`, `docs/ACTIVATION.md`,
  and `docs/ETW_CSV_FORMAT.md` now state the measured value.
- Regression test: `tests/protocol_test.c` checks the sync timeout policy
  (feature-query timeout non-fatal, descriptor timeout fatal, default
  6000 ms).

## 1.4.0 — Raw Multitouch Fixes, Unified Installer (2026-07-24)

### Raw multitouch pipeline

- Replaced the greedy zero-assignment blob-to-slot matcher with a correct
  Kuhn-Munkres augmenting-path Hungarian solver; added a track-continuity
  cost bias so two actively-tracked fingers aren't swapped for a marginal
  cost improvement during a pinch/rotate.
- Scaled the ghost-merge radius down (not up) as finger count rises, so
  genuinely distinct close fingers stop being merged into one at higher
  density.
- Fixed a CCL label-reuse bug that could corrupt a nearby blob's ellipse
  data in dense multi-finger frames.
- Fixed stale `TOUCH_MAJOR/MINOR/ORIENTATION` being reported after a blob
  split.
- Fixed `raw_detect_peaks()`: it compared each cell against only 4 fixed
  points at one exact radius instead of scanning its full neighborhood,
  so every touched cell of a blob (not just its center) counted as a
  "peak" — exhausting the shared 16-peak-per-frame budget after 1-2
  blobs and silently dropping any 3rd+ simultaneous finger. Replaced
  with a true local-maximum neighborhood scan.
- Closed a handshake-watchdog gap where a cold-boot retry timer could
  tear down an already-confirmed-working raw stream.
- Added `tests/raw_pipeline_replay_test.c`: links the real
  `driver/mshw0231-raw.c` pipeline (not hand-written mirrors) against
  saved synthetic heatmap fixtures (`tests/fixtures/raw-replay/`),
  replaying 1-5 simultaneous synthetic touches through the actual code.
  All two independently-verified bugs above (and the peak-detection fix)
  were found and confirmed via this harness plus a double-blind
  post-merge code review.
- Verified on real Surface Laptop 4 hardware: single-touch confirmed
  working end-to-end (raw `/dev/input` capture); simultaneous multi-touch
  not yet re-verified on hardware after the peak-detection fix.

### Installer consolidation

- Replaced `tools/install.sh`, `tools/uninstall.sh`, `tools/activate-fch.sh`,
  and `tools/rebuild_and_install.sh` with a single `tools/sl4a-touch.sh`
  covering `install` / `uninstall` / `activate` / `status` / `logs` /
  `rebuild`. `install` now prompts interactively for a profile (standard
  HID, marked stable/recommended, vs. raw multitouch, marked
  experimental) when run on a terminal without `--standard`/`--raw`;
  non-interactive runs still default to standard.
- Added `status` (installed vs. checkout version, active profile, loaded/
  bound state — read-only, no root) and `logs` (single-file diagnostic
  bundle: versions, DKMS/modprobe state, driver sysfs stats, filtered
  dmesg — for bug reports).
- Fixed a pre-existing bug in `install.sh` that made it fail to parse at
  all under this system's bash (`*[|/\\]*` inside a `[[ ]]` bracket
  expression needs the `|` escaped).
- `install` now activates immediately at the end — no separate manual
  step for the current session.
- `install` also writes and enables a systemd oneshot unit
  (`sl4a-touch-activate.service`, gated on `multi-user.target`) so the
  driver activates automatically on every future boot too. This runs
  after the base system is already up, not during early kernel/initrd
  boot — the actual failure mode the original "Boot safety (black-screen
  fix)" work (see the `refactor/sl4a-distribution (2026-07-23)` entry
  below) was protecting against came from kernel-level ACPI/SPI module
  aliases resolving during early boot with no shell and no recovery
  path. A unit gated on `multi-user.target` runs well past that point:
  a hung or failed activation here still leaves a working login.
  `uninstall` disables and removes the unit (ownership-marker gated,
  same pattern as the modprobe.d config).
- Running the script with no arguments now shows an arrow-key menu
  (Install/Uninstall/Activate/Status/Logs/Quit) instead of a usage/error
  message — no subcommand needs to be typed or remembered. Explicit
  subcommands still work unchanged for scripting/docs.
- Found and fixed four real bugs by actually exercising `activate`
  end-to-end against this machine's live installed state (DKMS,
  Secure Boot enabled, real Surface ACPI topology) rather than only
  `--check`/`--dry-run`, which none of the earlier testing had
  exercised:
  - The Secure Boot / MOK enrollment check trusted `mokutil
    --test-key`'s exit code, which is 1 on this system even when the
    key IS in the enrolled MOK database (mokutil separately checks the
    *running* kernel's live trusted keyring, which only reflects an
    enrollment after the next reboot, and still prints "already
    enrolled" while exiting 1). Combined with `set -o pipefail`, piping
    mokutil's output straight into `grep` made the whole pipeline
    report failure regardless of what grep found. Fixed by capturing
    mokutil's output separately from its exit code.
  - A "no other MSHW\* ACPI device may exist" check unconditionally
    refused to activate on real Surface hardware, which always exposes
    several unrelated MSHW\* nodes (keyboard, sensors, battery, ...)
    with their own drivers already bound. Removed it — the actual
    identifying check (exactly one MSHW0231) was already separate and
    sufficient.
  - The arrow-key menu leaked raw `tput` cursor/erase escape sequences
    onto stdout instead of stderr, corrupting the captured command name.
  - The menu's redraw logic moved the cursor up one line short of what
    it actually printed each frame, so every keypress scrolled the
    terminal instead of redrawing in place.
- Dropped a confusing "The git repo itself was not touched" line from
  `uninstall`'s output; replaced with how to reinstall.

### CI

All three GitHub Actions jobs (`whitespace`, `host-tests`,
`kernel-build`) had been failing since at least the `v1.3.0` tag:

- `kernel-build`: `driver/spi-amd.c`'s `platform_driver.remove` is
  void-returning, matching Linux 6.11+ (this driver targets upstream
  v6.15). Ubuntu 24.04 LTS's 6.8 kernel headers — CI's target, and what
  many real users on non-rolling distros still run — predate that
  signature change and reject it outright under `-Werror`. Fixed with a
  `LINUX_VERSION_CODE` conditional.
- `kernel-build`: `driver/spi-hid-capimg.c` unconditionally included
  `<linux/unaligned.h>`, which doesn't exist on that same 6.8 kernel
  (only the older `<asm/unaligned.h>`). Fixed with `__has_include`
  instead of another version check, since the exact boundary isn't a
  clean single version across distro backports.
- `host-tests`: `raw_pipeline_math_test` and `raw_pipeline_replay_test`
  linked with `-lm` placed *before* the object/source files needing it —
  works with this dev machine's linker defaults, fails under `-Wl,--as-needed`
  (the Ubuntu CI runner's default) with "undefined reference to `sqrt`".
  Fixed the link order.
- `whitespace`: `git diff --check` was flagging ~34 files, almost all
  verbatim forensic/reference data (`captures/`, `docs/acpi/`
  iasl-decompiled tables, `docs/decomp/` pasted decompiler output,
  `tools/windows_capture/`, `traces/`) where "fixing" the whitespace
  would mean editing evidence data. Excluded those paths from the check.
  Fixed the small number of remaining real hits directly
  (`tools/parse_spi.py`, one incidental blank-with-whitespace line, one
  inconsistent markdown hard-break, one stray blank line at EOF in
  `driver/hardcoded_rd.h`).
- Fixed a same-day regression: `tests/boot_binding_safety_test.py` still
  read the three now-deleted `tools/{install,uninstall,activate-fch}.sh`
  files after the installer consolidation above; updated it to read the
  consolidated `tools/sl4a-touch.sh`.
- Bumped `actions/checkout` to v5 (clears a Node.js 20 deprecation
  warning on every run).

## 1.3.0 — Production Hardening (2026-07-23)

### Double-Blind Audit
- 12 independent agents audited 6 domains (controller, HID transport, raw consumer,
  headers/protocol, tests, scripts/DKMS) in parallel.
- 3 findings confirmed as false positives by cross-verification against source.
- 45 confirmed findings: 6 critical, 21 high, 18 medium.

### Critical Fixes
- `spi_hid_latency_show`: fix snprintf buffer overflow (PAGE_SIZE not reduced).
- `hid_allocate_device`: fix NULL-vs-ERR_PTR check causing NULL dereference.
- `spi-amd` combined TX+RX: add tx_len chunking when exceeding FIFO (70 bytes).
- `spi-amd`: check `amd_spi_set_opcode` return value at all 3 call sites.
- `install.sh`: fix sed injection via PKG_VERSION metacharacters.
- `uninstall.sh`: add missing `-o pipefail`.

### High Fixes
- `debug_level` renamed to `sl4a_debug_level` (was non-static global).
- Remove no-op lock/unlock in suspend; stop bypassing HID core `claimed` field.
- Fix poll_work off-by-one byte offsets (data_buf[6]->[5], [8]->[7]).
- `spi-amd`: FIFO combined-length validation (tx+rx+echo <= 70).
- `spi-amd`: data preserved after PSP post-execute takeover.
- `mshw0231-raw`: clamp overflow guards on ghost_dist and blob_max_distance.
- `mshw0231-raw`: fix u32->u16 truncation in screen coordinates.
- `mshw0231-raw`: fix Hungarian row_match stale assignments per round.
- `mshw0231-raw`: `input_register_device` failures now return -ENODEV.
- `spi-hid-core.h`: remove __packed from isolated_set_frame (misaligned __u64).
- `spi-hid_trace.h`: guard memcpy against negative tx_len.
- `spi-hid-core.h`: unify device_descriptor_register/hid_desc_addr to u32.
- `activate-fch.sh`: wait timeout increased 2s -> 15s.

### Medium Cleanup (-177 lines)
- Removed ~30 unused constants, isolated_set subsystem, raw_transition fields,
  raw_capture_lock, latencies ring buffer, DEV_EVENT/BUS_ERROR/POWER_SUPPORT.
- Deduplicated vendor_init/sf_cmd/gf_cmd byte arrays (4->1 static const each).
- Removed V1 code paths and DBG_VERBOSE blocks from spi-amd.
- Fixed c590 LUT precision (22 -> 22204/1000).
- Removed touch_threshold_pct (dead param), GRID_CELLS (dead define).
- Moved param clamp from hot path to init (one-time instead of per-frame).
- Activated descreq_work polling (scheduled on WAIT_DESC transition).
- Added 6 orphaned test files to Makefile.

### New Tests (+30 assertions)
- `c590_atan2_host_test.c`: 33 assertions for LUT values and atan2 fixed-point math.
- `capimg_decoder_host_test.c`: extended from 41 to 71 assertions (null pointers,
  magic bytes, content_id, container length, body length boundaries, reserved
  fields, raster sample count, section types, missing sections, zero-length).
- `protocol_test.c`: extended from 92399 to 92409 assertions (parse_content
  boundaries, output_length_valid range).
- **Total gate**: 92,513 assertions, 0 failures, ASan/UBSan clean, LLVM=1 clean.

---

## refactor/sl4a-distribution (2026-07-23)

### Boot safety (black-screen fix)

- Renamed modules to `sl4a-spi-amd`/`sl4a-spi-hid`; no longer collide with
  in-tree `spi-amd` or auto-bind at boot.
- Removed all ACPI/OF/SPI module aliases from both modules.
- Added `tools/activate-fch.sh` for explicit post-login controller + HID
  transport binding with rollback on failure.
- Added PSP ownership gate (`SPI_MISC_CNTRL` bit 10) before every MMIO write;
  PSP-owned controller returns `-EBUSY`.
- Deferred speed programming to transfer path; setup performs no MMIO writes.
- SPI100/SPD7 bits are now set-only, never cleared — preserves firmware state
  and avoids documented FCH freeze condition.
- Added regression tests for alias absence, ownership gating, and speed safety.

### Phase 3 — Internal boundaries

- Extracted raw CapImg decoder, heatmap, blob tracker, and input publication
  into `driver/mshw0231-raw.c`. Core retains SPI, IRQ, V0 sequencing,
  activation timing, and watchdogs.
- Added `docs/AMDI0060_CONTRACT.md` — controller-specific invariants.
- Added `docs/PARAMETERS.md` — full release/diagnostic/experimental map.
- All raw pipeline and calibration controls are now load-time-only (`0444`).
- `raw_input_beta` defaults to `false`; `install.sh --raw` sets it explicitly.
- `raw_mode` defaults to `0` (standard HID), unlike v1.1.0 which defaulted to `1`.
  Standard profile provides single-touch only; opt-in to raw mode for multi-touch.
- Corrected misleading parameter descriptions (getfeat_delay_ms, touch_signal_mode,
  touch_threshold_pct, acpi_probe_power_cycle, grid geometry).

### Phase 4 — Replay and evidence

- Versioned replay corpus `tests/fixtures/replay/v1` with eight real V0 frames,
  SHA-256, provenance, three deterministic malformed inputs.
- Required `tests/replay_fixture_test.py` — no skip path.
- Decoder C test replays eight tracked raw-slot frames (41 assertions).
- Hardware evidence collection suite:
  - `tools/hardware_evidence/collect.sh` — dynamic ACPI/SPI/input discovery.
  - `tools/hardware_evidence/capture_direct_touch.sh` — bounded touch evtest.
  - `tools/hardware_evidence/capture_stylus.sh` — bounded stylus evtest.
  - `tools/hardware_evidence/capture_linux_trace_bundle.sh` — provenance,
    sysfs state, journal window, optional input capture, SHA-256 manifest.
  - `tools/hardware_evidence/run_blinded_session.sh` — opaque-session runner
    with role-separation worksheet and checksum verification.

### Phase 5 — CI

- `.github/workflows/ci.yml`: whitespace, host tests, sanitizers, out-of-tree
  Ubuntu generic-header build smoke test.
- `driver/Makefile` supports externally-set `KDIR` and `M`.
- Added `docs/ROLLBACK.md` with uninstall, upgrade, kernel-update, and
  Secure Boot / MOK enrollment procedures.

### Hardware evidence (standard HID)

Cold boot, warm boot, 30-minute stress, and suspend/resume observed
on CachyOS 7.1.3-2 with BIOS 4.391.140. Pen and raw multi-touch remain
unobserved/unqualified. See `docs/COMPATIBILITY.md` and
`docs/BLINDED_SESSION_LOG.md`.

### Installer and DKMS

- Default modprobe config: `/etc/modprobe.d/sl4a-spi-hid.conf`.
- Installer upgrades legacy artifacts to the opt-in controller format.
- Uninstaller verifies package ownership before touching config or DKMS tree.

---

## v1.2.0 (2026-07-19)

> [!WARNING]
> **Beta software.** This is an experimental, reverse-engineered kernel driver.
> It may cause touchscreen failure, system instability, or data loss.
> Use entirely at your own risk. No warranty of any kind.

### Windows pipeline alignment (~85%)

CCL flood-fill pipeline replacing the old 5×5 local centroid approach,
matching the `TouchPenProcessor0C19.dll` chain:

- **c590 LUT** — raw 16-bit → fixed-point signal conversion
- **Peak detection gate** — cross-shaped ±5 cells, min_rise=200, collects peaks for velocity rejection
- **CCL flood-fill** — 4-connected BFS component labeling on touched cells
- **Velocity rejection** — blob centroid must be within 6 cells of peak (dist² ≤ 36.0)
- **Edge penalty** — bottom edge ×0.23, other edges ×0.97 (DLL config+0x8D0/0x8D4)
- **Blob splitting** — multi-peak blobs (≥4 cells apart, pixel_count≥8) split by internal peak positions
- **Full-blob centroid** — signal-weighted ×100 fixed-point over blob extent
- **Eigenvalues** — second moments on blob bounding box → touch major/minor/orientation
- **Pre-merge** — ghost_dist=6 cells, keep strongest
- **Hungarian assignment** — multi-finger association radii from DLL config:
  1 finger ×2.2, 2×1.0, 3×2.8, 4×3.4, 5+×4.0
- **EMA + deadband + stationary lock** — alpha=7, ±0.8-cell deadband, 6-frame lock
- **Lift lookback** — 2-frame history position on lift

### DLL config table integration

20+ values extracted from `TouchPenProcessor0C19.dll` data segment
(VA 0x1808E0460, file offset 0x8DF060): association radii, edge weights,
coalescing threshold, noise floor, pre-assoc ratio, hold policy, touch
detection threshold. See `docs/CONFIG_TABLE.md`.

### Parameter tuning

- `min_rise` 300→200 for weak fingers at 3+ density
- `hold_frames=0` (disabled — caused scroll brake)
- `pre_assoc_ratio=0` (disabled — too aggressive without classifier)
- `ghost_dist=6` (matches DLL +0xC98=36.0)

### Documentation overhaul

- **GitHub Wiki** — 7-page technical wiki: protocol, pipeline, config table,
  hardware, build & install, reverse engineering
- **Source code** — per-field comments on `struct spi_hid`, kerneldoc for
  major functions, removed 10+ date-stamped developer diary entries
- **docs/** — 556 stale files deleted (diary, evidence, snapshot directories),
  7 technical docs rewritten, 4 new references created
- **README.md** — rewritten as project page with architecture diagram,
  feature table, module parameters, troubleshooting

### Repository cleanup

Removed 556 diary/research/evidence files including GROUND_TRUTH (3006-line
research journal), all GHIDRA snapshot directories, MSI database exports,
SESSION_HANDOFF, and experimental configs. Kept decomp/clean/, decomp/uefi/,
and acpi/ as technical reference.

Touch quality: 2-finger excellent, 3-finger good, 4-finger improved.

---

## v1.1.0 (2026-07-19)

### Multi-touch (active by default)

Raw multi-touch mode is **enabled by default** (`raw_mode=1`). The device
streams 72×48 heatmap frames processed through a pipeline matching the
Windows `TouchPenProcessor0C19.dll` chain:

- **Peak detection** — cross-shaped ±5-cell N/S/E/W scan, min rise 300
- **5×5 local centroid** — signal-weighted average with ×100 fixed-point
  sub-cell precision, eliminating visible grid stepping
- **Hungarian assignment** — minimum-cost bipartite matching with
  Windows-matching cost matrix; jump rejection prevents noise-blob
  slot theft for 3+ fingers
- **EMA + deadband + stationary lock** — alpha=7 smoothing, ±0.8-cell
  deadband, position frozen after 6 still frames to eliminate
  pinch-to-zoom jitter
- **Lift lookback** — emits lift at history position from 2 frames ago
- **Per-blob eigenvalues** — `ABS_MT_TOUCH_MAJOR/MINOR/ORIENTATION` for
  every active finger

Touch quality: 2-finger very good, 3-finger mostly stable (95%),
4-finger unstable.

### Standard HID (fallback)

Single-touch + pen (Report IDs 0x40 and 0x01) remain functional.
If the raw mode handshake fails after 3 attempts, the driver falls back
to standard HID automatically.

### 100% wire-read report descriptor

The 936-byte HID report descriptor is read live from the device every
boot. A historical PIO TX_COUNT bug (1 extra byte per 64-byte
continuation, corrupting bytes at offsets n·64+55) has been fixed by
matching the Windows `0x4bac` TX_COUNT=3 convention. The hardcoded
descriptor remains as an emergency fallback only.

### Auto-retry on cold boot

The first raw mode handshake attempt fails ~30% of the time after cold
boot. The driver now retries up to 2 additional times (5-second delay,
D2→D0 power cycle, fresh DESCREQ) before falling back to standard HID.

### Licensing

GNU General Public License v2.0. See [LICENSE](LICENSE).

### Installer

DKMS-based multi-distro installer (Arch/CachyOS, Ubuntu/Debian, Fedora,
openSUSE). Udev auto-loads the driver via `/etc/modprobe.d/spi-hid.conf`
(`raw_mode=Y skip_getfeat=Y`). No systemd service required.

### Repository cleanup

Removed 42+ stale files (calibration stubs, one-shot raw-mode scripts,
old test modprobe configs, build artifacts, disused analysis tools).
Added `.gitignore` coverage for all build artifact patterns.

---

## v1.0.0-beta1 (2026-07-08)

- Initial release: standard HID single-touch + pen working
- IRQ-driven HID-over-SPI V0 sequencer
- AMD FCH Cezanne SPI controller driver (V2)
- DKMS installer with multi-distro support
- Hardcoded report descriptor with ~98.5% wire-read + 14-byte patch
