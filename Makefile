ARCH:=arm
BOARD:=ti
SOC:=am33xx
CROSS_COMPILE:=arm-linux-gnueabi-

include .config

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld.bfd
OBJCOPY:=$(CROSS_COMPILE)objcopy

CFLAGS+=$(INCDIR) -fno-builtin -fno-stack-protector -Wall

# GNU Linker
LDFLAGS:=--gc-sections -Bstatic

# Entry Address
EADDR:=0x402F0400

# Linker Script
LDS:=arch/$(ARCH)/$(BOARD)/$(SOC)/lds/m-boot-spl.lds

LIBS_PATH:=/usr/lib/gcc-cross/$(CROSS_COMPILE:-=)/4.7
LIBS:=-lgcc

mBOOT_REPOSITORY = $(shell pwd)

DIR:=$(mBOOT_REPOSITORY)

#FILES+=board/ti/am33xx/board.c drivers/serial/ns16550.c

OBJS:=$(patsubst %.c,%.o,$(FILES))
OBJS:=$(patsubst %.S,%.o,$(OBJS))

#$(OBJS):%.o:%.c
#	$(CC) $(CFLAGS) -c $< -o $@
#	$(CC) $(CFLAGS) -c $(FILES) -o $<
#	echo ${OBJS}

%.o: %.c
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

MLO: ${OBJS}
	$(LD) -T $(LDS) $(LDFLAGS) -Ttext $(EADDR) $(OBJS) -L $(LIBS_PATH) $(LIBS) -Map u-boot-spl.map -o u-boot-spl
	$(OBJCOPY) --gap-fill=0xff -O binary u-boot-spl u-boot-spl.bin
	$(DIR)/tools/mkimage -T omapimage -a $(EADDR) -d u-boot-spl.bin MLO
	
all: MLO

clean:
	-rm *.o
	-rm *~

cleanall:
	-rm ${OBJS}
	-rm *.o *~
	rm MLO u-boot-spl u-boot-spl.bin u-boot-spl.map
