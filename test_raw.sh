#!/bin/bash
# ============================================================================
# SL4A — Raw MMIO Test Suite v2 (13 configurations)
# ============================================================================
set -e
T="/tmp/spi-raw"
cleanup() { sudo rmmod spi_hid spi_amd rawtest 2>/dev/null || true; rm -rf "$T"; }
trap cleanup EXIT
mkdir -p "$T"

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; NC='\033[0m'

echo "============================================"
echo " SL4A Raw MMIO Test Suite v2"
echo " $(date)"
echo "============================================"

# 1. Cleanup + power cycle
echo -e "${YELLOW}1. Cleanup + power cycle...${NC}"
for m in spi_hid spi_amd rawtest; do sudo rmmod "$m" 2>/dev/null || true; done
sleep 1

cat > "$T/pwr.c" << 'EOF'
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/delay.h>
static void m010(u64 a,u64 v){
    acpi_handle h;union acpi_object args[2];struct acpi_object_list al;
    if(ACPI_FAILURE(acpi_get_handle(NULL,"\\M010",&h)))return;
    args[0].type=ACPI_TYPE_INTEGER;args[0].integer.value=a;
    args[1].type=ACPI_TYPE_INTEGER;args[1].integer.value=v;
    al.count=2;al.pointer=args;acpi_evaluate_object(h,NULL,&al,NULL);
}
static int __init p_init(void){
    pr_info("pwr: OFF\n");m010(0x5B,0);msleep(200);
    pr_info("pwr: RST\n");m010(0x0103,0);msleep(100);
    pr_info("pwr: ON\n");m010(0x5B,1);msleep(100);
    pr_info("pwr: DE-RST\n");m010(0x0103,1);
    msleep(5000);pr_info("pwr: DONE\n");
    return -EIO;
}
module_init(p_init);MODULE_LICENSE("GPL");
EOF
echo 'obj-m += pwr.o' > "$T/Makefile"
make -C "/lib/modules/$(uname -r)/build" M="$T" LLVM=1 modules 2>&1|grep "error:"||true
sudo insmod "$T/pwr.ko" 2>/dev/null||true
sleep 2
echo -e "${GREEN}   Power-cycled${NC}"

# 2. PCI
echo -e "${YELLOW}2. PCI config...${NC}"
sudo setpci -s 00:14.3 B8.L=0x33ed0084 B4.L=0x7dffe000
echo -e "${GREEN}   PCI set${NC}"

# 3. Build test module
echo -e "${YELLOW}3. Building test module...${NC}"
cat > "$T/rawtest.c" << 'CEOF'
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define B      ((void __iomem*)b)
#define r32(o) readl(B+(o))
#define w32(o,v) writel(v,B+(o))
#define r8(o) readb(B+(o))
#define w8(o,v) writeb(v,B+(o))
#define r16(o) readw(B+(o))
#define w16(o,v) writew(v,B+(o))

static void __iomem *b;

static int wbusy(int ms){int i;for(i=0;i<ms*10;i++){if(!(r32(0)&BIT(31)))return 0;udelay(100);}return -1;}

/* V2 write */
static void wr(const u8 *d, u8 tx, u8 rx, int clr_txm, int nospd){
    int i;u32 c;
    w8(0x1D,(r8(0x1D)&0xFC)|0x01);
    c=r32(0);c|=BIT(20);w32(0,c);
    w8(0x45,0x02);
    c=r32(0);if(clr_txm)c&=~BIT(23);c|=(BIT(30)|BIT(29)|BIT(18));w32(0,c);
    if(!nospd){u8 n=r32(0x20)&0xF;u16 s=r16(0x44);s=(s&0xF0FF)|(n<<8);s=(s&0x0FFF)|(n<<12);w16(0x44,s);w8(0x45,0x02);}
    w8(0x48,tx);
    for(i=0;i<tx&&i<70;i++)w8(0x80+i,d[i]);
    w8(0x4B,rx);
    w8(0x45,0x02);
    w8(0x47,0x80);
    wbusy(2000);
}

/* V2 read approval */
static void rd(u8 *rx, u8 rxl){
    int i;u32 c;
    w8(0x1D,(r8(0x1D)&0xFC)|0x01);
    c=r32(0);c|=BIT(20);w32(0,c);
    w8(0x45,0x0B);
    c=r32(0);c|=(BIT(30)|BIT(29)|BIT(18));w32(0,c);
    {u8 n=r32(0x20)&0xF;u16 s=r16(0x44);s=(s&0xF0FF)|(n<<8);s=(s&0x0FFF)|(n<<12);w16(0x44,s);w8(0x45,0x0B);}
    w8(0x48,5);
    w8(0x80,0x0B);w8(0x81,0);w8(0x82,0);w8(0x83,0);w8(0x84,0xFF);
    w8(0x4B,rxl);
    w8(0x45,0x0B);
    w8(0x47,0x80);
    wbusy(2000);
    for(i=0;i<rxl;i++){u16 w=r16(0x80+5+(i/2)*2);rx[i]=(i&1)?(u8)(w>>8):(u8)(w&0xFF);}
}

static void drain(void){u8 r[9];rd(r,9);msleep(10);rd(r,9);}

static int has_desc(const u8 *d,int l){int i;for(i=0;i<l-3;i++)if(d[i]==0x72&&d[i+1]==0x80&&d[i+2]==0x00&&d[i+3]==0x5A)return 1;return 0;}
static int has_rst(const u8 *d,int l){int i;for(i=0;i<l-3;i++)if(d[i]==0x32&&d[i+1]==0x10&&d[i+2]==0x00&&d[i+3]==0x5A)return 1;return 0;}

