
/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */
#ifndef STATE_H
#define STATE_H
#include "analog_out.h"
#include "digital_out.h"
#include "digital_in.h"
#include "controller.h"
#include "PI_controller.h"
#include "encoder.h"

class Context;

class State {
protected:
    Context *context_;

public:
    virtual int get_state_id() = 0;
    virtual ~State();
    void set_context(Context *context);
    virtual void on_do() = 0;
    virtual void on_entry() = 0;
    virtual void on_exit() = 0;
    virtual void on_reset() = 0;
    virtual void on_stop() = 0;
    virtual void on_back_to_OpState() = 0;
    virtual void on_back_to_PreOpState() = 0;

    //static Analog_out analog;
    static Digital_out LED;
    // static Digital_in A;
    // static Digital_in B;   
    // static PI_controller PIcon;
    // static Encoder enc;
};

#endif // STATE_H
