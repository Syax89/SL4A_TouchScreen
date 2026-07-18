/*
 * D1+D2 hardware tests for AMD FCH SPI (AMDI0060) — Surface Laptop 4 AMD.
 * Replicates the exact register sequence of amdspi.sys fcn.0x3c20 small-read
 * path (see ~/Scrivania/PIANO-VERIFICA.md sez. 0.1) and probes FIFO write
 * semantics (fixed-address vs linear).
 *
 * Load with spi_hid unloaded. Logs everything to dmesg, returns -ENODEV so
 * it never stays loaded.
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/acpi.h>

#define FCH_MMIO	0xFEC10000
#define FCH_SIZE	0x100

static void dump_regs(void __iomem *b, const char *tag)
{
	pr_info("fch_test: [%s] CTRL0=0x%08x STAT04=0x%08x 1D=%02x 20-23=[%02x %02x %02x %02x] 44-4F=[%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x]\n",
		tag, ioread32(b + 0x00), ioread32(b + 0x04), ioread8(b + 0x1D),
		ioread8(b + 0x20), ioread8(b + 0x21), ioread8(b + 0x22), ioread8(b + 0x23),
		ioread8(b + 0x44), ioread8(b + 0x45), ioread8(b + 0x46), ioread8(b + 0x47),
		ioread8(b + 0x48), ioread8(b + 0x49), ioread8(b + 0x4A), ioread8(b + 0x4B),
		ioread8(b + 0x4C), ioread8(b + 0x4D), ioread8(b + 0x4E), ioread8(b + 0x4F));
}

static void dump_fifo(void __iomem *b, const char *tag, int n)
{
	u8 f[32];
	int i;

	n = min(n, 32);
	for (i = 0; i < n; i++)
		f[i] = ioread8(b + 0x80 + i);
	pr_info("fch_test: [%s] FIFO+0..+%d: %*ph\n", tag, n - 1, n, f);
}

static void fifo_clear(void __iomem *b)
{
	u32 c = ioread32(b + 0x00);

	iowrite32(c | BIT(20), b + 0x00);
}

/* D1: are FIFO writes to a fixed 0x80 port auto-incrementing? */
static void test_d1(void __iomem *b)
{
	pr_info("fch_test: ===== D1: FIFO write semantics =====\n");

	fifo_clear(b);
	iowrite8(0xAA, b + 0x80);
	iowrite8(0xBB, b + 0x80);
	iowrite8(0xCC, b + 0x80);
	dump_fifo(b, "D1 fixed-addr pass1", 8);
	dump_fifo(b, "D1 fixed-addr pass2", 8);

	fifo_clear(b);
	iowrite8(0x11, b + 0x80);
	iowrite8(0x22, b + 0x81);
	iowrite8(0x33, b + 0x82);
	dump_fifo(b, "D1 linear pass1", 8);
	dump_fifo(b, "D1 linear pass2", 8);
}

