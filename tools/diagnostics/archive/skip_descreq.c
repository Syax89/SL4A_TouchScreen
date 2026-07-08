/* Skip DESCREQ entirely. Just read the device repeatedly and look for ANY data.
 * Hardcoded: we know the descriptors from Windows dump.
 * insmod skip_descreq.ko && dmesg | grep "SKP:"
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

static int read_big(u32 addr, int n, u8 *rx)
{
	int i;
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x0B); w32(0x00,r32(0x00)|0x60040000);
	w8(0x48,3);
	w8(0x80,(addr>>16)&0xFF); w8(0x81,(addr>>8)&0xFF); w8(0x82,addr&0xFF);
	w8(0x4B,n);
	w32(0x00,r32(0x00)|0x60040000);
	w8(0x47,0x80);
	if(wbusy()) return -1;
	for(i=0;i<n;i++) rx[i]=r8(0x80+3+i);
	w8(0x45,0x0B);
	return 0;
}

static int write02(const u8 *pl, int txlen)
{
	w32(0x00,r32(0x00)&~BIT(20)); w32(0x00,r32(0x00)|BIT(20));
	w8(0x45,0x02); w32(0x00,r32(0x00)|0x60040000);
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,txlen); {int k;for(k=0;k<txlen;k++)w8(0x80+k,pl[k]);}
	w8(0x4B,0);
	w32(0x00,r32(0x00)|0x60040000); w8(0x45,0x02);
	w8(0x47,0x80); return wbusy();
}

static int __init skp_init(void)
{
	u8 big[512], dr[]={0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00};
	int i,j;

	base = ioremap(FCH_BASE, 0x100);
	if(!base) return -ENOMEM;

	/* ===== CS0 TEST ===== */
	pr_info("SKP: === CS0 ===\n");
	w8(0x1D,(r8(0x1D)&0xFC)|0x00);
	pr_info("SKP: CS0: draining RESET_RSP...\n");
	read_big(0x000000, 256, big);
	for(j=3;j<256;j++) {
		if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
			int t=(big[j-3]>>4)&0xF;
			pr_info("SKP: CS0 DRAIN: HID type=%d(%s)\n", t, t==3?"RESET_RSP":t==7?"DEV_DESC":"?");
		}
	}
	pr_info("SKP: CS0: sending DESCREQ @0x000001...\n");
	write02(dr, sizeof(dr));
	pr_info("SKP: CS0: polling for response...\n");
	for(i=0;i<5;i++) {
		msleep(30);
		memset(big,0xCC,256);
		read_big(0x000000, 64, big);
		for(j=3;j<64;j++) {
			if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
				int t=(big[j-3]>>4)&0xF;
				pr_info("SKP: CS0 POLL#%d: HID type=%d(%s) hdr=[%02x %02x %02x %02x]\n",
					i, t, t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":"?",
					big[j-3],big[j-2],big[j-1],big[j]);
				goto cs0_done;
			}
		}
	}
	pr_info("SKP: CS0: no response\n");
cs0_done:

	/* ===== CS1 TEST ===== */
	pr_info("SKP:\nSKP: === CS1 ===\n");
	w8(0x1D,(r8(0x1D)&0xFC)|0x01);
	pr_info("SKP: CS1: draining RESET_RSP...\n");
	read_big(0x000000, 256, big);
	for(j=3;j<256;j++) {
		if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
			int t=(big[j-3]>>4)&0xF;
			pr_info("SKP: CS1 DRAIN: HID type=%d(%s)\n", t, t==3?"RESET_RSP":"?");
		}
	}
	pr_info("SKP: CS1: sending DESCREQ @0x000001...\n");
	write02(dr, sizeof(dr));
	for(i=0;i<5;i++) {
		msleep(30);
		memset(big,0xCC,256);
		read_big(0x000000, 64, big);
		for(j=3;j<64;j++) {
			if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
				int t=(big[j-3]>>4)&0xF;
				pr_info("SKP: CS1 POLL#%d: HID type=%d(%s) hdr=[%02x %02x %02x %02x]\n",
					i, t, t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":"?",
					big[j-3],big[j-2],big[j-1],big[j]);
				goto cs1_done;
			}
		}
	}
	pr_info("SKP: CS1: no response\n");
cs1_done:

	pr_info("SKP: ==================================================\n");
	pr_info("SKP: SKIP DESCREQ — Poll for DATA reports directly\n");
	pr_info("SKP: ==================================================\n");

	/* Phase 1: Drain any pending RESET_RSP */
	pr_info("SKP: [1] Draining with 256B read...\n");
	read_big(0x000000, 256, big);
	for(j=3;j<256;j++) {
		if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
			int t=(big[j-3]>>4)&0xF; u16 bl=(((big[j-2]>>4)&0xF)|(big[j-1]<<4))*4;
			pr_info("SKP: DRAIN: HID type=%d(%s) blen=%u\n", t,
				t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?", bl);
		}
	}

	/* Phase 2: Poll for DATA reports over ~5 seconds */
	pr_info("SKP: [2] Polling for DATA reports (5s, 256B reads, 100ms intervals)...\n");
	for(i = 0; i < 50; i++) {
		msleep(100);
		memset(big, 0xCC, sizeof(big));
		if (read_big(0x000000, 256, big) < 0) continue;

		for(j=3;j<256;j++) {
			if(big[j]==0x5A && (big[j-3]&0x0F)==2 && (big[j-2]&0x0F)==0) {
				int t=(big[j-3]>>4)&0xF; u16 bl=(((big[j-2]>>4)&0xF)|(big[j-1]<<4))*4;
				pr_info("SKP: POLL#%d: HID type=%d(%s) blen=%u hdr=[%02x %02x %02x %02x]\n",
					i, t, t==1?"DATA":t==3?"RESET_RSP":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?",
					bl, big[j-3],big[j-2],big[j-1],big[j]);

				if(t==1) {
					/* DATA report! Show first bytes */
					int start = j+1;
					pr_info("SKP: *** DATA CONTENT[0..63]=[%*ph]\n", 64, big+start);
					pr_info("SKP: *** DATA CONTENT[64..127]=[%*ph]\n", 64, big+start+64);
				}

				/* Found one report, keep polling for more */
				break;
			}
		}
		if(i==0 || i==10 || i==20 || i==30 || i==40)
			pr_info("SKP:   poll#%d raw[0..32]=[%*ph]\n", i, 32, big);
	}

	w8(0x45,0x0B);
	iounmap(base);
	pr_info("SKP: DONE\n");
	return -ENODEV;
}
module_init(skp_init);
MODULE_LICENSE("GPL");
