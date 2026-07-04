# Build both spi-amd.ko and spi-hid.ko from this directory.
# Usage: make LLVM=1
KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
