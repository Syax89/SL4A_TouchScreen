/* uefi_gpio_replay_test.c — replay the EXACT reset/DESCREQ retry loop found by
 * decompiling MsTouchUnlockDxe.efi and SurfaceTouchHidDxe.efi (the real UEFI drivers
 * that make the touchscreen work in BIOS setup -- confirmed working by the user).
 *
 * Ground truth from Ghidra decompilation (tools/ghidra, see GROUND_TRUTH.md 15.17):
 *
 *   FUN_00002010 (the actual ResetTouchController retry loop):
 *     for (attempt = 0; attempt < 3; attempt++) {
 *         MMIO32(0xFED8120C) |= 0x400000;         // SET bit22 -- NEVER done by our driver
 *         wait_for_pin85_asserted(up to 1000ms, polling every 1ms);   // FUN_0000218c
 *         if (edge) {
 *             read 9B header (opcode 0x0B, addr 0, dummy 0xFF);       // FUN_00001ec4
 *             if (type == 3 /RESET_RSP/) {
 *                 send DESCREQ 02 00 00 01 42 00 00 03 00 00 (10B);   // FUN_000020fc
 *                 if (ok) break;  // SUCCESS
 *             }
 *         }
 *         MMIO32(0xFED8120C) &= ~0x400000;        // CLEAR bit22 on failure
 *         delay(300ms);
 *         // retry
 *     }
 *
 *   FUN_0000218c (wait for IRQ, the pin85-polling primitive):
 *     poll MMIO32(0xFED81654) bit16 == 0 (our exact PIN_STS/pin85), 1ms steps, up to 1000 tries.
 *
 * 0xFED8120C is NOT the per-pin status register (that's 0xFED81654, which we already use
 * correctly everywhere). It sits in a different part of the same AMD GPIO MMIO block and is
 * NEVER touched by irq_oracle.c, spi-hid-core.c, or any prior diagnostic tool in this
 * project -- confirmed by grep across the whole repo. This is the single most concrete,
 * ground-truth-sourced (not guessed) candidate found so far for what Windows/UEFI does
 * that Linux does not.
 *
 * This tool replicates the loop with our own already-proven do_read/do_write SPI helpers
 * (byte-identical to what irq_oracle.c uses, already verified correct at the SPI-framing
 * level many times over) and adds ONLY the 0xFED8120C bit22 dance around it, exactly as
 * decompiled. If DEVICE_DESC appears where it never has before, this is the fix.
 *
 * insmod uefi_gpio_replay_test.ko && dmesg | grep "ugrt:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/ktime.h>

#define FCH_BASE 0xFEC10000
#define FCH_SIZE 0x100
#define GPIO_BASE 0xFED81000   /* widened to cover both 0x20C (master ctrl) and 0x654 (pin85) */
#define GPIO_SIZE 0x800
#define PIN85_OFF   0x654
#define PIN_STS     (1u << 16)
#define MASTER_OFF  0x20C
#define MASTER_BIT22 (1u << 22)

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

static bool pin85_asserted(void){ return !(ioread32(gpio + PIN85_OFF) & PIN_STS); }
static void master_set_bit22(void){ iowrite32(ioread32(gpio+MASTER_OFF) | MASTER_BIT22, gpio+MASTER_OFF); }
static void master_clear_bit22(void){ iowrite32(ioread32(gpio+MASTER_OFF) & ~MASTER_BIT22, gpio+MASTER_OFF); }

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

/* microsecond-precision edge timer, same proven methodology as irq_oracle.c (15.9/15.10):
 * a plain msleep()-then-read misses everything, since the reset-loop's own re-assertion
 * lands at ~109ms, not within a millisecond of the write. */
