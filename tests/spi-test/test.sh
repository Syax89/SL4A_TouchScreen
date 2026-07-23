#!/bin/bash
# spi-cmd tester — send commands to /proc/spi-cmd and see results
# Usage: ./spi-test.sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
MOD_DIR="$SCRIPT_DIR"
PROC=/proc/spi-cmd

die() { echo "$@"; exit 1; }

# Build and load
cd $MOD_DIR || die "cd failed"
make || die "build failed"
sudo rmmod spi_cmd 2>/dev/null
# Make sure spi-amd is not loaded (MMIO conflict)
sudo rmmod sl4a-spi-hid sl4a-spi-amd 2>/dev/null
sudo insmod $MOD_DIR/spi-cmd.ko || die "insmod failed"
echo "spi-cmd loaded."

# Helper to send command via /proc write
cmd() {
	echo "$@" | sudo tee $PROC > /dev/null 2>&1
}

show() {
	echo "=== /proc/spi-cmd ==="
	cat $PROC
	echo "=== dmesg ==="
	sudo dmesg | tail -20
}

sudo dmesg -C

echo ""
echo "=== Test 1: dump registri ==="
cmd "dump"
sudo dmesg | tail -8
echo ""

echo "=== Test 2: VALIDATE — la read raw MMIO raggiunge il device? (atteso PASS, 32 10 00 5a) ==="
cmd "validate"
sleep 0.2
sudo dmesg | tail -6
echo ""
echo ">>> Se qui non c'e' *** PASS ***, il tool NON parla col device: fermarsi qui."
echo ""

echo "=== Test 3: WRITE descriptor request (0x02, addr=0x000001) ==="
cmd "w 02 00 00 01 42 00 00 03 00 00"
sleep 0.3
sudo dmesg | tail -10
echo ""

echo "=== Test 4: VALIDATE dopo la write — il device e' cambiato? (type 7=DEVICE_DESC = successo) ==="
cmd "validate"
sleep 0.2
sudo dmesg | tail -6
