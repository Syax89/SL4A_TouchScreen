#!/bin/bash
# ============================================================================
# SL4A Touchscreen Test Script
# - Power-cycles the touchscreen via ACPI GPIO
# - Verifies no stale modules
# - Sets PCI config
# - Loads and tests the driver
# ============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
DRIVER_DIR="$SCRIPT_DIR/driver"
GPIOTEST="$SCRIPT_DIR/tools/gpio_test.c"
TMPDIR="/tmp/spi-test-$$"
RESULTS=""

cleanup() {
    echo "=== Cleanup ==="
    sudo rmmod spi_hid 2>/dev/null || true
    sudo rmmod spi_amd 2>/dev/null || true
    sudo rmmod gpio_test 2>/dev/null || true
    rm -rf "$TMPDIR"
}

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

fail() { echo -e "${RED}FAIL: $1${NC}"; exit 1; }
pass() { echo -e "${GREEN}OK: $1${NC}"; }
info() { echo -e "${YELLOW}$1${NC}"; }

trap cleanup EXIT
mkdir -p "$TMPDIR"

echo "============================================"
echo " SL4A Touchscreen Driver Test"
echo " $(date)"
echo "============================================"
echo ""

# ---- Step 1: Check for stale modules ----
info "Step 1: Checking for stale modules..."
STALE=""
for mod in spi_hid spi_amd gpio_test; do
    if lsmod | grep -q "^$mod "; then
        STALE="$STALE $mod"
    fi
done
if [ -n "$STALE" ]; then
    info "  Found stale modules:$STALE — unloading..."
    for mod in $STALE; do
        sudo rmmod "$mod" 2>/dev/null || true
    done
    sleep 1
fi
pass "No stale modules"

# ---- Step 2: Build and load GPIO power-cycle module ----
info "Step 2: Power-cycling touchscreen via ACPI GPIO..."
cat > "$TMPDIR/gpio_cycle.c" << 'EOF'
#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/delay.h>

static void m010_call(u64 addr, u64 val) {
    acpi_handle h;
    union acpi_object args[2];
    struct acpi_object_list arg_list;
    if (ACPI_FAILURE(acpi_get_handle(NULL, "\\M010", &h))) {
        pr_err("gpio_cycle: M010 not found\n");
        return;
    }
    args[0].type = ACPI_TYPE_INTEGER; args[0].integer.value = addr;
    args[1].type = ACPI_TYPE_INTEGER; args[1].integer.value = val;
    arg_list.count = 2; arg_list.pointer = args;
    acpi_evaluate_object(h, NULL, &arg_list, NULL);
}

static int __init gpio_cycle_init(void)
{
    pr_info("gpio_cycle: POWER OFF (M010:0x5B=0)\n");
    m010_call(0x5B, 0);
    msleep(100);

    pr_info("gpio_cycle: RESET ASSERT (M010:0x0103=0)\n");
    m010_call(0x0103, 0);
    msleep(50);

    pr_info("gpio_cycle: POWER ON (M010:0x5B=1)\n");
    m010_call(0x5B, 1);
    msleep(20);

    pr_info("gpio_cycle: RESET DEASSERT (M010:0x0103=1)\n");
    m010_call(0x0103, 1);
    msleep(100);

    pr_info("gpio_cycle: done — touchscreen power-cycled\n");
    return -EIO; /* Unload immediately */
}
module_init(gpio_cycle_init);
MODULE_LICENSE("GPL");
EOF

echo 'obj-m += gpio_cycle.o' > "$TMPDIR/Makefile"
make -C "/lib/modules/$(uname -r)/build" M="$TMPDIR" LLVM=1 modules 2>&1 | grep -E "error:|warning:" || true

sudo dmesg -c > /dev/null
sudo insmod "$TMPDIR/gpio_cycle.ko" 2>/dev/null || true
sleep 1
dmesg_out=$(sudo dmesg | grep gpio_cycle)
echo "$dmesg_out"
if echo "$dmesg_out" | grep -q "not found"; then
    fail "ACPI M010 not available — cannot power-cycle"
fi
if echo "$dmesg_out" | grep -q "done"; then
    pass "Touchscreen power-cycled"
else
    info "Power cycle attempted (may not have GPIO on this kernel)"
fi

# ---- Step 3: Set PCI configuration ----
info "Step 3: Setting PCI config..."
sudo setpci -s 00:14.3 B8.L=0x33ED0084
sudo setpci -s 00:14.3 B4.L=0x7DFFE000
B8=$(sudo setpci -s 00:14.3 B8.L)
B4=$(sudo setpci -s 00:14.3 B4.L)
echo "  PCI 0xB8 = $B8"
echo "  PCI 0xB4 = $B4"
if [ "$B8" = "33ed0084" ] && [ "$B4" = "7dffe000" ]; then
    pass "PCI config set correctly"
else
    info "PCI config: B8=$B8 B4=$B4 (expected 33ed0084, 7dffe000)"
fi

# ---- Step 4: Build driver modules ----
info "Step 4: Building driver modules..."
make -C "$DRIVER_DIR" LLVM=1 clean 2>&1 | tail -1
make -C "$DRIVER_DIR" LLVM=1 2>&1 | grep -E "error:|warning:" || true
if [ -f "$DRIVER_DIR/spi-amd.ko" ] && [ -f "$DRIVER_DIR/spi-hid.ko" ]; then
    pass "Driver modules built"
else
    fail "Build failed"
fi

# ---- Step 5: Load modules and test ----
info "Step 5: Loading spi-amd..."
sudo dmesg -c > /dev/null
sudo insmod "$DRIVER_DIR/spi-amd.ko"
sleep 1
if lsmod | grep -q spi_amd; then
    pass "spi-amd loaded"
else
    fail "spi-amd failed to load"
fi

info "Step 6: Loading spi-hid and monitoring..."
sudo insmod "$DRIVER_DIR/spi-hid.ko"
sleep 5  # Wait for SEQ thread to run

# ---- Step 7: Collect results ----
info "Step 7: Results"
echo ""
DMESG=$(sudo dmesg)

# Check for successful reads
READ_COUNT=$(echo "$DMESG" | grep -c "32 10 00 5a" || true)
echo "  RESET_RSP headers detected: $READ_COUNT"

# Check for DESCREQ frames
DESCREQ_COUNT=$(echo "$DMESG" | grep -c "02 00 00 01 42 00 00 03 00 00" || true)
echo "  DESCREQ frames sent: $DESCREQ_COUNT"

# Check for DEVICE_DESC (success!)
if echo "$DMESG" | grep -q "72 80 00 5a"; then
    echo -e "  ${GREEN}★★★★★ DEVICE_DESC DETECTED! Write is WORKING! ★★★★★${NC}"
else
    echo -e "  ${RED}No DEVICE_DESC — device still ignoring writes${NC}"
fi

# Check for RPT_DESC
if echo "$DMESG" | grep -q "82 B0 0E 5a"; then
    echo -e "  ${GREEN}★★★★★ RPT_DESC DETECTED! ★★★★★${NC}"
fi

# Show the first few transactions
echo ""
echo "  --- First transactions ---"
echo "$DMESG" | grep -E "SEQ.*type=|DESCREQ sent|RESET_RSP" | head -12

# Show CTRL0 state
echo ""
echo "  --- CTRL0 snapshots ---"
echo "$DMESG" | grep "WRITE PRE-TRIG" | head -3

echo ""
echo "============================================"
echo " Test complete."
echo "============================================"