/* D2: Windows-exact small read, amdspi.sys fcn.0x3c20 path 0x414e-0x446a */
static void test_d2(void __iomem *b, int iter)
{
	u32 ctrl0;
	u8 r1d, s4c;
	u32 s04;
	int t;
	char tag[24];

	pr_info("fch_test: ===== D2.%d: Windows-exact read (rx_len=12) =====\n", iter);
	dump_regs(b, "D2 pre");
	dump_fifo(b, "D2 pre", 16);

	/* 0x3f0a-0x3f47: CS assert: 0x1D = (read & 0xFC) | 1, then readback */
	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & 0xFC) | 1, b + 0x1D);
	(void)ioread8(b + 0x1D);

	/* 0x4161: 0x45 = 0x0B, readback */
	iowrite8(0x0B, b + 0x45);
	(void)ioread8(b + 0x45);

	/* 0x41a0: CTRL0 |= BIT(20) (FIFO clear) */
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | BIT(20), b + 0x00);

	/* 0x41d8-0x4232: fresh read, CTRL0 |= 0x60040000 */
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | 0x60040000, b + 0x00);

	/* 0x4243: 0x48 = 3 (TX count: 3 address bytes) */
	iowrite8(3, b + 0x48);

	/* 0x4257-0x4280: three writes of 0x00 to fixed 0x80 */
	iowrite8(0x00, b + 0x80);
	iowrite8(0x00, b + 0x80);
	iowrite8(0x00, b + 0x80);

	/* 0x428a: 0x4B = rx_len */
	iowrite8(12, b + 0x4B);

	/* 0x42a2: 0x45 = 0x0B again, readback */
	iowrite8(0x0B, b + 0x45);
	(void)ioread8(b + 0x45);

	/* 0x42ea: status check before trigger */
	s4c = ioread8(b + 0x4C);
	s04 = ioread32(b + 0x04);
	pr_info("fch_test: D2 pre-trigger: 4C=%02x 04=0x%08x 4C.dw=0x%08x\n",
		s4c, s04, ioread32(b + 0x4C));

	/* 0x438e: trigger: 0x47 = 0x80 (direct write) */
	iowrite8(0x80, b + 0x47);

	/* watch status evolve */
	for (t = 0; t < 20; t++) {
		s4c = ioread8(b + 0x4C);
		s04 = ioread32(b + 0x04);
		pr_info("fch_test: D2 t=%dus: 4C=%02x 04=0x%08x 47=%02x\n",
			t * 50, s4c, s04, ioread8(b + 0x47));
		if (t > 2 && !(s04 & BIT(31)) && !(s4c & 0x80))
			break;
		udelay(50);
	}
	msleep(2);

	snprintf(tag, sizeof(tag), "D2.%d post", iter);
	dump_fifo(b, tag, 24);
	dump_regs(b, tag);
}

/*
 * D3a: Windows-exact write, amdspi.sys fcn.0x54d0 path 0x5720-0x5ad0.
 * Wire model: controller transmits [0x45 opcode=0x02] + tx_len FIFO bytes.
 */
static void fch_write(void __iomem *b, const u8 *tx, int tx_len)
{
	u32 ctrl0;
	u8 r1d;
	int i, t;

	/* CS assert (0x3c20 does it; 0x54d0's caller presumably too) */
	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & 0xFC) | 1, b + 0x1D);

	/* 0x5725: FIFO clear, 0x5736: opcode 0x02 */
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | BIT(20), b + 0x00);
	iowrite8(0x02, b + 0x45);

	/* 0x5771-0x577e: secret bits */
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | 0x60040000, b + 0x00);

	/* 0x5861: TX count; 0x58ac: linear FIFO fill */
	iowrite8(tx_len, b + 0x48);
	for (i = 0; i < tx_len; i++)
		iowrite8(tx[i], b + 0x80 + i);

	/* 0x5943: RX count = 0; 0x5954: opcode again; 0x5abc: trigger */
	iowrite8(0, b + 0x4B);
	iowrite8(0x02, b + 0x45);
	iowrite8(0x80, b + 0x47);

	for (t = 0; t < 20; t++) {
		if (!ioread8(b + 0x47))
			break;
		udelay(50);
	}
	pr_info("fch_test: D3 write done (%*ph) 47=%02x 4C=%02x t=%dus\n",
		tx_len, tx, ioread8(b + 0x47), ioread8(b + 0x4C), t * 50);
}

/* D2 read, silent variant: returns true if any RX byte is not 0x00/0xFF */
static bool fch_read12(void __iomem *b, u8 *rx)
{
	u32 ctrl0;
	u8 r1d;
	int i, t;
	bool alive = false;

	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & 0xFC) | 1, b + 0x1D);
	iowrite8(0x0B, b + 0x45);
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | BIT(20), b + 0x00);
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | 0x60040000, b + 0x00);
	iowrite8(3, b + 0x48);
	iowrite8(0x00, b + 0x80);
	iowrite8(0x00, b + 0x81);
	iowrite8(0x00, b + 0x82);
	iowrite8(12, b + 0x4B);
	iowrite8(0x0B, b + 0x45);
	iowrite8(0x80, b + 0x47);
	for (t = 0; t < 20; t++) {
		if (!ioread8(b + 0x47))
			break;
		udelay(50);
	}
	udelay(200);

	for (i = 0; i < 12; i++) {
		rx[i] = ioread8(b + 0x83 + i);
		if (rx[i] != 0x00 && rx[i] != 0xFF)
			alive = true;
	}
	return alive;
}

/*
 * Generic transfer mirroring the verified register model:
 * 0x45=opcode (HW sends as wire byte 0), FIFO=tx (tx_cnt bytes via 0x48),
 * 0x4B=rx_cnt clocked bytes landing at FIFO+tx_cnt.
 */
