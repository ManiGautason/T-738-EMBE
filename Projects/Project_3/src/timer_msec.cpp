#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer_msec.h>

Timer_msec::Timer_msec()
{
}

void Timer_msec::init(int period_ms, float duty_cycle)
{
// this code sets up timer1 for a 1s @ 16Mhz Clock (mode 4)
// counting 16000000/1024 cycles of a clock prescaled by 1024
TCCR1A = 0; // set timer1 to normal operation (all bits in control registers A and B set to zero)
TCCR1B = 0; //
TCNT1 = 0; // initialize counter value to 0
OCR1A = ((16000000 / 1024 - 1)/1000)*period_ms; // assign target count to compare register A (must be less than 65536)
OCR1B = (OCR1A * duty_cycle); // assign target count to compare register B (should not exceed value of OCR1A)
TCCR1B |= (1 << WGM12); // clear the timer on compare match A
TIMSK1 |= (1 << OCIE1A); // set interrupt on compare match A
TIMSK1 |= (1 << OCIE1B); // set interrupt on compare match B
TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer

}
void Timer_msec::set_duty_cycle(float duty_cycle)
{
    OCR1B = OCR1A * duty_cycle; // assign target count to compare register B (should not exceed value of OCR1A)
}
