#include "State.h"

Analog_out State::analog(4);
Digital_out State::LED(5);
PI_controller State::PIcon(4,7,120,255,1.0);

State::~State() {
}

void State::set_context(Context *context) {
    this->context_ = context;
}