static void fch_xfer(void __iomem *b, u8 opcode, const u8 *tx, u8 tx_cnt,
		     u8 rx_cnt)
{
	u32 ctrl0;
	u8 r1d;
	int i, t;

	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & 0xFC) | 1, b + 0x1D);
	iowrite8(opcode, b + 0x45);
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | BIT(20), b + 0x00);
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | 0x60040000, b + 0x00);
	iowrite8(tx_cnt, b + 0x48);
	for (i = 0; i < tx_cnt; i++)
		iowrite8(tx ? tx[i] : 0x00, b + 0x80 + i);
	iowrite8(rx_cnt, b + 0x4B);
	iowrite8(opcode, b + 0x45);
	iowrite8(0x80, b + 0x47);
	for (t = 0; t < 40; t++) {
		if (!ioread8(b + 0x47))
			break;
		udelay(50);
	}
	udelay(200);
}

/*
 * D4: replicate the historically-working wire sequence (from spi-amd logs):
 * wake = opcode 0x07 + FIFO [01 00 01], 0x48=3, 0x4B=7
 * read = opcode 0x0B + FIFO [00 00 00], 0x48=3, 0x4B=17
 * Valid header (sync 0x5A) historically appeared ~FIFO+9.
 */
static void test_d4(void __iomem *b)
{
	static const u8 wake[3] = { 0x01, 0x00, 0x01 };
	static const u8 zeros[3] = { 0x00, 0x00, 0x00 };
	u8 f[24];
	int i, k;

	pr_info("fch_test: ===== D4: historic wake (op 0x07) + read (op 0x0B, rx 17) =====\n");

	for (i = 0; i < 40; i++) {
		fch_xfer(b, 0x07, wake, 3, 7);
		usleep_range(1000, 1500);
		fch_xfer(b, 0x0B, zeros, 3, 17);

		for (k = 0; k < 24; k++)
			f[k] = ioread8(b + 0x80 + k);

		if (i < 3)
			pr_info("fch_test: D4 attempt %d FIFO: %*ph\n",
				i + 1, 24, f);

		for (k = 3; k < 21; k++) {
			if (f[k] == 0x5A) {
				pr_info("fch_test: D4 SYNC 0x5A at FIFO+%d after %d attempt(s), FIFO: %*ph\n",
					k, i + 1, 24, f);
				return;
			}
		}
		msleep(20);
	}
	pr_info("fch_test: D4 no sync after 40 attempts\n");
}

/* D3: ACPI _RST, then SET_POWER(D0) write + read cycle like HidSpiCx */
static void test_d3(void __iomem *b)
{
	static const u8 set_power[8] = { 0x07, 0x01, 0x00, 0x01, 0x01, 0, 0, 0 };
	acpi_handle h = NULL;
	acpi_status st;
	u8 rx[12];
	int i;

	pr_info("fch_test: ===== D3: _RST + SET_POWER + read =====\n");

	st = acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &h);
	if (ACPI_SUCCESS(st)) {
		st = acpi_evaluate_object(h, "_RST", NULL, NULL);
		pr_info("fch_test: D3 ACPI _RST -> %s\n",
			ACPI_SUCCESS(st) ? "ok" : "FAILED");
		msleep(2200); /* HidSpiCx ResettingSyncEntry waits up to 2s */
	} else {
		pr_info("fch_test: D3 no ACPI handle, skipping _RST\n");
	}

	for (i = 0; i < 5; i++) {
		fch_write(b, set_power, sizeof(set_power));
		usleep_range(1000, 1500);
		if (fch_read12(b, rx)) {
			pr_info("fch_test: D3 ALIVE after %d attempt(s): rx=%*ph\n",
				i + 1, 12, rx);
			return;
		}
		if (i < 2)
			pr_info("fch_test: D3 attempt %d rx=%*ph\n", i + 1, 12, rx);
		msleep(20);
	}
	pr_info("fch_test: D3 no response after 5 attempts (expected; D4 next)\n");
}

/*
 * D5: exact clone of the known-working spi-amd-v2-multi code path
 * (setup_v2_regs + exec_segment) for one input-header read:
 * approval [0B 00 00 00 FF] -> opcode 0x0B, FIFO=[00 00 00 FF]+zeros,
 * 0x48=3, 0x4B=13, RMW trigger, RX at FIFO+4 (9 bytes).
 */
