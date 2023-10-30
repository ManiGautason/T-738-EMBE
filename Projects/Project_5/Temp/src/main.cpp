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


#include <Arduino.h>

/* Arduino Serial Command Processor */

// Define the analog pin to which the TMP36 is connected
const int TMP36_PIN = A0;

void setup() {
   Serial.begin(115200, SERIAL_8N1);
   pinMode(TMP36_PIN, INPUT);
}

// Function to read temperature from TMP36 and return it as an integer
int readTMP36Temperature() {
   int raw = analogRead(TMP36_PIN);
   float voltage = raw * (5.0 / 1023.0);
   float temperatureC = (voltage - 0.5) * 100.0;
   return (int)temperatureC;
}

void loop() {
   if (Serial.available() >= 2) {  // At least 2 bytes available for a complete message
      byte ID = Serial.read();
      byte registerByte = Serial.read();

      // Process the binary message
      if (ID == 0x02) {  // Check for the LED register (0x02)
         if (registerByte == 0xBB) {  // Check for the brightness register (0x01) 
            // Serial.write(0xCF);  // Send an acknowledgement byte
            // Serial.println();
            int temperature = readTMP36Temperature();
            Serial.write((byte)temperature);  // Send the temperature as an integer
            Serial.println();
         } 
      }
      // else if (ID == 0x02) {  // Check for the temperature request command
      //    int temperature = readTMP36Temperature();
      //    Serial.write((byte)temperature);  // Send the temperature as an integer
      // }
   }
}