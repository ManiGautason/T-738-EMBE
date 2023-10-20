#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    // Enable Encoder pin
    int encoder_pin = open("/sys/class/gpio/export", O_WRONLY);
    write(encoder_pin, "20", 2);
    close(encoder_pin);

    // Set Encoder pin as input
    encoder_pin = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
    write(encoder_pin, "in", 2);
    close(encoder_pin);

    // Set Encoder trigger
    encoder_pin = open("/sys/class/gpio/gpio20/edge", O_WRONLY);
    write(encoder_pin, "both", 4);
    close(encoder_pin);

    // Enable Output pin
    int Output_pin = open("/sys/class/gpio/export", O_WRONLY);
    write(Output_pin, "21", 2);
    close(Output_pin);

    // Set Output pin as output
    Output_pin = open("/sys/class/gpio/gpio21/direction", O_WRONLY);
    write(Output_pin, "out", 3); // Should be "out", not "in"
    close(Output_pin);

    struct pollfd mypoll;
    memset(&mypoll, 0, sizeof(mypoll));
    mypoll.fd = open("/sys/class/gpio/gpio20/value", O_RDONLY); // Open value file directly
    mypoll.events = POLLPRI; // Use POLLPRI to detect edge changes

    char Encoder_state;
    char prevEncoder_state = '0'; // Initialize to LOW state
    char buffer[1]; 
    while (1) {
        poll(&mypoll, 1, -1); // Wait indefinitely for an event

        if (mypoll.revents & POLLPRI) {
            lseek(mypoll.fd, 0, SEEK_SET); // Move the file pointer to the beginning
            read(encoder_pin, &Encoder_state, 1);
            buffer[0] = Encoder_state;

            // Mirror the encoder state to the output GPIO pin
            Output_pin = open("/sys/class/gpio/gpio21/value", O_WRONLY);
            write(Output_pin, buffer, 1);
            close(Output_pin);
        }
    }

    Output_pin = open("/sys/class/gpio/unexport", O_WRONLY);
    write(Output_pin, "21", 2);
    close(Output_pin);

    encoder_pin = open("/sys/class/gpio/unexport", O_WRONLY);
    write(encoder_pin, "20", 2);
    close(encoder_pin);

    // You should add code to clean up and close GPIO pins before exiting
    return 0;
}