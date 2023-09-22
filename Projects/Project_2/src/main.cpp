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
unsigned long previousTime = 0;
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

  int PPScount = enc.speedPPS();
  int RPMcount = enc.speedRPM();

  unsigned long currentTime = millis();
    if(currentTime - previousTime >= 1000) {
        Serial.print("PPtenms: ");
        Serial.println(PPScount);
        Serial.print("RPM: ");
        Serial.println(RPMcount);
        previousTime = millis();
    }
}
  
// Define the ISR for INT0 (external interrupt)
ISR(INT0_vect) {
    // Call your interrupt action function
    enc.updateCount(A.is_hi(),B.is_hi());
    //Serial.println("A");
}