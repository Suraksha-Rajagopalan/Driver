#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");

#define DEVICE_NAME "memfile"
#define CLASS_NAME "memfile_class"
#define MAX_FILE_SIZE (8 * 1024)  // Maximum file size in bytes
#define MAX_FILES 10

// Define a structure to represent each file in memory
struct mem_file {
    char *data;            // Pointer to the data buffer
    size_t size;           // Current size of the file
    size_t capacity;       // Capacity of the data buffer
};

// Array of mem_file structures to represent multiple files in memory
static struct mem_file files[MAX_FILES];

static int major_number;
static struct class* memfile_class = NULL;
static struct device* memfile_device = NULL;

// Initialize the memory file system
static void init_mem_fs(void) {
    int i;
    for (i = 0; i < MAX_FILES; ++i) {
        files[i].data = NULL;
        files[i].size = 0;
        files[i].capacity = 0;
    }
    printk(KERN_INFO "Driver Registered\n");
}

// Cleanup the memory file system
static void cleanup_mem_fs(void) {
    int i;
    for (i = 0; i < MAX_FILES; ++i) {
        if (files[i].data) {
            kfree(files[i].data);
            files[i].data = NULL;
        }
        files[i].size = 0;
        files[i].capacity = 0;
    }
    printk(KERN_INFO "Driver unregistered\n");
}

// Open a file in the memory file system
static int open_mem_file(struct inode *inode, struct file *filp) {
    int i;
    struct mem_file *file = kmalloc(sizeof(struct mem_file), GFP_KERNEL);
    if (!file)
        return -ENOMEM;

    // return the first available file slot
    for (i = 0; i < MAX_FILES; ++i) {
        if (!files[i].data) {
            // Initialize the file structure
            file->data = kmalloc(MAX_FILE_SIZE, GFP_KERNEL);
            if (!file->data) {
                kfree(file);
                return -ENOMEM;
            }
            file->size = 0;
            file->capacity = MAX_FILE_SIZE;

            // Store the file structure pointer in filp
            filp->private_data = file;
            return 0;
        }
    }

    // No available file slots
    kfree(file->data);
    kfree(file);
    return -ENOSPC;
}

// Release a file in the memory file system
static int release_mem_file(struct inode *inode, struct file *filp) {
    struct mem_file *file = filp->private_data;

    // Free the memory allocated for the file
    kfree(file->data);
    kfree(file);
    return 0;
}

// Read data from a file in the memory file system
static ssize_t read_mem_file(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
    struct mem_file *file = filp->private_data;

    // Check if file is empty or reading beyond EOF
    if (file->size == 0 || *f_pos >= file->size)
        return 0;

    // Adjust count to prevent reading beyond EOF
    if (*f_pos + count > file->size)
        count = file->size - *f_pos;

    // Copy data to user space
    if (copy_to_user(buf, file->data + *f_pos, count))
        return -EFAULT;

    // Update file position
    *f_pos += count;

    return count;
}

// Write data to a file in the memory file system
static ssize_t write_mem_file(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
    struct mem_file *file = filp->private_data;

    // Check if file size exceeds the maximum limit
    if (*f_pos + count > MAX_FILE_SIZE)
        return -ENOSPC;

    // Copy data from user space
    if (copy_from_user(file->data + *f_pos, buf, count))
        return -EFAULT;

    // Update file position and size
    *f_pos += count;
    if (*f_pos > file->size)
        file->size = *f_pos;

    return count;
}

// Move the file position indicator to the specified position
static loff_t llseek_mem_file(struct file *filp, loff_t offset, int whence) {
    loff_t newpos;

    switch (whence) {
        case 0: // SEEK_SET
            newpos = offset;
            break;

        case 1: // SEEK_CUR
            newpos = filp->f_pos + offset;
            break;

        case 2: // SEEK_END
            newpos = files[0].size + offset;
            break;

        default: // Should not occur
            return -EINVAL;
    }

    if (newpos < 0 || newpos > MAX_FILE_SIZE)
        return -EINVAL;

    filp->f_pos = newpos;
    return newpos;
}

// File operations for the memory file system
static const struct file_operations mem_file_ops = {
    .read = read_mem_file,
    .write = write_mem_file,
    .open = open_mem_file,
    .release = release_mem_file,
    .llseek = llseek_mem_file, // Add llseek member
};

// Initialize the memory file system
static int __init init_memfs(void) {
    init_mem_fs();

    major_number = register_chrdev(0, DEVICE_NAME, &mem_file_ops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "Registered correctly with major number %d\n", major_number);

    memfile_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(memfile_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(memfile_class);
    }
    printk(KERN_INFO "Device class registered correctly\n");

    memfile_device = device_create(memfile_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(memfile_device)) {
        class_destroy(memfile_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(memfile_device);
    }
    printk(KERN_INFO "Device class created correctly: /dev/%s\n", DEVICE_NAME);

    return 0;
}

// Cleanup the memory file system
static void __exit cleanup_memfs(void) {
    device_destroy(memfile_class, MKDEV(major_number, 0));
    class_unregister(memfile_class);
    class_destroy(memfile_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    cleanup_mem_fs();
}

module_init(init_memfs);
module_exit(cleanup_memfs);

MODULE_AUTHOR("Suraksha");
MODULE_DESCRIPTION("A simple memory file system module");

