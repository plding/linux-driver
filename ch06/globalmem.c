#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
MODULE_LICENSE("Dual BSD/GPL");

#define GLOBALMEM_SIZE  0x1000
#define GLOBALMEM_MAJOR 250

static int globalmem_major = GLOBALMEM_MAJOR;

struct globalmem_dev {
    struct cdev cdev;
    char mem[GLOBALMEM_SIZE];
};

static struct globalmem_dev *globalmem_devp;

static int globalmem_open(struct inode *inode, struct file *filp)
{
    filp->private_data = globalmem_devp;
    return 0;
}

static int globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t globalmem_read(struct file *filp, char __user *buf,
    size_t size, loff_t *ppos)
{
    unsigned long pos = *ppos;
    unsigned long count = size;
    struct globalmem_dev *dev = filp->private_data;

    if (pos > GLOBALMEM_SIZE)
        return count ? -ENXIO : 0;
    if (count > GLOBALMEM_SIZE - pos)
        count = GLOBALMEM_SIZE - pos;

    if (copy_to_user(buf, dev->mem + pos, count) == 0) {
        printk(KERN_INFO "read %lu bytes from %lu\n", count, pos);

        *ppos += count;
        return count;
    }

    return -EFAULT;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf,
    size_t size, loff_t *ppos)
{
    unsigned long pos = *ppos;
    unsigned long count = size;
    struct globalmem_dev *dev = filp->private_data;

    if (pos > GLOBALMEM_SIZE)
        return count ? -ENXIO : 0;
    if (count > GLOBALMEM_SIZE - pos)
        count = GLOBALMEM_SIZE - pos;

    if (copy_from_user(dev->mem + pos, buf, count) == 0) {
        printk(KERN_INFO "write %lu bytes from %lu\n", count, pos);

        *ppos += count;
        return count;
    }

    return -EFAULT;
}

static struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .read = globalmem_read,
    .write = globalmem_write,
    .open = globalmem_open,
    .release = globalmem_release,
};

static int __init globalmem_init(void)
{
    int result;
    dev_t devno = MKDEV(globalmem_major, 0);

    if (globalmem_major) {
        result = register_chrdev_region(devno, 1, "globalmem");
    } else {
        result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (result < 0)
        return result;

    globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
    if (globalmem_devp == NULL) {
        unregister_chrdev_region(devno, 1);
        return -ENOMEM;
    }
    memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

    cdev_init(&globalmem_devp->cdev, &globalmem_fops);
    cdev_add(&globalmem_devp->cdev, devno, 1);

    return 0;
}

static void __exit globalmem_exit(void)
{
    cdev_del(&globalmem_devp->cdev);
    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
}

module_init(globalmem_init);
module_exit(globalmem_exit);
