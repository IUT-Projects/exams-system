#include <iostream>
#include <string>

using namespace std;

float FLOAT;
int INTEGER;
double DOUBLE;
string STRING;

class Validators
{
    /*
    - Purpose
        Check data type of user input.
    - Usage
        All methods of class are static.
        For instance, if you wanna validate string, you have to:

        string user_input;
        std::cin >> user_input;

        if ( Validators::validate_string(user_input) ) { data is valid}
        else { invalid data }
    */
public:
    Validators(){};
    static bool validate_integer(auto value);
    static bool validate_double(auto value);
    static bool validate_float(auto value);
    static bool validate_string(auto value);
};

// integer type validator
bool Validators::validate_integer(auto value)
{
    return is_same_v<decltype(value), decltype(INTEGER)>;
};

// string type validator
bool Validators::validate_string(auto value)
{
    return is_same_v<decltype(value), decltype(STRING)>;
};

// double type validator
bool Validators::validate_double(auto value)
{
    return is_same_v<decltype(value), decltype(DOUBLE)>;
};

// float type validator
bool Validators::validate_float(auto value)
{
    return is_same_v<decltype(value), decltype(FLOAT)>;
};