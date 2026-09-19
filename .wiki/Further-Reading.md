# Further Reading

The wiki is the curated technical reference; the repository's `docs/`
directory holds the evidence-grade material it is built on. Everything below
lives in the [SL4A_TouchScreen repository](https://github.com/Syax89/SL4A_TouchScreen).

## Reference documentation (repo `docs/`)

| Document | Purpose |
|---|---|
| [`HIDSPI_PROTOCOL.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/HIDSPI_PROTOCOL.md) | V0 protocol reference (basis for [Protocol](Protocol)) |
| [`SPI_REGISTERS.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/SPI_REGISTERS.md) | AMD FCH SPI + PCI register map (basis for [Hardware](Hardware)) |
| [`AMDI0060_CONTRACT.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/AMDI0060_CONTRACT.md) | SPI controller contract, quirks, speed tiers |
| [`CONFIG_TABLE.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/CONFIG_TABLE.md) | Pipeline constants and their provenance (basis for [Config Table](Config-Table)) |
| [`PARAMETERS.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/PARAMETERS.md) | Module parameter contract and defaults |
| [`PIPELINE.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/PIPELINE.md) | Raw pipeline overview (basis for [Touch Pipeline](Pipeline)) |
| [`ACTIVATION.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/ACTIVATION.md) | Raw-mode activation sequence |
| [`SUPPORT.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/SUPPORT.md) | Supported hardware and profiles contract |
| [`COMPATIBILITY.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/COMPATIBILITY.md) | Release-qualified hardware matrix (empty until rows are earned) |
| [`TESTING.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/TESTING.md) | Test procedure: host suite, hardware matrix, fault scope |
| [`HARDWARE_VALIDATION.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/HARDWARE_VALIDATION.md) | Hardware validation runbook and evidence template |
| [`ROLLBACK.md`](https://github.com/Syax89/SL4A_TouchScreen/blob/main/docs/ROLLBACK.md) | Uninstall / rollback / MOK enrollment steps |

## Supporting material

| Path | Purpose |
|---|---|
| `docs/acpi/` | ACPI tables: `dsdt.dsl`, `ssdt*.dsl` |
| `docs/` (other files) | The deeper design notes and the full investigation record |

## Tests (host, no kernel needed)

`make -C tests test` and `make -C tests SANITIZE=1 test` run the full host
suite: protocol policy, capimg decoder (both 3456- and 4056-cell frames),
c590/atan2 math, raw-pipeline math, and the replay test that pushes
synthetic 1–5-finger heatmap fixtures through the real pipeline code.
