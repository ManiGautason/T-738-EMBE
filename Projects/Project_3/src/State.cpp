#include "State.h"

//Analog_out State::analog(4);
Digital_out State::LED(5);
// Digital_in State::A(2);
// Digital_in State::B(3);
// PI_controller State::PIcon(3.6, 1.66, 100, 255, 1);
// Encoder State::enc;

State::~State() {
}

void State::set_context(Context *context) {
    this->context_ = context;
}