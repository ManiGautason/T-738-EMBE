#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/mydev"

int main() {
    int fd;
    char buffer[16];
    ssize_t bytes_read;

    // Open the character device for reading
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the device file");
        return 1;
    }

    // Read the pulseCount from the device
    bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Failed to read from the device");
        close(fd);
        return 1;
    }

    // Null-terminate the received data
    buffer[bytes_read] = '\0';

    // Display the pulseCount
    printf("Pulse Count: %s", buffer);

    // Close the device file
    close(fd);

    return 0;
}
