#include <avr/io.h>
#include <avr/interrupt.h>

class Digital_in {
public:
  Digital_in(uint8_t pin);
  void init();
  bool is_hi();
  bool is_lo();
  void initINT(); 
private:
  uint8_t pinMask;
};
