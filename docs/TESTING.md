# Validation Procedure

## Host Gate

Run from a clean checkout before every change:

```sh
git diff --check
make -C tests test
make -C tests clean
make -C tests SANITIZE=1 test
make -C tests clean
```

The protocol and CapImg decoder tests are required host coverage. Several
research checks currently print `SKIP` because their corpus or analyzer is not
tracked; they are not release evidence and will be split from the required gate
in Phase 5.

## DKMS Build

On a supported Linux target with matching kernel headers:

```sh
sudo ./tools/install.sh
dkms status
modinfo spi-amd
modinfo spi-hid
```

Record the exact kernel, compiler, DKMS version, distribution, Secure Boot
state, and build output in `COMPATIBILITY.md`.

## Target Hardware Matrix

Run each case with the standard profile first. Run raw mode only as a separate,
explicit test profile.

The installer selects the standard profile by default. Since `raw_mode` is read
only after module load, install the selected profile, then reboot before
testing:

```sh
# Standard profile
sudo ./tools/install.sh

# Raw experimental profile
sudo ./tools/install.sh --raw
```

Do not run the raw matrix unless the raw profile is recorded in the result.

| Case | Procedure | Expected evidence |
| --- | --- | --- |
| Cold boot | Fully power off, start, wait for the HID device. | `ready`, `protocol_stats`, dmesg excerpt. |
| Warm boot | Reboot without power removal. | Same as cold boot. |
| Reload | Unload/reload only when no input client uses the device. | No kernel warning, one controller/device binding. |
| Suspend/resume | Suspend for at least 30 seconds, resume, test input. | Lifecycle status before/after and dmesg. |
| Standard HID reports | Exercise pen, one touch, and release if reports are emitted. | evtest/libinput record; record absence as a result. |
| Raw contacts | Test one through five fingers. | Contact recording plus raw profile and frame counters. |
| Stress | Run mixed touch input for 30 minutes. | Start/end counters, error count, and dmesg. |
| Secure Boot | Install and reboot with Secure Boot enabled. | Signature/load result and MOK state. |

Useful sysfs attributes are exposed by the SPI HID device: `ready`,
`protocol_stats`, `baseline_status`, `lifecycle_status`, and `heatmap_debug`.
Their path is platform-assigned; discover it under `/sys/bus/spi/devices/`.

## Result Template

```text
Date:
Surface model:
BIOS/firmware:
Kernel and distribution:
Secure Boot:
Profile and module parameters:
Cases run:
Observed result:
Capture/log checksum and path:
Evidence level:
```
