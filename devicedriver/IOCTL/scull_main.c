#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "scull.h"

MODULE_LICENSE("GPL");

loff_t scull_max_size = 8 * 1048576; // 8 MB max file size

struct scull_qset *scull_follow(struct scull_dev *dev, int n) {
    struct scull_qset *qs = dev->data;

    if (!qs) {
        qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
        if (qs == NULL)
            return NULL;
        memset(qs, 0, sizeof(struct scull_qset));
    }

    while (n--) {
        if (!qs->next) {
            qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
            if (qs->next == NULL)
                return NULL;
            memset(qs->next, 0, sizeof(struct scull_qset));
        }
        qs = qs->next;
    }
    return qs;
}

// Initialization and cleanup functions

static int __init scull_init(void) {
    printk(KERN_INFO "SCULL: Module loaded\n");
    // Register the device here
    return 0; // Return success
}

static void __exit scull_exit(void) {
    printk(KERN_INFO "SCULL: Module unloaded\n");
    // Unregister the device here
}

module_init(scull_init);
module_exit(scull_exit);

