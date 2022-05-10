#include <linux/module.h>  // modules macros
#include <linux/kernel.h>  // basic kernel routines

int
init_module()
{
    printk("Hello from newly created module!\n");
    return 0; // on success
}

void
cleanup_module()
{
    printk("Unloading kernel module 'hello'\n");
}

MODULE_LICENSE("My Great License to use only in Russia");


