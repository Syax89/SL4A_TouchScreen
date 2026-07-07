/* ps3_ps0_cycle.c — force a REAL _PS3 (power off) -> _PS0 (power on) cycle
 * on \_SB.SPI1.HSPI, then read M009 before/after to confirm electrically.
 *
 * WHY: _INI (run once by ACPI before Linux's spi-hid probe) sets FLAG=0 at
 * the end. _PS0's power-on branch is guarded by "If (FLAG != Zero)" — so
 * calling _PS0 alone (e.g. via disable/enable in a driver's resume path) is
 * a NO-OP once FLAG is already 0, confirmed empirically on Windows (a real
 * capture showed _PS0 completing in 46us — too fast to contain its
 * Sleep(300ms) branch). Only after _PS3 runs (FLAG=3) does _PS0 execute the
 * real body: M010(0x5B,1) power-on, Sleep(300ms), M010(0x103,1) reset
 * deassert. This module forces that real cycle so we can observe whether a
 * genuine power-cycle (matching what Windows does on sleep/resume) changes
 * the device's behavior (still reset-looping vs. finally accepting writes),
 * tested by loading spi-amd+spi-hid AFTER this module, separately.
 *
 * SAFETY (self-review before running):
 *  - _PS3/_PS0 are official ACPI Device Power Management methods, defined
 *    directly in \_SB.SPI1.HSPI's own scope (docs/acpi/linux/dsdt.dsl:1606),
 *    not the odd cross-table M009 case — acpi_evaluate_object with the HSPI
 *    handle and a plain "_PS3"/"_PS0" name resolves normally.
 *  - Both bodies only ever call M010 (docs/acpi/linux/ssdt5.dsl:575), which
 *    for our small pin values (<0x800) resolves to a single 1-byte
 *    SystemMemory write (M012, ssdt5.dsl:811) — no SMN, no PCI config, no
 *    loops, no recursion. Windows performs the exact same calls routinely
 *    (confirmed via AmlMethodTrace capture) without harm.
 *  - No SPI driver (spi-amd/spi-hid) is loaded while this runs (checked
 *    manually before insmod) — avoids racing the ACPI GPIO toggle against
 *    concurrent SPI bus activity.
 *  - _PS3's real body only sleeps 1ms; _PS0's real body sleeps 300ms - both
 *    bounded, no unbounded waits.
 *
 * insmod ps3_ps0_cycle.ko && dmesg | grep "cyc:"
 */
#include <linux/module.h>
#include <linux/acpi.h>
#include <linux/delay.h>

static unsigned long long read_m009(u64 arg)
{
	struct acpi_object_list arg_list;
	union acpi_object obj;
	unsigned long long result = 0xdeadbeef;
	acpi_status st;

	obj.type = ACPI_TYPE_INTEGER;
	obj.integer.value = arg;
	arg_list.count = 1;
	arg_list.pointer = &obj;

	st = acpi_evaluate_integer(NULL, "\\M009", &arg_list, &result);
	if (ACPI_FAILURE(st))
		pr_err("cyc: M009(0x%llx) FAILED: %s\n", arg, acpi_format_exception(st));
	return result;
}

static int call_method(acpi_handle h, const char *name)
{
	acpi_status st = acpi_evaluate_object(h, (acpi_string)name, NULL, NULL);
	if (ACPI_FAILURE(st)) {
		pr_err("cyc: %s FAILED: %s\n", name, acpi_format_exception(st));
		return -EIO;
	}
	pr_info("cyc: %s OK\n", name);
	return 0;
}

static int __init cyc_init(void)
{
	acpi_handle hspi;
	acpi_status st;
	unsigned long long v5b_before, v103_before, v5b_after, v103_after;

	pr_info("cyc: ===== forcing real _PS3 -> _PS0 cycle on HSPI =====\n");

	st = acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &hspi);
	if (ACPI_FAILURE(st)) {
		pr_err("cyc: could not get HSPI handle: %s\n", acpi_format_exception(st));
		return -ENODEV;
	}

	v5b_before = read_m009(0x5B);
	v103_before = read_m009(0x103);
	pr_info("cyc: BEFORE  pin5B=0x%llx pin103=0x%llx\n", v5b_before, v103_before);

	pr_info("cyc: calling _PS3 (power off: reset assert, sleep 1ms, power off)...\n");
	if (call_method(hspi, "_PS3"))
		goto done;

	msleep(50);

	pr_info("cyc: calling _PS0 (power on: power on, sleep 300ms, reset deassert)...\n");
	if (call_method(hspi, "_PS0"))
		goto done;

	v5b_after = read_m009(0x5B);
	v103_after = read_m009(0x103);
	pr_info("cyc: AFTER   pin5B=0x%llx pin103=0x%llx\n", v5b_after, v103_after);
	pr_info("cyc: delta   pin5B %s  pin103 %s\n",
		v5b_before != v5b_after ? "CHANGED" : "same",
		v103_before != v103_after ? "CHANGED" : "same");

done:
	pr_info("cyc: DONE\n");
	return -ENODEV;
}
module_init(cyc_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Force a real _PS3->_PS0 power cycle on SL4A touch HSPI, read-back verified");
