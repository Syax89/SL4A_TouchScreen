/* IDEA #1: Byte-by-byte SPI with configurable inter-byte delay.
 * Simulates bit-bang by sending each byte as separate TX_COUNT=1 write,
 * keeping CS asserted between bytes.
 *
 * insmod bytebang.ko && dmesg | grep "BB:"
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

static void cs_on(void){ w8(0x1D,(r8(0x1D)&0xFC)|0x01); }
static void cs_off(void){ w8(0x1D,(r8(0x1D)&0xFC)|0x00); }

/* Send ONE byte via SPI, returns 0=ok, -1=timeout */
static int send_byte(u8 b, int delay_us)
{
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x02); w32(0x00,r32(0x00)|0x60040000);
	w8(0x48,1); w8(0x80,b); w8(0x4B,0);
	w32(0x00,r32(0x00)|0x60040000); w8(0x45,0x02);
	w8(0x47,0x80);
	if(wbusy()) return -1;
	if(delay_us) udelay(delay_us);
	return 0;
}

static int read9(u8 *rx)
{
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x0B); w32(0x00,r32(0x00)|0x60040000);
	w8(0x48,3); w8(0x80,0);w8(0x81,0);w8(0x82,0); w8(0x4B,9);
	w32(0x00,r32(0x00)|0x60040000);
	w8(0x47,0x80); if(wbusy()) return -1;
	{int i;for(i=0;i<9;i++) rx[i]=r8(0x80+3+i);}
	w8(0x45,0x0B); return 0;
}

static void dump_fifo(const char *tag)
{
	u8 f[70]; int i;
	for(i=0;i<70;i++) f[i]=r8(0x80+i);
	pr_info("BB: %s [0..69]=[%*ph]\n", tag, 70, f);
	/* Search for 0x5A */
	for(i=3;i<70;i++) {
		if(f[i]==0x5A && (f[i-3]&0x0F)==2 && (f[i-2]&0x0F)==0) {
			int t=(f[i-3]>>4)&0xF;
			pr_info("BB:   HDR@%d: type=%d(%s)\n", i-3, t,
				t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":t==1?"DATA":"?");
		}
	}
}

static int __init bb_init(void)
{
	u8 dr_full[] = {0x02,0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00};
	u8 rx[70];
	int i, delays[] = {0, 1, 5, 10, 50, 100, 500, 1000}; /* microseconds */
	int d;

	base = ioremap(FCH_BASE, 0x100);
	if(!base) return -ENOMEM;

	pr_info("BB: ============================================\n");
	pr_info("BB: BYTE-BY-BYTE DESCREQ with variable inter-byte delays\n");
	pr_info("BB: ============================================\n");

	for(d=0; d<ARRAY_SIZE(delays); d++) {
		int delay = delays[d];
		pr_info("BB:\nBB: --- DELAY=%d us ---\n", delay);

		/* Drain */
		cs_on();
		read9(rx);
		pr_info("BB:   drain rd=[%*ph]\n", 9, rx);
		dump_fifo("BEFORE");

		/* Send DESCREQ byte by byte, CS held */
		pr_info("BB:   sending 10 bytes...\n");
		for(i=0; i<sizeof(dr_full); i++) {
			if(send_byte(dr_full[i], delay)) {
				pr_err("BB:   byte %d FAILED\n", i);
				goto next_delay;
			}
		}
		pr_info("BB:   10 bytes sent.\n");

		dump_fifo("AFTER-WRITE");

		/* Read response */
		udelay(100);
		read9(rx);
		pr_info("BB:   post-write rd=[%*ph]\n", 9, rx);
		dump_fifo("AFTER-READ");

		cs_off();

next_delay:
		continue;
	}

	w8(0x45,0x0B);
	cs_off();
	iounmap(base);
	pr_info("BB: DONE — check for DEVICE_DESC (type=7)\n");
	return -ENODEV;
}
module_init(bb_init);
MODULE_LICENSE("GPL");
