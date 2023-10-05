#include <Arduino.h>
#include "digital_in.h"
#include "digital_out.h"
#include "analog_out.h"
#include <encoder.h>
#include <P_controller.h>

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
P_controller Pcon(7,120,255);

int initFlag = 0;
int stopFlag = 0;
int opFlag = 0;
int PreOpFlag = 0;
int K_p = 0;
int T_i = 0;  


// Function to read and parse an integer from Serial input
int readAndParseInt() {
    Serial.println("Enter an integer value: ");

    String inputBuffer = "";
    
    while (true) {
        if (Serial.available()) {
            char inputChar = Serial.read();
            if (isdigit(inputChar)) {
                inputBuffer += inputChar;
            } else if (inputChar == '\n' || inputChar == ' ') {
                if (inputBuffer.length() > 0) {
                    int parsedValue = inputBuffer.toInt();
                    Serial.println("Received value: " + String(parsedValue));
                    return parsedValue;
                }
                break;
            }
        }
    }
    return 0;
}


void setup() {
    Serial.begin(9600);
    sei();
}


void loop() {
    // Initialize the state machine in the Initialization state
    if (initFlag == 0) {
        initFlag = 1; // Reset initialization flag

        // Transition to the Initialization state
        context = new Context(new InitState);
        Serial.println("Initializing Motor");

        // Perform initialization tasks
        context->do_work();

        // Transition to the Pre-operational state
        context->back_to_PreOpState();
        PreOpFlag = 1;
    }

    // Check for fault condition
    if (flt_pin.is_lo() && stopFlag == 0) {
        stopFlag = 1;

        // Transition to the Stopped state
        context->stop();
    }

    // Handle serial commands
    if (Serial.available() > 0) {
        char command = Serial.read();
        Serial.print("I received: ");
        Serial.println(command);

        // Handle commands based on the current state
        switch (context->get_current_state_id()) {
            case INIT_STATE:
                Serial.print("INIT_STATE");
                if (command == 'r') {
                    initFlag = 0;
                    Serial.println("Executing Reset Command");
                    context->reset(); // Transition to Initialization state
                }
                break;

            case PRE_OP_STATE:
                Serial.print("PRE_OP_STATE");
                // Handle configuration commands and transitions
                if (command == 's') {
                    // Transition to Operational state
                    opFlag = 1;
                    PreOpFlag = 0;
                    context->transition_to(new OpState);
                } else if (command == 'r') {
                    initFlag = 0;
                    Serial.println("Executing Reset Command");
                    context->reset(); // Transition to Initialization state
                }
                else if (command == 'k') {
                    Serial.println("Enter value for K_p: ");
                    K_p = readAndParseInt();

                } else if (command == 't') {
                    Serial.println("Enter value for T_i: ");
                    T_i = readAndParseInt();
                }
                break;

            case OP_STATE:
                Serial.print("OP_STATE");
                // Handle operational commands and transitions
                if (command == 'p') {
                    // Transition to Pre-operational state
                    opFlag = 0;
                    PreOpFlag = 1;
                    context->transition_to(new PreOpState);
                } else if (command == 'r') {
                    initFlag = 0;
                    Serial.println("Executing Reset Command");
                    context->reset(); // Transition to Initialization state
                }
                break;

            case STOP_STATE:
                Serial.print("STOP_STATE");
                // Handle commands and transitions in the Stopped state
                if (command == 'c') {
                    stopFlag = 0;
                    // Transition back to the appropriate state
                    if (PreOpFlag == 1) {
                        context->transition_to(new PreOpState);
                    } else if (opFlag == 1) {
                        context->transition_to(new OpState);
                    }
                }
                break;

            default:
                
                break;
        }
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