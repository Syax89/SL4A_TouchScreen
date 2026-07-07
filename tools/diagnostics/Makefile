# Build the companion probe diagnostic module
# Usage: make -f Makefile.companion
KDIR := /lib/modules/$(shell uname -r)/build

obj-m += companion_probe.o

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
