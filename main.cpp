// imports
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <sstream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <cctype>
#define DEFAULT_VARIANTS_NUMBER 3
#define USER_DATA_PATH "./data/users.txt"
#define EXAMS_DATA_PATH "./data/exams/"

using namespace std;

// the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
const string RESET = "\033[0m";
const string BLACK = "\033[30m";              /* Black */
const string RED = "\033[31m";                /* Red */
const string GREEN = "\033[32m";              /* Green */
const string YELLOW = "\033[33m";             /* Yellow */
const string BLUE = "\033[34m";               /* Blue */
const string MAGENTA = "\033[35m";            /* Magenta */
const string CYAN = "\033[36m";               /* Cyan */
const string WHITE = "\033[37m";              /* White */
const string BOLDBLACK = "\033[1m\033[30m";   /* Bold Black */
const string BOLDRED = "\033[1m\033[31m";     /* Bold Red */
const string BOLDGREEN = "\033[1m\033[32m";   /* Bold Green */
const string BOLDYELLOW = "\033[1m\033[33m";  /* Bold Yellow */
const string BOLDBLUE = "\033[1m\033[34m";    /* Bold Blue */
const string BOLDMAGENTA = "\033[1m\033[35m"; /* Bold Magenta */
const string BOLDCYAN = "\033[1m\033[36m";    /* Bold Cyan */
const string BOLDWHITE = "\033[1m\033[37m";   /* Bold White */

const char *banner = R"""(
 _____                          _____           _                
|  ___|                        /  ___|         | |  
| |____  ____ _ _ __ ___  ___  \ `--. _   _ ___| |_ ___ _ __ ___ 
|  __\ \/ / _` | '_ ` _ \/ __|  `--. \ | | / __| __/ _ \ '_ ` _ \
| |___>  < (_| | | | | | \__ \ /\__/ / |_| \__ \ ||  __/ | | | | |
|____/_/\_\__,_|_| |_| |_|___/ \____/ \__, |___/\__\___|_| |_| |_|
                                       __/ |                      
                                      |___/
)""";

/* Utility functions */
void toLowerCase(string &data)
{
    // Convert string lowercase
    // e.g SANJAR -> sanjar
    // stolen from stackoverlow
    transform(data.begin(), data.end(), data.begin(),
              [](unsigned char c)
              { return std::tolower(c); });
}

void integerInput(string text, int &value)
{
    // For handling integer input
    // In asks from user to input until cin result is fully integer
    while (true)
    {
        cout << text << ": ";
        cin >> value;

        if (cin.fail()) // if it is not kind of integer
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "You have entered wrong input!"
                 << "\n";
        }
        else
        {
            // everything is ok
            break;
        }
    }
}

vector<string> split(const string &str, const string &delim)
{
    // stolen from stackoverflow
    // split word -> vector of tokens by some delimeter
    // e.g "Abduaziz,Ziyodov"" -> {"Abduaziz", "Ziyodov"}
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos)
            pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty())
            tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

template <typename T>
string objectMemoryAddrAsString(T *obj)
{
    // Memory addr. for every object -> unique
    // This function converts memory addr. to string that will be used unique IDs
    const void *address = static_cast<const void *>(obj);
    stringstream current_object_address;
    current_object_address << address;
    return current_object_address.str();
}

void clear()
// for clearing terminal screen
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

// user roles as variable
string TEACHER = "teacher", STUDENT = "student", ADMIN = "admin";

class User
{
private:
    string name, role, ID, password;

public:
    User(){};
    void setData(string name, string role, string password, string ID = "")
    {
        this->name = name;
        this->role = role;
        this->password = password;
        // If user provides id, then init with this id
        // If not, create it
        this->ID = ID.empty() ? User::createUserID(this) : ID;
    }
    // Getters
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
    string Password()
    {
        return password;
    }

    // Display basic info
    void display()
    {
        cout << "ID: " << ID << "\n";
        cout << "Name: " << name << "\n";
        cout << "Role: " << role << "\n";
    }
    static string createUserID(User *user)
    {
        // From mem. addr it creates unique user ID
        string ID = objectMemoryAddrAsString<User>(user);
        // (char)toupper(object->role[0]) - is first letter of role
        // if role is student, we have "S"
        return (char)toupper(user->role[0]) + ID.substr(5, ID.size());
    }

    static void addUser(User user)
    {
        // Write user object to data
        fstream file;
        file.open(USER_DATA_PATH, ios::app);

        file << user.name << "|" << user.role << "|" << user.password << "|" << user.ID << "\n";

        file.close();
    }
    static vector<User> loadUsers()
    {
        string line;
        vector<User> users; // saving output result

        User data; // temp. user object
        fstream file;
        file.open(USER_DATA_PATH, ios::in);

        while (getline(file, line)) // one user data IN one single line
        {
            vector<string> user_data = split(line, "|"); // split line by | and get vector of strings

            data.setData(
                user_data[0], // name
                user_data[1], // role
                user_data[2], // password
                user_data[3]  // ID
            );

            users.push_back(data); // add to users
        }
        file.close();
        return users;
    }
    friend class Exam;                        // Exam class needs to access private info of User
    friend string createUserID(User *object); // for access role, private attr
    friend User performAuth();                // same with performAuth function
};

