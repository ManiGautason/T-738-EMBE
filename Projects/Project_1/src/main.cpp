#include <encoder.h>
#include "digital_in.h"
#include <util/delay.h>
#include <digital_out.h>
#include <Arduino.h>

Encoder enc;
Digital_in A(2);//PD2 for the signal A, D2
Digital_in B(3);//PD3 for the signal B, D3

int main(){

  Digital_out LED(5);// Port B
  A.initINT();    // Initialize A signal as interrupt
  B.init(); // Initialize B signal
  LED.init();
  // Enable global interrupts
  sei();
  Serial.begin(115200);
  int count = 0;
  int countB = 0;
  while (1)
   {
  //   enc.updateCount(A.is_hi(),B.is_hi());
  //   countB = enc.position();
  //   Serial.println(countB);
  //   if(count != countB) {
  //     LED.toggle();
  //     count = countB;
  //   }
    countB = enc.position();
    Serial.println(countB);
  } 
}
  
// Define the ISR for INT0 (external interrupt)
ISR(INT0_vect) {
    // Call your interrupt action function
    enc.updateCount(A.is_hi(),B.is_hi());
    //Serial.println("A");
}