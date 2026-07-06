/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
/*
 * AMD SPI controller driver — multi-opcode variant
 * Based on upstream v6.15 spi-amd.c
 * Added: AMDI0060 support, multi-opcode intra-message switching
 */

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/io-64-nonatomic-lo-hi.h>
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

/* SPI speed control registers (from upstream spi-amd) */
#define AMD_SPI_ENA_REG		0x20
#define AMD_SPI_ALT_SPD_SHIFT	20
#define AMD_SPI_ALT_SPD_MASK	GENMASK(23, AMD_SPI_ALT_SPD_SHIFT)
#define AMD_SPI_SPI100_SHIFT	0
#define AMD_SPI_SPI100_MASK	GENMASK(AMD_SPI_SPI100_SHIFT, AMD_SPI_SPI100_SHIFT)
#define AMD_SPI_SPEED_REG	0x6C
#define AMD_SPI_SPD7_SHIFT	8
#define AMD_SPI_SPD7_MASK	GENMASK(13, AMD_SPI_SPD7_SHIFT)

/* Enum for speed register values */
enum amd_spi_speed_val {
	F_66_66MHz,
	F_33_33MHz,
	F_22_22MHz,
	F_16_66MHz,
	F_100MHz,
	F_800KHz,
	SPI_SPD7 = 0x7,
	F_50MHz,
	F_4MHz,
	F_3_17MHz,
};

struct amd_spi_freq {
	u32 speed_hz;
	enum amd_spi_speed_val enable_val;
	u8 spd7_val;
};

/* Undocumented registers from amdspi.sys decomp */
#define AMD_SPI_SECRET_BITS	0x60040000 /* bits 30+29+18 */
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

static void amd_spi_setclear_reg8(struct amd_spi *amd_spi, int idx, u8 set, u8 clear)
{
	u8 tmp = amd_spi_readreg8(amd_spi, idx);
	tmp = (tmp & ~clear) | set;
	amd_spi_writereg8(amd_spi, idx, tmp);
}

static void amd_spi_setclear_reg32(struct amd_spi *amd_spi, int idx, u32 set, u32 clear)
{
	u32 tmp = amd_spi_readreg32(amd_spi, idx);
	tmp = (tmp & ~clear) | set;
	amd_spi_writereg32(amd_spi, idx, tmp);
}

static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs)
{
	u8 tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
	tmp &= ~AMD_SPI_ALT_CS_MASK;
	/* Windows decomp always uses CS1 (0x01) — hardcode it */
	tmp |= 0x01;
	amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);
}

static inline void amd_spi_clear_chip(struct amd_spi *amd_spi, u8 cs)
{
	u8 tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
	tmp &= ~AMD_SPI_ALT_CS_MASK;
	amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);
}

static void amd_spi_clear_fifo_ptr(struct amd_spi *amd_spi)
{
	u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
	ctrl0 &= ~AMD_SPI_FIFO_CLEAR;
	amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	ctrl0 |= AMD_SPI_FIFO_CLEAR;
	amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
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

static void amd_set_spi_freq(struct amd_spi *amd_spi, u32 speed_hz)
{
	unsigned int i, spd7_val, alt_spd;

	for (i = 0; i < ARRAY_SIZE(amd_spi_freq) - 1; i++)
		if (speed_hz >= amd_spi_freq[i].speed_hz)
			break;

	if (amd_spi->speed_hz == amd_spi_freq[i].speed_hz)
		return;

	amd_spi->speed_hz = amd_spi_freq[i].speed_hz;
	amd_spi->speed_hz_index = i;

	alt_spd = (amd_spi_freq[i].enable_val << AMD_SPI_ALT_SPD_SHIFT)
		   & AMD_SPI_ALT_SPD_MASK;
	amd_spi_setclear_reg32(amd_spi, AMD_SPI_ENA_REG, alt_spd,
			       AMD_SPI_ALT_SPD_MASK);

	if (amd_spi->speed_hz == 100000000)
		amd_spi_setclear_reg32(amd_spi, AMD_SPI_ENA_REG, 1,
				       AMD_SPI_SPI100_MASK);

	if (amd_spi_freq[i].spd7_val) {
		spd7_val = (amd_spi_freq[i].spd7_val << AMD_SPI_SPD7_SHIFT)
			    & AMD_SPI_SPD7_MASK;
		amd_spi_setclear_reg32(amd_spi, AMD_SPI_SPEED_REG, spd7_val,
				       AMD_SPI_SPD7_MASK);
	}
}

static void amd_spi_setup_v2_regs(struct amd_spi *amd_spi)
{
	/* Bits 30,29,18. Windows never sets bit23 (TXMODE).
	 * TXMODE is only set for opcode 0xB0 firmware writes. */
	amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG,
			       AMD_SPI_SECRET_BITS, 0);
}