static int __init rt_init(void)
{
    int i,j,n=0;
    u8 d9[9]={0,0,1,0x42,0,0,3,0,0};
    u8 d14[14]={0,0,1,0x42,0,0,3,0,0,0,0,0,0,0};
    u8 d14f[14]={0,0,1,0x42,0,0,3,0,0,0xFF,0xFF,0xFF,0xFF,0xFF};
    u8 d20[20]={0,0,0,0,1,0,0x42,0,0,0,0,0,3,0,0,0,0,0,0,0};
    u8 r[64];
    struct res { int d,r; u32 c0; } rs[16];
    char buf[80];

    b=ioremap(0xFEC10000,0x200);
    pr_info("rt: INIT c0=0x%08x\n",r32(0));

#define T(label,fifo,tx,rx,txm,nosp,desc) do{ \
    pr_info("rt: %s %s\n",label,desc); \
    drain(); \
    wr(fifo,tx,rx,txm,nosp);msleep(300); \
    rd(r,37); \
    rs[n].d=has_desc(r,37);rs[n].r=has_rst(r,37);rs[n].c0=r32(0); \
    pr_info("rt: %s desc=%d rst=%d c0=0x%08x\n",label,rs[n].d,rs[n].r,rs[n].c0); \
    n++;msleep(400); \
}while(0)

    T("T1",d9,9,0,0,0,"V2 TX=9 RX=0 TXM=1 (baseline)");
    T("T2",d14,14,0,0,0,"V2 TX=14 RX=0 TXM=1 (Win TX_COUNT)");
    T("T3",d14,14,0,1,0,"V2 TX=14 RX=0 TXM=0 (Win-like)");
    T("T4",d9,9,0,1,0,"V2 TX=9 RX=0 TXM=0");
    T("T5",d14,14,14,0,0,"V2 TX=14 RX=14 TXM=1 (full-dup)");

    /* N1: no 0x44 dance */
    T("N1",d14,14,0,1,1,"V2 TX=14 RX=0 TXM=0 NO-0x44");
    /* N2: FF padding */
    T("N2",d14f,14,0,0,0,"V2 TX=14 RX=0 TXM=1 FF-pad");
    /* N3: 16-bit word write */
    T("N3",d20,20,0,0,0,"V2 TX=20 RX=0 TXM=1 (16bit word)");
    /* N4: NO DRAIN */
    pr_info("rt: N4 V2 TX=9 RX=0 TXM=1 NO-DRAIN\n");
    wr(d9,9,0,0,0);msleep(300);rd(r,37);
    rs[n].d=has_desc(r,37);rs[n].r=has_rst(r,37);rs[n].c0=r32(0);
    pr_info("rt: N4 desc=%d rst=%d c0=0x%08x\n",rs[n].d,rs[n].r,rs[n].c0);
    n++;msleep(400);
    /* N5: NO DRAIN + TX=14 + TXM=0 */
    pr_info("rt: N5 V2 TX=14 RX=0 TXM=0 NO-DRAIN\n");
    wr(d14,14,0,1,0);msleep(300);rd(r,37);
    rs[n].d=has_desc(r,37);rs[n].r=has_rst(r,37);rs[n].c0=r32(0);
    pr_info("rt: N5 desc=%d rst=%d c0=0x%08x\n",rs[n].d,rs[n].r,rs[n].c0);
    n++;msleep(400);
    /* N6: 3x DESCREQ burst */
    pr_info("rt: N6 3x DESCREQ TX=14 TXM=0 (burst)\n");
    drain();
    for(j=0;j<3;j++){wr(d14,14,0,1,0);msleep(50);}
    msleep(300);rd(r,37);
    rs[n].d=has_desc(r,37);rs[n].r=has_rst(r,37);rs[n].c0=r32(0);
    pr_info("rt: N6 desc=%d rst=%d c0=0x%08x\n",rs[n].d,rs[n].r,rs[n].c0);
    n++;

    pr_info("rt: ====== SUMMARY (%d tests) ======\n",n);
    for(i=0;i<n;i++){
        snprintf(buf,sizeof(buf),"%s desc=%d rst=%d c0=0x%08x",
            i<5?"T":"N",rs[i].d,rs[i].r,rs[i].c0);
        pr_info("rt: %s\n",buf);
    }
    {
        int w=0;for(i=0;i<n;i++)if(rs[i].d)w++;
        if(w)pr_info("rt: *** %d/%d DEVICE_DESC — WRITE WORKS! ***\n",w,n);
        else pr_info("rt: 0/%d DEVICE_DESC — all writes ignored\n",n);
    }
    iounmap(b);
    return -EIO;
}
module_init(rt_init);MODULE_LICENSE("GPL");
CEOF

echo 'obj-m += rawtest.o' > "$T/Makefile"
make -C "/lib/modules/$(uname -r)/build" M="$T" LLVM=1 modules 2>&1|grep "error:"||true
[ -f "$T/rawtest.ko" ] || { echo -e "${RED}Build failed${NC}"; exit 1; }
echo -e "${GREEN}   Built${NC}"

# 4. Run
echo -e "${YELLOW}4. Running tests...${NC}"
sudo dmesg -c >/dev/null
sudo insmod "$T/rawtest.ko" 2>/dev/null||true
sleep 20

echo ""
echo "============================================"
echo " RESULTS"
echo "============================================"
sudo dmesg | grep "rt: " | grep -E "SUMMARY|DESC|WRITE|ignored|T[0-9]|N[0-9]"
echo ""
echo "============================================"
echo " Done."
echo "============================================"
