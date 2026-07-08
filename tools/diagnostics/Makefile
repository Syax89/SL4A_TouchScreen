# Build the companion probe diagnostic module (superseded/dead-end, see archive/)
# Usage: make -f Makefile.companion
KDIR := /lib/modules/$(shell uname -r)/build

obj-m += archive/companion_probe.o

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
