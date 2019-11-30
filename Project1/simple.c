#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/gcd.h>
#include<linux/hash.h>

int simple_init(void)
{
    printk(KERN_INFO "loading Kernel Module\n");
    printk(KERN_INFO "%llu\n", GOLDEN_RATIO_PRIME);
    return 0;
}



void simple_exit(void)
{
    printk("%lu",gcd(3300,24));
    printk(KERN_INFO "Removing Kernel Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Gavin");