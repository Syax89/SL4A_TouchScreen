/* SPDX-License-Identifier: GPL-2.0 */
#ifndef SPI_HID_CAPIMG_H
#define SPI_HID_CAPIMG_H

#include <linux/types.h>

#define SPI_HID_CAPIMG_BODY_LENGTH 4304
#define SPI_HID_CAPIMG_RASTER_SAMPLES 3456

struct spi_hid_capimg_raster {
	const u8 *samples;
};

/*
 * Validate a complete V0 0x0c body and return its decoded single-run raster.
 * The returned pointer aliases body and is valid only while body is retained.
 */
int spi_hid_capimg_decode_v0(const u8 *body, size_t body_length,
			     struct spi_hid_capimg_raster *raster);

#endif
