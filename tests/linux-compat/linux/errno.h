/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_ERRNO_H
#define TEST_LINUX_ERRNO_H

#include <errno.h>

/* glibc's <errno.h> -> <bits/errno.h> re-enters this exact file via its
 * own `#include <linux/errno.h>` to pull in the E* constant values from
 * the kernel UAPI header. Since our include guard is already active for
 * that nested include, it no-ops and none of the real E* codes actually
 * get defined that way — so every errno code the driver sources
 * reference has to be listed here explicitly as a fallback. */
#ifndef EINVAL
#define EINVAL 22
#endif
#ifndef ENODEV
#define ENODEV 19
#endif

#endif
