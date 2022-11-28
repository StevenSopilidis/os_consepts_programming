#include "linux/init.h"
#include "linux/kernel.h"
#include "linux/module.h"

// function called for initializing module
int simple_init(void)
{
    printk(KERN_INFO, "Loading Kernel module\n");
    return 0;
}

// function called when moduler removed
void simple_exit(void)
{
    printk(KERN_INFO, "Loading Kernel module\n");
    return 0;
}


// macros for registering module entry and exit points
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENCE("GPL");
MODULE_DESCRIPTION("Simple module");
MODULE_AUTHOR("STEVEN_SOPILIDIS");