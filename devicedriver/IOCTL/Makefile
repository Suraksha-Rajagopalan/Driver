obj-m += scull.o
scull-objs := scull_main.o scull_ioctl.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.o *.mod.c *.mod.o *.order *.symvers

