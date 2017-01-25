#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static char *name = "dissecting Linux Device Driver";
static int num = 4000;

static int __init book_init(void)
{
    printk(KERN_INFO "book name: %s\n", name);
    printk(KERN_INFO "book num: %d\n", num);
    return 0;
}

static void __exit book_exit(void)
{
    printk(KERN_INFO "Book module exit\n");
}

module_init(book_init);
module_exit(book_exit);
module_param(name, charp, S_IRUGO);
module_param(num, int, S_IRUGO);
