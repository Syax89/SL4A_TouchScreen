/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2025, Advanced Micro Devices, Inc. */
/* Based on upstream v6.15 spi-amd.c; added AMDI0060 + multi-opcode support. */

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include "spi-amd.h"

#define AMD_SPI_CTRL0_REG	0x00
#define AMD_SPI_EXEC_CMD	BIT(16)
#define AMD_SPI_FIFO_CLEAR	BIT(20)
#define AMD_SPI_BUSY		BIT(31)

#define AMD_SPI_OPCODE_REG	0x45
#define AMD_SPI_CMD_TRIGGER_REG	0x47
#define AMD_SPI_TRIGGER_CMD	BIT(7)
#define AMD_SPI_OPCODE_MASK	0xFF

#define AMD_SPI_ALT_CS_REG	0x1D
#define AMD_SPI_ALT_CS_MASK	0x3

#define AMD_SPI_FIFO_BASE	0x80
#define AMD_SPI_TX_COUNT_REG	0x48
#define AMD_SPI_RX_COUNT_REG	0x4B
#define AMD_SPI_STATUS_REG	0x4C

#define AMD_SPI_FIFO_SIZE	70

/* SPI_MISC_CNTRL: PSP ownership prevents host-controller accesses. */
#define AMD_SPI_MISC_CNTRL_REG	0xFC
#define AMD_SPI_PSP_OWNS	BIT(10)

/* Diagnostic output is deliberately opt-in: level 1 records lifecycle and
 * controller state, level 2 records every SPI segment, and level 3 includes
 * the first bytes returned by each read. */
static int debug_trace;
module_param(debug_trace, int, 0644);
MODULE_PARM_DESC(debug_trace,
	"Diagnostic trace: 0=off, 1=lifecycle, 2=SPI segments, 3=read data");

#define amd_trace(dev, level, fmt, ...) \
	do { if (debug_trace >= (level)) \
		dev_info((dev), "TRACE[amd:%d] " fmt, (level), ##__VA_ARGS__); } while (0)

/* SPI speed control registers (from upstream spi-amd) */
#define AMD_SPI_ENA_REG		0x20
#define AMD_SPI_ALT_SPD_SHIFT	20
#define AMD_SPI_ALT_SPD_MASK	GENMASK(23, AMD_SPI_ALT_SPD_SHIFT)
#define AMD_SPI_SPI100_SHIFT	0
#define AMD_SPI_SPI100_MASK	GENMASK(AMD_SPI_SPI100_SHIFT, AMD_SPI_SPI100_SHIFT)
#define AMD_SPI_SPEED_REG	0x6C
#define AMD_SPI_SPD7_SHIFT	8
#define AMD_SPI_SPD7_MASK	GENMASK(13, AMD_SPI_SPD7_SHIFT)

/* Enum for speed register values.
 * Note: F_100MHz=4 and F_50MHz=0x4 share the same numeric value 4.
 * This is intentional — both hardware register fields (ALT_SPD in ENA_REG
 * and SPD7 in SPEED_REG) independently use the value 4 for their respective
 * speed settings. The frequency table disambiguates by using the enum in
 * different columns (enable_val vs spd7_val). */
enum amd_spi_speed_val {
	F_66_66MHz,
	F_33_33MHz,
	F_22_22MHz,
	F_16_66MHz,
	F_100MHz,
	F_800KHz,
	SPI_SPD7 = 0x7,
	F_50MHz = 0x4,
	F_4MHz = 0x32,
	F_3_17MHz = 0x3f,
};

struct amd_spi_freq {
	u32 speed_hz;
	enum amd_spi_speed_val enable_val;
	u8 spd7_val;
};

/* SPI controller configuration bits (from coreboot amdblocks/spi.h) */
#define AMD_SPI_READ_MODE_FAST	0x60040000 /* bits 30+29+18 = 0b111 = FAST_READ (value 7) */
#define AMD_SPI_TXMODE_BIT	0x00800000 /* bit 23: force TX, prevent RX switch */
#define AMD_SPI_SPEED_CONFIG_REG 0x44	/* write16 clobbers 0x45 (opcode), re-write needed */

static inline u8 amd_spi_readreg8(struct amd_spi *amd_spi, int idx)
{
	return readb((u8 __iomem *)amd_spi->io_remap_addr + idx);
}

static inline void amd_spi_writereg8(struct amd_spi *amd_spi, int idx, u8 val)
{
	writeb(val, ((u8 __iomem *)amd_spi->io_remap_addr + idx));
}

static inline u16 amd_spi_readreg16(struct amd_spi *amd_spi, int idx)
{
	return readw((u8 __iomem *)amd_spi->io_remap_addr + idx);
}

static inline void amd_spi_writereg16(struct amd_spi *amd_spi, int idx, u16 val)
{
	writew(val, ((u8 __iomem *)amd_spi->io_remap_addr + idx));
}

static inline u32 amd_spi_readreg32(struct amd_spi *amd_spi, int idx)
{
	return readl((u8 __iomem *)amd_spi->io_remap_addr + idx);
}

static inline void amd_spi_writereg32(struct amd_spi *amd_spi, int idx, u32 val)
{
	writel(val, ((u8 __iomem *)amd_spi->io_remap_addr + idx));
}

static int amd_spi_check_psp_ownership(struct amd_spi *amd_spi)
{
	if (amd_spi_readreg16(amd_spi, AMD_SPI_MISC_CNTRL_REG) & AMD_SPI_PSP_OWNS)
		return -EBUSY;

	return 0;
}

static void amd_spi_setclear_reg32(struct amd_spi *amd_spi, int idx, u32 set, u32 clear)
{
	u32 tmp = amd_spi_readreg32(amd_spi, idx);
	tmp = (tmp & ~clear) | set;
	amd_spi_writereg32(amd_spi, idx, tmp);
}

static int amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs)
{
	u8 tmp;
	int ret;

	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret)
		return ret;

	tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
	tmp &= ~AMD_SPI_ALT_CS_MASK;
	/* MSHW0231 is wired to CS1; generic chip-select support is unverified. */
	tmp |= 0x01;
	amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);

	return 0;
}

