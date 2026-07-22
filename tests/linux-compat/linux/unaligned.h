/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_UNALIGNED_H
#define TEST_LINUX_UNALIGNED_H

#include <stdint.h>

static inline uint16_t get_unaligned_le16(const void *address)
{
	const uint8_t *bytes = address;

	return (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8);
}

static inline uint32_t get_unaligned_le32(const void *address)
{
	const uint8_t *bytes = address;

	return (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8) |
		((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);
}

#endif
