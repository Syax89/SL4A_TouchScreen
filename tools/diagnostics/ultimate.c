/* ULTIMATE TEST: does DESCREQ actually work but response is at wrong offset?
 * Write DESCREQ, then dump ENTIRE 70-byte FIFO and search for ANY 0x5A marker.
 * Also try reading from register 0x0003 (output register from device descriptor).
 *
 * insmod ultimate.ko && dmesg | grep "ult:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>

#define FCH_BASE 0xFEC10000
static void __iomem *base;

#define w32(o,v) iowrite32(v,base+(o))
#define r32(o)   ioread32(base+(o))
#define w16(o,v) iowrite16(v,base+(o))
#define r16(o)   ioread16(base+(o))
#define w8(o,v)  iowrite8(v,base+(o))
#define r8(o)    ioread8(base+(o))

static int wbusy(void) { u32 v; return readl_poll_timeout(base, v, !(v & BIT(31)), 20, 2000000); }

static void dump_fifo_full(const char *tag)
{
	u8 f[70]; int i;
	for(i=0;i<70;i++) f[i]=r8(0x80+i);
	pr_info("ult: FIFO %s [0..69]=[%*ph]\n", tag, 70, f);
	for(i=3;i<70;i++) {
		if(f[i]==0x5A && (f[i-3]&0x0F)==2 && (f[i-2]&0x0F)==0) {
			int t=(f[i-3]>>4)&0xF; u16 bl=(((f[i-2]>>4)&0xF)|(f[i-1]<<4))*4;
			pr_info("ult: *** FIFO HDR @%d: type=%d(%s) blen=%u [%02x %02x %02x %02x]\n",
				i-3, t, t==1?"DATA":t==3?"RESET":t==7?"DEV_DESC":t==8?"RPT_DESC":"?",
				bl, f[i-3],f[i-2],f[i-1],f[i]);
		}
	}
}

static void do_read(u32 addr, int n, u8 *rx)
{
	int i;
	w32(0x00, r32(0x00) & ~BIT(20)); w32(0x00, r32(0x00) | BIT(20)); /* clr fifo */
	w8(0x45,0x0B); w32(0x00, r32(0x00) | 0x60040000);
	w8(0x48,3);
	w8(0x80,(addr>>16)&0xFF); w8(0x81,(addr>>8)&0xFF); w8(0x82,addr&0xFF);
	w8(0x4B,n);
	w32(0x00, r32(0x00) | 0x60040000);
	w8(0x47,0x80); wbusy();
	for(i=0;i<n;i++) rx[i]=r8(0x80+3+i);
	w8(0x45,0x0B);
}

static void do_write(u8 opcode, const u8 *pl, int txlen)
{
	w32(0x00, r32(0x00) & ~BIT(20)); w32(0x00, r32(0x00) | BIT(20));
	w8(0x45,opcode); w32(0x00, r32(0x00) | 0x60040000);
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,opcode); }
	w8(0x48,txlen); {int i;for(i=0;i<txlen;i++)w8(0x80+i,pl[i]);}
	w8(0x4B,0); /* TX-only */
	w32(0x00, r32(0x00) | 0x60040000); w8(0x45,opcode);
	w8(0x47,0x80); wbusy();
	w8(0x45,0x0B);
}