static int amd_spi_set_opcode(struct amd_spi *amd_spi, u8 cmd_opcode)
{
	switch (amd_spi->version) {
	case AMD_SPI_V1:
		amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG, cmd_opcode, AMD_SPI_OPCODE_MASK);
		return 0;
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

	/* Poll both CTRL0 (0x00) for busy (bit 31) — the reliable indicator */
	reg = AMD_SPI_CTRL0_REG;  /* 0x00 */
	return readl_poll_timeout(amd_spi->io_remap_addr + reg, val,
				  !(val & AMD_SPI_BUSY), 20, 2000000);
}

static int amd_spi_execute_opcode(struct amd_spi *amd_spi)
{
	int ret;

	ret = amd_spi_busy_wait(amd_spi);
	if (ret)
		return ret;

	/* V2: toggle trigger to preserve other bits in 0x47 */
	if (amd_spi->version == AMD_SPI_V2) {
		u8 trig = amd_spi_readreg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG);
		trig &= ~AMD_SPI_TRIGGER_CMD;
		amd_spi_writereg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG, trig);
		trig |= AMD_SPI_TRIGGER_CMD;
		amd_spi_writereg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG, trig);
	} else {
		u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
		ctrl0 &= ~AMD_SPI_EXEC_CMD;
		amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
		ctrl0 |= AMD_SPI_EXEC_CMD;
		amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	}
	return 0;
}

/*
 * Execute a single segment of transfers sharing the same opcode.
 * Returns the number of RX bytes read, or negative error.
 */
#define AMD_SPI_DEBUG_TRACE 0

#define DBG_VERBOSE 0

