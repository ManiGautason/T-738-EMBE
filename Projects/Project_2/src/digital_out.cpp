#include "digital_out.h"

Digital_out::Digital_out(uint8_t pin) {
  pinMask = 1 << pin;
}

void Digital_out::init() {
  DDRB |= pinMask;    // Set the corresponding pin as an output
}

void Digital_out::set_hi() {
  PORTB |= pinMask;     // Set the pin high
}

void Digital_out::set_lo() {
  PORTB &= ~pinMask;      // Set the pin low
}

void Digital_out::toggle() {
  PORTB ^= pinMask;     // Toggle the pin state
}