/* irq_oracle.c — Use the DEVICE INTERRUPT LINE as the oracle for "did the write reach the device".
 *
 * RATIONALE (why every previous write test was inconclusive):
 *   The whole "writes are ignored" verdict rests on MISO being zero DURING the 0x02 write.
 *   But HIDSPI_PROTOCOL.md itself says MISO during a write is garbage (FF*5 + reset body),
 *   NOT the response. The real response is asynchronous: the device raises its interrupt on
 *   GPIO pin 0x55 (85) ~58us after the DESCREQ, and ONLY THEN is a 0x0B read valid.
 *   descreq_test.c reads either immediately (too early) or after msleep(10) (too late) and
 *   never looks at the interrupt line. So we have never actually tested the correct question.
 *
 * THIS TOOL answers, in software only, no logic analyzer:
 *   Does sending DESCREQ (0x02 -> reg 0x000001) make the device pull pin 85 LOW within ~1ms?
 *     YES + a following 0x0B read returns type=7 (DEVICE_DESC) -> THE WRITE WORKS. The bug is
 *          that our reads were never synchronised to the interrupt. Overturns "software exhausted".
 *     NO  (pin 85 keeps the slow reset-loop cadence, read still type=3 RESET_RSP)
 *          -> the write genuinely never reaches the device. Confirms the physical verdict.
 *
 * Pin 85 raw input level = bit 16 (PIN_STS) of AMD-GPIO reg (0xFED81500 + 85*4 = 0xFED81654).
 * We ONLY READ that register (never write it) so we cannot disturb pinctrl-amd.
 * The GpioInt is ActiveLow -> "device wants attention" = level LOW = bit16 == 0.
 *
 * insmod irq_oracle.ko && dmesg | grep "orac:"
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
#define PIN_REG   (IRQ_PIN * 4)          /* 0x154 */
#define PIN_STS   (1u << 16)             /* raw input level of the pin */

static void __iomem *base;               /* FCH SPI controller */
static void __iomem *gpio;               /* AMD GPIO bank       */

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

/* SPI clock speed control (replicated from spi-amd.c amd_set_spi_freq(), bit-for-bit,
 * so a version mismatch here can't accidentally program an undefined/unsafe combination).
 * irq_oracle never called this before — it always inherited whatever ENA_REG/SPEED_REG
 * happened to already contain (firmware POR default, since no driver was bound when it
 * ran today). VERIFICATION_FINDINGS.md marks "SPI speed 33.33 MHz" as the value confirmed
 * to match Windows, but that is only what the REAL driver programs at its own probe time
 * — with no driver loaded, today's decisive test ran at an unknown/unverified speed. */
#define ENA_REG        0x20
#define ALT_SPD_SHIFT  20
#define ALT_SPD_MASK   (0xFu << ALT_SPD_SHIFT)   /* bits 23:20 */
#define SPI100_BIT     0x1u                       /* bit 0 */
#define SPEED_REG      0x6C
#define SPD7_SHIFT     8
#define SPD7_MASK      (0x3Fu << SPD7_SHIFT)      /* bits 13:8 */

enum { F_66_66=0, F_33_33=1, F_22_22=2, F_16_66=3, F_100=4, F_800K=5, SPD7=7, F_50=8, F_4=9, F_3_17=10 };
struct freq_entry { u32 hz; u32 enable_val; u32 spd7_val; const char *name; };
static const struct freq_entry freqs[] = {
	{100000000, F_100,   0, "100MHz"},
	{ 66660000, F_66_66, 0, "66.66MHz"},
	{ 50000000, SPD7, F_50, "50MHz"},
	{ 33330000, F_33_33, 0, "33.33MHz (Windows-matched, VERIFICATION_FINDINGS.md)"},
	{ 22220000, F_22_22, 0, "22.22MHz"},
	{ 16660000, F_16_66, 0, "16.66MHz"},
	{  4000000, SPD7, F_4, "4MHz"},
	{  3170000, SPD7, F_3_17, "3.17MHz"},
	{   800000, F_800K, 0, "800kHz"},
};

