#include "InitState.h"
#include "OpState.h"
#include "Context.h"
#include "Arduino.h"

void InitState::on_do() {
     Serial.println("Init state: initializing device...");
}

void InitState::on_entry() {
    Serial.println("Init state entry");
    
}

void InitState::on_exit() {
    Serial.println("Init state exit");
}

void InitState::on_event1() {
}

void InitState::on_event2() {
    // optionally do something on transition
    this->context_->transition_to(new OpState);
}