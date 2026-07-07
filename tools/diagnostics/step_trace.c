/* INSTRUMENTED Windows sequence trace.
 * Executes the EXACT Windows write sequence step-by-step,
 * logging every register read/write with before/after values.
 * 
 * insmod step_trace.ko && dmesg | grep "STP:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>

#define BASE 0xFEC10000
static void __iomem *io;

#define R8(o) r8(o); 
#define LOG_RD8(o,lbl) do { u8 _v=r8(o); pr_info("STP: %-20s RD8  +0x%02X = 0x%02X\n", lbl, o, _v); } while(0)
#define LOG_WR8(o,v,lbl) do { pr_info("STP: %-20s WR8  +0x%02X = 0x%02X\n", lbl, o, v); w8(o,v); } while(0)
#define LOG_RD16(o,lbl) do { u16 _v=r16(o); pr_info("STP: %-20s RD16 +0x%02X = 0x%04X\n", lbl, o, _v); } while(0)
#define LOG_WR16(o,v,lbl) do { pr_info("STP: %-20s WR16 +0x%02X = 0x%04X\n", lbl, o, v); w16(o,v); } while(0)
#define LOG_RD32(o,lbl) do { u32 _v=r32(o); pr_info("STP: %-20s RD32 +0x%02X = 0x%08X\n", lbl, o, _v); } while(0)
#define LOG_WR32(o,v,lbl) do { pr_info("STP: %-20s WR32 +0x%02X = 0x%08X\n", lbl, o, v); w32(o,v); } while(0)

static inline u8  r8(int o) { return ioread8(io+o); }
static inline void w8(int o, u8 v) { iowrite8(v, io+o); }
static inline u16 r16(int o) { return ioread16(io+o); }
static inline void w16(int o, u16 v) { iowrite16(v, io+o); }
static inline u32 r32(int o) { return ioread32(io+o); }
static inline void w32(int o, u32 v) { iowrite32(v, io+o); }

static int wbusy(void) { u32 v; return readl_poll_timeout(io, v, !(v & BIT(31)), 20, 2000000); }

static void ftx8(int o, const u8 *d, int n) { int i; for(i=0;i<n;i++) w8(o+i,d[i]); }

static int __init stp_init(void)
{
	u16 saved_22, r44;
	u8 nibble;
	u32 ctrl0;
	u8 descreq[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};
	int i;

	io = ioremap(BASE, 0x100);
	if (!io) return -ENOMEM;

	pr_info("STP: ===============================================================\n");
	pr_info("STP: STEP-BY-STEP WINDOWS SEQUENCE — Every MMIO access traced\n");
	pr_info("STP: ===============================================================\n");

	/* --- STEP 0: Initial state --- */
	pr_info("STP: --- STEP 0: Initial state ---\n");
	LOG_RD32(0x00, "CTRL0");
	LOG_RD32(0x0C, "CTRL1");
	LOG_RD8(0x1D, "ALT_CS");
	LOG_RD32(0x20, "ENA");
	LOG_RD16(0x22, "R22(SPD_CFG)");
	LOG_RD16(0x44, "R44(SPEED)");
	LOG_RD8(0x45, "OPCODE");
	LOG_RD8(0x47, "TRIGGER");
	LOG_RD8(0x48, "TX_COUNT");
	LOG_RD8(0x4B, "RX_COUNT");
	LOG_RD32(0x4C, "STATUS");
	LOG_RD16(0xFC, "MISC_CTRL");

	/* --- Windows sequence from decomp fcn.0x4bac --- */

	/* 1. save 0x22 */
	pr_info("STP:\nSTP: --- Windows fcn.0x4bac sequence ---\n");
	saved_22 = r16(0x22);
	LOG_RD16(0x22, "1.save_R22");

	/* 2. ALT_CS: read8(0x1D), &0xFC, |0x01, write8 */
	{
		u8 cs = r8(0x1D);
		LOG_RD8(0x1D, "2a.rd_ALT_CS");
		cs = (cs & 0xFC) | 0x01;
		w8(0x1D, cs);
		LOG_WR8(0x1D, cs, "2b.wr_ALT_CS");
	}

	/* 3. FIFO clear: single set (Windows style) */
	ctrl0 = r32(0x00);
	LOG_RD32(0x00, "3a.rd_CTRL0");
	ctrl0 |= BIT(20);
	w32(0x00, ctrl0);
	LOG_WR32(0x00, ctrl0, "3b.wr_CTRL0(FIFO_CLR)");

	/* 4. opcode #1 to 0x45 */
	w8(0x45, 0x02);
	LOG_WR8(0x45, 0x02, "4.wr_OPCODE");

	/* 5. READ_MODE: ctrl0 |= 0x60040000 */
	ctrl0 = r32(0x00);
	LOG_RD32(0x00, "5a.rd_CTRL0");
	ctrl0 |= 0x60040000;
	w32(0x00, ctrl0);
	LOG_WR32(0x00, ctrl0, "5b.wr_CTRL0(FAST_RD)");

	/* 6-7. 0x44 dance (for writes: opcode != 0x0B) */
	r44 = r16(0x44);
	LOG_RD16(0x44, "6.rd_R44");
	nibble = (r32(0x20) >> 20) & 0xF;
	LOG_RD32(0x20, "6b.rd_ENA(nib)");
	pr_info("STP:                  nibble=0x%X\n", nibble);
	r44 = (r44 & 0xF0FF) | ((u16)nibble << 8);
	r44 = (r44 & 0x0FFF) | ((u16)nibble << 12);
	w16(0x44, r44);
	LOG_WR16(0x44, r44, "7.wr_R44(dance)");

	/* Windows re-writes opcode after 0x44 clobber */
	w8(0x45, 0x02);
	LOG_WR8(0x45, 0x02, "7b.wr_OPCODE(re)");

	/* 8. TX_COUNT */
	w8(0x48, sizeof(descreq));
	LOG_WR8(0x48, sizeof(descreq), "8.wr_TX_COUNT");

	/* 9. Fill FIFO */
	LOG_RD8(0x80, "FIFO[0] pre");
	ftx8(0x80, descreq, sizeof(descreq));
	{
		u8 f[9];
		for(i=0;i<9;i++) f[i]=r8(0x80+i);
		pr_info("STP: 9.fill_FIFO       FIFO[0..8]=[%*ph]\n", 9, f);
	}
	LOG_RD8(0x80, "FIFO[0] post");

	/* 10. RX_COUNT (0 for TX-only write) */
	w8(0x4B, 0);
	LOG_WR8(0x4B, 0, "10.wr_RX_COUNT");

	/* 11. Re-write opcode after RX_COUNT */
	w8(0x45, 0x02);
	LOG_WR8(0x45, 0x02, "11.wr_OPCODE(final)");

	/* 12. Re-apply secret bits */
	ctrl0 = r32(0x00);
	LOG_RD32(0x00, "12a.rd_CTRL0");
	ctrl0 |= 0x60040000;
	w32(0x00, ctrl0);
	LOG_WR32(0x00, ctrl0, "12b.wr_CTRL0(re-secret)");

	wmb();

	/* --- PRE-TRIGGER STATE --- */
	pr_info("STP:\nSTP: --- PRE-TRIGGER ---\n");
	LOG_RD32(0x00, "CTRL0");
	LOG_RD32(0x0C, "CTRL1");
	LOG_RD8(0x1D,  "ALT_CS");
	LOG_RD16(0x44, "R44");
	LOG_RD8(0x45,  "OPCODE");
	LOG_RD8(0x47,  "TRIGGER");
	LOG_RD8(0x48,  "TX_COUNT");
	LOG_RD8(0x4B,  "RX_COUNT");
	LOG_RD32(0x4C, "STATUS");

	/* 13. Trigger */
	w8(0x47, 0x80);
	LOG_WR8(0x47, 0x80, "13.wr_TRIGGER");

	/* 14. Busy poll */
	{
		int ret = wbusy();
		pr_info("STP: 14.busy_poll       result=%d\n", ret);
	}
	LOG_RD32(0x00, "CTRL0(post-busy)");

	/* 15. Restore 0x22 */
	w16(0x22, saved_22);
	LOG_WR16(0x22, saved_22, "15.wr_R22(restore)");

	/* 16. Restore opcode */
	w8(0x45, 0x0B);
	LOG_WR8(0x45, 0x0B, "16.wr_OPCODE(restore)");

	/* 17. ALT_CS exit */
	{
		u8 cs = r8(0x1D);
		cs &= 0xFC;
		w8(0x1D, cs);
		LOG_RD8(0x1D, "17a.rd_ALT_CS");
		LOG_WR8(0x1D, cs, "17b.wr_ALT_CS(clear)");
	}

	/* --- POST-TRANSFER STATE --- */
	pr_info("STP:\nSTP: --- POST-TRANSFER ---\n");
	LOG_RD32(0x00, "CTRL0");
	LOG_RD32(0x0C, "CTRL1");
	LOG_RD8(0x1D,  "ALT_CS");
	LOG_RD8(0x45,  "OPCODE");
	LOG_RD8(0x48,  "TX_COUNT");
	LOG_RD8(0x4B,  "RX_COUNT");
	LOG_RD32(0x4C, "STATUS");

	/* Dump FIFO */
	{
		u8 f[70];
		for(i=0;i<70;i++) f[i]=r8(0x80+i);
		pr_info("STP: FIFO full [0..69]=[%*ph]\n", 70, f);
		for(i=3;i<70;i++) {
			if(f[i]==0x5A && (f[i-3]&0x0F)==2 && (f[i-2]&0x0F)==0) {
				int t=(f[i-3]>>4)&0xF;
				pr_info("STP:   FIFO HDR @%d: type=%d\n", i-3, t);
			}
		}
	}

	iounmap(io);
	pr_info("STP: DONE — Compare with Windows decomp\n");
	return -ENODEV;
}
module_init(stp_init);
MODULE_LICENSE("GPL");
