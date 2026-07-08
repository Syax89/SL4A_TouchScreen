#!/bin/bash
# One-shot test: run IMMEDIATELY after clean reboot.
# Tests both 8-bit and 16-bit FIFO modes.
#
# Usage: sudo bash oneshot.sh
#
set -e

MODULE=/home/simone/spi-hid/tools/diagnostics/archive/oneshot.ko
LPC="00:14.3"

echo "=== SL4A Touchscreen One-Shot Test ==="

# ---- MODE 1: 16-bit FIFO ----
echo ""
echo ">>> MODE 1: 16-bit FIFO (PCI 0xB8.7=1)"
sudo setpci -s $LPC B8.L=0x33ED0084
sudo setpci -s $LPC B4.L=0x7DFFE000
echo "    B8=$(sudo setpci -s $LPC B8.L)  B4=$(sudo setpci -s $LPC B4.L)"
sudo insmod $MODULE 2>/dev/null || true
sleep 2
sudo dmesg | grep "oneshot:" | tail -40
sudo rmmod oneshot 2>/dev/null || true

echo ""
echo ">>> MODE 2: 8-bit FIFO (PCI 0xB8.7=0)"
sudo setpci -s $LPC B8.L=0x33ED0004
sudo setpci -s $LPC B4.L=0x00000000
echo "    B8=$(sudo setpci -s $LPC B8.L)  B4=$(sudo setpci -s $LPC B4.L)"
sudo insmod $MODULE 2>/dev/null || true
sleep 2
sudo dmesg | grep "oneshot:" | tail -40
sudo rmmod oneshot 2>/dev/null || true

echo ""
echo "=== DONE ==="
echo "Full log: sudo dmesg | grep oneshot:"
