#include <Arduino.h>

const size_t MSG_LEN = 2;
uint8_t msg[MSG_LEN];

/* Arduino Serial Command Processor */

int ledPin = 11;          // LED with PWM brightness control

void setup() {            // called once on start up
   // A baud rate of 115200 (8-bit with No parity and 1 stop bit)
   Serial.begin(115200, SERIAL_8N1);
   pinMode(ledPin, OUTPUT);         // the LED is an output
}

void loop() {
    if ((size_t)Serial.available() >= MSG_LEN) {
        // Read the fixed-length message into the buffer
        Serial.readBytes(msg, MSG_LEN);
        
        // First byte is the register and the second byte is the value.
        uint8_t register_value = msg[0];
        uint8_t data_value = msg[1];

        // Send a response back to the Raspberry Pi
        if (register_value == 2) {
            analogWrite(ledPin, data_value); // Set LED brightness based on the buffer value
            char response[50];
            sprintf(response, "Set brightness to %d", data_value);
            Serial.print(response);
        } else {
            Serial.print("Invalid register!");
        }
    }
}