// #include <stdint.h>

// class Encoder {

//     public:
//         Encoder(){
//             counter = 0;
//             lastA = 0;
//             lastB = 0;
//         };
//         int position();
//         volatile int counter;
//         void updateCount(bool A, bool B);

//     private:
//         bool lastA;
//         bool lastB;
        
// };


#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

class Encoder {
public:
    Encoder();
    int position();
    void updateCount(bool A, bool B);
    float speedPPS();
    float speedRPM();

private:
    bool lastA;
    bool lastB;
    volatile long counter;
    // unsigned long prevTime;
    unsigned long previousMillis;
    unsigned long interval; // 1 second (in milliseconds)
    volatile long count;
    volatile long countA;
    volatile long countB;
};

#endif