
// class P_controller{
//   private:
//   double k_p;
//   int max_rpm_;
//   int max_pwm_;
//   public:
//     P_controller(double K_p, int MAX_RPM, int MAX_PWM);  
//     int update(double Setpoint, double current_Val);
// };

#ifndef P_CONTROLLER
#define P_CONTROLLER

#include "controller.h"

class P_controller : public controller {
public:
    P_controller(double K_p, int MAX_RPM, int MAX_PWM);  
    int update(double Setpoint, double current_Val) override;
};

#endif

