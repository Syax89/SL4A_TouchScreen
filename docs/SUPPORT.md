# Supported Hardware And Profiles

## Supported Target

This repository is a DKMS driver for one hardware contract only:

| Component | Required identity | Evidence |
| --- | --- | --- |
| Product | Microsoft Surface Laptop 4 AMD (Cezanne) | E1 required for a release row |
| SPI controller | ACPI `AMDI0060`, MMIO `0xFEC10000`, length `0x100` | `docs/acpi/dsdt.dsl` |
| Touch controller | Runtime ACPI `MSHW0231` under `\_SB.SPI1` | Windows capture and target-machine check |
| SPI resource | Logical CS 0, mode 0, 33.33 MHz, GPIO 0x55 | `docs/acpi/dsdt.dsl` |
| HID descriptor | Microsoft VID `045e`, PID `0c19` | captured report descriptor |
| Transport | MSHW0231 HID-over-SPI V0 | captured protocol traffic |

The controller programs physical ALT_CS 1 for this board even though ACPI
declares logical CS 0. This mapping is controller-specific and must not be
generalized to another AMDI0060 system without hardware evidence.

The current module match tables are broader than this support contract. Those
aliases are implementation debt, not a compatibility promise. Do not install
this DKMS package on another Surface, another AMD SPI controller, or a generic
`PNP0C51` device. The installer verifies both required ACPI IDs; an explicit
`--force` is required to bypass that check.

## Profiles

| Profile | `raw_mode` | Intended use | Release status |
| --- | --- | --- | --- |
| Standard | `N` | HID transport and descriptor discovery | Default for the planned controlled release |
| Raw | `Y` | MSHW0231 CapImg capture and beta multitouch pipeline | Experimental; requires `sl4a-touch.sh install --raw` |

The module's compiled default and the installer default are standard mode.
`sl4a-touch.sh install --raw` is required to write the experimental raw-mode profile.

`raw_input_beta` controls publication of the decoded multitouch input device.
Raw captures alone do not establish release-quality contact behavior.

## Feature Status

| Feature | Status | Release claim |
| --- | --- | --- |
| Module build and host protocol tests | Automated | Yes, for listed host tests only |
| HID descriptor discovery | Implemented | Hardware matrix required |
| Standard HID report forwarding | Implemented | Contact behavior requires hardware evidence |
| Raw CapImg capture | Captures recorded | Experimental |
| Raw multitouch pipeline | Implemented | Experimental |
| Suspend/resume | Implemented | Hardware matrix required |
| Secure Boot | DKMS guidance only | Untested until recorded |

See `COMPATIBILITY.md` for release-qualified hardware results and `TESTING.md`
for the procedure required to add one.

## Explicitly Unsupported

- Surface Laptop 3, Surface Pro X, and any other `MSHW*` device.
- Other AMD SPI controller IDs, including `AMDI0061` and `AMDI0062`.
- Firmware, kernel, or distribution combinations not listed in
  `COMPATIBILITY.md`.
- Any claim that Feature ID 5 alone produces a reliable raw stream. Existing
  captures show it is a useful hypothesis, not sufficient release evidence.
