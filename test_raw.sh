#!/bin/bash
# ============================================================================
# SL4A Touchscreen — Raw MMIO Test Suite (no driver)
# Tests TX_COUNT, TXMODE, V1/V2, READ_MODE variations
# ============================================================================
set -e
TMPDIR="/tmp/spi-raw-$$"
DRIVER_DIR="$(cd "$(dirname "$0")" && pwd)/driver"
cleanup() {
    sudo rmmod spi_hid spi_amd rawtest 2>/dev/null || true
    rm -rf "$TMPDIR"
}
trap cleanup EXIT
mkdir -p "$TMPDIR"

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'
fail() { echo -e "${RED}FAIL: $1${NC}"; exit 1; }
pass() { echo -e "${GREEN}OK: $1${NC}"; }
info() { echo -e "${YELLOW}$1${NC}"; }

echo "============================================"
echo " SL4A Raw MMIO Test Suite"
echo " $(date)"
echo "============================================"
echo ""

# 1. Cleanup
info "1. Cleanup stale modules..."
for m in spi_hid spi_amd rawtest; do
    lsmod | grep -q "^$m " && sudo rmmod "$m" 2>/dev/null && echo "   unloaded $m"
done
pass "Clean"

# 2. ACPI power cycle
info "2. Power-cycling touchscreen..."
cat > "$TMPDIR/pwr.c" << 'EOF'
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/delay.h>
static void m010(u64 a, u64 v) {
    acpi_handle h; union acpi_object args[2]; struct acpi_object_list al;
    if (ACPI_FAILURE(acpi_get_handle(NULL,"\\M010",&h))) { pr_err("pwr: M010 not found\n"); return; }
    args[0].type=ACPI_TYPE_INTEGER; args[0].integer.value=a;
    args[1].type=ACPI_TYPE_INTEGER; args[1].integer.value=v;
    al.count=2; al.pointer=args;
    acpi_evaluate_object(h,NULL,&al,NULL);
}
static int __init p_init(void) {
    pr_info("pwr: OFF\n"); m010(0x5B,0); msleep(200);
    pr_info("pwr: RST\n"); m010(0x0103,0); msleep(100);
    pr_info("pwr: ON\n");  m010(0x5B,1); msleep(100);
    pr_info("pwr: DE-RST\n"); m010(0x0103,1);
    msleep(5000);
    pr_info("pwr: DONE (waited 5s)\n");
    return -EIO;
}
module_init(p_init); MODULE_LICENSE("GPL");
EOF
echo 'obj-m += pwr.o' > "$TMPDIR/Makefile"
make -C "/lib/modules/$(uname -r)/build" M="$TMPDIR" LLVM=1 modules 2>&1 | grep -E "error:" || true
sudo insmod "$TMPDIR/pwr.ko" 2>/dev/null || true
sleep 2
sudo dmesg | grep "pwr:" | tail -5
pass "Power-cycled"

# 3. PCI config
info "3. Setting PCI config..."
sudo setpci -s 00:14.3 B8.L=0x33ed0084 B4.L=0x7dffe000
B8=$(sudo setpci -s 00:14.3 B8.L)
echo "   B8=$B8 B4=$(sudo setpci -s 00:14.3 B4.L)"
pass "PCI configured"

# 4. Build and run raw test module
info "4. Running raw MMIO test suite..."
cat > "$TMPDIR/rawtest.c" << 'CEOF'
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define MMIO    0xFEC10000
#define C0      0x00
#define C1      0x0C
#define ACS     0x1D
#define SP44    0x44
#define OP45    0x45
#define TR47    0x47
#define TX48    0x48
#define RX4B    0x4B
#define FIFO    0x80

#define BIT(n)       (1UL<<(n))
#define EXEC         BIT(16)
#define CLR          BIT(20)
#define BSY          BIT(31)
#define TXM          BIT(23)
#define SEC          (BIT(30)|BIT(29)|BIT(18))

static void __iomem *b;
static u32 r32(int o) { return readl(b+o); }
static void w32(int o, u32 v) { writel(v,b+o); }
static u8 r8(int o) { return readb(b+o); }
static void w8(int o, u8 v) { writeb(v,b+o); }
static u16 r16(int o) { return readw(b+o); }
static void w16(int o, u16 v) { writew(v,b+o); }

static int wait_busy(int ms) {
    int i;
    for (i=0; i<ms*10; i++) { if (!(r32(C0)&BSY)) return 0; udelay(100); }
    return -1;
}

/* Raw V2 write: opcode in 0x45, trigger via 0x47 */
static int v2_write(const u8 *fifo, u8 tx, u8 rx, int clr_txmode) {
    int i; u32 c;
    w8(ACS, (r8(ACS)&0xFC)|0x01);
    c=r32(C0); c|=CLR; w32(C0,c);
    w8(OP45,0x02);
    c=r32(C0); if(clr_txmode) c&=~TXM; c|=SEC; w32(C0,c);
    { u8 n=r32(0x20)&0xF; u16 s=r16(SP44); s=(s&0xF0FF)|(n<<8); s=(s&0x0FFF)|(n<<12); w16(SP44,s); w8(OP45,0x02); }
    w8(TX48,tx);
    for(i=0;i<tx&&i<70;i++) w8(FIFO+i,fifo[i]);
    w8(RX4B,rx);
    w8(OP45,0x02);
    w8(TR47,0x80);
    return wait_busy(2000);
}

