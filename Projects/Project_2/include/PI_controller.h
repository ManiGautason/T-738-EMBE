
class PI_controller{
  private:
  double k_p;
  double t_i;
  double dt_;
  double Error;
  int max_rpm_;
  int max_pwm_;
  public:
    PI_controller(double K_p,double T_i,double dt, int MAX_RPM, int MAX_PWM);  
    int update(double Setpoint, double current_Val);
};
