#include "Arduino.h"
#include "Context.h"
#include "InitState.h"
#include "StopState.h"
#include "OpState.h"

void StopState::on_do() {
}

void StopState::on_entry() {
    Serial.println("Operational state entry: turn ON GREEN LIGHT");
}

void StopState::on_exit() {
    Serial.println("Operational state exit: turn OFF GREEN LIGHT");
}

void StopState::on_reset() {
    //this->context_->transition_to(new InitState);
}

void StopState::on_stop() {
    //this->context_->transition_to(new InitState);
}

void StopState::on_back_to_OpState() {
    //this->context_->transition_to(new OpState);
}

void StopState::on_back_to_PreOpState() {
    //this->context_->transition_to();
}