/* QUESTIONS */

class Question
{
    // Basic class for representing question data
protected:
    string question;

public:
    Question(){};
    void setQuestion(string question)
    {
        this->question = question;
    }
    // not implemented yet, after inheriting they will be
    void start();
    void display();
    bool checkAnswer();
};

class ShortAnswerQuestion : public Question
{
    // Every object stores correct answer and the answer that user entered
    // It helps for evaluation of total score
private:
    string correct_answer;
    string user_answer;

public:
    string type = "written";

    void input()
    {
        // called in every question creation proccess
        string question, answer;

        cout << "Enter the question: ";
        cin.ignore();

        getline(cin, question);

        cout << "Enter the short answer: ";
        getline(cin, answer);

        this->setQuestion(question);
        this->setAnswer(answer);
    }
    void setAnswer(string answer)
    {
        // We need to use toLowerCase
        // The answer should not be case sensitive
        // If answer is Mark and user entered mark MaRk -> it must be true answer
        toLowerCase(answer);
        this->correct_answer = answer;
    }
    void setUserAnswer(string answer)
    {
        toLowerCase(answer);
        this->user_answer = answer;
    }
    void display()
    {
        cout << "Short Answer question:" << this->question << "\n";
    }

    void start()
    {
        // Called on examination process
        string user_answer;

        this->display();

        cout << "Your answer: ";
        cin.ignore();
        getline(cin, user_answer);

        this->setUserAnswer(user_answer);
        cout << "The answer is -> " << boolalpha << this->checkAnswer() << ", " << correct_answer << " was correct!"
             << "\n";
    }
    bool checkAnswer()
    {
        return this->correct_answer == this->user_answer;
    }
};

class MultipleChoice : public Question
{
private:
    // Variants (A,B,C)
    vector<string> variants;
    int variants_count, correct_option, user_option{0};

public:
    string type = "multi";
    MultipleChoice() : variants_count(DEFAULT_VARIANTS_NUMBER){};
    MultipleChoice(int _variants_count) : variants_count(_variants_count){};

    void input()
    {
        string question_text;

        cout << "Enter the question text: ";
        cin.ignore();
        getline(cin, question_text);
        this->setQuestion(question_text);
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
        cout << "Enter the variants: "
             << "\n";
        for (int i = 0; i < variants_count; i++)
        {

            cout << "Variant " << option << ": ";
            getline(cin, variant_option);
            variants.push_back(variant_option);
            option++; // By incrementing value of char, we get next letter. e.g option = A, After ++option we get
        }
    }
    void showVariants()
    {

        char option = 'A';
        for (string variant : variants)
        {
            cout << option << ")" << variant << "\n";
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
        cout << this->question << "\n";
        showVariants();
    }

    void start()
    {
        this->display();
        char user_answer;
        cout << "Your answer is: ";
        cin >> user_answer;
        this->setUserAnswer(user_answer);
        cout << "The answer is -> " << boolalpha << this->checkAnswer() << "\n";
    }

    bool checkAnswer()
    {
        return (correct_option == user_option);
    }
};

class Exam
{
private:
    string ID;
    User author;
    string title;
    vector<User> participants;
    vector<MultipleChoice> multi_questions;
    vector<ShortAnswerQuestion> short_answer_questions;

public:
    Exam(User _author, string ID = "") : author(_author)
    {
        // If user provides id, then init with this id
        // If not, create it
        this->ID = ID.empty() ? Exam::createExamID(this) : ID;
    };

    static string createExamID(Exam *exam)
    {
        // From mem. addr it creates unique user ID
        string ID = objectMemoryAddrAsString<Exam>(exam);
        // It will return E3921893 like unique ID.
        return "E" + ID.substr(5, ID.size());
    }

    // Functions for adding questions directly
    void includeMultipleChoiceQuestions(vector<MultipleChoice> questions)
    {
        for (MultipleChoice question : questions)
        {

            this->multi_questions.push_back(question);
        }
    }
    void includeShortAnswerQuestions(vector<ShortAnswerQuestion> questions)
    {
        for (ShortAnswerQuestion question : questions)
        {

            this->short_answer_questions.push_back(question);
        }
    }
    void includeMultipleChoiceQuestion(MultipleChoice question)
    {
        this->multi_questions.push_back(question);
    }
    void includeShortAnswerQuestion(ShortAnswerQuestion question)
    {
        this->short_answer_questions.push_back(question);
    }

