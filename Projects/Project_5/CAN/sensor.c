//TEMPERATURE

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

const int TMP36_PIN = A0;

static uint16_t temperature = 0;

int readTMP36Temperature() {
   int raw = analogRead(TMP36_PIN);
   float voltage = raw * (5.0 / 1023.0);
   float temperatureC = (voltage - 0.5) * 100.0;
   return (uint16_t)temperatureC;
}

void Operational() {
   temperature = readTMP36Temperature();
   // analogWrite(11, 255);
   // delay(100);
   // analogWrite(11, 0);
   // delay(100);
   Serial.write((uint8_t)(temperature >> 8));
   Serial.write((uint8_t)(temperature & 0xFF));
   Serial.flush();
}

void Stop(){
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
   pinMode(11, OUTPUT);
   pinMode(TMP36_PIN, INPUT);
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

      if((COB_ID == 0x000 && (Node == 0x00) ) || (COB_ID == 0x000 && (Node == 0x02))){
         switch (State) {
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
   
   // Get the current time
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the last time you updated the LED
    previousMillis = currentMillis;

    // Change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // Reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
      brightness = brightness + fadeAmount; // Ensure brightness is within 0-255
    }

    // Set the brightness of the LED
    analogWrite(11, brightness);
  }
}