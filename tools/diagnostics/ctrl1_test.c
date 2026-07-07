/* Quick test: write CTRL1[15:8]=0x06, then test DESCREQ.
 * Windows has CTRL1=0x020006B5, Linux has 0x020007ff.
 * This module forces bits[15:8] to the Windows value.
 *
 * insmod ctrl1_test.ko && dmesg | grep "c1t:"
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

static void fifo_rx8(int off, u8 *d, int n) { int i; for(i=0;i<n;i++) d[i]=r8(off+i); }
static void fifo_tx8(int off, const u8 *d, int n) { int i; for(i=0;i<n;i++) w8(off+i,d[i]); }

#define CTRL0 0x00
#define CTRL1 0x0C
#define OP45  0x45
#define TR47  0x47
#define TX48  0x48
#define RX4B  0x4B
#define FIFO  0x80
#define CS1D  0x1D
#define R22   0x22
#define R44   0x44

static void cs1(void) { w8(CS1D, (r8(CS1D) & 0xFC) | 0x01); }
static int wbusy(void) { u32 v; return readl_poll_timeout(base+CTRL0, v, !(v & BIT(31)), 20, 2000000); }
static void clrfifo(void) { u32 c=r32(CTRL0); w32(CTRL0,c&~BIT(20)); w32(CTRL0,c|BIT(20)); }
static void secret(void) { w32(CTRL0, r32(CTRL0) | 0x60040000); }

static int readN(u8 *rx, int n)
{
	clrfifo(); w8(OP45,0x0B); secret();
	w8(TX48,3); fifo_tx8(FIFO,(u8[]){0,0,0},3); w8(RX4B,n);
	secret(); w8(TR47,0x80);
	if (wbusy()) return -1;
	fifo_rx8(FIFO+3, rx, n);
	w8(OP45,0x0B); return 0;
}

static int write02(const u8 *pl, int txlen)
{
	clrfifo(); w8(OP45,0x02); secret();
	{ u16 r44=r16(R44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(R44,r44); w8(OP45,0x02); }
	w8(TX48,txlen); fifo_tx8(FIFO,pl,txlen); w8(RX4B,0);
	secret(); w8(OP45,0x02);
	w8(TR47,0x80); return wbusy();
}

static int __init c1t_init(void)
{
	u8 rx[64];
	u32 c1;
	int i;

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;

	cs1();

	pr_info("c1t: =================================================\n");
	pr_info("c1t: CTRL1 WRITE TEST → DESCREQ\n");
	pr_info("c1t: =================================================\n");

	/* Start in 8-bit mode for clean reads */
	pr_info("c1t: Initial CTRL1=0x%08x  CTRL0=0x%08x\n", r32(CTRL1), r32(CTRL0));

	/* ===== STEP 1: Set CTRL1 bits[15:8] = 0x06 (Windows) ===== */
	c1 = r32(CTRL1);
	pr_info("c1t: [1] Writing CTRL1 with bits[15:8]=0x06...\n");
	pr_info("c1t:     Before: 0x%08x\n", c1);

	/* Clear bits[15:8] first, then set to 0x06 */
	w32(CTRL1, (c1 & 0xFFFF00FF) | 0x00000600);
	c1 = r32(CTRL1);
	pr_info("c1t:     After write(0x06): 0x%08x\n", c1);

	/* Try writing the FULL Windows value including bits[7:0] */
	w32(CTRL1, 0x020006B5);
	c1 = r32(CTRL1);
	pr_info("c1t:     After write(0x06B5): 0x%08x\n", c1);

	/* Try different bit patterns for bits[15:8] to see which stick */
	{
		int j;
		for (j = 0; j < 16; j++) {
			u32 try_val = (r32(CTRL1) & 0xFFFF00FF) | (j << 8);
			w32(CTRL1, try_val);
		}
		c1 = r32(CTRL1);
		pr_info("c1t:     After sweeping 0x00-0x0F: 0x%08x (bits[15:8]=0x%02x)\n",
			c1, (c1 >> 8) & 0xFF);
	}

	/* Force to 0x06 one more time */
	w32(CTRL1, 0x020006B5);
	c1 = r32(CTRL1);
	pr_info("c1t:     Final CTRL1: 0x%08x\n", c1);

	/* ===== STEP 2: Read RESET_RSP ===== */
	pr_info("c1t: [2] Reading RESET_RSP...\n");
	for (i = 0; i < 5; i++) {
		msleep(30);
		memset(rx, 0xCC, sizeof(rx));
		if (readN(rx, 32) < 0) continue;
		{
			int k;
			for (k = 3; k < 32; k++) {
				if (rx[k] == 0x5A && (rx[k-3] & 0x0F) == 2 && (rx[k-2] & 0x0F) == 0) {
					int t = (rx[k-3] >> 4) & 0xF;
					pr_info("c1t:   HID type=%d hdr=[%02x %02x %02x %02x]\n",
						t, rx[k-3], rx[k-2], rx[k-1], rx[k]);
					break;
				}
			}
		}
	}

	/* ===== STEP 3: Send DESCREQ with CTRL1 = Windows-like ===== */
	pr_info("c1t: [3] Sending DESCREQ with CTRL1=0x%08x...\n", r32(CTRL1));
	{
		u8 dr[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		write02(dr, sizeof(dr));
	}

	/* ===== STEP 4: Read post-DESCREQ response ===== */
	pr_info("c1t: [4] Reading post-DESCREQ response...\n");
	for (i = 0; i < 8; i++) {
		msleep(30);
		memset(rx, 0xCC, sizeof(rx));
		if (readN(rx, 20) < 0) continue;
		{
			int k;
			for (k = 3; k < 20; k++) {
				if (rx[k] == 0x5A && (rx[k-3] & 0x0F) == 2 && (rx[k-2] & 0x0F) == 0) {
					int t = (rx[k-3] >> 4) & 0xF;
					u16 bl = (((rx[k-2] >> 4) & 0xF) | (rx[k-1] << 4)) * 4;
					pr_info("c1t:   *** HID type=%d(%s) body_len=%u hdr=[%02x %02x %02x %02x]\n",
						t, t==7?"DEVICE_DESC":t==3?"RESET_RSP":"?",
						bl, rx[k-3], rx[k-2], rx[k-1], rx[k]);
					goto done;
				}
			}
		}
	}
	pr_info("c1t:   No HID response found\n");

done:
	pr_info("c1t: Final CTRL1=0x%08x CTRL0=0x%08x\n", r32(CTRL1), r32(CTRL0));
	iounmap(base);
	pr_info("c1t: =================================================\n");
	return -ENODEV;
}

module_init(c1t_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CTRL1 write test for SL4A");
