#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <controller.h>
#include <PI_controller.h>

PI_controller Pcon(0.4,0,70,999999,10);

int K_p = 0;
int T_i = 0;  


// #define DEVICE_PATH "/dev/encoder1"
#define DEVICE_PATH "/dev/enc"

void EnablePWM(void){
    //Export PWM on pin 18
    int PWM = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    write(PWM, "0", 1);
    close(PWM);

    //Set PWM pin period
    PWM = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
    write(PWM, "100000",6 );
    close(PWM);

    //Set PWM pin duty cycle
    PWM = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    write(PWM, "10000", 5); 
    close(PWM);

    //Enable PWM pin
    PWM = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
    write(PWM, "1", 1); 
    close(PWM);
}

void SetDutyCycle(int dutyCycle) {
    //Set PWM pin duty cycle
    char buffer[16];  // Choose a buffer size that can accommodate your number
    sprintf(buffer, "%d", dutyCycle);
    int PWM = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    write(PWM, buffer, 16 ); 
    close(PWM);
}
void DisablePWM(void){
    //Unexport PWM on pin 18
    int PWM = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
    write(PWM, "0", 1);
    close(PWM);
}

int main() {
    EnablePWM();

    int fd;
    char buffer[16];
    ssize_t bytes_read;

    struct timeval lastTime, currentTime;
    gettimeofday(&lastTime, NULL);


    // Open the character device for reading
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the device file");
        return 1;
    }


    int CountNow = 0;
    int LastCount = 0;
    int PV = 0;
    int PWM = 0;
    int RPMint = 0;
    int setpoint = 40;
    while(1){


        // Display the pulseCount
        //printf("Pulse Count: %s", buffer);

        gettimeofday(&currentTime, NULL);

        // Calculate the time elapsed since the last iteration in milliseconds
        unsigned long long elapsedTime =
            (unsigned long long)(currentTime.tv_sec - lastTime.tv_sec) * 1000 +
            (unsigned long long)(currentTime.tv_usec - lastTime.tv_usec) / 1000;

        // Check if 10 milliseconds have passed
        if (elapsedTime >= 10) {
            // Read the pulseCount from the device
            bytes_read = read(fd, buffer, sizeof(buffer));
            if (bytes_read == -1) {
                perror("Failed to read from the device");
                close(fd);
                return 1;
            }
            

            // Null-terminate the received data
            buffer[bytes_read] = '\0';
            int CountNow = atoi(buffer);
            float RPM = ((float)(CountNow - LastCount) * 100 / (7.0 * 100.0)) * 60.0;
            RPMint = int(RPM/4);
            printf("%d\r\n", RPMint);
            
            
            
            PV = Pcon.update(setpoint,RPMint);
            //printf("%d\r\n",PV);
            PWM = int(PV);
            SetDutyCycle(PWM);
            
            
            
            
            // Update the last time to the current time
            lastTime = currentTime;
            LastCount = CountNow;

        }
    }

    // Close the device file
    close(fd);

    //Export PWM pin
    DisablePWM();
    return 0;
}

