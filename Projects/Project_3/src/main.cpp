#include <Arduino.h>
#include "digital_in.h"
#include "State.h"
#include "Context.h"
#include "OpState.h"
#include "InitState.h"
#include "StopState.h"
#include "PreOpState.h"


Context *context;
Digital_in flt_pin(10);


int initFlag = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {


    context = new Context(new InitState);
    context->do_work(); // Green to Yellow
    context->back_to_PreOpState(); // Green to Yellow


    while (true) {

        // Check if Fault in motor controller
        if (flt_pin.is_lo()){
            context->stop();
        }


        if (Serial.available() > 0)
            {
                // read the incoming byte:
                char command = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(command);

                // if(initFlag == 0){
                //     Serial.println("Initializing Motor"); 
                //     context->do_work(); // Yellow: Initialize timer
                //     initFlag = 1; // Reset timer flag
                //     context->event1(); // Yellow to Red
                // }

                if (command == 'r'){
                    initFlag = 0;
                    Serial.println("Executing Reset Command");

                }

            }
    }

        delete context;
}