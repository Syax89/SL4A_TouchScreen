# Windows capture kit — historical power-sequence research

## Purpose
Standard HID initialization now works on Linux. This kit remains useful for raw-mode
calibration and for comparing Windows ACPI, power, PnP, and Surface/SAM activity.
It does not establish that an external Windows component is required for standard
HID initialization. The existing SPI traces cover SPB, GPIO-interrupt, and HIDCLASS,
but not ACPI/SAM/power activity.

## What this kit captures (that the old traces did NOT)
- **ACPI** (power methods, _PS0/_INI/_RST, the 2 `AcpiEventMethod`s seen before touch init)
- **Kernel-Power** (the device's Dx transitions: who brings the touch to D0, and when)
- **Kernel-PnP** (device start order: what starts before the touch)
- **Surface/SAM** (Surface Aggregator requests: whether Windows sends a SAM command for the
  touch)

The touch initializes at COLD BOOT (a disable/enable in Device Manager isn't enough — that
only triggers a D2→D0 resume). So a **boot trace** is used.

## Steps (run on Windows, in an Administrator prompt)

### Runtime centroid capture for multi-touch mapping

The shared `touch_boot.wprp` profile also enables `Microsoft.Surface.TouchAndPen.Prod`
(`{3FA102E9-1A62-5490-7AF8-6088C2F9E6BE}`), the TraceLogging provider in
`TouchPenProcessor0C19.dll`. Its event schema contains the Windows-computed
`TouchBlobCoMX` and `TouchBlobCoMY` fields for each tracked finger.

For coordinate calibration, copy `touch_boot.wprp` and `capture_runtime.cmd` to the
Windows capture directory, run the latter as Administrator, and hold one finger at each
point of the indicated 3x3 screen grid. Repeat with two separated fingers. Stop the trace
and run `export_runtime.cmd`. Preserve the resulting ETL; it retains TraceLogging metadata and
is preferable to CSV. Use `tracerpt` or WPA on Windows to export the decoded fields, then copy
both the export and ETL back for correlation with physical grid coordinates. This is read-only and does not send
experimental HID commands to the controller.

### Capture the callback host

After using the touchscreen in Windows, run `collect_touch_host.cmd` as Administrator from
the same directory. It writes `touch_host/touch_host_modules.txt` with the process command
line and loaded modules for the process that contains `TouchPenProcessor0C19.dll`, and copies
its executable when Windows permits it. Copy the entire `touch_host/` directory back with the
ETL. This identifies the binary owning the indirect callback that supplies raw heatmap frames
to the processor DLL.

### 1. Find this machine's exact Surface/SAM providers
```
00_enumerate.cmd > providers.txt
```
Send me `providers.txt`. I'll look for the Surface/Aggregator/ACPI GUIDs and add them to the
profile if they're missing. (The profile already includes ACPI/Power/PnP by name.)

### 2. Start the boot trace, reboot, stop
```
wpr -boottrace -addboot touch_boot.wprp
shutdown /r /t 0
```
After rebooting and logging in, wait ~30 seconds (so the touch has finished initializing),
then:
```
wpr -boottrace -stopboot %SystemDrive%\touch_boot.etl
```

### 3. Export to text and send me the result
```
02_export.cmd
```
Produces `touch_boot.csv` (+ possibly `touch_boot.txt`). Send me that.

## What I'll look for in the log
- A SAM/Surface Serial Hub event around the touch's power-up (a specific TC/command).
- Which ACPI method the 2 `AcpiEventMethod`s are, and whether a _PS0/M009/M010 precedes the
  first RESET_RSP.
- PnP ordering: whether a companion/SAM device starts and enables something before the touch.
- D0/Dx transitions of the touch and any related devices.

## In parallel (optional, Linux side)
If your kernel has the `ssam_*` ftrace events, we can log what SAM does on Linux and compare.
They weren't compiled in on this boot; happy to check if you're interested.

---

## UPDATE after providers.txt (07/2026)

`providers.txt` revealed the Windows touch stack. The `touch_boot.wprp` profile
now uses this machine's real GUIDs. The extension-package hypothesis below was
later falsified: it is a registry-only overlay, not a separate initializer.
What to look for in the log:

- **`ACPI\MSHW0231\A` = "Surface Digitizer HidSpi Extn Package"** — a
  registry-only cosmetic/power-management overlay, not a separate binary or
  proven initializer (see `docs/GROUND_TRUTH.md` section 15.13/15.14).
- The touch is a multi-collection HID device: **COL01 "Touch Communications"** (a CONTROL
  channel, distinct from touch data), COL06 "Touch Screen Device", + pen/digitizer/VHF enum.
- **Surface Serial Hub (MSHW0084)** + **SMF Core/Client** + **SMF Display Client**: the SAM
  stack that might power/enable the panel before the touch.
- **"ACPI Driver Trace Provider"** (GUID DAB01D4D...) traces ACPI method execution — i.e. it
  should finally tell us WHAT the 2 `AcpiEventMethod`s seen before touch init actually are.

The profile captures all of this plus SPB/GPIO/HIDCLASS on the same timeline, so I can
correlate "SAM request / ACPI method → the touch's first RESET_RSP".
