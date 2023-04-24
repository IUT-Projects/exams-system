// imports
#include <iostream>
#include <limits>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#define DEFAULT_VARIANTS_NUMBER 3

using namespace std;

const char *banner = R""""(
 _____                          _____           _                
|  ___|                        /  ___|         | |  
| |____  ____ _ _ __ ___  ___  \ `--. _   _ ___| |_ ___ _ __ ___ 
|  __\ \/ / _` | '_ ` _ \/ __|  `--. \ | | / __| __/ _ \ '_ ` _ \
| |___>  < (_| | | | | | \__ \ /\__/ / |_| \__ \ ||  __/ | | | | |
|____/_/\_\__,_|_| |_| |_|___/ \____/ \__, |___/\__\___|_| |_| |_|
                                       __/ |                      
                                      |___/
)"""";

/* Utility functions */

// Usage:

// data_type value_name;
// value_name  = data_typeInput("Enter value: ")
void stringInput(string text, string &value)
{
    cout << text << ":";
    cin.ignore();
    getline(cin, value);
}

void integerInput(string text, int &value)
{
    while (true)
    {
        cout << text << ":";
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "You have entered wrong input!" << endl;
        }
        else
        {
            cout << "So value is: " << value << endl;
            break;
        }
    }
}

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

/* Users & Register */
string TEACHER = "teacher", STUDENT = "student", ADMIN = "admin";

class User
{
private:
    int age;
    string name, role, ID, password;

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
    string Name()
    {
        return name;
    }
    string Role()
    {
        return role;
    }
    string getID()
    {
        return ID;
    }
    int Age()
    {
        return age;
    }
    string Password()
    {
        return password;
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
    friend User performAuth();
    friend class Exam;
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

/* QUESTIONS */

class Question
{
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
    string type = "written";

    void input()
    {
        cout << "Calling input function> " << endl;
    }
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
        return this->answer == answer;
    }
};

class MultipleChoice : public Question
{
private:
    vector<string> variants;
    int variants_count, correct_option, user_option{0};

public:
    string type = "multi";
    MultipleChoice() : variants_count(DEFAULT_VARIANTS_NUMBER){};
    MultipleChoice(int _variants_count) : variants_count(_variants_count){};
    void input()
    {
        stringInput("Enter the question text", this->question);
        this->setVariants();
        char correct_answer;
        cout << "Enter correct answer: ";
        cin >> correct_answer;
        this->setAnswer(correct_answer);
    }

    void setVariants()
    {
        char option = 'A';
        string variant_option;
        cout << "Enter the variants" << endl;
        for (int i = 0; i < variants_count; i++)
        {

            cout << "Variant " << option << ": ";
            stringInput("", variant_option);
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
    }

    void start()
    {
        this->display();
        char user_answer;
        cout << "Your answer is: ";
        cin >> user_answer;
        this->setUserAnswer(user_answer);
        cout << "The answer is -> " << boolalpha << this->checkAnswer() << endl;
    }

    bool checkAnswer()
    {
        return (correct_option == user_option) ? true : false;
    }
};

class Exam
{
private:
    string title;
    User author;
    vector<User> participants;
    vector<MultipleChoice> multi_questions;
    vector<WrittenQuestion> written_questions;

public:
    Exam(User _author) : author(_author){};
    Exam(User _author, string _title) : author(_author), title(_title){};
    // Functions for adding questions to exam
    void includeMultipleChoiceQuestions(vector<MultipleChoice> questions)
    {
        for (MultipleChoice question : questions)
        {

            this->multi_questions.push_back(question);
        }
    }
    void includeWrittenQuestions(vector<WrittenQuestion> questions)
    {
        for (WrittenQuestion question : questions)
        {

            this->written_questions.push_back(question);
        }
    }
    void includeMultipleChoiceQuestion(MultipleChoice question)
    {
        this->multi_questions.push_back(question);
    }
    void includeWrittenQuestion(WrittenQuestion question)
    {
        this->written_questions.push_back(question);
    }

