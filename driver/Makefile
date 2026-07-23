# Build sl4a-spi-amd.ko and sl4a-spi-hid.ko from this directory.
# Usage: make LLVM=1 [KDIR=/path/to/kernel/build]
KDIR ?= /lib/modules/$(shell uname -r)/build
M ?= $(CURDIR)

all:
	$(MAKE) -C $(KDIR) M=$(M) modules

clean:
	$(MAKE) -C $(KDIR) M=$(M) clean
