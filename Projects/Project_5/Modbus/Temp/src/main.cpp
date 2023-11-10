#include <Arduino.h>
#include <stdint.h>

const int TMP36_PIN = A0;

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
                crc >>= 1; // Just shift rightX
        }
    }
    return crc;
}

void setup() {
    Serial.begin(115200, SERIAL_8N1);
    pinMode(TMP36_PIN, INPUT);
}

int readTMP36Temperature() {
    int raw = analogRead(TMP36_PIN);
    float voltage = raw * (5.0 / 1023.0);
    float temperatureC = (voltage - 0.5) * 100.0;
    return (int)temperatureC;
}

void loop() {
    if (Serial.available() >= 8) {
        byte requestData[8];
        for (int i = 0; i < 8; i++) {
            requestData[i] = Serial.read();
        }

        byte serverAddress = requestData[0];
        byte functionCode = requestData[1];
        uint16_t startRegister = (requestData[2] << 8) | requestData[3];
        uint16_t endRegister = (requestData[4] << 8) | requestData[5];
        uint16_t recievedCRC = ((uint16_t)requestData[6] << 8) | requestData[7];

        if ((recievedCRC == ModRTU_CRC(requestData,6)) && serverAddress == 2) {
            if (functionCode == 0x03) {  // Read holding register
                if (startRegister == 1 && endRegister == 1) {  // Assuming we're only supporting one register at address 1
                    uint16_t temperature = readTMP36Temperature();
                    byte response[7];
                    response[0] = serverAddress;
                    response[1] = functionCode;
                    response[2] = 0x02;  // Byte count
                    response[3] = (byte)(temperature >> 8);
                    response[4] = (byte)temperature;
                    uint16_t sentCRC = ModRTU_CRC(response, 5);
                    response[5] = (uint8_t)(sentCRC & 0xFF);    // Store the low byte of the CRC
                    response[6] = (uint8_t)((sentCRC >> 8) & 0xFF); // Store the high byte of the CRC
                    Serial.write(response, 7);
                }
            }
        }
    }
}