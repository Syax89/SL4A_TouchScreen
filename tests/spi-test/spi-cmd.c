/*
 * spi-cmd.c — Interactive SPI command tester for Surface Laptop 4 AMD
 * Usage:
 *   echo "w 02 00 00 01 42 00 00 03 00 00" > /proc/spi-cmd   # write opcode 0x02
 *   echo "r 0B 00 00 00" > /proc/spi-cmd                       # read opcode 0x0B, addr 0
 *   cat /proc/spi-cmd                                            # show last response
 *   echo "raw 02:00 00 01 42 00 00 03 00 00" > /proc/spi-cmd    # raw: opcode+tx_data,rx_len
 *   echo "dump" > /proc/spi-cmd                                  # dump key registers
 *   echo "loop w 02 00 00 01 42 00 00 03 00 00" > /proc/spi-cmd # loop: send cmd repeatedly
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/io.h>

#define DRV_NAME "spi_cmd"

/* MMIO registers from spi-amd.c */
#define CTRL0		0x00
#define ALT_CS		0x1D
#define ENA_REG		0x20
#define OPCODE_REG	0x45
#define TRIGGER_REG	0x47
#define TX_COUNT_REG	0x48
#define RX_COUNT_REG	0x4B
#define STATUS_REG	0x4C
#define SPEED_REG	0x6C
#define FIFO_BASE	0x80
#define FIFO_SIZE	70

#define SECRET_BITS	0x60040000
#define FIFO_CLEAR	BIT(20)
#define TRIGGER_CMD	BIT(7)
#define BUSY		BIT(31)

/* ACPI MMIO from _CRS: AMDI0060 @ 0xFEC10000 */
#define FCH_MMIO_BASE	0xFEC10000
#define FCH_MMIO_SIZE	0x100
#define REG_PREFIX_OFF	0x22

struct spi_cmd_ctx {
	void __iomem *mmio;
	u8 rx_buf[512];
	int rx_len;
	char last_cmd[256];
	u16 reg_prefix;
};

static struct spi_cmd_ctx *ctx;

static inline u8 rd8(int off)  { return readb(ctx->mmio + off); }
static inline u32 rd32(int off) { return readl(ctx->mmio + off); }
static inline void wr8(int off, u8 v)  { writeb(v, ctx->mmio + off); }
static inline void wr32(int off, u32 v) { writel(v, ctx->mmio + off); }

static void dump_regs(struct seq_file *m)
{
	u8 fd[16];
	int i;
	for (i = 0; i < 16; i++) fd[i] = rd8(FIFO_BASE + i);

#define D(fmt, ...) do { if (m) seq_printf(m, fmt, ##__VA_ARGS__); else printk(KERN_INFO "spi-cmd: " fmt, ##__VA_ARGS__); } while(0)

	D("CTRL0=%08x ENA=%08x SPD=%08x\n",
	  rd32(CTRL0), rd32(ENA_REG), rd32(SPEED_REG));
	D("1D=%02x  44=%04x  45=%02x  47=%02x  48=%02x  4B=%02x  4C=%02x\n",
	  rd8(ALT_CS),
	  readw(ctx->mmio + 0x44),
	  rd8(OPCODE_REG), rd8(TRIGGER_REG),
	  rd8(TX_COUNT_REG), rd8(RX_COUNT_REG),
	  rd8(STATUS_REG));
	D("FIFO[0..15]=%*ph\n", 16, fd);
	D("reg_prefix@0x22=%04x\n", readw(ctx->mmio + REG_PREFIX_OFF));
#undef D
}

static void setup_v2(void)
{
	u32 ctrl0;

	/* secret bits */
	ctrl0 = rd32(CTRL0);
	ctrl0 |= SECRET_BITS;
	wr32(CTRL0, ctrl0);

	/* strobe */
	wr8(0x49, 0x00);
	wr8(0x4a, 0x00);

	/* CS = 1 — exact Windows decomp: (read & 0xFC) | 1 */
	{
		u8 v = rd8(ALT_CS);
		v = (v & 0xFC) | 1;
		wr8(ALT_CS, v);
		(void)rd8(ALT_CS); /* readback as barrier (Windows does this too) */
	}
}

static void clear_fifo(void)
{
	u32 v = rd32(CTRL0);
	v |= FIFO_CLEAR;
	wr32(CTRL0, v);
}

static int busy_wait(void)
{
	u32 v;
	int timeout = 100000;
	do {
		v = rd32(CTRL0);
		if (!(v & BUSY))
			return 0;
		cpu_relax();
	} while (--timeout);
	return -ETIMEDOUT;
}