static int amd_spi_clear_chip(struct amd_spi *amd_spi, u8 cs)
{
	u8 tmp;
	int ret;

	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret)
		return ret;

	tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
	tmp &= ~AMD_SPI_ALT_CS_MASK;
	amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);

	return 0;
}

static const struct amd_spi_freq amd_spi_freq[] = {
	{ 100000000,   F_100MHz,         0},
	{  66660000, F_66_66MHz,         0},
	{  50000000,   SPI_SPD7,   F_50MHz},
	{  33330000, F_33_33MHz,         0},
	{  22220000, F_22_22MHz,         0},
	{  16660000, F_16_66MHz,         0},
	{   4000000,   SPI_SPD7,    F_4MHz},
	{   3170000,   SPI_SPD7, F_3_17MHz},
	{    800000,   F_800KHz,         0},
};

static int amd_set_spi_freq(struct amd_spi *amd_spi, u32 speed_hz)
{
	unsigned int i, spd7_val, alt_spd;
	int ret;

	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret)
		return ret;

	for (i = 0; i < ARRAY_SIZE(amd_spi_freq) - 1; i++)
		if (speed_hz >= amd_spi_freq[i].speed_hz)
			break;

	if (amd_spi->speed_hz == amd_spi_freq[i].speed_hz)
		return 0;

	amd_spi->speed_hz = amd_spi_freq[i].speed_hz;

	alt_spd = (amd_spi_freq[i].enable_val << AMD_SPI_ALT_SPD_SHIFT)
		   & AMD_SPI_ALT_SPD_MASK;
	amd_spi_setclear_reg32(amd_spi, AMD_SPI_ENA_REG, alt_spd,
			       AMD_SPI_ALT_SPD_MASK);

	/* Firmware may retain SPI100/SPD7 state required by another FCH client.
	 * Set these extensions only when selected; never clear them for a lower
	 * requested speed. Clearing either field has previously stalled this FCH.
	 */
	if (amd_spi->speed_hz == 100000000)
		amd_spi_setclear_reg32(amd_spi, AMD_SPI_ENA_REG,
				       AMD_SPI_SPI100_MASK, AMD_SPI_SPI100_MASK);

	if (amd_spi_freq[i].spd7_val) {
		spd7_val = (amd_spi_freq[i].spd7_val << AMD_SPI_SPD7_SHIFT) &
			AMD_SPI_SPD7_MASK;
		amd_spi_setclear_reg32(amd_spi, AMD_SPI_SPEED_REG, spd7_val,
				       AMD_SPI_SPD7_MASK);
	}

	return 0;
}

