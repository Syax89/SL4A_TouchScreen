/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_JIFFIES_H
#define TEST_LINUX_JIFFIES_H

#include <stdbool.h>

/*
 * Host stub clock. 1 jiffy == 1 ms (matches msecs_to_jiffies() below being
 * the identity function), so tests can reason in milliseconds directly.
 * Defined once in raw-pipeline-stubs/stubs.c; tests advance it directly
 * between fed frames to simulate elapsed capture time.
 */
extern unsigned long jiffies;

static inline unsigned long msecs_to_jiffies(unsigned int msecs)
{
	return msecs;
}

/* Same wraparound-safe comparison as the kernel's time_after(). */
static inline bool time_after(unsigned long a, unsigned long b)
{
	return (long)(b - a) < 0;
}

#endif
