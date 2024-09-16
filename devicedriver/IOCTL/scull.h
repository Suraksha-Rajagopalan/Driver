#ifndef SCULL_H
#define SCULL_H

#include <linux/ioctl.h>
#include <linux/mutex.h>
#include <linux/types.h> // For loff_t

#define SCULL_IOC_MAGIC 'k'
#define SCULL_IOCRESET _IO(SCULL_IOC_MAGIC, 0)
#define SCULL_IOCSQUANTUM _IOW(SCULL_IOC_MAGIC, 1, int)
#define SCULL_IOCTQUANTUM _IO(SCULL_IOC_MAGIC, 2)
#define SCULL_IOCGQUANTUM _IOR(SCULL_IOC_MAGIC, 3, int)
#define SCULL_IOCQQUANTUM _IO(SCULL_IOC_MAGIC, 4)
#define SCULL_IOCXQUANTUM _IOWR(SCULL_IOC_MAGIC, 5, int)
#define SCULL_IOCHQUANTUM _IO(SCULL_IOC_MAGIC, 6)
#define SCULL_IOCSQSET _IOW(SCULL_IOC_MAGIC, 7, int)
#define SCULL_IOCTQSET _IO(SCULL_IOC_MAGIC, 8)
#define SCULL_IOCGQSET _IOR(SCULL_IOC_MAGIC, 9, int)
#define SCULL_IOCQQSET _IO(SCULL_IOC_MAGIC, 10)
#define SCULL_IOCXQSET _IOWR(SCULL_IOC_MAGIC, 11, int)
#define SCULL_IOCHQSET _IO(SCULL_IOC_MAGIC, 12)
#define SCULL_P_IOCTSIZE _IO(SCULL_IOC_MAGIC, 13)
#define SCULL_P_IOCQSIZE _IO(SCULL_IOC_MAGIC, 14)
#define SCULL_IOCGMAXSIZE _IOR(SCULL_IOC_MAGIC, 15, loff_t)
#define SCULL_IOCSMAXSIZE _IOW(SCULL_IOC_MAGIC, 16, loff_t)

#define SCULL_IOC_MAXNR 16

struct scull_qset {
    void **data;
    struct scull_qset *next;
};

struct scull_dev {
    struct scull_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    unsigned int access_key;
    struct mutex lock;
};

extern loff_t scull_max_size;

struct scull_qset *scull_follow(struct scull_dev *dev, int n);

#endif // SCULL_H

