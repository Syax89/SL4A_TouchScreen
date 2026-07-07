/* 3 RADICAL IDEAS — one module:
 * A: CS held low between read(0x0B) and write(0x02)
 * B: Opcode 0x02 in FIFO (not 0x45), TX_COUNT=10
 * C: DESCREQ via opcode 0x0B (read opcode) with payload
 *
 * insmod radical3.ko && dmesg | grep "RD3:"
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

static int wbusy(void){u32 v;return readl_poll_timeout(base,v,!(v&BIT(31)),20,2000000);}
static void cs(int on){ w8(0x1D,(r8(0x1D)&0xFC)|(on?0x01:0x00)); }
static void clr(void){ u32 c=r32(0x00);w32(0x00,c&~BIT(20));w32(0x00,c|BIT(20)); }
static void sec(void){ w32(0x00,r32(0x00)|0x60040000); }

static void ftx8(int o,const u8*d,int n){int i;for(i=0;i<n;i++)w8(o+i,d[i]);}
static void frx8(int o,u8*d,int n){int i;for(i=0;i<n;i++)d[i]=r8(o+i);}

static void dump(const char *tag)
{
	u8 f[70]; frx8(0x80,f,70);
	pr_info("RD3: %s [0..69]=[%*ph]\n", tag, 70, f);
	int i; for(i=3;i<70;i++) {
		if(f[i]==0x5A && (f[i-3]&0x0F)==2 && (f[i-2]&0x0F)==0) {
			int t=(f[i-3]>>4)&0xF;
			pr_info("RD3:   HDR@%d type=%d(%s)\n", i-3, t,
				t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":t==1?"DATA":"?");
		}
	}
}

static int __init rd3_init(void)
{
	u8 dr_fifo[] = {0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00}; /* 9 bytes - for 0x45 */
	u8 dr_full[] = {0x02,0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00}; /* 10 bytes - opcode in FIFO */
	int i;

	base = ioremap(FCH_BASE,0x100);
	if(!base) return -ENOMEM;

	pr_info("RD3: ================================================\n");
	pr_info("RD3: 3 RADICAL IDEAS\n");
	pr_info("RD3: ================================================\n");

	/* ===== A: CS held low between READ and WRITE ===== */
	pr_info("RD3:\nRD3: --- A: CS held low read→write ---\n");
	cs(1); /* assert CS */
	dump("A-INIT");

	/* Read RESET_RSP (CS stays low) */
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); wbusy();
	dump("A-AFTER-READ");
	w8(0x45,0x0B);

	/* IMMEDIATELY send DESCREQ without toggling CS */
	pr_info("RD3: A: sending DESCREQ (CS still low)...\n");
	clr(); w8(0x45,0x02); sec();
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,sizeof(dr_fifo)); ftx8(0x80,dr_fifo,sizeof(dr_fifo)); w8(0x4B,0);
	sec(); w8(0x45,0x02);
	w8(0x47,0x80); wbusy();
	dump("A-AFTER-WRITE");
	w8(0x45,0x0B);

	/* Read response */
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); wbusy();
	dump("A-AFTER-READ2");
	cs(0); /* deselect */
	w8(0x45,0x0B);

	msleep(100);

	/* ===== B: Opcode in FIFO (TX_COUNT=10) ===== */
	pr_info("RD3:\nRD3: --- B: Opcode 0x02 in FIFO, TX_COUNT=10 ---\n");
	cs(1);
	/* Drain first */
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); wbusy();
	dump("B-AFTER-DRAIN");
	w8(0x45,0x0B);

	/* Send DESCREQ with opcode in FIFO[0] */
	pr_info("RD3: B: sending DESCREQ (opcode in FIFO)...\n");
	clr();
	/* Put opcode 0x02 in CTRL0[7:0] AND 0x45 AND FIFO[0] — triple guarantee */
	w8(0x45,0x02);
	sec();
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,sizeof(dr_full)); ftx8(0x80,dr_full,sizeof(dr_full)); w8(0x4B,0);
	sec(); w8(0x45,0x02);

	/* Also set CTRL0[7:0] = 0x02 (V1 opcode field) */
	{ u32 c0=r32(0x00); c0=(c0&~0xFF)|0x02; w32(0x00,c0); sec(); }

	pr_info("RD3: B: PRE-TRIG c0=0x%08x c1=0x%08x op45=0x%02x tx48=%u\n",
		r32(0x00), r32(0x0C), r8(0x45), r8(0x48));
	w8(0x47,0x80); wbusy();
	dump("B-AFTER-WRITE");
	w8(0x45,0x0B);

	/* Read */
	msleep(5);
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); wbusy();
	dump("B-AFTER-READ");
	cs(0);
	w8(0x45,0x0B);

	msleep(100);

	/* ===== C: DESCREQ via opcode 0x0B ===== */
	pr_info("RD3:\nRD3: --- C: DESCREQ via opcode 0x0B (read opcode) ---\n");
	cs(1);
	/* Drain */
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); wbusy();
	w8(0x45,0x0B);

	/* Send DESCREQ payload but with opcode 0x0B instead of 0x02 */
	pr_info("RD3: C: sending payload via opcode 0x0B...\n");
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,sizeof(dr_full)); ftx8(0x80,dr_full,sizeof(dr_full)); w8(0x4B,0);
	sec();
	pr_info("RD3: C: PRE-TRIG c0=0x%08x c1=0x%08x op45=0x%02x tx48=%u\n",
		r32(0x00), r32(0x0C), r8(0x45), r8(0x48));
	w8(0x47,0x80); wbusy();
	dump("C-AFTER-WRITE");
	w8(0x45,0x0B);

	/* Read */
	msleep(5);
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); wbusy();
	dump("C-AFTER-READ");
	cs(0);
	w8(0x45,0x0B);

	iounmap(base);
	pr_info("RD3: DONE\n");
	return -ENODEV;
}
module_init(rd3_init);
MODULE_LICENSE("GPL");
