// #include <encoder.h>

// Encoder encoder;

// int Encoder::position() {
//     return encoder.counter;
// }

// void Encoder::updateCount(bool A, bool B) {
//     //if(A != encoder.lastA) {
//         if(B != A) {
//             encoder.counter++;
//         }
//         else {
//             encoder.counter--;
//         }
//     //}
//     //encoder.lastA = A;
// }


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

// Define the number of encoder pulses per revolution (you need to replace this value)
// const int encoder_pulses_per_revolution = 1000; // Replace with your encoder's value




//   unsigned long currentMillis = millis();
//   count = enc.position();
//   // Serial.println(count);
//   while(previousMillis - currentMillis <= interval) {
//     // It's been 1 second, perform your action here
//     previousMillis = millis();
//     countB = enc.position();
//     // Update the previousMillis to the current time for the next interval
//     //previousMillis = currentMillis;
//   }
//   int PulsesPerSecond = countB - count;
//   float RPM = ((float)PulsesPerSecond * 100 / (14.0 * 100.0)) * 60.0;
//   int RPMint = int(RPM);
//   //float RPM = (PulsesPerSecond / (7) * 60.0);
//   Serial.println(RPMint);