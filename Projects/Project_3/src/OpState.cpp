#include "InitState.h"
#include "Context.h"
#include "Arduino.h"
#include "PreOpState.h"
#include "OpState.h"
#include "StopState.h"
#include "analog_out.h"
#include "encoder.h"
#include "digital_in.h"

void OpState::on_do() {
    analog.set(0.7);
    //Serial.println("OPSTATEDOO:");
    // int PPScount = enc.speedPPS();
    // int RPMcount = enc.speedRPM();
    // Serial.println(RPMcount);
    LED.set_hi();
    // PV = PIcon.update(setpoint,RPMcount);
    // PWM = (1.0/255.0)*float(PV);
    // // analog.set(PWM);
}

void OpState::on_entry() {
    Serial.println("Operational state entry:");
    LED.init();
}

void OpState::on_exit() {
    Serial.println("Operational state exit:");
}

void OpState::on_reset() {
    this->context_->transition_to(new InitState);
}

void OpState::on_stop() {
    // optionally do something on transition
    this->context_->transition_to(new StopState);

}

void OpState::on_back_to_OpState() {
    //this->context_->transition_to();
}

void OpState::on_back_to_PreOpState() {
    this->context_->transition_to(new PreOpState);
}
