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



void createModbusMessage(uint8_t* buffer, uint8_t serverAddress, uint8_t cmd, uint16_t serverRegister, uint16_t value, uint16_t CRC) {
    buffer[0] = serverAddress;            // Server address (cast to byte)
    buffer[1] = cmd; // Function code (for writing a single register)

    // Server register and value (both are 16-bit values, big-endian format)
    buffer[2] = (uint8_t)((serverRegister >> 8) & 0xFF); // High byte of serverRegister
    buffer[3] = (uint8_t)(serverRegister & 0xFF);       // Low byte of serverRegister

    buffer[4] = (uint8_t)((value >> 8) & 0xFF); // High byte of value
    buffer[5] = (uint8_t)(value & 0xFF);       // Low byte of value


    // CRC (16-bit value, little-endian format)
    buffer[6] = (uint8_t)((CRC >> 8) & 0xFF); // High byte of CRC(CRC & 0xFF);       // Low byte of CRC
    buffer[7] = (uint8_t)(CRC & 0xFF);       // Low byte of CRC
}


void parseModbusResponse(unsigned char* response, int length) {
    printf("Slave Address: %02X\n", response[0]);
    printf("Function Code: %02X\n", response[1]);
    printf("Byte Count: %02X\n", response[2]);

    printf("Data: ");
    for (int i = 3; i < length - 2; i++) {
        printf("%02X ", response[i]);
    }
    printf("\n");

    printf("CRC: %02X %02X\n", response[length - 2], response[length - 1]);
}

int main(int argc, char *argv[]) {
    int CRC = 43737;
    uint8_t modbusSendBuffer[8];
    uint8_t modbusRecieveBuffer[8];
    
    if (argc != 5) {
        printf("Invalid number of arguments, exiting!\n");
        return -2;
    }

    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        perror("UART: Failed to open the file.\n");
        return -1;
    }

    struct termios options;

    tcgetattr(file, &options);

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
            createModbusMessage(modbusSendBuffer, ServerAddress, WRITE_SINGLE_REGISTER, RegisterAddress, Value, CRC);
            printf("Modbus Message: ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);
            }


            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            printf("Message transmitted\n");
            usleep(100000);

            printf("Reading\n");
            if ((count = read(file, (void*)modbusRecieveBuffer, 8))<0){
                perror("Failed to read from the input\n");
                return -1;
            }

            parseModbusResponse(modbusRecieveBuffer, count);

            //send(ServerAddress, RegisterAddress, Value, CRC);
        } else if (Command == READ_HOLDING_REGISTERS) {
            createModbusMessage(modbusSendBuffer, ServerAddress, READ_HOLDING_REGISTERS, RegisterAddress, Value, CRC);
            printf("Modbus Message: ");
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);

            }
            if ((count = write(file, &modbusSendBuffer, 8))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
            printf("Message transmitted\n");
            usleep(10000);

            printf("Reading\n");
            if ((count = read(file, (void*)modbusSendBuffer, 8)) < 0){
                perror("Failed to read from the input\n");
                return -1;
            }
            for (int i = 0; i < 8; i++) {
                printf("%02X ", modbusSendBuffer[i]);

            }
            //parseModbusResponse(modbusRecieveBuffer, count);
        }
    }

    close(file);
    return 0;
}
