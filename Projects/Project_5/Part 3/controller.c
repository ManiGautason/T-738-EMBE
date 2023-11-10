#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define WRITE_SINGLE_REGISTER 6
#define READ_HOLDING_REGISTERS 3
int file, count;

// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t buf[], int len) {
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos]; // XOR byte into least sig. byte of crc
        for (int i = 8; i != 0; i--) { // Loop over each bit
            if ((crc & 0x0001) != 0) { // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    return crc;
}

void createModbusMessage(uint8_t* buffer, uint8_t serverAddress, uint8_t cmd, uint16_t serverRegister, uint16_t value) {
    buffer[0] = serverAddress;            // Server address (cast to byte)
    buffer[1] = cmd; // Function code (for writing a single register)

    // Server register and value (both are 16-bit values, big-endian format)
    buffer[2] = (uint8_t)((serverRegister >> 8) & 0xFF); // High byte of serverRegister
    buffer[3] = (uint8_t)(serverRegister & 0xFF);       // Low byte of serverRegister

    buffer[4] = (uint8_t)((value >> 8) & 0xFF); // High byte of value
    buffer[5] = (uint8_t)(value & 0xFF);       // Low byte of value


    // CRC (16-bit value, little-endian format)
    uint16_t CRC = ModRTU_CRC(buffer,6);
    buffer[6] = (uint8_t)((CRC >> 8) & 0xFF); // High byte of CRC(CRC & 0xFF);       // Low byte of CRC
    buffer[7] = (uint8_t)(CRC & 0xFF);       // Low byte of CRC
}


void parseModbusResponse(unsigned char* response, int length) {
    printf("Slave Address: %02X\n", response[0]);
    printf("Function Code: %02X\n", response[1]);


    if (response[1] == 0x03){
        printf("Byte Count: %02X\n", response[2]);

        printf("Data: ");
        for (int i = 3; i < length - 2; i++) {
            printf("%02X ", response[i]);
        }
        printf("\n");

        printf("CRC: %02X %02X\n", response[length - 1], response[length - 2]);
    }

    if (response[1] == 0x06){
        printf("Byte Count: %02X\n", response[2]);

        printf("Register Address: ");
        for (int i = 2; i < length - 4; i++) {
            printf("%02X ", response[i]);
        }
        printf("\n");

        printf("Written Value: ");
        for (int i = 4; i < length - 2; i++) {
            printf("%02X ", response[i]);
        }
        printf("\n");

        printf("CRC: %02X %02X\n", response[length - 1], response[length - 2]);
    }
    
}

int main(int argc, char *argv[]) {

    uint8_t modbusSendBuffer[8];
    uint8_t modbusRecieveBuffer[8];

    if (argc != 5) {
        logMessage("Invalid number of arguments, exiting!");
        return -2;
    }

    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        logMessage("UART: Failed to open the file.");
        perror("UART: Failed to open the file.");
        return -1;
    }

    struct termios options;

    tcgetattr(file, &options);
    cfmakeraw(&options);            // set as raw
    options.c_lflag &= ~ICANON;
    options.c_cc[VTIME] = 0;  
    options.c_cc[VMIN] = 8;
    options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
    options.c_iflag = IGNPAR | ICRNL;
    tcflush(file, TCIFLUSH);
    tcsetattr(file, TCSANOW, &options);

    if (argc >= 4) {
        int ServerAddress = atoi(argv[1]);
        int Command = atoi(argv[2]);
        int RegisterAddress = atoi(argv[3]);
        int Value = atoi(argv[4]);

        if (Command == WRITE_SINGLE_REGISTER) {
            createModbusMessage(modbusSendBuffer, ServerAddress, WRITE_SINGLE_REGISTER, RegisterAddress, Value);
            logMessage("Modbus Sent Message:");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");

            if ((count = write(file, &modbusSendBuffer, 8))<0){
                logMessage("Failed to write to the output.");
                perror("Failed to write to the output");
                return -1;
            }
            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                logMessage("Failed to read from the input.");
                perror("Failed to read from the input");
                return -1;
            }

            uint16_t receivedCRC = ((uint16_t)modbusRecieveBuffer[7] << 8) | modbusRecieveBuffer[6];
            if(receivedCRC == ModRTU_CRC(modbusRecieveBuffer, 6)){
                logMessage("Modbus Received Message:");
                for (int i = 0; i < 6; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X %02X\n", modbusRecieveBuffer[7], modbusRecieveBuffer[6]);
            } else {
                logMessage("CRC Error in Modbus response.");
                printf("CRC Error\n");
            }

        }
        else if (Command == READ_HOLDING_REGISTERS) {
            createModbusMessage(modbusSendBuffer, ServerAddress, READ_HOLDING_REGISTERS, RegisterAddress, Value);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");
            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }

            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 7)) < 0){
                perror("Failed to read from the input\n");
                return -1;
            }

            uint16_t recievedCRC = ((uint16_t)modbusRecieveBuffer[6] << 8) | modbusRecieveBuffer[5];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,5)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 5; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[6]);
                printf("%02X ", modbusRecieveBuffer[5]);
                printf("\n");
            } else {
                printf("CRC Error\n");
            }
        } 
        else if(Command == 1){ // Control task that reads a value from one Arduino that controls an action on the other Arduino
            printf("============== Control task =================\n");
            // Set temperature sensor in preop state   
            createModbusMessage(modbusSendBuffer, 2, 6, 0, 80);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");


            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                perror("Failed to read from the input\n");
                return -1;
            }

            uint16_t recievedCRC = ((uint16_t)modbusRecieveBuffer[7] << 8) | modbusRecieveBuffer[6];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,6)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 6; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[7]);
                printf("%02X ", modbusRecieveBuffer[6]);                
                printf("\n");
            } else {
                printf("CRC Error\n");
            }

            // Set temperature sensor in op state
            createModbusMessage(modbusSendBuffer, 2, 6, 0, 1);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");


            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                perror("Failed to read from the input\n");
                return -1;
            }

            recievedCRC = ((uint16_t)modbusRecieveBuffer[7] << 8) | modbusRecieveBuffer[6];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,6)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 6; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[7]);
                printf("%02X ", modbusRecieveBuffer[6]);                
                printf("\n");
            } else {
                printf("CRC Error\n");
            }



            // Read temperature sensor value
            createModbusMessage(modbusSendBuffer, 2, 3, 2, 2);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");
            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }

            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 7)) < 0){
                perror("Failed to read from the input\n");
                return -1;
            }

            recievedCRC = ((uint16_t)modbusRecieveBuffer[6] << 8) | modbusRecieveBuffer[5];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,5)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 5; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[6]);
                printf("%02X ", modbusRecieveBuffer[5]);
                printf("\n");
            } else {
                printf("CRC Error\n");
            }

            // Get value from recieved buffer
            uint8_t pwm = modbusRecieveBuffer[4];

            // Set motor in pre operation state
            createModbusMessage(modbusSendBuffer, 1, 6, 0, 80);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");


            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                perror("Failed to read from the input\n");
                return -1;
            }

            recievedCRC = ((uint16_t)modbusRecieveBuffer[7] << 8) | modbusRecieveBuffer[6];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,6)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 6; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[7]);
                printf("%02X ", modbusRecieveBuffer[6]);                
                printf("\n");
            } else {
                printf("CRC Error\n");
            }

            // set motor in operational state
            createModbusMessage(modbusSendBuffer, 1, 6, 0, 1);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");


            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                perror("Failed to read from the input\n");
                return -1;
            }

            recievedCRC = ((uint16_t)modbusRecieveBuffer[7] << 8) | modbusRecieveBuffer[6];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,6)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 6; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[7]);
                printf("%02X ", modbusRecieveBuffer[6]);                
                printf("\n");
            } else {
                printf("CRC Error\n");
            }

            // set PWM value
            createModbusMessage(modbusSendBuffer, 1, 6, 1, pwm);
            printf("Modbus Sent Message:     ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }
            printf("\n");


            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            usleep(10000);

            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                perror("Failed to read from the input\n");
                return -1;
            }

            recievedCRC = ((uint16_t)modbusRecieveBuffer[7] << 8) | modbusRecieveBuffer[6];
            if(recievedCRC == ModRTU_CRC(modbusRecieveBuffer,6)){
                printf("Modbus Received Message: ");
                for (int i = 0; i < 6; i++) {
                    printf("%02X ", modbusRecieveBuffer[i]);
                }
                printf("%02X ", modbusRecieveBuffer[7]);
                printf("%02X ", modbusRecieveBuffer[6]);                
                printf("\n");
            } else {
                printf("CRC Error\n");
            }


        }
    }
    close(file);
    return 0;
}