#include "digital_out.h"


void Digital_out::init()
{
    DDRB |= OUTPUT_PIN; //Set pin as output
}

Digital_out::Digital_out(uint8_t pinNumber)
{
    Digital_out::OUTPUT_PIN = (1 << pinNumber);
}


void Digital_out::set_hi()
{
    PORTB |= OUTPUT_PIN; //Turn on pin
}

void Digital_out::set_lo()
{
    PORTB &= ~OUTPUT_PIN; //Turn off pin
}

void Digital_out::toggle()
{
    PORTB ^= OUTPUT_PIN; //toggle pin
}
