/* Full HID init sequence with ACPI _RST reset.
 * 1. Trigger _RST via ACPI (safe: only RESET line toggle, no power cycle)
 * 2. Drain RESET_RSP header + body (as Windows does)
 * 3. Send DESCREQ via iowrite8 TX
 * 4. Read back — expect DEVICE_DESC (type=7, 72 80 00 5A)
 *
 * insmod reset_test.ko && dmesg | grep "rst:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/acpi.h>

#define FCH_BASE 0xFEC10000

static void __iomem *base;
static acpi_handle dev_handle;

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

static void fifo_write8(int off, const u8 *d, int n) { int i; for(i=0;i<n;i++) w8(off+i,d[i]); }

#define CTRL0 0x00; #define OP45 0x45; #define TR47 0x47; #define TX48 0x48; #define RX4B 0x4B; #define FIFO 0x80; #define CS1D 0x1D; #define R22 0x22; #define R44 0x44

static void cs1(void) { w8(0x1D, (r8(0x1D) & 0xFC) | 0x01); }

static int wbusy(void) { u32 v; return readl_poll_timeout(base+0x00, v, !(v & BIT(31)), 20, 2000000); }

static void clrfifo(void) { u32 c=r32(0x00); w32(0x00,c&~BIT(20)); w32(0x00,c|BIT(20)); }

static void secret(void) { w32(0x00, r32(0x00) | 0x60040000); }

/* Read 9 bytes from register 0x000000 */
static int read9(u8 *rx)
{
	clrfifo(); w8(0x45,0x0B); secret();
	w8(0x48,3); fifo_write8(0x80, (u8[]){0,0,0},3); w8(0x4B,9);
	secret(); w8(0x47,0x80);
	if (wbusy()) return -1;
	fifo_read16(0x80+3, rx, 9);
	w8(0x45,0x0B); return 0;
}

/* Longer read */
static int readN(u8 *rx, int n)
{
	clrfifo(); w8(0x45,0x0B); secret();
	w8(0x48,3); fifo_write8(0x80, (u8[]){0,0,0},3); w8(0x4B,n);
	secret(); w8(0x47,0x80);
	if (wbusy()) return -1;
	fifo_read16(0x80+3, rx, n);
	w8(0x45,0x0B); return 0;
}

static int write02(const u8 *pl, int txlen)
{
	clrfifo(); w8(0x45,0x02); secret();
	/* 0x44 dance */
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,txlen); fifo_write8(0x80,pl,txlen); w8(0x4B,0); /* TX-only */
	secret(); w8(0x45,0x02);
	w8(0x47,0x80);
	if (wbusy()) return -1;
	w8(0x45,0x0B); return 0;
}

static void find_hid(const u8 *d, int n, const char *ctx)
{
	int i;
	for (i=3;i<n;i++) {
		if (d[i]==0x5A && (d[i-3]&0x0F)==2 && (d[i-2]&0x0F)==0) {
			int t=(d[i-3]>>4)&0xF; u16 bl=(((d[i-2]>>4)&0xF)|(d[i-1]<<4))*4;
			pr_info("rst:   HID %s type=%d(%s) blen=%u hdr=[%02x %02x %02x %02x]\n",
				ctx, t, t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?",
				bl, d[i-3],d[i-2],d[i-1],d[i]);
			return;
		}
	}
	pr_info("rst:   No HID header in %s (%d bytes)\n", ctx, n);
}

/* Call ACPI _RST method on MSHW0231 */
static int acpi_rst(acpi_handle h)
{
	acpi_status s = acpi_evaluate_object(h, "_RST", NULL, NULL);
	pr_info("rst: ACPI _RST -> %d\n", s);
	return s == AE_OK ? 0 : -EIO;
}

static int __init rst_init(void)
{
	u8 rx[64];

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;

	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &dev_handle))) {
		pr_err("rst: Cannot find HSPI ACPI handle\n");
		iounmap(base);
		return -ENODEV;
	}
	pr_info("rst: HSPI ACPI handle = %px\n", dev_handle);

	cs1();

	pr_info("rst: ===========================================\n");
	pr_info("rst: FULL INIT SEQUENCE WITH ACPI _RST\n");
	pr_info("rst: ===========================================\n");

	/* Step 1: Reset via _RST */
	pr_info("rst: [1] Calling _RST...\n");
	acpi_rst(dev_handle);
	msleep(350); /* _RST does 300ms internally, extra margin */

	/* Step 2: Drain RESET_RSP header (9 bytes, Windows style) */
	pr_info("rst: [2] Reading RESET_RSP header (9B)...\n");
	memset(rx,0xCC,64);
	read9(rx);
	pr_info("rst:   raw=[%*ph]\n", 9, rx);
	find_hid(rx, 9, "hdr");

	/* Step 3: Drain RESET_RSP body (+170µs, Windows does this) */
	pr_info("rst: [3] Reading RESET_RSP body (+170us)...\n");
	udelay(170);
	memset(rx,0xCC,64);
	read9(rx);
	pr_info("rst:   raw=[%*ph]\n", 9, rx);
	find_hid(rx, 9, "body");

	/* Step 4: Send DESCREQ (TX-only, RX_COUNT=0) */
	pr_info("rst: [4] Sending DESCREQ @0x000001 (TX-only)...\n");
	{
		u8 dr[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		u32 c0_pre=r32(0x00);
		write02(dr, sizeof(dr));
		pr_info("rst:   pre-trig c0=0x%08x -> post c0=0x%08x\n", c0_pre, r32(0x00));
	}

	/* Step 5: Wait, then read response header */
	pr_info("rst: [5] Waiting + reading post-DESCREQ (9B)...\n");
	udelay(200);
	memset(rx,0xCC,64);
	read9(rx);
	pr_info("rst:   raw=[%*ph]\n", 9, rx);
	find_hid(rx, 9, "post1");

	/* Step 6: Longer wait + longer read */
	pr_info("rst: [6] 2ms wait + 37B read...\n");
	udelay(2000);
	memset(rx,0xCC,64);
	readN(rx, 37);
	pr_info("rst:   raw=[%*ph]\n", 37, rx);
	find_hid(rx, 37, "post2");

	/* Final: full FIFO dump */
	{
		u8 f[70];
		pr_info("rst: [7] Full FIFO dump:\n");
		fifo_read16(0x80, f, 70);
		pr_info("rst:   [%*ph]\n", 70, f);
	}

	w8(0x45,0x0B);
	iounmap(base);
	pr_info("rst: TEST COMPLETE\n");
	return -ENODEV;
}

module_init(rst_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Full HID init with ACPI _RST reset");
