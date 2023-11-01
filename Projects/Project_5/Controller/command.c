#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int getTemp();
int setMotor(int);
int file, count;

int main(){
  

   if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }

   struct termios options;

   tcgetattr(file, &options);

   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);



   while(1){

      int temperature = getTemp();
      
      if(temperature != -1)
         printf("Temperature: %d\n", temperature);
      else
         printf("Failed to get temperature\n");
      

      if(setMotor(temperature) != -1)
         printf("Setpoint: %d\n", setMotor(temperature));
      else
         printf("Failed to set setpoint\n");

      
      usleep(3000000);
   }

   close(file);
   return 0;
}

int getTemp(){

   // Send 0x02 to specify the Arduino ID
   unsigned char arduino_id = 0x02;
   if ((count = write(file, &arduino_id, 1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   // Send 0xBB to request temperature data
   unsigned char temp_request = 0xBB;
   if ((count = write(file, &temp_request, 1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   unsigned char receive[100];

   if ((count = read(file, (void*)receive, 100))<0){
      perror("Failed to read from the input\n");
      return -1;
   }

   if (count==0) {
      printf("There was no data available to read!\n");
   } else {
      receive[count]=0;
      return (int)receive[0];
   }

   return -1;
}

int setMotor(int setpoint){
   
   // Send 0x01 to specify the Arduino ID(motor)
   unsigned char arduino_id = 0x01;
   if ((count = write(file, &arduino_id, 1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   // Send 0xBB to request setpoint register
   unsigned char setpoint_register = 0xAA;
   if ((count = write(file, &setpoint_register, 1))<0){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   // Write data to setpoint register
   unsigned char setpoint_data = setpoint;
   if ((count = write(file, &setpoint_data, 1))<0){
      perror("Failed to write to the register\n");
      return -1;
   }
   return setpoint;
}