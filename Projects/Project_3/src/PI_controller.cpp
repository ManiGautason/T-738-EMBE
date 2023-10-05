#include "PI_controller.h"
#include <avr/io.h>



PI_controller::PI_controller(double K_p,double T_i,double dt, int max_rpm, int max_pwm) {
  k_p=K_p;
  t_i = T_i;
  max_rpm_ = max_rpm;
  max_pwm_ = max_pwm;
  Error = 0;
  dt_ = dt;
}

int PI_controller::update(double Setpoint, double curent_Val){
  
  
  Error += (Setpoint-curent_Val)*dt_;

  double u=k_p*(Setpoint-curent_Val) + (k_p/t_i)*Error;
  u = u/max_rpm_;
  u = u*max_pwm_;
  if (u>255) u=254;
  if (u<0) u=1;

  return (int)u;
}
