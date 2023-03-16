/*
Include all required header & cpp files
*/
#include <iostream>
#include "src/project.h"

int main()
{
    Counter counter;
    for (int index = 0; index < 10; index++)
    {
        counter.increment();
        std::cout << counter;
    }
    Salam gopnik;
    gopnik.saySalam();
    return 0;
}