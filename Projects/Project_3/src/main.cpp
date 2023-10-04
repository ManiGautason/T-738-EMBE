#include <Arduino.h>
#include "digital_in.h"
#include "digital_out.h"
#include "analog_out.h"
#include <encoder.h>

// Includes for STATE MACHINE
#include "State.h"
#include "Context.h"
#include "InitState.h"
#include "PreOpState.h"
#include "OpState.h"
#include "StopState.h"

Context *context;


Digital_in A(2);        //PD2 for the signal A, D2
Digital_in B(3);        //PD3 for the signal B, D3
Digital_in flt_pin(4);  //PD4 for the motor controller fault
Analog_out analog(4);   
Encoder enc;

int initFlag = 0;
int stopFlag = 0;
int opFlag = 0;
int PreOpFlag = 0;

void setup() {
    Serial.begin(9600);
    sei();
}

void loop() {

    context = new Context(new InitState);
    //LED.init(); //
    while (true) {

        if(initFlag == 0){
            initFlag = 1; // Reset timer flag
            // Serial.println("Initializing Motor"); 
            context->do_work(); 
            PreOpFlag = 1;
            context->back_to_PreOpState(); 
        }

        //Stop motor due to fault
        if(flt_pin.is_lo() && stopFlag == 0){
            stopFlag = 1;
            context->stop();
        }
        // Fault has been fixed
        if(flt_pin.is_hi() && stopFlag == 1){
            stopFlag = 0;
            if (PreOpFlag == 1 && opFlag == 0){
              context->back_to_PreOpState();
            } 
            if(opFlag == 1 && PreOpFlag == 0){
              context->back_to_OpState();
            }
        }

        context->do_work(); 
        if (Serial.available() > 0)
            {
                // read the incoming byte:
                char command = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(command);


                if (command == 'r'){
                    initFlag = 0;
                    Serial.println("Executing Reset Command");
                    context->reset(); // From current state to initial state 
                }
            }
    }

        delete context;
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