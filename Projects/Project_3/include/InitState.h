#ifndef INIT_STATE_H
#define INIT_STATE_H

#include "State.h"
#include "analog_out.h"

class InitState : public State {
public:
    void on_do() override;
    void on_entry() override;
    void on_exit() override;
    void on_reset()override;
    void on_stop() override;
    void on_back_to_OpState() override;
    void on_back_to_PreOpState() override;

};


#endif