/* Raw V2 read approval: opcode 0x0B in 0x45, trigger 0x47 */
static int v2_read(u8 *rx, u8 rxlen) {
    int i; u32 c;
    w8(ACS, (r8(ACS)&0xFC)|0x01);
    c=r32(C0); c|=CLR; w32(C0,c);
    w8(OP45,0x0B);
    c=r32(C0); c|=SEC; w32(C0,c);
    { u8 n=r32(0x20)&0xF; u16 s=r16(SP44); s=(s&0xF0FF)|(n<<8); s=(s&0x0FFF)|(n<<12); w16(SP44,s); w8(OP45,0x0B); }
    w8(TX48,5);
    w8(FIFO+0,0x0B); w8(FIFO+1,0); w8(FIFO+2,0); w8(FIFO+3,0); w8(FIFO+4,0xFF);
    w8(RX4B,rxlen);
    w8(OP45,0x0B);
    w8(TR47,0x80);
    int ret=wait_busy(2000);
    for(i=0;i<rxlen;i++) { u16 w=r16(FIFO+5+(i/2)*2); rx[i]=(i&1)?(u8)(w>>8):(u8)(w&0xFF); }
    return ret;
}

/* check if a buffer contains DEVICE_DESC header */
static int got_desc(const u8 *rx, int len) {
    int i;
    for(i=0;i<len-3;i++)
        if(rx[i]==0x72 && rx[i+1]==0x80 && rx[i+2]==0x00 && rx[i+3]==0x5A) return 1;
    return 0;
}
/* check if a buffer contains RESET_RSP header */
static int got_rst(const u8 *rx, int len) {
    int i;
    for(i=0;i<len-3;i++)
        if(rx[i]==0x32 && rx[i+1]==0x10 && rx[i+2]==0x00 && rx[i+3]==0x5A) return 1;
    return 0;
}

static void drain(u8 *hdr, u8 *body) {
    u8 r[9];
    v2_read(r,9);
    memcpy(hdr,r,9);
    msleep(10);
    v2_read(r,9);
    memcpy(body,r,9);
}

struct result { int test; const char *desc; int tx; int rx; int txml; int got_d; int got_r; u32 c0; u32 c1; u8 hdr[9]; u8 body[9]; u8 resp[37]; int resp_len; };

