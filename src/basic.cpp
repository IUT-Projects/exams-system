/*
Include all required standart library headers (iostream, string, vector, math ...)

Define your class:
* Constructors & destructors
* attributes
* private && public methods

Then implement all of them!

WHY: It is easy to read. You read code, what kind of attrs, methods it have. If you wanna read their implementation
you navigate to code. Otherwise, you have scroll many times, and project becomes less readable when its really big,

Examples:

ClassName::ClassName() {...} // default constructors.
ClassName::ClassName(param, param) {...} // parametrized constructors.
ClassName::~ClassName() {...} // destructor.

return_type ClassName::methodName(arguments) {...} // methods, functions of class

NOTE: you also able to access current object by using `this` pointer
NOTE: you have to include it in project.h (for using in main.cpp)
*/

#include <iostream>

// definition
class Counter
{
private:
    int counter{0};

public:
    Counter();
    Counter(int count);
    ~Counter();
    int getCount();
    void increment();
    friend void operator<<(std::ostream &o, Counter &c);
};

// implementation
Counter::Counter()
{
    std::cout << "\n[INFO] Calling default constructor from " << this;
}

Counter::Counter(int count) : counter{count}
{
    std::cout << "\n[INFO] Calling param. constructor from " << this;
}

Counter::~Counter()
{
    std::cout << "\n[INFO] Calling destructor from " << this;
}

int Counter::getCount()
{
    return counter;
}

void Counter::increment()
{
    this->counter += 1;
}

void operator<<(std::ostream &o, Counter &c)
{
    std::cout << "\nCout = " << c.getCount();
}
