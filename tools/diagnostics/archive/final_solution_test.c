/* tools/diagnostics/final_solution_test.c
 * Final verification of the touchscreen write path on SL4 AMD.
 *
 * This test:
 * 1. Performs a clean ACPI _PS3 -> _PS0 power cycle to reset the device.
 * 2. Waits for the initial RESET_RSP on pin 85.
 * 3. Reads the RESET_RSP (header + body) in two separate transactions.
 * 4. Sends the 11-byte DESCREQ write transaction.
 * 5. Waits for the response on pin 85.
 * 6. Reads the response header.
 *
 * We test this in 8-bit FIFO mode and 16-bit FIFO mode.
 *
 * insmod final_solution_test.ko && dmesg | grep "sol:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/acpi.h>

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
#define CTRL1 0x0C
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
static void select_cs1(void){ u8 v=r8(ALT_CS); w8(ALT_CS,(v&0xFC)|0x01); }
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

static bool wait_for_pin85(int timeout_ms) {
	int tries;
	for (tries = 0; tries < timeout_ms; tries++) {
		if (pin_asserted())
			return true;
		msleep(1);
	}
	return false;
}

/* Perform read operation */
static int do_read(u32 addr, int rx, u8 *buf, bool mode16) {
	u32 c;
	u8 tx[3] = { (addr >> 16) & 0xFF, (addr >> 8) & 0xFF, addr & 0xFF };
	
	c = r32(CTRL0); c &= ~BIT(20); w32(CTRL0, c); c |= BIT(20); w32(CTRL0, c);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	
	w8(TX_COUNT, 3);
	if (mode16)
		fifo_write16(FIFO, tx, 3);
	else
		fifo_write8(FIFO, tx, 3);
		
	w8(RX_COUNT, rx);
	
	w8(OPCODE, 0x0B);
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

/* Perform write operation */
static int do_write(u8 op, const u8 *tx, int tx_len, bool mode16) {
	u32 c;
	
	c = r32(CTRL0); c &= ~BIT(20); w32(CTRL0, c); c |= BIT(20); w32(CTRL0, c);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	
	w8(TX_COUNT, tx_len);
	if (mode16)
		fifo_write16(FIFO, tx, tx_len);
	else
		fifo_write8(FIFO, tx, tx_len);
		
	w8(RX_COUNT, 0);
	
	w8(OPCODE, op);
	c = r32(CTRL0); c |= 0x60040000; w32(CTRL0, c);
	
	w8(OPCODE, op);
	wmb();
	w8(TRIGGER, 0x80);
	
	if (wait_busy())
		return -1;
		
	w8(OPCODE, 0x0B);
	return 0;
}

static int run_test(acpi_handle hspi, struct pci_dev *lpc, bool mode16, u32 orig_b8, const char *label) {
	u8 rx[64];
	u32 val_b8 = orig_b8;
	int t;

	/* The actual FIFO payload starts with 0x02 (the opcode) followed by register address 0x000001, etc. */
	u8 descreq[] = { 0x02, 0x00, 0x00, 0x01, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00 };

	pr_info("sol: --- Testing %s ---\n", label);

	/* 1. Set FIFO Mode */
	if (mode16)
		val_b8 |= 0x80;
	else
		val_b8 &= ~0x80;
	pci_write_config_dword(lpc, 0xB8, val_b8);

	/* 2. Perform ACPI power cycle to reset device */
	pr_info("sol:   Power cycling device via ACPI _PS3 -> _PS0...\n");
	acpi_evaluate_object(hspi, "_PS3", NULL, NULL);
	msleep(50);
	acpi_evaluate_object(hspi, "_PS0", NULL, NULL);

	/* 3. Wait for Pin 85 interrupt edge (initial RESET_RSP) */
	pr_info("sol:   Waiting for initial RESET_RSP interrupt...\n");
	if (!wait_for_pin85(500)) {
		pr_err("sol:   Timeout waiting for initial RESET_RSP interrupt!\n");
		return -ETIMEDOUT;
	}

	/* 4. Read RESET_RSP header (10 bytes: 5 prefix + 4 header + 1 sync byte) */
	memset(rx, 0, sizeof(rx));
	if (do_read(0x000000, 10, rx, mode16) != 0) {
		pr_err("sol:   Read RESET_RSP header failed!\n");
		return -EIO;
	}
	t = hid_type(rx, 10);
	pr_info("sol:   Read initial response header: type=%d(%s) raw=[%*ph]\n",
		t, tname(t), 10, rx);
	if (t != 3) {
		pr_err("sol:   Expected RESET_RSP (type 3), got type %d!\n", t);
		return -EINVAL;
	}

	/* 5. Read RESET_RSP body (10 bytes: 5 prefix + 4 body + 1 padding/sync) */
	memset(rx, 0, sizeof(rx));
	if (do_read(0x000000, 10, rx, mode16) != 0) {
		pr_err("sol:   Read RESET_RSP body failed!\n");
		return -EIO;
	}
	pr_info("sol:   Read initial response body: raw=[%*ph]\n", 10, rx);

	/* 6. Wait a tiny bit (like Windows does) */
	udelay(100);

	/* 7. Write DESCREQ (total 11 bytes: opcode 02 + 10 bytes payload) */
	pr_info("sol:   Sending DESCREQ write...\n");
	if (do_write(0x02, descreq, sizeof(descreq), mode16) != 0) {
		pr_err("sol:   DESCREQ write failed!\n");
		return -EIO;
	}

	/* 8. Wait for Pin 85 interrupt (DEVICE_DESC response) */
	pr_info("sol:   Waiting for response interrupt...\n");
	if (!wait_for_pin85(200)) {
		pr_err("sol:   Timeout waiting for response interrupt!\n");
		return -ETIMEDOUT;
	}

	/* 9. Read response header (10 bytes: 5 prefix + 4 header + 1 sync byte) */
	memset(rx, 0, sizeof(rx));
	if (do_read(0x000000, 10, rx, mode16) != 0) {
		pr_err("sol:   Read response header failed!\n");
		return -EIO;
	}
	t = hid_type(rx, 10);
	pr_info("sol:   *** RESPONSE RECEIVED: type=%d(%s) raw=[%*ph] ***\n",
		t, tname(t), 10, rx);

	if (t == 7) {
		pr_info("sol:   *** SUCCESS! DEVICE RETURNED DEVICE_DESC (type 7) ***\n");
		return 0;
	} else {
		pr_err("sol:   Device returned type %d(%s) instead of DEVICE_DESC (type 7)!\n",
			t, tname(t));
		return -EINVAL;
	}
}

static int __init sol_init(void) {
	struct pci_dev *lpc;
	acpi_handle hspi;
	acpi_status st;
	u32 orig_b8 = 0, orig_b4 = 0, orig_ctrl1 = 0;

	pr_info("sol: ================================================================\n");
	pr_info("sol: FINAL SOLUTION TEST\n");
	pr_info("sol: ================================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if (!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if (!gpio) { iounmap(base); return -ENOMEM; }

	select_cs1();

	/* Read and overwrite CTRL1 with Windows timings (0x020006B5) */
	orig_ctrl1 = r32(CTRL1);
	w32(CTRL1, 0x020006B5);

	st = acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &hspi);
	if (ACPI_FAILURE(st)) {
		pr_err("sol: could not get HSPI handle: %s\n", acpi_format_exception(st));
		w32(CTRL1, orig_ctrl1);
		iounmap(gpio);
		iounmap(base);
		return -ENODEV;
	}

	lpc = pci_get_device(PCI_VENDOR_ID_AMD, 0x790E, NULL);
	if (!lpc) {
		pr_err("sol: LPC bridge not found\n");
		w32(CTRL1, orig_ctrl1);
		iounmap(gpio);
		iounmap(base);
		return -ENODEV;
	}

	pci_read_config_dword(lpc, 0xB4, &orig_b4);
	pci_read_config_dword(lpc, 0xB8, &orig_b8);

	/* Set B4 to Windows value */
	pci_write_config_dword(lpc, 0xB4, 0x7DFFE000);

	/* Test Case 1: 8-bit FIFO Mode */
	run_test(hspi, lpc, false, orig_b8, "8-bit FIFO Mode");

	/* Test Case 2: 16-bit FIFO Mode */
	run_test(hspi, lpc, true, orig_b8, "16-bit FIFO Mode");

	/* Restore registers */
	pci_write_config_dword(lpc, 0xB4, orig_b4);
	pci_write_config_dword(lpc, 0xB8, orig_b8);
	pci_dev_put(lpc);

	w32(CTRL1, orig_ctrl1);

	w8(OPCODE, 0x0B);
	iounmap(gpio);
	iounmap(base);
	pr_info("sol: TEST COMPLETE\n");
	return -ENODEV;
}

static void __exit sol_exit(void) {}

module_init(sol_init);
module_exit(sol_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Touchscreen write path final solution test");
