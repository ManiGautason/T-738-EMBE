#include <Arduino.h>
#include <stdint.h>

const int TMP36_PIN = A0;

// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t buf[], int len) {
    uint16_t crc = 43737;
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

void setup() {
    Serial.begin(115200, SERIAL_8N1);
    pinMode(11, OUTPUT);
}


void loop() {
    if (Serial.available() >= 8) {  // Modbus RTU request size for our case
        byte requestData[8];
        for (int i = 0; i < 8; i++) {
            requestData[i] = Serial.read();
        }

        byte serverAddress = requestData[0];
        byte functionCode = requestData[1];
        uint16_t startRegister = (requestData[2] << 8) | requestData[3];
        uint16_t endRegister = (requestData[4] << 8) | requestData[5];
        // We're not using CRC for validation right now but it's there in requestData[6] and requestData[7].

        if (ModRTU_CRC(requestData, 6) == 0 && serverAddress == 1) {
            if (functionCode == 0x06) {  // Read holding register
                if (startRegister == 1 ) {  // Assuming we're only supporting one register at address 1
                  if (endRegister >= 0 && endRegister <= 255) {
                     analogWrite(11, endRegister); //
                     byte response[6];
                     response[0] = serverAddress;
                     response[1] = functionCode;
                     response[2] = startRegister;  // Byte count
                     response[3] = endRegister;
                     response[4] = 0xaa;
                     response[5] = 0xd9;

                  //   uint16_t responseCRC = ModRTU_CRC(response, 5);
                    Serial.write(response, 6);
                  //   Serial.write((byte)(responseCRC & 0xFF));
                  //   Serial.write((byte)(responseCRC >> 8));
                    Serial.write("\n");
                  }
                }
            }
        }
    }
}