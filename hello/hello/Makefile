obj-m := hello.o

#KERNELDIR = /lib/modules/3.18.10-v7+/build

default:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) clean
