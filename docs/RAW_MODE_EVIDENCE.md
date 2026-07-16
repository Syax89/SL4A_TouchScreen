# Raw Mode Evidence Contract

This document freezes the offline evidence and test boundary for raw-mode work.
It does not authorize a raw-mode hardware experiment.

`raw_capture_only=1` is the sole supported raw-related mode. It can preserve a
complete `0x0c` body that independently arrives, but it cannot send the feature
vectors below or cause the device to emit raw data. See `RAW_CAPTURE_ONLY.md`.

## Baseline

- Driver commit: `483520f51afb8687a5bcf4c350d2aaa5428c926b`
- Standard mode: `raw_mode=N`; wire-only descriptor boot verified.
- Recovery: reboot or the Limine touch-disabled recovery entry only. Never reload
  `spi_amd` live or invoke direct ACPI GPIO recovery.

## Immutable Inputs

| File | SHA-256 |
| --- | --- |
| `traces/surface_boot_auto.csv` | `2b8d965fb223bc7350ffae58f775ef820200c2b0c0ba3a6e7241511440452fb2` |
| `traces/surface_init.csv` | `ab312f0d481455334fff9017d888034441a30cb2ea6f3b93376b11a8e963677a` |
| `traces/surface_touch.csv` | `6733f1db5f54a9108ed2455e0607eb5244d0b0336b8df7725692df4c55d44e10` |
| `docs/HIDSPI_PROTOCOL.md` | `16cab8239bcbbcd241cf788d5e5069536f200c37cca5ad449d94ddb47faf94d7` |
| `docs/STANDARD_BOOT_EVIDENCE.md` | `9bbc1f8db05ccfe66ffa866a42efb85b6861992cc2c431c4cb8e4ad652320ed5` |
| `docs/NEXT_STEPS.md` | `e0cda9db50fdcec4a46bf09d85bd7b8a77dbc42c957b0794d77feade1500ec74` |

## Doubly Verified Facts

Two independent transport analyses agree on these captured TX vectors:

```
GET_FEATURE: 02 00 00 03 42 00 04 03 00 06
SET_FEATURE: 02 00 00 03 82 00 03 04 00 05 01 00 00 00
```

Two independent payload analyses agree that a full `0x0c` frame has:

- an aligned V0 body length of 4304 bytes;
- `total_len=4302` at body offsets 0-1;
- `content_id=0x0c` at body offset 2;
- exactly 4299 opaque content bytes beginning at body offset 3;
- two aligned tail bytes outside `total_len`, with unknown semantics.

The first two content bytes are preserved as opaque data. They must not be
interpreted as a scan counter or discarded without new independent evidence.

The captured report descriptor independently contains Report ID `0x0c` with
exactly 4299 report-data bytes and Report ID `0x08` with exactly 211 report-data
bytes. Those sizes exactly match the V0 content spans in the paired 4304-byte
and 216-byte DATA bodies. This proves descriptor/framing compatibility; it does
not assign a meaning to either report payload or prove a raw-mode transition.

For `0x0c`, the descriptor classifies the first two report-data bytes as a
16-bit Digitizers `Usage 0x56` Data field and the remaining 4297 bytes as a
Digitizers `Usage 0x61` Constant field. HID Constant classification is metadata,
not an explanation of the bytes; both spans remain uninterpreted here.

## Offline Contract

`spi_hid_protocol_parse_content()` is a pure bounded parser. It accepts only a
body with `3 <= total_len <= available bytes`, returns the V0 content ID and an
unmodified span beginning after the three-byte envelope, and has no hardware,
workqueue, HID-input, retry, or geometry behavior.

`tests/protocol_test.c` freezes the full captured GET/SET wire vectors and a
synthetic `0x0c` envelope fixture with the captured lengths and explicit aligned
tail. It is an oracle for future transport refactors, not permission to issue
the commands live.

## Explicitly Unresolved

- Feature selector semantics: traces contain fixed bytes, but selector naming
  and the historical 4-versus-5 observation conflict. No selector scan is allowed.
- Descriptor-to-GET and GET-to-SET timing: observed traces differ by lifecycle.
- Screen orientation, calibration, contact extraction thresholds, slot tracking,
  and the final grid-to-screen transform. The decoded CapImg raster itself is a
  48-row by 72-column single RLV run; the processor's 288-byte stride is an
  internal representation, not a competing wire layout.

No runtime raw feature template, geometry, coordinate behavior, or transition
may change until a new contract resolves the relevant uncertainty with two
independent analyses and fixture coverage.
