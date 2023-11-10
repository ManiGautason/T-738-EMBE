#include <Arduino.h>
#include <stdint.h>

const int TMP36_PIN = A0;

// Define IDs or command specifiers as constants
const long SET_OPERATIONAL = 0x01;
const long STOP_NODE = 0x02;
const long SET_PRE_OPERATIONAL = 0x80;
const long RESET_NODE = 0x81;
const long RESET_COMMUNICATIONS = 0x82;

static uint16_t temperature = 0;

int readTMP36Temperature() {
   int raw = analogRead(TMP36_PIN);
   float voltage = raw * (5.0 / 1023.0);
   float temperatureC = (voltage - 0.5) * 100.0;
   return (uint16_t)temperatureC;
}

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

void Operational() {
   digitalWrite(11,1);
   int flag = 0;
   while(flag == 0){
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
                  if (startRegister == 2 && endRegister == 2) {  // Assuming we're only supporting one register at address 1
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
         flag = 1;
      }
   }
}

void Stop(){
   digitalWrite(11,0);
}

void PreOperational() {
   // Serial.begin(115200, SERIAL_8N1);
   pinMode(TMP36_PIN, INPUT);
}

void Reset() {
   Serial.end();
   Serial.begin(115200, SERIAL_8N1);
}

void ResetCommunications() {
   Serial.end();
   Serial.begin(115200, SERIAL_8N1);
}


void setup() {
    Serial.begin(115200, SERIAL_8N1);
    pinMode(TMP36_PIN, INPUT);
    pinMode(11, OUTPUT);
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
            else if(functionCode == 0x06){   
               if(startRegister == 0x00){
                  byte response[8];
                  response[0] = serverAddress;
                  response[1] = functionCode;
                  response[2] = startRegister << 8;  // Byte count
                  response[3] = startRegister;
                  response[4] = endRegister << 8;  // Byte count
                  response[5] = endRegister;
                  uint16_t sentCRC = ModRTU_CRC(response, 6);
                  response[6] = (uint8_t)(sentCRC & 0xFF);    // Store the low byte of the CRC
                  response[7] = (uint8_t)((sentCRC >> 8) & 0xFF); // Store the high byte of the CRC
                  Serial.write(response, 8);
                  switch (endRegister) {
                  case 0x01:
                     Operational();
                     break;
                  case STOP_NODE:
                     Stop();
                     break;
                  case SET_PRE_OPERATIONAL:
                     PreOperational();
                     break;
                  case RESET_NODE:
                     Reset();
                     break;
                  case RESET_COMMUNICATIONS:
                     ResetCommunications();
                     break;
                  default: 
                     break;
                  }

               }
            }
        }
    }
}