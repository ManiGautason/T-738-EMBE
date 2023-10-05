#include "PI_controller.h"
#include <avr/io.h>



     PI_controller::PI_controller(double K_p, double K_i, int max_rpm, int max_pwm, float dt) {
        k_p = K_p;
        k_i = K_i;
        max_rpm_ = max_rpm;
        max_pwm_ = max_pwm;
        Error = 0;
        dt_ = dt/1000;

    }

    int PI_controller::update(double Setpoint, double current_Val) {
        if (current_Val < max_rpm_) {
            Error += (Setpoint-current_Val)*dt_;
        }
        double u=k_p*(Setpoint-current_Val);
        if (k_i != 0.0) {
            u = u + (k_p/k_i)*Error;
        }
        u = u/max_rpm_;
        u = u*max_pwm_;
        if (u>255) u=254;
        if (u<0) u=1;

        return (int)u;
    }

