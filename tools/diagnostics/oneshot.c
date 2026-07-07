/* ONE-SHOT test: run on pristine device after clean boot.
 * Detects FIFO mode, executes full Windows init sequence.
 *
 * insmod oneshot.ko && dmesg | grep "oneshot:"
 */
#include <linux/module.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/iopoll.h>
#include <linux/pci.h>

#define FCH_BASE 0xFEC10000
static void __iomem *base;

#define w32(o,v) iowrite32(v, base+(o))
#define r32(o)   ioread32(base+(o))
#define w16(o,v) iowrite16(v, base+(o))
#define r16(o)   ioread16(base+(o))
#define w8(o,v)  iowrite8(v, base+(o))
#define r8(o)    ioread8(base+(o))

static int fifo_16bit; /* detected at init: 0=8bit, 1=16bit */

/* Read RX from FIFO at given offset */
static void fifo_rx(int off, u8 *dst, int n)
{
	int i;
	if (fifo_16bit) {
		for (i = 0; i < n; i++) {
			u16 w = r16(off + (i/2)*2);
			dst[i] = (i & 1) ? (u8)(w >> 8) : (u8)(w & 0xFF);
		}
	} else {
		for (i = 0; i < n; i++)
			dst[i] = r8(off + i);
	}
}

static void fifo_tx(int off, const u8 *d, int n) { int i; for(i=0;i<n;i++) w8(off+i,d[i]); }

#define CTRL0 0x00
#define OP45  0x45
#define TR47  0x47
#define TX48  0x48
#define RX4B  0x4B
#define FIFO  0x80
#define CS1D  0x1D
#define R22   0x22
#define R44   0x44

static void cs1(void) { w8(CS1D, (r8(CS1D) & 0xFC) | 0x01); }

static int wbusy(void) { u32 v; return readl_poll_timeout(base+CTRL0, v, !(v & BIT(31)), 20, 2000000); }

static void clr_fifo(void) { u32 c=r32(CTRL0); w32(CTRL0,c&~BIT(20)); w32(CTRL0,c|BIT(20)); }
static void secbits(void) { w32(CTRL0, r32(CTRL0) | 0x60040000); }

/* Read N bytes from register 0x000000 */
static int read_reg(int n, u8 *rx)
{
	clr_fifo(); w8(OP45,0x0B); secbits();
	w8(TX48,3); fifo_tx(FIFO, (u8[]){0,0,0},3); w8(RX4B,n);
	secbits(); w8(TR47,0x80);
	if (wbusy()) return -1;
	fifo_rx(FIFO+3, rx, n);
	w8(OP45,0x0B); return 0;
}

/* Write TX-only: v2 puts opcode in 0x45, TX_COUNT excludes opcode.
 * But: does the controller include the opcode byte in TX_COUNT?
 * Test BOTH modes: v2 (opcode in 0x45, txlen=9) and v1-style (opcode in FIFO, txlen=10) */
