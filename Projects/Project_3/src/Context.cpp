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

void Context::event1() {
    this->state_->on_event1();
}

void Context::event2() {
    this->state_->on_event2();
}