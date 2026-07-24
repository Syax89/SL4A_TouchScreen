/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_MODULE_H
#define TEST_LINUX_MODULE_H

/*
 * On host, module params are plain globals declared right above the
 * macro call in mshw0231-raw.c (e.g. `static bool invert_x;`); the
 * test binary sets them directly instead of via modprobe/sysfs. These
 * macros only need to expand to something syntactically valid at file
 * scope so the declaration site still compiles — they emit an unused,
 * uniquely-named stub function rather than a bare `;` so behaviour
 * does not depend on the compiler's tolerance for stray top-level
 * semicolons under -Wall -Wextra -Werror.
 */
#define module_param(name, type, perm) \
	static void __attribute__((unused)) __module_param_##name(void) {}

#define MODULE_PARM_DESC(name, desc) \
	static void __attribute__((unused)) __module_parm_desc_##name(void) {}

#endif
