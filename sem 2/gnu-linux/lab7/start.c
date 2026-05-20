#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
    printk("Hello, world - this is the kernel speaking\n");
    return 0;
}

MODULE_LICENSE("GPL");
