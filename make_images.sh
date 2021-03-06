#!/bin/sh
set -x
#set -e

SERIAL_UART=/dev/ttyUSB1

if [ ! -f u-boot-spl.bin ] ; then
    exit
fi
tty1=`ps -eaf | grep minicom | grep -v color | grep -o pts/...`
tty1=`echo $tty1 | head -n1 | cut -d " " -f1`
echo $tty1

sudo pkill minicom

sx -vvv u-boot-spl.bin < $SERIAL_UART > $SERIAL_UART

if [ ! -z "$tty1" ] ; then
    ./tools/a.out -n /dev/$tty1 "sudo minicom -D $SERIAL_UART"
fi

exit
if [ ! -d /media/$USER/boot ]; then
    sudo mkdir /media/$USER/boot
    sudo mount /dev/sdc1 /media/$USER/boot
fi

sudo cp MLO /media/$USER/boot/
sudo umount /media/$USER/*

if [ -d /media/$USER/boot ]; then
    sudo rm -r /media/$USER/boot
fi







