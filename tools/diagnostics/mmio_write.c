/* RAW MMIO test: Windows-exact SPI write using FCH registers directly.
 * Bypasses the Linux SPI framework entirely.
 * insmod mmio_write.ko → sends DESCREQ, reads response, shows result.
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>

#define FCH_BASE 0xFEC10000
#define FCH_SIZE 0x100

static void __iomem *base;

static u32 reg_read32(int off) { return ioread32(base + off); }
static void reg_write32(int off, u32 v) { iowrite32(v, base + off); }
static u16 reg_read16(int off) { return ioread16(base + off); }
static void reg_write16(int off, u16 v) { iowrite16(v, base + off); }
static u8  reg_read8(int off)  { return ioread8(base + off); }
static void reg_write8(int off, u8 v)  { iowrite8(v, base + off); }

static void fifo_fill(int off, const u8 *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
		reg_write8(0x80 + off + i, data[i]);
}

static void fifo_dump(int len)
{
	u8 buf[70];
	int i;
	for (i = 0; i < min(len, 70); i++)
		buf[i] = reg_read8(0x80 + i);
	pr_info("mmio: FIFO[0..%d]: %*ph\n", min(len, 70)-1, min(len, 70), buf);
}

static int wait_busy(void)
{
	int timeout = 100000;
	while (!(reg_read32(0x00) & BIT(31)) && --timeout)
		udelay(1);
	if (timeout <= 0) {
		pr_err("mmio: BUSY start timeout\n");
		return -1;
	}
	while ((reg_read32(0x00) & BIT(31)) && --timeout)
		udelay(1);
	if (timeout <= 0) {
		pr_err("mmio: BUSY end timeout\n");
		return -1;  
	}
	return 0;
}

/* Windows-exact write sequence from amdspi.sys decomp fcn.0x4bac */
static int windows_write(u8 opcode, const u8 *data, int tx_len, int rx_len)
{
	u16 saved_22;
	u8 nibble;
	u16 r44;
	u32 ctrl0;
	int i;

	pr_info("mmio: WRITE op=0x%02x tx=%d rx=%d\n", opcode, tx_len, rx_len);

	/* Step 1: save 0x22 */
	saved_22 = reg_read16(0x22);

	/* Step 2: ALT_CS: AND 0xFC, OR 0x01 */
	reg_write8(0x1D, (reg_read8(0x1D) & 0xFC) | 0x01);

	/* Step 3: Clear FIFO — toggle (clear then set, per decomp) */
	ctrl0 = reg_read32(0x00);
	ctrl0 &= ~BIT(20);
	reg_write32(0x00, ctrl0);
	ctrl0 |= BIT(20);
	reg_write32(0x00, ctrl0);

	/* Step 4: Write opcode to 0x45 (1st time) */
	reg_write8(0x45, opcode);

	/* Step 5: Secret bits on CTRL0 */
	ctrl0 = reg_read32(0x00);
	ctrl0 |= 0x60040000;  /* bits 30,29,18 — NO TXMODE bit23 */
	reg_write32(0x00, ctrl0);

	/* Step 6-7: 0x44 dance (write path only) */
	if (opcode != 0x0B) {
		r44 = reg_read16(0x44);
		nibble = reg_read8(0x20) & 0x0F;
		r44 = (r44 & 0xF0FF) | ((u16)nibble << 8);
		r44 = (r44 & 0x0FFF) | ((u16)nibble << 12);
		reg_write16(0x44, r44);
		/* 0x44 write16 clobbers 0x45 — re-write opcode */
		reg_write8(0x45, opcode);
	}

	/* Step 8: TX_COUNT */
	reg_write8(0x48, tx_len);

	/* Step 9: Fill FIFO */
	for (i = 0; i < tx_len; i++)
		reg_write8(0x80 + i, data[i]);

	/* Step 10: RX_COUNT (0 for TX-only writes) */
	if (rx_len == 0)
		reg_write8(0x4B, tx_len + 1);  /* Full-duplex: receive response */
	else
		reg_write8(0x4B, rx_len);

	/* Step 11: Re-write opcode after RX_COUNT */
	reg_write8(0x45, opcode);

	/* Step 12: Re-apply secret bits */
	ctrl0 = reg_read32(0x00);
	ctrl0 |= 0x60040000;
	reg_write32(0x00, ctrl0);

	/* Dump pre-trigger state */
	pr_info("mmio: PRE-TRIG CTRL0=0x%08x CTRL1=0x%08x ALT_CS=0x%02x r44=0x%04x op45=0x%02x tx48=%u rx4b=%u\n",
		reg_read32(0x00), reg_read32(0x0C), reg_read8(0x1D),
		reg_read16(0x44), reg_read8(0x45),
		reg_read8(0x48), reg_read8(0x4B));

	/* Step 13: Trigger — write 0x80 to 0x47 */
	reg_write8(0x47, 0x80);

	/* Step 14: Wait busy */
	if (wait_busy()) return -1;

	/* Step 15: Restore 0x22 */
	reg_write16(0x22, saved_22);

	/* Read back response */
	if (rx_len || (opcode != 0x0B)) {
		u8 rx[32];
		int read_off = opcode == 0x0B ? 3 : tx_len; /* after TX data */
		for (i = 0; i < 32 && i + read_off < 70; i++)
			rx[i] = reg_read8(0x80 + read_off + i);
		pr_info("mmio: RX read_off=%d: %*ph\n", read_off, min(32, 70 - read_off), rx);
	}

	/* Restore opcode to 0x0B for subsequent reads */
	reg_write8(0x45, 0x0B);

	pr_info("mmio: WRITE done\n");
	return 0;
}

