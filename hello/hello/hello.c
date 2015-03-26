#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void)
{
    printk("Hello, Roy. \n");
    return 0;
}

static void hello_exit(void)
{
    printk("byte. \n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("james.huang");
MODULE_DESCRIPTION("Raspberry Pi 2 B driver");
MODULE_VERSION("0.1");