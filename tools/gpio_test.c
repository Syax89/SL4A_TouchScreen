/* GPIO test module: call M009/M010 via ACPI to check touchscreen power rails */
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/delay.h>

static int __init gpio_test_init(void)
{
	acpi_handle m009_handle, m010_handle;
	acpi_status st;
	unsigned long long val = 0xDEADBEEF;
	struct acpi_object_list args;
	union acpi_object arg1, arg2;

	pr_info("gpio_test: looking for \\M009 and \\M010 (root scope from SSDT5)\n");

	st = acpi_get_handle(NULL, "\\M009", &m009_handle);
	pr_info("gpio_test: M009 handle = %d (ptr=%px)\n", st, m009_handle);

	st = acpi_get_handle(NULL, "\\M010", &m010_handle);
	pr_info("gpio_test: M010 handle = %d (ptr=%px)\n", st, m010_handle);

	if (ACPI_FAILURE(st)) {
		pr_err("gpio_test: M010 not found\n");
		return -ENODEV;
	}

	/* Read M009(0x5B): Bank 0, Pin 91 - touchscreen power enable */
	arg1.type = ACPI_TYPE_INTEGER;
	arg1.integer.value = 0x5B;
	args.count = 1;
	args.pointer = &arg1;

	val = 0xDEADBEEF;
	st = acpi_evaluate_object(m009_handle, NULL, &args, NULL);
	pr_info("gpio_test: M009(0x5B) st=%d (AE_OK=%d)\n", st, AE_OK);
	/* Try to get return value via integer */
	{
		struct acpi_buffer ret = { ACPI_ALLOCATE_BUFFER, NULL };
		st = acpi_evaluate_object(m009_handle, NULL, &args, &ret);
		pr_info("gpio_test: M009(0x5B) ret_buf st=%d\n", st);
		if (ACPI_SUCCESS(st) && ret.pointer) {
			union acpi_object *obj = ret.pointer;
			pr_info("gpio_test: M009(0x5B) type=%d", obj->type);
			if (obj->type == ACPI_TYPE_INTEGER)
				pr_cont(" value=%llu (0x%llx)\n", obj->integer.value, obj->integer.value);
			else
				pr_cont("\n");
			kfree(ret.pointer);
		}
	}

	/* Read M009(0x0103): Bank 1, Pin 3 - touchscreen reset */
	arg1.integer.value = 0x0103;
	{
		struct acpi_buffer ret = { ACPI_ALLOCATE_BUFFER, NULL };
		st = acpi_evaluate_object(m009_handle, NULL, &args, &ret);
		pr_info("gpio_test: M009(0x0103) ret_buf st=%d\n", st);
		if (ACPI_SUCCESS(st) && ret.pointer) {
			union acpi_object *obj = ret.pointer;
			pr_info("gpio_test: M009(0x0103) type=%d", obj->type);
			if (obj->type == ACPI_TYPE_INTEGER)
				pr_cont(" value=%llu (0x%llx)\n", obj->integer.value, obj->integer.value);
			else
				pr_cont("\n");
			kfree(ret.pointer);
		}
	}

	/* Try M010(0x5B, 1): enable power */
	{
		union acpi_object pargs[2];
		pargs[0].type = ACPI_TYPE_INTEGER;
		pargs[0].integer.value = 0x5B;
		pargs[1].type = ACPI_TYPE_INTEGER;
		pargs[1].integer.value = 1;
		args.count = 2;
		args.pointer = pargs;
		st = acpi_evaluate_object(m010_handle, NULL, &args, NULL);
		pr_info("gpio_test: M010(0x5B, 1) st=%d\n", st);
	}

	/* Try M010(0x0103, 1): de-assert reset */
	{
		union acpi_object pargs[2];
		pargs[0].type = ACPI_TYPE_INTEGER;
		pargs[0].integer.value = 0x0103;
		pargs[1].type = ACPI_TYPE_INTEGER;
		pargs[1].integer.value = 1;
		args.count = 2;
		args.pointer = pargs;
		st = acpi_evaluate_object(m010_handle, NULL, &args, NULL);
		pr_info("gpio_test: M010(0x0103, 1) st=%d\n", st);
	}

	return -ENODEV; /* Always fail so module doesn't stay loaded */
}

module_init(gpio_test_init);
MODULE_LICENSE("GPL");
