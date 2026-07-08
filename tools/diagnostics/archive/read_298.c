/* tools/diagnostics/read_298.c
 * Read register 0xFED80298 to see if bits 31 or 28 are set.
 * If they are set, then UEFI overrides the chip select to CS0.
 *
 * insmod read_298.ko && dmesg | grep "r298:"
 */
#include <linux/module.h>
#include <linux/io.h>

#define ADDR 0xFED80298

static int __init r298_init(void) {
	void __iomem *base = ioremap(ADDR, 4);
	if (!base) {
		pr_err("r298: ioremap failed\n");
		return -ENOMEM;
	}
	pr_info("r298: value at 0xFED80298 = 0x%08x\n", ioread32(base));
	iounmap(base);
	return -ENODEV;
}

static void __exit r298_exit(void) {}

module_init(r298_init);
module_exit(r298_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Read 0xFED80298 register");
