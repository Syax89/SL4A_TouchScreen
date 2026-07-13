# Boot Diagnostics

Use this only while investigating a boot freeze. The trace is opt-in and does
not alter the SPI protocol or power sequence.

Create `/etc/modprobe.d/sl4a-touch-debug.conf` with:

```conf
options spi_amd debug_trace=2
options spi_hid raw_mode=0 debug_level=2 acpi_probe_power_cycle=1
```

`debug_trace=1` records controller setup and lifecycle. `debug_trace=2` also
records every SPI message and segment. `debug_level=1` records HID state
transitions and ACPI phases. `debug_level=2` also records every synchronous
sequencer read and write. Level 3 includes short raw buffers and can produce a
large journal.

After a freeze and forced reboot, collect the previous boot without reloading
either module:

```bash
journalctl -b -1 -k -o short-monotonic | grep -E 'TRACE\[(amd|hid)'
journalctl -b -1 -k -o short-monotonic | grep -E 'spi-amd: TRACE busy timeout|spi_hid|spi_amd'
```

The last `TRACE[...]` record identifies the operation that did not return.
Leave `raw_mode=0` while investigating controller or boot stability.
