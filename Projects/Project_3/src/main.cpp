// #include <encoder.h>
// #include "digital_in.h"
// #include <util/delay.h>
// #include <digital_out.h>
// #include <Arduino.h>
// #include <avr/interrupt.h>
// #include <analog_out.h>
// #include <PI_controller.h>
// Analog_out analog(4);
// //PI_controller controller(kp, ki, MAX_RPM, MAX_PWM, DELTA_T);
// PI_controller Pcon(3.6,0.0016,100,255,1);
// //PI_controller Pcon(0.8,1,0.001,100,255);
// Encoder enc;
// Digital_in A(2);//PD2 for the signal A, D2
// Digital_in B(3);//PD3 for the signal B, D3

// //Digital_out LED(5);// Port B

// int count = 0;
// int countA = 0;
// int countB = 0;
// int PV = 0;
// float PWM = 0;
// unsigned long StateChangeTime = 0;
// unsigned long PreviousStateChangeTime = 0;
// unsigned long StatePeriod = 0;
// unsigned long previousTime = 0;
// unsigned long previousTimeStep = 0;
// const unsigned long interval = 10; // 1 second (in milliseconds)
// int setpoint = 0;

// void setup(){
//     Serial.begin(115200);
//     A.initINT();    // Initialize A signal as interrupt
//     B.init(); // Initialize B signal
//     //LED.init();
//     analog.init(); //default value of 1000ms
//     analog.set(0); // default value is 0.5
//     // Enable global interrupts
//     sei();
//     Serial.println("Setpoint, RPM, PWM");
// }
// void loop(){
  
//   int PPScount = enc.speedPPS();
//   int RPMcount = enc.speedRPM();

//   unsigned long currentTime = millis();
//     if(currentTime - previousTime >= 1) {
//         //Serial.print("PPtenms: ");
//         //Serial.println(PPScount);
//         //Serial.print("RPM: ");
        
//         Serial.print(setpoint);
//         Serial.print(",");
//         Serial.print(RPMcount);
//         Serial.print(",");
//         previousTime = millis();
//         Serial.print(PV);
//         Serial.println();
//         PV = Pcon.update(setpoint,RPMcount);
//         PWM = (1.0/255.0)*float(PV);
//         analog.set(PWM);
//     }

//     unsigned long currentTimeStep = millis();
//     if(currentTimeStep - previousTimeStep >= 5000) {
//       setpoint = 90;
//     }

// }
  
// // Define the ISR for INT0 (external interrupt)
// ISR(INT0_vect) {
//     // Call your interrupt action function
//     enc.updateCount(A.is_hi(),B.is_hi());
//     //Serial.println("A");
// }
// ISR (TIMER1_COMPA_vect)
// {
// // action to be taken at the start of the on-cycle
//   analog.pin.set_hi();
// }
// ISR (TIMER1_COMPB_vect)
// {
// // action to be taken at the start of the off-cycle
//   analog.pin.set_lo();
// }

#include <Arduino.h>
#include "digital_in.h"
#include "digital_out.h"
#include "analog_out.h"
#include <encoder.h>
<<<<<<< Updated upstream
#include <PI_controller.h>
=======
#include <controller.h>
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
PI_controller Pcon(3.6,0.0016,100,255,1);

=======


>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    // Initialize the state machine in the Initialization state
=======
    //Initialize the state machine in the Initialization state
>>>>>>> Stashed changes
    if (initFlag == 0) {
        initFlag = 1; // Reset initialization flag

        // Transition to the Initialization state
        context = new Context(new InitState());
        Serial.println("Initializing Motor");

        //Perform initialization tasks
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
<<<<<<< Updated upstream
=======
                    //PIcon.set_kp(K_p);
>>>>>>> Stashed changes

                } else if (command == 't') {
                    Serial.println("Enter value for T_i: ");
                    T_i = readAndParseInt();
<<<<<<< Updated upstream
=======
                    //PIcon.set_kp(T_i);
>>>>>>> Stashed changes
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
    //context->do_work();
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