#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "colors.cpp"

using namespace std;

// lets define some colors for our cout based MENU XD

Color::Modifier green(Color::FG_GREEN);
Color::Modifier greenBg(Color::BG_GREEN);

Color::Modifier red(Color::FG_RED);
Color::Modifier redBg(Color::BG_RED);

Color::Modifier blue(Color::FG_BLUE);
Color::Modifier blueBg(Color::BG_BLUE);

Color::Modifier defaultFG(Color::FG_DEFAULT);
Color::Modifier defaultBG(Color::BG_DEFAULT);

void clear()
{
    /*
     Simple function for clearing terminal screen.
     In windows CMD command is cls, and linux based OS its clear.
     */
#if defined _WIN32
    system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined(__APPLE__)
    system("clear");
#endif
}

// creating kind of new type for functions (that returns void)
typedef void (*function)();

// global counter for options
int global_counter = 0;

class Option
{
private:
    string name;
    int counter;
    function fn;

public:
    Option(string _name, function _fn) : name(_name), fn(_fn), counter(global_counter)
    {
        /*
        we have menu like this:
        1. Option
        2. Option

        1 and 2 - counters
        If we add new option, it should be 3.
        1. Option
        2. Option
        3. Option
        Thats why we need global varibale.
        If we use static variable, then:

        3. Option
        3. Option
        3. Option

        Because data will be share around all of these objects
        */
        global_counter++;
    }
    void run()
    {
        clear();
        cout << endl;
        fn();
        cout << endl;
    }
    int getCounter()
    {
        return this->counter;
    }
    friend void operator<<(ostream &os, Option _opt);
};
void operator<<(ostream &os, Option _opt)
{
    cout << greenBg << " " << _opt.counter << " " << defaultBG << " " << _opt.name << endl;
}

bool optionExists(int user_option, vector<Option> main_options)
{
    // Function for checking existence of option
    // e.g user entered 1, it tries to find option which has 1

    for (Option option : main_options)
    {
        if (option.getCounter() == user_option)
        {
            return true;
        }
    }
    return false;
}

void handleUserInput(int user_option, vector<Option> main_options, bool &isRunning);

void runMainMenu()
{
    int user_option;
    bool isRunning = true;

    // we put all menu options to there:
    // Option(name, function_to_execute)
    // [] { ... } - this kind of syntax creates function :D (lambda)
    vector<Option> main_options = {
        Option(
            "Quit", []
            { cout << blue << "Good bye!" << defaultFG << endl; exit(0); }),
        Option(
            "Say hello", []
            { cout << "Hello world!"; })};

    while (isRunning)
    {
        // display all available options
        cout << endl
             << blueBg
             << " [ MENU ] " << defaultBG << endl;
        for (Option option : main_options)
        {
            cout << option;
        }

        // take input from user
        cout << "Your Option> ";
        cin >> user_option;

        // iterate again, find option that matches with user_input
        // then execute spec. function for this option

        handleUserInput(user_option, main_options, isRunning);
    }
};

void handleUserInput(int user_option, vector<Option> main_options, bool &isRunning)
{
    // if options exists, then run it!
    if (optionExists(user_option, main_options))
    {
        for (Option option : main_options)
        {
            if (option.getCounter() == user_option)
            {
                option.run();
            }
        }
    }
    else
    {
        // for colorful cout :D
        Color::Modifier red(Color::BG_RED);
        cout << red << "No matching option!" << endl;
        isRunning = false;
    }
}