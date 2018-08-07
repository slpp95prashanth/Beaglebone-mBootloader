include .config

EADDR:=0x402f0400

BOOTSTRAP_OBJS:=$(patsubst %.c,%.o,$(BOOTSTRAP_FILES))
BOOTSTRAP_OBJS:=$(patsubst %.S,%.o,$(BOOTSTRAP_OBJS))

LZMA_IMAGE_SIZE:=$(shell ls -g $(DIR)/u-boot-spl.bin.lzma | cut -d " " -f 4)

BOOTSTRAP_LDS:=m-boot-spl.lds

BOOTSTRAP_CFLAGS+=-fno-stack-protector -DLZMA_SIZE=$(LZMA_IMAGE_SIZE)

BOOTSTRAP_LDFLAGS:=-Bstatic -nostartfiles -nostdlib -Ttext $(EADDR)

%.o: %.c
	$(CC) $(BOOTSTRAP_CFLAGS) $(INCDIR) -c $< -o $@

# To remove the Implicit rule

%.o: %.S
	$(CC) $(BOOTSTRAP_CFLAGS) $(INCDIR) -c $< -o $@

all: ${BOOTSTRAP_OBJS}
	echo $(DIR)
	$(LD) -T $(BOOTSTRAP_LDS) $(BOOTSTRAP_LDFLAGS) $(BOOTSTRAP_OBJS) $(LIBS_PATH) $(LIBS) -Map bootstrap.map -o bootstrap
	$(OBJCOPY) --gap-fill=0xff -O binary bootstrap bootstrap.bin
	$(DIR)/tools/mkimage -T omapimage -a $(EADDR) -d bootstrap.bin MLO