static void amd_spi_setup_v2_regs(struct amd_spi *amd_spi)
{
	/* Bits 30,29,18. Windows never sets bit23 (TXMODE).
	 * TXMODE is only set for opcode 0xB0 firmware writes. */
	amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG,
			       AMD_SPI_READ_MODE_FAST, 0);
}

static int amd_spi_set_opcode(struct amd_spi *amd_spi, u8 cmd_opcode)
{
	switch (amd_spi->version) {
	case AMD_SPI_V2:
		amd_spi_writereg8(amd_spi, AMD_SPI_OPCODE_REG, cmd_opcode);
		return 0;
	default:
		return -ENODEV;
	}
}

static int amd_spi_busy_wait(struct amd_spi *amd_spi)
{
	u32 val;
	int reg;
	int ret;

	/* Poll both CTRL0 (0x00) for busy (bit 31) — the reliable indicator */
	reg = AMD_SPI_CTRL0_REG;  /* 0x00 */
	ret = readl_poll_timeout(amd_spi->io_remap_addr + reg, val,
				  !(val & AMD_SPI_BUSY), 20, 2000000);
	if (ret)
		pr_err("spi-amd: TRACE busy timeout ctrl0=0x%08x status=0x%02x opcode=0x%02x trigger=0x%02x\n",
		       val, amd_spi_readreg8(amd_spi, AMD_SPI_STATUS_REG),
		       amd_spi_readreg8(amd_spi, AMD_SPI_OPCODE_REG),
		       amd_spi_readreg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG));
	return ret;
}

static int amd_spi_execute_opcode(struct amd_spi *amd_spi)
{
	int ret;

	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret)
		return ret;

	ret = amd_spi_busy_wait(amd_spi);
	if (ret)
		return ret;

	/* V2: write 0x80 hardcoded to 0x47 (matches Windows decomp fcn.0x4bac) */
	amd_spi_writereg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG, 0x80);

	return amd_spi_busy_wait(amd_spi);
}

/*
 * Execute a single segment of transfers sharing the same opcode.
 * Returns the number of RX bytes read, or negative error.
 */
