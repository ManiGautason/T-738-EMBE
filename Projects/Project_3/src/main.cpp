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

// Constants for state IDs
#define INIT_STATE 1
#define PRE_OP_STATE 2
#define OP_STATE 3
#define STOP_STATE 4

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
    static int current_state = INIT_STATE;  // Initialize to INIT_STATE

    // Handle state transitions
    switch (current_state) {
        case INIT_STATE:
            // Initialize the state machine in the Initialization state
            if (current_state == INIT_STATE) {
                // Transition to the Initialization state
                context = new Context(new InitState);
                Serial.println("Initializing Motor");

                // Perform initialization tasks
                context->do_work();

                // Transition to the Pre-operational state
                context->back_to_PreOpState();
                current_state = PRE_OP_STATE;
            }
            break;

        case PRE_OP_STATE:
            // Handle configuration commands and transitions
            if (Serial.available() > 0) {
                char command = Serial.read();
                Serial.print("I received: ");
                Serial.println(command);

                switch (command) {
                    case 's':
                        // Transition to Operational state
                        opFlag = 1;
                        PreOpFlag = 0;
                        context->transition_to(new OpState);
                        current_state = OP_STATE;
                        break;

                    case 'r':
                        // Reset to Initialization state
                        current_state = INIT_STATE;
                        break;

                    // Handle other commands as needed
                    default:
                        break;
                }
            }
            break;

        case OP_STATE:
            // Handle operational commands and transitions
            if (Serial.available() > 0) {
                char command = Serial.read();
                Serial.print("I received: ");
                Serial.println(command);

                switch (command) {
                    case 'p':
                        // Transition to Pre-operational state
                        opFlag = 0;
                        PreOpFlag = 1;
                        context->transition_to(new PreOpState);
                        current_state = PRE_OP_STATE;
                        break;

                    case 'r':
                        // Reset to Initialization state
                        current_state = INIT_STATE;
                        break;

                    // Handle other commands as needed
                    default:
                        break;
                }
            }
            break;

        case STOP_STATE:
            // Handle commands and transitions in the Stopped state
            if (Serial.available() > 0) {
                char command = Serial.read();
                Serial.print("I received: ");
                Serial.println(command);

                switch (command) {
                    case 'c':
                        // Transition back to the appropriate state
                        if (PreOpFlag == 1) {
                            context->transition_to(new PreOpState);
                            current_state = PRE_OP_STATE;
                        } else if (opFlag == 1) {
                            context->transition_to(new OpState);
                            current_state = OP_STATE;
                        }
                        break;

                    // Handle other commands as needed
                    default:
                        break;
                }
            }
            break;

        default:
            // Handle other states or commands as needed
            break;
    }

    // Continue performing tasks within the current state
    context->do_work();


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