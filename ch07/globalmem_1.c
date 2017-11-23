#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GLOBALMEM_SIZE  0x100000
#define MEM_CLEAR       0x01
#define GLOBALMEM_MAJOR 230

static int globalmem_major = GLOBALMEM_MAJOR;

struct globalmem_dev {
    struct cdev cdev;
    unsigned char mem[GLOBALMEM_SIZE];
    struct mutex mutex;
};

struct globalmem_dev *globalmem_devp;

static int globalmem_open(struct inode *inode, struct file *filp)
{
    filp->private_data = globalmem_devp;
    return 0;
}

static int globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count,
    loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int size = count;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if (p > GLOBALMEM_SIZE)
        return 0;
    if (size > GLOBALMEM_SIZE - p)
        size = GLOBALMEM_SIZE - p;

    mutex_lock(&dev->mutex);

    if (copy_to_user(buf, dev->mem + p, size)) {
        ret = -EFAULT;
    } else {
        *ppos += size;
        ret = size;

        printk(KERN_INFO "read %u bytes from %lu\n", size, p);
    }

    mutex_unlock(&dev->mutex);

    return ret;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int size = count;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if (p > GLOBALMEM_SIZE)
        return 0;
    if (size > GLOBALMEM_SIZE - p)
        size = GLOBALMEM_SIZE - p;

    mutex_lock(&dev->mutex);

    if (copy_from_user(dev->mem + p, buf, size)) {
        ret = -EFAULT;
    } else {
        *ppos += size;
        ret = size;

        printk(KERN_INFO "written %u bytes from %lu\n", size, p);
    }

    mutex_unlock(&dev->mutex);

    return ret;
}

static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
    loff_t ret = 0;

    switch (orig) {

    case 0:
        if (offset < 0 || offset > GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos = offset;
        ret = filp->f_pos;
        break;

    case 1:
        if (filp->f_pos + offset < 0 || filp->f_pos + offset > GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos += offset;
        ret = filp->f_pos;
        break;

    case 2:
        if (offset > 0 || -offset >= GLOBALMEM_SIZE) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos = GLOBALMEM_SIZE + offset;
        ret = filp->f_pos;
        break;

    default:
        ret = -EINVAL;
        break;
    }

    return ret;
}

static long globalmem_ioctl(struct file *filp, unsigned int cmd,
    unsigned long arg)
{
    struct globalmem_dev *dev = filp->private_data;

    switch (cmd) {

    case MEM_CLEAR:
        memset(dev->mem, 0, GLOBALMEM_SIZE);
        printk(KERN_INFO "globalmem is set to zero\n");
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .open = globalmem_open,
    .release = globalmem_release,
    .read = globalmem_read,
    .write = globalmem_write,
    .llseek = globalmem_llseek,
    .unlocked_ioctl = globalmem_ioctl,
};

static void globalmem_setup_dev(struct globalmem_dev *dev, int index)
{
    int err, devno = MKDEV(globalmem_major, index);

    cdev_init(&dev->cdev, &globalmem_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err)
        printk(KERN_NOTICE "Error %d adding globalmem%d\n", err, index);
}

static int __init globalmem_init(void)
{
    int ret;
    dev_t devno;

    if (globalmem_major) {
        devno = MKDEV(globalmem_major, 0);
        ret = register_chrdev_region(devno, 1, "globalmem");
    } else {
        ret = alloc_chrdev_region(&devno, 0, 1, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (ret < 0)
        return ret;

    globalmem_devp = kzalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
    if (!globalmem_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    mutex_init(&globalmem_devp->mutex);

    globalmem_setup_dev(globalmem_devp, 0);
    return 0;

fail_malloc:
    unregister_chrdev_region(devno, 1);
    return ret;
}

static void __exit globalmem_exit(void)
{
    cdev_del(&globalmem_devp->cdev);
    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
}

MODULE_AUTHOR("Ding Peilong");
MODULE_LICENSE("Dual BSD/GPL");

module_init(globalmem_init);
module_exit(globalmem_exit);