static int amd_spi_exec_segment(struct amd_spi *amd_spi, u8 opcode,
				const u8 *tx_data, u32 tx_len,
				u8 *rx_data, u32 rx_len)
{
	void __iomem *base = amd_spi->io_remap_addr;
	u32 fifo_pos = AMD_SPI_FIFO_BASE;
	u16 saved_0x22;
	int i, ret;

	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret)
		return ret;

	if (opcode != 0x0B)
		pr_debug("spi-amd: exec op=0x%02x tx=%u rx=%u\n", opcode, tx_len, rx_len);
	if (debug_trace >= 2)
		pr_info("spi-amd: TRACE segment begin op=0x%02x tx=%u rx=%u ctrl0=0x%08x ena=0x%08x\n",
			opcode, tx_len, rx_len,
			amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG),
			amd_spi_readreg32(amd_spi, AMD_SPI_ENA_REG));

	/* Windows fcn.0x6fc0: save SPI100_SPEED_CONFIG (0x22) before transfer */
	saved_0x22 = readw(base + 0x22);

	/* Clear FIFO — single set (not toggle), matching Windows decomp */
	{
		u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
		ctrl0 |= AMD_SPI_FIFO_CLEAR;
		amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	}

	/* V2: write opcode to 0x45 FIRST (before READ_MODE, as Windows does) */
	ret = amd_spi_set_opcode(amd_spi, opcode);
	if (ret)
		return ret;

	/* V2: set SPI_READ_MODE=FAST_READ (bits 30+29+18=0b111) AFTER opcode */
	amd_spi_setup_v2_regs(amd_spi);

	/* V2: 0x44 speed/opcode sequence used by both supported V2 opcodes.
	 * Windows fcn.0x4bac decomps:
	 *   r44 = read16(0x44)
	 *   r44 = (r44 & 0xF0FF) | (nibble << 8)
	 *   r44 = (r44 & 0x0FFF) | (nibble << 12)
	 *   write16(0x44, r44)
	 * Each write16 clobbers 0x45 — re-write opcode after. */
	if (opcode == 0x02 || opcode == 0x0B) {
		u16 w = amd_spi_readreg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG);
		u8 speed_nibble = amd_spi_readreg8(amd_spi, AMD_SPI_ENA_REG) & 0xF;
		w = (w & 0xF0FF) | ((u16)speed_nibble << 8);
		w = (w & 0x0FFF) | ((u16)speed_nibble << 12);
		amd_spi_writereg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG, w);
		ret = amd_spi_set_opcode(amd_spi, opcode);
		if (ret)
			return ret;
	}

	if (tx_len > AMD_SPI_FIFO_SIZE) {
		pr_err("spi-amd: tx_len %u exceeds FIFO size %u\n", tx_len, AMD_SPI_FIFO_SIZE);
		return -EINVAL;
	}
	if (opcode == 0x0B && tx_len + rx_len + 1 > AMD_SPI_FIFO_SIZE) {
		pr_err("spi-amd: tx(%u) + rx(%u) + echo > FIFO(%u)\n",
		       tx_len, rx_len, AMD_SPI_FIFO_SIZE);
		return -EINVAL;
	}
	writeb(tx_len, base + AMD_SPI_TX_COUNT_REG);

	for (i = 0; i < tx_len; i++)
		writeb(tx_data[i], base + fifo_pos + i);

	/* Windows write path (0x54d0): RX_COUNT=0 for writes — TX-only.
	 * The response arrives via a separate 0x0B read after GPIO IRQ.
	 * For read operations (0x0B), rx_len is set by the caller.
	 * The +1 on rx_len matches Windows PIO read behavior: the controller
	 * always transfers one extra byte (the opcode echo / status byte)
	 * into the FIFO before the actual read data. */

	writeb(opcode == 0x0B ? rx_len + 1 : rx_len,
	       base + AMD_SPI_RX_COUNT_REG);

	/* Windows decomp 0x4bac: re-write opcode after RX_COUNT, just before trigger.
	 * (Needed because the 0x44 speed config writes 16 bits, clobbering 0x45.) */
	if (opcode == 0x02 || opcode == 0x0B) {
		ret = amd_spi_set_opcode(amd_spi, opcode);
		if (ret)
			return ret;
	}

	/* wmb() ensures all MMIO FIFO/TX/RX count writes are visible before
	 * the trigger command. On x86 this is only a compiler barrier (stores
	 * are strongly ordered), but it documents the required ordering for
	 * clarity and serves as a real barrier on weakly-ordered architectures. */
	wmb();
	if (debug_trace >= 1 && opcode == 0x02)
		pr_info("spi-amd: DIAG write pre-trigger tx=%u fifo=[%*ph] ctrl0=0x%08x cs=0x%02x ena=0x%08x speed44=0x%04x op45=0x%02x txc=%u rxc=%u\n",
			tx_len, (int)min_t(u32, tx_len, 16), tx_data,
			amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG),
			amd_spi_readreg32(amd_spi, AMD_SPI_ENA_REG),
			amd_spi_readreg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_OPCODE_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_TX_COUNT_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_RX_COUNT_REG));

	if (opcode == 0x02) {
		u32 c0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
		u32 c1 = readl(base + 0x0C);
		u8 st = readb(base + AMD_SPI_STATUS_REG);
		u8 al = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
		u16 sp = amd_spi_readreg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG);
		u8 op = amd_spi_readreg8(amd_spi, AMD_SPI_OPCODE_REG);
		u8 tr = amd_spi_readreg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG);
		u32 ena = amd_spi_readreg32(amd_spi, AMD_SPI_ENA_REG);
		u8 nib = (u8)(ena & 0xF);
		pr_debug("spi-amd: WRITE PRE-TRIG c0=0x%08x c1=0x%08x st=0x%02x al=0x%02x sp=0x%04x op45=0x%02x tr47=0x%02x ena=0x%08x nib=%u\n",
			c0, c1, st, al, sp, op, tr, ena, nib);
	}

	ret = amd_spi_execute_opcode(amd_spi);
	if (ret) {
		pr_err("spi-amd: TRACE segment execution failed op=0x%02x ret=%d\n",
			       opcode, ret);
		if (!amd_spi_check_psp_ownership(amd_spi))
			writew(saved_0x22, base + 0x22);
		return ret;
	}
	if (debug_trace >= 1 && opcode == 0x02)
		pr_info("spi-amd: DIAG write post-trigger ctrl0=0x%08x status=0x%02x cs=0x%02x op45=0x%02x\n",
			amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_STATUS_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_OPCODE_REG));

	/* execute_opcode polls CTRL0 bit31 after triggering (real busy indicator).
	 * STATUS (0x4C) is an 8-bit register, so bit31 is always 0 —
	 * matching Windows behavior where the STATUS poll is effectively a no-op. */

	if (rx_len) {
		u32 read_off;

		if (opcode == 0x0B)
			read_off = fifo_pos + tx_len + 1;
		else
			read_off = fifo_pos + 4;
		u8 scratch[80];
		u8 *dst = rx_data ? rx_data : scratch;
		u32 rmax = min_t(u32, rx_len, AMD_SPI_FIFO_SIZE);
		for (i = 0; i < rmax; i++)
			dst[i] = readb(base + read_off + i);
		if (debug_trace >= 3)
			pr_info("spi-amd: TRACE segment data op=0x%02x rx=[%*ph]\n",
				opcode, (int)min_t(u32, rmax, 32), dst);
		if (!rx_data && dst == scratch) {
			/* Write with forced RX — discard MISO, restore rx_len */
			rx_len = 0;
		}
	}

	if (debug_trace >= 2)
		pr_info("spi-amd: TRACE segment complete op=0x%02x rx=%u ctrl0=0x%08x status=0x%02x\n",
			opcode, rx_len, amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_STATUS_REG));
	/* Windows fcn.0x6f84: restore SPI100_SPEED_CONFIG (0x22) after transfer.
	 * Skip restore if PSP took ownership post-transfer (data already valid). */
	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret)
		return rx_len;
	writew(saved_0x22, base + 0x22);
	return rx_len;
}

