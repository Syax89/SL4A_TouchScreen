# H8a — `wire_double_opcode=1` combinations + the standard evdev verdict

Tool-only change. Touched files: `tools/sl4a-touch.sh`, `tests/hunt_sandbox_test.sh`
(plus this write-up). No driver change.

---

## 1. The new battery plan, as implemented

`HUNT_VARIANTS` in `tools/sl4a-touch.sh` now holds **14** variants, in plan order
(10 raw + 4 standard). The three new rows sit directly after the existing
`raw wire_double_opcode=1` row, exactly as the task lists them:

| # | profile | label | params appended |
|---|---------|-------|-----------------|
| 1 | raw | `raw control` | – |
| 2 | raw | `raw raw_pre_desc_reg0=1` | `raw_pre_desc_reg0=1` |
| 3 | raw | `raw raw_fallback_on_reset=1` | `raw_fallback_on_reset=1` |
| 4 | raw | `raw raw_pre_desc_reg0=1+raw_fallback_on_reset=1` | `raw_pre_desc_reg0=1 raw_fallback_on_reset=1` |
| 5 | raw | `raw read_frame_variant=2` | `read_frame_variant=2` |
| 6 | raw | `raw wire_double_opcode=1` | `wire_double_opcode=1` |
| **7** | raw | `raw wire_double_opcode=1+read_frame_variant=2` | `wire_double_opcode=1 read_frame_variant=2` |
| **8** | raw | `raw wire_double_opcode=1+raw_pre_desc_reg0=1` | `wire_double_opcode=1 raw_pre_desc_reg0=1` |
| **9** | raw | `raw wire_double_opcode=1+read_frame_variant=2+raw_pre_desc_reg0=1` | `wire_double_opcode=1 read_frame_variant=2 raw_pre_desc_reg0=1` |
| 10 | raw | `raw wire_double_opcode=1+skip_vendor_stop=1` | `wire_double_opcode=1 skip_vendor_stop=1` |
| 11 | standard | `std control` | – |
| 12 | standard | `std wire_double_opcode=1` | `wire_double_opcode=1` |
| 13 | standard | `std skip_std_getfeat=1` | `skip_std_getfeat=1` |
| 14 | standard | `std wire_double_opcode=1+skip_std_getfeat=1` | `wire_double_opcode=1 skip_std_getfeat=1` |

The rationale (rows 1–5, 10–14 answered already; the doubled WRITE is the one
shape that delivered a descriptor on 2026-09-17, so its read-side partners are
the untried shapes) is now a comment on the array.

The counts pinned elsewhere were updated: the usage/help block reads
`(10 raw + 4 standard)`, and the two places that assert the plan
(`expected-loads.txt`, the summary/`raw_mode` aggregate counts) in
`tests/hunt_sandbox_test.sh` were moved to 14 / 10 / 4.

The summary table's label column is no longer a hard-coded `%-48s`: it is
derived from the longest label in the plan by the new `hunt_summary_label_width()`
(floor 48). The longest new label
(`raw wire_double_opcode=1+read_frame_variant=2+raw_pre_desc_reg0=1`, 64 chars)
would otherwise have shoved the numeric columns out of line on exactly one row,
and hard-coding a new width would have broken the "a new variant is a one-line
addition and nothing else changes" promise the plan is built on. Column set and
row shape are unchanged; the table still prints one `raw `/`std ` row per variant
before the self-tests.

---

## 2. Root cause: why every standard row read `human:no`

`hunt_touch_event()` used to find the panel's input node **by name only**, against
`*MSHW* | *sl4a* | *SL4A* | *[Tt]ouchscreen*`. That matches the node the **raw**
profile registers, but not the **standard** profile's node:

* **raw mode** — the driver allocates its own `input_dev` and names it
  `"MSHW0231 Touchscreen"` (`driver/mshw0231-raw.c`, `mshw0231_raw_input_register`),
  so the name matched.