static int amd_spi_exec_segment(struct amd_spi *amd_spi, u8 opcode,
				const u8 *tx_data, u32 tx_len,
				u8 *rx_data, u32 rx_len)
{
	void __iomem *base = amd_spi->io_remap_addr;
	u32 fifo_pos = AMD_SPI_FIFO_BASE;
	u16 saved_0x22;
	int i, ret;

	if (DBG_VERBOSE || opcode != 0x0B)
		pr_err("spi-amd: exec op=0x%02x tx=%u rx=%u\n", opcode, tx_len, rx_len);

	/* Windows fcn.0x6fc0: save register 0x22 before transfer */
	saved_0x22 = readw(base + 0x22);

	/* Clear FIFO — single set (not toggle), matching Windows decomp */
	if (amd_spi->version == AMD_SPI_V2) {
		u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
		ctrl0 |= AMD_SPI_FIFO_CLEAR;
		amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	}

	/* V2: write opcode to 0x45 FIRST (before secret bits, as Windows does) */
	if (amd_spi->version == AMD_SPI_V2)
		amd_spi_set_opcode(amd_spi, opcode);
	else {
		u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
		ctrl0 = (ctrl0 & ~0xFF) | opcode;
		amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	}

	/* V2: set secret bits (30+29+18) AFTER opcode, as Windows fcn.0x2be4 */
	if (amd_spi->version == AMD_SPI_V2)
		amd_spi_setup_v2_regs(amd_spi);

	/* V2: 0x44 dance — only for writes (opcode 0x02), as Windows does.
	 * Windows fcn.0x4bac does TWO separate writew to 0x44:
	 * first: r44 = (r44 & 0xF0FF) | (nibble << 8)
	 * second: r44 = (r44 & 0x0FFF) | (nibble << 12)
	 * Each writew to 0x44 clobbers 0x45 — latch side-effect may differ. */
	if (amd_spi->version == AMD_SPI_V2 && opcode == 0x02) {
		u16 w = amd_spi_readreg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG);
		u8 speed_nibble = amd_spi_readreg8(amd_spi, AMD_SPI_ENA_REG) & 0xF;
		/* First write: insert nibble at bits 11:8, mask 0xF0FF */
		w = (w & 0xF0FF) | ((u16)speed_nibble << 8);
		amd_spi_writereg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG, w);
		amd_spi_set_opcode(amd_spi, opcode);  /* re-write after 0x44 clobber */
		/* Second write: insert nibble at bits 15:12, mask 0x0FFF */
		w = (w & 0x0FFF) | ((u16)speed_nibble << 12);
		amd_spi_writereg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG, w);
		amd_spi_set_opcode(amd_spi, opcode);
	}

	if (tx_len > AMD_SPI_FIFO_SIZE) {
		pr_err("spi-amd: tx_len %u exceeds FIFO size %u\n", tx_len, AMD_SPI_FIFO_SIZE);
		return -EINVAL;
	}
	writeb(tx_len, base + AMD_SPI_TX_COUNT_REG);

	for (i = 0; i < tx_len; i++)
		writeb(tx_data[i], base + fifo_pos + i);

	writeb(rx_len, base + AMD_SPI_RX_COUNT_REG);

	/* Re-apply secret bits after FIFO fill (v2-multi approach).
	 * The 0x44 dance or FIFO operations may have cleared them. */
	if (amd_spi->version == AMD_SPI_V2)
		amd_spi_setup_v2_regs(amd_spi);

	wmb();

	if (opcode == 0x02) {
		u32 c0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
		u32 st = readl(base + AMD_SPI_STATUS_REG);
		u8 al = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
		u16 sp = amd_spi_readreg16(amd_spi, AMD_SPI_SPEED_CONFIG_REG);
		u8 op = amd_spi_readreg8(amd_spi, AMD_SPI_OPCODE_REG);
		u8 tr = amd_spi_readreg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG);
		pr_err("spi-amd: WRITE PRE-TRIG c0=0x%08x st=0x%08x al=0x%02x sp=0x%04x op45=0x%02x tr47=0x%02x\n",
			c0, st, al, sp, op, tr);
	}

	ret = amd_spi_execute_opcode(amd_spi);
	if (ret) { pr_err("spi-amd: execute_opcode failed %d\n", ret); writew(saved_0x22, base + 0x22); return ret; }

	ret = amd_spi_busy_wait(amd_spi);
	if (ret) { pr_err("spi-amd: busy_wait timeout %d\n", ret); writew(saved_0x22, base + 0x22); return ret; }

	{
		u32 st = readl(base + AMD_SPI_STATUS_REG);
		if (opcode == 0x02)
			pr_err("spi-amd: WRITE DONE status=0x%08x\n", st);
	}

	if (rx_len) {
		u32 read_off = fifo_pos + tx_len;
		for (i = 0; i < rx_len && i < AMD_SPI_FIFO_SIZE; i++)
			rx_data[i] = readb(base + read_off + i);
		pr_err("spi-amd: RX[0..%u]=[%*ph]\n",
			(u32)min_t(u32, rx_len, 16), (int)min_t(u32, rx_len, 16), rx_data);
	}

	if (DBG_VERBOSE) pr_err("spi-amd: done\n");
	/* Windows fcn.0x6f84: restore register 0x22 after transfer */
	writew(saved_0x22, base + 0x22);
	/* Windows fcn.0x4684: restore original opcode to 0x45 after transfer */
	writeb(0x0B, base + AMD_SPI_OPCODE_REG);
	return rx_len;
}

/*
 * Multi-opcode FIFO transfer: detects opcode changes between transfers
 * and executes them as separate CS-held segments.
 */
