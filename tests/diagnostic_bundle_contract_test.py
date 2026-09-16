#!/usr/bin/env python3
"""Pin the diagnostic bundle's frame contract.

The bundle must be enough to analyse a problem on its own, so it carries the
last captured frame's cell field (one byte per cell, 3456 on MSHW0231 / 4056 on
MSHW0162) as hex. It cannot come from heatmap_debug: a sysfs show() attribute is
limited to one page and the 78x52 cell field does not fit, so the frame's tail
was cut. The driver exposes the same buffer as the heatmap_raw binary attribute
(streamed, complete) and the bundle falls back to the truncated hex view on
older modules.

This test is structural — the behaviour itself is run in
tests/diagnostic_bundle_frame_test.py, and the callback signature that makes the
attribute buildable on current kernels in tests/binattr_signature_test.py.
"""

from pathlib import Path

root = Path(__file__).parents[1]
tool = (root / "tools" / "sl4a-touch.sh").read_text()
core = (root / "driver" / "spi-hid-core.c").read_text()

# ── tool: the frame section, inside the bundle, with its fallbacks ──
assert "--- Last captured frame ---" in tool
assert '[ -r "$spidev/heatmap_raw" ]' in tool
assert 'cat "$spidev/heatmap_raw" > "$frame_tmp"' in tool     # one read, not two
assert 'od -An -v -tx1 -w32 "$frame_tmp"' in tool
assert "no frame data" in tool                  # empty attribute is not "0 bytes, complete"
assert "truncated to one page" in tool          # the fallback says what it is
assert 'cat "$spidev/heatmap_debug"' in tool    # ...and still collects something
assert tool.index("--- Last captured frame ---") < tool.index('} > "$OUT"'), \
    "the frame section must be inside the bundle redirect"

# ── tool: the rest of the driver's readable stats ──
for name in ("build_info", "ready", "bus_error_count", "device_initiated_reset_count"):
    assert name in tool, f"{name} missing from the bundle's sysfs stats"

# ── driver: the binary attribute exists, and is created and removed ──
assert "static ssize_t heatmap_raw_read(" in core
assert "static BIN_ATTR_RO(heatmap_raw, 0);" in core, \
    "size 0: the length is per-device, nothing honest to advertise"
assert "sysfs_create_bin_file(&dev->kobj, &bin_attr_heatmap_raw)" in core
assert core.count("sysfs_remove_bin_file(&dev->kobj, &bin_attr_heatmap_raw)") == 2, \
    "both teardown paths (remove and the err1 probe failure) must drop it"

# ── driver: the callback keeps compiling on kernels that changed its signature ──
assert "#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 13, 0)" in core
assert "#define SPI_HID_BIN_ATTR_PTR struct bin_attribute *attr" in core
assert "#define SPI_HID_BIN_ATTR_PTR const struct bin_attribute *attr" in core
assert "SPI_HID_BIN_ATTR_PTR, char *buf," in core
assert "#include <linux/version.h>" in core, \
    "the guard evaluates LINUX_VERSION_CODE: without linux/version.h it is 0/undefined " \
    "and the #else branch is taken on every kernel (-Wundef, then a hard " \
    "-Werror=incompatible-pointer-types build failure)"
assert core.index("#include <linux/version.h>") < core.index("LINUX_VERSION_CODE"), \
    "linux/version.h has to be included before the guard, not after it"

# ── driver: the read handler is complete and consistent with heatmap_debug ──
body = core[core.index("static ssize_t heatmap_raw_read("):]
body = body[:body.index("\n}\n")]
assert "mutex_lock(&shid->seq_lock)" in body and "mutex_unlock(&shid->seq_lock)" in body, \
    "the handler must hold the same lock heatmap_debug uses"
assert "off < shid->heatmap_len" in body, "a read past the frame must yield EOF (return 0)"
assert "memcpy(buf, shid->heatmap_buf + off, n)" in body

print("diagnostic bundle frame contract: PASS")
