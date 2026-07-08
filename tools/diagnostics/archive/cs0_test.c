/* tools/diagnostics/cs0_test.c
 * Test if communicating on CS0 (ALT_CS bits 1-0 = 00) works,
 * and if it resolves the write path failure.
 *
 * insmod cs0_test.ko && dmesg | grep "cs0t:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/iopoll.h>

#define FCH_BASE 0xFEC10000
#define FCH_SIZE 0x100
#define GPIO_BASE 0xFED81500
#define GPIO_SIZE 0x300
#define IRQ_PIN   85
#define PIN_REG   (IRQ_PIN * 4)
#define PIN_STS   (1u << 16)

static void __iomem *base;
static void __iomem *gpio;

#define CTRL0 0x00
#define OPCODE 0x45
#define TRIGGER 0x47
#define TX_COUNT 0x48
#define RX_COUNT 0x4B
#define FIFO 0x80
#define ALT_CS 0x1D

static u32 r32(int o){ return ioread32(base+o); }
static void w32(int o,u32 v){ iowrite32(v,base+o); }
static u8  r8(int o){ return ioread8(base+o); }
static void w8(int o,u8 v){ iowrite8(v,base+o); }

static bool pin_asserted(void){ return !(ioread32(gpio + PIN_REG) & PIN_STS); }
static void select_cs0(void){ u8 v=r8(ALT_CS); w8(ALT_CS,(v&0xFC)|0x00); }
static int wait_busy(void){ u32 v; return readl_poll_timeout(base+CTRL0,v,!(v&BIT(31)),20,2000000); }

/* 8-bit FIFO read/write */
static void fifo_write8(int off, const u8 *d, int n) {
	int i;
	for (i = 0; i < n; i++)
		iowrite8(d[i], base + off + i);
}
static void fifo_read8(int off, u8 *dst, int n) {
	int i;
	for (i = 0; i < n; i++)
		dst[i] = ioread8(base + off + i);
}

/* 16-bit FIFO read/write */
static void fifo_write16(int off, const u8 *d, int n) {
	int i;
	for (i = 0; i < n; i += 2) {
		u16 w = d[i];
		if (i + 1 < n)
			w |= (u16)d[i+1] << 8;
		iowrite16(w, base + off + i);
	}
}
static void fifo_read16(int off, u8 *dst, int n) {
	int i;
	for (i = 0; i < n; i++) {
		u16 w = ioread16(base + off + (i/2)*2);
		dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
	}
}

static int hid_type(const u8 *d, int n) {
	int i;
	for (i = 0; i < n - 3; i++)
		if (d[i+3] == 0x5A && (d[i] & 0x0F) == 2 && (d[i+1] & 0x0F) == 0)
			return (d[i] >> 4) & 0xF;
	return -1;
}

static const char *tname(int t) {
	switch (t) {
	case 1: return "DATA";
	case 3: return "RESET_RSP";
	case 7: return "DEVICE_DESC";
	case 8: return "RPT_DESC";
	default: return "none/invalid";
	}
}

static bool wait_for_pin85(void) {
	int tries;
	for (tries = 0; tries < 1000; tries++) {
		if (pin_asserted())
			return true;
		usleep_range(900, 1100);
	}
	return false;
}

