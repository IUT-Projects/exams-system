// imports
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

int global_counter = 0;
typedef void (*function_type)();

// for clearing terminal screen
void clear()
{

#if defined _WIN32
    system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined(__APPLE__)
    system("clear");
#endif
}

class Option
{
private:
    string name;
    int counter;
    function_type fn;

public:
    Option(string _name, function_type _fn) : name(_name), fn(_fn), counter(global_counter)
    {
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
    cout << " " << _opt.counter << " " << _opt.name << endl;
}

bool optionExists(int user_option, vector<Option> main_options)
{
    for (Option option : main_options)
    {
        if (option.getCounter() == user_option)
        {
            return true;
        }
    }
    return false;
}

// list of menu options
vector<Option> main_options = {
    Option(
        "Quit", []
        { cout << "Good bye!" << endl; exit(0); }),
    Option(
        "Say hello", []
        { cout << "Hello world!"; })};

/* QUESTIONS */

class Question
{
private:
    string question, answer, user_answer;

public:
    Question(){};
    Question(string question, string answer)
    {
        this->question = question;
        this->answer = answer;
    }
    string getQuestion() { return question; }
    string getAnswer() { return answer; }
    string getUser_Answer() { return user_answer; }
    void setQuestion(string question) { this->question = question; }
    void setAnswer(string answer) { this->answer = answer; }
    void setUser_Answer(string user_answer) { this->user_answer = user_answer; }
};
class MultipleChoice : private Question
{
private:
    vector<string> variants;
    int num_variants;

public:
    MultipleChoice(int num_variants)
    {
        this->num_variants = num_variants;
    }

    void setVariants()
    {
        cout << "Enter the variants" << endl;
        for (int i = 0; i > num_variants; i++)
        {
            char variant = 'A';
            cout << "Variant " << variant << ":";
            cin >> variants[i];
            variant++;
        }
    }
    void getVariants()
    {
        for (int i = 0; i > num_variants; i++)
        {
            char variant = 'A';
            cout << "Variant " << variant << ": " << variants[i] << endl;
            variant++;
        }
    }
    void userAnswer(char answer)
    {
        int user_answer = answer % 65;
        setUser_Answer(variants[user_answer]);
    }
    int checkAnswer(char answer)
    {
        int user_answer = answer % 65;
        if (variants[user_answer] == getUser_Answer())
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};

class QuestionSet
{
};

int main()
{
    int user_option;
    bool isRunning = true;

    while (isRunning)
    {
        cout << endl
             << " [ MENU ] " << endl;
        for (Option option : main_options)
        {
            cout << option;
        }

        cout << "Your Option> ";
        cin >> user_option;

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
            cout << "No matching option!" << endl;
            isRunning = false;
        }
    }

    return 0;
};