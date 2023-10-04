#include "P_controller.h"
#include <avr/io.h>
P_controller::P_controller(double K_p, int max_rpm, int max_pwm) {
  k_p=K_p;
  max_rpm_ = max_rpm;
  max_pwm_ = max_pwm;
}

int P_controller::update(double Setpoint, double curent_Val){
  
  double u=k_p*(Setpoint-curent_Val)/max_rpm_;
  u = u*max_pwm_;
  if (u>255) u=254;
  if (u<0) u=1;

  return (int)u;
}
