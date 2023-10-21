#include "controller.h"

controller::controller(double K_p, int max_rpm, long max_pwm) {
    k_p = K_p;
    max_rpm_ = max_rpm;
    max_pwm_ = max_pwm;
}

int controller::update(double Setpoint, double current_Val) {
    double u = k_p * (Setpoint - current_Val) / max_rpm_;
    u = u * max_pwm_;
    if (u > 100000) u = 99999;
    if (u < 0) u = 1;

    return (long)u;
}