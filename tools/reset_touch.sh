#!/bin/bash
# ============================================================================
# reset_touch.sh — Power-cycle the MSHW0231 touchscreen via its known ACPI
# GPIO method (\M010), without touching spi-amd/spi-hid or doing a full
# system reboot.
#
# Why this exists: raw_mode=1 (GET_FEATURE/SET_FEATURE raw heatmap mode)
# connects reliably only sometimes; repeated insmod/rmmod cycles of the
# driver do NOT power-cycle the physical device, so a bad state from a
# previous attempt may persist across reloads. This script asks ACPI to
# power the device off and back on directly (same M010 0x5B/0x0103 calls
# used in test.sh/test_raw.sh's inline gpio_cycle module), independent of
# whether spi-amd/spi-hid are currently loaded.
#
# Usage: sudo ./reset_touch.sh   (or just ./reset_touch.sh — it sudo's itself)
# ============================================================================
set -e

if [ "$EUID" -ne 0 ]; then
    exec sudo "$0" "$@"
fi

T="$(mktemp -d /tmp/reset-touch-XXXXXX)"
trap 'rm -rf "$T"' EXIT

cat > "$T/gpio_reset.c" << 'EOF'
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/delay.h>

static void m010_call(u64 addr, u64 val)
{
	acpi_handle h;
	union acpi_object args[2];
	struct acpi_object_list arg_list;

	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\M010", &h))) {
		pr_err("gpio_reset: M010 not found\n");
		return;
	}
	args[0].type = ACPI_TYPE_INTEGER; args[0].integer.value = addr;
	args[1].type = ACPI_TYPE_INTEGER; args[1].integer.value = val;
	arg_list.count = 2; arg_list.pointer = args;
	acpi_evaluate_object(h, NULL, &arg_list, NULL);
}

static int __init gpio_reset_init(void)
{
	pr_info("gpio_reset: POWER OFF (M010:0x5B=0)\n");
	m010_call(0x5B, 0);
	msleep(200);

	pr_info("gpio_reset: RESET ASSERT (M010:0x0103=0)\n");
	m010_call(0x0103, 0);
	msleep(100);

	pr_info("gpio_reset: POWER ON (M010:0x5B=1)\n");
	m010_call(0x5B, 1);
	msleep(50);

	pr_info("gpio_reset: RESET DEASSERT (M010:0x0103=1)\n");
	m010_call(0x0103, 1);
	msleep(200);

	pr_info("gpio_reset: done — touchscreen power-cycled\n");
	return -EIO; /* unload immediately, this is a one-shot action */
}
module_init(gpio_reset_init);
MODULE_LICENSE("GPL");
EOF

echo 'obj-m += gpio_reset.o' > "$T/Makefile"

echo "Building..."
make -C "/lib/modules/$(uname -r)/build" M="$T" LLVM=1 modules 2>&1 | grep -E "error:" && exit 1

echo "Power-cycling touchscreen via ACPI M010..."
dmesg -C
insmod "$T/gpio_reset.ko" 2>/dev/null || true
sleep 1

OUT=$(dmesg | grep gpio_reset)
echo "$OUT"

if echo "$OUT" | grep -q "not found"; then
    echo "FAIL: ACPI \\M010 method not available on this kernel/BIOS"
    exit 1
fi
if echo "$OUT" | grep -q "done"; then
    echo "OK: touchscreen power-cycled"
else
    echo "WARN: power cycle attempted but completion line not seen — check dmesg"
fi
