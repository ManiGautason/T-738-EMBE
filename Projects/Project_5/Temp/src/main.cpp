// #include <Arduino.h>

// /* Arduino Serial Command Processor */

// void setup() {
//    Serial.begin(115200, SERIAL_8N1);
// }

// void loop() {
//    if (Serial.available() >= 2) {  // At least 2 bytes available for a complete message
//       byte ID = Serial.read();
//       byte registerByte = Serial.read();

//       // Process the binary message
//       if (ID == 0x02) {  // Check for the LED register (0x02)
//          if (registerByte == 0xBB) {  // Check for the brightness register (0x01) 
//             Serial.write(0xCF);  // Send an acknowledgement byte
//             Serial.println();
//          } 
//       }
//    }
// }


// #include <Arduino.h>

// /* Arduino Serial Command Processor */

// // Define the analog pin to which the TMP36 is connected
// const int TMP36_PIN = A0;

// void setup() {
//    Serial.begin(115200, SERIAL_8N1);
//    pinMode(TMP36_PIN, INPUT);
// }

// // Function to read temperature from TMP36 and return it as an integer
// int readTMP36Temperature() {
//    int raw = analogRead(TMP36_PIN);
//    float voltage = raw * (5.0 / 1023.0);
//    float temperatureC = (voltage - 0.5) * 100.0;
//    return (int)temperatureC;
// }

// void loop() {
//    if (Serial.available() >= 2) {  // At least 2 bytes available for a complete message
//       byte ID = Serial.read();
//       byte registerByte = Serial.read();

//       // Process the binary message
//       if (ID == 0x02) {  // Check for the LED register (0x02)
//          if (registerByte == 0xBB) {  // Check for the brightness register (0x01) 
//             // Serial.write(0xCF);  // Send an acknowledgement byte
//             // Serial.println();
//             int temperature = readTMP36Temperature();
//             Serial.write((byte)temperature);  // Send the temperature as an integer
//             Serial.println();
//          } 
//       }
//       // else if (ID == 0x02) {  // Check for the temperature request command
//       //    int temperature = readTMP36Temperature();
//       //    Serial.write((byte)temperature);  // Send the temperature as an integer
//       // }
//    }
// }




// #include <Arduino.h>

// /* Arduino Serial Command Processor */

// // Define the analog pin to which the TMP36 is connected
// const int TMP36_PIN = A0;

// void setup() {
//    Serial.begin(115200, SERIAL_8N1);
//    pinMode(TMP36_PIN, INPUT);
// }

// // Function to read temperature from TMP36 and return it as an integer
// int readTMP36Temperature() {
//    int raw = analogRead(TMP36_PIN);
//    float voltage = raw * (5.0 / 1023.0);
//    float temperatureC = (voltage - 0.5) * 100.0;
//    return (int)temperatureC;
// }

// void loop() {
//    if (Serial.available() >= 2) {  // At least 2 bytes available for a complete message
//       byte serverAddress = Serial.read();
//       byte functionCode = Serial.read();

//       // Process the binary message
//       if (serverAddress == 0x02) {  // Check for the LED register (0x02)


//          if (functionCode == 0x03) {  // Check for the brightness register (0x01) 
//             // Serial.write(0xCF);  // Send an acknowledgement byte
//             // Serial.println();
//             int temperature = readTMP36Temperature();
//             Serial.write((byte)temperature);  // Send the temperature as an integer
//             Serial.println();
//          }

//          else if (functionCode == 0x06) {
//             /* code */
//          }
         
//       }
//    }
// }

#include <Arduino.h>

const int TMP36_PIN = A0;

uint16_t calculateCRC(byte *array, byte num) {
    uint16_t crc = 0xFFFF;
    while (num--) {
        crc ^= *array++;
        for (byte i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
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
    if (Serial.available() >= 6) {  // Modbus RTU request size for our case
        byte requestData[6];
        for (int i = 0; i < 6; i++) {
            requestData[i] = Serial.read();
        }

        // Extract the address, function code, and CRC from the request
        byte serverAddress = requestData[0];
        byte functionCode = requestData[1];
        uint16_t crcReceived = (requestData[4] << 8) | requestData[5];
        uint16_t crcCalculated = calculateCRC(requestData, 4);

        if (crcReceived == crcCalculated) {  // CRC matches
            if (serverAddress == 1) {
                if (functionCode == 0x03) {  // Read holding register
                    int temperature = readTMP36Temperature();
                    byte response[5];
                    response[0] = serverAddress;
                    response[1] = functionCode;
                    response[2] = 0x02;  // Byte count
                    response[3] = (byte)(temperature >> 8);
                    response[4] = (byte)temperature;
                    uint16_t responseCRC = calculateCRC(response, 5);
                    Serial.write(response, 5);
                    Serial.write((byte)(responseCRC & 0xFF));
                    Serial.write((byte)(responseCRC >> 8));
                }
                // Implement functionCode 0x06 later
            }
        } else {
            // CRC error; ignore the message
        }
    }
}
