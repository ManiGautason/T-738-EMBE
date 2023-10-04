#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

   
    # include <avr/io.h>

    class Digital_out
    {

    public:
        Digital_out(uint8_t pinNumber);

        void init();

        void set_hi();

        void set_lo();

        void toggle();

    private:
        /* data */
        uint8_t OUTPUT_PIN;
    };

#endif