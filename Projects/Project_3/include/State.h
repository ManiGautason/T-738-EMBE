
/**
 * The base State class declares methods that all concrete states should
 * implement and also provides a backreference to the Context object, associated
 * with the State. This backreference can be used by States to transition the
 * Context to another State.
 */
#ifndef STATE_H
#define STATE_H

class Context;

class State {
protected:
    Context *context_;

public:
    virtual ~State();
    void set_context(Context *context);
    virtual void on_do() = 0;
    virtual void on_entry() = 0;
    virtual void on_exit() = 0;
    virtual void on_reset() = 0;
    virtual void on_stop() = 0;
    virtual void on_back_to_OpState() = 0;
    virtual void on_back_to_PreOpState() = 0;
    
};

#endif // STATE_H
