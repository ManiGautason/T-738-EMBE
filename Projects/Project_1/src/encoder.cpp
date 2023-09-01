#include <encoder.h>

Encoder encoder;

int Encoder::position() {
    return encoder.counter;
}

void Encoder::updateCount(bool A, bool B) {
    if (A != encoder.lastA && B == encoder.lastB) {
        // A transitioned from low to high while B stayed the same
        encoder.counter++;
    } else if (B != encoder.lastB && A == encoder.lastA) {
        // B transitioned from low to high while A stayed the same
        encoder.counter--;
    }
    
    encoder.lastA = A;
    encoder.lastB = B;
}