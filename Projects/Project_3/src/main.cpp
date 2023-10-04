#include <Arduino.h>
#include "digital_in.h"
#include "State.h"
#include "Context.h"
#include "InitState.h"
#include "PreOpState.h"
#include "OpState.h"
#include "StopState.h"



Context *context;
Digital_in flt_pin(10);


int initFlag = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {

    context = new Context(new InitState);

    while (true) {

        if(initFlag == 0){
            initFlag = 1; // Reset timer flag
            // Serial.println("Initializing Motor"); 
            context->do_work(); // Yellow: Initialize timer
            context->back_to_PreOpState(); // Yellow to Red
        }


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