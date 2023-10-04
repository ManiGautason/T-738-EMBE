#ifndef STOP_STATE_H
#define STOP_STATE_H

#include "State.h"

class StopState : public State {
public:
    void on_do() override;
    void on_entry() override;
    void on_exit() override;
    void on_reset() override;
    void on_stop() override;
    void on_back_to_OpState() override;
    void on_back_to_PreOpState() override;
    static const int STOP_STATE = 4;
    int get_state_id() override {
        return STOP_STATE;
    }
private:
    unsigned long previousMillis;
    const unsigned long interval = 250; // Blink interval in milliseconds
};


#endif