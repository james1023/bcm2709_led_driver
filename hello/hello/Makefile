obj-m := hello.o

KDIR := /home/workspace/opt/rpi/linux/include

default:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules
