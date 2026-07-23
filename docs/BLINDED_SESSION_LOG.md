# Blinded Hardware Session Log

This is an opaque-session log, not a compatibility qualification. Profile
mapping remains outside this file until assessment is frozen and unblinded.

## Artifact Storage

Only `cold-boot-state` survives in durable storage (`evidence/`). The remaining
artifacts below were lost when `/tmp` was cleared on reboot. A new collection
with durable output paths is required before any E1 claim.

## Surviving

### Cold boot — `evidence/cold-boot-state`

Full collection with checksums archived.

## Lost (ephemeral /tmp, cleared on reboot)

## p1 Direct Touch

- Case: `direct-touch`
- Requested duration: 30 seconds
- Artifact: `/tmp/sl4a-blind-p1-direct-touch`
- Integrity: `checksums.txt` verified.
- Controller result: `ready`, lifecycle active, `frames_dropped=0`.
- Input outcome: not observed. The bounded trace reached its timeout without
  recording input events, so it is not scored as a touch pass.

## p1 Stress

- Case: `stress-30m`
- Requested duration: 1800 seconds
- Artifact: `/tmp/sl4a-blind-p1-stress-30m`
- Integrity: `checksums.txt` verified.
- Input outcome: direct-touch trace contains `BTN_TOUCH` press/release and
  absolute X/Y events.
- Start counters: `data=4406`, `irq_count=4414`, `frames_dropped=0`.
- End counters: `data=7932`, `irq_count=7940`, `frames_dropped=0`.
- Lifecycle after stress: ready, sequencer enabled, IRQ requested and enabled.
- Kernel outcome: no SL4A warning, Oops, panic, or call trace was found in the
  bounded stress trace.

## Resume Observation

- Pre/post artifacts: `/tmp/sl4a-pre-suspend`, `/tmp/sl4a-post-resume`
- The touchscreen returned ready; reset, device descriptor, and report
  descriptor counters advanced from `2/1/1` to `4/2/2` with zero drops.
- The system journal emitted warnings in `surface_aggregator` and
  `surface_charger` during resume. They are platform deviations, not attributed
  to SL4A, and block release qualification until independently resolved.

## p1 Stylus

- Case: `stylus`
- Requested duration: 30 seconds
- Artifact: `/tmp/sl4a-blind-p1-stylus`
- Integrity: `checksums.txt` verified.
- Device discovery: the HID `045E:0C19` stylus node exposed pen, touch,
  position, pressure, tilt, and direct-input capabilities.
- Input outcome: not observed. No stylus events were recorded during the bounded
  window, so capability discovery is not scored as a functional pen pass.

## Cold Boot

- DKMS `sl4a-touch/1.2.0` installed.
- After full power-off, unplug AC 30s, then boot: no SL4A module loaded, zero
  SL4A/SPI/HID kernel messages during boot.
- Post-login activation: controller and HID transport bound successfully.
- Descriptor negotiation: `reset_rsp=2`, `device_desc=1`, `rpt_desc=1`, `936
  bytes read from wire`, `ready`, all eight input nodes published.
- Counters: `data=0`, `irq_count=4`, `frames_dropped=0`.
- Collection artifact: `/tmp/sl4a-cold-boot-state`.

## Warm Boot

- Period label: same as ongoing opaque session.
- DKMS `sl4a-touch/1.2.0` installed for kernel `7.1.3-2-cachyos`.
- After reboot: no SL4A module loaded, no SPI device bound, zero SL4A/SPI/HID
  kernel messages.
- Post-login activation: `activate-fch.sh` bound sl4a_spi_amd and sl4a_spi_hid.
- Journal: reset/descriptor/report descriptor negotiation completed; `936 bytes
  read from wire`; all eight input nodes published.
- Collection artifact: `/tmp/sl4a-warm-boot-state`.
