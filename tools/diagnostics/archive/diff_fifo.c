/* IDEA #2: Differential FIFO analysis.
 * Read FIFO before and after DESCREQ write. Show which bytes changed.
 * Changed bytes = device MISO response (not residual data).
 *
 * insmod diff_fifo.ko && dmesg | grep "DIF:"
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

static void snap_fifo(u8 *out, const char *tag)
{
	int i;
	for(i=0;i<70;i++) out[i]=r8(0x80+i);
	pr_info("DIF: %s FIFO[0..69]=[%*ph]\n", tag, 70, out);
}

static int __init dif_init(void)
{
	u8 before[70], after[70], dr[]={0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00};
	int i, changes=0;

	base = ioremap(FCH_BASE, 0x100);
	if(!base) return -ENOMEM;
	w8(0x1D,(r8(0x1D)&0xFC)|0x01); /* CS1 */

	pr_info("DIF: ============================================\n");
	pr_info("DIF: DIFFERENTIAL FIFO — before vs after DESCREQ\n");
	pr_info("DIF: ============================================\n");

	/* Drain pending RESET_RSP with a read first */
	pr_info("DIF: [0] Draining with 70B read...\n");
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x0B); w32(0x00,r32(0x00)|0x60040000);
	w8(0x48,3); w8(0x80,0);w8(0x81,0);w8(0x82,0); w8(0x4B,70);
	w32(0x00,r32(0x00)|0x60040000);
	w8(0x47,0x80); wbusy();
	w8(0x45,0x0B);

	/* Snapshot 1: BEFORE */
	snap_fifo(before, "BEFORE");

	/* Send DESCREQ */
	pr_info("DIF: [1] Sending DESCREQ @0x000001...\n");
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x02); w32(0x00,r32(0x00)|0x60040000);
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,sizeof(dr));
	for(i=0;i<sizeof(dr);i++) w8(0x80+i,dr[i]);
	w8(0x4B,0); /* TX-only */
	w32(0x00,r32(0x00)|0x60040000); w8(0x45,0x02);

	{
		u32 c0=r32(0x00);
		pr_info("DIF:   PRE-TRIG c0=0x%08x c1=0x%08x op45=0x%02x tx48=%u\n",
			c0, r32(0x0C), r8(0x45), r8(0x48));
	}

	w8(0x47,0x80); wbusy();
	w8(0x45,0x0B);

	/* Snapshot 2: AFTER */
	snap_fifo(after, "AFTER");

	/* DIFF */
	pr_info("DIF: ============================================\n");
	pr_info("DIF: BYTES CHANGED (MISO response from device):\n");
	for(i=0;i<70;i++) {
		if(before[i] != after[i]) {
			pr_info("DIF:   FIFO[%02d]  BEFORE=0x%02X  AFTER=0x%02X  (%s)\n",
				i, before[i], after[i],
				i < sizeof(dr) ? "TX data" : "MISO!");
			changes++;
		}
	}
	if(changes==0) pr_info("DIF:   NO BYTES CHANGED! Device did not respond.\n");
	else pr_info("DIF:   Total %d bytes changed\n", changes);

	/* Also show MISO-only bytes (after TX data region) */
	pr_info("DIF: MISO region (FIFO[%zu..69]):\n", sizeof(dr));
	for(i=sizeof(dr);i<70;i++) {
		if(before[i] != after[i])
			pr_info("DIF:   FIFO[%02d]=0x%02X (was 0x%02X)\n", i, after[i], before[i]);
	}

	/* Now do a READ and show what changed */
	pr_info("DIF:\nDIF: [2] 70B read from 0x000000 after DESCREQ...\n");
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x0B); w32(0x00,r32(0x00)|0x60040000);
	w8(0x48,3); w8(0x80,0);w8(0x81,0);w8(0x82,0); w8(0x4B,70);
	w32(0x00,r32(0x00)|0x60040000);
	w8(0x47,0x80); wbusy();
	snap_fifo(before, "POST-READ");
	w8(0x45,0x0B);

	/* Search for 0x5A headers in post-read FIFO */
	pr_info("DIF: Searching for HID headers in POST-READ FIFO...\n");
	for(i=3;i<70;i++) {
		if(before[i]==0x5A && (before[i-3]&0x0F)==2 && (before[i-2]&0x0F)==0) {
			int t=(before[i-3]>>4)&0xF; u16 bl=(((before[i-2]>>4)&0xF)|(before[i-1]<<4))*4;
			pr_info("DIF: *** HID hdr@%d: type=%d(%s) blen=%u [%02x %02x %02x %02x]\n",
				i-3, t, t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":"?",
				bl, before[i-3],before[i-2],before[i-1],before[i]);
		}
	}

	iounmap(base);
	pr_info("DIF: DONE\n");
	return -ENODEV;
}
module_init(dif_init);
MODULE_LICENSE("GPL");
