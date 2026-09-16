#!/usr/bin/env python3
"""Pin the diagnostic bundle's frame contract.

The bundle must be enough to analyse a problem on its own, so it carries the
last captured frame as raw bytes. It cannot come from heatmap_debug: a sysfs
show() attribute is limited to one page and the 4304-byte V0 body does not fit,
so the frame's tail — where the per-frame lists live — was cut. The driver
exposes the same buffer as the heatmap_raw binary attribute (streamed, complete)
and the bundle falls back to the truncated hex view on older modules.

A regression here is silent: the bundle still gets written, it just stops
containing the data an analysis needs.
"""

from pathlib import Path

root = Path(__file__).parents[1]
tool = (root / "tools" / "sl4a-touch.sh").read_text()
core = (root / "driver" / "spi-hid-core.c").read_text()

# ── tool: the frame section, inside the bundle, with its fallback ──
assert "--- Last captured frame ---" in tool
assert '[ -r "$spidev/heatmap_raw" ]' in tool
assert 'wc -c < "$spidev/heatmap_raw"' in tool
assert 'od -An -v -tx1 -w32 "$spidev/heatmap_raw"' in tool
assert "truncated to one page" in tool          # the fallback says what it is
assert 'cat "$spidev/heatmap_debug"' in tool     # ...and still collects something
assert tool.index("--- Last captured frame ---") < tool.index('} > "$OUT"'), \
    "the frame section must be inside the bundle redirect"

# ── tool: the rest of the driver's readable stats ──
for name in ("build_info", "ready", "bus_error_count", "device_initiated_reset_count"):
    assert name in tool, f"{name} missing from the bundle's sysfs stats"

# ── driver: the binary attribute exists, and is created and removed ──
assert "static ssize_t heatmap_raw_read(" in core
assert "static BIN_ATTR_RO(heatmap_raw, SPI_HID_RAW_CAPTURE_BODY_LENGTH);" in core
assert "sysfs_create_bin_file(&dev->kobj, &bin_attr_heatmap_raw)" in core
assert core.count("sysfs_remove_bin_file(&dev->kobj, &bin_attr_heatmap_raw)") == 2, \
    "both teardown paths (remove and the err1 probe failure) must drop it"

# ── driver: the read handler is complete and consistent with heatmap_debug ──
body = core[core.index("static ssize_t heatmap_raw_read("):]
body = body[:body.index("\n}\n")]
assert "mutex_lock(&shid->seq_lock)" in body and "mutex_unlock(&shid->seq_lock)" in body, \
    "the handler must hold the same lock heatmap_debug uses"
assert "off < shid->heatmap_len" in body, "a read past the frame must yield EOF (return 0)"
assert "memcpy(buf, shid->heatmap_buf + off, n)" in body

print("diagnostic bundle frame contract: PASS")
