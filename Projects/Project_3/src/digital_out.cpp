#include "digital_out.h"
#include <avr/io.h>
Digital_out::Digital_out(uint8_t pinNumber):pinMask(0) {
    // Calculate the pin mask based on the pin number
    pinMask = 1 << pinNumber;
}

void Digital_out::init() {
    // Set the corresponding bit in the DDR register to make the pin an output
    DDRB |= pinMask;
}

void Digital_out::set_hi() {
    // Set the corresponding bit in the PORT register to set the pin high
    PORTB |= pinMask;
}

void Digital_out::set_lo() {
    // Clear the corresponding bit in the PORT register to set the pin low
    PORTB &= ~pinMask;
}

void Digital_out::toggle() {
    // Toggle the corresponding bit in the PORT register
    PORTB ^= pinMask;
}
