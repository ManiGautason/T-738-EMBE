#include "digital_in.h"
//#include <avr/interrupt.h>

Digital_in::Digital_in(uint8_t pin) {
  pinMask = 1 << pin;
}

void Digital_in::init() {
  DDRD &= ~pinMask;     // Set the corresponding pin as an input
  PORTD |= pinMask;     // Enable internal pull-up resistor
}
void Digital_in::initINT() {
  DDRD &= ~pinMask;     // Set the corresponding pin as an input
  EIMSK |= (1 << INT0); 
  EICRA |= (1 << ISC01) | (1 << ISC00);
}

bool Digital_in::is_hi() {
  return (PIND & pinMask) != 0;     // Check if the correspoding pin is high
}

bool Digital_in::is_lo() {
  return (PIND & pinMask) == 0;     // Check if the correspoding pin is low
}
