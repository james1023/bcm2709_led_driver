#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>


volatile unsigned long *gpio;

#define BCM2709_PERI_BASE        0x3F000000
#define GPIO_BASE                (BCM2709_PERI_BASE + 0x200000)

#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

static struct class *firstdrv_class;

static int first_drv_open(struct inode *inode, struct file *file)
{
	printk("first_drv_open\n");
    INP_GPIO(4); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(4);
    
	return 0;
}

static ssize_t first_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;

	printk("first_drv_write\n");

	copy_from_user(&val, buf, count); //	copy_to_user();

	if (val == 1)
	{
		GPIO_SET = 1<<4;
	}
	else
	{
		GPIO_CLR = 1<<4;
	}
	
	return 0;
}

static struct file_operations first_drv_fops = {
    .owner  =   THIS_MODULE,
    .open   =   first_drv_open,     
	.write	=	first_drv_write,	   
};


int major;
static int first_drv_init(void)
{
	major = register_chrdev(0, "first_drv", &first_drv_fops);

	firstdrv_class = class_create(THIS_MODULE, "firstdrv");

    device_create(firstdrv_class, NULL, MKDEV(major, 0), NULL, "xyz");

	gpio = (volatile unsigned long *)ioremap(GPIO_BASE, 16);
	
    return 0;
}

static void first_drv_exit(void)
{
	unregister_chrdev(major, "first_drv");

	device_destroy(firstdrv_class, MKDEV(major, 0));
	class_destroy(firstdrv_class);
	iounmap(gpio);
}

module_init(first_drv_init);
module_exit(first_drv_exit);


MODULE_LICENSE("GPL");

