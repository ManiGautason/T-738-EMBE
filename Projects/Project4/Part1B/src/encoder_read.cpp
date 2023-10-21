#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstdlib> 


int main(void){

    //Enable Encoder pin
    int encoder_pin = open("/sys/class/gpio/export", O_WRONLY);
    write(encoder_pin, "20", 2);
    close(encoder_pin);

    //Set Encoder pin as input
    encoder_pin = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
    write(encoder_pin, "in", 2);
    close(encoder_pin);

    //Enable Output pin
    int Output_pin = open("/sys/class/gpio/export", O_WRONLY);
    write(Output_pin, "21", 2);
    close(Output_pin);

    //Set Output pin as output
    Output_pin = open("/sys/class/gpio/gpio20/direction", O_WRONLY);
    write(Output_pin, "in", 2);
    close(Output_pin);
    
    char Encoder_state; 
    char buffer[1]; 
    

    while(1){
        // Read the encoder pin state
        encoder_pin = open("/sys/class/gpio/gpio20/value", O_RDONLY); 
        read(encoder_pin, &Encoder_state, 1);
        close(encoder_pin);

        buffer[0] = Encoder_state;

        // Mirror the encoder state to the output GPIO pin
        Output_pin = open("/sys/class/gpio/gpio21/value", O_WRONLY);
        write(Output_pin, buffer, 1);
        close(Output_pin);

    }
}