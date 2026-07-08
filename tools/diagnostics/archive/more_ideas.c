/* IDEAS #9, #4, #8: SET_POWER, GPIO reset, register sweep.
 *
 * insmod more_ideas.ko && dmesg | grep "MOI:"
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
	u8 f[70]; frx8(0x80,f,70);
	int i; for(i=3;i<70;i++) {
		if(f[i]==0x5A && (f[i-3]&0x0F)==2 && (f[i-2]&0x0F)==0) {
			int t=(f[i-3]>>4)&0xF;
			pr_info("MOI: %s HDR@%d type=%d(%s)\n", tag, i-3, t,
				t==7?"DEVICE_DESC!!!":t==3?"RESET_RSP":t==1?"DATA":t==4?"CMD_RESP":"?");
		}
	}
}

static int send_write(u8 opcode, const u8 *pl, int txlen)
{
	clr(); w8(0x45,opcode); sec();
	if(opcode!=0x0B) {
		u16 r44=r16(0x44); u8 nib=(r32(0x20)>>20)&0xF;
		r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
		w16(0x44,r44); w8(0x45,opcode);
	}
	w8(0x48,txlen); ftx8(0x80,pl,txlen); w8(0x4B,0);
	sec(); w8(0x45,opcode);
	w8(0x47,0x80); return wbusy();
}

static int read70(void)
{
	clr(); w8(0x45,0x0B); sec();
	w8(0x48,3); ftx8(0x80,(u8[]){0,0,0},3); w8(0x4B,70);
	sec(); w8(0x47,0x80);
	if(wbusy()) return -1;
	w8(0x45,0x0B); return 0;
}

static int __init moi_init(void)
{
	u8 dr[]={0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00}; /* DESCREQ @0x01 */
	acpi_handle hspi;
	int i;

	base = ioremap(FCH_BASE,0x100);
	if(!base) return -ENOMEM;
	cs1();

	pr_info("MOI: ================================================\n");
	pr_info("MOI: MORE IDEAS: SET_POWER, GPIO reset, register sweep\n");
	pr_info("MOI: ================================================\n");

	/* ===== #9: SET_POWER ON via command register 0x0004 ===== */
	pr_info("MOI:\nMOI: --- #9: SET_POWER ON @0x0004 then DESCREQ ---\n");
	read70(); /* drain */
	dump("9-DRAIN");

	/* SET_POWER: content_type=0x07 (SPI_HID_CONTENT_TYPE_SET_POWER?),
	 * register 0x0004 (command register), no payload */
	pr_info("MOI: #9: Sending SET_POWER to reg 0x0004...\n");
	{
		u8 sp[] = {0x00,0x00,0x04, 0x42,0x00, 0x07,0x03,0x01,0x00};
		send_write(0x02, sp, sizeof(sp));
	}
	msleep(10);
	dump("9-AFTER-SET_POWER");

	/* Now DESCREQ */
	pr_info("MOI: #9: Sending DESCREQ @0x000001...\n");
	send_write(0x02, dr, sizeof(dr));
	msleep(10);
	read70();
	dump("9-AFTER-DESCREQ");

	/* ===== #4: GPIO reset via ACPI _RST then immediate DESCREQ ===== */
	pr_info("MOI:\nMOI: --- #4: ACPI _RST + immediate DESCREQ ---\n");
	if (!ACPI_FAILURE(acpi_get_handle(NULL, "\\_SB.SPI1.HSPI", &hspi))) {
		pr_info("MOI: #4: Calling _RST...\n");
		acpi_evaluate_object(hspi, "_RST", NULL, NULL);
		msleep(350); /* _RST takes ~300ms */
		pr_info("MOI: #4: _RST done, draining...\n");
		read70();
		dump("4-DRAIN");
		pr_info("MOI: #4: Sending DESCREQ immediately...\n");
		send_write(0x02, dr, sizeof(dr));
		udelay(200); /* Windows ~58µs */
		read70();
		dump("4-AFTER-DESCREQ");
	} else {
		pr_info("MOI: #4: Cannot find HSPI ACPI handle\n");
	}

	/* ===== #8: DESCREQ to different registers ===== */
	pr_info("MOI:\nMOI: --- #8: DESCREQ to registers 0x00,0x03,0x04,0xFF,0x000100 ---\n");
	{
		u32 regs[] = {0x000000, 0x000003, 0x000004, 0x0000FF, 0x000100};
		for(i=0; i<ARRAY_SIZE(regs); i++) {
			u8 w[] = {(regs[i]>>16)&0xFF, (regs[i]>>8)&0xFF, regs[i]&0xFF,
				  0x42,0x00, 0x01,0x03,0x00,0x00};
			pr_info("MOI: #8: DESCREQ to 0x%06X...\n", regs[i]);
			read70();
			send_write(0x02, w, sizeof(w));
			udelay(200);
			read70();
			dump("8-AFTER");
		}
	}

	w8(0x45,0x0B);
	iounmap(base);
	pr_info("MOI: DONE\n");
	return -ENODEV;
}
module_init(moi_init);
MODULE_LICENSE("GPL");