    int getTotalNumberOfQuestions()
    {
        return this->multi_questions.size() + this->short_answer_questions.size();
    }

    void info()
    {
        cout << "\n"
             << "\n["
             << this->title << " exam ]"
             << "\n";
        cout << "Author: " << this->author.Name() << "\n";
        cout << "Total number of questions: " << this->getTotalNumberOfQuestions() << "\n";
        cout << "Total number of participants: " << this->participants.size() << "\n";
        cout << "Number of written questions: " << this->short_answer_questions.size() << "\n";
        cout << "Number of multi-choice questions: " << this->multi_questions.size() << "\n"
                                                                                        "\n";
    }

    template <typename T> // T canbe MultipleChoice or ShortAnswer based object
    void shuffleQuestions(vector<T> &questions)
    {
        // code below stolen from stackoverlow
        auto random_engine = default_random_engine{};
        shuffle(begin(questions), end(questions), random_engine);
    }
    void start(User user)
    {
        cout << "Exam is started by "
             << "\n";
        user.display();

        this->participants.push_back(user);

        this->shuffleQuestions(this->multi_questions);
        this->shuffleQuestions(this->short_answer_questions);

        for (MultipleChoice question : multi_questions)
        {
            question.start();
        }
        for (ShortAnswerQuestion question : short_answer_questions)
        {
            question.start();
        }
    }
    void insertQuestions()
    {
        int type, number_of_questions;

        cout << "Title of exam: ";
        cin.ignore();
        getline(cin, this->title);

        integerInput("Enter number of questions that you want", number_of_questions);

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
                ShortAnswerQuestion question;
                question.input();
                this->includeShortAnswerQuestion(question);
            }
            else
            {
                cout << "Wrong input, try again!"
                     << "\n";
            }
        }
    }

    static string parseExamIDfromFileName(string path)
    {
        // implement this bitch
        return path;
    }
    // static vector<Exam> loadExams()
    // {
    //     string line;
    //     fstream file;
    //     vector<Exam> exams;

    //     for (const auto &entry : filesystem::directory_iterator(EXAMS_DATA_PATH))
    //     {
    //         string file_path = entry.path();
    //         file.open(file_path);
    //         cout << "For file " << file_path << "\n";
    //         while (getline(file, line))
    //         {
    //             cout << line << "\n";
    //         }
    //         file.close();
    //     }

    //     return exams;
    // }
};

/* Menu */

User performAuth()
{
    User user;
    string answer;
    bool isCompleted = false;

    cout << "Do you have an account? (y/n)";
    getline(cin, answer);

    toLowerCase(answer);
    bool hasAccount = (answer == "y" || answer == "yes") ? true : false;

    vector<User> users_data = {};

    if (hasAccount)
    {
        // there should be logic to checking user input
        // function should search from file, filter user's name and password
        cout << "You are great!"
             << "\n";
    }
    else
    {
        int role_option;
        string name, role, password;

        while (true)
        {

            cout << "What is your name? ";
            getline(cin, name);

            cout << "Your password: ";
            getline(cin, password);

            cout << "Select your role: "
                 << "\n";
            cout << "1. Teacher"
                 << "\n";
            cout << "2. Student"
                 << "\n";
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
                cout << "Input again!"
                     << "\n";
            }
        }

        user.setData(name, role, password);
        cout << "[ Save that information ]"
             << "\n";
        cout << "Your ID: " << user.getID() << "\n";
        cout << "Your Password: " << user.password << "\n";
        User::addUser(user);
    }

    return user;
}

void teacherMenu(User user)
{
    int option;
    bool isRunning = true;

    while (isRunning)
    {
        cout << "\n[ MENU ]"
             << "\n";
        cout << "1. Create exam"
             << "\n";
        cout << "2. List of exams"
             << "\n";
        cout << "3. User info"
             << "\n";
        cout << "(other) Quit"
             << "\n";
        integerInput("Your option", option);
        clear();

        if (option == 1)
        {
            Exam exam(user);
            exam.insertQuestions();
            cout << "\n";
            exam.start(user);
            exam.info();
        }
        else if (option == 2)
        {
            cout << "List of exams: "
                 << "\n";
        }
        else if (option == 3)
        {
            user.display();
        }
        else
        {
            cout << "Good bye!"
                 << "\n";
            exit(0);
        }
    }
}

void studentMenu(User user)
{
    cout << "Student menu!"
         << "\n";
    user.display();
}

int main()
{
    cout << BOLDGREEN << banner << RESET;
    // vector<Exam> exams = Exam::loadExams();
    User user = performAuth();
    while (true)
    {
        cout << "User role is: " << user.Role() << "\n";

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
            cout << "Good bye!"
                 << "\n";
            exit(0);
        }
    }

    return 0;
};