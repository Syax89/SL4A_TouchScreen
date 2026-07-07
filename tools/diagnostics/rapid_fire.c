/* IDEA #3: Rapid-fire DESCREQ — send 100 writes in 1 second.
 * Maybe the device needs a burst or specific timing.
 *
 * insmod rapid_fire.ko && dmesg | grep "RPF:"
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

static int send_write(u8 opcode, const u8 *pl, int txlen)
{
	int k;
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,opcode); w32(0x00,r32(0x00)|0x60040000);
	if(opcode!=0x0B) {
		u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
		r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
		w16(0x44,r44); w8(0x45,opcode);
	}
	w8(0x48,txlen);
	for(k=0;k<txlen;k++) w8(0x80+k,pl[k]);
	w8(0x4B,0);
	w32(0x00,r32(0x00)|0x60040000); w8(0x45,opcode);
	w8(0x47,0x80);
	return wbusy();
}

static int read70(u8 *rx)
{
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x0B); w32(0x00,r32(0x00)|0x60040000);
	w8(0x48,3); w8(0x80,0);w8(0x81,0);w8(0x82,0); w8(0x4B,70);
	w32(0x00,r32(0x00)|0x60040000);
	w8(0x47,0x80);
	if(wbusy()) return -1;
	{int i;for(i=0;i<70;i++) rx[i]=r8(0x80+i);}
	w8(0x45,0x0B); return 0;
}

static int __init rpf_init(void)
{
	u8 rx[70], dr[]={0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00};
	int i,j, found=0;

	base = ioremap(FCH_BASE, 0x100);
	if(!base) return -ENOMEM;
	w8(0x1D,(r8(0x1D)&0xFC)|0x01);

	pr_info("RPF: ============================================\n");
	pr_info("RPF: RAPID-FIRE DESCREQ — 100 writes\n");
	pr_info("RPF: ============================================\n");

	/* Drain */
	read70(rx);

	/* Send DESCREQ 100 times as fast as possible */
	pr_info("RPF: Sending 100 DESCREQ...\n");
	for(i=0;i<100;i++) {
		if(send_write(0x02, dr, sizeof(dr)) < 0) {
			pr_err("RPF: write %d BUSY timeout\n", i);
			break;
		}
		if(i==0||i==50||i==99)
			pr_info("RPF:   write #%d done\n", i);
	}
	pr_info("RPF: 100 writes done. Reading response...\n");

	/* Read 70 bytes and search for ANY non-RESET_RSP response */
	if(read70(rx) < 0) { pr_err("RPF: read failed\n"); goto out; }

	for(j=3;j<70;j++) {
		if(rx[j]==0x5A && (rx[j-3]&0x0F)==2 && (rx[j-2]&0x0F)==0) {
			int t=(rx[j-3]>>4)&0xF; u16 bl=(((rx[j-2]>>4)&0xF)|(rx[j-1]<<4))*4;
			pr_info("RPF: *** HID hdr@%d: type=%d(%s) blen=%u [%02x %02x %02x %02x]\n",
				j-3, t, t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC!!!":"?",
				bl, rx[j-3],rx[j-2],rx[j-1],rx[j]);
			if(t!=3) found=1;
		}
	}

	if(!found) {
		pr_info("RPF: Only RESET_RSP or no response. Trying one more read...\n");
		msleep(100);
		read70(rx);
		for(j=3;j<70;j++) {
			if(rx[j]==0x5A && (rx[j-3]&0x0F)==2 && (rx[j-2]&0x0F)==0) {
				int t=(rx[j-3]>>4)&0xF;
				pr_info("RPF: *** HDR@%d: type=%d(%s)\n", j-3, t,
					t==7?"DEVICE_DESC!!!":t==1?"DATA":t==3?"RESET_RSP":"?");
			}
		}
	}

out:
	w8(0x45,0x0B);
	iounmap(base);
	pr_info("RPF: DONE\n");
	return -ENODEV;
}
module_init(rpf_init);
MODULE_LICENSE("GPL");
