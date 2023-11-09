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

int CANBusSend(uint16_t COB_ID, uint8_t state, uint8_t node){
    
    uint8_t sendBuffer[4];

    sendBuffer[0] = (uint8_t)(COB_ID >> 8);
    sendBuffer[1] = (uint8_t)(COB_ID & 0xFF); 

    sendBuffer[2] = state;
    sendBuffer[3] = node;

    if ((count = write(file, &sendBuffer, 4))<0){
                perror("Failed to write to the output\n");
                return -1;
            }
    return 0;
}

int main(int argc, char *argv[]) {
    int error = 0;
    uint8_t SendBuffer[2];
    uint8_t RecieveBuffer[2];
    uint8_t node;

    if(argc < 3) {
        // argv[2] is not provided, default to 0x00
        node = 0x00;
    } else {
        // Convert argv[2] to an integer
        node = (uint8_t)strtol(argv[2], NULL, 16);
    }

    if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY)) < 0) {
        perror("UART: Failed to open the file.\n");
        return -1;
    }

    struct termios options;

    if (tcgetattr(file, &options) != 0) {
        perror("UART: Failed to get attributes.\n");
        close(file);
        return -1;
    }

    cfmakeraw(&options);
    options.c_lflag &= ~ICANON;
    options.c_cc[VTIME] = 5;  
    options.c_cc[VMIN] = 0;   // Read returns immediately with whatever data is available
    options.c_cflag = B115200 | CS8 | CREAD | CLOCAL; // Set baud rate to 115200, 8n1 (no parity)

    tcflush(file, TCIFLUSH);

    if (tcsetattr(file, TCSANOW, &options) != 0) {
        perror("UART: Failed to set attributes.\n");
        close(file);
        return -1;
    }


    if (argc >= 1) {
        
//      +--------+-----------------+-----------------+
//      | COB-ID |   Data Byte 0   |   Data Byte 1   |
//      +--------+-----------------+-----------------+
//      | 0x000  | Requested state | Addressed node  |
//      +--------+-----------------+-----------------+

        uint16_t COB_ID = 0x000;
        uint8_t state = (uint8_t)strtol(argv[1], NULL, 16); // Convert hex string to an integer

        
        switch(state){
            case 0x01:
                printf("Command: %02X, Set node operational\n", state);
                error = CANBusSend(COB_ID, state, node);
                if(error == -1)
                    printf("Error sending CAN message\n");
                break;

            case 0x02:
                printf("Command: %02X, Stop node\n", state);
                error = CANBusSend(COB_ID, state, node);
                if(error == -1)
                    printf("Error sending CAN message\n");
                break;

            case 0x80:
                printf("Command: %02X, Set node pre-operational\n", state);
                error = CANBusSend(COB_ID, state, node);     
                if(error == -1)
                    printf("Error sending CAN message\n");   
                break;

            case 0x81:
                printf("Command: %02X\n, Reset node", state);
                error = CANBusSend(COB_ID, state, node);
                if(error == -1)
                    printf("Error sending CAN message\n");
                break;

            case 0x82:
                printf("Command: %02X\n, Reset communications", state);
                error = CANBusSend(COB_ID, state, node);
                if(error == -1)
                    printf("Error sending CAN message\n");
                break;
                
            default:
                printf("Unknown command: 0x%02X\n", state);
                //Get temperature
                usleep(10000);
                CANBusSend(COB_ID, 0x80, 0x02); //Temp Preoperational
                usleep(10000);
                CANBusSend(COB_ID, 0x01, 0x02); //Temp Operational
                usleep(100000);               // Her er maelt delay 270 microsekunder
                printf("Reading\n");
                if ((count = read(file, (void*)RecieveBuffer, 2))<0){
                    perror("Failed to get temperature\n");
                    return -1;
                }
                printf("Temperature1: %02X\n", RecieveBuffer[0]);
                printf("Temperature2: %02X\n", RecieveBuffer[1]);
                // usleep(10000);
                // CANBusSend(COB_ID, 0x02, 0x02); //Stop node
                usleep(10000);

                
                //Send temperature to motor
                //CANBusSend(COB_ID, 0x80, 0x01); //Motor Preoperational
                //usleep(10000);
                CANBusSend(COB_ID, 0x01, 0x01); //Motor Operational
                usleep(10000);
                if ((count = write(file, &RecieveBuffer, 2))<0){
                    perror("Failed to write to the output\n");
                    return -1;
                }
                usleep(10000);
        
                // CANBusSend(COB_ID, 0x02, 0x02); //Stop node
                break;
        }

    }

    close(file);
    return 0;
}
