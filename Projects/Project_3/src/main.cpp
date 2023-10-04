#include <Arduino.h>
#include "State.h"
#include "Context.h"
#include "OpState.h"
#include "InitState.h"

Context *context;

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
            context->event2(); // Yellow to Red
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
                    context->event1(); // From current state to initial state 
                }

            }
    }

        delete context;
}