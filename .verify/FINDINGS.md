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
