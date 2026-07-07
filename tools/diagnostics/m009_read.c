/* m009_read.c — READ-ONLY diagnostic: call \_SB.M009(pin) via ACPI evaluate.
 *
 * M009 is a pure getter: read its AML body (docs/acpi/linux/ssdt5.dsl:484-573)
 * and its helpers M011 (ssdt5.dsl:796) / M249 (ssdt5.dsl:275) — for pin values
 * 0x5B and 0x0103 (both < 0x800) the call resolves to M011, which does exactly
 * one 1-byte SystemMemory read (OperationRegion(...SystemMemory...); Local1 = VARR;
 * Return masked bit). No M012/M250 (the write counterparts) are ever invoked.
 * M249 (the SMN-indirect path, used only for pin values >= 0x800) is not reached
 * for our arguments either, and is itself read-only + mutex-protected + restores
 * the SMN index register afterward when it is used.
 *
 * WHY: _INI (run once by ACPI before Linux's spi-hid probe) guards its power-on
 * sequence with:
 *   Local0 = M009(0x5B); If (Local0 == Zero) { M010(0x103,0); M010(0x5B,1); Sleep(300ms) }
 * If GPIO pin 0x5B already reads 1 at boot (firmware default), this guard is
 * FALSE and _INI skips the power-on toggle + 300ms settle entirely — the
 * touchscreen would never get a clean power cycle. This module only READS
 * M009(0x5B) and M009(0x0103) to check whether that guard would be true or
 * false right now, without writing anything.
 *
 * insmod m009_read.ko && dmesg | grep "m009:"
 */
#include <linux/module.h>
#include <linux/acpi.h>

static void read_pin(acpi_handle scope, const char *label, u64 arg)
{
	struct acpi_object_list arg_list;
	union acpi_object obj;
	unsigned long long result;
	acpi_status st;

	obj.type = ACPI_TYPE_INTEGER;
	obj.integer.value = arg;
	arg_list.count = 1;
	arg_list.pointer = &obj;

	/* Relative walk-up from HSPI ("M009") failed with AE_NOT_FOUND even
	 * though the handle itself resolved fine — the kernel's evaluate API
	 * doesn't replicate AML's automatic parent-scope search for a bare
	 * NameSeg. ssdt5.dsl shows M009's Method() with no enclosing Scope()
	 * at all (root-level in that table), so try the absolute root path. */
	(void)scope;
	st = acpi_evaluate_integer(NULL, "\\M009", &arg_list, &result);
	if (ACPI_FAILURE(st))
		pr_err("m009: %s (arg=0x%llx) FAILED: %s\n",
			label, arg, acpi_format_exception(st));
	else
		pr_info("m009: %s (arg=0x%llx) = 0x%llx\n", label, arg, result);
}

static int __init m009_read_init(void)
{
	acpi_handle hspi;
	acpi_status st;

	pr_info("m009: ===== M009 read-only probe (touch power/reset pin state) =====\n");

	st = acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &hspi);
	if (ACPI_FAILURE(st)) {
		pr_err("m009: could not get handle for \\_SB.SPI1.HSPI: %s\n",
			acpi_format_exception(st));
		return -ENODEV;
	}

	read_pin(hspi, "pin 0x5B  (power gate, checked by _INI's guard)", 0x5B);
	read_pin(hspi, "pin 0x103 (reset line)", 0x103);
	pr_info("m009: DONE (0 = _INI's power-on branch WOULD run; 1 = it was SKIPPED)\n");
	return -ENODEV;
}
module_init(m009_read_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Read-only M009 probe for SL4A touch power sequencing guard");
