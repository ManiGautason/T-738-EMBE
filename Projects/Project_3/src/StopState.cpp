#include "Arduino.h"
#include "State.h"
#include "Context.h"
#include "InitState.h"
#include "StopState.h"
#include "PreOpState.h"
#include "OpState.h"

void StopState::on_do() {
    analog.set(0);
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        // Save the last time you blinked the LED
        previousMillis = currentMillis;
        LED.toggle(); // Toggle the LED state
    }
}

void StopState::on_entry() {
    Serial.println("Stop state entry:");
    previousMillis = millis();  // Initialize previousMillis
    LED.init();                 // Initialize the LED
    // TURN OFF MOTOR
}

void StopState::on_exit() {
    Serial.println("Stop state exit:");
}

void StopState::on_reset() {
    this->context_->transition_to(new InitState);
}

void StopState::on_stop() {
    //this->context_->transition_to(new InitState);
}

void StopState::on_back_to_OpState() {
    this->context_->transition_to(new OpState);
}

void StopState::on_back_to_PreOpState() {
    this->context_->transition_to(new PreOpState);
}