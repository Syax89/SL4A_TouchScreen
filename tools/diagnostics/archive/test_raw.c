/* Test: bare SPI reads without HID framing, try multiple register offsets */
#include <linux/module.h>
#include <linux/io.h>

#define FCH_MMIO 0xFEC10000
#define FCH_SIZE 0x100

static int __init rawtest_init(void)
{
	void __iomem *base;
	int i, r;

	base = ioremap(FCH_MMIO, FCH_SIZE);
	if (!base) return -ENOMEM;

	pr_info("rawtest: started\n");

	/* Try pure READ (opcode 0x0B) with NO TX data — just read from device */
	for (r = 0; r < 3; r++) {
		/* Clear FIFO */
		u32 ctrl0 = ioread32(base + 0x00);
		ctrl0 &= ~BIT(20);
		iowrite32(ctrl0, base + 0x00);
		ctrl0 |= BIT(20);
		iowrite32(ctrl0, base + 0x00);

		/* Write 0 to TX_COUNT (no TX data) */
		iowrite8(0, base + 0x48);
		/* Read up to 32 bytes */
		iowrite8(32, base + 0x4B);

		/* Set READ opcode */
		iowrite8(0x0B, base + 0x45);

		/* Trigger */
		{
			u8 trig = ioread8(base + 0x47);
			trig &= ~BIT(7);
			iowrite8(trig, base + 0x47);
			trig |= BIT(7);
			iowrite8(trig, base + 0x47);
		}

		/* Wait for busy to toggle */
		{
			int timeout = 100000;
			u32 s;
			while (!((s = ioread32(base + 0x04)) & BIT(31)) && --timeout)
				udelay(1);
			pr_info("rawtest[%d] BUSY_START status=0x%08x to=%d\n", r, s, timeout);

			while ((ioread32(base + 0x04) & BIT(31)) && --timeout)
				udelay(1);
			pr_info("rawtest[%d] BUSY_END to=%d\n", r, timeout);
		}

		/* Read FIFO */
		{
			u8 buf[32];
			for (i = 0; i < 32; i++)
				buf[i] = ioread8(base + 0x80 + i);
			pr_info("rawtest[%d] FIFO: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
				r, buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],
				buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],buf[15]);
			pr_info("rawtest[%d] +16: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
				r, buf[16],buf[17],buf[18],buf[19],buf[20],buf[21],buf[22],buf[23],
				buf[24],buf[25],buf[26],buf[27],buf[28],buf[29],buf[30],buf[31]);
		}

		msleep(100);
	}

	iounmap(base);
	return -ENODEV;
}

module_init(rawtest_init);
MODULE_LICENSE("GPL");
