#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

#define GLOBALFIFO_MAJOR 200
#define GLOBALFIFO_SIZE  0x1000

static int globalfifo_major = GLOBALFIFO_MAJOR;

struct globalfifo_dev {
    struct cdev cdev;
    unsigned long current_len;
    unsigned char mem[GLOBALFIFO_SIZE];
    wait_queue_head_t r_wait;
    wait_queue_head_t w_wait;
} *globalfifo_devp;

static int globalfifo_open(struct inode *inode, struct file *filp)
{
    filp->private_data = globalfifo_devp;
    return 0;
}

static int globalfifo_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t globalfifo_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_ops)
{
    int ret = 0;
    struct globalfifo_dev *dev = filp->private_data;

    wait_event_interruptible(dev->r_wait, dev->current_len != 0);

    if (count > dev->current_len)
        count = dev->current_len;

    if (copy_to_user(buf, dev->mem, count)) {
        ret = -EFAULT;
    } else {
        dev->current_len -= count;
        memcpy(dev->mem, dev->mem + count, dev->current_len);
        printk(KERN_INFO "read %zu bytes, current_len: %lu\n", count, dev->current_len);

        wake_up_interruptible(&dev->w_wait);

        ret = count;
    }

    return ret;
}

static ssize_t globalfifo_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_ops)
{
    int ret = 0;
    struct globalfifo_dev *dev = filp->private_data;

    wait_event_interruptible(dev->w_wait, dev->current_len < GLOBALFIFO_SIZE);

    if (count > GLOBALFIFO_SIZE - dev->current_len)
        count = GLOBALFIFO_SIZE - dev->current_len;

    if (copy_from_user(dev->mem, buf, count)) {
        ret = -EFAULT;
    } else {
        dev->current_len += count;
        printk(KERN_INFO "written %zu bytes, current_len: %lu\n", count, dev->current_len);

        wake_up_interruptible(&dev->r_wait);

        ret = count;
    }

    return ret;
}

static const struct file_operations globalfifo_fops = {
    .owner = THIS_MODULE,
    .open = globalfifo_open,
    .release = globalfifo_release,
    .read = globalfifo_read,
    .write = globalfifo_write,
};

static void globalfifo_setup_cdev(struct globalfifo_dev *dev, int index)
{
    int ret;
    dev_t devno = MKDEV(globalfifo_major, index);

    cdev_init(&dev->cdev, &globalfifo_fops);
    dev->cdev.owner = THIS_MODULE;
    ret = cdev_add(&dev->cdev, devno, 1);
    if (ret)
        printk(KERN_WARNING "Error %d on add globalfifo%d\n", ret, index);
}

static int __init globalfifo_init(void)
{
    int ret;
    dev_t devno = MKDEV(globalfifo_major, 0);

    if (globalfifo_major) {
        ret = register_chrdev_region(devno, 1, "globalfifo");
    } else {
        ret = alloc_chrdev_region(&devno, 0, 1, "globalfifo");
        globalfifo_major = MAJOR(devno);
    }
    if (ret < 0)
        return ret;

    globalfifo_devp = kzalloc(sizeof(struct globalfifo_dev), GFP_KERNEL);
    if (!globalfifo_devp) {
        unregister_chrdev_region(devno, 1);
        return -ENOMEM;
    }

    globalfifo_setup_cdev(globalfifo_devp, 0);
    init_waitqueue_head(&globalfifo_devp->r_wait);
    init_waitqueue_head(&globalfifo_devp->w_wait);

    return 0;
}

static void __exit globalfifo_exit(void)
{
    cdev_del(&globalfifo_devp->cdev);
    kfree(globalfifo_devp);
    unregister_chrdev_region(MKDEV(globalfifo_major, 0), 1);
}

MODULE_AUTHOR("plding");
MODULE_LICENSE("Dual BSD/GPL");

module_init(globalfifo_init);
module_exit(globalfifo_exit);
