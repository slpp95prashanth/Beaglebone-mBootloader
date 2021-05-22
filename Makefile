ARCH:=arm
BOARD:=ti
SOC:=am33xx
CROSS_COMPILE:=arm-linux-gnueabi-
export DIR:= $(shell pwd)/

include .config
include ./scripts/.config
include .CFLAGS
export CC:=$(CROSS_COMPILE)gcc -g
LD:=$(CROSS_COMPILE)ld.bfd
OBJCOPY:=$(CROSS_COMPILE)objcopy

export CFLAGS+= -fno-builtin -fno-stack-protector -Wall -nostdinc -Os -mcpu=cortex-a8

ifeq ($(DEBUG), 1)
CFLAGS+= -g
endif

# GNU Linker
LDFLAGS:=-Bstatic -nostartfiles -nostdlib

# Entry Address
EADDR:=0x402F0400

# Linker Script
LDS:=arch/$(ARCH)/$(BOARD)/$(SOC)/lds/m-boot-spl.lds

#LIBS_PATH:= -L /usr/lib/gcc-cross/$(CROSS_COMPILE:-=)/4.7
#LIBS:=-lgcc

ifdef CONFIG_NET_UIP
LIBS += -L ./uip/ -luip
endif
mBOOT_REPOSITORY = $(shell pwd)

DIR:=$(mBOOT_REPOSITORY)

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
ifdef CONFIG_NET_UIP
	make -C uip/
endif
	$(LD) -T $(LDS) $(LDFLAGS) -Ttext $(EADDR) $(OBJS) $(LIBS_PATH) $(LIBS) -Map u-boot-spl.map -o u-boot-spl
	$(OBJCOPY) --gap-fill=0xff -O binary u-boot-spl u-boot-spl.bin
	$(DIR)/tools/mkimage -T omapimage -a $(EADDR) -d u-boot-spl.bin MLO
	
all: MLO
	echo "BUILD COMPLETE"

clean:
	-rm *.o
	-rm *~ .*~
	-rm $(addsuffix *~,$(dir $(FILES)))
	-rm $(addsuffix *~,$(dir $(INCDIR)))

cleanall:
	-rm ${OBJS}
	-rm $(addsuffix *~,$(dir $(FILES)))
	-rm $(addsuffix *~,$(dir $(INCDIR)))
	-rm *.o *~ .*~
	-rm MLO u-boot-spl u-boot-spl.bin u-boot-spl.map
	-make -C scripts/ distclean
	-make -C uip clean

menuconfig:
	make -C scripts/ menuconfig
	cp scripts/.config ./config
