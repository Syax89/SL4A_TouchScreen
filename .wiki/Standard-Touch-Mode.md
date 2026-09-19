# Standard Touch Mode (stable, default)

The default mode (`raw_mode=0`) and the one that is stable for daily use:
**single-touch + pen**, with coordinates computed by the device's own
firmware.

## The key insight: don't ask for raw mode

The device has two output modes:

| Mode | How it is entered | What it sends | Coordinates |
|---|---|---|---|
| **Standard HID** (default) | simply don't send GET_FEATURE/SET_FEATURE | Report ID `0x40` (touch), `0x01` (pen) | Pre-computed by firmware |
| **Raw heatmap** | `GET_FEATURE` then `SET_FEATURE(id=5, val=1)` | Report ID `0x0C` frames (~4302 bytes of sensor data) | None — raw data, needs host blob detection |

The device firmware is fully capable of producing ready-to-use single-touch
coordinates in the default mode — no one has to ask for it. The driver in its
default configuration therefore **never sends GET_FEATURE/SET_FEATURE at all**:
it reaches sequencer state `4` (`DONE`) right after the report-descriptor
exchange (see [Architecture](Architecture)), and the device streams Report ID
`0x40`/`0x01` with real coordinates at roughly 10 ms intervals (a field
observation) — no calibration,
no signal processing on the Linux side.

## Report ID 0x40 — TouchScreen

| Field | Size | Notes |
|---|---|---|
| Report ID | 1 byte | `0x40` |
| TipSwitch | 1 byte | touch down/up |
| X | 16-bit LE | 0–32767 logical range |
| Y | 16-bit LE | 0–32767 logical range |

6 bytes total. Forwarded unchanged to the HID input subsystem via
`hid_input_report()`; `hid-generic` and the standard `hid-input` quirk
handling take it from there — no custom processing. The transport length
bookkeeping is: a standard report is admitted when `rl - 3 <= avail`
(`avail = rblen - 8`) and handed to `hid_input_report()` as `rl - 2` bytes.

## Report ID 0x01 — Pen/Stylus

Standard Digitizer-Pen collection fields: InRange, TipSwitch, BarrelSwitch,
Invert, Eraser, X, Y, TipPressure. Also forwarded directly via
`hid_input_report()`. The pen node is published but **unqualified**: pen
behavior has not been validated on hardware.

## What this mode does not give you

Multi-touch (2+ simultaneous fingers) is **not** available here — the
firmware's coordinate path tracks a single contact. Real multi-touch needs
raw mode and host-side blob detection: see
[Multi-touch (Beta)](Multi-touch-Experimental).

## Connect-time feature queries

A userspace feature query (hidraw `HIDIOCGFEATURE`) can take up to ~3.6 s while
the device settles before it answers. Feature-query timeouts are **non-fatal**
and `sync_timeout_ms` (default 6000) bounds them, so standard mode is robust to
such queries — see [Protocol](Protocol) and [Architecture](Architecture).
