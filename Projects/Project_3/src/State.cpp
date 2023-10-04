#include "State.h"

Analog_out State::analog(4);
Digital_out State::LED(5);

State::~State() {
}

void State::set_context(Context *context) {
    this->context_ = context;
}