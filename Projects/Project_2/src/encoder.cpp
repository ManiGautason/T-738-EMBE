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
    prevTime = 0;
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
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - prevTime;
    float pps = static_cast<float>(counter) / deltaTime * 1000.0;
    prevTime = currentTime;
    return pps;
}

float Encoder::speedRPM() {
    float pps = speedPPS();
    float rpm = pps / (encoder_pulses_per_revolution / 2) * 60.0;
    return rpm;
}

// Define the number of encoder pulses per revolution (you need to replace this value)
const int encoder_pulses_per_revolution = 1000; // Replace with your encoder's value