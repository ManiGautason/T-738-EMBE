#include <encoder.h>
#include "digital_in.h"
#include <util/delay.h>
#include <digital_out.h>
#include <Arduino.h>
#include <avr/interrupt.h>
#include <analog_out.h>
#include <P_controller.h>
Analog_out analog(4);
P_controller Pcon(7,120,255);
Encoder enc;
Digital_in A(2);//PD2 for the signal A, D2
Digital_in B(3);//PD3 for the signal B, D3

//Digital_out LED(5);// Port B

int count = 0;
int countA = 0;
int countB = 0;
int PV = 0;
float PWM = 0;
unsigned long StateChangeTime = 0;
unsigned long PreviousStateChangeTime = 0;
unsigned long StatePeriod = 0;
unsigned long previousTime = 0;
unsigned long previousTimeStep = 0;
const unsigned long interval = 10; // 1 second (in milliseconds)
int setpoint = 0;

void setup(){
    Serial.begin(115200);
    A.initINT();    // Initialize A signal as interrupt
    B.init(); // Initialize B signal
    //LED.init();
    analog.init(); //default value of 1000ms
    analog.set(0.99); // default value is 0.5
    // Enable global interrupts
    sei();
    Serial.println("Setpoint, RPM, PWM");
}
void loop(){
  
  int PPScount = enc.speedPPS();
  int RPMcount = enc.speedRPM();

  unsigned long currentTime = millis();
    if(currentTime - previousTime >= 1) {
        //Serial.print("PPtenms: ");
        //Serial.println(PPScount);
        //Serial.print("RPM: ");
        
        Serial.print(setpoint);
        Serial.print(",");
        Serial.print(RPMcount);
        Serial.print(",");
        previousTime = millis();
        Serial.print(PV);
        Serial.println();
        PV = Pcon.update(setpoint,RPMcount);
        PWM = (1.0/255.0)*float(PV);
        analog.set(PWM);
    }

    unsigned long currentTimeStep = millis();
    if(currentTimeStep - previousTimeStep >= 10000) {
      setpoint = 90;
    }

}
  
// Define the ISR for INT0 (external interrupt)
ISR(INT0_vect) {
    // Call your interrupt action function
    enc.updateCount(A.is_hi(),B.is_hi());
    //Serial.println("A");
}
ISR (TIMER1_COMPA_vect)
{
// action to be taken at the start of the on-cycle
  analog.pin.set_hi();
}
ISR (TIMER1_COMPB_vect)
{
// action to be taken at the start of the off-cycle
  analog.pin.set_lo();
}