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

private:
    unsigned long previousMillis;
    const unsigned long interval = 500; // Blink interval in milliseconds
};


#endif