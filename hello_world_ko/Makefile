ifneq ($(KERNELRELEASE),)
	obj-m := hello_world.o
else
	#KERNELDIR := /lib/modules/$(shell uname -r)/build
        KERNELDIR := /usr/src/linux-headers-4.4.0-64-generic/
	PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	#rm -f *.ko *.mod.c *.mod.o *.o
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
