#include "Context.h"
#include "State.h"
#include "Arduino.h"
Context::Context(State *state) : state_(nullptr) {
    this->transition_to(state);
}

Context::~Context() {
    delete state_;
}

void Context::transition_to(State *state) {
    if (this->state_ != nullptr) {
        this->state_->on_exit();
        delete this->state_;
    }

    this->state_ = state;

    this->state_->set_context(this);
    this->state_->on_entry();
}

void Context::do_work() {
    this->state_->on_do();
}

void Context::reset() {
    this->state_->on_reset();
}

void Context::stop() {
    this->state_->on_stop();
}

void Context::back_to_OpState() {
    this->state_->on_back_to_OpState();
}

void Context::back_to_PreOpState() {
    this->state_->on_back_to_PreOpState();
}