static void test_d5(void __iomem *b)
{
	static const u8 tx[13] = { 0x00, 0x00, 0x00, 0xFF };
	u32 ctrl0;
	u16 r44;
	u8 r1d, r47, f[24];
	int i, k, t, attempt;

	pr_info("fch_test: ===== D5: spi-amd exact clone read =====\n");

	{
		acpi_handle h = NULL;

		if (ACPI_SUCCESS(acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &h))) {
			acpi_status st = acpi_evaluate_object(h, "_RST", NULL, NULL);

			pr_info("fch_test: D5 _RST -> %s, waiting 2.2s\n",
				ACPI_SUCCESS(st) ? "ok" : "FAILED");
			msleep(2200);
		}
	}

	for (attempt = 1; attempt <= 10; attempt++) {
		/* amd_spi_setup_v2_regs */
		ctrl0 = ioread32(b + 0x00);
		iowrite32(ctrl0 | 0x60040000, b + 0x00);
		r44 = ioread16(b + 0x44);
		r44 &= 0xf0ff; r44 |= 1 << 8;
		r44 &= 0x0fff; r44 |= 1 << 12;
		iowrite16(r44, b + 0x44);
		r1d = ioread8(b + 0x1D);
		iowrite8((r1d & 0xFC) | 1, b + 0x1D);
		iowrite8(0x00, b + 0x49);
		iowrite8(0x00, b + 0x4A);

		/* clear_fifo_ptr (RMW, single write) */
		ctrl0 = ioread32(b + 0x00);
		iowrite32((ctrl0 & ~BIT(20)) | BIT(20), b + 0x00);

		/* FIFO: approval remainder + explicit zero padding */
		for (i = 0; i < sizeof(tx); i++)
			iowrite8(tx[i], b + 0x80 + i);

		iowrite8(0x0B, b + 0x45);
		iowrite8(3, b + 0x48);
		iowrite8(13, b + 0x4B);

		/* busy wait then RMW trigger like amd_spi_execute_opcode */
		for (t = 0; t < 2000; t++) {
			if (!(ioread32(b + 0x00) & BIT(31)))
				break;
			udelay(20);
		}
		r47 = ioread8(b + 0x47);
		iowrite8((r47 & 0x7F) | 0x80, b + 0x47);

		for (t = 0; t < 100; t++) {
			if (!ioread8(b + 0x47))
				break;
			udelay(50);
		}
		msleep(1);

		for (k = 0; k < 24; k++)
			f[k] = ioread8(b + 0x80 + k);
		pr_info("fch_test: D5 attempt %d 47=%02x FIFO: %*ph\n",
			attempt, ioread8(b + 0x47), 24, f);

		for (k = 4; k < 20; k++) {
			if (f[k] == 0x5A) {
				pr_info("fch_test: D5 *** SYNC 0x5A at FIFO+%d ***\n", k);
				return;
			}
		}
		msleep(60);
	}
	pr_info("fch_test: D5 no sync after 10 attempts\n");
}

/* One spi-amd-style read message with explicit CS assert/deassert */
static bool d6_read_once(void __iomem *b, u8 cs, u8 *fifo_out)
{
	static const u8 tx[13] = { 0x00, 0x00, 0x00, 0xFF };
	u32 ctrl0;
	u16 r44;
	u8 r1d, r47;
	int i, t;
	bool found = false;

	/* select_chip: setclear8(0x1D, cs, 3) */
	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & ~0x3) | cs, b + 0x1D);

	/* setup_v2_regs */
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | 0x60040000, b + 0x00);
	r44 = ioread16(b + 0x44);
	r44 &= 0xf0ff; r44 |= 1 << 8;
	r44 &= 0x0fff; r44 |= 1 << 12;
	iowrite16(r44, b + 0x44);
	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & 0xFC) | 1, b + 0x1D);
	iowrite8(0x00, b + 0x49);
	iowrite8(0x00, b + 0x4A);

	ctrl0 = ioread32(b + 0x00);
	iowrite32((ctrl0 & ~BIT(20)) | BIT(20), b + 0x00);

	for (i = 0; i < sizeof(tx); i++)
		iowrite8(tx[i], b + 0x80 + i);

	iowrite8(0x0B, b + 0x45);
	iowrite8(3, b + 0x48);
	iowrite8(13, b + 0x4B);

	for (t = 0; t < 2000; t++) {
		if (!(ioread32(b + 0x00) & BIT(31)))
			break;
		udelay(20);
	}
	r47 = ioread8(b + 0x47);
	iowrite8((r47 & 0x7F) | 0x80, b + 0x47);
	for (t = 0; t < 100; t++) {
		if (!ioread8(b + 0x47))
			break;
		udelay(50);
	}
	msleep(1);

	for (i = 0; i < 24; i++) {
		fifo_out[i] = ioread8(b + 0x80 + i);
		if (i >= 4 && fifo_out[i] == 0x5A)
			found = true;
	}

	/* clear_chip: write8(0x1D, cs & ~3) — CS deassert edge */
	iowrite8(cs & ~0x3, b + 0x1D);

	return found;
}

