/* Clone Windows MMIO state and test DESCREQ.
 * Sets ALL writable MMIO registers to match Windows IDLE dump,
 * then runs full init sequence including vendor init.
 *
 * insmod clone_win.ko && dmesg | grep "clw:"
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

static void frx8(int o, u8 *d, int n) { int i; for(i=0;i<n;i++) d[i]=r8(o+i); }
static void ftx8(int o, const u8 *d, int n) { int i; for(i=0;i<n;i++) w8(o+i,d[i]); }

static int wbusy(void) { u32 v; return readl_poll_timeout(base+0x00, v, !(v & BIT(31)), 20, 2000000); }

static int readN(u8 *rx, int n) {
	u32 c = r32(0x00); w32(0x00, c & ~BIT(20)); w32(0x00, c | BIT(20));
	w8(0x45,0x0B); w32(0x00, r32(0x00) | 0x60040000);
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,n);
	w32(0x00, r32(0x00) | 0x60040000);
	w8(0x47,0x80); if(wbusy()) return -1;
	frx8(0x80+3, rx, n);
	w8(0x45,0x0B); return 0;
}

static int writeN(u8 opcode, const u8 *pl, int txlen) {
	u32 c = r32(0x00); w32(0x00, c & ~BIT(20)); w32(0x00, c | BIT(20));
	w8(0x45,opcode); w32(0x00, r32(0x00) | 0x60040000);
	/* 0x44 dance */
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,opcode); }
	w8(0x48,txlen); ftx8(0x80,pl,txlen); w8(0x4B,0);
	w32(0x00, r32(0x00) | 0x60040000); w8(0x45,opcode);
	w8(0x47,0x80); return wbusy();
}

static void dump_mmio(const char *tag)
{
	int i;
	pr_info("clw: %s MMIO dump:\n", tag);
	pr_info("clw:   CTRL0=0x%08X CTRL1=0x%08X ALT_CS=0x%02X\n", r32(0x00), r32(0x0C), r8(0x1D));
	pr_info("clw:   ENA=0x%08X R22=0x%04X R44=0x%04X OP45=0x%02X TR47=0x%02X\n",
		r32(0x20), r16(0x22), r16(0x44), r8(0x45), r8(0x47));
	pr_info("clw:   TX48=%u RX4B=%u STATUS=0x%08X\n", r8(0x48), r8(0x4B), r32(0x4C));
	pr_info("clw:   HOST_PREF=0x%08X MISC=0x%04X\n", r32(0x2C), r16(0xFC));
	/* FIFO first 32 bytes */
	{ u8 f[32]; frx8(0x80, f, 32);
	  pr_info("clw:   FIFO[0..31]=[%*ph]\n", 32, f); }
}

/* Find HID header in buffer */
static int find_hid(const u8 *d, int n, const char *ctx)
{
	int k;
	for(k=3;k<n;k++) {
		if(d[k]!=0x5A || (d[k-3]&0x0F)!=2 || (d[k-2]&0x0F)) continue;
		int t=(d[k-3]>>4)&0xF; u16 bl=(((d[k-2]>>4)&0xF)|(d[k-1]<<4))*4;
		pr_info("clw: *** HID %s: type=%d(%s) blen=%u hdr=[%02x %02x %02x %02x]\n",
			ctx, t, t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?",
			bl, d[k-3],d[k-2],d[k-1],d[k]);
		return t;
	}
	return -1;
}