static int amd_spi_fifo_xfer(struct amd_spi *amd_spi,
			     struct spi_controller *host,
			     struct spi_message *message)
{
	struct spi_transfer *xfer;
	u8 *tx_bufs[16]; u32 tx_lens[16];
	u8 *rx_bufs[16]; u32 rx_lens[16];
	u8 opcodes[16];
	int seg_count = 0;
	int total_tx = 0, total_rx = 0;
	int i, ret;

	dev_info(&message->spi->dev, "spi-amd-multi: fifo_xfer ENTER\n");

	/* Apply per-transfer speed if specified */
	list_for_each_entry(xfer, &message->transfers, transfer_list) {
		if (xfer->speed_hz) {
			amd_set_spi_freq(amd_spi, xfer->speed_hz);
			break;
		}
	}

	/* Pass 1: collect segments by scanning transfers for opcode changes */
	{
		u8 current_opcode = 0;
		bool has_opcode = false;

		tx_bufs[0] = NULL; tx_lens[0] = 0;
		rx_bufs[0] = NULL; rx_lens[0] = 0;

		list_for_each_entry(xfer, &message->transfers, transfer_list) {
			u8 *buf = (u8 *)xfer->tx_buf;
			u32 len;

			if (xfer->tx_buf && xfer->len > 0) {
				/* Each TX transfer starts with its own opcode byte */
				if (!has_opcode) {
					current_opcode = buf[0];
					has_opcode = true;
				} else if (buf[0] != current_opcode) {
					/* Opcode changed: start new segment */
					seg_count++;
					tx_bufs[seg_count] = NULL;
					tx_lens[seg_count] = 0;
					rx_bufs[seg_count] = NULL;
					rx_lens[seg_count] = 0;
					current_opcode = buf[0];
				}

				opcodes[seg_count] = current_opcode;
				len = xfer->len - 1; /* strip opcode byte */
				buf++;

				if (!tx_bufs[seg_count])
					tx_bufs[seg_count] = buf;
				tx_lens[seg_count] += len;
				total_tx += len;
			}

			if (xfer->rx_buf && xfer->len > 0) {
				if (!rx_bufs[seg_count])
					rx_bufs[seg_count] = (u8 *)xfer->rx_buf;
				rx_lens[seg_count] += xfer->len;
				total_rx += xfer->len;
			}
		}
	}

	if (seg_count < 0 && total_tx == 0 && total_rx == 0) {
		dev_info(&message->spi->dev, "spi-amd-multi: no transfers to execute\n");
		message->status = -EINVAL;
		goto fin_msg;
	}

	dev_info(&message->spi->dev, "spi-amd-multi: %d segments detected (total_tx=%u total_rx=%u)\n",
		 seg_count + 1, total_tx, total_rx);

	/* Pass 2: execute each segment */
	for (i = 0; i <= seg_count; i++) {
		u8 opcode = opcodes[i];
		u32 tx_len = tx_lens[i];
		u32 rx_len = rx_lens[i];
		u8 *tx_data = tx_bufs[i];
		u8 *rx_data = rx_bufs[i];

		dev_info(&message->spi->dev, "spi-amd-multi: seg[%d] op=0x%02x tx=%u rx=%u\n",
			 i, opcode, tx_len, rx_len);

		ret = amd_spi_exec_segment(amd_spi, opcode,
					   tx_data, tx_len,
					   rx_data, rx_len);
		if (ret < 0) {
			message->status = ret;
			goto fin_msg;
		}
	}

	message->actual_length = total_tx + total_rx + seg_count + 1;
	message->status = 0;

fin_msg:
	if (amd_spi->version == AMD_SPI_V2)
		amd_spi_clear_chip(amd_spi, spi_get_chipselect(message->spi, 0));

	spi_finalize_current_message(host);
	return message->status;
}

static int amd_spi_host_setup(struct spi_device *spi)
{
	struct amd_spi *amd_spi = spi_controller_get_devdata(spi->controller);

	amd_spi_clear_fifo_ptr(amd_spi);
	amd_set_spi_freq(amd_spi, spi->max_speed_hz);
	dev_info(&spi->dev, "spi-amd-v2-multi: set speed to %u Hz\n",
		 amd_spi->speed_hz);
	return 0;
}

