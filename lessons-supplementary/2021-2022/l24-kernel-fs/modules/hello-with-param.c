#include <linux/module.h>  // modules macros
#include <linux/moduleparam.h>
#include <linux/kernel.h>  // basic kernel routines

int some_flag_value = 0;

module_param(some_flag_value, int, 0600);

int
init_module()
{
    if (some_flag_value) {
        printk("Module created with some non-zero flag\n");
    }
    else {
        printk("Module created with zero flag\n");
    }
    return 0; // on success
}

void
cleanup_module()
{
    printk("Unloading kernel module 'hello'\n");
}

MODULE_LICENSE("GPL");