static int amd_spi_host_setup(struct spi_device *spi)
{
	amd_trace(&spi->dev, 1, "host setup begin cs=%u mode=0x%x requested_hz=%u\n",
		  spi_get_chipselect(spi, 0), spi->mode, spi->max_speed_hz);
	/* Frequency programming is deferred until the guarded transfer path. */
	return 0;
}

static int amd_spi_host_transfer(struct spi_controller *host,
				 struct spi_message *msg)
{
	struct amd_spi *amd_spi = spi_controller_get_devdata(host);
	struct spi_device *spi = msg->spi;
	struct spi_transfer *xfer, *next;
	struct device *dev = &spi->dev;
	int ret;

	amd_trace(dev, 2, "message begin frame=%u transfers=%u cs=%u\n",
		  msg->frame_length, msg->actual_length, spi_get_chipselect(spi, 0));
	ret = amd_spi_check_psp_ownership(amd_spi);
	if (ret) {
		msg->status = ret;
		goto finalize;
	}

	ret = amd_set_spi_freq(amd_spi, spi->max_speed_hz);
	if (ret) {
		msg->status = ret;
		goto finalize;
	}

	ret = amd_spi_select_chip(amd_spi, spi_get_chipselect(spi, 0));
	if (ret) {
		msg->status = ret;
		goto finalize;
	}

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		u32 remaining, sent;

		if (xfer->speed_hz) {
			ret = amd_set_spi_freq(amd_spi, xfer->speed_hz);
			if (ret) {
				msg->status = ret;
				goto out;
			}
		}

