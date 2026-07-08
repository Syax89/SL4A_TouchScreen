# Windows capture kit — "what puts the touchscreen into a receptive state?"

## Hypothesis to test
The MSHW0231 touchscreen on Linux receives a byte-perfect DESCREQ but doesn't respond and
stays in a perpetual reset loop; on Windows it responds on the first try. The SPI transaction
itself has been replicated and ruled out as the cause. Suspicion: **some Windows component
other than `hidspi.sys`** (an ACPI power method, Surface Aggregator/SAM via the Surface
Serial Hub, power/PnP management) puts the device into a receptive state before/during init.
The traces we have (`traces/*.csv`) only capture SPB + GPIO-interrupt + HIDCLASS → they
CANNOT show ACPI/SAM/power activity.

## What this kit captures (that the old traces did NOT)
- **ACPI** (power methods, _PS0/_INI/_RST, the 2 `AcpiEventMethod`s seen before touch init)
- **Kernel-Power** (the device's Dx transitions: who brings the touch to D0, and when)
- **Kernel-PnP** (device start order: what starts before the touch)
- **Surface/SAM** (Surface Aggregator requests: whether Windows sends a SAM command for the
  touch)

The touch initializes at COLD BOOT (a disable/enable in Device Manager isn't enough — that
only triggers a D2→D0 resume). So a **boot trace** is used.

## Steps (run on Windows, in an Administrator prompt)

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

`providers.txt` revealed the real architecture of touch on Windows and confirms the
suspicion that "some non-hidspi component enables the touch". The `touch_boot.wprp` profile
now uses this machine's REAL GUIDs. What to look for in the log:

- **`ACPI\MSHW0231\A` = "Surface Digitizer HidSpi Extn Package"** — this is NOT plain
  hidspi.sys: there's a **vendor extension package** for the digitizer. It's the prime
  candidate for whatever special init we're missing.
  (Later determined to be a registry-only cosmetic/power-management overlay, not a
  separate binary — see `docs/GROUND_TRUTH.md` §15.13/§15.14.)
- The touch is a multi-collection HID device: **COL01 "Touch Communications"** (a CONTROL
  channel, distinct from touch data), COL06 "Touch Screen Device", + pen/digitizer/VHF enum.
- **Surface Serial Hub (MSHW0084)** + **SMF Core/Client** + **SMF Display Client**: the SAM
  stack that might power/enable the panel before the touch.
- **"ACPI Driver Trace Provider"** (GUID DAB01D4D...) traces ACPI method execution — i.e. it
  should finally tell us WHAT the 2 `AcpiEventMethod`s seen before touch init actually are.

The profile captures all of this plus SPB/GPIO/HIDCLASS on the same timeline, so I can
correlate "SAM request / ACPI method → the touch's first RESET_RSP".
