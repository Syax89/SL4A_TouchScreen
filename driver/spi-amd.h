/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * AMD SPI controller driver
 *
 * Copyright (c) 2025, Advanced Micro Devices, Inc.
 *
 * Author: Krishnamoorthi M <krishnamoorthi.m@amd.com>
 */

#ifndef AMD_SPI_H
#define AMD_SPI_H

#include <linux/spi/spi.h>

enum amd_spi_versions {
	AMD_SPI_V1 = 1,	/* AMDI0061 */
	AMD_SPI_V2,	/* AMDI0062 */
	AMD_HID2_SPI,	/* AMDI0063 */
};

/**
 * struct amd_spi - AMD SPI driver instance
 * @io_remap_addr:	Start address of the SPI controller registers
 * @phy_dma_buf:	Physical address of DMA buffer
 * @dma_virt_addr:	Virtual address of DMA buffer
 * @version:		SPI controller version
 * @speed_hz:		Device frequency
 */
struct amd_spi {
	void __iomem *io_remap_addr;
	dma_addr_t phy_dma_buf;
	void *dma_virt_addr;
	enum amd_spi_versions version;
	u32 speed_hz;
	u16 speed_cfg;		/* SPI100_SPEED_CONFIG at MMIO+0x22 (4 speed tiers) */
	u32 txmode_saved_ctrl0;	/* CTRL0 before TXMODE set, restored after segment */
	u8 speed_hz_index;	/* 0=F_66_66MHz, 1=F_33_33MHz, etc. */
	struct pci_dev *lpc;		/* Future: LPC bridge for 8-bit FIFO config */
	u32 lpc_b4;			/* Saved LPC 0xB4 config (future use) */
	u32 lpc_b8;			/* Saved LPC 0xB8 config (future use) */
	bool lpc_configured;		/* Future: true if LPC bridge was reconfigured */
};

int amd_spi_probe_common(struct device *dev, struct spi_controller *host);

#endif /* AMD_SPI_H */
