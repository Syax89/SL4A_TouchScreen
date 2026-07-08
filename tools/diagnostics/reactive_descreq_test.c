/* reactive_descreq_test.c — test the "reaction window" hypothesis: does the device only
 * accept DESCREQ within a short window after freshly asserting RESET_RSP?
 *
 * MOTIVATION (docs/CSV_SEQUENCE.md, surface_boot_auto.csv, a real Windows warm-boot ETW
 * trace): Windows reads the RESET_RSP header (TXN #0), reads its body 168us later (TXN #1),
 * then sends DESCREQ only 147us after that (TXN #2) -- i.e. Windows reacts to a freshly
 * observed RESET_RSP and fires DESCREQ within ~300us total. Every diagnostic tool in this
 * project so far (irq_oracle.c, uefi_exact_replica_test.c, etc.) sends DESCREQ "blind" --
 * at an arbitrary phase of the ongoing reset cycle, not synced to having just observed a
 * fresh RESET_RSP assertion. This has never been tested. If the device only accepts a new
 * command within a short window after asserting RESET_RSP (then ignores/resets again), this
 * would explain why byte-identical writes always fail on Linux but always succeed on
 * Windows -- pure timing, not content.
 *
 * THIS TOOL: waits for the pin85 falling edge (a fresh RESET_RSP assertion, spontaneous
 * idle-cadence or otherwise), then reacts as fast as software allows -- read header, read
 * body, send DESCREQ, zero artificial delay anywhere -- replicating Windows's read-read-write
 * reaction profile instead of firing blind. Times the whole reaction and the response.
 *
 * insmod reactive_descreq_test.ko && dmesg | grep "rdt:"
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

/* proven read/write path, identical to irq_oracle.c */
static int do_read(u32 addr,int rx,u8 *buf){
	u32 c; u8 tx[3]={(addr>>16)&0xFF,(addr>>8)&0xFF,addr&0xFF};
	c=r32(CTRL0); c&=~BIT(20); w32(CTRL0,c); c|=BIT(20); w32(CTRL0,c);
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
	c=r32(CTRL0); c&=~BIT(20); w32(CTRL0,c); c|=BIT(20); w32(CTRL0,c);
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
	if(was) return -2; /* stuck asserted for the whole window */
	t0=ktime_get();
	while(ktime_us_delta(ktime_get(),t0)<max_us){
		if(pin_asserted()) return ktime_us_delta(ktime_get(),t0);
		cpu_relax();
	}
	return -1;
}

#define ROUNDS 15

static int __init rdt_init(void){
	u8 hdr[10], body[10], resp[37];
	int round, n_reacted = 0, n_type7 = 0;
	s64 idle_edge_us, react_us, resp_edge_us;
	ktime_t t_edge, t_after_write;
	int hdr_type, resp_type;

	/* DESCREQ FIFO payload (9 bytes, opcode 0x02 written to OPCODE register separately --
	 * this is the already-proven byte-identical-to-Windows format from irq_oracle.c) */
	static const u8 descreq[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};

	pr_info("rdt: ================================================================\n");
	pr_info("rdt: REACTIVE DESCREQ TEST -- react to a fresh RESET_RSP edge as fast as\n");
	pr_info("rdt: software allows (read header, read body, send DESCREQ, zero sleep),\n");
	pr_info("rdt: matching Windows's ~300us reaction profile (docs/CSV_SEQUENCE.md).\n");
	pr_info("rdt: ================================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if(!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!gpio){ iounmap(base); return -ENOMEM; }
	select_cs1();

	pr_info("rdt: pin85=%s at load\n", pin_asserted()?"LOW/ASSERTED":"HIGH/idle");

	/* drain any stale pending interrupt before starting real measurement */
	if(pin_asserted()){
		int tries;
		for(tries=0; tries<5 && pin_asserted(); tries++){
			do_read(0x000000, 10, hdr);
			if(hid_type(hdr,10)==3) do_read(0x000000, 20, hdr);
		}
		pr_info("rdt: post-prime pin85=%s\n", pin_asserted()?"LOW/ASSERTED (stuck)":"HIGH/idle");
	}

	for(round=0; round<ROUNDS; round++){
		/* (1) wait for the NEXT falling edge -- spontaneous idle-cadence RESET_RSP,
		 *     or the accelerated post-DESCREQ one from the previous round. Window wide
		 *     enough to span the known ~609ms idle / ~108ms accelerated cadence. */
		idle_edge_us = wait_edge_us(800000);
		if(idle_edge_us < 0){
			pr_info("rdt: round %2d: no spontaneous edge in 800ms (idle_edge=%lld) -- skipping\n",
				round, idle_edge_us);
			continue;
		}
		t_edge = ktime_get();

		/* (2) REACT: read header immediately -- no sleep, minimal ops */
		do_read(0x000000, 10, hdr);
		hdr_type = hid_type(hdr,10);
		if(hdr_type != 3){
			pr_info("rdt: round %2d: post-edge header type=%d(%s), not RESET_RSP -- skipping\n",
				round, hdr_type, tname(hdr_type));
			continue;
		}

		/* (3) read body -- mirrors Windows TXN #1 (body drain), still zero sleep */
		do_read(0x000000, 10, body);

		/* (4) send DESCREQ immediately -- mirrors Windows TXN #2 */
		if(do_write(0x02, descreq, sizeof(descreq)) != 0){
			pr_info("rdt: round %2d: do_write FAILED (busy timeout)\n", round);
			continue;
		}
		t_after_write = ktime_get();
		react_us = ktime_us_delta(t_after_write, t_edge);
		n_reacted++;

		/* (5) wait for the response edge and read it */
		resp_edge_us = wait_edge_us(200000);
		do_read(0x000000, 37, resp);
		resp_type = hid_type(resp,37);
		if(resp_type == 7) n_type7++;

		pr_info("rdt: round %2d: idle_edge=%lldus react(edge->write done)=%lldus resp_edge=%lldus type=%d(%s)%s\n",
			round, idle_edge_us, react_us, resp_edge_us, resp_type, tname(resp_type),
			(resp_type==7)?"  <<<< DEVICE_DESC":"");
	}

	pr_info("rdt: ----------------------------------------------------------------\n");
	pr_info("rdt: DONE: %d/%d rounds reacted successfully, %d/%d got DEVICE_DESC\n",
		n_reacted, ROUNDS, n_type7, ROUNDS);
	pr_info("rdt: Windows reference (docs/CSV_SEQUENCE.md): header-read -> body-read -> DESCREQ\n");
	pr_info("rdt: in ~300us total (168us + 147us gaps). Compare against react= above.\n");

	w8(OPCODE,0x0B);
	iounmap(gpio);
	iounmap(base);
	return -ENODEV;
}
module_init(rdt_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test whether the device only accepts DESCREQ within a short window after a fresh RESET_RSP assertion");
