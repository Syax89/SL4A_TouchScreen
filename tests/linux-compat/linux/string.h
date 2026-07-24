/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_STRING_H
#define TEST_LINUX_STRING_H

/*
 * mshw0231-raw.c only uses memset()/memcpy() from <linux/string.h>,
 * both of which have identical signatures/semantics to libc's. Just
 * forward to the host C library rather than reimplementing anything.
 */
#include <string.h>

#endif
