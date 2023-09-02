#include <encoder.h>

Encoder encoder;

int Encoder::position() {
    return encoder.counter;
}

void Encoder::updateCount(bool A, bool B) {
    if(A != encoder.lastA) {
        if(B != A) {
            encoder.counter++;

        }
        else {
            encoder.counter--;
        }
    }
    encoder.lastA = A;
}