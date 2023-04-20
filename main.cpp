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
/* QUESTIONS */

class Question
{
protected:
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
class MultipleChoice : public Question
{
private:
    int num_variants;
    vector<string> variants;

public:
    MultipleChoice() : num_variants(3){};
    MultipleChoice(int num_variants)
    {
        this->num_variants = num_variants;
    }

    void setVariants()
    {
        char variant = 'A'; // for A
        string variant_option;
        cout << "Enter the variants" << endl;
        for (int i = 0; i < num_variants; i++)
        {

            cout << "Variant " << variant << ": ";
            cin >> variant_option;
            variants.push_back(variant_option);
            variant++;
        }
    }
    void showVariants()
    {

        char variant = 'A'; // for A
        for (int i = 0; i < num_variants; i++)
        {
            cout << variant << ") " << variants[i] << endl;
            variant++;
        }
    }

    void display()
    {
        cout << this->question << endl;
        showVariants();
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

void testAddQuestion()
{

    MultipleChoice question(3);
    question.setQuestion("integral from x^2");
    question.setAnswer("x^3/3");
    question.setVariants();
    question.display();
    cout << "Answer is " << question.getAnswer() << endl;
}

void quit()
{
    cout << "Good bye!" << endl;
    exit(0);
}

int main()
{
    int user_option;
    bool isRunning = true;

    while (isRunning)
    {
        cout << endl
             << " [ MENU ] " << endl;
        cout << " 1. Add question" << endl;
        cout << " 0. Quit" << endl;

        cout << "Your Option> ";
        cin >> user_option;

        // clear();
        switch (user_option)
        {
        case 1:
            testAddQuestion();
            break;
        case 0:
            quit();
            isRunning = false;
            break;
        default:
            isRunning = false;
            cout << "No Matching Option!" << endl;
            break;
        }
    }

    return 0;
};