static int __init ult_init(void)
{
	u8 rx[128], dr[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
	int i;

	base = ioremap(FCH_BASE,0x100);
	if(!base) return -ENOMEM;
	w8(0x1D,(r8(0x1D)&0xFC)|0x01);

	pr_info("ult: ================================================\n");
	pr_info("ult: ULTIMATE FIFO HUNT — where is the response?\n");
	pr_info("ult: ================================================\n");

	dump_fifo_full("INITIAL");

	/* Drain any pending RESET_RSP */
	pr_info("ult: [1] Draining RESET_RSP with 70B read...\n");
	do_read(0x000000, 70, rx);
	dump_fifo_full("POST-DRAIN");

	/* Send DESCREQ */
	pr_info("ult: [2] Sending DESCREQ @0x000001...\n");
	do_write(0x02, dr, sizeof(dr));
	pr_info("ult:     CTRL0 post-write=0x%08X CTRL1=0x%08X\n", r32(0x00), r32(0x0C));

	/* Dump FIFO immediately after write (MISO captured even with RX_COUNT=0?) */
	dump_fifo_full("POST-DESCREQ(immediate)");

	/* Wait various delays and dump */
	msleep(1);
	dump_fifo_full("POST-DESCREQ(+1ms)");

	msleep(5);
	dump_fifo_full("POST-DESCREQ(+6ms)");

	/* Now do a read from input register 0x000000 and capture 70 bytes */
	pr_info("ult: [3] 70B read from 0x000000...\n");
	do_read(0x000000, 70, rx);
	dump_fifo_full("POST-READ(0x000000)");

	/* Read from output register 0x000003 (device descriptor says this is the output reg) */
	pr_info("ult: [4] 70B read from 0x000003 (output register)...\n");
	do_read(0x000003, 70, rx);
	dump_fifo_full("POST-READ(0x000003)");

	/* Read from register 0x000002 (report descriptor register) */
	pr_info("ult: [5] 70B read from 0x000002...\n");
	do_read(0x000002, 70, rx);
	dump_fifo_full("POST-READ(0x000002)");

	/* Try DESCREQ again with different register */
	pr_info("ult: [6] Sending write to 0x000003...\n");
	{ u8 w3[] = {0x00,0x00,0x03, 0x42,0x00, 0x00,0x03,0x00,0x00}; do_write(0x02,w3,sizeof(w3)); }
	msleep(5);
	dump_fifo_full("POST-WRITE(0x000003)");

	pr_info("ult: [7] 70B read from 0x000000...\n");
	do_read(0x000000, 70, rx);
	dump_fifo_full("POST-READ2(0x000000)");

	/* ===== RADICAL: V1-mode write (opcode in CTRL0[7:0], trigger via BIT16) ===== */
	pr_info("ult: [8] V1-MODE WRITE: opcode in CTRL0[7:0], trigger via BIT16...\n");
	{
		u8 dr[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		u32 c0;
		int k;

		w32(0x00, r32(0x00) & ~BIT(20));
		w32(0x00, r32(0x00) | BIT(20));

		c0 = r32(0x00);
		c0 = (c0 & ~0xFF) | 0x02;  /* opcode 0x02 in CTRL0[7:0] */
		c0 |= 0x60040000;           /* secret bits */
		w32(0x00, c0);

		w8(0x48, sizeof(dr));      /* TX_COUNT */
		for(k=0;k<sizeof(dr);k++) w8(0x80+k, dr[k]);
		w8(0x4B, 0);               /* RX_COUNT=0 */

		c0 = r32(0x00);
		c0 |= 0x60040000;
		w32(0x00, c0);

		pr_info("ult:     V1 pre-trig CTRL0=0x%08X\n", r32(0x00));

		/* V1 trigger: toggle BIT16 */
		c0 = r32(0x00);
		c0 &= ~BIT(16);
		w32(0x00, c0);
		c0 |= BIT(16);
		w32(0x00, c0);

		wbusy();
		pr_info("ult:     V1 write done, CTRL0=0x%08X\n", r32(0x00));
	}
	dump_fifo_full("POST-V1-WRITE");

	/* Wait and read */
	msleep(10);
	do_read(0x000000, 70, rx);
	dump_fifo_full("POST-V1-READ(0x000000)");

	/* ===== RADICAL 2: Read with TX_COUNT=4 (include 0xFF dummy byte) ===== */
	pr_info("ult: [9] READ with TX_COUNT=4 (address + 0xFF dummy)...\n");
	{
		int k;
		w32(0x00, r32(0x00) & ~BIT(20));
		w32(0x00, r32(0x00) | BIT(20));
		w8(0x45,0x0B);
		w32(0x00, r32(0x00) | 0x60040000);
		w8(0x48, 4);  /* TX_COUNT=4 */
		w8(0x80,0x00); w8(0x81,0x00); w8(0x82,0x00); w8(0x83,0xFF);
		w8(0x4B, 70);
		w32(0x00, r32(0x00) | 0x60040000);
		w8(0x47,0x80);
		wbusy();
		for(k=0;k<70;k++) rx[k]=r8(0x80+4+k);
	}
	dump_fifo_full("POST-READ-TX4");

	/* ===== RADICAL 3: V1-mode everything (read AND write) ===== */
	pr_info("ult: [10] V1-MODE READ from 0x000000...\n");
	{
		u32 c0; int k;
		w32(0x00, r32(0x00) & ~BIT(20));
		w32(0x00, r32(0x00) | BIT(20));
		c0 = r32(0x00);
		c0 = (c0 & ~0xFF) | 0x0B;  /* opcode 0x0B in CTRL0[7:0] */
		c0 |= 0x60040000;
		w32(0x00, c0);
		w8(0x48, 4);  /* TX_COUNT=4 (address + 0xFF) */
		w8(0x80,0x00); w8(0x81,0x00); w8(0x82,0x00); w8(0x83,0xFF);
		w8(0x4B, 70);
		c0 = r32(0x00); c0 |= 0x60040000; w32(0x00, c0);
		/* V1 trigger */
		c0 = r32(0x00); c0 &= ~BIT(16); w32(0x00, c0);
		c0 |= BIT(16); w32(0x00, c0);
		wbusy();
		for(k=0;k<70;k++) rx[k]=r8(0x80+4+k);
	}
	dump_fifo_full("POST-V1-READ");

	/* ===== RADICAL 4: Try DESCREQ to EVERY register 0x00-0x04 ===== */
	pr_info("ult: [11] DESCREQ sweep: trying registers 0x00-0x04...\n");
	{
		int reg;
		for (reg = 0; reg <= 5; reg++) {
			u8 dr[] = {reg>>16, (reg>>8)&0xFF, reg&0xFF, 0x42,0x00, 0x00,0x03,0x00,0x00};
			u32 c0; int k;
			pr_info("ult:     DESCREQ to reg 0x%06X...\n", reg);
			/* Use V2 write */
			w32(0x00, r32(0x00) & ~BIT(20));
			w32(0x00, r32(0x00) | BIT(20));
			w8(0x45,0x02);
			w32(0x00, r32(0x00) | 0x60040000);
			{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
			  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
			  w16(0x44,r44); w8(0x45,0x02); }
			w8(0x48,sizeof(dr));
			for(k=0;k<sizeof(dr);k++) w8(0x80+k,dr[k]);
			w8(0x4B,0);
			w32(0x00, r32(0x00) | 0x60040000);
			w8(0x45,0x02);
			w8(0x47,0x80);
			wbusy();
			msleep(5);
			/* Read 70 bytes and search for any HID response */
			{
				u8 rd[70]; int m;
				for(m=0;m<70;m++) rd[m]=0xCC;
				do_read(0x000000, 70, rx);
				/* Check raw FIFO */
				for(m=0;m<70;m++) rd[m]=r8(0x80+m);
				for(m=3;m<70;m++) {
					if(rd[m]==0x5A && (rd[m-3]&0x0F)==2 && (rd[m-2]&0x0F)==0) {
						int t=(rd[m-3]>>4)&0xF;
						pr_info("ult:       *** RESPONSE to reg 0x%X: type=%d(%s)\n",
							reg, t, t==7?"DEVICE_DESC!":t==3?"RESET_RSP":t==1?"DATA":"?");
					}
				}
			}
			w8(0x45,0x0B);
		}
	}

	w8(0x45,0x0B);
	iounmap(base);
	pr_info("ult: DONE — check FIFO dumps for 0x5A markers\n");
	return -ENODEV;
}
module_init(ult_init);
MODULE_LICENSE("GPL");