* **standard HID mode** — there is no raw device. The only input node is the one
  the HID core creates, and it is named after the HID device, which the driver
  sets to `snprintf(hid->name, ..., "spi %04hX:%04hX", vendor, product)`
  (`driver/spi-hid-core.c`). On this panel that is **`spi 045E:0C19`**
  (`docs/QUICKSTART.md` states the same). That string contains none of
  `MSHW` / `sl4a` / `Touchscreen`, so the discovery returned nothing, the read
  branch was skipped, and the fallback prompt produced `human:no` on every
  standard variant — while the node it should have read was sitting right there.

So it was the **device-node discovery**, not the read path or a wrong fallback
condition, that broke the standard profile. (The read path is profile-agnostic —
same `/dev/input/eventN`, same `hunt_evdev_read`.)

### The fix

Discovery now runs **two passes**:

1. **by name** — the raw patterns (unchanged), so a raw run still prefers its own
   stream device when the HID node also exists; then
2. **by ancestry** — the node whose resolved `eventN/device` path
   (`readlink -f`) lies under the panel's controller `$SYSFS_DIR`.

Both node shapes hang off the controller: the raw device's parent *is* the SPI
device, and the standard hid device is created with
`hid->dev.parent = &shid->spi->dev`. So the resolved path of either node's
`device` entry is `<spi-MSHW0231:00>/…`, and the ancestry test finds the
standard node without hard-coding `MSHW0231`/`MSHW0162` or the `045E:0C19`
name — it is scoped to *our* panel, not to "any SPI HID device".

### The fallback is now explicit

In the touch window the two genuinely-fallback cases are separated and named in
the artifact:

* a node is present but its char device is not readable →
  `no readable char device at /dev/input/<node> for <node> — human fallback: …`
* **no node was registered by this load** →
  `no input event node under the panel's controller (<sysfs>) — this load registered none`,
  followed by the human `y/n`.

The last one is the honest field answer for standard mode whenever the handshake
never delivers a descriptor (no descriptor → no HID device → no input node), so
the tool now says exactly that instead of emitting a bare `human:no`.

---

## 3. Sandbox updates (`tests/hunt_sandbox_test.sh`)

* The fake panel input node is rebuilt so its `device` entry **resolves under the
  panel controller** (`.../spi-MSHW0231:00/input/input10`), and the `modprobe`
  stub now **names it per profile** — `MSHW0231 Touchscreen` for `raw_mode=Y`,
  `spi 045E:0C19` for `raw_mode=N` — mirroring the driver.
* `expected-loads.txt` gained the three new load lines in plan order;
  `NVARIANTS=11 → 14`.
* Pins updated: `=== SUMMARY (14 variants) ===`, the awk row count `n == 14`,
  `[1/14]` progress, and the aggregate `raw_mode=Y` count `NSWEEPS * 10`.
* **Per-profile evdev expectation**: new asserts that the standard node is
  recognised (`spi 045E:0C19.*<-- touch device`) and that the `std control` row
  carries an event count (`… | 2 events | resets +6`) — not `human:no`.
* No-panel run: the node goes away with the controller, so the run now states
  `no input event node under the panel`; stdin stays `/dev/null` so the honest
  fallback read returns immediately.

**The new pin bites.** Mutating the ancestry case pattern to a never-matching
value makes the sandbox fail with
`the standard profile's HID node was not recognised as the touch device`
(then restored).

---

## 4. Full suite tail

`make -C tests test` — exit 0. Tail:

```
python3 binattr_signature_test.py
bin_attribute signature guard: PASS (compiles on 6.8/6.13/6.16, revert fails on 6.16)
python3 diagnostic_bundle_contract_test.py
diagnostic bundle frame contract: PASS
python3 diagnostic_bundle_frame_test.py
complete frame: 3456/3456 bytes round-tripped
count follows the read that produced the bytes
empty frame: explicit, not '0 bytes, complete'
older module: labelled truncated hex view
no attribute: explicit message
diagnostic bundle frame section behaviour: PASS
python3 installer_recovery_contract_test.py
installer recovery contract: PASS
python3 tools_contract_test.py
tools_contract_test: PASS
python3 hardcoded_descriptor_pin_test.py
hardcoded_descriptor_pin_test: PASS (936 bytes identical to the surface_init capture)
/bin/sh hunt_sandbox_test.sh
hunt sandbox contract: PASS (battery completes, 14 variants loaded in plan order with the right raw/standard mode, rebuild path survives and really rebuilds, summary table with 14 rows before the self-tests, evdev touch verdict measured, counter deltas computed, first write survives the window, live readback present, no-panel run warns and degrades honestly, quiet load states its absence and survives, wrapped ring labels the fallback read, no-evdev run falls back to y/n, module reads scoped to the stub sysfs)
```

