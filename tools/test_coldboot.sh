#!/bin/bash
# Test driver dopo cold boot
echo "=== Cold Boot Test SL4A Touchscreen ==="
echo "Caricamento moduli..."
sudo modprobe spi_amd
sudo modprobe spi_hid
echo "Attendo 5 secondi..."
sleep 5
echo ""
echo "=== dmesg (ultimi eventi SEQ) ==="
sudo dmesg | grep -E "SEQ|spi-amd.*AMDI|DEVICE_DESC|RESET_RSP|probe" | tail -30
echo ""
echo "=== Interrupts ==="
cat /proc/interrupts | grep spi-MSHW
