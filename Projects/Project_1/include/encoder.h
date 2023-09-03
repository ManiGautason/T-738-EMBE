#include <stdint.h>

class Encoder {
public:
    Encoder(){
        counter = 0;
        lastA = 0;
        lastB = 0;
    };
    int position();
    volatile int counter;
    void updateCount(bool A, bool B);
private:
    
    bool lastA;
    bool lastB;



};