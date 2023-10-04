#ifndef CONTEXT_H
#define CONTEXT_H

#include "State.h"

class Context {
private:
    State *state_;

public:
    Context(State *state);
    ~Context();

    void transition_to(State *state);
    void do_work();
    void reset();
    void stop();
    void back_to_OpState();
    void back_to_PreOpState();
    int get_current_state_id();
};

#endif 
