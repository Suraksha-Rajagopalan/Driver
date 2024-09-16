#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define TEST_FILE "/dev/memfile"
#define TEST_STRING "Hello, memory file system!"

int main() {
    int fd;
    ssize_t bytes_written, bytes_read;
    char read_buf[256];

    // Open the file
    fd = open(TEST_FILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write to the file
    bytes_written = write(fd, TEST_STRING, strlen(TEST_STRING));
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Written %zd bytes to the file: %s\n", bytes_written, TEST_STRING);

    // Seek to the beginning of the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Read from the file
    bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes_read == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the read buffer and print the contents
    read_buf[bytes_read] = '\0';
    printf("Read %zd bytes from the file: %s\n", bytes_read, read_buf);

    // Close the file
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}

