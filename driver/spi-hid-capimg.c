// SPDX-License-Identifier: GPL-2.0
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/unaligned.h>

#include "spi-hid-capimg.h"

#define V0_USAGE_61_OFFSET 5
#define V0_USAGE_61_LENGTH 4297
#define CAPIMG_HEADER_LENGTH 7
#define CAPIMG_HEATMAP_SECTION 0x0100
#define CAPIMG_VENDOR_SECTION 0xff00

int spi_hid_capimg_decode_v0(const u8 *body, size_t body_length,
			     struct spi_hid_capimg_raster *raster)
{
	const u8 *payload;
	size_t container_length;
	size_t offset;
	bool heatmap_seen = false;
	bool vendor_seen = false;

	/* Accept minor frame size variations (observed 4302-4304 bytes). */
	if (!body || !raster || body_length < 4300 || body_length > 4400 ||
	    body[0] != 0xce || body[1] != 0x10 || body[2] != 0x0c)
		return -EINVAL;

	payload = body + V0_USAGE_61_OFFSET;
	container_length = get_unaligned_le32(payload);
	if (get_unaligned_le16(payload + 4) != 0 || payload[6] != 0 ||
	    container_length < CAPIMG_HEADER_LENGTH ||
	    container_length > V0_USAGE_61_LENGTH)
		return -EINVAL;

	offset = CAPIMG_HEADER_LENGTH;
	while (offset < container_length) {
		size_t section_length;
		u16 section_type;

		if (container_length - offset < CAPIMG_HEADER_LENGTH)
			return -EINVAL;
		section_length = get_unaligned_le32(payload + offset);
		section_type = get_unaligned_le16(payload + offset + 4);
		if (section_length < CAPIMG_HEADER_LENGTH ||
		    section_length > container_length - offset)
			return -EINVAL;

		if (section_type == CAPIMG_HEATMAP_SECTION) {
			if (heatmap_seen || section_length != 16 + SPI_HID_CAPIMG_RASTER_SAMPLES ||
			    payload[offset + 6] != 1 || payload[offset + 7] != 8 ||
			    get_unaligned_le32(payload + offset + 8) != 0 ||
			    get_unaligned_le32(payload + offset + 12) != SPI_HID_CAPIMG_RASTER_SAMPLES)
				return -EINVAL;
			raster->samples = payload + offset + 16;
			heatmap_seen = true;
		} else if (section_type == CAPIMG_VENDOR_SECTION) {
			if (vendor_seen)
				return -EINVAL;
			vendor_seen = true;
		}
		offset += section_length;
	}

	return offset == container_length && heatmap_seen && vendor_seen ? 0 : -EINVAL;
}
