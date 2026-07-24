/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_KERNEL_H
#define TEST_LINUX_KERNEL_H

#include <stdbool.h>
#include <stdint.h>
#include <linux/types.h>

/* READ_ONCE() is a no-op on a single-threaded host test: no concurrent
 * writer exists, so a plain read is observationally equivalent. */
#define READ_ONCE(x) (x)
#define WRITE_ONCE(x, val) ((x) = (val))

#ifndef min
#define min(a, b) __extension__ ({ \
	__typeof__(a) _min_a = (a); \
	__typeof__(b) _min_b = (b); \
	_min_a < _min_b ? _min_a : _min_b; })
#endif

#ifndef max
#define max(a, b) __extension__ ({ \
	__typeof__(a) _max_a = (a); \
	__typeof__(b) _max_b = (b); \
	_max_a > _max_b ? _max_a : _max_b; })
#endif

#ifndef clamp_t
#define clamp_t(type, val, lo, hi) __extension__ ({ \
	type _clamp_v = (type)(val); \
	type _clamp_lo = (type)(lo); \
	type _clamp_hi = (type)(hi); \
	_clamp_v < _clamp_lo ? _clamp_lo : (_clamp_v > _clamp_hi ? _clamp_hi : _clamp_v); })
#endif

/* Kernel's div_s64() truncates toward zero, same as C integer division;
 * a plain division is behaviourally equivalent for the value ranges
 * exercised by the raw touch pipeline (no s64 overflow at 72x48 cells). */
static inline s64 div_s64(s64 dividend, s64 divisor)
{
	return dividend / divisor;
}

#define S32_MAX ((s32)0x7fffffff)

#endif
