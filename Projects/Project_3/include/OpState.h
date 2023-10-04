#ifndef OP_STATE_H
#define OP_STATE_H

#include "State.h"

class OpState : public State {
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