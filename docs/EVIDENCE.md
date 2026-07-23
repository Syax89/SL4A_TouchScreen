# Evidence Ledger

This ledger is the authority for release behavior. A document or code comment
that disagrees with it is a discrepancy to resolve, not a reason to change the
wire protocol.

## Levels

| Level | Meaning |
| --- | --- |
| E1 | Reproducible Linux hardware result with machine, firmware, kernel, distribution, date, and procedure recorded. |
| E2 | Versioned ACPI table, Windows trace, or captured descriptor. |
| E3 | Tracked host test or replay fixture. |
| E4 | Static proof of memory safety, ownership, or locking. |
| E0 | Hypothesis or partial observation. It cannot set a release default. |

## Accepted Facts

| Claim | Level | Source |
| --- | --- | --- |
| The controller is `AMDI0060` at `0xFEC10000` with a `0x100` resource. | E2 | `docs/acpi/dsdt.dsl:1523-1538` |
| The child SPI resource uses logical CS 0, mode 0, 33.33 MHz, and GPIO 0x55. | E2 | `docs/acpi/dsdt.dsl:1541-1573` |
| The HID DSM GUID is `6e2ac436-0fcf-41af-a265-b32a220dcfab`, revision 1, function 1. | E2 | `docs/acpi/dsdt.dsl:1576-1601`, `driver/spi-hid-core.c` |
| The captured MSHW0231 device is identified as VID/PID `045e:0c19`. | E2 | `captures/wintrace/analisi_MSHW0231.md:3-7` |
| The host protocol and CapImg decoder have executable bounds coverage. | E3 | `tests/protocol_test.c`, `tests/capimg_decoder_host_test.c` |
| Eight retained V0 raw-slot bodies have checksum-validated decoder replay coverage, including deterministic malformed-input rejection. | E3 | `tests/fixtures/replay/v1/manifest.json`, `tests/replay_fixture_test.py` |

## Observations Not Yet Release Evidence

| Observation | Status | Source |
| --- | --- | --- |
| 484 valid 4304-byte raw captures were recorded. | Partial Linux observation; machine and kernel provenance are absent. | `captures/id5-20260718/raw_capture_status` |
| An isolated GET/SET Feature experiment wrote successfully but timed out. | Does not prove activation sufficiency. | `captures/id5-20260718/isolated_set_status` |
| The raw pipeline creates beta multitouch contacts. | Requires replay comparison and hardware gesture matrix. | `driver/spi-hid-core.c` |
| The current controller driver forces physical ALT_CS 1. | Implementation behavior; board mapping needs target-machine verification. | `driver/spi-amd.c` |

## Open Discrepancies

| Topic | Conflicting material | Required resolution |
| --- | --- | --- |
| Raw frame layout | `HIDSPI_PROTOCOL.md` and `ACTIVATION.md` describe 16-bit/6912-byte cells; the replay corpus covers eight 4304-byte CapImg bodies only. | Capture labelled reset, descriptor, and gap traffic before changing either implementation or release claim. |
| Raw activation | Documents imply ID5 alone activates raw mode; isolated capture times out after GET/SET. The replay metadata records this as unqualified and has no activation response bytes. | Record a full cold-boot activation trace and resulting stream. |
| Clock description | Older register docs mention 12 MHz; tracked ACPI requests 33.33 MHz. | Treat ACPI 33.33 MHz as the current target default; verify with an E1 controller trace before changing speed logic. |
| Physical chip select | Driver forces ALT_CS 1 while ACPI exposes logical CS 0. | Record an E1 target trace before treating this mapping as portable board evidence. |
| Raw profile | Historical v1.2.0 installer enabled raw although the module default was standard. | Current installer uses standard mode; `--raw` is explicit opt-in. |
| `raw_enabled` interface | Older activation documentation refers to it; no such module parameter exists. | Removed from release instructions. Use `ready`, `protocol_stats`, and `baseline_status` sysfs attributes instead. |

No E4 entry is currently release-qualified: the branch contains lifecycle and
buffer hardening, but its review record is not a versioned static-analysis
artifact. Add an auditable analysis result before assigning E4 to that work.

## Recording New Evidence

Add a dated row to `COMPATIBILITY.md` and a source entry here for every E1
result. Preserve raw captures only when they contain no personal data and record
their checksum, firmware, kernel, profile, procedure, and expected result.