static void test_d6(void __iomem *b)
{
	static const u8 cs_list[3] = { 1, 0, 2 };
	u8 f[24];
	int c, attempt;

	pr_info("fch_test: ===== D6: CS sweep + deassert framing =====\n");

	for (c = 0; c < 3; c++) {
		u8 cs = cs_list[c];

		for (attempt = 1; attempt <= 5; attempt++) {
			bool hit = d6_read_once(b, cs, f);

			pr_info("fch_test: D6 cs=%u a%d FIFO: %*ph%s\n",
				cs, attempt, 24, f, hit ? "  *** SYNC ***" : "");
			if (hit)
				return;
			msleep(60);
		}
	}
	pr_info("fch_test: D6 no sync\n");
}

/*
 * D7: read with full 9-byte approval image in FIFO.
 * Wire = [0B] + FIFO[0..(3+rx-1)] clocked. Working trace showed approval
 * byte7 (FIFO[6]) = 0x03 is required for the device to respond.
 */
static bool d7_read(void __iomem *b, const u8 *fifo_img, int img_len,
		    u8 *out, const char *tag)
{
	u32 ctrl0;
	u16 r44;
	u8 r1d, r47;
	int i, t;
	bool found = false;

	r1d = ioread8(b + 0x1D);
	iowrite8((r1d & 0xFC) | 1, b + 0x1D);
	ctrl0 = ioread32(b + 0x00);
	iowrite32(ctrl0 | 0x60040000, b + 0x00);
	r44 = ioread16(b + 0x44);
	r44 &= 0xf0ff; r44 |= 1 << 8;
	r44 &= 0x0fff; r44 |= 1 << 12;
	iowrite16(r44, b + 0x44);
	iowrite8(0x00, b + 0x49);
	iowrite8(0x00, b + 0x4A);

	ctrl0 = ioread32(b + 0x00);
	iowrite32((ctrl0 & ~BIT(20)) | BIT(20), b + 0x00);

	for (i = 0; i < img_len; i++)
		iowrite8(fifo_img[i], b + 0x80 + i);

	iowrite8(0x0B, b + 0x45);
	iowrite8(3, b + 0x48);
	iowrite8(13, b + 0x4B);

	for (t = 0; t < 2000; t++) {
		if (!(ioread32(b + 0x00) & BIT(31)))
			break;
		udelay(20);
	}
	r47 = ioread8(b + 0x47);
	iowrite8((r47 & 0x7F) | 0x80, b + 0x47);
	for (t = 0; t < 100; t++) {
		if (!ioread8(b + 0x47))
			break;
		udelay(50);
	}
	msleep(1);

	for (i = 0; i < 24; i++) {
		out[i] = ioread8(b + 0x80 + i);
		if (i >= 4 && out[i] == 0x5A)
			found = true;
	}
	pr_info("fch_test: D7 %s FIFO: %*ph%s\n", tag, 24, out,
		found ? "  *** SYNC ***" : "");

	/* CS deassert like amd_spi_clear_chip */
	iowrite8(ioread8(b + 0x1D) & ~0x3, b + 0x1D);
	return found;
}