static int amd_spi_host_transfer(struct spi_controller *host,
				 struct spi_message *msg)
{
	struct amd_spi *amd_spi = spi_controller_get_devdata(host);
	struct spi_device *spi = msg->spi;
	struct spi_transfer *xfer, *next;

	amd_spi_select_chip(amd_spi, spi_get_chipselect(spi, 0));

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		u32 remaining, sent;
		int ret;

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
					ret = amd_spi_exec_segment(amd_spi, opcode,
						tx_buf, tx_len,
						(u8 *)next->rx_buf, next->len);
					if (ret < 0) { msg->status = ret; goto out; }
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
				u32 chunk = remaining > AMD_SPI_FIFO_SIZE ?
					    AMD_SPI_FIFO_SIZE : remaining;
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
	amd_spi_clear_chip(amd_spi, spi_get_chipselect(msg->spi, 0));
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

	host->num_chipselect = 4;
	host->mode_bits = 0;
	host->flags = 0;
	host->setup = amd_spi_host_setup;
	host->transfer_one_message = amd_spi_host_transfer;
	host->max_transfer_size = amd_spi_max_transfer_size;
	host->max_message_size = amd_spi_max_transfer_size;

	err = devm_spi_register_controller(dev, host);
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

	host = devm_spi_alloc_host(dev, sizeof(struct amd_spi));
	if (!host)
		return -ENOMEM;

	amd_spi = spi_controller_get_devdata(host);
	amd_spi->io_remap_addr = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(amd_spi->io_remap_addr))
		return dev_err_probe(dev, PTR_ERR(amd_spi->io_remap_addr),
				     "ioremap of SPI registers failed\n");

	dev_info(dev, "spi-amd-v2-multi: io_remap=%p\n", amd_spi->io_remap_addr);

	/* Dump initial CTRL0 value (BIOS/UEFI preset) */
	{
		u32 c0 = readl(amd_spi->io_remap_addr + 0x00);
		u32 st = readl(amd_spi->io_remap_addr + 0x4C);
		u16 r44 = readw(amd_spi->io_remap_addr + 0x44);
		u8 r45 = readb(amd_spi->io_remap_addr + 0x45);
		u8 r1d = readb(amd_spi->io_remap_addr + 0x1D);
		dev_info(dev, "BIOS regs: CTRL0=0x%08x STATUS=0x%08x 0x44=0x%04x 0x45=0x%02x 0x1D=0x%02x\n",
			 c0, st, r44, r45, r1d);
	}

	amd_spi->version = (uintptr_t)device_get_match_data(dev);
	host->bus_num = 0;

	/* Read register prefix from MMIO+0x22 (amdspi.sys decomp: fcn.0x6fc0)
	 * Windows reads 16-bit via fcn.0x1400019e0 (read16), not 8-bit.
	 */
	amd_spi->reg_prefix = ioread16(amd_spi->io_remap_addr + 0x22);
	dev_info(dev, "reg_prefix at MMIO+0x22 = 0x%04X\n", amd_spi->reg_prefix);

	return amd_spi_probe_common(dev, host);
}

#ifdef CONFIG_ACPI
static const struct acpi_device_id spi_acpi_match[] = {
	{ "AMDI0060", AMD_SPI_V2 },  /* Surface Laptop 4 AMD — force V2 */
	{ "AMDI0061", AMD_SPI_V1 },
	{ "AMDI0062", AMD_SPI_V2 },
	{},
};
MODULE_DEVICE_TABLE(acpi, spi_acpi_match);
#endif

static struct platform_driver amd_spi_driver = {
	.driver = {
		.name = "spi_amd_v2_multi",
		.acpi_match_table = ACPI_PTR(spi_acpi_match),
	},
	.probe = amd_spi_probe,
};

module_platform_driver(amd_spi_driver);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("AMD SPI V2 Multi-Opcode Driver");
