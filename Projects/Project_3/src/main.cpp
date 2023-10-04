#include <Arduino.h>
#include "State.h"
#include "Context.h"
#include "OpState.h"
#include "InitState.h"

Context *context;

int timerFlag = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {

    Serial.println("Initializing Motor");
    Serial.println("Initializing Green State"); 
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

                if(timerFlag == 1){
                    context->do_work(); // Yellow: Initialize timer
                    timerFlag = 0; // Reset timer flag
                    context->event1(); // Yellow to Red
                }

                if (command == 'r'){
                    Serial.println("Executing Reset Command");

                }

            }
    }

        delete context;
}