static s64 wait_edge_us(int max_us){
	ktime_t t0=ktime_get();
	bool was=pin85_asserted();
	while(was && ktime_us_delta(ktime_get(),t0)<max_us){ was=pin85_asserted(); cpu_relax(); }
	t0=ktime_get();
	while(ktime_us_delta(ktime_get(),t0)<max_us){
		if(pin85_asserted()) return ktime_us_delta(ktime_get(),t0);
		cpu_relax();
	}
	return -1;
}

/* FUN_0000218c: poll pin85 asserted, 1ms steps, up to 1000 tries (~1000ms) */
static bool wait_for_pin85(void){
	int tries;
	for(tries=0; tries<1000; tries++){
		if(pin85_asserted()) return true;
		usleep_range(900,1100);
	}
	return false;
}

static int __init ugrt_init(void){
	u8 rx[64];
	int attempt, t;
	bool ok = false;

	pr_info("ugrt: ===============================================================\n");
	pr_info("ugrt: UEFI GPIO REPLAY -- exact ResetTouchController loop from decompiled\n");
	pr_info("ugrt: MsTouchUnlockDxe.efi/SurfaceTouchHidDxe.efi (0xFED8120C bit22 dance)\n");
	pr_info("ugrt: ===============================================================\n");

	base = ioremap(FCH_BASE, FCH_SIZE);
	if(!base) return -ENOMEM;
	gpio = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!gpio){ iounmap(base); return -ENOMEM; }
	select_cs1();

	pr_info("ugrt: pin85=%s  master(0xFED8120C)=0x%08x (bit22=%d) at load\n",
		pin85_asserted()?"LOW/ASSERTED":"HIGH/idle",
		ioread32(gpio+MASTER_OFF), !!(ioread32(gpio+MASTER_OFF)&MASTER_BIT22));

	for(attempt=0; attempt<3 && !ok; attempt++){
		pr_info("ugrt: --- attempt %d ---\n", attempt);
		master_set_bit22();
		pr_info("ugrt:   master after SET = 0x%08x\n", ioread32(gpio+MASTER_OFF));

		if(wait_for_pin85()){
			/* 10B (not 9B) so the 0x5A discriminator is actually captured -- see the
			 * off-by-one bug fixed in irq_oracle.c (GROUND_TRUTH.md 15.9) */
			do_read(0x000000, 10, rx);
			t = hid_type(rx,10);
			pr_info("ugrt:   post-edge read type=%d(%s) raw=[%*ph]\n", t, tname(t), 10, rx);
			if(t==3){
				u8 descreq[]={0x02,0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
				s64 us;
				(void)pin85_asserted();
				if(do_write(descreq[0], descreq+1, sizeof(descreq)-1)==0){
					/* proper edge-synced wait (200ms window, matches the ~109ms
					 * reset-loop cadence seen in every prior test), NOT a blind sleep */
					us = wait_edge_us(200000);
					do_read(0x000000, 37, rx);
					t = hid_type(rx,37);
					pr_info("ugrt:   post-DESCREQ edge=%lldus type=%d(%s) raw=[%*ph]\n",
						us, t, tname(t), 37, rx);
					if(t==7){
						pr_info("ugrt:   *** DEVICE_DESC *** the 0xFED8120C dance WAS the missing piece!\n");
						ok = true;
						break;
					}
				}
			}
		} else {
			pr_info("ugrt:   wait_for_pin85 timed out (no edge in ~1000ms)\n");
		}

		master_clear_bit22();
		pr_info("ugrt:   master after CLEAR = 0x%08x\n", ioread32(gpio+MASTER_OFF));
		msleep(300);
	}

	if(!ok) pr_info("ugrt: VERDICT: all 3 attempts failed, same as every previous test (falsifies this hypothesis)\n");

	w8(OPCODE,0x0B);
	iounmap(gpio);
	iounmap(base);
	pr_info("ugrt: DONE\n");
	return -ENODEV;
}
module_init(ugrt_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Replay the exact decompiled UEFI ResetTouchController retry loop (0xFED8120C bit22)");
