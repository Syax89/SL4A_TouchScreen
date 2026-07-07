/* DESCREQ test: exact Windows init sequence via raw MMIO.
 * 1. Read RESET_RSP from register 0x000000
 * 2. Send DESCREQ to register 0x000001 (CORRECT register)
 * 3. Read response from register 0x000000 — expect DEVICE_DESC (type=7)
 * 4. If DEVICE_DESC received, send DESCREQ2 and get RPT_DESC
 *
 * insmod descreq_test.ko && dmesg | grep "descreq:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>

#define FCH_BASE 0xFEC10000
#define FCH_SIZE 0x100

static void __iomem *base;

static u32 r32(int off) { return ioread32(base + off); }
static void w32(int off, u32 v) { iowrite32(v, base + off); }
static u16 r16(int off) { return ioread16(base + off); }
static void w16(int off, u16 v) { iowrite16(v, base + off); }
static u8  r8(int off)  { return ioread8(base + off); }
static void w8(int off, u8 v)  { iowrite8(v, base + off); }

static void fifo_read16(int off, u8 *dst, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		u16 w = ioread16(base + off + (i/2)*2);
		dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
	}
}

/* Write TX data to FIFO byte by byte */
static void fifo_write8(int off, const u8 *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
		w8(off + i, data[i]);
}

#define CTRL0   0x00
#define OPCODE  0x45
#define TRIGGER 0x47
#define TX_COUNT 0x48
#define RX_COUNT 0x4B
#define FIFO    0x80
#define ALT_CS  0x1D
#define REG_22  0x22

static void select_cs1(void)
{
	u8 val = r8(ALT_CS);
	val = (val & 0xFC) | 0x01;
	w8(ALT_CS, val);
}

static int wait_busy(void)
{
	u32 val;
	return readl_poll_timeout(base + CTRL0, val, !(val & BIT(31)), 20, 2000000);
}

static void setup_ctrl0(void)
{
	u32 ctrl0 = r32(CTRL0);
	ctrl0 &= ~BIT(20);  /* clear FIFO clear */
	w32(CTRL0, ctrl0);
	ctrl0 |= BIT(20);   /* set FIFO clear */
	w32(CTRL0, ctrl0);
}

/* Read from device, register 24-bit BE address, rx_len bytes */
static int do_read(u32 reg_addr, int rx_len, u8 *rx_buf, const char *label)
{
	u32 ctrl0;
	u8 tx[3] = {
		(reg_addr >> 16) & 0xFF,
		(reg_addr >> 8) & 0xFF,
		reg_addr & 0xFF,
	};

	ctrl0 = r32(CTRL0);
	ctrl0 &= ~BIT(20); w32(CTRL0, ctrl0);
	ctrl0 |= BIT(20);  w32(CTRL0, ctrl0);

	w8(OPCODE, 0x0B);

	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	w32(CTRL0, ctrl0);

	w8(TX_COUNT, 3);
	fifo_write8(FIFO, tx, 3);
	w8(RX_COUNT, rx_len);

	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	w32(CTRL0, ctrl0);

	wmb();
	w8(TRIGGER, 0x80);

	if (wait_busy()) {
		pr_err("descreq: READ %s busy timeout\n", label);
		return -1;
	}

	fifo_read16(FIFO + 3, rx_buf, rx_len);
	w8(OPCODE, 0x0B);
	return 0;
}

/* Write to device: opcode via 0x45, payload in FIFO (tx_len bytes), rx_len RX bytes */
static int do_write(u8 opcode, const u8 *tx_data, int tx_len, int rx_len, u8 *rx_buf, const char *label)
{
	u32 ctrl0;

	ctrl0 = r32(CTRL0);
	ctrl0 &= ~BIT(20); w32(CTRL0, ctrl0);
	ctrl0 |= BIT(20);  w32(CTRL0, ctrl0);

	w8(OPCODE, opcode);

	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	w32(CTRL0, ctrl0);

	w8(TX_COUNT, tx_len);
	fifo_write8(FIFO, tx_data, tx_len);
	w8(RX_COUNT, rx_len);

	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	w32(CTRL0, ctrl0);

	wmb();

	/* Dump pre-trigger state */
	{
		u32 c0 = r32(CTRL0);
		u32 c1 = ioread32(base + 0x0C);
		u8 cs = r8(ALT_CS);
		pr_info("descreq: WRITE %s pre-trig c0=0x%08x c1=0x%08x cs=0x%02x op45=0x%02x tx48=%u rx4b=%u\n",
			label, c0, c1, cs, r8(OPCODE), r8(TX_COUNT), r8(RX_COUNT));
	}

	w8(TRIGGER, 0x80);

	if (wait_busy()) {
		pr_err("descreq: WRITE %s busy timeout\n", label);
		return -1;
	}

	if (rx_len > 0 && rx_buf)
		fifo_read16(FIFO + tx_len, rx_buf, rx_len);

	w8(OPCODE, 0x0B);
	return 0;
}

