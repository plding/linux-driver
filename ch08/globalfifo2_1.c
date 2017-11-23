#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/poll.h>

#define GLOBALFIFO_SIZE     10
#define GLOBALFIFO_MAJOR    231
#define MEM_CLEAR           0x01

static int globalfifo_major = GLOBALFIFO_MAJOR;

struct globalfifo_dev {
    struct cdev cdev;
    unsigned int current_len;
    unsigned char mem[GLOBALFIFO_SIZE];
    struct mutex mutex;
    wait_queue_head_t r_wait;
    wait_queue_head_t w_wait;
};

struct globalfifo_dev *globalfifo_devp;

static int globalfifo_open(struct inode *inode, struct file *filp)
{
    filp->private_data = container_of(inode->i_cdev, struct globalfifo_dev, cdev);
    return 0;
}

static int globalfifo_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t globalfifo_read(struct file *filp, char __user *buf, size_t count,
    loff_t *ppos)
{
    int ret;
    struct globalfifo_dev *dev = filp->private_data;
    DECLARE_WAITQUEUE(wait, current);

    mutex_lock(&dev->mutex);
    add_wait_queue(&dev->r_wait, &wait);

    while (dev->current_len == 0) {
        if (filp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto out;
        }

        __set_current_state(TASK_INTERRUPTIBLE);
        mutex_unlock(&dev->mutex);

        schedule();
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
            goto out2;
        }

        mutex_lock(&dev->mutex);
    }

    if (count > dev->current_len)
        count = dev->current_len;

    if (copy_to_user(buf, dev->mem, count)) {
        ret = -EFAULT;
    } else {
        memmove(dev->mem, dev->mem + count, dev->current_len - count);
        dev->current_len -= count;
        ret = count;

        printk(KERN_INFO "read %zu bytes, current_len: %u\n", count, dev->current_len);
        wake_up_interruptible(&dev->w_wait);
    }

out:
    mutex_unlock(&dev->mutex);
out2:
    remove_wait_queue(&dev->r_wait, &wait);
    set_current_state(TASK_RUNNING);
    return ret;
}

static ssize_t globalfifo_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *ppos)
{
    int ret;
    struct globalfifo_dev *dev = filp->private_data;
    DECLARE_WAITQUEUE(wait, current);

    mutex_lock(&dev->mutex);
    add_wait_queue(&dev->w_wait, &wait);

    while (dev->current_len == GLOBALFIFO_SIZE) {
        if (filp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto out;
        }

        __set_current_state(TASK_INTERRUPTIBLE);
        mutex_unlock(&dev->mutex);

        schedule();
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
            goto out2;
        }

        mutex_lock(&dev->mutex);
    }

    if (count > GLOBALFIFO_SIZE - dev->current_len)
        count = GLOBALFIFO_SIZE - dev->current_len;

    if (copy_from_user(dev->mem + dev->current_len, buf, count)) {
        ret = -EFAULT;
    } else {
        dev->current_len += count;
        ret = count;

        printk(KERN_INFO "written %zu bytes, current_len: %u\n", count, dev->current_len);
        wake_up_interruptible(&dev->r_wait);
    }

out:
    mutex_unlock(&dev->mutex);
out2:
    remove_wait_queue(&dev->w_wait, &wait);
    set_current_state(TASK_RUNNING);
    return ret;
}

static long globalfifo_ioctl(struct file *filp, unsigned int cmd,
    unsigned long arg)
{
    struct globalfifo_dev *dev = filp->private_data;

    mutex_lock(&dev->mutex);

    switch (cmd) {

    case MEM_CLEAR:
        memset(dev->mem, 0, GLOBALFIFO_SIZE);
        printk(KERN_INFO "globalfifo is set to zero\n");
        break;

    default:
        return -EINVAL;
    }

    mutex_unlock(&dev->mutex);
    return 0;
}

static unsigned int globalfifo_poll(struct file *filp, poll_table *wait)
{
    unsigned int mask = 0;
    struct globalfifo_dev *dev = filp->private_data;

    mutex_lock(&dev->mutex);

    poll_wait(filp, &dev->r_wait, wait);
    poll_wait(filp, &dev->w_wait, wait);

    if (dev->current_len != 0)
        mask |= POLLIN | POLLRDNORM;
    if (dev->current_len != GLOBALFIFO_SIZE)
        mask |= POLLOUT | POLLWRNORM;

    mutex_unlock(&dev->mutex);

    return mask;
}

static const struct file_operations globalfifo_fops = {
    .owner = THIS_MODULE,
    .open = globalfifo_open,
    .release = globalfifo_release,
    .read = globalfifo_read,
    .write = globalfifo_write,
    .unlocked_ioctl = globalfifo_ioctl,
    .poll = globalfifo_poll,
};

static void globalfifo_setup_dev(struct globalfifo_dev *dev, int index)
{
    int err, devno = MKDEV(globalfifo_major, index);

    cdev_init(&dev->cdev, &globalfifo_fops);
    dev->cdev.owner = THIS_MODULE;
    err = cdev_add(&dev->cdev, devno, 1);
    if (err)
        printk(KERN_NOTICE "Error %d adding globalfifo%d\n", err, index);
}

static int __init globalfifo_init(void)
{
    int ret;
    dev_t devno;

    if (globalfifo_major) {
        devno = MKDEV(globalfifo_major, 0);
        ret = register_chrdev_region(devno, 1, "globalfifo");
    } else {
        ret = alloc_chrdev_region(&devno, 0, 1, "globalfifo");
        globalfifo_major = MAJOR(devno);
    }
    if (ret < 0)
        return ret;

    globalfifo_devp = kzalloc(sizeof(struct globalfifo_dev), GFP_KERNEL);
    if (!globalfifo_devp) {
        ret = -ENOMEM;
        goto fail_malloc;
    }

    mutex_init(&globalfifo_devp->mutex);

    init_waitqueue_head(&globalfifo_devp->r_wait);
    init_waitqueue_head(&globalfifo_devp->w_wait);

    globalfifo_setup_dev(globalfifo_devp, 0);
    return 0;

fail_malloc:
    unregister_chrdev_region(devno, 1);
    return ret;
}

static void __exit globalfifo_exit(void)
{
    cdev_del(&globalfifo_devp->cdev);
    kfree(globalfifo_devp);
    unregister_chrdev_region(MKDEV(globalfifo_major, 0), 1);
}

MODULE_AUTHOR("Ding Peilong");
MODULE_LICENSE("Dual BSD/GPL");

module_init(globalfifo_init);
module_exit(globalfifo_exit);
