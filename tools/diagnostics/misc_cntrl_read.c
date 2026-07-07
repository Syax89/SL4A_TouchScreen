/* misc_cntrl_read.c — read-only check of SPI_MISC_CNTRL (0xFC) semaphore:
 * PSP_OWNS(bit10), HFP_OWNS(bit9), HBIOS_OWNS(bit5). Windows is 0x0000 (no lock)
 * in every captured moment; Linux was never actually read. Read-only, zero risk. */
#include <linux/module.h>
#include <linux/io.h>
#define FCH_BASE 0xFEC10000
static int __init mc_init(void){
	void __iomem *base = ioremap(FCH_BASE, 0x100);
	u16 v;
	if(!base) return -ENOMEM;
	v = ioread16(base+0xFC);
	pr_info("misc_cntrl: SPI_MISC_CNTRL(0xFC) = 0x%04x  PSP_OWNS=%d HFP_OWNS=%d HBIOS_OWNS=%d\n",
		v, !!(v&BIT(10)), !!(v&BIT(9)), !!(v&BIT(5)));
	iounmap(base);
	return -ENODEV;
}
module_init(mc_init);
MODULE_LICENSE("GPL");
