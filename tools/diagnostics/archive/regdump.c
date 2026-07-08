/* Quick kernel module to dump AMD FCH SPI registers at 0xFEC10000 */
#include <linux/module.h>
#include <linux/io.h>

#define FCH_MMIO 0xFEC10000
#define FCH_SIZE 0x100

static int __init regdump_init(void)
{
	void __iomem *base;
	int i;

	base = ioremap(FCH_MMIO, FCH_SIZE);
	if (!base) {
		pr_err("regdump: ioremap failed\n");
		return -ENOMEM;
	}

	pr_info("=== AMD FCH SPI Registers at 0x%08X ===\n", FCH_MMIO);

	/* CTRL0 (32-bit at 0x00) */
	pr_info("CTRL0   (0x00): 0x%08X\n", ioread32(base + 0x00));
	/* STATUS (32-bit at 0x04) */
	pr_info("STATUS  (0x04): 0x%08X\n", ioread32(base + 0x04));

	/* Read key byte registers */
	for (i = 0x08; i <= 0x50; i += 8) {
		pr_info("0x%02X-0x%02X: %02X %02X %02X %02X %02X %02X %02X %02X\n",
			i, i+7,
			ioread8(base + i), ioread8(base + i+1),
			ioread8(base + i+2), ioread8(base + i+3),
			ioread8(base + i+4), ioread8(base + i+5),
			ioread8(base + i+6), ioread8(base + i+7));
	}

	/* Read the REGISTER PREFIX at 0x22 (from Windows decomp) */
	pr_info("REG_PREFIX (0x22): 0x%02X  <-- should be 3?\n", ioread8(base + 0x22));
	pr_info("0x44 (VERSION):    0x%02X%02X\n",
		ioread8(base + 0x45), ioread8(base + 0x44));

	/* FIFO area dump (first 16 bytes) */
	pr_info("FIFO (0x80..0x8F): %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
		ioread8(base + 0x80), ioread8(base + 0x81),
		ioread8(base + 0x82), ioread8(base + 0x83),
		ioread8(base + 0x84), ioread8(base + 0x85),
		ioread8(base + 0x86), ioread8(base + 0x87),
		ioread8(base + 0x88), ioread8(base + 0x89),
		ioread8(base + 0x8a), ioread8(base + 0x8b),
		ioread8(base + 0x8c), ioread8(base + 0x8d),
		ioread8(base + 0x8e), ioread8(base + 0x8f));

	iounmap(base);
	return -ENODEV; /* Don't stay loaded */
}

static void __exit regdump_exit(void) {}

module_init(regdump_init);
module_exit(regdump_exit);
MODULE_LICENSE("GPL");
