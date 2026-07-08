#!/bin/bash
# Auto-executed by systemd at boot — tests SL4A touchscreen before anything else
LOG=/tmp/oneshot_boot.log
MOD=/home/simone/spi-hid/tools/diagnostics/archive/oneshot.ko
LPC=00:14.3

echo "=== $(date) SL4A One-Shot ===" > $LOG

# Mode 1: 16-bit FIFO
echo "--- MODE 16-bit ---" >> $LOG
setpci -s $LPC B8.L=0x33ED0084 2>>$LOG
setpci -s $LPC B4.L=0x7DFFE000 2>>$LOG
echo "B8=$(setpci -s $LPC B8.L) B4=$(setpci -s $LPC B4.L)" >> $LOG
insmod $MOD 2>>$LOG
sleep 2
dmesg | grep "oneshot:" >> $LOG
rmmod oneshot 2>>$LOG

# Mode 2: 8-bit FIFO
echo "--- MODE 8-bit ---" >> $LOG
setpci -s $LPC B8.L=0x33ED0004 2>>$LOG
setpci -s $LPC B4.L=0x00000000 2>>$LOG
echo "B8=$(setpci -s $LPC B8.L) B4=$(setpci -s $LPC B4.L)" >> $LOG
insmod $MOD 2>>$LOG
sleep 2
dmesg | grep "oneshot:" | grep -v "MODE 16" >> $LOG
rmmod oneshot 2>>$LOG

echo "=== $(date) DONE ===" >> $LOG
