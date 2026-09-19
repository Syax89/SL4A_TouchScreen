# Multi-touch (Beta — `raw_mode=1`)

The raw mode streams the sensor grid to the host, where the driver runs the
blob-detection pipeline from [Touch Pipeline](Pipeline) to produce multitouch
contacts.

> **Beta feature.** Raw mode is functional but younger than the standard path.
> It can fail to activate on a cold boot and can leave the controller silent;
> a reboot is the supported recovery. The standard profile (`raw_mode=0`) stays
> the qualified one.

## Activation sequence

Raw mode performs a vendor feature exchange before the device starts
streaming (details and exact frames: [Protocol](Protocol)):

1. **Vendor init** — command register, content ID `0xC2`
2. **GET_FEATURE** — report ID 6 (the configuration read)
3. **SET_FEATURE** — report ID `0x05`, value `0x01`
4. Device streams `content_id=0x0C` **CapImg frames** (~4302–4304 bytes)

After SET_FEATURE the driver confirms the handshake on the first raw data frame
(IRQ path or the poll fallback), keeping the handshake watchdog armed until
then. Reaching `DONE` also arms a 20 ms poller alongside the edge-triggered IRQ,
so a lost IRQ edge during the SET_FEATURE write does not stall the stream.

## Per-device geometry

The CapImg raster is device-specific and validated against the
probe-selected count — frames carrying a different count are rejected:

| Device | ACPI ID | Grid | Raster samples | Baseline frames | Baseline EMA alpha |
|---|---|---|---|---|---|
| Surface Laptop 4 AMD | `MSHW0231` | 72×48 | 3456 | 30 | 7 |
| Surface Laptop 3 AMD | `MSHW0162` | 78×52 | 4056 | 33 | 7 |

## What the frames contain

The `0x0C` frame body is a per-cell byte raster: one byte per sensor cell,
row-major, row stride equal to the grid width (72 or 78). Each byte is an index
into the c590 signal lookup table, so the pipeline converts each cell to a
fixed-point signal before it works on it. On top of that signal the driver
estimates a per-cell baseline, candidate regions, centroids and orientation.
It does not provide calibrated multitouch coordinates: gain adaptation, blob
separation and per-device calibration are incomplete.

## Track lifecycle (reference model)

The reference model publishes **one peak per frame** (see
[Touch Pipeline](Pipeline)); a tracker turns that sequence of peaks into
contacts:

| Step | Rule |
|---|---|
| Handover | A peak within **6 cells** of a track's prediction closes that track and releases its slot ids; the peak opens a new track (the usual case while a contact moves) |
| Free survival | A track further than 6 cells from the peak keeps its position and survives while **its own cell** still reads `c590 ≥ 0.04` (400 fixed point) |
| Release | A track that fails the survival test is dropped, its ids return to the pool, and the active-contact count is decremented |

The survival test is **absolute**: it is evaluated on that contact's own cell,
never against the frame's strongest blob. A weak third contact therefore keeps
being reported next to two strong ones. The Linux chain approximates the same
behaviour through its merge and assignment stages (§7 and §9 of
[Touch Pipeline](Pipeline)) with the ghost-merge radii and association radii
listed in [Config Table](Config-Table).

## Reliability mechanisms

| Mechanism | Behavior |
|---|---|
| Handshake watchdog | If no data frame confirms the handshake within 2000 ms, re-discovery up to 3 times (`RAW_HANDSHAKE_MAX_RETRIES`) |
| Stream watchdog | `stream_watchdog_ms` (default **2000**; 0 disables) — after 3 silent intervals the raw pipeline re-initializes, up to `stream_watchdog_max_retries` (3) |
| Cold-boot retry | The discovery path retries with backoff; the retry is gated so it cannot tear down an already-working stream |

## Known limitations

- **4+ fingers**: tracking degrades — no contact classifier and no per-cycle
  gain adaptation (both need device firmware access).
- **Activation after cold boot**: not guaranteed; if the touchscreen is
  silent after reboot, run `sudo ./tools/sl4a-touch.sh status` and check
  dmesg, then re-activate or return to standard mode.
- **Operational safety**: never unload/reload/bind/unbind `sl4a-spi-amd` on
  a live system; always install through DKMS and reboot to change modes.

## Testing on real hardware

Host regression coverage: `raw_pipeline_replay_test` replays synthetic
1–5-finger heatmap fixtures through the **real** pipeline code, and the capimg
decoder test covers both the 3456- and 4056-cell frame shapes.

See [Pipeline](Pipeline) for the algorithm and [Config Table](Config-Table)
for the pipeline constants.
