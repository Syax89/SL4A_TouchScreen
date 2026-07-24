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

The protocol, CapImg decoder, replay-fixture, and raw-capture-export tests are
required host coverage. `replay_fixture_test.py` has no success-by-skip path:
it verifies the eight tracked V0 bodies, deterministic malformed inputs, and
the recorded lifecycle-evidence classifications.

Six research checks (raw_transition_safety_test, isolated_set_safety_test,
post_set_timeline_test, surface_tracker_oracle_test, raw_corpus_test, and
v0_capimg_decoder_test) call `skip_optional_contract()` when their untracked
corpus, analyzer, or harness is absent. They are kept on disk for future
implementation but are excluded from the CI gate. Their empty or unused
Makefile targets (raw_corpus_test, raw_capture_export_test,
v0_capimg_decoder_test) were also removed.

## CI

GitHub Actions checks tracked whitespace, runs the normal and ASan/UBSan host
gates, and compiles the modules out-of-tree against Ubuntu generic headers. The
kernel job is a compile smoke test only: it does not load modules or qualify a
kernel, DKMS lifecycle, Secure Boot, or Surface hardware behavior.

## Hardware Evidence Helpers

Validate the shell syntax without collecting host evidence:

```sh
bash -n tools/hardware_evidence/collect.sh
bash -n tools/hardware_evidence/capture_direct_touch.sh
bash -n tools/hardware_evidence/capture_linux_trace_bundle.sh
bash -n tools/hardware_evidence/run_blinded_session.sh
```

On target hardware, use a new output path and a short bounded session. This is
read-only, but journal and input permissions may require the caller to invoke
the helper with `sudo`; it never escalates itself:

```sh
sudo ./tools/hardware_evidence/capture_linux_trace_bundle.sh --duration 20 \
  --capture-direct-touch --output evidence/<run-id>/p1-touch-session
```

Check `manifest.txt` for the UTC bounds, journal exit status, and checksums.
Retain the complete listed bundle, including a permission-error journal artifact
when journal access was unavailable.

## DKMS Build

On a supported Linux target with matching kernel headers:

```sh
sudo ./tools/sl4a-touch.sh install --standard
dkms status
modinfo sl4a-spi-amd
modinfo sl4a-spi-hid
# The alias queries must print no output.
modinfo -F alias sl4a-spi-amd
modinfo -F alias sl4a-spi-hid
```

For a direct kernel build against a Clang-built kernel, add `LLVM=1`.

Record the exact kernel, compiler, DKMS version, distribution, Secure Boot
state, and build output in `COMPATIBILITY.md`.

## Target Hardware Matrix

Run each case only after login and after retaining a local console or remote
shell for recovery. The install step must not bind the experimental controller.
Activate it explicitly with `sudo ./tools/sl4a-touch.sh activate`; it refuses to
displace existing AMDI0060 or MSHW0231 drivers and verifies both bindings.
Recover with `sudo modprobe -r sl4a-spi-hid sl4a-spi-amd` followed by a reboot.

The installer selects the standard profile by default. Since `raw_mode` is read
only after module load, install the selected profile, then reboot before
testing:

```sh
# Standard profile
sudo ./tools/sl4a-touch.sh install --standard

# Raw experimental profile
sudo ./tools/sl4a-touch.sh install --raw
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
