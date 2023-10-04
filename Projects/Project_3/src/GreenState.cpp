#include "GreenState.h"
#include "YellowState.h"
#include "RedState.h"
#include "Context.h"
#include "Arduino.h"

void GreenState::on_do() {
}

void GreenState::on_entry() {
    Serial.println("Greenstate entry: turn ON GREEN LIGHT");
}

void GreenState::on_exit() {
    Serial.println("Greenstate exit: turn OFF GREEN LIGHT");
}

void GreenState::on_event1() {
}

void GreenState::on_event2() {
    // optionally do something on transition
    this->context_->transition_to(new YellowState);
}