static void set_spi_freq(const struct freq_entry *f){
	u32 ena = ioread32(base+ENA_REG);
	ena = (ena & ~ALT_SPD_MASK) | ((f->enable_val << ALT_SPD_SHIFT) & ALT_SPD_MASK);
	if(f->hz == 100000000) ena |= SPI100_BIT;   /* matches driver: only ever SET, mirrors real amd_set_spi_freq() */
	iowrite32(ena, base+ENA_REG);
	if(f->spd7_val){
		u32 spd = ioread32(base+SPEED_REG);
		spd = (spd & ~SPD7_MASK) | ((f->spd7_val << SPD7_SHIFT) & SPD7_MASK);
		iowrite32(spd, base+SPEED_REG);
	}
}

/* --- pin 85 live level (true = LOW = device asserting the interrupt) --- */
static bool pin_asserted(void){ return !(ioread32(gpio + PIN_REG) & PIN_STS); }

static void select_cs1(void){ u8 v=r8(ALT_CS); w8(ALT_CS,(v&0xFC)|0x01); }
static int wait_busy(void){ u32 v; return readl_poll_timeout(base+CTRL0,v,!(v&BIT(31)),20,2000000); }

static void fifo_write8(int off,const u8 *d,int n){ int i; for(i=0;i<n;i++) w8(off+i,d[i]); }
static void fifo_read16(int off,u8 *dst,int n){
	int i; for(i=0;i<n;i++){ u16 w=ioread16(base+off+(i/2)*2); dst[i]=(i&1)?(u8)(w>>8):(u8)(w&0xFF); }
}

/* proven read path from descreq_test.c */
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

/* proven write path (TX-only, RX_COUNT=0) from descreq_test.c */
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

/* Busy-poll pin 85 for a falling edge (idle HIGH -> asserted LOW) up to max_us.
 * Returns microseconds to the edge, or -1 if none within the window. No sleeping. */
static s64 wait_edge_us(int max_us){
	ktime_t t0=ktime_get();
	bool was=pin_asserted();
	/* if it is already asserted at entry, wait for it to go idle first so we time a real edge */
	while(was && ktime_us_delta(ktime_get(),t0)<max_us){ was=pin_asserted(); cpu_relax(); }
	t0=ktime_get();
	while(ktime_us_delta(ktime_get(),t0)<max_us){
		if(pin_asserted()) return ktime_us_delta(ktime_get(),t0);
		cpu_relax();
	}
	return -1;
}

