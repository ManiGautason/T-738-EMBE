#include "InitState.h"
#include "Context.h"
#include "Arduino.h"
#include "PreOpState.h"
#include "StopState.h"
#include "OpState.h"


void PreOpState::on_do() {
    unsigned long currMillis = millis();

    if (currMillis - prevMillis >= interval) {
        // Save the last time you blinked the LED
        prevMillis = currMillis;
        LED.toggle(); // Toggle the LED state
    }
}

void PreOpState::on_entry() {
    Serial.println("PreOp state entry:");
    prevMillis = millis(); // Initialize previousMillis

}

void PreOpState::on_exit() {
    Serial.println("PreOp state exit:");
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

void PreOpState::on_back_to_PreOpState() {
    //this->context_->transition_to();
}