#include <iostream>

class Salam
{
private:
    int salam_counter;

public:
    Salam();
    ~Salam();
    void saySalam();
};

Salam::Salam()
{
    std::cout << "\nSalam works!\n";
}
Salam::~Salam()
{
    std::cout << "destroying object!\n";
}

void Salam::saySalam()
{
    std::cout << "Salam yopta, djalla!\n";
    this->salam_counter++;
}