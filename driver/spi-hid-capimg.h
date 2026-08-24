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
 * expected_samples is the device's CapImg raster cell count (3456 on SL4
 * MSHW0231, 4056 on SL3 MSHW0162); frames carrying a different count are
 * rejected.
 * The returned pointer aliases body and is valid only while body is retained.
 */
int spi_hid_capimg_decode_v0(const u8 *body, size_t body_length,
			     u32 expected_samples,
			     struct spi_hid_capimg_raster *raster);

#endif
