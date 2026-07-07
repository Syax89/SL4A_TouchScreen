/* Test hypothesis B: 16-bit FIFO mode TX data alignment.
 * In 16-bit mode (PCI 0xB8 bit7=1), iowrite8() for TX might produce
 * misaligned data because the FIFO is word-addressed.
 *
 * This module does TWO writes (byte vs word) and compares results.
 *
 * insmod tx16_test.ko && dmesg | grep "tx16:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>

#define FCH_BASE 0xFEC10000

static void __iomem *base;

#define w32(o,v) iowrite32(v, base+(o))
#define r32(o)   ioread32(base+(o))
#define w16(o,v) iowrite16(v, base+(o))
#define r16(o)   ioread16(base+(o))
#define w8(o,v)  iowrite8(v, base+(o))
#define r8(o)    ioread8(base+(o))

static void fifo_read16(int off, u8 *dst, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		u16 w = r16(off + (i/2)*2);
		dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
	}
}

/* Write TX data as BYTES (current method) */
static void fifo_write_bytes(int off, const u8 *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
		w8(off + i, data[i]);
}

/* Write TX data as 16-bit WORDS (hypothesis B fix) */
static void fifo_write_words(int off, const u8 *data, int len)
{
	int i;
	for (i = 0; i < len; i += 2) {
		u16 w = data[i];
		if (i + 1 < len)
			w |= (u16)data[i+1] << 8;
		w16(off + i, w);
	}
}

#define CTRL0   0x00
#define CTRL1   0x0C
#define ALT_CS  0x1D
#define OPCODE  0x45
#define TRIGGER 0x47
#define TXCNT   0x48
#define RXCNT   0x4B
#define FIFO    0x80
#define REG22   0x22
#define REG44   0x44

static void cs1(void) { w8(ALT_CS, (r8(ALT_CS) & 0xFC) | 0x01); }

static int wait_bsy(void)
{
	u32 v;
	return readl_poll_timeout(base + CTRL0, v, !(v & BIT(31)), 20, 2000000);
}

static void clr_fifo(void)
{
	u32 c = r32(CTRL0);
	w32(CTRL0, c & ~BIT(20));
	w32(CTRL0, c | BIT(20));
}

static void set_secret(void)
{
	w32(CTRL0, r32(CTRL0) | 0x60040000);
}

/* Read 9 bytes from register addr using 0x0B */
static int read_reg(u32 addr, u8 *rx)
{
	clr_fifo();
	w8(OPCODE, 0x0B);
	set_secret();
	w8(TXCNT, 3);
	fifo_write_bytes(FIFO, (u8[]){ (addr>>16)&0xFF, (addr>>8)&0xFF, addr&0xFF }, 3);
	w8(RXCNT, 9);
	set_secret();
	w8(TRIGGER, 0x80);
	if (wait_bsy()) return -1;
	fifo_read16(FIFO + 3, rx, 9);
	w8(OPCODE, 0x0B);
	return 0;
}

/* Write payload to device with given TX_COUNT, capture RX */
static int write_dev(u8 opcode, const u8 *payload, int tx_len, int rx_len, u8 *rx,
		     void (*fill)(int, const u8*, int), const char *method)
{
	u32 c0_before = r32(CTRL0);

	clr_fifo();
	w8(OPCODE, opcode);
	set_secret();

	/* 0x44 dance for non-0x0B writes */
	if (opcode != 0x0B) {
		u16 r44 = r16(REG44);
		u8 nib = (r32(0x20) >> 20) & 0xF;
		r44 = (r44 & 0xF0FF) | ((u16)nib << 8);
		r44 = (r44 & 0x0FFF) | ((u16)nib << 12);
		w16(REG44, r44);
		w8(OPCODE, opcode);
	}

	w8(TXCNT, tx_len);
	fill(FIFO, payload, tx_len);
	w8(RXCNT, rx_len);

	set_secret();

	if (opcode != 0x0B)
		w8(OPCODE, opcode);

	/* Dump pre-trigger */
	{
		u32 c0 = r32(CTRL0);
		pr_info("tx16: WRITE[%s] pre-trig c0=0x%08x (delta=0x%08x) c1=0x%08x op45=0x%02x tx48=%u rx4b=%u\n",
			method, c0, c0 ^ c0_before, r32(CTRL1), r8(OPCODE), r8(TXCNT), r8(RXCNT));
	}

	w8(TRIGGER, 0x80);
	if (wait_bsy()) { pr_err("tx16: BUSY TIMEOUT\n"); return -1; }

	if (rx_len && rx)
		fifo_read16(FIFO + tx_len, rx, rx_len);

	w8(OPCODE, 0x0B);
	return 0;
}

