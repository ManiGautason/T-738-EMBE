#ifndef PREOP_STATE_H
#define PREOP_STATE_H

#include "State.h"

class PreOpState : public State {
public:
    void on_do() override;
    void on_entry() override;
    void on_exit() override;
    void on_reset() override;
    void on_stop() override;
    void on_back_to_OpState() override;
    void on_back_to_PreOpState() override;
    static const int PRE_OP_STATE = 2;
    int get_state_id() override {
        return PRE_OP_STATE;
    }
private:
    unsigned long previousMillis;
    const unsigned long interval = 500; // Blink interval in milliseconds
};


#endif