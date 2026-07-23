/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_ERRNO_H
#define TEST_LINUX_ERRNO_H

#include <errno.h>

/* glibc's errno.h may re-enter this shim through linux/errno.h. */
#ifndef EINVAL
#define EINVAL 22
#endif

#endif
