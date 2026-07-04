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

static inline u8 amd_spi_readreg8(struct amd_spi *amd_spi, int idx)
{
	return readb((u8 __iomem *)amd_spi->io_remap_addr + idx);
}

static inline void amd_spi_writereg8(struct amd_spi *amd_spi, int idx, u8 val)
{
	writeb(val, ((u8 __iomem *)amd_spi->io_remap_addr + idx));
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
	amd_spi_setclear_reg8(amd_spi, AMD_SPI_ALT_CS_REG, cs, AMD_SPI_ALT_CS_MASK);
}

static inline void amd_spi_clear_chip(struct amd_spi *amd_spi, u8 cs)
{
	u8 tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
	tmp &= ~AMD_SPI_ALT_CS_MASK;
	amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);
}

static void amd_spi_clear_fifo_ptr(struct amd_spi *amd_spi)
{
	amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG, AMD_SPI_FIFO_CLEAR, AMD_SPI_FIFO_CLEAR);
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
	/* Secret bits from decomp: 0x60040000 (bits 30, 29, 18).
	 * Do NOT set bit23 (TXMODE) — Windows never sets it. */
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

	switch (amd_spi->version) {
	case AMD_SPI_V1:
		reg = AMD_SPI_CTRL0_REG;
		break;
	case AMD_SPI_V2:
		/* On V1 hardware forced to V2, STATUS_REG(0x4C) is unreliable.
		 * Use CTRL0 busy bit instead, plus a fixed delay fallback. */
		reg = AMD_SPI_CTRL0_REG;
		break;
	default:
		return -ENODEV;
	}

	return readl_poll_timeout(amd_spi->io_remap_addr + reg, val,
				  !(val & AMD_SPI_BUSY), 20, 2000000);
}