static void trigger_v2(void)
{
	wr8(TRIGGER_REG, TRIGGER_CMD);
}

static int do_raw_xfer(u8 opcode, const u8 *tx_data, int tx_len,
		       u8 *rx_data, int rx_len, char *errmsg, int errsz)
{
	void __iomem *base = ctx->mmio;
	int i, ret;

	if (tx_len < 0 || rx_len < 0 || tx_len + rx_len > FIFO_SIZE) {
		snprintf(errmsg, errsz, "bad lengths tx=%d rx=%d (max=%d)",
			 tx_len, rx_len, FIFO_SIZE);
		return -EINVAL;
	}

	/*
	 * Match the VERIFIED spi-amd V2 semantics exactly:
	 *  - READ (opcode 0x0B): only the 3 address bytes are transmitted
	 *    (TX_COUNT=3); the device's reply (turnaround + header) lands in
	 *    the FIFO starting at FIFO_BASE + tx_count = FIFO_BASE + 3.
	 *  - WRITE (opcode 0x02) / other: the whole payload is transmitted
	 *    (TX_COUNT=tx_len); any reply lands at FIFO_BASE + tx_len.
	 * (The old code used TX_COUNT=tx_len and read from FIFO+4 for reads,
	 *  which does NOT match the working spi-amd read.)
	 */
	{
		int tx_count = (opcode == 0x0B) ? 3 : tx_len;
		int rx_off   = (opcode == 0x0B) ? 3 : tx_len;

		/* setup CTRL0 (secret bits, strobe, CS) */
		setup_v2();
		/* clear FIFO */
		clear_fifo();

		wr8(TX_COUNT_REG, tx_count);

		/* fill FIFO */
		for (i = 0; i < tx_len; i++)
			writeb(tx_data[i], base + FIFO_BASE + i);

		wr8(RX_COUNT_REG, rx_len);

		/* opcode (matches Windows: after TX_COUNT/RX_COUNT, with readback) */
		wr8(OPCODE_REG, opcode);
		(void)rd8(OPCODE_REG); /* readback as barrier */

		trigger_v2();
		ret = busy_wait();
		if (ret) {
			snprintf(errmsg, errsz, "busy wait timeout (CTRL0=%08x)", rd32(CTRL0));
			return ret;
		}

		if (rx_len > 0) {
			for (i = 0; i < rx_len && i < 512; i++)
				rx_data[i] = readb(base + FIFO_BASE + rx_off + i);
		}
	}

	return rx_len;
}

/* Proven V2 split: tx_count controls pure-TX zone; response at FIFO+4 */
static int do_raw_xfer_proven(u8 opcode, const u8 *tx_data, int tx_fifo_len,
			      int tx_count, int rx_count,
			      u8 *rx_data, char *errmsg, int errsz)
{
	void __iomem *base = ctx->mmio;
	int i, ret;

	if (tx_fifo_len > FIFO_SIZE || rx_count > FIFO_SIZE) {
		snprintf(errmsg, errsz, "bad lengths");
		return -EINVAL;
	}

	setup_v2();
	clear_fifo();

	wr8(TX_COUNT_REG, tx_count);

	for (i = 0; i < tx_fifo_len; i++)
		writeb(tx_data[i], base + FIFO_BASE + i);

	wr8(RX_COUNT_REG, rx_count);
	wr8(OPCODE_REG, opcode);

	trigger_v2();
	ret = busy_wait();
	if (ret) {
		snprintf(errmsg, errsz, "busy wait timeout");
		return ret;
	}

	/* Windows reads from FIFO+4, NOT FIFO+tx_len! */
	if (rx_count > 0)
		for (i = 0; i < rx_count && i < 512; i++)
			rx_data[i] = readb(base + FIFO_BASE + 4 + i);

	return rx_count;
}

/* Parse commands:
 * "w 02 00 00 01 ..."  — write: opcode + data bytes, no rx
 * "r 0B 00 00 00 FF"  — read: opcode + tx bytes (max 5 for approval + extra), rx_len=24
 * "raw 02:00 00 01 42 ..."  — raw: opcode:tx_data,rx_len
 * "raw 02:5:24 00 00 01 ..." — raw: opcode:tx_len:rx_len tx_bytes
 * "dump"               — dump registers
 * "loop w 02 ..."      — loop: send command every ~100ms (stop with "stop")
 */