static int do_read(u32 addr, int rx, u8 *buf, bool mode16) {
	u32 c;
	u8 tx[3] = { (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF };
	
	c = r32(CTRL0); c &= ~BIT(20); w32(CTRL0, c); c |= BIT(20); w32(CTRL0, c);
	w8(OPCODE, 0x0B);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	
	w8(TX_COUNT, 3);
	if (mode16)
		fifo_write16(FIFO, tx, 3);
	else
		fifo_write8(FIFO, tx, 3);
		
	w8(RX_COUNT, rx);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	wmb();
	w8(TRIGGER, 0x80);
	
	if (wait_busy())
		return -1;
		
	if (mode16)
		fifo_read16(FIFO + 3, buf, rx);
	else
		fifo_read8(FIFO + 3, buf, rx);
		
	w8(OPCODE, 0x0B);
	return 0;
}

static int do_write(u8 op, const u8 *tx, int tx_len, bool mode16) {
	u32 c;
	
	c = r32(CTRL0); c &= ~BIT(20); w32(CTRL0, c); c |= BIT(20); w32(CTRL0, c);
	w8(OPCODE, op);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	
	w8(TX_COUNT, tx_len);
	if (mode16)
		fifo_write16(FIFO, tx, tx_len);
	else
		fifo_write8(FIFO, tx, tx_len);
		
	w8(RX_COUNT, 0);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	wmb();
	w8(TRIGGER, 0x80);
	
	if (wait_busy())
		return -1;
		
	w8(OPCODE, 0x0B);
	return 0;
}

static int __init cs0_init(void) {
	struct pci_dev *lpc;
	u32 orig_b8 = 0, orig_b4 = 0;
	u8 rx[64];
	int i;
	u8 descreq[] = { 0x00, 0x00, 0x01, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00 };
	
	struct test_case {
		bool mode16;
		u32 b4_val;
		const char *label;
	} cases[2] = {
		{ false, 0x00000000, "CS0: 8-bit FIFO" },
		{ true,  0x00000000, "CS0: 16-bit FIFO" },
	};

	pr_info("cs0t: ================================================================\n");
	pr_info("cs0t: CS0 CHIP SELECT TEST\n");
	pr_info("cs0t: ================================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if (!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if (!gpio) { iounmap(base); return -ENOMEM; }

	select_cs0();
	pr_info("cs0t: Configured ALT_CS to CS0: 0x%02x\n", r8(ALT_CS));

	lpc = pci_get_device(PCI_VENDOR_ID_AMD, 0x790E, NULL);
	if (!lpc) {
		pr_err("cs0t: LPC bridge not found\n");
		iounmap(gpio);
		iounmap(base);
		return -ENODEV;
	}

	pci_read_config_dword(lpc, 0xB4, &orig_b4);
	pci_read_config_dword(lpc, 0xB8, &orig_b8);

	for (i = 0; i < 2; i++) {
		struct test_case *c = &cases[i];
		u32 val_b8 = orig_b8;
		int round, t;
		bool success = false;

		pr_info("cs0t: --- CASE %d: %s ---\n", i, c->label);

		if (c->mode16)
			val_b8 |= 0x80;
		else
			val_b8 &= ~0x80;
		pci_write_config_dword(lpc, 0xB8, val_b8);
		pci_write_config_dword(lpc, 0xB4, c->b4_val);

		for (round = 0; round < 3; round++) {
			do_read(0x000000, 10, rx, c->mode16);
			if (hid_type(rx, 10) == 3) {
				do_read(0x000000, 20, rx, c->mode16);
			}

			(void)pin_asserted();

			if (do_write(0x02, descreq, sizeof(descreq), c->mode16) != 0) {
				pr_info("cs0t:     Round %d: write failed\n", round);
				continue;
			}

			if (wait_for_pin85()) {
				do_read(0x000000, 37, rx, c->mode16);
				t = hid_type(rx, 37);
				pr_info("cs0t:     Round %d: edge caught! type=%d(%s) raw=[%*ph]\n",
					round, t, tname(t), 37, rx);
				if (t == 7) {
					pr_info("cs0t:     *** SUCCESS! %s WORKED! ***\n", c->label);
					success = true;
					break;
				}
			} else {
				pr_info("cs0t:     Round %d: timed out waiting for edge\n", round);
			}
			msleep(100);
		}

		if (success)
			break;

		msleep(200);
	}

	pci_write_config_dword(lpc, 0xB4, orig_b4);
	pci_write_config_dword(lpc, 0xB8, orig_b8);
	pci_dev_put(lpc);

	w8(OPCODE, 0x0B);
	iounmap(gpio);
	iounmap(base);
	pr_info("cs0t: TEST COMPLETE\n");
	return -ENODEV;
}

static void __exit cs0_exit(void) {}

module_init(cs0_init);
module_exit(cs0_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CS0 chip select test");
