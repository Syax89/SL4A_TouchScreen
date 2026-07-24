/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_MATH_H
#define TEST_LINUX_MATH_H

/*
 * int_sqrt() — integer square root, floor(sqrt(x)).
 *
 * This is a host transcription of the real kernel algorithm
 * (lib/math/int_sqrt.c): a binary digit-by-digit square root that
 * needs no floating point and matches kernel behaviour exactly for
 * every unsigned long input, not just an approximation.
 */
static inline unsigned long int_sqrt(unsigned long x)
{
	unsigned long b, m, y = 0;

	if (x <= 1)
		return x;

	m = 1UL << (sizeof(unsigned long) * 8 - 2);
	while (m != 0) {
		b = y + m;
		y >>= 1;
		if (x >= b) {
			x -= b;
			y += m;
		}
		m >>= 2;
	}
	return y;
}

#endif
