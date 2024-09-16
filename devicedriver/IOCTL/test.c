#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "scull.h"

int main() {
    int fd;
    loff_t max_size;
    loff_t new_max_size = 8 * 1048576; // 8 MB

    // Open the device file
    fd = open("/dev/scull0", O_RDWR);
    if (fd == -1) {
        perror("Failed to open /dev/scull0");
        return EXIT_FAILURE;
    }

    // Get the current max file size
    if (ioctl(fd, SCULL_IOCGMAXSIZE, &max_size) == -1) {
        perror("Failed to get max file size");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Current max file size: %lld bytes\n", max_size);

    // Set a new max file size
    if (ioctl(fd, SCULL_IOCSMAXSIZE, &new_max_size) == -1) {
        perror("Failed to set new max file size");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("New max file size set to: %lld bytes\n", new_max_size);

    // Get the new max file size to verify
    if (ioctl(fd, SCULL_IOCGMAXSIZE, &max_size) == -1) {
        perror("Failed to get max file size");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Verified new max file size: %lld bytes\n", max_size);

    // Clean up and close the device file
    close(fd);
    return EXIT_SUCCESS;
}

