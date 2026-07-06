#!/bin/bash
# Test different ACPI power-cycle sequences, check which makes device respond
set -e
T="/tmp/spi-seq"
cleanup() { sudo rmmod spi_hid spi_amd seqtest 2>/dev/null || true; rm -rf "$T"; }
trap cleanup EXIT
mkdir -p "$T"

GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'; CYAN='\033[1;36m'; NC='\033[0m'

echo "============================================"
echo " SL4A — Power Sequence Test"
echo " $(date)"
echo "============================================"

# Cleanup
for m in spi_hid spi_amd seqtest; do sudo rmmod "$m" 2>/dev/null || true; done
sleep 1

# PCI config
sudo setpci -s 00:14.3 B8.L=0x33ed0084 B4.L=0x7dffe000

# Build test module
cat > "$T/seqtest.c" << 'CEOF'
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/string.h>

static void __iomem *b;
#define BIT(n) (1UL<<(n))

static u32 r32(int o){return readl(b+o);}
static void w32(int o,u32 v){writel(v,b+o);}
static u8 r8(int o){return readb(b+o);}
static void w8(int o,u8 v){writeb(v,b+o);}
static u16 r16(int o){return readw(b+o);}
static void w16(int o,u16 v){writew(v,b+o);}

static int wbusy(int ms){int i;for(i=0;i<ms*10;i++){if(!(r32(0)&BIT(31)))return 0;udelay(100);}return -1;}

/* Quick read — just get 9 bytes, check for RESET_RSP header */
static int quick_read(void){
    int i;u32 c;u8 rx[18];
    w8(0x1D,(r8(0x1D)&0xFC)|0x01);
    c=r32(0);c|=BIT(20);w32(0,c);
    w8(0x45,0x0B);
    c=r32(0);c|=(BIT(30)|BIT(29)|BIT(18));w32(0,c);
    {u8 n=r32(0x20)&0xF;u16 s=r16(0x44);s=(s&0xF0FF)|(n<<8);s=(s&0x0FFF)|(n<<12);w16(0x44,s);w8(0x45,0x0B);}
    w8(0x48,5);
    w8(0x80,0x0B);w8(0x81,0);w8(0x82,0);w8(0x83,0);w8(0x84,0xFF);
    w8(0x4B,9);
    w8(0x45,0x0B);
    w8(0x47,0x80);
    wbusy(2000);
    for(i=0;i<9;i++){u16 w=r16(0x80+5+(i/2)*2);rx[i]=(i&1)?(u8)(w>>8):(u8)(w&0xFF);}
    /* Check for RESET_RSP header 32 10 00 5A */
    for(i=0;i<6;i++)if(rx[i]==0x32&&rx[i+1]==0x10&&rx[i+2]==0x00&&rx[i+3]==0x5A)return 1;
    /* Check for DEVICE_DESC header 72 80 00 5A */
    for(i=0;i<6;i++)if(rx[i]==0x72&&rx[i+1]==0x80&&rx[i+2]==0x00&&rx[i+3]==0x5A)return 2;
    /* Print raw for debugging */
    pr_info("seq: raw=[%*ph]\n",9,rx);
    return 0;
}

/* ACPI helpers */
static void m009(u64 a){acpi_handle h;union acpi_object arg;struct acpi_object_list al;if(ACPI_FAILURE(acpi_get_handle(NULL,"\\M009",&h))){pr_err("seq: M009 not found\n");return;}arg.type=ACPI_TYPE_INTEGER;arg.integer.value=a;al.count=1;al.pointer=&arg;acpi_evaluate_object(h,NULL,&al,NULL);}
static void m010(u64 a,u64 v){acpi_handle h;union acpi_object args[2];struct acpi_object_list al;if(ACPI_FAILURE(acpi_get_handle(NULL,"\\M010",&h))){pr_err("seq: M010 not found\n");return;}args[0].type=ACPI_TYPE_INTEGER;args[0].integer.value=a;args[1].type=ACPI_TYPE_INTEGER;args[1].integer.value=v;al.count=2;al.pointer=args;acpi_evaluate_object(h,NULL,&al,NULL);}

/* Sequence definitions */
static void seq_baseline(void){pr_info("seq: baseline (M010 only)\n");m010(0x5B,0);msleep(200);m010(0x0103,0);msleep(100);m010(0x5B,1);msleep(100);m010(0x0103,1);msleep(5000);}
static void seq_full_ini(void){pr_info("seq: full _INI (M009 read + M010 write)\n");m009(0x5B);m009(0x0103);m010(0x5B,0);msleep(200);m010(0x0103,0);msleep(100);m010(0x5B,1);msleep(200);m010(0x0103,1);msleep(5000);}
static void seq_no_reset(void){pr_info("seq: power ON only, no reset toggle\n");m010(0x5B,0);msleep(200);m010(0x5B,1);msleep(5000);}
static void seq_long_wait(void){pr_info("seq: M010 with 10s wait\n");m010(0x5B,0);msleep(200);m010(0x0103,0);msleep(100);m010(0x5B,1);msleep(200);m010(0x0103,1);msleep(10000);}
static void seq_reset_only(void){pr_info("seq: reset toggle only (no power cycle)\n");m010(0x0103,0);msleep(100);m010(0x0103,1);msleep(5000);}
static void seq_reverse(void){pr_info("seq: reverse order (reset then power)\n");m010(0x0103,0);msleep(100);m010(0x5B,0);msleep(200);m010(0x5B,1);msleep(200);m010(0x0103,1);msleep(5000);}
/* No sequence — just wait and test */
static void seq_none(void){pr_info("seq: no power cycle (just wait 5s)\n");msleep(5000);}

static int __init st_init(void)
{
    int results[10];
    char *names[10];
    void (*seqs[10])(void);
    int i, n=0;

    b=ioremap(0xFEC10000,0x200);
    if(!b)return -ENOMEM;

    pr_info("seq: INIT c0=0x%08x\n",r32(0));

    /* Check current state without any power cycle */
    pr_info("seq: === S0: current state (no touch) ===\n");
    results[0]=quick_read();
    names[0]="S0: current state"; seqs[0]=NULL; n=1;

    /* Run sequences */
    #define ADD_SEQ(fn,label) do{names[n]=label;seqs[n]=fn;n++;}while(0)
    ADD_SEQ(seq_none,"S1: no cycle (wait 5s)");
    ADD_SEQ(seq_baseline,"S2: M010 baseline (5s)");
    ADD_SEQ(seq_full_ini,"S3: full _INI (M009+M010, 5s)");
    ADD_SEQ(seq_no_reset,"S4: power only, no reset (5s)");
    ADD_SEQ(seq_long_wait,"S5: baseline + 10s wait");
    ADD_SEQ(seq_reset_only,"S6: reset only, no power (5s)");
    ADD_SEQ(seq_reverse,"S7: reset then power (5s)");

    for(i=1;i<n;i++){
        pr_info("seq: ========================================\n");
        seqs[i]();
        pr_info("seq: quick-read after %s...\n",names[i]);
        results[i]=quick_read();
        pr_info("seq: %s result=%d (0=nothing 1=RESET_RSP 2=DEVICE_DESC)\n",names[i],results[i]);
    }

    pr_info("seq: ====== RESULTS ======\n");
    for(i=0;i<n;i++)
        pr_info("seq: %s → %s\n",names[i],
            results[i]==2?"DEVICE_DESC ★★★":
            results[i]==1?"RESET_RSP ✓":
            results[i]==0?"nothing ✗":"unknown");

    iounmap(b);
    return -EIO;
}
module_init(st_init);MODULE_LICENSE("GPL");
CEOF

echo 'obj-m += seqtest.o' > "$T/Makefile"
make -C "/lib/modules/$(uname -r)/build" M="$T" LLVM=1 modules 2>&1|grep "error:"||true
[ -f "$T/seqtest.ko" ] || { echo -e "${RED}Build failed${NC}"; exit 1; }

echo -e "${YELLOW}Running sequence tests...${NC}"
sudo dmesg -c >/dev/null
sudo insmod "$T/seqtest.ko" 2>/dev/null||true
sleep 55  # 7 sequences × ~8s each

echo ""
echo "============================================"
echo " POWER SEQUENCE RESULTS"
echo "============================================"
sudo dmesg | grep "seq:.*→\|seq:.*result=" | while IFS= read -r line; do
    if echo "$line" | grep -q "★★★"; then
        echo -e "${GREEN}$line${NC}"
    elif echo "$line" | grep -q "✓"; then
        echo -e "${CYAN}$line${NC}"
    elif echo "$line" | grep -q "✗"; then
        echo -e "${RED}$line${NC}"
    else
        echo "$line"
    fi
done
echo ""
echo " --- Raw data from failed sequences ---"
sudo dmesg | grep "seq: raw="

echo ""
echo "Done."
