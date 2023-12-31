#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include <stdint.h>
#include <stdlib.h>

const size_t MSG_LEN = 2;
uint8_t msg[MSG_LEN];

int main(int argc, char *argv[]){
   int file, count;

   if(argc!=3){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }

   if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }

   struct termios options;

   tcgetattr(file, &options);

   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   cfmakeraw(&options);
   options.c_lflag &= ~(ICANON);  // Set line mode to Non-canonical
   options.c_cc[VMIN] = MSG_LEN;  // Set the minimum number of bytes to the fixed packet length
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);

   // send the fixed-length binary message
   msg[0] = atoi(argv[1]);
   msg[1] = atoi(argv[2]);
   if ((count = write(file, msg, MSG_LEN))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   unsigned char receive[100];

   if ((count = read(file, (void*)receive, 100))<0){
      perror("Failed to read from the input\n");
      return -1;
   }

   if (count==0) printf("There was no data available to read!\n");
   else {
      receive[count]=0;  //There is no null character sent by the Arduino
      printf("The following was read in [%d]: %s\n",count,receive);
   }

   close(file);
   return 0;
}