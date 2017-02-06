#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GLOBALMEM_SIZE  0x1000
#define GLOBALMEM_MAJOR 250

static int globalmem_major = GLOBALMEM_MAJOR;

struct globalmem_dev {
    struct cdev cdev;
    unsigned char mem[GLOBALMEM_SIZE];
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

static ssize_t globalmem_read(struct file *filp, char __user *buf,
    size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if (p >= GLOBALMEM_SIZE)
        return 0;
    if (count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;

    if (copy_to_user(buf, dev->mem + p, count))
        ret = -EFAULT;
    else {
        *ppos += count;
        ret = count;

        printk(KERN_INFO "read %u bytes from %lu\n", count, p);
    }

    return ret;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf,
    size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if (p >= GLOBALMEM_SIZE)
        return 0;
    if (count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;

    if (copy_from_user(dev->mem + p, buf, count))
        ret = -EFAULT;
    else {
        *ppos += count;
        ret = count;

        printk(KERN_INFO "written %u bytes from %lu\n", count, p);
    }

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
        ret = filp->f_pos = offset;
        break;

    case 1:
        if (filp->f_pos + offset < 0 ||
            filp->f_pos + offset > GLOBALMEM_SIZE)
        {
            ret = -EINVAL;
            break;
        }
        ret = (filp->f_pos += offset);
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
    case 0x01:
        memset(dev->mem, 0, GLOBALMEM_SIZE);
        printk(KERN_INFO "globalmem is set to zero");
        break;

    default:
        return -EINVAL;
    }

    return 0;
}

static const struct file_operations globalmem_fops = {
    .owner = THIS_MODULE,
    .open = globalmem_open,
    .read = globalmem_read,
    .write = globalmem_write,
    .llseek = globalmem_llseek,
    .unlocked_ioctl = globalmem_ioctl,
    .release = globalmem_release,
};

int globalmem_init(void)
{
    int result;
    dev_t devno = MKDEV(globalmem_major, 0);

    if (globalmem_major)
        result = register_chrdev_region(devno, 1, "globalmem");
    else {
        result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
        globalmem_major = MAJOR(devno);
    }
    if (result < 0)
        return result;

    globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
    if (!globalmem_devp) {
        unregister_chrdev_region(devno, 1);
        return -ENOMEM;
    }
    memset(globalmem_devp, 0, sizeof(struct globalmem_dev));

    cdev_init(&globalmem_devp->cdev, &globalmem_fops);
    cdev_add(&globalmem_devp->cdev, devno, 1);

    return 0;
}

void globalmem_exit(void)
{
    cdev_del(&globalmem_devp->cdev);
    kfree(globalmem_devp);
    unregister_chrdev_region(MKDEV(globalmem_major, 0), 1);
}

MODULE_AUTHOR("plding");
MODULE_LICENSE("Dual BSD/GPL");

module_init(globalmem_init);
module_exit(globalmem_exit);