static int __init orac_init(void){
	u8 rx[64];
	s64 us; int t, i, edges;
	ktime_t t0;

	pr_info("orac: =========================================================\n");
	pr_info("orac: INTERRUPT-LINE ORACLE — is the DESCREQ write reaching the device?\n");
	pr_info("orac: =========================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if(!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!gpio){ iounmap(base); return -ENOMEM; }
	select_cs1();

	pr_info("orac: pin85 raw reg=0x%08x  level=%s (idle should be HIGH/not-asserted)\n",
		ioread32(gpio+PIN_REG), pin_asserted()?"LOW/ASSERTED":"HIGH/idle");

	/* (S) Log the speed actually in effect BEFORE touching it (this is what today's earlier
	 * decisive run actually used — no driver was loaded, so it's whatever firmware POR left),
	 * then explicitly program the Windows-matched 33.33MHz so the rest of this run is
	 * controlled instead of an unverified inherited value. */
	pr_info("orac: [S] pre-existing ENA_REG=0x%08x SPEED_REG=0x%08x (ALT_SPD=0x%x, SPD7=0x%x) — this is what the earlier decisive run actually used\n",
		r32(ENA_REG), r32(SPEED_REG),
		(r32(ENA_REG) & ALT_SPD_MASK) >> ALT_SPD_SHIFT,
		(r32(SPEED_REG) & SPD7_MASK) >> SPD7_SHIFT);
	set_spi_freq(&freqs[3]); /* 33.33MHz, Windows-matched */
	pr_info("orac: [S] forced 33.33MHz (Windows-matched): ENA_REG=0x%08x SPEED_REG=0x%08x\n",
		r32(ENA_REG), r32(SPEED_REG));

	/* (P) PRIME: if pin85 is already asserted at load time, an interrupt from a previous
	 *     RESET_RSP is pending and NEVER got drained/acked (no driver was bound). Every
	 *     baseline/timing measurement below is meaningless while stuck in that state, so
	 *     read header (10B, need the byte at offset 9 for the 0x5A discriminator — the
	 *     previous version of this tool read only 9B and always saw type=-1) and, if it
	 *     is RESET_RSP, drain a body too (mirrors spi-hid-core.c's state0-clean drain),
	 *     then re-check the pin. Bounded to 5 tries / 50ms total, read-only on GPIO. */
	if(pin_asserted()){
		int tries;
		pr_info("orac: [P] pin85 already asserted at load — priming/draining before baseline\n");
		for(tries=0; tries<5 && pin_asserted(); tries++){
			do_read(0x000000, 10, rx);
			t = hid_type(rx,10);
			pr_info("orac: [P]  try %d: header read type=%d(%s) raw=[%*ph]\n", tries, t, tname(t), 10, rx);
			if(t==3){
				u8 body[20];
				do_read(0x000000, sizeof(body), body);
				pr_info("orac: [P]  body drain=[%*ph]\n", (int)sizeof(body), body);
			}
			usleep_range(1000,2000);
		}
		pr_info("orac: [P] post-prime pin85 level=%s\n", pin_asserted()?"LOW/ASSERTED (still stuck)":"HIGH/idle (drained OK)");
	}

	/* (A) BASELINE: how often does the device raise the line on its own (reset loop)?
	 *     Count falling edges over 700ms WITHOUT sending anything — wide enough to span
	 *     the ~109-609ms reset-loop cadence measured in earlier sessions (a 60ms window
	 *     can legitimately see 0 edges even when the device is healthy and idle). */
	edges=0; t0=ktime_get();
	{
		bool prev=pin_asserted();
		while(ktime_ms_delta(ktime_get(),t0)<700){
			bool now=pin_asserted();
			if(now && !prev) edges++;
			prev=now; cpu_relax();
		}
	}
	pr_info("orac: [A] baseline spontaneous edges in 700ms = %d  (this is the reset-loop cadence)\n", edges);

	/* (B) Drain whatever is pending: a couple of reset reads to reach a known state. */
	do_read(0x000000, 10, rx);
	t = hid_type(rx,10);
	pr_info("orac: [B] pre-DESCREQ read type=%d(%s) raw=[%*ph]\n", t, tname(t), 10, rx);

	/* (C) CONTROL: measure time to next spontaneous edge WITHOUT a write, 5ms window.
	 *     If the device spontaneously toggles within 5ms, an edge after DESCREQ is not
	 *     necessarily caused by our write — this control tells us the false-positive rate. */
	us = wait_edge_us(5000);
	pr_info("orac: [C] control (no write) time-to-edge = %lld us %s\n",
		us, us<0?"(no spontaneous edge in 5ms — good, clean baseline)":"(device toggles on its own this fast)");

	/* (D) THE TEST: send DESCREQ, then race to catch the follow-up edge (Windows sees ~58us).
	 *     Window widened to 200ms: the [G] sweep below showed a real (non-spontaneous, idle
	 *     cadence is ~609ms) edge at 2.6ms in one round, so 5ms was too short to catch it
	 *     reliably every time — 200ms safely spans the ~109ms accelerated reset-loop cadence
	 *     seen in earlier sessions once the device has been poked. */
	{
		u8 descreq[]={0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		/* prime the level so wait_edge_us times a genuine post-write edge */
		(void)pin_asserted();
		do_write(0x02, descreq, sizeof(descreq));
		us = wait_edge_us(200000);
	}
	pr_info("orac: [D] DESCREQ sent. time-to-edge = %lld us  (Windows reference: ~58 us)\n", us);

	/* (E) VERDICT read: the instant the edge fired (or after the window) read the header. */
	do_read(0x000000, 10, rx);
	t = hid_type(rx,10);
	pr_info("orac: [E] post-edge read type=%d(%s) raw=[%*ph]\n", t, tname(t), 10, rx);

	pr_info("orac: ---------------------------------------------------------\n");
	if(us>=0 && us<1000 && t==7){
		pr_info("orac: VERDICT: *** WRITE REACHES THE DEVICE *** edge %lldus + DEVICE_DESC.\n", us);
		pr_info("orac:          The bug is READ-vs-IRQ SYNCHRONISATION, not the write. FIXABLE IN SW.\n");
	} else if(us>=0 && us<1000 && t!=7){
		pr_info("orac: VERDICT: tight edge (%lldus) BUT read still type=%d(%s).\n", us, t, tname(t));
		pr_info("orac:          Write may reach device but response read/register is wrong — investigate read addr/len/timing.\n");
	} else if(us>=1000 && t==3){
		pr_info("orac: VERDICT: device DID react to the DESCREQ (edge at %lldus, far faster than the\n", us);
		pr_info("orac:          ~609ms idle cadence) but only re-asserts RESET_RSP, never DEVICE_DESC.\n");
		pr_info("orac:          Not a read/IRQ sync bug (we caught the edge and read it correctly).\n");
		pr_info("orac:          The device receives the write, restarts its reset-loop, and never\n");
		pr_info("orac:          progresses past RESET_RSP — confirms the reset-loop pathology.\n");
	} else if(us<0 && edges>0){
		pr_info("orac: VERDICT: NO edge from our DESCREQ within the window, but device DOES toggle\n");
		pr_info("orac:          spontaneously (%d edge(s)/700ms baseline). The write is silently dropped.\n", edges);
	} else {
		pr_info("orac: VERDICT: no edge at all (baseline %d, post-write us=%lld). Device not signalling — check reset/power state.\n", edges, us);
	}
	pr_info("orac: ---------------------------------------------------------\n");

	/* (F) If any edge came at all, chase the full descriptor immediately (37B) — informative
	 *     for both the tight-Windows-like case and the slow reset-loop-reaction case. */
	if(us>=0){
		do_read(0x000000, 37, rx);
		t = hid_type(rx,37);
		pr_info("orac: [F] immediate 37B read: type=%d(%s) raw=[%*ph]\n",
			t, tname(t), 37, rx);
	}

	/* Also sample the reset-loop N times reading tightly on each edge, to see if ANY read
	 * ever returns something other than RESET_RSP (would prove writes land intermittently).
	 * Window widened to 200ms to reliably catch the accelerated (~109ms) post-write cadence
	 * instead of missing it like the original 3ms window did in 9/10 rounds. */
	pr_info("orac: [G] edge-synced sweep (10 rounds): DESCREQ then read on first edge:\n");
	for(i=0;i<10;i++){
		u8 dq[]={0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		(void)pin_asserted();
		do_write(0x02, dq, sizeof(dq));
		us = wait_edge_us(200000);
		do_read(0x000000, 10, rx);
		t = hid_type(rx,10);
		pr_info("orac:    round %d: edge=%lldus type=%d(%s)\n", i, us, t, tname(t));
	}

	/* (H) SPEED SWEEP: the old "0.8-33MHz matrix" (pre-svolta) judged success via a delayed,
	 * non-IRQ-synced read. Redo it with the correct methodology (edge-timed read) across
	 * every documented speed tier, in case some untested speed produces DEVICE_DESC instead
	 * of RESET_RSP on the tight edge. Bounded: 9 speeds x <=200ms = ~1.8s worst case. */
	pr_info("orac: [H] speed sweep (correct edge-synced methodology this time):\n");
	for(i=0;i<(int)(sizeof(freqs)/sizeof(freqs[0]));i++){
		u8 dq[]={0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
		set_spi_freq(&freqs[i]);
		/* drain whatever is pending at this speed before judging the DESCREQ response */
		do_read(0x000000, 10, rx);
		if(hid_type(rx,10)==3) do_read(0x000000, 20, rx);
		(void)pin_asserted();
		do_write(0x02, dq, sizeof(dq));
		us = wait_edge_us(200000);
		do_read(0x000000, 10, rx);
		t = hid_type(rx,10);
		pr_info("orac:    %-45s edge=%lldus type=%d(%s)%s\n", freqs[i].name, us, t, tname(t),
			(t==7)?"  <<<< DEVICE_DESC, DIFFERENT FROM EVERYTHING ELSE TODAY":"");
	}
	set_spi_freq(&freqs[3]); /* leave the controller at the Windows-matched 33.33MHz on exit */
	pr_info("orac: [H] sweep done, speed restored to 33.33MHz\n");

	w8(OPCODE,0x0B);
	iounmap(gpio);
	iounmap(base);
	pr_info("orac: DONE\n");
	return -ENODEV;
}
module_init(orac_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Use GPIO-85 interrupt edge as oracle for DESCREQ write success (SL4A touchscreen)");
