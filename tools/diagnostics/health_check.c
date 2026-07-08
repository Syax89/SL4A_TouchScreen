#include <linux/module.h>
#include <linux/io.h>
static int __init hc_init(void){
	void __iomem *base = ioremap(0xFEC10000, 0x100);
	void __iomem *gpio;
	if(!base) return -ENOMEM;
	gpio = ioremap(0xFED81500, 0x300);
	if(!gpio){ iounmap(base); return -ENOMEM; }
	pr_info("health: CTRL0=0x%08x STATUS=0x%08x ENA=0x%08x pin85_raw=0x%08x\n",
		ioread32(base+0x00), ioread32(base+0x4c), ioread32(base+0x20), ioread32(gpio+0x154));
	iounmap(gpio); iounmap(base);
	return -ENODEV;
}
module_init(hc_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("One-shot dump of CTRL0/STATUS/ENA/pin85_raw to rule out register corruption");
