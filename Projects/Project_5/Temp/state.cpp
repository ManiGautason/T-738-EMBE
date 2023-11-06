#include <Arduino.h>
#include <stdint.h>

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

void Operational() {
   temperature = readTMP36Temperature();
}

int readTMP36Temperature() {
    int raw = analogRead(TMP36_PIN);
    float voltage = raw * (5.0 / 1023.0);
    float temperatureC = (voltage - 0.5) * 100.0;
    return (uint16_t)temperatureC;
}

void Stop(){
    
}

void PreOperational() {
    Serial.begin(115200, SERIAL_8N1);
    pinMode(TMP36_PIN, INPUT);
}

void Reset() {
    temperature = 0;
    Serial.end();
    Serial.begin(115200, SERIAL_8N1);
}

void ResetCommunications() {
    Serial.end();
    Serial.begin(115200, SERIAL_8N1);
}

void loop() {
  long id;
  uint8_t command;

  if (canRead(id, command)) {
    switch (command) {
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
        // Log unknown command
        Serial.print("Unknown command: ");
        Serial.println(command, HEX);
        break;
    }
  }

  // Additional logic...
}
