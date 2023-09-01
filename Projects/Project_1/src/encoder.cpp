#include <encoder.h>

int getCount() const {
    return counter;
}

void update(bool A, bool B) {
    if (A != lastA && B == lastB) {
        // A transitioned from low to high while B stayed the same
        counter++;
    } else if (B != lastB && A == lastA) {
        // B transitioned from low to high while A stayed the same
        counter--;
    }
    
    lastA = A;
    lastB = B;
}