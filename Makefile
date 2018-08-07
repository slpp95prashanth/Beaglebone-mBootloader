export ARCH:=arm
export BOARD:=ti
export SOC:=am33xx
export CROSS_COMPILE:=arm-linux-gnueabi-

include .config
include ./config
include .CFLAGS

export CC:=$(CROSS_COMPILE)gcc
export LD:=$(CROSS_COMPILE)ld.bfd
export OBJCOPY:=$(CROSS_COMPILE)objcopy

CFLAGS+= -fno-builtin -fno-stack-protector -Wall -nostdinc -Os -mcpu=cortex-a8

ifeq ($(DEBUG), 1)
CFLAGS+= -g
endif

# Entry Address
EADDR:=0x80000000

# GNU Linker
LDFLAGS:=-Bstatic -nostartfiles -nostdlib -Ttext $(EADDR)

# Linker Script
LDS:=arch/$(ARCH)/$(BOARD)/$(SOC)/lds/m-boot-spl.lds

#LIBS_PATH:= -L /usr/lib/gcc-cross/$(CROSS_COMPILE:-=)/4.7
#LIBS:=-lgcc

mBOOT_REPOSITORY = $(shell pwd)

export DIR:=$(mBOOT_REPOSITORY)

#FILES+=board/ti/am33xx/board.c drivers/serial/ns16550.c

OBJS:=$(patsubst %.c,%.o,$(FILES))
OBJS:=$(patsubst %.S,%.o,$(OBJS))

#$(OBJS):%.o:%.c
#	$(CC) $(CFLAGS) -c $< -o $@
#	$(CC) $(CFLAGS) -c $(FILES) -o $<

%.o: %.c
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

# To remove the Implicit rule

%.o: %.S
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

MLO: ${OBJS} .config
	$(LD) -T $(LDS) $(LDFLAGS) $(OBJS) $(LIBS_PATH) $(LIBS) -Map u-boot-spl.map -o u-boot-spl
	$(OBJCOPY) --gap-fill=0xff -O binary u-boot-spl u-boot-spl.bin
	$(DIR)/tools/mkimage -T omapimage -a $(EADDR) -d u-boot-spl.bin MLO

bootstrap:
	make
	lzma -v u-boot-spl.bin
	make -C bootstrap/
	cat bootstrap/bootstrap.bin u-boot-spl.bin.lzma > u-boot-spl.bin

all: MLO

.PHONY: bootstrap

clean:
	-rm *.o bootstrap/*.o
	-rm *~ .*~ bootstrap/*~
	-rm $(addsuffix *~,$(dir $(FILES)))
	-rm $(addsuffix *~,$(dir $(INCDIR)))

cleanall:
	-rm ${OBJS}
	-rm $(addsuffix *~,$(dir $(FILES)))
	-rm $(addsuffix *~,$(dir $(INCDIR)))
	-rm *.o *~ .*~ bootstrap/*.o bootstrap/*~ *.lzma
	-rm MLO u-boot-spl *.bin *.map 
	-rm bootstrap/*.bin bootstrap/bootstrap bootstrap/*.map bootstrap/MLO
	-make -C scripts/ distclean

menuconfig:
	make -C scripts/ menuconfig
	cp scripts/.config ./config
