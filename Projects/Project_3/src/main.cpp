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
    context->do_work(); // Green to Yellow
    context->event2(); // Green to Yellow


    while (true) {

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