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
    volatile int counter;
    unsigned long prevTime;
};

#endif
