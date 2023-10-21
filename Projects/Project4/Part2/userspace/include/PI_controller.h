#ifndef PI_CONTROLLER
#define PI_CONTROLLER

#include "controller.h"

class PI_controller : public controller {
public:
    PI_controller(double K_p, double K_i, int max_rpm, long max_pwm, float dt);
    int update(double Setpoint, double current_Val) override;

private:
    double k_i;
    double Error;
    double dt_;
};

#endif