static int write02_both(const u8 *pl, int txlen)
{
	u8 v1pl[10];
	int ret_v2, ret_v1;

	/* V2: opcode in 0x45, TX_COUNT=txlen */
	pr_info("oneshot: DESCREQ V2: opcode in 0x45, TX_COUNT=%d...\n", txlen);
	clr_fifo(); w8(OP45,0x02); secbits();
	{ u16 r44=r16(R44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(R44,r44); w8(OP45,0x02); }
	w8(TX48,txlen); fifo_tx(FIFO,pl,txlen); w8(RX4B,0);
	secbits(); w8(OP45,0x02);
	pr_info("oneshot:   V2 pre-trig c0=0x%08x c1=0x%08x op45=0x%02x tx48=%u\n",
		r32(CTRL0), r32(0x0C), r8(OP45), r8(TX48));
	w8(TR47,0x80); ret_v2 = wbusy();
	w8(OP45,0x0B);
	if (ret_v2) pr_info("oneshot:   V2 write BUSY TIMEOUT\n");

	/* V1-style: opcode in FIFO byte 0, TX_COUNT=txlen+1,
	 * opcode in 0x45 also set for compatibility */
	v1pl[0] = 0x02;
	memcpy(v1pl+1, pl, txlen);
	pr_info("oneshot: DESCREQ V1: opcode in FIFO, TX_COUNT=%d...\n", txlen+1);
	clr_fifo(); w8(OP45,0x02); secbits();
	{ u16 r44=r16(R44); u8 nib=(r32(0x20)>>20)&0xF;
	  r44=(r44&0xF0FF)|((u16)nib<<8); r44=(r44&0x0FFF)|((u16)nib<<12);
	  w16(R44,r44); w8(OP45,0x02); }
	w8(TX48,txlen+1); fifo_tx(FIFO,v1pl,txlen+1); w8(RX4B,0);
	secbits(); w8(OP45,0x02);
	pr_info("oneshot:   V1 pre-trig c0=0x%08x c1=0x%08x op45=0x%02x tx48=%u\n",
		r32(CTRL0), r32(0x0C), r8(OP45), r8(TX48));
	w8(TR47,0x80); ret_v1 = wbusy();
	w8(OP45,0x0B);
	if (ret_v1) pr_info("oneshot:   V1 write BUSY TIMEOUT\n");

	return ret_v2 && ret_v1 ? -1 : 0;
}

/* Find and decode HID header in byte array */
static int find_hid(const u8 *d, int n, const char *ctx)
{
	int i;
	for (i = 3; i < n; i++) {
		if (d[i] != 0x5A) continue;
		if ((d[i-3] & 0x0F) != 2) continue;
		if (d[i-2] & 0x0F) continue; /* low nibble must be 0 */
		{
			int t = (d[i-3] >> 4) & 0xF;
			u16 bl = (((d[i-2] >> 4) & 0xF) | (d[i-1] << 4)) * 4;
			const char *tn = t==1?"DATA":t==3?"RESET_RSP":t==4?"CMD_RSP":
					 t==5?"GET_FEAT":t==7?"DEVICE_DESC":t==8?"RPT_DESC":"?";
			pr_info("oneshot: *** HID %s: type=%d(%s) ver=%d body_len=%u hdr=[%02x %02x %02x %02x]\n",
				ctx, t, tn, (int)(d[i-3]&0xF), bl,
				d[i-3], d[i-2], d[i-1], d[i]);
			return t;
		}
	}
	return -1;
}

/* Check PCI 0xB8 bit7 to detect 16-bit FIFO mode */
static int detect_fifo_mode(void)
{
	struct pci_dev *lpc;
	int val;
	lpc = pci_get_device(PCI_VENDOR_ID_AMD, 0x790E, NULL);
	if (!lpc) {
		pr_warn("oneshot: LPC bridge 1022:790E not found, assuming 8-bit\n");
		return 0;
	}
	pci_read_config_dword(lpc, 0xB8, &val);
	pci_dev_put(lpc);
	pr_info("oneshot: PCI 0xB8 = 0x%08x -> %d-bit FIFO\n", val, (val & 0x80) ? 16 : 8);
	return (val & 0x80) ? 1 : 0;
}

/* Dump full FIFO + show register state */
static void dump_all(void)
{
	u8 f[70];
	fifo_rx(FIFO, f, 70);
	pr_info("oneshot: FIFO[0..69]=[%*ph]\n", 70, f);
	pr_info("oneshot: REGS c0=0x%08x c1=0x%08x cs=0x%02x op45=0x%02x\n",
		r32(CTRL0), r32(0x0C), r8(CS1D), r8(OP45));
}

static int __init oneshot_init(void)
{
	u8 rx[128];
	int got_type, i;
	const u8 descreq[] = {0x00,0x00,0x01, 0x42,0x00, 0x00,0x03,0x00,0x00};

	base = ioremap(FCH_BASE, 0x100);
	if (!base) return -ENOMEM;

	fifo_16bit = detect_fifo_mode();
	cs1();

	pr_info("oneshot: ===========================================================\n");
	pr_info("oneshot: ONE-SHOT INIT — FIFO mode=%d-bit — %s\n",
		fifo_16bit ? 16 : 8, fifo_16bit ? "16bit" : "8bit");
	pr_info("oneshot: ===========================================================\n");

	dump_all();

	/* Phase 1: Read RESET_RSP header + body in one 32-byte read */
	pr_info("oneshot: [Phase 1] Waiting for RESET_RSP (32B read)...\n");
	got_type = -1;
	for (i = 0; i < 24; i++) {
		msleep(50);
		memset(rx, 0xCC, sizeof(rx));
		if (read_reg(32, rx) < 0) { pr_info("oneshot:   read busy\n"); continue; }
		got_type = find_hid(rx, 32, "RESET_RSP");
		if (got_type == 3) {
			pr_info("oneshot:   got RESET_RSP! raw(32B)=[%*ph]\n", 32, rx);
			/* Body immediately follows header in the same buffer.
			 * The header is 4 bytes, body_len is in the header.
			 * We just need to note the position for the state machine. */
			break;
		}
		if (i == 0 || i == 6 || i == 12 || i == 18)
			pr_info("oneshot:   attempt %d raw=[%*ph]\n", i, 12, rx);
	}

	if (got_type != 3) {
		pr_info("oneshot: *** NO RESET_RSP. Sending blind DESCREQ anyway...\n");
		write02_both(descreq, sizeof(descreq));
		goto descreq_blind;
	}

	/* Phase 2: Body was already captured in the 32B read above. Skip drain.
	 * Windows does a second 0x0B read for the body but it's the same report.
	 * We already have it — just proceed to DESCREQ. */
	pr_info("oneshot: [Phase 2] RESET_RSP confirmed, proceeding to DESCREQ...\n");

	/* Phase 3: Send DESCREQ */
	pr_info("oneshot: [Phase 3] Sending DESCREQ @0x000001...\n");
	write02_both(descreq, sizeof(descreq));

	/* Phase 4: Wait for GPIO IRQ + read response. */
	pr_info("oneshot: [Phase 4] Waiting for DEVICE_DESC response...\n");
	got_type = -1;
	for (i = 0; i < 10; i++) {
		msleep(20);
		memset(rx, 0xCC, sizeof(rx));
		read_reg(20, rx);
		got_type = find_hid(rx, 20, "read_after_descreq");
		if (got_type > 0) {
			pr_info("oneshot:   raw=[%*ph]\n", 20, rx);
			break;
		}
	}

	if (got_type == 7) {
		/* DEVICE_DESC received! Read full descriptor */
		pr_info("oneshot: *** DEVICE_DESC FOUND! Reading full descriptor...\n");
		memset(rx, 0xCC, sizeof(rx));
		read_reg(37, rx);
		pr_info("oneshot:   raw=[%*ph]\n", 37, rx);
		find_hid(rx, 37, "desc_body");

		/* Parse descriptor (28 bytes starting at body+3 offset) */
		{
			int sync_off = 0;
			while (sync_off < 33 && rx[sync_off] == 0xFF) sync_off++;
			/* Skip body prefix (3 bytes: len16 LE + ContentID) */
			int desc_off = sync_off + 6; /* +3 body prefix + at least 3 bytes */
			if (desc_off + 28 <= 37) {
				u16 vid = rx[desc_off+17] | (rx[desc_off+18] << 8);
				u16 pid = rx[desc_off+19] | (rx[desc_off+20] << 8);
				u16 ver = rx[desc_off+21] | (rx[desc_off+22] << 8);
				pr_info("oneshot:   DEVICE: vid=0x%04X pid=0x%04X ver=0x%04X\n", vid, pid, ver);
			}
		}

		/* Phase 5: Send DESCREQ2, get RPT_DESC */
		{
			u8 dr2[] = {0x00,0x00,0x02, 0x42,0x00, 0x00,0x03,0x00,0x00};
			pr_info("oneshot: [Phase 5] Sending DESCREQ2 @0x000002...\n");
			write02_both(dr2, sizeof(dr2));
			pr_info("oneshot: [Phase 5b] Reading RPT_DESC...\n");
			for (i = 0; i < 10; i++) {
				msleep(20);
				memset(rx, 0xCC, sizeof(rx));
				read_reg(9, rx);
				got_type = find_hid(rx, 9, "rpt_desc_hdr");
				if (got_type == 8) {
					pr_info("oneshot:   raw=[%*ph]\n", 9, rx);
					/* Read body */
					msleep(5);
					memset(rx, 0xCC, sizeof(rx));
					read_reg(128, rx);
					pr_info("oneshot:   RPT_DESC body[0..127]=[%*ph]\n",
						min(128, 128), rx);
					find_hid(rx, 128, "rpt_body");
					break;
				}
			}
		}
	} else {
		pr_info("oneshot: *** No DEVICE_DESC (type 7) received after DESCREQ\n");
	}

descreq_blind:
	dump_all();

	w8(OP45, 0x0B);
	iounmap(base);
	pr_info("oneshot: ===========================================================\n");
	pr_info("oneshot: TEST COMPLETE\n");
	pr_info("oneshot: ===========================================================\n");
	return -ENODEV;
}

module_init(oneshot_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SL4A Touchscreen One-Shot Init Test");
