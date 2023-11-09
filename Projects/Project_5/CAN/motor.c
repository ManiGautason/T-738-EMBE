// MOTOR

#include <Arduino.h>
#include <stdint.h>

int setpoint;
int brightness = 0;
int fadeAmount = 10;
unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 30; // interval at which to update the LED (milliseconds)

bool canRead(long &id, uint8_t &command);
void canWrite(long id, uint8_t command);
void Operational();
void Stop();
void PreOperational();
void Reset();
void ResetCommunications();

// Define IDs or command specifiers as constants
const long SET_OPERATIONAL = 0x01;
const long STOP_NODE = 0x02;
const long SET_PRE_OPERATIONAL = 0x80;
const long RESET_NODE = 0x81;
const long RESET_COMMUNICATIONS = 0x82;

const int MOTOR_PIN = 11;

void Operational() {

   delay(10);  // Necessary delay to allow the serial buffer to fill up
   if (Serial.available() >= 2) {
      // Read two bytes from the serial buffer
      byte requestData[2];
      for (int i = 0; i < 2; i++) {
        requestData[i] = Serial.read();
      }
      // Use the last byte to set the LED brightness
      setpoint = requestData[1];
      analogWrite(MOTOR_PIN, setpoint);
      delay(1000);
  }
}

void Stop(){
   setpoint = 0;
   analogWrite(MOTOR_PIN, setpoint);
}

void PreOperational() {
   setpoint = 0;
   Serial.begin(115200, SERIAL_8N1);
   pinMode(MOTOR_PIN, OUTPUT);
}

void Reset() {
   setpoint = 0;
   analogWrite(MOTOR_PIN, setpoint);
   Serial.end();
   Serial.begin(115200, SERIAL_8N1);
}

void ResetCommunications() {
   Serial.end();
   Serial.begin(115200, SERIAL_8N1);
}

void setup() {
   Serial.begin(115200, SERIAL_8N1);
   pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
   
   if (Serial.available() >= 4) {  // Modbus RTU request size for our case
      byte requestData[4];
      for (int i = 0; i < 4; i++) {
         requestData[i] = Serial.read();
      }

      uint16_t COB_ID = (requestData[0] << 8) | requestData[1];
      byte State = requestData[2];
      byte Node = requestData[3];

      if((COB_ID == 0x000 && (Node == 0x00) ) || (COB_ID == 0x000 && (Node == 0x01))){
         switch (State) {
         case SET_OPERATIONAL:
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


//    // Get the current time
//   unsigned long currentMillis = millis();

//   if (currentMillis - previousMillis >= interval) {
//     // Save the last time you updated the LED
//     previousMillis = currentMillis;

//     // Change the brightness for next time through the loop:
//     brightness = brightness + fadeAmount;

//     // Reverse the direction of the fading at the ends of the fade:
//     if (brightness <= 0 || brightness >= 255) {
//       fadeAmount = -fadeAmount;
//       brightness = brightness + fadeAmount; // Ensure brightness is within 0-255
//     }

//     // Set the brightness of the LED
//     analogWrite(11, brightness);
//   }
}