		if (xfer->tx_buf && xfer->len > 0) {
			u8 *tx_buf = (u8 *)xfer->tx_buf;
			u32 tx_len = xfer->len;
			u8 opcode = tx_buf[0];

			tx_buf++;
			tx_len--;

			/* Strip trailing 0xFF for reads — first RX clock, not TX */
			if (opcode == 0x0B && tx_len > 0 && tx_buf[tx_len - 1] == 0xFF)
				tx_len--;

			/* Windows pattern: TX+RX combined in single opcode transaction
			 * Check if next transfer is an RX-only of matching size */
			if (!list_is_last(&xfer->transfer_list, &msg->transfers)) {
				next = list_next_entry(xfer, transfer_list);
				if (next->rx_buf && next->len > 0 && !next->tx_buf) {
					u8 *rx_ptr = (u8 *)next->rx_buf;
					u32 rx_remaining = next->len;
					u32 tx_sent = 0;
					u32 tx_rem = tx_len;
					/* Keep descriptor reads aligned to 64-byte chunks. */
					u32 first_chunk = min_t(u32, rx_remaining, 64);

					/* Chunk TX if needed (FIFO size is 70 bytes) */
					while (tx_rem > 0) {
						u32 tx_chunk = min_t(u32, tx_rem, AMD_SPI_FIFO_SIZE);
						u32 rx_now = (tx_sent + tx_chunk >= tx_len) ? first_chunk : 0;

						ret = amd_spi_exec_segment(amd_spi, opcode,
							tx_buf + tx_sent, tx_chunk,
							rx_now ? rx_ptr : NULL, rx_now);
						if (ret < 0) { msg->status = ret; goto out; }
						tx_sent += tx_chunk;
						tx_rem -= tx_chunk;
						if (rx_now) {
							rx_ptr += rx_now;
							rx_remaining -= rx_now;
						}
					}

					while (rx_remaining > 0) {
						u32 chunk = min_t(u32, rx_remaining, 64);
						/* Keep each 64-byte PIO continuation in the same
						 * TX_COUNT=3/FIFO+0x84 shape as Windows 0x4bac. */
						ret = amd_spi_exec_segment(amd_spi, opcode,
							tx_buf, tx_len, rx_ptr, chunk);
						if (ret < 0) { msg->status = ret; goto out; }
						rx_ptr += chunk;
						rx_remaining -= chunk;
					}
					xfer = next;
					continue;
				}
			}

			sent = 0;
			remaining = tx_len;
			while (remaining > 0) {
				u32 chunk = remaining > AMD_SPI_FIFO_SIZE ?
					    AMD_SPI_FIFO_SIZE : remaining;
				ret = amd_spi_exec_segment(amd_spi, opcode,
					tx_buf + sent, chunk, NULL, 0);
				if (ret < 0) { msg->status = ret; goto out; }
				sent += chunk;
				remaining -= chunk;
			}
		} else if (xfer->rx_buf && xfer->len > 0) {
			u8 *rx_ptr = (u8 *)xfer->rx_buf;
			for (remaining = xfer->len; remaining > 0; ) {
				u32 chunk = min_t(u32, remaining, 64);
				ret = amd_spi_exec_segment(amd_spi, 0x0B,
					NULL, 0, rx_ptr, chunk);
				if (ret < 0) { msg->status = ret; goto out; }
				rx_ptr += chunk;
				remaining -= chunk;
			}
		}
	}
	msg->status = 0;
	msg->actual_length = msg->frame_length;
out:
	/* Do not write the chip-select register if PSP took ownership mid-message. */
	if (amd_spi_clear_chip(amd_spi, spi_get_chipselect(msg->spi, 0)) &&
	    !msg->status)
		msg->status = -EBUSY;

finalize:
	amd_trace(dev, 2, "message complete status=%d actual=%u ctrl0=0x%08x\n",
		  msg->status, msg->actual_length,
		  amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG));
	spi_finalize_current_message(host);
	return msg->status;
}

static size_t amd_spi_max_transfer_size(struct spi_device *spi)
{
	return 65536; /* chunking handled internally in host_transfer */
}

