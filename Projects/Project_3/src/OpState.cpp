#include "InitState.h"
#include "Context.h"
#include "Arduino.h"
#include "OpState.h"
#include "StopState.h"
#include "analog_out.h"
#include "encoder.h"
#include "digital_in.h"

void OpState::on_do() {
    analog.set(0.7);
}

void OpState::on_entry() {
    Serial.println("Operational state entry: turn ON GREEN LIGHT");
}

void OpState::on_exit() {
    Serial.println("Operational state exit: turn OFF GREEN LIGHT");
}

void OpState::on_reset() {
    this->context_->transition_to(new InitState);
}

void OpState::on_stop() {
    // optionally do something on transition
    //this->context_->transition_to(new StopState);

}

void OpState::on_back_to_OpState() {
    //this->context_->transition_to();
}

void OpState::on_back_to_PreOpState() {
    //this->context_->transition_to();
}
