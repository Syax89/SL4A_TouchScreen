#include <linux/module.h>
#include <linux/io.h>
#include <linux/ktime.h>
static void __iomem *gpio;
static bool pin_asserted(void){ return !(ioread32(gpio+0x154) & (1u<<16)); }
static int __init bc_init(void){
	ktime_t t0; int edges=0; bool prev;
	gpio = ioremap(0xFED81500, 0x300);
	if(!gpio) return -ENOMEM;
	prev = pin_asserted();
	t0 = ktime_get();
	pr_info("baseline: watching pin85 for 2000ms, no writes at all...\n");
	while(ktime_ms_delta(ktime_get(),t0) < 2000){
		bool now = pin_asserted();
		if(now && !prev){ edges++; pr_info("baseline: edge #%d at +%lldms\n", edges, ktime_ms_delta(ktime_get(),t0)); }
		prev = now;
		cpu_relax();
	}
	pr_info("baseline: total spontaneous edges in 2000ms = %d\n", edges);
	iounmap(gpio);
	return -ENODEV;
}
module_init(bc_init);
MODULE_LICENSE("GPL");