static int parse_and_execute(const char *cmd, struct seq_file *m)
{
	char err[128] = {0};
	u8 tx[256], rx[512];
	int tx_len = 0, rx_len = 0;
	u8 opcode = 0;
	int i, ret;

#define P(fmt, ...) do { if (m) seq_printf(m, fmt, ##__VA_ARGS__); else printk(KERN_INFO "spi-cmd: " fmt, ##__VA_ARGS__); } while(0)

	if (strncmp(cmd, "dump", 4) == 0) {
		dump_regs(m);
		return 0;
	}

	/*
	 * "validate" — the trust check for this tool.  Does the verified
	 * spi-amd read (opcode 0x0B, addr 0x000000, TX_COUNT=3, RX@FIFO+3)
	 * and looks for the RESET_RESP header `32 10 00 5A` (sync 0x5A at
	 * off+3, type 3).  If this fails, raw MMIO is NOT reaching the device
	 * and any write experiment below is meaningless.
	 */
	if (strncmp(cmd, "validate", 8) == 0) {
		u8 addr[3] = { 0x00, 0x00, 0x00 };
		u8 rxb[24];
		char e[128] = {0};
		int off, found = 0, r;

		r = do_raw_xfer(0x0B, addr, 3, rxb, sizeof(rxb), e, sizeof(e));
		if (r < 0) { P("VALIDATE: xfer error: %s\n", e); return r; }

		P("VALIDATE read @0x000000: [%*ph]\n", 24, rxb);
		for (off = 0; off + 3 < (int)sizeof(rxb); off++) {
			if (rxb[off] == 0xFF || rxb[off] == 0x00)
				continue;
			if (rxb[off + 3] == 0x5A) {
				int type = (rxb[off] >> 4) & 0xF;

				P("VALIDATE: SYNC 0x5A at off=%d header=%02x %02x %02x %02x type=%d\n",
				  off, rxb[off], rxb[off+1], rxb[off+2], rxb[off+3], type);
				if (type == 3)
					P("VALIDATE: *** PASS *** raw MMIO reaches the device (RESET_RESP)\n");
				else
					P("VALIDATE: sync found but type=%d (expected 3=RESET_RESP)\n", type);
				found = 1;
				break;
			}
		}
		if (!found)
			P("VALIDATE: *** FAIL *** no 0x5A sync - raw MMIO NOT reaching the device\n");
		return 0;
	}

	if (strncmp(cmd, "sweep", 5) == 0) {
		u8 tx[8] = { 0x00, 0x10, 0x00, 0xFF, 0xAA, 0xAA, 0xAA, 0xAA };
		u8 rx[8];
		u32 orig_ctrl0 = rd32(CTRL0);
		int bit;

		P("CTRL0 bit sweep: orig=0x%08x\n", orig_ctrl0);
		for (bit = 0; bit < 32; bit++) {
			if (bit < 8 || bit == 16 || bit == 20 || bit == 31)
				continue;

			setup_v2();
			clear_fifo();
			{
				u32 c = rd32(CTRL0) ^ (1 << bit);
				wr32(CTRL0, c);
			}
			{
				u32 c = rd32(CTRL0);
				c = (c & ~0xFF) | 0x0b;
				wr32(CTRL0, c);
			}

			for (i = 0; i < 8; i++)
				writeb(tx[i], ctx->mmio + FIFO_BASE + i);
			wr8(TX_COUNT_REG, 4);
			wr8(RX_COUNT_REG, 8);
			wr8(OPCODE_REG, 0x0B);

			trigger_v2();
			if (busy_wait() == 0) {
				for (i = 0; i < 8; i++)
					rx[i] = readb(ctx->mmio + FIFO_BASE + 4 + i);
				if (rx[0] != 0xFF || rx[1] != 0xFF || rx[2] != 0xFF || rx[3] != 0xFF)
					P("CTRL0 bit %2d (0x%08x): RX=[%02x %02x %02x %02x %02x %02x %02x %02x] ***\n",
					   bit, 1 << bit, rx[0],rx[1],rx[2],rx[3],rx[4],rx[5],rx[6],rx[7]);
			}
			wr32(CTRL0, orig_ctrl0);
		}
		wr32(CTRL0, orig_ctrl0);
		P("CTRL0 bit sweep done\n");
		return 0;
	}

	if (strncmp(cmd, "ctrl", 4) == 0) {
		unsigned int set_bits = 0, clear_bits = 0;
		int n;
		/* ctrl set=0xNN clear=0xMM */
		sscanf(cmd, "ctrl set=%x clear=%x%n", &set_bits, &clear_bits, &n);
		if (n > 0) {
			u32 c = rd32(CTRL0);
			c |= set_bits;
			c &= ~clear_bits;
			wr32(CTRL0, c);
			P("CTRL0 now 0x%08x (set=0x%x clear=0x%x)\n", c, set_bits, clear_bits);
		} else {
			P("Usage: ctrl set=0xNN clear=0xMM\n");
		}
		return 0;
	}

	if (strncmp(cmd, "proven", 6) == 0) {
		/* Use proven V2 split from diagnostic module */
		const char *p = cmd + 7;
		int bytes_parsed;

		while (*p == ' ') p++;
		while (*p && tx_len < 250) {
			if (*p == ' ') { p++; continue; }
			if (sscanf(p, "%hhx%n", &tx[tx_len], &bytes_parsed) == 1) {
				tx_len++;
				p += bytes_parsed;
			} else break;
		}
		if (tx_len < 1) { P("ERR: need opcode\n"); return -EINVAL; }

		opcode = tx[0];
		memmove(tx, tx + 1, tx_len - 1);
		tx_len--;

		if (opcode == 0x0B) {
			/* Trusted header read: TX=7, RX=1 */
			ret = do_raw_xfer_proven(opcode, tx, tx_len, 7, 1, rx, err, sizeof(err));
			rx_len = 1;
		} else {
			/* Descriptor write: TX=4, RX=5 */
			ret = do_raw_xfer_proven(opcode, tx, tx_len, 4, 5, rx, err, sizeof(err));
			rx_len = 5;
		}
		if (ret < 0) { P("ERR: %s\n", err); return ret; }
		P("CMD proven: op=%02x tx_fifo=%d tx_count=%d rx_count=%d\n", opcode, tx_len,
		  opcode==0x0B?7:4, opcode==0x0B?1:5);
		dump_regs(m);
		if (rx_len > 0) {
			P("RX[%d]: ", rx_len);
			for (i = 0; i < min(rx_len, 64); i++) { if(m)seq_printf(m,"%02x ",rx[i]); else printk(KERN_CONT "%02x ", rx[i]); }
			if (m) seq_printf(m, "\n"); else printk(KERN_CONT "\n");
			for (i = 0; i + 3 < rx_len; i++) {
				if (rx[i+3] == 0x5A) { P("  SYNC at off=%d type=%d\n", i, (rx[i]>>4)&0xF); break; }
			}
			memcpy(ctx->rx_buf, rx, rx_len); ctx->rx_len = rx_len;
		}
		return 0;
	}

	if (strncmp(cmd, "loop ", 5) == 0) {
		P("loop not supported in this version\n");
		return 0;
	}

	if (strncmp(cmd, "stop", 4) == 0) {
		P("OK\n");
		return 0;
	}

	if (strncmp(cmd, "raw ", 4) == 0) {
		const char *p = cmd + 4;
		int rx_requested = 0;

		while (*p == ' ') p++;
		if (sscanf(p, "%hhx", &opcode) != 1) {
			P("ERR: bad opcode\n");
			return -EINVAL;
		}
		while (*p && *p != ' ' && *p != ':') p++;
		if (*p == ':') p++;
		while (*p == ' ') p++;

		while (*p && *p != ',' && tx_len < 250) {
			if (*p == ' ') { p++; continue; }
			if (*p == '\n' || *p == '\0') break;
			if (sscanf(p, "%hhx", &tx[tx_len]) == 1) {
				tx_len++;
				while (*p && *p != ' ' && *p != ',' && *p != '\n') p++;
			} else break;
		}
		if (*p == ',') p++;
		if (*p == ' ') p++;
		if (*p) sscanf(p, "%d", &rx_requested);

		rx_len = min(rx_requested, 200);
	} else if (cmd[0] == 'w' || cmd[0] == 'r') {
		const char *p = cmd + 1;
		u8 is_read = (cmd[0] == 'r');
		int bytes_parsed;

		while (*p == ' ') p++;

		while (*p && tx_len < 250) {
			if (*p == ' ' || *p == '\n') { p++; continue; }
			if (sscanf(p, "%hhx%n", &tx[tx_len], &bytes_parsed) == 1) {
				tx_len++;
				p += bytes_parsed;
			} else break;
		}

		if (tx_len < 1) {
			P("ERR: need at least opcode byte\n");
			return -EINVAL;
		}

		opcode = tx[0];
		memmove(tx, tx + 1, tx_len - 1);
		tx_len--;

		if (is_read) {
			rx_len = 24;
		} else {
			rx_len = 0;
		}
	} else {
		P("ERR: unknown command '%s'\n", cmd);
		P("Use: w/r <hex bytes> | raw <op:data,rx_len> | dump\n");
		return -EINVAL;
	}

	snprintf(ctx->last_cmd, sizeof(ctx->last_cmd), "op=%02x tx=%d rx=%d", opcode, tx_len, rx_len);

	ret = do_raw_xfer(opcode, tx, tx_len, rx, rx_len, err, sizeof(err));
	if (ret < 0) {
		P("ERR: %s\n", err);
		return ret;
	}

	P("CMD: op=%02x tx=%d rx=%d\n", opcode, tx_len, rx_len);
	dump_regs(m);

	if (rx_len > 0) {
		P("RX[%d]: ", rx_len);
		for (i = 0; i < min(rx_len, 64); i++) {
			if (m)
				seq_printf(m, "%02x ", rx[i]);
			else
				printk(KERN_CONT "%02x ", rx[i]);
		}
		if (m) seq_printf(m, "\n"); else printk(KERN_CONT "\n");

		for (i = 0; i + 3 < rx_len; i++) {
			if (rx[i] == 0xFF || rx[i] == 0x00)
				continue;
			if (rx[i + 3] == 0x5A) {
				int rtype = (rx[i] >> 4) & 0xF;
				P("  SYNC at off=%d type=%d (header: %02x %02x %02x %02x)\n",
				   i, rtype, rx[i], rx[i+1], rx[i+2], rx[i+3]);
				break;
			}
		}

		memcpy(ctx->rx_buf, rx, rx_len);
		ctx->rx_len = rx_len;
	}

	return 0;
#undef P
}

