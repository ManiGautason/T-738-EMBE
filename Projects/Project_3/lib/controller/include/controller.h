#ifndef CONTROLLER
#define CONTROLLER

class controller {
public:
    controller(double K_p, int max_rpm, int max_pwm);
    virtual int update(double Setpoint, double current_Val);
    virtual ~controller() {}

protected:
    double k_p;
    int max_rpm_;
    int max_pwm_;
};

#endif