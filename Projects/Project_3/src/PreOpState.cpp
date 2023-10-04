#include "InitState.h"
#include "Context.h"
#include "Arduino.h"
#include "PreOpState.h"
#include "StopState.h"
#include "OpState.h"


void PreOpState::on_do() {
}

void PreOpState::on_entry() {
    Serial.println("Operational state entry: turn ON GREEN LIGHT");
}

void PreOpState::on_exit() {
    Serial.println("Operational state exit: turn OFF GREEN LIGHT");
}

void PreOpState::on_reset() {
    this->context_->transition_to(new InitState);
}

void PreOpState::on_stop() {
    // optionally do something on transition
    this->context_->transition_to(new StopState);
}

void PreOpState::on_back_to_OpState() {
    //this->context_->transition_to();
}