/* --- /proc interface --- */

static ssize_t spi_cmd_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	char *kbuf;

	if (count > 2048)
		return -EINVAL;

	kbuf = kmalloc(count + 1, GFP_KERNEL);
	if (!kbuf)
		return -ENOMEM;

	if (copy_from_user(kbuf, buf, count)) {
		kfree(kbuf);
		return -EFAULT;
	}
	kbuf[count] = '\0';

	/* strip trailing newline */
	if (kbuf[count - 1] == '\n')
		kbuf[count - 1] = '\0';

	parse_and_execute(kbuf, NULL);

	kfree(kbuf);
	return count;
}

static int spi_cmd_show(struct seq_file *m, void *v)
{
	dump_regs(m);
	if (ctx->rx_len > 0) {
		int i;
		seq_printf(m, "\nLast command: %s\n", ctx->last_cmd);
		seq_printf(m, "Last RX[%d]: ", ctx->rx_len);
		for (i = 0; i < min(ctx->rx_len, 64); i++)
			seq_printf(m, "%02x ", ctx->rx_buf[i]);
		seq_printf(m, "\n");
		/* Find sync */
		for (i = 0; i + 3 < ctx->rx_len; i++) {
			if (ctx->rx_buf[i] == 0xFF || ctx->rx_buf[i] == 0x00)
				continue;
			if (ctx->rx_buf[i + 3] == 0x5A) {
				seq_printf(m, "  SYNC at off=%d type=%d\n",
					   i, (ctx->rx_buf[i] >> 4) & 0xF);
			}
		}
	}
	return 0;
}

