/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_SLAB_H
#define TEST_LINUX_SLAB_H

#include <stdlib.h>
#include <stdint.h>

#define GFP_KERNEL 0

static inline void *kmalloc(unsigned long size, int flags)
{
	(void)flags;
	return malloc((size_t)size);
}

static inline void kfree(const void *ptr)
{
	free((void *)(uintptr_t)ptr);
}

#endif
