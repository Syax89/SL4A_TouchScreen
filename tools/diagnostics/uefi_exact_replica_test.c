/* uefi_exact_replica_test.c — test every remaining structural difference found in the
 * three-way Windows/UEFI/Linux comparison (GROUND_TRUTH.md 15.18-15.19), individually and
 * combined, using the correct edge-synced DESCREQ methodology (irq_oracle.c, proven since
 * 15.9). Nothing here is guessed -- every variant is exactly what was decompiled from
 * AmdSpiHcProtocolDxe.efi's real Transfer()/busy-wait functions.
 *
 * Variants:
 *   A: TRIGGER written as UEFI does it -- read-modify-write "|= 0x80" instead of our
 *      blind write8(TRIGGER, 0x80). Windows uses blind-write (matches us); UEFI uses RMW.
 *   B: busy-wait via STATUS(0x4C) read as a 32-bit dword, bit31, instead of CTRL0 bit31.
 *      (The PCI-LPC 0xB8 bit0 gate UEFI also checks is skipped here since it's already
 *      confirmed identical -- 0 -- on both Windows and Linux; adding it would be a no-op.)
 *   C: do NOT set the FAST_READ mode bits (CTRL0 bits 30/29/18, 0x60040000) for the
 *      DESCREQ write -- UEFI's Transfer() never sets them at all, in any function, for
 *      any transfer type. Windows and our own driver always set them.
 *   D: A+B+C combined -- the closest possible replica of UEFI's actual write path.
 *
 * Baseline (our current exact behavior) is run first for direct comparison in the same
 * boot/session.
 *
 * insmod uefi_exact_replica_test.ko && dmesg | grep "uert:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/ktime.h>

#define FCH_BASE 0xFEC10000
#define FCH_SIZE 0x100
#define GPIO_BASE 0xFED81500
#define GPIO_SIZE 0x300
#define IRQ_PIN   85
#define PIN_REG   (IRQ_PIN * 4)
#define PIN_STS   (1u << 16)

static void __iomem *base;
static void __iomem *gpio;

static u32 r32(int o){ return ioread32(base+o); }
static void w32(int o,u32 v){ iowrite32(v,base+o); }
static u8  r8(int o){ return ioread8(base+o); }
static void w8(int o,u8 v){ iowrite8(v,base+o); }

#define CTRL0 0x00
#define OPCODE 0x45
#define TRIGGER 0x47
#define TX_COUNT 0x48
#define RX_COUNT 0x4B
#define STATUS 0x4C
#define FIFO 0x80
#define ALT_CS 0x1D

static bool pin_asserted(void){ return !(ioread32(gpio + PIN_REG) & PIN_STS); }
static void select_cs1(void){ u8 v=r8(ALT_CS); w8(ALT_CS,(v&0xFC)|0x01); }

/* our normal busy-wait: poll CTRL0 bit31 */
static int wait_busy_ctrl0(void){ u32 v; return readl_poll_timeout(base+CTRL0,v,!(v&BIT(31)),20,2000000); }
/* UEFI-style busy-wait: poll STATUS(0x4C) read as a dword, bit31 */
static int wait_busy_status32(void){ u32 v; return readl_poll_timeout(base+STATUS,v,!(v&BIT(31)),20,2000000); }

static void fifo_write8(int off,const u8 *d,int n){ int i; for(i=0;i<n;i++) w8(off+i,d[i]); }
static void fifo_read16(int off,u8 *dst,int n){
	int i; for(i=0;i<n;i++){ u16 w=ioread16(base+off+(i/2)*2); dst[i]=(i&1)?(u8)(w>>8):(u8)(w&0xFF); }
}

static int hid_type(const u8 *d,int n){
	int i; for(i=0;i<n-3;i++) if(d[i+3]==0x5A && (d[i]&0x0F)==2 && (d[i+1]&0x0F)==0) return (d[i]>>4)&0xF;
	return -1;
}
static const char *tname(int t){
	switch(t){case 1:return"DATA";case 3:return"RESET_RSP";case 7:return"DEVICE_DESC";case 8:return"RPT_DESC";default:return"none/invalid";}
}
static s64 wait_edge_us(int max_us){
	ktime_t t0=ktime_get();
	bool was=pin_asserted();
	while(was && ktime_us_delta(ktime_get(),t0)<max_us){ was=pin_asserted(); cpu_relax(); }
	/* stuck asserted for the whole window -- no idle baseline to time a real edge from;
	 * report distinctly (-2) instead of falling into the second loop and reporting a bogus ~0us edge */
	if(was) return -2;
	t0=ktime_get();
	while(ktime_us_delta(ktime_get(),t0)<max_us){
		if(pin_asserted()) return ktime_us_delta(ktime_get(),t0);
		cpu_relax();
	}
	return -1;
}