static int spi_cmd_open(struct inode *inode, struct file *file)
{
	return single_open(file, spi_cmd_show, NULL);
}

static const struct proc_ops spi_cmd_ops = {
	.proc_open	= spi_cmd_open,
	.proc_read	= seq_read,
	.proc_write	= spi_cmd_write,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init spi_cmd_init(void)
{
	struct proc_dir_entry *entry;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->mmio = ioremap(FCH_MMIO_BASE, FCH_MMIO_SIZE);
	if (!ctx->mmio) {
		pr_err("spi-cmd: ioremap failed\n");
		kfree(ctx);
		return -ENOMEM;
	}

	ctx->reg_prefix = readw(ctx->mmio + REG_PREFIX_OFF);
	pr_info("spi-cmd: MMIO=%p reg_prefix=%04x CTRL0=%08x\n",
		ctx->mmio, ctx->reg_prefix, rd32(CTRL0));

	entry = proc_create("spi-cmd", 0666, NULL, &spi_cmd_ops);
	if (!entry) {
		iounmap(ctx->mmio);
		kfree(ctx);
		return -ENOMEM;
	}

	pr_info("spi-cmd: /proc/spi-cmd ready\n");
	return 0;
}

static void __exit spi_cmd_exit(void)
{
	remove_proc_entry("spi-cmd", NULL);
	if (ctx) {
		iounmap(ctx->mmio);
		kfree(ctx);
		ctx = NULL;
	}
	pr_info("spi-cmd: unloaded\n");
}

module_init(spi_cmd_init);
module_exit(spi_cmd_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Interactive SPI command tester");