static void test_d7(void __iomem *b)
{
	/* exact replica of the working trace FIFO image */
	static const u8 img_trace[13] = { 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
					  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	/* canonical spec-style approval: [FF 00 00 03 00] after address */
	static const u8 img_spec[13] = { 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
					 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	/* control: byte7 = 0 (should stay silent if theory holds) */
	static const u8 img_zero[13] = { 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	u8 out[24];
	int a;

	pr_info("fch_test: ===== D7: approval byte7 theory =====\n");
	for (a = 0; a < 3; a++) {
		if (d7_read(b, img_trace, 13, out, "trace-img"))
			break;
		msleep(60);
	}
	msleep(100);
	for (a = 0; a < 3; a++) {
		if (d7_read(b, img_spec, 13, out, "spec-img "))
			break;
		msleep(60);
	}
	msleep(100);
	for (a = 0; a < 3; a++) {
		if (d7_read(b, img_zero, 13, out, "zero-img "))
			break;
		msleep(60);
	}
}

/* D8: _RST, wait, then the FIRST transaction is the well-formed read */
static void test_d8(void __iomem *b)
{
	static const u8 img_trace[13] = { 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
					  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	static const u8 img_spec[13] = { 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	acpi_handle h = NULL;
	u8 out[24];
	int a;

	pr_info("fch_test: ===== D8: fresh _RST + first-transaction read =====\n");

	if (ACPI_SUCCESS(acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &h))) {
		acpi_status st = acpi_evaluate_object(h, "_RST", NULL, NULL);

		pr_info("fch_test: D8 _RST -> %s, waiting 2.2s\n",
			ACPI_SUCCESS(st) ? "ok" : "FAILED");
		msleep(2200);
	}

	for (a = 0; a < 3; a++) {
		if (d7_read(b, img_trace, 13, out, "D8 trace-img"))
			return;
		msleep(60);
	}
	for (a = 0; a < 3; a++) {
		if (d7_read(b, img_spec, 13, out, "D8 spec-img "))
			return;
		msleep(60);
	}
}

/*
 * D9: write-pointer theory — TX length may be driven by the NUMBER of FIFO
 * write accesses, not (only) by 0x48. Mimic spi-amd exactly: 4 writes.
 */
static void test_d9(void __iomem *b)
{
	static const u8 tx4[4] = { 0x00, 0x00, 0x00, 0xFF };
	static const u8 tx3[3] = { 0x00, 0x00, 0x00 };
	static const u8 tx9[9] = { 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x03,
				   0x00, 0x00 };
	acpi_handle h = NULL;
	u8 out[24];
	int a;

	pr_info("fch_test: ===== D9: FIFO write-count theory =====\n");

	if (ACPI_SUCCESS(acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &h))) {
		acpi_status st = acpi_evaluate_object(h, "_RST", NULL, NULL);

		pr_info("fch_test: D9 _RST -> %s, waiting 2.2s\n",
			ACPI_SUCCESS(st) ? "ok" : "FAILED");
		msleep(2200);
	}

	for (a = 0; a < 2; a++) {
		if (d7_read(b, tx4, 4, out, "D9 4-writes"))
			return;
		msleep(60);
	}
	for (a = 0; a < 2; a++) {
		if (d7_read(b, tx3, 3, out, "D9 3-writes"))
			return;
		msleep(60);
	}
	for (a = 0; a < 2; a++) {
		if (d7_read(b, tx9, 9, out, "D9 9-writes"))
			return;
		msleep(60);
	}
}

static int testnum = 5;
module_param(testnum, int, 0444);

static int __init fch_test_init(void)
{
	void __iomem *b;

	b = ioremap(FCH_MMIO, FCH_SIZE);
	if (!b) {
		pr_err("fch_test: ioremap failed\n");
		return -ENOMEM;
	}

	dump_regs(b, "boot");
	dump_fifo(b, "boot", 24);

	switch (testnum) {
	case 1:
		test_d1(b);
		test_d2(b, 1);
		break;
	case 3:
		test_d3(b);
		break;
	case 4:
		test_d4(b);
		break;
	case 6:
		test_d6(b);
		break;
	case 7:
		test_d7(b);
		break;
	case 8:
		test_d8(b);
		break;
	case 9:
		test_d9(b);
		break;
	default:
		test_d5(b);
		break;
	}

	iounmap(b);
	return -ENODEV; /* never stay loaded */
}

module_init(fch_test_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("AMD FCH SPI decomp-verification tests D1/D2");
