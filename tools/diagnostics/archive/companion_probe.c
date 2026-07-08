/* Companion device diagnostic: probe all CS lines for companion device.
 * Tests whether WRITE operations work on ANY device on the SPI bus.
 *
 * insmod companion_probe.ko
 * dmesg | grep "companion:"
 *
 * Key diagnostic: if writes fail on ALL devices, the problem is at controller level.
 * If writes work on companion but not touchscreen, the problem is device-specific.
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

/* 16-bit FIFO read with byte extraction (PCI 0xB8 bit7=1) */
static void fifo_read16(int off, u8 *dst, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		u16 w = ioread16(base + off + (i/2)*2);
		dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
	}
}

#define CTRL0   0x00
#define CTRL1   0x0C
#define ALT_CS  0x1D
#define ENA     0x20
#define REG_22  0x22
#define REG_44  0x44
#define OPCODE  0x45
#define TRIGGER 0x47
#define TX_COUNT 0x48
#define RX_COUNT 0x4B
#define STATUS  0x4C
#define FIFO    0x80

#define BIT_BUSY       BIT(31)
#define BIT_FIFO_CLEAR BIT(20)
#define BIT_TXMODE     BIT(23)

static int wait_busy(void)
{
	u32 val;
	return readl_poll_timeout(base + CTRL0, val, !(val & BIT_BUSY), 20, 2000000);
}

/* Apply Windows-like controller setup */
static void setup_controller(void)
{
	/* Set PCI 0xB8 bit7 if not already (16-bit FIFO mode) — user must do via setpci */
	/* Windows CTRL1 value (likely read-only but try) */
	w32(CTRL1, 0x020006B5);
	/* ENA: set ALT_SPD for 33MHz */
	w32(ENA, 0x11110713);
}

static void select_cs(u8 cs)
{
	u8 val = r8(ALT_CS);
	val = (val & 0xFC) | (cs & 0x03);
	w8(ALT_CS, val);
}

/* Read from device at register address. Returns -1 on error, 0 on success. */
static int read_reg(u32 addr, int rx_len, u8 *rx_buf)
{
	u16 saved_22;
	u32 ctrl0;

	saved_22 = r16(REG_22);

	/* Clear FIFO */
	ctrl0 = r32(CTRL0);
	w32(CTRL0, ctrl0 & ~BIT_FIFO_CLEAR);
	w32(CTRL0, ctrl0 | BIT_FIFO_CLEAR);

	/* Set opcode 0x0B */
	w8(OPCODE, 0x0B);

	/* Secret bits (READ_MODE FAST) */
	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	w32(CTRL0, ctrl0);

	/* TX_COUNT = 3 (register address) */
	w8(TX_COUNT, 3);

	/* Fill FIFO with address bytes (24-bit BE) */
	w8(FIFO + 0, (addr >> 16) & 0xFF);
	w8(FIFO + 1, (addr >> 8) & 0xFF);
	w8(FIFO + 2, addr & 0xFF);

	/* RX_COUNT */
	w8(RX_COUNT, rx_len);

	/* Re-apply secret bits */
	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	w32(CTRL0, ctrl0);

	wmb();

	/* Trigger on 0x47 */
	w8(TRIGGER, 0x80);

	if (wait_busy()) {
		w16(REG_22, saved_22);
		return -1;
	}

	/* Read RX at offset 0x80 + TX_COUNT(3) — use 16-bit word reads */
	fifo_read16(FIFO + 3, rx_buf, rx_len);

	w16(REG_22, saved_22);
	w8(OPCODE, 0x0B);
	return 0;
}

/* Write to device with opcode. txmode=1 for non-standard opcodes (0x70, 0x00 etc) */
static int write_opcode(u8 opcode, const u8 *tx_data, int tx_len, int rx_len, u8 *rx_buf, int txmode)
{
	u16 saved_22;
	u32 ctrl0;
	int i;

	saved_22 = r16(REG_22);

	/* Clear FIFO */
	ctrl0 = r32(CTRL0);
	w32(CTRL0, ctrl0 & ~BIT_FIFO_CLEAR);
	w32(CTRL0, ctrl0 | BIT_FIFO_CLEAR);

	/* Set opcode */
	w8(OPCODE, opcode);

	/* Secret bits + TXMODE if needed */
	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	if (txmode)
		ctrl0 |= BIT_TXMODE;
	w32(CTRL0, ctrl0);

	/* TX_COUNT */
	w8(TX_COUNT, tx_len);

	/* Fill FIFO */
	for (i = 0; i < tx_len; i++)
		w8(FIFO + i, tx_data[i]);

	/* RX_COUNT */
	w8(RX_COUNT, rx_len);

	/* Re-apply secret bits + TXMODE before trigger */
	ctrl0 = r32(CTRL0);
	ctrl0 |= 0x60040000;
	if (txmode)
		ctrl0 |= BIT_TXMODE;
	w32(CTRL0, ctrl0);

	wmb();

	/* Dump pre-trigger state for write operations */
	if (opcode != 0x0B) {
		u32 c0 = r32(CTRL0);
		u32 c1 = r32(CTRL1);
		u8 cs = r8(ALT_CS);
		u8 op = r8(OPCODE);
		u8 tx = r8(TX_COUNT);
		u8 rx = r8(RX_COUNT);
		pr_info("companion: WRITE PRE-TRIG op=0x%02x txmode=%d c0=0x%08x c1=0x%08x alt_cs=0x%02x op45=0x%02x tx48=%u rx4b=%u\n",
			opcode, txmode, c0, c1, cs, op, tx, rx);
	}

	/* Trigger */
	w8(TRIGGER, 0x80);

	if (wait_busy()) {
		w16(REG_22, saved_22);
		if (txmode) { ctrl0 = r32(CTRL0); w32(CTRL0, ctrl0 & ~BIT_TXMODE); }
		return -1;
	}

	/* Read RX if requested — use 16-bit word reads */
	if (rx_len > 0 && rx_buf)
		fifo_read16(FIFO + tx_len, rx_buf, rx_len);

	w16(REG_22, saved_22);
	w8(OPCODE, 0x0B);
	if (txmode) {
		ctrl0 = r32(CTRL0);
		w32(CTRL0, ctrl0 & ~BIT_TXMODE);
	}
	return 0;
}

