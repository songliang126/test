#KERNELDIR := /lib/modules/$(shell uname -r)/build
KERNELDIR := /usr/src/linux-headers-4.4.0-64-generic/
PWD := $(shell pwd)
obj-m := create_chrdev.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
