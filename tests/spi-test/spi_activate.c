/* Minimal test: send activation + B0 firmware via SPI 0x0B workaround */
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/firmware.h>
#include <linux/delay.h>

static struct spi_device *spi_dev;

static int spi_write_cmd(struct spi_device *spi, const u8 *buf, int len)
{
	struct spi_transfer xf = {
		.tx_buf = buf,
		.len = len,
	};
	struct spi_message msg;
	spi_message_init_with_transfers(&msg, &xf, 1);
	return spi_sync(spi, &msg);
}

static int spi_wr_rd(struct spi_device *spi, const u8 *tx, int tx_len, u8 *rx, int rx_len)
{
	struct spi_transfer xf[2] = {
		{ .tx_buf = tx, .len = tx_len },
		{ .rx_buf = rx, .len = rx_len },
	};
	struct spi_message msg;
	spi_message_init(&msg);
	spi_message_add_tail(&xf[0], &msg);
	spi_message_add_tail(&xf[1], &msg);
	return spi_sync(spi, &msg);
}

static int spi_read_approval(struct spi_device *spi, u32 reg, u8 byte7, u8 *rx, int rx_len)
{
	u8 tx[9];
	tx[0] = 0x0B;
	tx[1] = (reg >> 16) & 0xff;
	tx[2] = (reg >> 8) & 0xff;
	tx[3] = reg & 0xff;
	tx[4] = 0xFF;
	tx[5] = 0x00;
	tx[6] = 0x00;
	tx[7] = byte7;
	tx[8] = 0x00;
	return spi_wr_rd(spi, tx, 9, rx, rx_len);
}

static int test_activate(void)
{
	struct device *dev = &spi_dev->dev;
	const struct firmware *fw = NULL;
	u8 rx[24];
	int ret, i;

	dev_info(dev, "=== ACTIVATION TEST START ===\n");

	/* Step 1: read RESET_RSP header */
	ret = spi_read_approval(spi_dev, 0x000000, 0x00, rx, 24);
	dev_info(dev, "STEP1 read: ret=%d rx[0..15]=[%*ph]\n", ret, 16, rx);

	/* Step 2: activation cmd1 via read register 0x0E0000 */
	ret = spi_read_approval(spi_dev, 0x0E0000, 0x03, rx, 0);
	dev_info(dev, "STEP2 cmd1 (read 0x0E0000): %d\n", ret);
	usleep_range(1000, 1500);

	/* Step 3: cmd2 write 0x00 */
	{
		u8 c = 0x00;
		ret = spi_write_cmd(spi_dev, &c, 1);
		dev_info(dev, "STEP3 cmd2 (write 00): %d\n", ret);
	}
	usleep_range(1500, 2000);

	/* Step 4: read ACK */
	ret = spi_read_approval(spi_dev, 0x000000, 0x03, rx, 16);
	dev_info(dev, "STEP4 ACK[16]=[%*ph] ret=%d\n", 16, rx, ret);
	usleep_range(25000, 30000);

	/* Step 5: cmd3 0x70 write+read */
	{
		u8 c = 0x70;
		ret = spi_wr_rd(spi_dev, &c, 1, rx, 14);
		dev_info(dev, "STEP5 cmd3 (70) rx=[%*ph] ret=%d\n", 14, rx, ret);
	}

	/* Step 6: read header after activation */
	usleep_range(50000, 60000);
	ret = spi_read_approval(spi_dev, 0x000000, 0x03, rx, 24);
	dev_info(dev, "STEP6 post-activation read: rx=[%*ph]\n", 24, rx);

	/* Step 7: send B0 firmware blocks */
	ret = request_firmware(&fw, "mshw0231_b0.bin", dev);
	if (ret || !fw) {
		dev_warn(dev, "STEP7 firmware not found (%d)\n", ret);
	} else {
		int nblocks = fw->size / 241;
		dev_info(dev, "STEP7 sending %d B0 blocks (%zu bytes)\n", nblocks, fw->size);
		for (i = 0; i < nblocks && i < 3; i++) {
			ret = spi_write_cmd(spi_dev, &fw->data[i * 241], 241);
			dev_info(dev, "  B0[%d] -> %d\n", i + 1, ret);
			usleep_range(3500, 4000);
		}
		dev_info(dev, "STEP7 done (%d blocks sent)\n", i);
		release_firmware(fw);
	}

	/* Step 8: read header after B0 */
	usleep_range(50000, 60000);
	ret = spi_read_approval(spi_dev, 0x000000, 0x03, rx, 24);
	dev_info(dev, "STEP8 post-B0 read: rx=[%*ph]\n", 24, rx);

	dev_info(dev, "=== ACTIVATION TEST DONE ===\n");
	return 0;
}

static int spi_activate_probe(struct spi_device *spi)
{
	spi_dev = spi;
	dev_info(&spi->dev, "spi_activate: probed, running test\n");
	test_activate();
	return 0;
}

static void spi_activate_remove(struct spi_device *spi)
{
	dev_info(&spi->dev, "spi_activate: removed\n");
}

static const struct spi_device_id spi_activate_id[] = {
	{ "spi_activate", 0 },
	{ }
};
MODULE_DEVICE_TABLE(spi, spi_activate_id);

static struct spi_driver spi_activate_driver = {
	.driver = { .name = "spi_activate" },
	.probe = spi_activate_probe,
	.remove = spi_activate_remove,
	.id_table = spi_activate_id,
};
module_spi_driver(spi_activate_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SPI activation test for MSHW0231");

/* ACPI match for MSHW0231 */
static const struct acpi_device_id spi_activate_acpi_match[] = {
	{ "MSHW0231", 0 },
	{ }
};
MODULE_DEVICE_TABLE(acpi, spi_activate_acpi_match);

/* Update driver to include ACPI match */
#undef module_spi_driver
