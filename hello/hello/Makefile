obj-m := hello.o

KDIR := /home/workspace/opt/rpi/linux

default:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