/* Full FIFO dump */
static void dump_fifo(void)
{
	u8 f[70];
	fifo_read16(FIFO, f, 70);
	pr_info("tx16: FIFO[0..69]=[%*ph]\n", 70, f);
}

static int __init tx16_init(void)
{
	u8 rx_a[16], rx_b[16];
	u8 descreq[] = { 0x00, 0x00, 0x01, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00 };
	int txlen = sizeof(descreq);

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;

	cs1();

	pr_info("tx16: ==============================================\n");
	pr_info("tx16: 16-BIT FIFO TX TEST: byte vs word\n");
	pr_info("tx16: ==============================================\n");

	dump_fifo();

	/* ====== PART A: Write using BYTES (current method) ====== */
	pr_info("tx16: --- PART A: TX via iowrite8 (byte mode) ---\n");
	memset(rx_a, 0xCC, sizeof(rx_a));
	write_dev(0x02, descreq, txlen, sizeof(rx_a), rx_a, fifo_write_bytes, "iowrite8");
	pr_info("tx16:   MISO (byte TX)=[%*ph]\n", (int)sizeof(rx_a), rx_a);

	/* Read back to see device state */
	{
		u8 rd[9];
		memset(rd, 0xCC, 9);
		read_reg(0x000000, rd);
		pr_info("tx16:   post-DESCREQ[byte] read=[%*ph]\n", 9, rd);
	}

	/* Wait a bit between tests */
	msleep(5);

	/* ====== PART B: Write using WORDS (hypothesis B fix) ====== */
	pr_info("tx16: --- PART B: TX via iowrite16 (word mode) ---\n");
	memset(rx_b, 0xCC, sizeof(rx_b));
	write_dev(0x02, descreq, txlen, sizeof(rx_b), rx_b, fifo_write_words, "iowrite16");
	pr_info("tx16:   MISO (word TX)=[%*ph]\n", (int)sizeof(rx_b), rx_b);

	/* Read back to see device state */
	{
		u8 rd[9];
		memset(rd, 0xCC, 9);
		read_reg(0x000000, rd);
		pr_info("tx16:   post-DESCREQ[word] read=[%*ph]\n", 9, rd);
	}

	dump_fifo();

	/* ====== PART C: Try larger read to find HID header ====== */
	pr_info("tx16: --- PART C: Large read (37B) after word-mode DESCREQ ---\n");
	{
		u8 rd[37];
		int i;
		memset(rd, 0xCC, sizeof(rd));

		clr_fifo();
		w8(OPCODE, 0x0B);
		set_secret();
		w8(TXCNT, 3);
		fifo_write_bytes(FIFO, (u8[]){0,0,0}, 3);
		w8(RXCNT, sizeof(rd));
		set_secret();
		w8(TRIGGER, 0x80);
		wait_bsy();
		fifo_read16(FIFO + 3, rd, sizeof(rd));

		pr_info("tx16:   large read=[%*ph]\n", (int)sizeof(rd), rd);
		for (i = 3; i < sizeof(rd); i++) {
			if (rd[i] == 0x5A && (rd[i-3] & 0x0F) == 2) {
				int t = (rd[i-3] >> 4) & 0xF;
				u16 bl = (((rd[i-2] >> 4) & 0xF) | (rd[i-1] << 4)) * 4;
				const char *tn = t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?";
				pr_info("tx16:   HID hdr@%d: type=%d(%s) blen=%u raw=[%02x %02x %02x %02x]\n",
					i-3, t, tn, bl, rd[i-3], rd[i-2], rd[i-1], rd[i]);
			}
		}
	}

	w8(OPCODE, 0x0B);
	iounmap(base);
	pr_info("tx16: TEST COMPLETE\n");
	return -ENODEV;
}

module_init(tx16_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("16-bit FIFO TX byte vs word alignment test");
