#include <encoder.h>
#include "digital_in.h"
#include <util/delay.h>
#include <digital_out.h>
#include <Arduino.h>
int main(){
  Digital_in A(3);//PB3 for the signal A, D11
  Digital_in B(4);//PB4 for the signal B, D12
  Digital_out LED(5);
  Encoder enc;
  A.init();    // Initialize LED pin
  B.init(); // Initialize button pin
  LED.init();
  Serial.begin(115200);
  int count = 0;
  int countB = 0;
  while (1)
  {
    enc.updateCount(A.is_hi(),B.is_hi());
    countB = enc.position();
    Serial.println(countB);
    if(count != countB) {
      LED.toggle();
      count = countB;
    }
  
  }
  
  

  

}