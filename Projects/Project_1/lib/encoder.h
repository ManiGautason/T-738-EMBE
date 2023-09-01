class Encoder {
private:
    int counter;
    bool lastA;
    bool lastB;

public:
    Encoder() : counter(0), lastA(false), lastB(false) {}

    int getCount() const {}

    void update(bool A, bool B) {}
};