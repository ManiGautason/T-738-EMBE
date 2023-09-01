#include "digital_in.h"

Digital_in::Digital_in(uint8_t pin) {
  pinMask = 1 << pin;
}

void Digital_in::init() {
  DDRB &= ~pinMask;     // Set the corresponding pin as an input
  PORTB |= pinMask;     // Enable internal pull-up resistor
}

bool Digital_in::is_hi() {
  return (PINB & pinMask) != 0;     // Check if the correspoding pin is high
}

bool Digital_in::is_lo() {
  return (PINB & pinMask) == 0;     // Check if the correspoding pin is low
}
