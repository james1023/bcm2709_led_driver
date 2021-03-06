

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>


volatile unsigned int *gpio;

// rpi1
// #define GPIO_BASE            0X20200000

// rpi2
#define GPIO_BASE               0x3F200000
#define BLOCK_SIZE              4096

// GPIO macros
#define INP_GPIO_04(b)          *(volatile unsigned int *)(0x00000000+(unsigned int)b) &= (unsigned int)0xFFFF8FFF
#define OUT_GPIO_04(b)          *(volatile unsigned int *)(0x00000000+(unsigned int)b) |= (unsigned int)0x00001000

#define GPIO_SET_04(b)          *(volatile unsigned int *)(0x0000001C+(unsigned int)b) |= 0x10
#define GPIO_CLR_04(b)          *(volatile unsigned int *)(0x00000028+(unsigned int)b) |= 0x10


struct timer_list led_timer;
char kbledstatus = 0;
int status = 0;

static void hello_timer(unsigned long ptr)
{
    printk("jiffies + HZ = %ld + %ld. \n", jiffies, HZ);
    
    //printk("jiffies + HZ = %ld + %ld, gpio=%p, gpio_set=%p, gpio_clr=%p. \n", jiffies, HZ, gpio, (1+gpio), 0x00000001+gpio);
    
    if (status == 0) {
        GPIO_SET_04(gpio);
        //*(gpio+7) = 1<<4;
        status = 1;
    }
    else {
        GPIO_CLR_04(gpio);
        //*(gpio+10) = 1<<4;
        status = 0;
    }
    
    led_timer.expires = jiffies + HZ;
    add_timer(&led_timer);
}

static int hello_init(void)
{
    printk("Hello, LED World! \n");
    
    gpio = (volatile unsigned int *)ioremap(GPIO_BASE, 16);
    
    printk("gpio=%#x. \n", gpio);
    
    INP_GPIO_04(gpio);
    OUT_GPIO_04(gpio);
    
    init_timer(&led_timer);
    led_timer.function = hello_timer;
    led_timer.data = (unsigned long)&kbledstatus;
    led_timer.expires = jiffies + HZ;
    add_timer(&led_timer);
    
    return 0;
}

static void hello_exit(void)
{
    del_timer(&led_timer);
    iounmap(gpio);
    printk("byte. \n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("james.huang");
MODULE_DESCRIPTION("Raspberry Pi 2 B driver");
MODULE_VERSION("0.1");