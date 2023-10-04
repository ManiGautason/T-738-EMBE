#include "InitState.h"
#include "Context.h"
#include "Arduino.h"
#include "OpState.h"

void OpState::on_do() {
}

void OpState::on_entry() {
    Serial.println("Greenstate entry: turn ON GREEN LIGHT");
}

void OpState::on_exit() {
    Serial.println("Greenstate exit: turn OFF GREEN LIGHT");
}

void OpState::on_event1() {
}

void OpState::on_event2() {
    // optionally do something on transition
    this->context_->transition_to(new InitState);
}