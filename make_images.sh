#!/bin/sh
arm-linux-gnueabi-gcc -c arch/arm/cpu/armv7/start.S
arm-linux-gnueabi-gcc -c board.c -I .
arm-linux-gnueabi-ld.bfd -T u-boot-spl.lds --gc-sections -Bstatic -Ttext 0x402F0400 --start-group board.o -L /usr/lib/gcc-cross/arm-linux-gnueabi/4.7 -lgcc -L /usr/lib/gcc-cross/arm-linux-gnueabi/4.7 -lgcc -Map u-boot-spl.map -o u-boot-spl
arm-linux-gnueabi-objcopy --gap-fill=0xff -O binary u-boot-spl u-boot-spl.bin
/home/prashanth/BBB/u-boot/u-boot/tools/mkimage  -T omapimage -a 0x402F0400 -d u-boot-spl.bin MLO



 
