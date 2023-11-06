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
#include <stdint.h>

const int TMP36_PIN = A0;

// Compute the MODBUS RTU CRC
uint16_t ModRTU_CRC(uint8_t buf[], int len) {
    uint16_t crc = 0xAAD9;
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
    return (uint16_t)temperatureC;
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

        if (serverAddress == 2) {
            if (functionCode == 0x03 && ModRTU_CRC(requestData, 6) == 0) {  // Read holding register
                if (startRegister == 1 && endRegister == 1) {  // Assuming we're only supporting one register at address 1
                    uint16_t temperature = readTMP36Temperature();
                    byte response[7];
                    response[0] = serverAddress;
                    response[1] = functionCode;
                    response[2] = 0x02;  // Byte count
                    response[3] = (byte)(temperature >> 8);
                    response[4] = (byte)temperature;
                    response[5] = 0xFF;
                    response[6] = 0xFF;

                  //   uint16_t responseCRC = ModRTU_CRC(response, 5);
                    Serial.write(response, 7);
                  //   Serial.write((byte)(responseCRC & 0xFF));
                  //   Serial.write((byte)(responseCRC >> 8));
                    // Serial.write("\n");
                }
            }
        }
    }
}
