#include <linux/kernel.h>   // For printk(), types, etc.
#include <linux/uaccess.h>  // For copy_from_user, copy_to_user
#include <linux/fs.h>       // For struct file_operations
#include <linux/types.h>    // For ssize_t, size_t, loff_t
#include <linux/slab.h>     // For kmalloc, kfree
#include "scull.h"          // For struct scull_dev, struct scull_qset, etc.

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    struct scull_dev *dev = filp->private_data;
    struct scull_qset *dptr;
    int quantum = dev->quantum, qset = dev->qset;
    int itemsize = quantum * qset;
    int item, s_pos, q_pos, rest;
    ssize_t retval = -ENOMEM; /* value used in "goto out" statements */

    if (mutex_lock_interruptible(&dev->lock))
        return -ERESTARTSYS;

    /* Check if we exceed max file size */
    if (*f_pos + count > scull_max_size) {
        count = scull_max_size - *f_pos;
        if (count <= 0) {
            retval = -EFBIG; // File too large
            goto out;
        }
    }

    /* find listitem, qset index and offset in the quantum */
    item = (long)*f_pos / itemsize;
    rest = (long)*f_pos % itemsize;
    s_pos = rest / quantum; q_pos = rest % quantum;

    /* follow the list up to the right position */
    dptr = scull_follow(dev, item);
    if (dptr == NULL)
        goto out;
    if (!dptr->data) {
        dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
        if (!dptr->data)
            goto out;
        memset(dptr->data, 0, qset * sizeof(char *));
    }
    if (!dptr->data[s_pos]) {
        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
        if (!dptr->data[s_pos])
            goto out;
    }
    /* write only up to the end of this quantum */
    if (count > quantum - q_pos)
        count = quantum - q_pos;

    if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {
        retval = -EFAULT;
        goto out;
    }
    *f_pos += count;
    retval = count;

    /* update the size */
    if (dev->size < *f_pos)
        dev->size = *f_pos;

  out:
    mutex_unlock(&dev->lock);
    printk(KERN_INFO "scull_write called\n");
    return retval;
}