static int __init clw_init(void)
{
	u8 rx[128];
	int i;

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;

	/* Select CS1 */
	w8(0x1D, (r8(0x1D) & 0xFC) | 0x01);

	dump_mmio("BEFORE");

	pr_info("clw: ==================================================\n");
	pr_info("clw: CLONING WINDOWS MMIO STATE\n");
	pr_info("clw: ==================================================\n");

	/* ===== CLONE WINDOWS IDLE MMIO STATE =====
	 * From MMIO_IDLE_V2.TXT (Windows idle dump @ 0xFEC10000).
	 * We write everything that's writable. */

	/* CTRL0: Win=0x6F8C0E02. We can't write bits[15:8] (hw-managed).
	 * But we CAN try to write the whole value and see what sticks. */
	w32(0x00, 0x6F8C0E02);
	pr_info("clw: Wrote CTRL0=0x6F8C0E02 -> readback=0x%08X\n", r32(0x00));

	/* CTRL1: Win=0x020006B5. Partially writable (bits[15:8] change). */
	w32(0x0C, 0x020006B5);
	pr_info("clw: Wrote CTRL1=0x020006B5 -> readback=0x%08X\n", r32(0x0C));

	/* ENA: Win=0x11110713 (should already match) */
	w32(0x20, 0x11110713);

	/* HOST_PREF (0x2C): Win=0x8000D4C0 */
	w32(0x2C, 0x8000D4C0);

	/* SPI100_SPEED (0x22): Win=0x1111 (matches) */
	w16(0x22, 0x1111);

	/* SPEED_CFG (0x44): Win=0x0200 — THIS IS DIFFERENT from Linux! */
	w16(0x44, 0x0200);
	pr_info("clw: Wrote 0x44=0x0200 -> readback=0x%04X\n", r16(0x44));

	/* OPCODE (0x45): Win=0x02 */
	w8(0x45, 0x02);

	/* TX_COUNT (0x48): Win=0x0E (14 — vendor init payload size) */
	w8(0x48, 0x0E);

	/* RX_COUNT (0x4B): Win=0x00 */
	w8(0x4B, 0x00);

	pr_info("clw: Cloned state:\n");
	dump_mmio("AFTER CLONE");

	/* ===== VENDOR INIT: try BOTH formats ===== */
	pr_info("clw: [1a] Vendor init V2 (opcode in 0x45, TX=13)...\n");
	{
		u8 vi[] = {0x00,0x00,0x04, 0x82,0x00,0x00,0x04, 0x00,0x01,0x02, 0xA1,0x01,0x00};
		writeN(0x02, vi, sizeof(vi));
	}
	msleep(50);
	{
		u8 rd[32]; memset(rd,0xCC,32);
		readN(rd,32); pr_info("clw:   post-V2 rd=[%*ph]\n", 32, rd);
		find_hid(rd,32,"post_vi_V2");
	}

	pr_info("clw: [1b] Vendor init V1 (opcode in FIFO[0], TX=14)...\n");
	{
		u8 vi[] = {0x02, 0x00,0x00,0x04, 0x82,0x00,0x00,0x04, 0x00,0x01,0x02, 0xA1,0x01,0x00};
		writeN(0x02, vi, sizeof(vi));
	}
	msleep(50);
	{
		u8 rd[32]; memset(rd,0xCC,32);
		readN(rd,32); pr_info("clw:   post-V1 rd=[%*ph]\n", 32, rd);
		find_hid(rd,32,"post_vi_V1");
	}

	pr_info("clw: [1c] Vendor init from CSV + LARGE READ (256B)...\n");
	{
		u8 vi[] = {0x02, 0x00,0x00,0x04, 0x82,0x00,0x00,0x04, 0x00,0x01,0x01, 0x0C,0xEE,0x5B};
		writeN(0x02, vi, sizeof(vi));
	}
	/* Wait and read 256 bytes to capture DATA report */
	msleep(100);
	{
		u8 big[256];
		int j;
		memset(big,0xCC,256);
		readN(big, 256);
		pr_info("clw:   post-CSV 256B rd[0..63]=[%*ph]\n", 64, big);
		pr_info("clw:   post-CSV 256B rd[64..127]=[%*ph]\n", 64, big+64);
		pr_info("clw:   post-CSV 256B rd[128..191]=[%*ph]\n", 64, big+128);
		for(j=3;j<256;j++) {
			if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
				int t=(big[j-3]>>4)&0xF; u16 bl=(((big[j-2]>>4)&0xF)|(big[j-1]<<4))*4;
				pr_info("clw: *** HID post_vi_CSV_256: type=%d(%s) blen=%u hdr=[%02x %02x %02x %02x]\n",
					t, t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?",
					bl, big[j-3],big[j-2],big[j-1],big[j]);
				/* If type=1 DATA, dump the first 64 bytes of content */
				if(t==1 && bl>0) {
					int start = j+1; /* body starts after header (4 bytes) */
					pr_info("clw:   DATA content[0..63]=[%*ph]\n", 64, big+start);
				}
			}
		}
	}

	/* ===== DESCREQ ===== */
	pr_info("clw: [3] Sending DESCREQ @0x000001...\n");
	{
		u8 dr[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		writeN(0x02, dr, sizeof(dr));
	}

	/* Read after DESCREQ */
	pr_info("clw: [4] Reading after DESCREQ...\n");
	{
		u8 rd[64];
		for (i=0;i<8;i++) {
			msleep(30);
			memset(rd,0xCC,64);
			if(readN(rd,32)<0) continue;
			pr_info("clw:   rd=[%*ph]\n", 32, rd);
			if(find_hid(rd,32,"post_DESCREQ")>0) break;
		}
	}

	dump_mmio("FINAL");
	w8(0x45,0x0B);
	iounmap(base);
	pr_info("clw: DONE\n");
	return -ENODEV;
}
module_init(clw_init);
MODULE_LICENSE("GPL");
