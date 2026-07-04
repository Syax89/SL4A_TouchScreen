#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};


MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Chid-over-spi");
MODULE_ALIAS("of:N*T*Chid-over-spiC*");
MODULE_ALIAS("acpi*:MSHW0134:*");
MODULE_ALIAS("acpi*:MSHW0162:*");
MODULE_ALIAS("acpi*:MSHW0235:*");
MODULE_ALIAS("acpi*:PNP0C51:*");
MODULE_ALIAS("spi:hid");
MODULE_ALIAS("spi:hid-over-spi");

MODULE_INFO(srcversion, "6E1665CB6B28C98482E91C7");