/* proven read path (unchanged across all variants -- only the WRITE path is varied) */
static int do_read(u32 addr,int rx,u8 *buf){
	u32 c; u8 tx[3]={(addr>>16)&0xFF,(addr>>8)&0xFF,addr&0xFF};
	c=r32(CTRL0); c&=~BIT(20); w32(CTRL0,c); c|=BIT(20); w32(CTRL0,c);
	w8(OPCODE,0x0B);
	c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c);
	w8(TX_COUNT,3); fifo_write8(FIFO,tx,3); w8(RX_COUNT,rx);
	c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c);
	wmb(); w8(TRIGGER,0x80);
	if(wait_busy_ctrl0()) return -1;
	fifo_read16(FIFO+3,buf,rx); w8(OPCODE,0x0B); return 0;
}

struct write_opts {
	bool rmw_trigger;      /* variant A */
	bool status_busywait;  /* variant B */
	bool skip_fastread;    /* variant C */
	const char *label;
};

static int do_write_variant(u8 op,const u8 *tx,int tx_len,const struct write_opts *o){
	u32 c;
	c=r32(CTRL0); c&=~BIT(20); w32(CTRL0,c); c|=BIT(20); w32(CTRL0,c); /* FIFO_CLEAR single-set-equivalent (already tested identical to toggle) */
	w8(OPCODE,op);
	if(!o->skip_fastread){ c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c); }
	w8(TX_COUNT,tx_len); fifo_write8(FIFO,tx,tx_len); w8(RX_COUNT,0);
	if(!o->skip_fastread){ c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c); }
	wmb();
	if(o->rmw_trigger){ w8(TRIGGER, r8(TRIGGER) | 0x80); } else { w8(TRIGGER,0x80); }
	if(o->status_busywait){ if(wait_busy_status32()) return -1; }
	else { if(wait_busy_ctrl0()) return -1; }
	w8(OPCODE,0x0B); return 0;
}

static void run_variant(const struct write_opts *o, u8 *rx){
	s64 us; int t, round;
	u8 descreq[]={0x02,0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};

	for(round=0; round<5; round++){
		/* drain to a known state first */
		do_read(0x000000, 10, rx);
		if(hid_type(rx,10)==3) do_read(0x000000, 20, rx);

		(void)pin_asserted();
		if(do_write_variant(descreq[0], descreq+1, sizeof(descreq)-1, o) != 0){
			pr_info("uert: [%s] round %d: do_write FAILED (busy timeout)\n", o->label, round);
			continue;
		}
		us = wait_edge_us(200000);
		do_read(0x000000, 37, rx);
		t = hid_type(rx,37);
		pr_info("uert: [%-20s] round %d: edge=%lldus type=%d(%s)%s\n", o->label, round, us, t, tname(t),
			(t==7)?"  <<<< DEVICE_DESC":"");
	}
}

static int __init uert_init(void){
	u8 rx[64];
	struct write_opts variants[5] = {
		{ false, false, false, "baseline (ours)" },
		{ true,  false, false, "A: RMW trigger" },
		{ false, true,  false, "B: STATUS busy-wait" },
		{ false, false, true,  "C: skip FAST_READ" },
		{ true,  true,  true,  "D: A+B+C combined" },
	};
	int i;

	pr_info("uert: ================================================================\n");
	pr_info("uert: UEFI EXACT REPLICA TEST -- every remaining structural difference\n");
	pr_info("uert: from the Windows/UEFI/Linux 3-way comparison (GROUND_TRUTH 15.18-19)\n");
	pr_info("uert: ================================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if(!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!gpio){ iounmap(base); return -ENOMEM; }
	select_cs1();

	for(i=0;i<5;i++){
		run_variant(&variants[i], rx);
		msleep(300);
	}

	pr_info("uert: DONE -- if none show DEVICE_DESC, all remaining structural deltas are ruled out\n");

	w8(OPCODE,0x0B);
	iounmap(gpio);
	iounmap(base);
	return -ENODEV;
}
module_init(uert_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test every remaining Windows/UEFI/Linux SPI structural difference on the DESCREQ path");
