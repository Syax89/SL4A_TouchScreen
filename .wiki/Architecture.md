# Architecture

The driver is split into two kernel modules with one responsibility each:
`sl4a-spi-amd.ko` owns the SPI controller hardware, `sl4a-spi-hid.ko` owns the
HID-over-SPI V0 protocol and the touch pipelines.

## Module split

```
sl4a-spi-hid.ko                        sl4a-spi-amd.ko
V0 protocol + HID integration          AMD FCH SPI V2 controller
─────────────────────                  ─────────────────────────
• spi_hid_ll_driver (HID LL)           • registers an SPI controller
• IRQ-driven sequencer                 • TX/RX FIFO transaction execution
• descriptor parsing (936-byte RD)     • bulk PIO segmentation (64-byte)
• report forwarding → hid_input        • TX_COUNT=3 read quirk
• raw heatmap pipeline (mshw0231-raw)  • speed/CS config (12 MHz, mode 0)
          \                                /
           └────────── SPI framework ──────┘
                          │
                 AMDI0060 MMIO controller
                          │
              MSHW0231 / MSHW0162 (SPI1, CS0)
```

Both modules are **opt-in**: the installer never loads them at install time, and
neither replaces the in-tree SPI/HID drivers while running. Loading
`sl4a-spi-amd` while the system is live can freeze it — always install and
reboot.

## Sequencer state machine

`spi-hid-core.c` runs a small IRQ-driven state machine that walks the device
through discovery and then forwards input reports. States (values shown by
`seq_state`):

| # | State | What happens |
|---:|---|---|
| 0 | `WAIT_RESET` | Consume the power-on `RESET_RSP`, then send `DESCREQ` |
| 1 | `WAIT_DESC` | Consume the device descriptor, send the second (report) descriptor request |
| 2 | `WAIT_RPT` | Consume and parse the 936-byte report descriptor |
| 3 | `VENDOR_INIT` | Defined for the reference's ordering; the driver never enters it |
| 4 | `DONE` | Forward input reports through `hid_input_report()` |
| 5 | `WAIT_FEATURE` | Await the SET_FEATURE response (raw mode, only when the connect-time feature exchange is not skipped) |

In standard mode the driver reaches `DONE` right after the descriptor exchange.
In raw mode it either proceeds straight to `DONE` after the vendor
initialization and SET_FEATURE it sends itself, or parks briefly in
`WAIT_FEATURE` when the connect-time feature query is awaited. State `3` is
declared to keep the numbering aligned with the reference's ordering and is
never assigned.

Transitions are logged with a reason tag (`seq_dbg`, visible in dmesg at
`sl4a_debug_level>=1`). Every transition is audited: `spi_hid_seq_set_state()`
records the previous state, the reason and a counter, which powers the
`lifecycle_status` sysfs attribute.

## Synchronous requests and timeouts

Some exchanges (feature queries) are synchronous. They are classified so a
timeout is handled correctly:

| Class | Examples | On timeout |
|---|---|---|
| `SPI_HID_SYNC_DESCRIPTOR` | DESCREQ, RPT_DESC | **Fatal** — the transport geometry is unknown, recovery is required |
| `SPI_HID_SYNC_FEATURE` | GET_FEATURE from userspace (hidraw `HIDIOCGFEATURE`) | **Non-fatal** — the IRQ-driven input stream is independent of feature queries |

The device answers feature queries only after it has settled (~3.6 s). The
`sync_timeout_ms` parameter (default **6000**) bounds every synchronous request,
so a feature-query timeout leaves the touchscreen running. The policy is
enforced by the shared inline `spi_hid_protocol_sync_timeout_fatal()` in
`spi-hid-protocol.h`, which the host test suite checks directly. Descriptor
geometry is learned through the DESCREQ sequencer path, so the fatal descriptor
class has no live caller in the current driver.

## IRQ model

Input is **IRQ-driven**: the touch controller asserts a data-ready GPIO
interrupt (edge-triggered, active-low, declared in ACPI `_CRS` as `GpioInt`).
The threaded handler (`spi_hid_seq_thread`) reads the pending frame from the
SPI FIFO and either feeds the HID stack (standard mode) or the raw pipeline
(raw mode).

