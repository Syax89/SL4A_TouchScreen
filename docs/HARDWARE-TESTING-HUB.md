# Hardware Testing Hub

The project has working hardware results, but independent machines are needed
to distinguish device-specific behavior from broadly reliable behavior.

## Read-only preflight

Before installing anything:

```bash
grep -E 'MSHW0230|MSHW0231|MSHW0162|AMDI0060' /sys/bus/acpi/devices/*/hid 2>/dev/null
./tools/sl4a-touch.sh install --check
```

Report the exact Surface model, ACPI IDs, distro, kernel, Secure Boot state and
the exact driver commit tested.

## Useful test matrix

At minimum, record cold boot and suspend/resume. For input behavior, the most
useful observations are one finger, two fingers far apart, close approach and
separation, pinch, crossing/identity retention, rapid lift/re-contact, 3–5
fingers, edges/corners and stylus coexistence when available.

Failures are as useful as successes.

## Evidence capture

The evidence helpers are read-only and dynamically discover input nodes rather
than assuming an `eventN` number.

For the driver-owned MSHW0231 heatmap multitouch device:

```bash
./tools/hardware_evidence/capture_beta_multitouch.sh --duration 20 \
  --output /tmp/sl4a-beta-mt.evtest.txt
```

For a complete bounded session:

```bash
./tools/hardware_evidence/capture_linux_trace_bundle.sh --duration 20 \
  --capture-direct-touch --capture-beta-multitouch --capture-stylus \
  --output /tmp/sl4a-session
```

The beta capture helper intentionally requires the exact
`MSHW0231 Touchscreen` device identity plus multitouch capability so a test
cannot silently record the standard HID child instead.

See [TESTERS-WANTED.md](TESTERS-WANTED.md) and
[../tools/hardware_evidence/README.md](../tools/hardware_evidence/README.md).
