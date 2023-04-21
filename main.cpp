// imports
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cctype>
#define DEFAULT_VARIANTS_NUMBER 3

using namespace std;

/* Utility functions */
vector<string> splitWord(string answer)
{
    string word;
    vector<string> new_answer;
    char space = ' ';
    for (char chunk : answer)
    {
        if (chunk == space)
        {
            new_answer.push_back(word);
            word.clear();
        }
        else
        {
            word += chunk;
        }
        new_answer.push_back(word);
    }
    return new_answer;
}
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

// for creating ID

/* QUESTIONS */

class Question
{
    // Base class for all questions
protected:
    string question;

public:
    Question(){};
    void setQuestion(string question_text)
    {
        this->question = question_text;
    }
};

class WrittenQuestion : public Question
{
private:
    string answer;
    vector<string> real_answer;

public:
    void getRealAnswer()
    {
        int number_of_possible_answers;
        string possible_answer;
        cout << "Enter how many possible answers do you want to include: ";
        cin >> number_of_possible_answers;
        for (int i = 0; i < number_of_possible_answers; i++)
        {
            cout << "Enter possible answer number " << i;
            cin >> possible_answer;
            real_answer.push_back(possible_answer);
            possible_answer.clear();
        }
    }
    void compareAnswer(string answer)
    {
        // spliting all possible answers into words and then putting it into vector of vectors
        vector<vector<string>> new_real_answer, matching_words;
        for (string possible_answer : real_answer)
        {
            new_real_answer.push_back(splitWord(possible_answer));
        }
        // finding matching words with possible answers
        vector<string> new_answer = splitWord(answer), matching_words_1;
        for (vector<string> possible_answer : new_real_answer)
        {
            for (string word1 : possible_answer)
            {
                for (string word2 : new_answer)
                {
                    if (word2 == word1)
                    {
                        matching_words_1.push_back(word2);
                        matching_words.push_back(matching_words_1);
                        break;
                    }
                }
                matching_words_1.clear();
            }
        }
        // printing matching words with all possible answers
        for (int i = 0; i < new_real_answer.size(); i++)
        {
            cout << "\nMatching words with possible answer number " << i << " : ";
            for (string word : matching_words[i])
            {
                cout << word << " ";
            }
        }
    }
    bool checkAnswer(string answer)
    {
        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
        if (this->answer == answer)
        {
            return true;
        }
        return false;
    }
};

class MultipleChoice : public Question
{
private:
    vector<string> variants;
    int variants_count, correct_option, user_option{0};

public:
    MultipleChoice() : variants_count(DEFAULT_VARIANTS_NUMBER){};
    MultipleChoice(int _variants_count) : variants_count(_variants_count){};

    void setVariants()
    {
        char option = 'A';
        string variant_option;
        cout << "Enter the variants" << endl;
        for (int i = 0; i < variants_count; i++)
        {

            cout << "Variant " << option << ": ";
            cin >> variant_option;
            variants.push_back(variant_option);
            option++;
        }
    }
    void showVariants()
    {

        char option = 'A';
        for (string variant : variants)
        {
            cout << option << ") " << variant << endl;
            option++;
        }
    }
    void setUserAnswer(char user_answer)
    {

        this->user_option = toupper(user_answer) % 65;
    }
    void setAnswer(char correct_answer)
    {
        this->correct_option = toupper(correct_answer) % 65;
    }

    void display()
    {
        cout << this->question << endl;
        showVariants();
        cout << "c option " << correct_option << endl;
        cout << "u option " << user_option << endl;
    }

    bool checkAnswer()
    {
        return (correct_option == user_option) ? true : false;
    }
};

class Exam
{
    /*
    TODO
    exam is main object that contains:
    - list of all questions
    - list of users that attend. to this exam.
    Method run() that starts exam, prints question one by one, takes answer from user
    then shows results

    If you have any other idea you can implement it
    Then just write test function for menu, and add your block to switch case statement and run.
    */
};

/* USERS */

string TEACHER = "teacher", STUDENT = "student", ADMIN = "admin";

class User
{
private:
    string name, role, ID, password;
    int age;

public:
    User(){};
    void setData(string name, int age, string role, string password)
    {
        this->age = age;
        this->name = name;
        this->role = role;
        this->password = password;
        this->ID = User::createID(this);
    }
    void display()
    {
        cout << "ID: " << ID << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Role: " << role << endl;
    }
    static string createID(User *object)
    {
        // Takes object as an argument,
        // function will get its memory address.
        // converts it to string
        // returns all contents from the 5th char (we dont need first chars like 0x)
        // user id should be unique, and memory addr. also unique.
        const void *address = static_cast<const void *>(object);
        stringstream current_object_address;
        current_object_address << address;
        string ID = current_object_address.str();
        // (char)toupper(object->role[0]) - is first letter of role
        // if role is student, we have "S"
        return (char)toupper(object->role[0]) + ID.substr(5, ID.size());
    }
};
/*
TODO

we have user object.
write function or member function to user class
that stores all user data to one file.

write function that searches user by its username and password.
if user is not found, then inform about it.
That's main ideas, i think you understood problem.
*/

/* Some kind of testing functions for menu*/
void testRegisterUser()
{
    User user1, user2;
    string name, role;
    int age;
    cout << "Enter name, age and role: ";
    cin >> name >> age >> role;
    user1.setData(name, age, role, "some_password");
    user2.setData(name, age, role, "some_password");
    user1.display();
    user2.display();
}

void testMultipleChoice()
{
    MultipleChoice question(3);
    question.setQuestion("integral from x^2");
    question.setVariants();
    char correct_answer;
    cout << "Enter correct answer: ";
    cin >> correct_answer;
    question.setAnswer(correct_answer);
    question.display();

    char answer;
    cout << "Enter your answer: ";
    cin >> answer;

    question.setUserAnswer(answer);
    cout << "Answer is " << boolalpha << question.checkAnswer() << endl;
}

void testWritten()
{
    WrittenQuestion question;
    cout << "Enter question: ";
    string question_text;
    cin >> question_text;

    cout << "Enter answer: ";
    string answer_text;
    cin >> answer_text;

    question.setQuestion(question_text);
    question.getRealAnswer();
    question.compareAnswer(answer_text);
}

int main()
{
    int user_option;
    bool isRunning = true;

    while (isRunning)
    {
        cout << "\n[ MENU ] " << endl;
        cout << " 1. Test user register" << endl;
        cout << " 2. Test multi choice question" << endl;
        cout << " 3. Test written question" << endl;
        cout << " 0. Quit" << endl;

        cout << "Your Option> ";
        cin >> user_option;

        clear();
        switch (user_option)
        {
        case 1:
            testRegisterUser();
            break;
        case 2:
            testMultipleChoice();
            break;
        case 3:
            testWritten();
            break;
        case 0:
            isRunning = false;
            cout << "Good bye!" << endl;
            exit(0);
            break;
        default:
            isRunning = false;
            cout << "No Matching Option!" << endl;
            break;
        }
    }

    return 0;
};