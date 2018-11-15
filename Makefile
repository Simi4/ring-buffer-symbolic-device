ifneq ($(KERNELRELEASE),)
obj-m := ring.o
else
KDIR := /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KDIR) M=$(shell pwd)
endif
