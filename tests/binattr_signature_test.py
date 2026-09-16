#!/usr/bin/env python3
"""Compile-check the bin_attribute callback signature guard.

struct bin_attribute::read() took a non-const attribute until 6.13, when the
const read_new() variant appeared and __BIN_ATTR()'s _Generic picked between the
two; 6.16 dropped the non-const callback and the kernel builds with
-Werror=incompatible-pointer-types, so a handler written for the old signature
fails the DKMS build on current Arch/CachyOS/Fedora kernels. The CI kernel-build
job uses Ubuntu 24.04's 6.8 headers and cannot see that, so the guard is checked
here against a stub of each kernel shape, and the revert that motivated the
guard is checked to actually fail.

Skips (exit 0) when no C compiler is installed, like the other host tests do.

Run: python3 tests/binattr_signature_test.py
"""
import pathlib
import re
import shutil
import subprocess
import tempfile

REPO = pathlib.Path(__file__).resolve().parents[1]
src = (REPO / "driver" / "spi-hid-core.c").read_text()

START = "#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 13, 0)"
END = "static BIN_ATTR_RO(heatmap_raw, 0);"
block = src[src.index(START):src.index(END, src.index(START)) + len(END)]

COMMON = r"""
#include <stddef.h>
#include <string.h>
#include <errno.h>
typedef long ssize_t;
typedef long long loff_t;
typedef unsigned char u8;
typedef unsigned int u32;
struct attribute { const char *name; };
struct kobject { int x; };
struct file { int x; };
struct device { int x; };
struct mutex { int x; };
struct spi_hid { u8 *heatmap_buf; u32 heatmap_len; struct mutex seq_lock; };
#define KERNEL_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))
#define min_t(type, a, b) ((type)(a) < (type)(b) ? (type)(a) : (type)(b))
static struct spi_hid *dev_get_drvdata(struct device *d) { (void)d; return 0; }
static struct device *kobj_to_dev(struct kobject *k) { (void)k; return 0; }
static void mutex_lock(struct mutex *m) { (void)m; }
static void mutex_unlock(struct mutex *m) { (void)m; }
"""

# The three shapes the guard has to survive, each mirroring the real header.
SHAPES = {
    "6.8": ("6, 8, 0", r"""
struct bin_attribute {
	struct attribute attr;
	ssize_t (*read)(struct file *, struct kobject *, struct bin_attribute *, char *, loff_t, size_t);
};
#define BIN_ATTR_RO(name, size) struct bin_attribute bin_attr_##name = { .read = name##_read }
"""),
    "6.13": ("6, 13, 0", r"""
struct bin_attribute;	/* declared before the typedef, as in the real header */
typedef ssize_t __sysfs_bin_rw_handler_new(struct file *, struct kobject *,
					   const struct bin_attribute *, char *, loff_t, size_t);
struct bin_attribute {
	struct attribute attr;
	ssize_t (*read)(struct file *, struct kobject *, struct bin_attribute *, char *, loff_t, size_t);
	ssize_t (*read_new)(struct file *, struct kobject *, const struct bin_attribute *, char *, loff_t, size_t);
};
#define BIN_ATTR_RO(name, size) struct bin_attribute bin_attr_##name = {	\
	.read = _Generic(name##_read, __sysfs_bin_rw_handler_new * : NULL, default : name##_read),	\
	.read_new = _Generic(name##_read, __sysfs_bin_rw_handler_new * : name##_read, default : NULL) }
"""),
    "6.16": ("6, 16, 0", r"""
struct bin_attribute {
	struct attribute attr;
	ssize_t (*read)(struct file *, struct kobject *, const struct bin_attribute *, char *, loff_t, size_t);
};
#define BIN_ATTR_RO(name, size) struct bin_attribute bin_attr_##name = { .read = name##_read }
"""),
}


def compiles(kernel: str, source_block: str) -> bool:
    version, shape = SHAPES[kernel]
    tu = (f"#define LINUX_VERSION_CODE KERNEL_VERSION({version})\n" + COMMON + shape
          + source_block + "\nint main(void) { return 0; }\n")
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as fh:
        fh.write(tu)
        path = fh.name
    try:
        # -Werror=incompatible-pointer-types is what the kernel builds with.
        proc = subprocess.run(["cc", "-fsyntax-only", "-Werror=incompatible-pointer-types", path],
                              capture_output=True, text=True)
        return proc.returncode == 0
    finally:
        pathlib.Path(path).unlink()


if not shutil.which("cc"):
    print("SKIP: no C compiler on this host")
    raise SystemExit(0)

for kernel in SHAPES:
    assert compiles(kernel, block), f"the handler does not compile against the {kernel} shape"

reverted = re.sub(r"#define SPI_HID_BIN_ATTR_PTR const struct bin_attribute \*attr",
                  "#define SPI_HID_BIN_ATTR_PTR struct bin_attribute *attr", block)
assert reverted != block, "guard macro not found — did the #if branch change?"
assert compiles("6.8", reverted) and compiles("6.13", reverted), \
    "the non-const signature should be fine before 6.16"
assert not compiles("6.16", reverted), \
    "the non-const signature must fail against 6.16 — the guard exists for that"

print("bin_attribute signature guard: PASS (compiles on 6.8/6.13/6.16, revert fails on 6.16)")
