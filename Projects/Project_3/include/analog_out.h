#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

#include <timer_msec.h>
#include <digital_out.h>

class Analog_out
{
public:
    Analog_out(int pin);
    void init(int period_ms = 2);
    void set(float duty_cycle);

private:
    Timer_msec timer;
public:    
    Digital_out pin;
};

#endif

