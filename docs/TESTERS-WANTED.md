# Testers Wanted

Independent Surface Laptop AMD results are particularly valuable.

## Before installing

Run:

```bash
grep -E 'MSHW0230|MSHW0231|MSHW0162|AMDI0060' /sys/bus/acpi/devices/*/hid 2>/dev/null
./tools/sl4a-touch.sh install --check
```

Keep recovery access available and read [SUPPORT.md](SUPPORT.md) and
[ROLLBACK.md](ROLLBACK.md) first.

## What to report

Please include the exact Surface model, ACPI IDs, distro, `uname -r`, Secure
Boot state and exact driver commit. Record cold boot and suspend/resume, then
describe one/two-finger behavior, close contacts, pinch/crossing, rapid
lift/re-contact, 3–5 finger behavior if comfortable, edge/corner contacts and
stylus behavior if available.

For failures, attach a bounded evidence bundle when possible. Do not include
private information you do not want published.

## Qualification scope

A result on one ACPI ID or firmware revision should not automatically be
generalized to another. In particular, experimental CapImg/multitouch behavior
must be reported with the exact hardware ID and revision tested.
