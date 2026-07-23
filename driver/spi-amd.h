/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2025, Advanced Micro Devices, Inc. */

#ifndef AMD_SPI_H
#define AMD_SPI_H

#include <linux/spi/spi.h>

enum amd_spi_versions {
	AMD_SPI_V2 = 2,	/* AMDI0062 */
	AMD_HID2_SPI = 3,	/* AMDI0063 */
};

/**
 * struct amd_spi - AMD SPI driver instance
 * @io_remap_addr:	Start address of the SPI controller registers
 * @version:		SPI controller version
 * @speed_hz:		Device frequency
 */
struct amd_spi {
	void __iomem *io_remap_addr;
	enum amd_spi_versions version;
	u32 speed_hz;
};

int amd_spi_probe_common(struct device *dev, struct spi_controller *host);

#endif /* AMD_SPI_H */
