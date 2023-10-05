#ifndef CONTROLLER
    #define CONTROLLER

    class controller {
        public:
            virtual int update(double Setpoint, double current_Val);
            void set_kp(double kp);
            void set_ki(double ki);

            double k_p;
            double k_i;
            int max_rpm_;
            int max_pwm_;
            double Error;
        protected:
        double dt_;
    };

#endif
