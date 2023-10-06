#include "State.h"

Analog_out State::analog(4);
Digital_out State::LED(5);
P_controller State::Pcon(7,120,255);

State::~State() {
}

void State::set_context(Context *context) {
    this->context_ = context;
}