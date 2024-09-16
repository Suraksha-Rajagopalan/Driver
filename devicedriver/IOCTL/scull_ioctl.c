#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "scull.h"

MODULE_LICENSE("GPL");

loff_t scull_max_size = 8 * 1048576; // 8 MB max file size

int scull_open(struct inode *inode, struct file *filp) {
    struct scull_dev *dev;

    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filp->private_data = dev;

    // Assuming the semaphore is initialized somewhere else
    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;

    // Call scull_trim if O_WRONLY
    if ((filp->f_flags & O_ACCMODE) == O_WRONLY) {
        scull_trim(dev); /* ignore errors */
    }

    up(&dev->sem);
    return 0;
}

long scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int err = 0, tmp;
    int retval = 0;
    struct scull_dev *dev = filp->private_data;

    if (_IOC_TYPE(cmd) != SCULL_IOC_MAGIC) return -ENOTTY;
    if (_IOC_NR(cmd) > SCULL_IOC_MAXNR) return -ENOTTY;

    if (_IOC_DIR(cmd) & _IOC_READ)
        err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
    if (err) return -EFAULT;

    switch(cmd) {
        case SCULL_IOCRESET:
            dev->quantum = QUANTUM;
            dev->qset = QSET;
            break;
        case SCULL_IOCSQUANTUM:
            retval = __get_user(dev->quantum, (int __user *)arg);
            break;
        case SCULL_IOCTQUANTUM:
            dev->quantum = arg;
            break;
        case SCULL_IOCGQUANTUM:
            retval = __put_user(dev->quantum, (int __user *)arg);
            break;
        case SCULL_IOCQQUANTUM:
            return dev->quantum;
        case SCULL_IOCXQUANTUM:
            retval = __get_user(tmp, (int __user *)arg);
            if (retval == 0)
                retval = __put_user(dev->quantum, (int __user *)arg);
            break;
        case SCULL_IOCHQUANTUM:
            retval = __get_user(dev->quantum, (int __user *)arg);
            if (retval == 0)
                dev->quantum = arg;
            break;
        case SCULL_IOCSQSET:
            retval = __get_user(dev->qset, (int __user *)arg);
            break;
        case SCULL_IOCTQSET:
            dev->qset = arg;
            break;
        case SCULL_IOCGQSET:
            retval = __put_user(dev->qset, (int __user *)arg);
            break;
        case SCULL_IOCQQSET:
            return dev->qset;
        case SCULL_IOCXQSET:
            retval = __get_user(tmp, (int __user *)arg);
            if (retval == 0)
                retval = __put_user(dev->qset, (int __user *)arg);
            break;
        case SCULL_IOCHQSET:
            retval = __get_user(dev->qset, (int __user *)arg);
            if (retval == 0)
                dev->qset = arg;
            break;
        case SCULL_P_IOCTSIZE:
            /* wait until the system is ready to provide a reasonable value */
            return -ENOTTY;
        case SCULL_P_IOCQSIZE:
            return scull_max_size;
        case SCULL_IOCGMAXSIZE:
            retval = __put_user(scull_max_size, (loff_t __user *)arg);
            break;
        case SCULL_IOCSMAXSIZE:
            retval = __get_user(scull_max_size, (loff_t __user *)arg);
            break;
        default:  /* redundant, as cmd was checked against MAXNR */
            return -ENOTTY;
    }
    return retval;
}

void scull_exit(void) {
    int i;

    /* Get rid of our char dev entries */
    if (scull_devices) {
        for (i = 0; i < scull_nr_devs; ++i) {
            if (scull_devices[i].data)
                scull_trim(&scull_devices[i]);
        }
        kfree(scull_devices);
    }

    /* cleanup_module is never called if registering failed */
    unregister_chrdev_region(MKDEV(scull_major, scull_minor), scull_nr_devs);

    /* and call the cleanup functions for friend devices */
    if (scull_foo)
    if (scull_foo)
        scull_trim(scull_foo);
    if (scull_bar)
        scull_trim(scull_bar);
}

module_exit(scull_exit);