int amd_spi_probe_common(struct device *dev, struct spi_controller *host)
{
	int err;

	/* The sole logical CS maps to the MSHW0231's physical CS1. */
	host->num_chipselect = 1;
	host->mode_bits = 0;
	host->bits_per_word_mask = SPI_BPW_MASK(8);
	host->flags = SPI_CONTROLLER_HALF_DUPLEX;
	host->setup = amd_spi_host_setup;
	host->transfer_one_message = amd_spi_host_transfer;
	host->max_transfer_size = amd_spi_max_transfer_size;
	host->max_message_size = amd_spi_max_transfer_size;

	err = spi_register_controller(host);
	if (err)
		return dev_err_probe(dev, err, "error registering SPI controller\n");

	return 0;
}
EXPORT_SYMBOL_GPL(amd_spi_probe_common);

static int amd_spi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct spi_controller *host;
	struct amd_spi *amd_spi;
	int ret;

	amd_trace(dev, 1, "probe begin\n");
	host = devm_spi_alloc_host(dev, sizeof(struct amd_spi));
	if (!host)
		return -ENOMEM;

	amd_spi = spi_controller_get_devdata(host);
	amd_spi->io_remap_addr = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(amd_spi->io_remap_addr))
		return dev_err_probe(dev, PTR_ERR(amd_spi->io_remap_addr),
				     "ioremap of SPI registers failed\n");

	dev_info(dev, "spi-amd-v2-multi: io_remap=%p\n", amd_spi->io_remap_addr);
	amd_trace(dev, 1, "probe mapped controller registers\n");

	/* Dump initial CTRL0 value (BIOS/UEFI preset) */
	{
		u32 c0 = readl(amd_spi->io_remap_addr + 0x00);
		u32 c1 = readl(amd_spi->io_remap_addr + 0x0C);
		u32 st = readl(amd_spi->io_remap_addr + 0x4C);
		u16 r44 = readw(amd_spi->io_remap_addr + 0x44);
		u8 r45 = readb(amd_spi->io_remap_addr + 0x45);
		u8 r1d = readb(amd_spi->io_remap_addr + 0x1D);
		u32 ena = readl(amd_spi->io_remap_addr + 0x20);
		dev_info(dev, "BIOS regs: CTRL0=0x%08x CTRL1=0x%08x ENA=0x%08x STATUS=0x%08x 0x44=0x%04x 0x45=0x%02x 0x1D=0x%02x\n",
			 c0, c1, ena, st, r44, r45, r1d);
	}

	amd_spi->version = (uintptr_t)device_get_match_data(dev);
	host->bus_num = 0;

	/* Windows reads SPI100_SPEED_CONFIG as a 16-bit MMIO value. */
	dev_info(dev, "SPI100_SPEED_CONFIG at MMIO+0x22 = 0x%04X\n",
		 ioread16(amd_spi->io_remap_addr + 0x22));

	amd_trace(dev, 1, "probe registering SPI controller version=%u\n", amd_spi->version);
	platform_set_drvdata(pdev, host);
	ret = amd_spi_probe_common(dev, host);
	amd_trace(dev, 1, "probe complete ret=%d\n", ret);
	return ret;
}

#ifdef CONFIG_ACPI
static const struct acpi_device_id spi_acpi_match[] = {
	{ "AMDI0060", AMD_SPI_V2 },  /* Surface Laptop 4 AMD — force V2 */
	{},
};
/*
 * Do not export an ACPI modalias for this experimental controller.  It must
 * only bind after an explicit post-login modprobe of sl4a-spi-amd.
 */
#endif

static void amd_spi_remove(struct platform_device *pdev)
{
	struct spi_controller *host = platform_get_drvdata(pdev);

	if (!host)
		return;
	spi_unregister_controller(host);
	amd_trace(&pdev->dev, 1, "remove entered\n");
}

static struct platform_driver amd_spi_driver = {
	.driver = {
		.name = "sl4a_spi_amd_v2_multi",
		.acpi_match_table = ACPI_PTR(spi_acpi_match),
	},
	.probe = amd_spi_probe,
	.remove = amd_spi_remove,
};

module_platform_driver(amd_spi_driver);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Syax89");
MODULE_DESCRIPTION("AMD SPI V2 Multi-Opcode Driver");
