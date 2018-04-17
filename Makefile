ARCH:=arm
BOARD:=ti
SOC:=am33xx
CROSS_COMPILE:=arm-linux-gnueabi-

CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld.bfd
OBJCOPY:=$(CROSS_COMPILE)objcopy

INCDIR:= -I . -I include/ -I drivers/include 

LDFLAGS:=--gc-sections -Bstatic
CFLAGS:=-g $(INCDIR)

# Entry Address
EADDR:=0x402F0400
# Linker Script
LDS:=arch/$(ARCH)/$(BOARD)/$(SOC)/lds/u-boot-spl.lds

LIBS_PATH:=/usr/lib/gcc-cross/arm-linux-gnueabi/4.7
LIBS:=-lgcc

mBOOT_REPOSITORY = $(shell pwd)

DIR:=$(mBOOT_REPOSITORY)
FILES:=board/ti/am33xx/board.o

OBJS:=$(FILES)

all: $(OBJS)
	$(CC) $(CFLAGS) -c board/ti/am33xx/board.c -o $(FILES)
	$(LD) -T $(LDS) $(LDFLAGS) -Ttext $(EADDR) $(OBJS) -L $(LIBS_PATH) $(LIBS) -Map u-boot-spl.map -o u-boot-spl
	$(OBJCOPY) --gap-fill=0xff -O binary u-boot-spl u-boot-spl.bin
	$(DIR)/tools/mkimage -T omapimage -a $(EADDR) -d u-boot-spl.bin MLO

clean:
	-rm *.o
	-rm *~

cleanall:
	-rm *.o *~
	-rm MLO u-boot-spl u-boot-spl.bin u-boot-spl.map
