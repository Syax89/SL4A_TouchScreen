/* fifo_clear_singleset_test.c — test the user's "flip the problem" hypothesis:
 * maybe Linux does something extra that Windows does NOT do, rather than Windows doing
 * something extra that Linux is missing.
 *
 * The one concrete, already-documented (SPI_REGISTERS.md) but never individually tested
 * candidate: CTRL0 bit20 (FIFO_CLEAR). Windows does a SINGLE SET (read, OR in bit20, write)
 * before every transaction. Our driver (and irq_oracle.c) does an explicit clear->set
 * TOGGLE (read, AND-NOT bit20 write, then OR bit20 write) before every transaction --
 * every single read AND write, always, an extra step Windows never performs.
 *
 * This tool reruns the same decisive edge-synced DESCREQ test from irq_oracle.c (proven
 * methodology: read 10B for correct type detection, prime/drain if pin85 already
 * asserted, then send DESCREQ and catch the interrupt edge precisely) but with FIFO_CLEAR
 * done Windows' way (single set only, never cleared first) for BOTH the read and the
 * write path. If the reset-loop pathology changes (edge tight + DEVICE_DESC instead of
 * the usual ~109ms + RESET_RSP), the toggle is implicated. If identical, this specific
 * "Linux does extra" candidate is closed too.
 *
 * insmod fifo_clear_singleset_test.ko && dmesg | grep "fcst:"
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
#define FIFO 0x80
#define ALT_CS 0x1D

static bool pin_asserted(void){ return !(ioread32(gpio + PIN_REG) & PIN_STS); }
static void select_cs1(void){ u8 v=r8(ALT_CS); w8(ALT_CS,(v&0xFC)|0x01); }
static int wait_busy(void){ u32 v; return readl_poll_timeout(base+CTRL0,v,!(v&BIT(31)),20,2000000); }
static void fifo_write8(int off,const u8 *d,int n){ int i; for(i=0;i<n;i++) w8(off+i,d[i]); }
static void fifo_read16(int off,u8 *dst,int n){
	int i; for(i=0;i<n;i++){ u16 w=ioread16(base+off+(i/2)*2); dst[i]=(i&1)?(u8)(w>>8):(u8)(w&0xFF); }
}

/* Windows-style FIFO_CLEAR: single set, never cleared first. */
static void fifo_clear_winstyle(void){ u32 c=r32(CTRL0); c|=BIT(20); w32(CTRL0,c); }

static int do_read(u32 addr,int rx,u8 *buf){
	u32 c; u8 tx[3]={(addr>>16)&0xFF,(addr>>8)&0xFF,addr&0xFF};
	fifo_clear_winstyle();
	w8(OPCODE,0x0B);
	c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c);
	w8(TX_COUNT,3); fifo_write8(FIFO,tx,3); w8(RX_COUNT,rx);
	c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c);
	wmb(); w8(TRIGGER,0x80);
	if(wait_busy()) return -1;
	fifo_read16(FIFO+3,buf,rx); w8(OPCODE,0x0B); return 0;
}

static int do_write(u8 op,const u8 *tx,int tx_len){
	u32 c;
	fifo_clear_winstyle();
	w8(OPCODE,op);
	c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c);
	w8(TX_COUNT,tx_len); fifo_write8(FIFO,tx,tx_len); w8(RX_COUNT,0);
	c=r32(CTRL0); c|=0x60040000; w32(CTRL0,c);
	wmb(); w8(TRIGGER,0x80);
	if(wait_busy()) return -1;
	w8(OPCODE,0x0B); return 0;
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

static int __init fcst_init(void){
	u8 rx[64];
	s64 us; int t, i, tries;

	pr_info("fcst: ================================================================\n");
	pr_info("fcst: FIFO_CLEAR single-set (Windows style) vs toggle (Linux style) test\n");
	pr_info("fcst: every read/write below uses SINGLE-SET ONLY, never clears bit20 first\n");
	pr_info("fcst: ================================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if(!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!gpio){ iounmap(base); return -ENOMEM; }
	select_cs1();

	pr_info("fcst: pin85 level=%s at load\n", pin_asserted()?"LOW/ASSERTED":"HIGH/idle");

	if(pin_asserted()){
		for(tries=0; tries<5 && pin_asserted(); tries++){
			do_read(0x000000, 10, rx);
			t = hid_type(rx,10);
			pr_info("fcst: [prime] try %d type=%d(%s) raw=[%*ph]\n", tries, t, tname(t), 10, rx);
			if(t==3){ u8 body[20]; do_read(0x000000, sizeof(body), body); }
			usleep_range(1000,2000);
		}
	}

	do_read(0x000000, 10, rx);
	t = hid_type(rx,10);
	pr_info("fcst: pre-test drain read type=%d(%s) raw=[%*ph]\n", t, tname(t), 10, rx);

	pr_info("fcst: edge-synced DESCREQ sweep (10 rounds), single-set FIFO_CLEAR throughout:\n");
	for(i=0;i<10;i++){
		u8 dq[]={0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		(void)pin_asserted();
		if(do_write(0x02, dq, sizeof(dq)) != 0){
			pr_info("fcst:    round %d: do_write FAILED (busy timeout) — skipping\n", i);
			continue;
		}
		us = wait_edge_us(200000);
		do_read(0x000000, 10, rx);
		t = hid_type(rx,10);
		pr_info("fcst:    round %d: edge=%lldus type=%d(%s)%s\n", i, us, t, tname(t),
			(t==7)?"  <<<< DEVICE_DESC -- FIFO_CLEAR TOGGLE WAS THE CULPRIT":"");
	}

	w8(OPCODE,0x0B);
	iounmap(gpio);
	iounmap(base);
	pr_info("fcst: DONE\n");
	return -ENODEV;
}
module_init(fcst_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test FIFO_CLEAR single-set (Windows) vs toggle (Linux) on the DESCREQ reset-loop");
