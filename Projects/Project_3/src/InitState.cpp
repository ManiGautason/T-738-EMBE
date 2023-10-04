#include "InitState.h"
#include "OpState.h"
#include "Context.h"
#include "Arduino.h"
#include "State.h"

void InitState::on_do() {
     Serial.println("Init state: initializing device...");
}

void InitState::on_entry() {
    Serial.println("Init state entry");
    
}

void InitState::on_exit() {
    Serial.println("Init state exit");
}

void InitState::on_reset() {
}

void InitState::on_stop() {
}

void InitState::on_back_to_OpState() {
    // optionally do something on transition
    //this->context_->transition_to(new OpState);
}

void InitState::on_back_to_PreOpState() {
    // optionally do something on transition
    this->context_->transition_to(new OpState);
}
