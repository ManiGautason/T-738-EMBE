// #include <stdio.h>
// #include <stdlib.h>
// #include <fcntl.h>
// #include <poll.h>
// #include <unistd.h>
// #include <string.h>

// int main(void) {
//     // Enable Encoder pin
//     int encoder_pin = open("/sys/class/gpio/export", O_WRONLY);
//     write(encoder_pin, "20", 2);
//     close(encoder_pin);

//     // Set Encoder pin as input
//     encoder_pin = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
//     write(encoder_pin, "in", 2);
//     close(encoder_pin);

//     // Set Encoder trigger
//     encoder_pin = open("/sys/class/gpio/gpio20/edge", O_WRONLY);
//     write(encoder_pin, "both", 2);
//     close(encoder_pin);

//     // Enable Output pin
//     int Output_pin = open("/sys/class/gpio/export", O_WRONLY);
//     write(Output_pin, "21", 2);
//     close(Output_pin);

//     // Set Output pin as output
//     Output_pin = open("/sys/class/gpio/gpio21/direction", O_WRONLY);
//     write(Output_pin, "out", 3); // Should be "out", not "in"
//     close(Output_pin);

//     struct pollfd mypoll;
//     memset(&mypoll, 0, sizeof(mypoll));
//     mypoll.fd = open("/sys/class/gpio/gpio20/value", O_RDONLY); // Open value file directly
//     mypoll.events = POLLPRI; // Use POLLPRI to detect edge changes

//     char Encoder_state;
//     char prevEncoder_state = '0'; // Initialize to LOW state
//     char buffer[1]; 
//     while (1) {
//         poll(&mypoll, 1, -1); // Wait indefinitely for an event

//         if (mypoll.revents & POLLPRI) {
//             lseek(mypoll.fd, 0, SEEK_SET); // Move the file pointer to the beginning
//             read(encoder_pin, &Encoder_state, 1);
//             buffer[0] = Encoder_state;

//             // Mirror the encoder state to the output GPIO pin
//             Output_pin = open("/sys/class/gpio/gpio21/value", O_WRONLY);
//             write(Output_pin, buffer, 1);
//             close(Output_pin);
//         }
//     }

//     Output_pin = open("/sys/class/gpio/unexport", O_WRONLY);
//     write(Output_pin, "21", 2);
//     close(Output_pin);

//     encoder_pin = open("/sys/class/gpio/unexport", O_WRONLY);
//     write(encoder_pin, "20", 2);
//     close(encoder_pin);

//     // You should add code to clean up and close GPIO pins before exiting
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    // Enable Encoder pin
    int encoder_pin = open("/sys/class/gpio/export", O_WRONLY);
    if (encoder_pin == -1) {
        perror("Error opening gpio export for encoder");
        return 1;
    }
    write(encoder_pin, "20", 2);
    close(encoder_pin);

    // Set Encoder pin as input
    encoder_pin = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
    if (encoder_pin == -1) {
        perror("Error opening gpio20 direction");
        return 1;
    }
    write(encoder_pin, "in", 2);
    close(encoder_pin);

    // Set Encoder trigger
    encoder_pin = open("/sys/class/gpio/gpio20/edge", O_WRONLY);
    if (encoder_pin == -1) {
        perror("Error opening gpio20 edge");
        return 1;
    }
    write(encoder_pin, "both", 2);
    close(encoder_pin);

    // Enable Output pin
    int Output_pin = open("/sys/class/gpio/export", O_WRONLY);
    if (Output_pin == -1) {
        perror("Error opening gpio export for output");
        return 1;
    }
    write(Output_pin, "21", 2);
    close(Output_pin);

    // Set Output pin as output
    Output_pin = open("/sys/class/gpio/gpio21/direction", O_WRONLY);
    if (Output_pin == -1) {
        perror("Error opening gpio21 direction");
        return 1;
    }
    write(Output_pin, "out", 3);
    close(Output_pin);

    struct pollfd mypoll;
    memset(&mypoll, 0, sizeof(mypoll));
    mypoll.fd = open("/sys/class/gpio/gpio20/value", O_RDONLY);
    if (mypoll.fd == -1) {
        perror("Error opening gpio20 value");
        return 1;
    }

    mypoll.events = POLLPRI;

    char Encoder_state;
    char buffer[1];
    while (1) {
        int ret = poll(&mypoll, 1, -1);
        
        if (ret == -1) {
            perror("Error in poll");
            break;
        }

        if (mypoll.revents & POLLPRI) {
            lseek(mypoll.fd, 0, SEEK_SET);
            if (read(mypoll.fd, &Encoder_state, 1) != 1) {
                perror("Error reading gpio20 value");
                break;
            }
            buffer[0] = Encoder_state;

            Output_pin = open("/sys/class/gpio/gpio21/value", O_WRONLY);
            if (Output_pin == -1) {
                perror("Error opening gpio21 value for writing");
                break;
            }
            if (write(Output_pin, buffer, 1) != 1) {
                perror("Error writing to gpio21");
                close(Output_pin);
                break;
            }
            fsync(Output_pin);
            close(Output_pin);
        }

        usleep(100); // Sleep for 100 microseconds for efficiency
    }

    close(mypoll.fd); // Close the reading file descriptor

    // Unexport pins
    Output_pin = open("/sys/class/gpio/unexport", O_WRONLY);
    if (Output_pin != -1) {
        write(Output_pin, "21", 2);
        close(Output_pin);
    }

    encoder_pin = open("/sys/class/gpio/unexport", O_WRONLY);
    if (encoder_pin != -1) {
        write(encoder_pin, "20", 2);
        close(encoder_pin);
    }

    return 0;
}
