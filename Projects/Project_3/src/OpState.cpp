#include "InitState.h"
#include "Context.h"
#include "Arduino.h"
#include "OpState.h"

void OpState::on_do() {
}

void OpState::on_entry() {
    Serial.println("Operational state entry: turn ON GREEN LIGHT");
}

void OpState::on_exit() {
    Serial.println("Operational state exit: turn OFF GREEN LIGHT");
}

void OpState::on_event1() {
        this->context_->transition_to(new InitState);
}

void OpState::on_event2() {
    // optionally do something on transition

}