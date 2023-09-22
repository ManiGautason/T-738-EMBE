#include "Encoder.h"
#include <Arduino.h> // Include this if you're using Arduino IDE and want to use millis()

Encoder::Encoder() {
    counter = 0;
    lastA = 0;
    lastB = 0;
    // prevTime = 0;
    previousMillis = 0;
    // interval = 10;
    count = 0;
    // int countA = 0;
    countB = 0;
    interval = 10;
}

int Encoder::position() {
    return counter;
}

void Encoder::updateCount(bool A, bool B) {
    if (B != A) {
        counter++;
    } else {
        counter--;
    }
}

float Encoder::speedPPS() {
    unsigned long currentMillis = millis();
    count = position();
    while(previousMillis - currentMillis <= interval) {
        previousMillis = millis();
        countB = position();
    }
    int PulsesPerTenmSec = countB - count;
    return PulsesPerTenmSec;
}

float Encoder::speedRPM() {
    int PPtenms = speedPPS();
    float RPM = ((float)PPtenms * 100 / (7.0 * 100.0)) * 60.0;
    int RPMint = int(RPM);
    return RPMint;
}