    int getTotalNumberOfQuestions()
    {
        return this->multi_questions.size() + this->written_questions.size();
    }

    void info()
    {
        cout << this->title << " exam" << endl;
        cout << "Author: " << this->author.Name() << endl;
        cout << "Total number of questions: " << this->getTotalNumberOfQuestions() << endl;
        cout << "Total number of participants: " << this->participants.size() << endl;
        cout << "Number of written questions: " << this->written_questions.size() << endl;
        cout << "Number of multi-choice questions: " << this->multi_questions.size() << endl;
    }
    void start(User user)
    {
        cout << "Exam is started by " << endl;
        user.display();
        this->participants.push_back(user);

        for (MultipleChoice question : multi_questions)
        {
            question.start();
        }
    }
    void insertQuestions()
    {
        int type, number_of_questions;
        integerInput("Enter number of questions that you want: ", number_of_questions);

        for (int counter = 0; counter < number_of_questions; counter++)
        {
            integerInput("Type of question -> Multiple Choice(1) or Written(2)", type);

            if (type == 1)
            {
                MultipleChoice question;
                question.input();
                this->includeMultipleChoiceQuestion(question);
            }
            else if (type == 2)
            {
                WrittenQuestion question;
                question.input();
                this->includeWrittenQuestion(question);
            }
            else
            {
                cout << "Wrong input, try again" << endl;
            }
        }
    }
};

/* Menu */

User performAuth()
{
    User user;
    string answer;
    bool isCompleted = false;

    cout << "Do you have an account? (y/n)";
    cin >> answer;

    bool hasAccount = (answer == "Y" || answer == "y") ? true : false;

    vector<User> users_data = {};

    if (hasAccount)
    {
        // there should be logic to checking user input
        // function should search from file, filter user's name and password
        cout << "You are great!" << endl;
    }
    else
    {
        int age, role_option;
        string name, role, password;

        while (true)
        {

            stringInput("What is your name?", name);

            integerInput("How old are you?", age);

            cout << "Select your role:" << endl;
            cout << "1. Teacher" << endl;
            cout << "2. Student" << endl;
            integerInput("Your role", role_option);

            if (role_option == 1)
            {
                role = TEACHER;
                break;
            }
            else if (role_option == 2)
            {
                role = STUDENT;
                break;
            }
            else
            {
                cout << "Input again!" << endl;
            }

            stringInput("Your password?", password);
        }

        user.setData(name, age, role, password);
        cout << "[ Save that information ]" << endl;
        cout << "Your ID: " << user.getID() << endl;
        cout << "Your Password: " << user.password << endl;
    }
    return user;
}

void teacherMenu(User user)
{
    int option;
    bool isRunning = true;

    while (isRunning)
    {
        cout << "\n[ MENU ]" << endl;
        cout << "1. Create exam" << endl;
        cout << "2. List of exams" << endl;
        cout << "3. User info" << endl;
        cout << "(other) Quit" << endl;
        integerInput("Your option", option);

        clear();

        if (option == 1)
        {
            Exam exam(user, "Calculus 2");
            exam.insertQuestions();
            cout << endl;
            exam.start(user);
            exam.info();
        }
        else if (option == 2)
        {
            cout << "List of exams: " << endl;
        }
        else if (option == 3)
        {
            user.display();
        }
        else
        {
            cout << "Good bye!" << endl;
            exit(0);
        }
    }
}

void studentMenu(User user)
{
    cout << "Student menu!" << endl;
    user.display();
}

int main()
{
    cout << banner;

    User user = performAuth();

    while (true)
    {
        cout << "User role is: " << user.Role() << endl;

        if (user.Role() == TEACHER)
        {
            teacherMenu(user);
            break;
        }
        else if (user.Role() == STUDENT)
        {
            studentMenu(user);
            break;
        }
        else
        {
            cout << "Good bye!" << endl;
            exit(0);
        }
    }

    return 0;
};