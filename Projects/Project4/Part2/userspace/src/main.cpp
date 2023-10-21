#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <controller.h>
#include <PI_controller.h>
#include <time.h>

PI_controller Pcon(0.3,0.23,70,999999,10);

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
    char buffer[16]; 
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
    int CountNow = 0;
    int LastCount = 0;
    int PV = 0;
    int PWM = 0;
    int RPMint = 0;
    int setpoint = 40;
    char buffer[16];
    ssize_t bytes_read;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); // Start the timer
    long long interval = 10000000;

    // Open ecoder
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the device file");
        return 1;
    }

    while(1){

        ////////////////////////////////////////////////////////////////////////////////////////////////
        //                                                                                            //      
        //                              IMPLEMENTATION WITHOUT DELAY                                  //
        //                                                                                            //
        ////////////////////////////////////////////////////////////////////////////////////////////////

        // //printf("Pulse Count: %s", buffer);
        // clock_gettime(CLOCK_MONOTONIC, &end_time);
        // long long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);

        // if (elapsed_time >= interval) {
        //     //printf("%lld\n", elapsed_time);
        //     PI_controller Pcon(0.3,0.23,70,999999,elapsed_time);
        //     // Read the pulseCount from the device
        //     bytes_read = read(fd, buffer, sizeof(buffer));
        //     if (bytes_read == -1) {
        //         perror("Failed to read from the device");
        //         close(fd);
        //         return 1;
        //     }
            
        //     buffer[bytes_read] = '\0';
        //     int CountNow = atoi(buffer);
        //     float RPM = ((float)(CountNow - LastCount) * 100 / (7.0 * 100.0)) * 60.0;
        //     RPMint = int(RPM/4);
        //     printf("%d\r\n", RPMint);
            
        //     PV = Pcon.update(setpoint,RPMint);
        //     //printf("%d\r\n",PV);
        //     PWM = int(PV);
        //     SetDutyCycle(PWM);
            
        
        //     start_time = end_time;
        //     LastCount = CountNow;


        ////////////////////////////////////////////////////////////////////////////////////////////////
        //                                                                                            //      
        //                                TESTING USLEEP PERFORMANCE                                  //
        //                                                                                            //
        ////////////////////////////////////////////////////////////////////////////////////////////////

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        long long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
        //printf("%lld\n", elapsed_time);
        usleep(10000);
        bytes_read = read(fd, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("Failed to read from the device");
            close(fd);
            return 1;
        }
        
        buffer[bytes_read] = '\0';
        int CountNow = atoi(buffer);
        float RPM = ((float)(CountNow - LastCount) * 100 / (7.0 * 100.0)) * 60.0;
        RPMint = int(RPM/4);
        printf("%d\r\n", RPMint);
        
        PV = Pcon.update(setpoint,RPMint);
        PWM = int(PV);
        SetDutyCycle(PWM);
        
    
        start_time = end_time;
        LastCount = CountNow;
        
    }

    // Close the device file
    close(fd);

    //Export PWM pin
    DisablePWM();
    return 0;
}

