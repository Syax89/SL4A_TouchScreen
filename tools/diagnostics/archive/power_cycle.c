/* LAST CARD: _PS3 → _PS0 full power cycle.
 * If the device becomes unresponsive, reboot required.
 *
 * insmod power_cycle.ko && dmesg | grep "PWC:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/acpi.h>

#define FCH_BASE 0xFEC10000
static void __iomem *base;

#define w32(o,v) iowrite32(v,base+(o))
#define r32(o)   ioread32(base+(o))
#define w16(o,v) iowrite16(v,base+(o))
#define r16(o)   ioread16(base+(o))
#define w8(o,v)  iowrite8(v,base+(o))
#define r8(o)    ioread8(base+(o))

static int wbusy(void){u32 v;return readl_poll_timeout(base,v,!(v&BIT(31)),20,2000000);}
static void cs1(void){ w8(0x1D,(r8(0x1D)&0xFC)|0x01); }
static void clr(void){ u32 c=r32(0x00);w32(0x00,c&~BIT(20));w32(0x00,c|BIT(20)); }
static void sec(void){ w32(0x00,r32(0x00)|0x60040000); }
static void ftx8(int o,const u8*d,int n){int i;for(i=0;i<n;i++)w8(o+i,d[i]);}
static void frx8(int o,u8*d,int n){int i;for(i=0;i<n;i++)d[i]=r8(o+i);}

static void dump(const char *tag)
{
	u8 f[70]; frx8(0x80,f,70); int i;
	pr_info("PWC: %s FIFO[0..31]=[%*ph]\n", tag, 32, f);
	for(i=3;i<70;i++) {
		if(f[i]==0x5A && (f[i-3]&0x0F)==2 && (f[i-2]&0x0F)==0) {
			int t=(f[i-3]>>4)&0xF; u16 bl=(((f[i-2]>>4)&0xF)|(f[i-1]<<4))*4;
			pr_info("PWC: *** HDR@%d type=%d(%s) blen=%u [%02x%02x%02x%02x]\n", i-3, t,
				t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":t==1?"DATA":t==8?"RPT_DESC":"?",
				bl, f[i-3],f[i-2],f[i-1],f[i]);
		}
	}
}

static int read70(void)
{
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80); if(wbusy()) return -1;
	w8(0x45,0x0B); return 0;
}

static int send_wr(const u8 *pl, int n)
{
	clr(); w8(0x45,0x02); sec();
	{ u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(0x44,r44); w8(0x45,0x02); }
	w8(0x48,n); ftx8(0x80,pl,n); w8(0x4B,0);
	sec(); w8(0x45,0x02);
	w8(0x47,0x80); return wbusy();
}

static int __init pwc_init(void)
{
	acpi_handle hspi;
	u8 dr[]={0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00};
	int i;

	base = ioremap(FCH_BASE,0x100);
	if(!base) return -ENOMEM;

	if (ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &hspi))) {
		pr_err("PWC: Cannot find HSPI handle\n");
		iounmap(base); return -ENODEV;
	}

	cs1();
	pr_info("PWC: ============================================\n");
	pr_info("PWC: FULL POWER CYCLE: _PS3 → _PS0 → DESCREQ\n");
	pr_info("PWC: ============================================\n");

	/* Save current state */
	dump("INITIAL");

	/* _PS3: power OFF */
	pr_info("PWC: [1] Calling _PS3 (power OFF)...\n");
	acpi_evaluate_object(hspi, "_PS3", NULL, NULL);
	msleep(500);
	dump("AFTER-_PS3");
	pr_info("PWC: [1] _PS3 complete\n");

	/* _PS0: power ON (this does M010 sequence) */
	pr_info("PWC: [2] Calling _PS0 (power ON + 300ms reset)...\n");
	acpi_evaluate_object(hspi, "_PS0", NULL, NULL);
	msleep(500); /* Extra wait for device firmware to boot */
	dump("AFTER-_PS0");
	pr_info("PWC: [2] _PS0 complete, device should be freshly booted\n");

	/* Drain RESET_RSP — poll up to 5 seconds */
	pr_info("PWC: [3] Draining RESET_RSP (polling 5s)...\n");
	for(i=0;i<25;i++) {
		msleep(200);
		if(read70()<0) continue;
		dump("DRAIN-POLL");
		/* Check if we got a HID header */
		{
			u8 f[70]; int j; frx8(0x80,f,70);
			for(j=3;j<70;j++) {
				if(f[j]==0x5A && (f[j-3]&0x0F)==2 && (f[j-2]&0x0F)==0) {
					int t=(f[j-3]>>4)&0xF;
					pr_info("PWC: *** POLL %d: HDR type=%d(%s)\n", i, t,
						t==3?"RESET_RSP":t==7?"DEVICE_DESC!!!":"?");
					if(t==3) goto got_reset;
				}
			}
		}
	}
	pr_info("PWC: [3] No RESET_RSP found after 5s — device dead until reboot?\n");
	goto out;

got_reset:
	pr_info("PWC: [3] RESET_RSP found! Sending DESCREQ...\n");

	/* Send DESCREQ */
	pr_info("PWC: [4] Sending DESCREQ @0x000001...\n");
	send_wr(dr, sizeof(dr));
	pr_info("PWC: [4] DESCREQ sent\n");

	/* Poll for DEVICE_DESC */
	pr_info("PWC: [5] Polling for DEVICE_DESC (5s)...\n");
	for(i=0;i<25;i++) {
		msleep(200);
		if(read70()<0) continue;
		{
			u8 f[70]; int j; frx8(0x80,f,70);
			for(j=3;j<70;j++) {
				if(f[j]==0x5A && (f[j-3]&0x0F)==2 && (f[j-2]&0x0F)==0) {
					int t=(f[j-3]>>4)&0xF;
					pr_info("PWC: *** POST-DESCREQ POLL %d: type=%d(%s)\n", i, t,
						t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":"?");
				}
			}
		}
	}

out:
	w8(0x45,0x0B);
	iounmap(base);
	pr_info("PWC: DONE — if device is dead, reboot\n");
	return -ENODEV;
}
module_init(pwc_init);
MODULE_LICENSE("GPL");
