#KERN_DIR = /work/system/linux-2.6.22.6

all:
	make -C $(KERNELDIR) M=`pwd` modules 

clean:
	make -C $(KERNELDIR M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= first_drv.o