---
## M1a — docs wave

# M1a — parameter documentation brought to the actual code state
Docs-only change. Touched files: `docs/PARAMETERS.md`, `README.md`,
`.wiki/Config-Table.md` (plus this write-up). No driver change, nothing
committed.

Source of truth: every `module_param` in `driver/*.c`. Verified with
`grep -rn module_param driver/*.c` and the checker `.verify/_paramcheck.py`
(removed after use).
## 1. THE TASK'S PARAMETER LIST IS WRONG — flagged loudly
The task states **49** `module_param`s at the pinned revision and lists **45**
names. The code contains exactly **40** `module_param` identifiers, each declared
once:

```
$ grep -rhoP 'module_param\(\s*\K[A-Za-z0-9_]+' driver/*.c | wc -l
40
```

**Five of the listed names do not exist anywhere in the repository or its git
history:**

| Name (as listed in the task) | Evidence |
|---|---|
| `get_noread` | not in any file; `git log --all -S get_noread` → no commit |
| `raw_handshake_first_ms` | not in any file; `git log --all -S raw_handshake_first_ms` → no commit |
| `raw_no_enable` | not in any file; `git log --all -S raw_no_enable` → no commit |
| `raw_watchdog_teardown` | not in any file; `git log --all -S raw_watchdog_teardown` → no commit |
| `std_raw_transition` | not in any file; `git log --all -S std_raw_transition` → no commit |

A whole-tree, case-insensitive search also returns nothing:
`grep -rniI '<name>' .` → no hits (only `.git` excluded).
The task also cites commit `99c4338` as "main"; `git cat-file -t 99c4338`
returns `fatal: Not a valid object name` — that commit is not in this clone
either.
**Decision:** per the hard rule ("Never invent a parameter, a class or a
default: every row must be grep-traceable to `driver/*.c`"), these five names
were **not** placed in any table, and no class or default was invented for
them. They are recorded here instead so the coordinator can see the mismatch
rather than a silently "completed" 45/49-row table that would be fiction.
### Stale names already present in the docs (also not real parameters)
| Name | Status | Evidence |
|---|---|---|
| `debug_level` | wrong name — the real parameter is `sl4a_debug_level` | only `sl4a_debug_level` is a `module_param` (`driver/spi-hid-core.c:2376`); `grep -rn '\bdebug_level\b' driver/` → no match |
| `touch_signal_mode` | not a parameter at all | `grep -rn touch_signal_mode .` → only `docs/PARAMETERS.md` |
| `touch_threshold_pct` | not a parameter at all | `grep -rn touch_threshold_pct .` → only `docs/PARAMETERS.md`; `CHANGELOG.md:1206` already records "Removed touch_threshold_pct (dead param)" |
`docs/PARAMETERS.md` was corrected: `debug_level` → `sl4a_debug_level`, and the
two phantom placeholders were removed (the "unused placeholders" sentence went
with them).
---

## 2. Inventory — all 40 parameters, class, declaration line, default

`class` = the class used in `docs/PARAMETERS.md` / `.wiki/Config-Table.md`.
`default` = the value read from the declaration, not guessed. `module_param`
line is the `module_param()` call; the default lives on the declaration line
(shown where different).

| Parameter | Class | module_param `file:line` | default (as read) |
|---|---|---|---|
| `raw_mode` | Standard safety | `spi-hid-core.c:2157` | `0` (`static bool raw_mode;` :2152) |
| `sl4a_debug_level` | Diagnostic | `spi-hid-core.c:2376` | `0` (`int sl4a_debug_level;` :54) |
| `debug_trace` | Diagnostic | `spi-amd.c:47` | `0` (`static int debug_trace;` :46) |
| `std_liveness_ms` | Diagnostic | `spi-hid-core.c:2410` | `0` (`static int std_liveness_ms;` :2409) |
| `raw_input_beta` | Experimental activation | `spi-hid-core.c:2163` | `0` (`static bool raw_input_beta;` :2162) |
| `skip_getfeat` | Experimental activation | `spi-hid-core.c:2352` | `1` (`static bool skip_getfeat = true;` :56) |
| `getfeat_delay_ms` | Experimental activation | `spi-hid-core.c:2380` | `0` (`static int getfeat_delay_ms;` :55) |
| `setfeat_speed_hz` | Experimental activation | `spi-hid-core.c:2185` | `0` (`static uint setfeat_speed_hz;` :2184) |
| `wire_double_opcode` | Experimental activation | `spi-hid-core.c:2211` | `0` (`= SPI_HID_WIRE_DOUBLE_DEFAULT` :60; macro `0` `spi-hid-wire-frames.h:67`) |
| `setfeat_no_double` | Experimental activation | `spi-hid-core.c:2220` | `0` (`static bool setfeat_no_double;` :135) |
| `read_frame_variant` | Experimental activation | `spi-hid-core.c:2153` | `1` (`= SPI_HID_READ_FRAME_LEGACY` :1325; macro `1` `spi-hid-core.h:32`) |
| `acpi_probe_power_cycle` | Experimental activation | `spi-hid-core.c:2168` | `0` (`= false` :2167) |
| `sync_timeout_ms` | Experimental activation | `spi-hid-core.c:2172` | `6000` (`= SPI_HID_PROTOCOL_SYNC_TIMEOUT_MS_DEFAULT` :149; macro `6000` `spi-hid-protocol.h:39`) |
| `stream_watchdog_ms` | Experimental activation | `spi-hid-core.c:2386` | `2000` (`static int stream_watchdog_ms = 2000;` :2385) |
| `stream_watchdog_max_retries` | Experimental activation | `spi-hid-core.c:2391` | `3` (`static int stream_watchdog_max_retries = 3;` :2390) |
| `skip_vendor_stop` | Experimental activation **(new)** | `spi-hid-core.c:2194` | `0` (`static bool skip_vendor_stop;` :69) |
| `raw_fallback_on_reset` | Experimental activation **(new)** | `spi-hid-core.c:2197` | `0` (`static bool raw_fallback_on_reset;` :79) |
| `raw_pre_desc_reg0` | Experimental activation **(new)** | `spi-hid-core.c:2200` | `0` (`static bool raw_pre_desc_reg0;` :93) |
| `raw_b1f8109_preset` | Experimental activation **(new)** | `spi-hid-core.c:2204` | `0` (`static bool raw_b1f8109_preset;` :131) |
| `std_liveness_recover` | Standard-mode recovery | `spi-hid-core.c:2362` | `0` (`static bool std_liveness_recover;` :142) |
| `skip_std_getfeat` | Standard-mode recovery | `spi-hid-core.c:2357` | `0` (`static bool skip_std_getfeat;` :139) |
| `wait_reset_kick_ms` | Standard-mode recovery | `spi-hid-core.c:2367` | `0` (`static int wait_reset_kick_ms;` :145) |
| `blob_min_weight` | Raw pipeline | `mshw0231-raw.c:49` | `1000` (`= 1000` :48) |
| `ema_alpha` | Raw pipeline | `mshw0231-raw.c:54` | `2` (`= HEATMAP_EMA_ALPHA_DEFAULT` :53; macro `2` `mshw0231-raw-constants.h:113`) |
| `dfa_data_offset` | Raw pipeline | `mshw0231-raw.c:59` | `0` (`static int dfa_data_offset;` :58) |
| `ghost_dist` | Raw pipeline | `mshw0231-raw.c:64` | `6` (`= 6` :63) |
| `grid_cols` | Raw pipeline | `mshw0231-raw.c:69` | `0` (`= 0` :68) |
| `grid_rows` | Raw pipeline | `mshw0231-raw.c:73` | `0` (`= 0` :72) |
| `blob_debounce` | Raw pipeline | `mshw0231-raw.c:93` | `3` (`= 3` :92) |
| `blob_lift_frames` | Raw pipeline | `mshw0231-raw.c:97` | `3` (`= 3` :96) |
| `hold_frames` | Raw pipeline | `mshw0231-raw.c:101` | `0` (`= 0` :100) |
| `pre_assoc_ratio` | Raw pipeline | `mshw0231-raw.c:106` | `0` (`= 0` :105) |
| `blob_max_distance` | Raw pipeline | `mshw0231-raw.c:111` | `3` (`= 3` :110) |
| `invert_x` | Raw calibration | `mshw0231-raw.c:35` | `0` (`static bool invert_x;` :34) |
| `invert_y` | Raw calibration | `mshw0231-raw.c:39` | `0` (`static bool invert_y;` :38) |
| `swap_xy` | Raw calibration | `mshw0231-raw.c:43` | `0` (`static bool swap_xy;` :42) |
| `calib_scale_x` | Raw calibration | `mshw0231-raw.c:77` | `0` (`= 0` :76) |
| `calib_scale_y` | Raw calibration | `mshw0231-raw.c:81` | `0` (`= 0` :80) |
| `calib_offset_x` | Raw calibration | `mshw0231-raw.c:85` | `0` (`= 0` :84) |
| `calib_offset_y` | Raw calibration | `mshw0231-raw.c:89` | `0` (`= 0` :88) |

`_paramcheck.py` confirms: 40 real params, **0** missing from the
`docs/PARAMETERS.md` class table, **0** duplicate class members, **0** table
members that are not real params. Same for the `.wiki/Config-Table.md` table
(40 rows).
---
## 3. The four new rows — the file:line that put each in its class
"New" = present in the code but absent from the previous tables. All four are
consumed on the **raw handshake / recovery** path (the same path as the
already-classified `skip_getfeat`, `wire_double_opcode`, `read_frame_variant`),
so they go in **Experimental activation**.
| New row | Class-determining use site | What the site does |
|---|---|---|
| `skip_vendor_stop` | `driver/spi-hid-core.c:761` (`if (skip_vendor_stop)`, inside `spi_hid_vendor_init`) | returns before the `vendor_stop` + `D2`/`D0` preamble → handshake/power sequencing |
| `raw_fallback_on_reset` | `driver/spi-hid-core.c:1986` (`if (raw_fallback_on_reset || raw_b1f8109_preset)`, in the DESCREQ poller `RESET_RSP` branch) | gives up to the hardcoded fallback descriptors instead of retrying → raw recovery |
| `raw_pre_desc_reg0` | `driver/spi-hid-core.c:1466` (pre-`DONE` read destination) **and** `driver/spi-hid-core.c:4288` (skips the probe-time stream-register force) | read-path/handshake destination |
| `raw_b1f8109_preset` | `driver/spi-hid-core.c:688` + `:696` (doubled-opcode OR), `:768` (vendor-init `STOP` skip), `:1466` (read destination), `:1986` (poller give-up), `:4288` (probe force skip) | composite handshake/recovery restore |
Defaults for all four are read from their declarations (`:69`, `:79`, `:93`,
`:131`) and are all `0`.
The old tables were also missing nothing else: the other 36 params were already
present (with the `debug_level`/placeholder corrections in §1).
## 4. Qualified language — deliberately NOT changed

`docs/PARAMETERS.md` still opens "No input behavior is release-qualified yet."
This is **not demonstrably wrong**, so it was left as-is (the task says to leave
qualification language alone minus what is demonstrably wrong, and forbids
declaring a release qualification):

- `docs/COMPATIBILITY.md:15` — the release-qualified table's only row reads
  `| No qualified result recorded | … |`.
- `README.md:24` — "**Raw mode is experimental** and not release-qualified."

The opener was **not** turned into a positive qualification claim.
## 5. Ambiguities / flagged for the coordinator

1. **The task's list is wrong (§1).** 40 real params, not 49; 5 names
   (`get_noread`, `raw_handshake_first_ms`, `raw_no_enable`,
   `raw_watchdog_teardown`, `std_raw_transition`) exist nowhere. Nothing was
   invented for them.
2. **`std_liveness_ms` deduplicated to Diagnostic.** It previously appeared in
   *both* Diagnostic and recovery; the task requires each param exactly once.
   Its `MODULE_PARM_DESC` is "Logs, never recovers" and its default is `0`, which
   matches the Diagnostic contract ("Logging only. All default to zero."), so it
   sits in Diagnostic and is cross-referenced from the recovery row. If the
   reviewer prefers it under recovery, it is a one-line move.
3. **Wiki "standing rule".** The `.wiki/Config-Table.md` table I added/refreshed
   contains no dates, no decompilation offsets (`+0x…`), and no analysis-tool
   names — verified. **However, the rest of that page still carries the
   pre-existing Windows-DLL material (offsets like `+0x8DC`, `TouchPenProcessor
   0C19.dll`, `FUN_1806…`).** I left that untouched because the task scoped this
   to "same coverage" of the parameters; if the standing rule is meant to apply
   to the *whole* wiki page, that is a separate, larger edit.
4. **`.wiki/Config-Table.md` "Linux parameter mapping" section was replaced** by
   a full "Module parameters" table. The old 5-row mapping table used `+0x…`
   config-value cells and only covered `ghost_dist`, `blob_max_distance`,
   `hold_frames`, `pre_assoc_ratio` (+ built-in edge penalties); the parameter
   coverage now lives in the new table. The DLL↔parameter linkage is still
   described in the page's per-field sections above it.
5. **`ema_alpha` default** is the macro `HEATMAP_EMA_ALPHA_DEFAULT` = `2`
   (`mshw0231-raw-constants.h:113`), matching the value already stated in
   `docs/PARAMETERS.md`; recorded here as read-from-code, not guessed.
6. **No driver change and no commit.** Coordinator harvests.

---
## 6. Verification run
$ grep -rn module_param driver/*.c | grep -v MODULE_PARM_DESC | wc -l   # 40
$ python3 .verify/_paramcheck.py
  total module_params = 40
  missing from PARAMETERS.md = []
  dupe class members = []
  wiki table rows = 40 ; wiki rows not a real param = []
  real params missing from wiki = [] ; rule violations in new table = []

---
## M2a — docs wave

# M2a — wiki refresh: findings
Scope: port the 7 legacy wiki pages into `.wiki/`, refresh every capability /
status claim against the driver source, and apply the standing wiki rules
(no dates, no decompilation references, no analysis-tool or script names, no
field-log narration). Every claim below was checked against the code at the
pin; the code wins. `file:line` is the evidence.
## A. Headline: the task's "doubled form" premise is contradicted by the code

The task states: *"both installed profiles carry the doubled form now — verify
against the profiles in `tools/sl4a-touch.sh`"*, implying `.wiki/Protocol.md`
needs to drop its "single/undoubled is the default" prose.

**Verified: the premise is false; the single (undoubled) form is still the
default, and neither installed profile selects the doubled form.**

- `tools/sl4a-touch.sh:840` — raw profile:
  `options sl4a_spi_hid raw_mode=Y raw_input_beta=Y skip_getfeat=Y`
- `tools/sl4a-touch.sh:845` — standard profile:
  `options sl4a_spi_hid raw_mode=N`
  (neither sets `wire_double_opcode` or `raw_b1f8109_preset`)
- `driver/spi-hid-wire-frames.h:67` — `#define SPI_HID_WIRE_DOUBLE_DEFAULT 0`
- `driver/spi-hid-core.c:60` — `static bool wire_double_opcode = SPI_HID_WIRE_DOUBLE_DEFAULT;`
- `driver/spi-hid-core.c:684-697` — `spi_hid_wire_doubled()` returns
  `wire_double_opcode || raw_b1f8109_preset`, both `0` by default.
- `tests/wire_frames_test.c:32` — `_Static_assert(SPI_HID_WIRE_DOUBLE_DEFAULT == 0, …)`

**Action taken:** `.wiki/Protocol.md`'s opcode-default prose
("opcode `0x02` (a single one…)" at line 31; "one `0x02` opcode … the
doubled-opcode legacy form (`02 02 …`) is emitted only with
`wire_double_opcode=1`" at lines 126-129) is **correct and was left unchanged**.
The doubled form is a non-default, opt-in module parameter. No profile, install
path or default enables it.

*(The doubled form is what the last raw-answering build emitted
(`raw_b1f8109_preset`, OR-ed in at `spi-hid-core.c:686-688`), but that preset is
default OFF and is not set by any installed profile either.)*
## B. Corrected claims (page · old text · code evidence)

1. **Architecture.md** — *"state 3 `VENDOR_INIT` … Raw mode only: vendor
   initialization before feature exchange"*.
   The driver never enters state 3.
   Evidence: `driver/spi-hid-core.h:102` ("nothing sets it today"); the only
   occurrences of `SPI_HID_SEQ_VENDOR_INIT` in `spi-hid-core.c` are `switch`
   case labels (`:209`, `:2888`) — there is no assignment. Corrected to "defined
   for the reference's ordering; the driver never enters it".

2. **Architecture.md** — *"in raw mode it first passes through
   `VENDOR_INIT`/`WAIT_FEATURE`"*.
   Raw mode reaches `DONE` directly (or parks in `WAIT_FEATURE` when the
   connect-time feature query is awaited); it never passes through
   `VENDOR_INIT`.
   Evidence: `driver/spi-hid-core.c:1886` (DONE) and `:1901` (WAIT_FEATURE).

3. **Architecture.md** — *"A poll fallback (`poll_work`) exists in the raw path
   but is not armed in the current driver"*.
   It **is** armed: entering `DONE` in raw mode sets `poll_active` and schedules
   `poll_work` at 20 ms.
   Evidence: `driver/spi-hid-core.c:304-308`.

4. **Architecture.md** — sysfs table lists `spi_hid_perf_mode` as **RW**
   ("Performance mode tunable").
   It is **read-only**; the writable attribute was removed because nothing
   consumes the value.
   Evidence: `driver/spi-hid-core.c:3756-3759` (`DEVICE_ATTR_RO`, comment
   "Read-only on purpose"). Also added the missing `heatmap_debug` (RO) row.

5. **Architecture.md, Multi-touch-Experimental.md, Build-and-Install.md** —
   `stream_watchdog_ms` *"default 0 = disabled"* / *"disabled by default"*.
   The default is **2000**.
   Evidence: `driver/spi-hid-core.c:2385` (`static int stream_watchdog_ms = 2000;`),
   `:2387-2388` (desc "0=disable").

6. **Multi-touch-Experimental.md** — *"The payload is dual-frequency DFT antenna
   data, **not** a plain rectangular capacitance map"*.
   The code consumes it as a rectangular per-cell raster: one byte per cell,
   row-major, row stride = grid width (72 / 78).
   Evidence: `driver/spi-hid-core.c:2135-2142` ("capacitive node magnitudes
   (72 columns x 48 rows, row-major, 1 byte each, 72-byte row stride)");
   `driver/spi-hid-capimg.c:62-69` (heatmap section validated as
   `16 + expected_samples` bytes). Rewritten to describe the raster rather than
   to deny it.

7. **Multi-touch-Experimental.md, Standard-Touch-Mode.md,
   Building-Usage-and-Troubleshooting.md** — mode labelled *"experimental"*.
   Per the task the raw path is functional and carries the **Beta** label; the
   word "Experimental" was removed from the status prose describing the I/O
   paths. ("Multi-touch (Beta)" titles/labels; filename unchanged — see E.)

8. **Architecture.md** — *"`sudo ./tools/sl4a-touch.sh status` and
   `diagnostics` read these"*.
   There is no `diagnostics` subcommand; the bundle collector is `logs`.
   Evidence: `tools/sl4a-touch.sh:2280-2288` (only
   `install|uninstall|activate|status|logs|hunt|rebuild`).

9. **Standard-Touch-Mode.md** — the removed "real bug that lived here" note
   claimed the guard "*checks `rl - 2 <= avail`*".
   The actual length check is `rl - 3 > avail` → drop; the report is forwarded
   with `rl - 2`.
   Evidence: `driver/spi-hid-core.c:3336` (`rl >= 3 && rl - 3 > avail`);
   `:3396` (`hid_input_report(..., rl - 2, 1)`). Anecdote dropped (also
   field-log/history narration under the standing rules).
---
## C. Standing-rule cleanups applied to the existing pages

- **Home.md** — removed "decompiled Windows drivers" /
  "ACPI/decompilation dumps"; `HidSpiDeviceV0` component name dropped;
  status table row **Raw heatmap → Beta**; link labels → "(Beta)";
  license line → "beta, reverse-engineered driver".
- **Protocol.md** — removed "cross-validated against decompiled Windows
  `hidspi.sys` / `HidSpiCx.sys`" and the `HidSpiDeviceV0` name; "field-settled"
  narration dropped; `getfeat_delay_ms` row de-narrated; link label → "(Beta)".
- **Pipeline.md** — removed `TouchPenProcessor0C19.dll`, "~85% functional
  alignment", `DLL +0x8D0/+0x8D4`, "DLL config +0xECC", "Windows DLL
  provenance"; link label → "(Beta)".
- **Hardware.md** — removed "Windows `amdspi.sys` decompilation at function
  `0x4bac`"; removed `docs/decomp/…`, `tools/ghidra/`, and the NDA spec from
  References; dropped `(M010)`.
- **Build-and-Install.md** — `stream_watchdog_ms` default corrected to 2000;
  "Raw mode is experimental" → "beta"; subcommand list completed with `hunt`.
- **Reverse-Engineering.md** — rewritten: no analysis tools/scripts, no
  binary/`.sys`/`.dll` names, no function addresses, no dates; describes the
  reference model, the derivation/validation method, and the known gaps
  abstractly.

Verification (whole `.wiki/`, excluding `Config-Table.md` which another leg
owns): no dates (`20xx-xx-xx`), and no matches for
`Ghidra|RWEverything|ETW|surface_tracker|parse_spi|windows_capture|Decompile|HidSpiCx|amdspi|TouchPenProcessor|SurfaceSystemTelemetry|FUN_|0x4bac|decompil|.sys|.dll|.rdata|DAT_|PDB`
(the `HIDSPI_PROTOCOL.md` doc reference in Further-Reading is a
repository document name, not a decompilation reference).
The only surviving "Experimental" tokens are inside the link target
`Multi-touch-Experimental` (the page slug, not prose) and in `Config-Table.md`
(out of scope).
## D. NOT settled by the code — flagged, not guessed

1. **Wire-Protocol.md** carried a "Contact reports (device → host)" section
   claiming the device sends *"its own contact reports (report ID 8): a 16-bit
   scan-time stamp, a constant field block and a CRC16"* whose coordinates are
   *"pairs of float32 in raster-cell units"*. **No code or test in the repo
   consumes or documents a report ID 8 contact report with float32
   coordinates** — the standard touch report the driver forwards is ID `0x40`
   (`driver/spi-hid-core.c:3386-3396`). The section was **removed** rather than
   carried unverified. If it reflects the device, it belongs in the repo docs,
   not the wiki.

2. **Report-Descriptor.md** "four lengths, four layers". Three layers are
   code-documented (`driver/spi-hid-core.c:1146-1149`: 936 content / 939 prefix
   / 940 frame). The **945** ("wire" = 940 + the 5-byte `FF FF FF FF FF`
   preamble of `driver/spi-hid-protocol.h:107-112`) is arithmetic, not a named
   constant; kept but it is a derived number, not a code assertion.

3. **Protocol.md** "Raw-mode feature exchange" table labels the GET_FEATURE
   frame's register as `0x0042`. The frame bytes are pinned
   (`driver/spi-hid-wire-frames.h:217-227`, `02 00 00 03 42 00 04 03 00 06`), but
   the table's Register/Content-ID column semantics were **not** re-derived, so
   the table was left as-is. Flagged for a future pass with the V0 header
   encoder in hand (`spi-hid-protocol.h:130-143`).

4. The "raw = Beta" status and "both modes functional" are product statements
   from the task, not code assertions; applied as instructed.
## E. Assumption (user-owned choice, no safe code default)

The raw-mode page is titled after its historical slug
`Multi-touch-Experimental.md`, and cross-links keep that target (so every link
resolves and the public page URL is preserved). The **prose** title, status
labels and nav/sidebar labels say **"Multi-touch (Beta)"**, satisfying "keep the
Beta label / the word must disappear from the status prose". If a full rename
(slug `Multi-touch-Beta`) is wanted, it is a one-step change to the file name
plus the four link targets.