static void decode_hid_header(const u8 *data, int len)
{
	int off = -1;
	int i;
	/* Find sync byte 0x5A with valid version nibble */
	for (i = 3; i < len; i++) {
		if (data[i] == 0x5A && (data[i-3] & 0x0F) == 2) {
			off = i - 3;
			break;
		}
	}
	if (off < 0) {
		pr_info("descreq:   No HID header found in %d bytes\n", len);
		return;
	}
	/* Also check for header at earlier offset (before 0x5A) */
	for (i = 0; i < len - 3; i++) {
		if (data[i+3] == 0x5A && (data[i] & 0x0F) == 2 && (data[i+1] & 0x0F) == 0) {
			off = i;
			break;
		}
	}

	{
		u8  type    = (data[off] >> 4) & 0xF;
		u8  version = data[off] & 0xF;
		u16 body_len = (((data[off+1] >> 4) & 0xF) | (data[off+2] << 4)) * 4;
		const char *tname = "?";
		switch (type) {
		case 1: tname = "DATA"; break;
		case 3: tname = "RESET_RSP"; break;
		case 7: tname = "DEVICE_DESC"; break;
		case 8: tname = "RPT_DESC"; break;
		}
		pr_info("descreq:   HID hdr@%d: type=%d(%s) ver=%d body_len=%u raw=[%02x %02x %02x %02x]\n",
			off, type, tname, version, body_len,
			data[off], data[off+1], data[off+2], data[off+3]);
	}
}

static int __init descreq_init(void)
{
	u8 rx[64];

	pr_info("descreq: =========================================\n");
	pr_info("descreq: DESCREQ TEST — EXACT WINDOWS INIT SEQUENCE\n");
	pr_info("descreq: =========================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if (!base) return -ENOMEM;

	select_cs1();

	/* Phase 1: Read RESET_RSP header (9 bytes as Windows does) */
	pr_info("descreq: [1/N] Reading RESET_RSP header (9 bytes)...\n");
	memset(rx, 0xCC, sizeof(rx));
	do_read(0x000000, 9, rx, "RESET_RSP_hdr");
	pr_info("descreq:   raw=[%*ph]\n", 9, rx);
	decode_hid_header(rx, 9);

	/* Phase 2: Read RESET_RSP body (9 bytes — Windows drains it) */
	pr_info("descreq: [2/N] Reading RESET_RSP body (9 bytes, +170µs delay)...\n");
	udelay(170);
	memset(rx, 0xCC, sizeof(rx));
	do_read(0x000000, 9, rx, "RESET_RSP_body");
	pr_info("descreq:   raw=[%*ph]\n", 9, rx);

	/* Phase 3: Send DESCREQ to register 0x000001 (TX-only, no RX_COUNT like Windows) */
	pr_info("descreq: [3/N] Sending DESCREQ to reg 0x000001 (TX-only, RX_COUNT=0)...\n");
	{
		u8 descreq_payload[] = {
			0x00, 0x00, 0x01,  /* register 0x000001 */
			0x42,              /* version | len_nibble */
			0x00,              /* len low */
			0x00, 0x03, 0x00, 0x00 /* body + padding */
		};
		memset(rx, 0xCC, sizeof(rx));
		do_write(0x02, descreq_payload, sizeof(descreq_payload), 0, NULL, "DESCREQ@0x01(TX-only)");
		/* Now read MISO separately to see what the device sent during the write */
		memset(rx, 0xCC, sizeof(rx));
		do_read(0x000000, 9, rx, "post-DESCREQ read 9B");
		pr_info("descreq:   raw=[%*ph]\n", 9, rx);
		decode_hid_header(rx, 9);
	}

	/* Phase 4: Wait, then read response (expect DEVICE_DESC type=7) */
	pr_info("descreq: [4/N] Waiting 10ms, then reading response...\n");
	msleep(10);
	memset(rx, 0xCC, sizeof(rx));
	do_read(0x000000, 9, rx, "post-DESCREQ+10ms");
	pr_info("descreq:   raw=[%*ph]\n", 9, rx);
	decode_hid_header(rx, 9);

	/* Phase 5: Longer read (37 bytes as Windows does for DEVICE_DESC body) */
	pr_info("descreq: [5/N] Longer read (37B) to capture body...\n");
	memset(rx, 0xCC, sizeof(rx));
	do_read(0x000000, 37, rx, "post-DESCREQ 37B");
	pr_info("descreq:   raw=[%*ph]\n", 37, rx);
	decode_hid_header(rx, 37);

	/* Full FIFO dump */
	{
		u8 fifo[70];
		pr_info("descreq: FIFO dump (70B):\n");
		fifo_read16(FIFO, fifo, 70);
		pr_info("descreq:   [%*ph]\n", 70, fifo);
	}

	w8(OPCODE, 0x0B);
	iounmap(base);
	pr_info("descreq: TEST COMPLETE\n");
	return -ENODEV;
}

module_init(descreq_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DESCREQ diagnostic: test correct descriptor request sequence");
