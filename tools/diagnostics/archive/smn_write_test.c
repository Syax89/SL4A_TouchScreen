/* Write Windows SMN values to FCH registers, then test DESCREQ.
 * The SMN diff between Windows/Linux might affect SPI controller.
 * insmod smn_write_test.ko && dmesg | grep "swt:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <asm/amd/node.h>

#define FCH_BASE 0xFEC10000
static void __iomem *base;

#define w32(o,v) iowrite32(v, base+(o))
#define r32(o)   ioread32(base+(o))
#define w16(o,v) iowrite16(v, base+(o))
#define r16(o)   ioread16(base+(o))
#define w8(o,v)  iowrite8(v, base+(o))
#define r8(o)    ioread8(base+(o))

static void fifo_rx8(int o, u8 *d, int n) { int i; for(i=0;i<n;i++) d[i]=r8(o+i); }
static void fifo_tx8(int o, const u8 *d, int n) { int i; for(i=0;i<n;i++) w8(o+i,d[i]); }

#define CTRL0 0x00; #define OP45 0x45; #define TR47 0x47; #define TX48 0x48; #define RX4B 0x4B; #define FIFO 0x80; #define CS1D 0x1D; #define R22 0x22; #define R44 0x44

static void cs1(void) { w8(0x1D, (r8(0x1D) & 0xFC) | 0x01); }
static int wbusy(void) { u32 v; return readl_poll_timeout(base+0x00, v, !(v & BIT(31)), 20, 2000000); }
static void clrfifo(void) { u32 c=r32(0x00); w32(0x00,c&~BIT(20)); w32(0x00,c|BIT(20)); }
static void secret(void) { w32(0x00, r32(0x00) | 0x60040000); }

static int readN(u8 *rx, int n) {
	clrfifo(); w8(0x45,0x0B); secret();
	w8(0x48,3); fifo_tx8(0x80,(u8[]){0,0,0},3); w8(0x4B,n);
	secret(); w8(0x47,0x80);
	if(wbusy()) return -1;
	fifo_rx8(0x80+3, rx, n);
	w8(0x45,0x0B); return 0;
}

static int write02(const u8 *pl, int txlen) {
	clrfifo(); w8(0x45,0x02); secret();
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,txlen); fifo_tx8(0x80,pl,txlen); w8(0x4B,0);
	secret(); w8(0x45,0x02);
	w8(0x47,0x80); return wbusy();
}

static int __init swt_init(void)
{
	u8 rx[64];
	u32 v_before, v_after, win_val;
	int i;

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;
	cs1();

	pr_info("swt: ================================================\n");
	pr_info("swt: SMN WRITE TEST — Apply Windows values\n");
	pr_info("swt: ================================================\n");

	/* Windows SMN values to apply */
	struct { u32 addr; u32 win_val; } smn_v[] = {
		{0x03B10528, 0x00000059},
		{0x03B10998, 0x00000006},
		{0x03B1099C, 0x00000102},
		{0x03B109A0, 0x0000000D},
		{0x03B109A4, 0x00000001},
		{0x03B109A8, 0x0001C324},
	};

	/* Write Windows values */
	for (i = 0; i < ARRAY_SIZE(smn_v); i++) {
		amd_smn_read(0, smn_v[i].addr, &v_before);
		amd_smn_write(0, smn_v[i].addr, smn_v[i].win_val);
		amd_smn_read(0, smn_v[i].addr, &v_after);
		pr_info("swt: SMN[0x%08X]: before=0x%08X -> wrote=0x%08X -> after=0x%08X %s\n",
			smn_v[i].addr, v_before, smn_v[i].win_val, v_after,
			v_after == smn_v[i].win_val ? "OK" : "REJECTED!");
	}

	/* Dump MMIO after SMN changes */
	pr_info("swt: After SMN writes: CTRL0=0x%08X CTRL1=0x%08X\n", r32(0x00), r32(0x0C));

	/* Try DESCREQ */
	pr_info("swt: Sending DESCREQ...\n");
	{
		u8 dr[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		write02(dr, sizeof(dr));
	}

	/* Read response */
	pr_info("swt: Reading response...\n");
	for (i = 0; i < 5; i++) {
		msleep(30);
		memset(rx,0xCC,sizeof(rx));
		if (readN(rx, 20) < 0) continue;
		{ int k; for(k=3;k<20;k++) {
			if(rx[k]==0x5A && (rx[k-3]&0x0F)==2 && (rx[k-2]&0x0F)==0) {
				int t=(rx[k-3]>>4)&0xF; u16 bl=(((rx[k-2]>>4)&0xF)|(rx[k-1]<<4))*4;
				pr_info("swt: *** HID type=%d(%s) blen=%u hdr=[%02x %02x %02x %02x]\n",
					t, t==7?"DEVICE_DESC":t==3?"RESET_RSP":"?",
					bl, rx[k-3],rx[k-2],rx[k-1],rx[k]);
				goto done;
			}
		}}
	}
	pr_info("swt: No HID response\n");

done:
	w8(0x45,0x0B);
	iounmap(base);
	pr_info("swt: DONE\n");
	return -ENODEV;
}
module_init(swt_init);
MODULE_LICENSE("GPL");