/* Windows-exact read sequence */
static int windows_read(u32 reg_addr, int rx_len, u8 *out)
{
	u8 tx[5];
	u16 saved_22;
	u32 ctrl0;
	int i, j;

	/* Read approval: 5 bytes */
	tx[0] = 0x0B;
	tx[1] = (reg_addr >> 16) & 0xFF;
	tx[2] = (reg_addr >> 8) & 0xFF;
	tx[3] = reg_addr & 0xFF;
	tx[4] = 0xFF;

	/* Step 1-3: same as write */
	saved_22 = reg_read16(0x22);
	reg_write8(0x1D, (reg_read8(0x1D) & 0xFC) | 0x01);
	ctrl0 = reg_read32(0x00);
	ctrl0 &= ~BIT(20);
	reg_write32(0x00, ctrl0);
	ctrl0 |= BIT(20);
	reg_write32(0x00, ctrl0);

	/* Write opcode (0x0B) to 0x45 */
	reg_write8(0x45, 0x0B);

	/* Secret bits — TXMODE for 0x0B (required on this hw, see driver) */
	ctrl0 = reg_read32(0x00);
	ctrl0 |= 0x60040000;
	reg_write32(0x00, ctrl0);

	/* TX_COUNT: 3 bytes (register address) */
	reg_write8(0x48, 3);

	/* FIFO: 3 address bytes */
	reg_write8(0x80 + 0, tx[1]);
	reg_write8(0x80 + 1, tx[2]);
	reg_write8(0x80 + 2, tx[3]);

	/* RX_COUNT */
	reg_write8(0x4B, rx_len);

	/* Re-apply secret bits */
	ctrl0 = reg_read32(0x00);
	ctrl0 |= 0x60040000;
	reg_write32(0x00, ctrl0);

	/* Trigger */
	reg_write8(0x47, 0x80);

	if (wait_busy()) return -1;

	reg_write16(0x22, saved_22);

	/* Read RX from FIFO at offset tx_len (3) */
	for (i = 0; i < rx_len && i < 64; i++)
		out[i] = reg_read8(0x80 + 3 + i);

	return rx_len;
}

static int __init mmio_init(void)
{
	u8 rx[64];
	int ret, i;

	base = ioremap(FCH_BASE, FCH_SIZE);
	if (!base) return -ENOMEM;

	pr_info("mmio: FCH mapped at %px\n", base);

	/* Dump initial state */
	pr_info("mmio: INIT CTRL0=0x%08x CTRL1=0x%08x ALT_CS=0x%02x ENA=0x%08x r44=0x%04x\n",
		reg_read32(0x00), reg_read32(0x0C), reg_read8(0x1D),
		reg_read32(0x20), reg_read16(0x44));

	/* Write CTRL1 matching Windows */
	reg_write32(0x0C, 0x020006B5);
	pr_info("mmio: CTRL1 set to 0x%08x\n", reg_read32(0x0C));

	/* === READ 1: Get RESET_RSP === */
	memset(rx, 0, sizeof(rx));
	windows_read(0x000000, 9, rx);
	pr_info("mmio: READ1: %*ph\n", 9, rx);

	/* === READ 2: Drain ACK (device auto-advances) === */
	msleep(1); /* ~168us as Windows does */
	memset(rx, 0, sizeof(rx));
	windows_read(0x000000, 9, rx);
	pr_info("mmio: READ2 (drain ACK): %*ph\n", 9, rx);

	/* === WRITE: Send DESCREQ === */
	{
		u8 descreq[] = {
			0x00, 0x00, 0x01,   /* register 0x000001 */
			0x42,               /* version | len_nibble */
			0x00,               /* len (low byte of remaining) */
			0x00, 0x03, 0x00, 0x00 /* output body + padding */
		};
		pr_info("mmio: Sending DESCREQ: %*ph\n", 9, descreq);
		windows_write(0x02, descreq, 9, 10);
	}

	/* === Wait for IRQ-like delay (matches Windows ~58µs) === */
	udelay(100);

	/* === READ 3: Get DEVICE_DESC === */
	memset(rx, 0, sizeof(rx));
	windows_read(0x000000, 9, rx);
	pr_info("mmio: READ3 (post-DESCREQ): %*ph\n", 9, rx);

	/* Full FIFO dump */
	fifo_dump(70);

	iounmap(base);
	return -ENODEV; /* don't stay loaded */
}

module_init(mmio_init);
MODULE_LICENSE("GPL");