The data-ready IRQ is edge-triggered, so an edge that fires while the driver is
inside the SET_FEATURE write path can be lost. In raw mode the driver therefore
arms a periodic poller (`poll_work`, 20 ms) alongside the IRQ path whenever it
reaches `DONE`; whichever path observes data first confirms the handshake and
the other becomes a no-op. The poller keeps running while the stream watchdog
is disabled, because there it is the only backstop; once a raw frame confirms
the handshake and the stream watchdog is enabled, the poller is retired.

## Recovery paths

The driver distinguishes three failure classes:

| Failure | Recovery |
|---|---|
| Descriptor-request timeout | ACPI power cycle `_PS3`→`_PS0`, then re-arm the sequencer to `WAIT_RESET` so the power-on `RESET_RSP` restarts discovery |
| Raw handshake never confirmed (raw mode) | `raw_handshake_watchdog` — up to `RAW_HANDSHAKE_MAX_RETRIES` (3) re-discovery attempts with `RAW_HANDSHAKE_TIMEOUT_MS` (2000) |
| Raw stream stalls (raw mode) | `stream_watchdog_ms` (default **2000**) — after 3 silent intervals, re-init up to `stream_watchdog_max_retries` (3) |

**Never `_RST`**: the ACPI `_RST` method physically destroys the device on this
hardware. Recovery always goes through `_PS3`→`_PS0`.

The raw handshake watchdog is armed on entry to every pre-`DONE` state while the
handshake is unconfirmed, so a device that never answers the DESCREQ (or answers
with `RESET_RSP`) is retried instead of parking silently.

## Per-device configuration

At probe, the ACPI match data selects a `spi_hid_dev_cfg` (see
[Hardware](Hardware)):

| Device | ACPI ID | Grid | CapImg samples | Baseline frames | Baseline EMA alpha |
|---|---|---|---|---|---|
| Surface Laptop 4 AMD | `MSHW0231` | 72×48 | 3456 | 30 | 7 |
| Surface Laptop 3 AMD | `MSHW0162` | 78×52 | 4056 | 33 | 7 |

The probe logs the active values as a `device config:` line in dmesg.
`grid_cols`/`grid_rows` module parameters override the config when set
explicitly.

## Sysfs interface

The driver exposes read-only diagnostics under the SPI device's sysfs node
(`/sys/devices/.../spi-MSHW0231:00/` or `spi-MSHW0162:00/`):

| Attribute | Type | Contents |
|---|---|---|
| `ready` | RO | Driver readiness state |
| `seq_state` | RO | Current sequencer state number (0–5, see table above) |
| `protocol_stats` | RO | Frame counters: reset responses, descriptors, data, feature responses, dropped frames, IRQs, wire patches, missed polls |
| `lifecycle_status` | RO | Lifecycle flags (removing, suspended, sequencer, IRQ, work items) |
| `baseline_status` | RO | Raw-mode baseline state (frames collected, grid rows/cols) |
| `bus_error_count` | RO | SPI bus error counter (and last error code) |
| `device_initiated_reset_count` | RO | Count of `RESET_RSP` frames the device sent |
| `heatmap_debug` | RO | Last captured raw frame's cell field |
| `build_info` | RO | Build/version stamp of the loaded module |
| `spi_hid_perf_mode` | RO | Performance-mode value (reported but not consumed) |

`sudo ./tools/sl4a-touch.sh status` and `logs` read these for you.

## Source map

| File | Responsibility |
|---|---|
| `driver/spi-hid-core.c` | V0 protocol, sequencer, HID LL driver, sync requests, sysfs |
| `driver/spi-hid-protocol.h` | Wire constants, header/content decoding, sync policy |
| `driver/spi-hid-capimg.c` | V0 CapImg body decoder (per-device sample count) |
| `driver/mshw0231-raw.c` | Raw heatmap pipeline: baseline, peaks, CCL, Hungarian, MT slots |
| `driver/mshw0231-raw-constants.h` | All pipeline constants |
| `driver/spi-amd.c` | AMD FCH SPI V2 controller driver (PIO) |
| `tools/sl4a-touch.sh` | Installer: install/uninstall/activate/status/logs/rebuild/hunt |

See [Protocol](Protocol) for the wire format and [Touch Pipeline](Pipeline) for
the raw processing chain.