static int amd_spi_execute_opcode(struct amd_spi *amd_spi)
{
	int ret;

	ret = amd_spi_busy_wait(amd_spi);
	if (ret)
		return ret;

	switch (amd_spi->version) {
	case AMD_SPI_V1:
		amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG, AMD_SPI_EXEC_CMD, AMD_SPI_EXEC_CMD);
		return 0;
	case AMD_SPI_V2:
		amd_spi_setclear_reg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG, AMD_SPI_TRIGGER_CMD, AMD_SPI_TRIGGER_CMD);
		return 0;
	default:
		return -ENODEV;
	}
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
	int i, ret;
	u16 old_reg_prefix = 0, old_reg44 = 0;

	pr_info("spi-amd-multi: seg op=0x%02x tx=%u rx=%u\n", opcode, tx_len, rx_len);

	/* Save the reg_prefix and 0x44 registers before the transaction */
	if (amd_spi->version == AMD_SPI_V2) {
		old_reg_prefix = readw(base + 0x22);
		old_reg44 = readw(base + 0x44);
	}

	/* Windows amdspi.sys order (fcn.0x54d0 / fcn.0x4bac):
	 * 1. Set reg 0x1D bit 0 = 1 (AND 0xFC, OR 1)
	 * 2. Clear FIFO (CTRL0 bit 20)
	 * 3. Write opcode to 0x45 (first time — hardware updates CTRL0 bits 15-8)
	 * 4. Read CTRL0, set bits 30/29/18/23, clear bit 21, write CTRL0
	 * 5. Speed config (reg 0x44, overwrites 0x45)
	 * 6. TX_COUNT (reg 0x48)
	 * 7. Fill FIFO
	 * 8. RX_COUNT (reg 0x4B) — 0 for writes, rx_len+1 for reads
	 * 9. Write opcode to 0x45 again (after speed config overwrote it)
	 * 10. Trigger (reg 0x47 = 0x80)
	 */

	/* 1. Set reg 0x1D bit 0 = 1.
	 * Windows amdspi.sys decomp (fcn.0x3c20, 0x4bac, 0x54d0):
	 *   read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
	 * Only bit 0 is set. DO NOT set bit 7 — that was a DKMS
	 * artifact that may disable write mode. */
	if (amd_spi->version == AMD_SPI_V2) {
		u8 reg1d = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
		reg1d = (reg1d & 0xFC) | 0x01;  /* match Windows: only bit 0 */
		amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, reg1d);
	}

	/* 2. Clear FIFO (CTRL0 bit 20) */
	amd_spi_clear_fifo_ptr(amd_spi);

	/* 3. Write opcode to 0x45 (first time — lets HW update CTRL0 bits 15-8) */
	amd_spi_set_opcode(amd_spi, opcode);

	/* 4. Write CTRL0 secret bits NOW (before FIFO fill, matching Windows).
	 * Bits 30, 29, 18 only. Do NOT set bit 23 (TXMODE) or clear bit 21
	 * (Windows preserves both from hardware defaults). */
	if (amd_spi->version == AMD_SPI_V2) {
		u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
	ctrl0 |= 0x60040000;   /* bits 30, 29, 18 */
	ctrl0 |= 0x00800000;   /* bit 23: TXMODE — force MOSI drive (CRITICAL!) */
	amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	}

	/* 5. Speed config (reg 0x44 — spans 0x44-0x45, overwrites opcode!)
	 * Use same speed for all opcodes. Reg 0x45 gets fixed to 0x0B after. */
	if (amd_spi->version == AMD_SPI_V2) {
		u16 r44 = readw(base + 0x44);
		u8 nibble = amd_spi->speed_hz_index & 0x0F;

		r44 &= 0xF0FF;
		r44 |= (nibble << 8);
		r44 &= 0x0FFF;
		r44 |= (nibble << 12);
		writew(r44, base + 0x44);
	}

	/* 6. TX_COUNT */
	writeb(tx_len, base + AMD_SPI_TX_COUNT_REG);

	/* 7. Fill FIFO (only if tx_len > 0) */
	for (i = 0; i < tx_len; i++)
		writeb(tx_data[i], base + fifo_pos + i);

	/* 8. RX_COUNT — rx_len for full-duplex. CTRL0 bits 15-12
	 * scale with RX count: 0→0x0, 10→0xA, 14→0xE(?) */
	if (amd_spi->version == AMD_SPI_V2) {
		u32 rx_cnt = (opcode == 0x0B && rx_len) ? rx_len + 1 : rx_len;
		writeb(rx_cnt, base + AMD_SPI_RX_COUNT_REG);
	} else {
		writeb(rx_len, base + AMD_SPI_RX_COUNT_REG);
	}

	/* 9. Write opcode to 0x45 again (speed config overwrote it) */
	amd_spi_set_opcode(amd_spi, opcode);

	/* 10. CTRL0 bits 15-8 are set automatically by RX_COUNT.
	 * Restore secret bits (speed config may have cleared them). */
	if (amd_spi->version == AMD_SPI_V2) {
		u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
	ctrl0 |= 0x60040000;   /* bits 30, 29, 18 */
	ctrl0 |= 0x00800000;   /* bit 23: TXMODE — force MOSI drive (CRITICAL!) */
	amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
	}
	{
		u8 fdump[16];
		int k;

		for (k = 0; k < 16; k++)
			fdump[k] = readb(base + AMD_SPI_FIFO_BASE + k);
		pr_info("spi-amd-TRACE pre-trig: op=%02x CTRL0=%08x ENA=%08x SPD=%08x 1D=%02x 44=%04x 45=%02x 47=%02x 48=%02x 4B=%02x 4C=%02x FIFO=%*ph\n",
			opcode,
			amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG),
			amd_spi_readreg32(amd_spi, AMD_SPI_ENA_REG),
			amd_spi_readreg32(amd_spi, AMD_SPI_SPEED_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG),
			readw(base + 0x44),
			amd_spi_readreg8(amd_spi, AMD_SPI_OPCODE_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_CMD_TRIGGER_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_TX_COUNT_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_RX_COUNT_REG),
			amd_spi_readreg8(amd_spi, AMD_SPI_STATUS_REG),
			16, fdump);
	}

	/* Memory barrier: ensure all MMIO writes (FIFO, CTRL0, counts)
	 * reach the controller before we trigger the transfer.
	 * Windows uses lfence at this point. */
	wmb();

	ret = amd_spi_execute_opcode(amd_spi);
	if (ret)
		return ret;

	ret = amd_spi_busy_wait(amd_spi);
	if (ret)
		return ret;

	{
		u8 fdump[24];
		int k;

		for (k = 0; k < 24; k++)
			fdump[k] = readb(base + AMD_SPI_FIFO_BASE + k);
		pr_info("spi-amd-TRACE post:  47=%02x 4C=%02x 22=%04x FIFO=%*ph\n",
			amd_spi_readreg8(amd_spi, 0x47),
			amd_spi_readreg8(amd_spi, 0x4C),
			readw(base + 0x22),
			24, fdump);
	}

	/*
	 * Windows amdspi.sys fcn.0x6f84 (restore_register_prefix):
	 * writes dev_ctx[0x58] back to MMIO+0x22 via write16 after
	 * every transfer. Also restores reg 0x44 (speed/opcode).
	 */
	if (amd_spi->version == AMD_SPI_V2) {
		writew(old_reg_prefix, base + 0x22);
		writew(old_reg44, base + 0x44);
	}

	if (rx_len) {
		u32 read_off = fifo_pos + tx_len;

		for (i = 0; i < rx_len && i < 64; i++)
			rx_data[i] = readb(base + read_off + i);

		pr_info("spi-amd-multi: rx[0..15]=[%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x]\n",
			rx_data[0],rx_data[1],rx_data[2],rx_data[3],
			rx_data[4],rx_data[5],rx_data[6],rx_data[7],
			rx_data[8],rx_data[9],rx_data[10],rx_data[11],
			rx_data[12],rx_data[13],rx_data[14],rx_data[15]);
	}

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

	amd_spi_select_chip(amd_spi, spi_get_chipselect(spi, 0));

	/* V2 register setup (secret bits, reg44, strobe, 0x1D) */
	if (amd_spi->version == AMD_SPI_V2)
		amd_spi_setup_v2_regs(amd_spi);

	return amd_spi_fifo_xfer(amd_spi, host, msg);
}

static size_t amd_spi_max_transfer_size(struct spi_device *spi)
{
	return AMD_SPI_FIFO_SIZE;
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
                                       