static int __init rt_init(void)
{
    int i;
    u8 descreq9[9] = {0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00};
    u8 descreq14[14]= {0x00,0x00,0x01,0x42,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    u8 r[64];
    struct result res[8];
    int n=0;

    b=ioremap(MMIO,0x200); if(!b) return -ENOMEM;

    pr_info("rt: INIT c0=0x%08x c1=0x%08x\n", r32(C0), r32(C1));

    /* ---- TEST 1: Baseline V2 TX=9 RX=0 TXMODE=1 ---- */
    pr_info("rt: === T1: V2 TX=9 RX=0 TXMODE=1 (our driver) ===\n");
    drain(res[n].hdr, res[n].body);
    v2_write(descreq9,9,0,0);
    msleep(300);
    v2_read(r,37);
    res[n].test=1; res[n].desc="V2 TX=9 RX=0 TXM=1"; res[n].tx=9; res[n].rx=0; res[n].txml=1;
    res[n].got_d=got_desc(r,37); res[n].got_r=got_rst(r,37);
    res[n].c0=r32(C0); res[n].c1=r32(C1);
    memcpy(res[n].resp,r,37); res[n].resp_len=37;
    pr_info("rt: T1 result: got_desc=%d got_rst=%d c0=0x%08x c1=0x%08x resp[0..8]=[%*ph]\n",
        res[n].got_d, res[n].got_r, res[n].c0, res[n].c1, 9, r);
    n++; msleep(500);

    /* ---- TEST 2: V2 TX=14 RX=0 TXMODE=1 (match Windows TX_COUNT) ---- */
    pr_info("rt: === T2: V2 TX=14 RX=0 TXMODE=1 (Windows TX_COUNT match) ===\n");
    drain(res[n].hdr, res[n].body);
    v2_write(descreq14,14,0,0);
    msleep(300);
    v2_read(r,37);
    res[n].test=2; res[n].desc="V2 TX=14 RX=0 TXM=1"; res[n].tx=14; res[n].rx=0; res[n].txml=1;
    res[n].got_d=got_desc(r,37); res[n].got_r=got_rst(r,37);
    res[n].c0=r32(C0); res[n].c1=r32(C1);
    memcpy(res[n].resp,r,37); res[n].resp_len=37;
    pr_info("rt: T2 result: got_desc=%d got_rst=%d c0=0x%08x c1=0x%08x resp[0..8]=[%*ph]\n",
        res[n].got_d, res[n].got_r, res[n].c0, res[n].c1, 9, r);
    n++; msleep(500);

    /* ---- TEST 3: V2 TX=14 RX=0 TXMODE=0 (clear TXMODE like Windows) ---- */
    pr_info("rt: === T3: V2 TX=14 RX=0 TXMODE=0 (Windows-like) ===\n");
    drain(res[n].hdr, res[n].body);
    v2_write(descreq14,14,0,1);
    msleep(300);
    v2_read(r,37);
    res[n].test=3; res[n].desc="V2 TX=14 RX=0 TXM=0"; res[n].tx=14; res[n].rx=0; res[n].txml=0;
    res[n].got_d=got_desc(r,37); res[n].got_r=got_rst(r,37);
    res[n].c0=r32(C0); res[n].c1=r32(C1);
    memcpy(res[n].resp,r,37); res[n].resp_len=37;
    pr_info("rt: T3 result: got_desc=%d got_rst=%d c0=0x%08x c1=0x%08x resp[0..8]=[%*ph]\n",
        res[n].got_d, res[n].got_r, res[n].c0, res[n].c1, 9, r);
    n++; msleep(500);

    /* ---- TEST 4: V2 TX=9 RX=0 TXMODE=0 ---- */
    pr_info("rt: === T4: V2 TX=9 RX=0 TXMODE=0 ===\n");
    drain(res[n].hdr, res[n].body);
    v2_write(descreq9,9,0,1);
    msleep(300);
    v2_read(r,37);
    res[n].test=4; res[n].desc="V2 TX=9 RX=0 TXM=0"; res[n].tx=9; res[n].rx=0; res[n].txml=0;
    res[n].got_d=got_desc(r,37); res[n].got_r=got_rst(r,37);
    res[n].c0=r32(C0); res[n].c1=r32(C1);
    memcpy(res[n].resp,r,37); res[n].resp_len=37;
    pr_info("rt: T4 result: got_desc=%d got_rst=%d c0=0x%08x c1=0x%08x resp[0..8]=[%*ph]\n",
        res[n].got_d, res[n].got_r, res[n].c0, res[n].c1, 9, r);
    n++; msleep(500);

    /* ---- TEST 5: V2 TX=14 RX=14 TXMODE=1 (full dup, see what device sends) ---- */
    pr_info("rt: === T5: V2 TX=14 RX=14 (full-duplex, peek at MISO) ===\n");
    drain(res[n].hdr, res[n].body);
    v2_write(descreq14,14,14,0);
    msleep(300);
    v2_read(r,37);
    res[n].test=5; res[n].desc="V2 TX=14 RX=14 TXM=1"; res[n].tx=14; res[n].rx=14; res[n].txml=1;
    res[n].got_d=got_desc(r,37); res[n].got_r=got_rst(r,37);
    res[n].c0=r32(C0); res[n].c1=r32(C1);
    memcpy(res[n].resp,r,37); res[n].resp_len=37;
    pr_info("rt: T5 result: got_desc=%d got_rst=%d c0=0x%08x c1=0x%08x resp[0..8]=[%*ph]\n",
        res[n].got_d, res[n].got_r, res[n].c0, res[n].c1, 9, r);
    n++;

    /* Summary */
    pr_info("rt: ========================================\n");
    pr_info("rt: SUMMARY:\n");
    for(i=0;i<n;i++)
        pr_info("rt: T%d %-30s got_desc=%d got_rst=%d c0=0x%08x c1=0x%08x\n",
            res[i].test, res[i].desc, res[i].got_d, res[i].got_r, res[i].c0, res[i].c1);

    if (res[0].got_d || res[1].got_d || res[2].got_d || res[3].got_d)
        pr_info("rt: ★★★ DEVICE_DESC DETECTED — WRITE IS WORKING! ★★★\n");
    else
        pr_info("rt: All writes ignored — logic analyzer needed.\n");

    iounmap(b);
    return -EIO;
}
module_init(rt_init); MODULE_LICENSE("GPL");
CEOF

echo 'obj-m += rawtest.o' > "$TMPDIR/Makefile"
make -C "/lib/modules/$(uname -r)/build" M="$TMPDIR" LLVM=1 modules 2>&1 | grep -E "error:" || true
if [ ! -f "$TMPDIR/rawtest.ko" ]; then fail "Build failed"; fi
pass "Module built"

# 5. Run tests
info "5. Running tests..."
sudo dmesg -c >/dev/null
sudo insmod "$TMPDIR/rawtest.ko" 2>/dev/null || true
sleep 12  # tests take ~10s

echo ""
echo "============================================"
echo " RESULTS"
echo "============================================"
sudo dmesg | grep "rt: T[0-9] result\|rt: SUMMARY\|rt: ★\|rt: All\|rt: INIT\|rt: ==="
echo ""
echo "============================================"
echo " DRAIN DATA (first test)"
echo "============================================"
sudo dmesg | grep "rt:.*drain\|rt:.*hdr\|rt:.*body" 2>/dev/null || echo "(embedded in result lines)"

echo ""
echo "============================================"
echo " Done."
echo "============================================"