/* Probe a single CS line */
static void probe_cs(u8 cs)
{
	u8 rx[32];
	int ret;

	pr_info("companion: === CS%d ===\n", cs);
	select_cs(cs);

	/* Try standard read (0x0B) — read from register 0x000000 */
	memset(rx, 0xCC, sizeof(rx));
	ret = read_reg(0x000000, 16, rx);
	if (ret < 0) {
		pr_info("companion: CS%d READ failed (busy timeout)\n", cs);
		return;
	}
	pr_info("companion: CS%d READ reg=0x000000 len=16 -> [%*ph]\n", cs, 16, rx);

	/* Try write 0x02 to register 0x000000 (HID-style DESCREQ-like) */
	{
		u8 wr[] = { 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00 };
		memset(rx, 0xCC, sizeof(rx));
		pr_info("companion: CS%d WRITE 0x02 (NO_TXMODE) tx=%zu -> ", cs, sizeof(wr));
		ret = write_opcode(0x02, wr, sizeof(wr), 16, rx, 0);
		if (ret < 0) pr_cont("BUSY_TIMEOUT\n");
		else pr_cont("rx=[%*ph]\n", 16, rx);
	}

	/* Try write 0x02 WITH TXMODE */
	{
		u8 wr[] = { 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00 };
		memset(rx, 0xCC, sizeof(rx));
		pr_info("companion: CS%d WRITE 0x02 (WITH_TXMODE) tx=%zu -> ", cs, sizeof(wr));
		ret = write_opcode(0x02, wr, sizeof(wr), 16, rx, 1);
		if (ret < 0) pr_cont("BUSY_TIMEOUT\n");
		else pr_cont("rx=[%*ph]\n", 16, rx);
	}

	/* Try cmd70 (companion activation command) WITH TXMODE */
	{
		u8 wr[] = { };
		memset(rx, 0xCC, sizeof(rx));
		pr_info("companion: CS%d WRITE 0x70 (cmd70) WITH TXMODE tx=1 -> ", cs);
		ret = write_opcode(0x70, wr, 0, 14, rx, 1);
		if (ret < 0) pr_cont("BUSY_TIMEOUT\n");
		else pr_cont("rx=[%*ph]\n", 14, rx);
	}

	/* Try cmd00 (companion activation command on conn 0x18) WITH TXMODE */
	{
		u8 wr[] = { 0x0E, 0x00, 0x00, 0x00 };
		memset(rx, 0xCC, sizeof(rx));
		pr_info("companion: CS%d WRITE 0x00 (cmd00) WITH TXMODE tx=%zu -> ", cs, sizeof(wr));
		ret = write_opcode(0x00, wr, sizeof(wr), 4, rx, 1);
		if (ret < 0) pr_cont("BUSY_TIMEOUT\n");
		else pr_cont("rx=[%*ph]\n", 4, rx);
	}
}

/* Probe just CS0 and CS1 (the most likely candidates per Windows ALT_CS behavior) */
static int __init companion_probe_init(void)
{
	u8 cs;
	pr_info("companion: ==============================\n");
	pr_info("companion: COMPANION DEVICE DIAGNOSTIC\n");
	pr_info("companion: ==============================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if (!base) return -ENOMEM;

	/* Dump initial controller state */
	pr_info("companion: INIT c0=0x%08x c1=0x%08x alt_cs=0x%02x ena=0x%08x r22=0x%04x r44=0x%04x op45=0x%02x tr47=0x%02x tx48=%u rx4b=%u\n",
		r32(CTRL0), r32(CTRL1), r8(ALT_CS), r32(ENA),
		r16(REG_22), r16(REG_44), r8(OPCODE), r8(TRIGGER),
		r8(TX_COUNT), r8(RX_COUNT));

	setup_controller();

	/* Probe CS 0-3 */
	for (cs = 0; cs <= 3; cs++)
		probe_cs(cs);

	/* Restore CS to CS1 (touchscreen) */
	select_cs(1);
	w8(OPCODE, 0x0B);

	iounmap(base);
	pr_info("companion: PROBE COMPLETE — check dmesg for results\n");
	return -ENODEV; /* Don't stay loaded */
}

module_init(companion_probe_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Companion device probe for SL4A touchscreen debug");
