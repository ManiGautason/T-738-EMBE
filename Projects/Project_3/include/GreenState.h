#ifndef GREEN_STATE_H
#define GREEN_STATE_H

#include "State.h"

class GreenState : public State {
public:
    void on_do() override;
    void on_entry() override;
    void on_exit() override;
    void on_event1() override;
    void on_event2() override;
};


#endif
