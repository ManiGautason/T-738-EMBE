#include <encoder.h>
#include "digital_in.h"
#include <util/delay.h>
#include <digital_out.h>
#include <Arduino.h>

Encoder enc;
Digital_in A(2);//PD2 for the signal A, D2
Digital_in B(3);//PD3 for the signal B, D3

Digital_out LED(5);// Port B

int count = 0;
int countA = 0;
int countB = 0;

unsigned long StateChangeTime = 0;
unsigned long PreviousStateChangeTime = 0;
unsigned long StatePeriod = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 10; // 1 second (in milliseconds)
void setup(){
    Serial.begin(115200);
    A.initINT();    // Initialize A signal as interrupt
    B.init(); // Initialize B signal
    LED.init();
    // Enable global interrupts
    sei();
}
void loop(){

  // unsigned long currentMillis = millis();
  // count = enc.position();
  // // Serial.println(count);
  // while(previousMillis - currentMillis <= interval) {
  //   // It's been 1 second, perform your action here
  //   previousMillis = millis();
  //   countB = enc.position();
  //   // Update the previousMillis to the current time for the next interval
  //   //previousMillis = currentMillis;
  // }
  // int PulsesPerSecond = countB - count;
  // float RPM = ((float)PulsesPerSecond * 100 / (14.0 * 100.0)) * 60.0;
  // int RPMint = int(RPM);
  // //float RPM = (PulsesPerSecond / (7) * 60.0);
  // Serial.println(RPMint);
  // Serial.print("Pulses per 10 ms: ");
  // Serial.println(enc.speedPPS());
  unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= 1000) {
    // It's been 1 second, perform your action here
        Serial.print("PPtenms: ");
        Serial.println(enc.speedPPS());
        previousMillis = millis();
    // Update the previousMillis to the current time for the next interval
    //previousMillis = currentMillis;
    }


}
  
// Define the ISR for INT0 (external interrupt)
ISR(INT0_vect) {
    // Call your interrupt action function
    enc.updateCount(A.is_hi(),B.is_hi());
    //Serial.println("A");
}