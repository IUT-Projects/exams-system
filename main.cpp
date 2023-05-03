// imports
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <sstream>
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

const char *loginBanner = R"""(
 _    ___   ___ ___ _  _ 
| |  / _ \ / __|_ _| \| |
| |_| (_) | (_ || || .` |
|____\___/ \___|___|_|\_|
)""";
const char *registerBanner = R"""(
 ___ ___ ___ ___ ___ _____ ___ ___ 
| _ \ __/ __|_ _/ __|_   _| __| _ \
|   / _| (_ || |\__ \ | | | _||   /
|_|_\___\___|___|___/ |_| |___|_|_\
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

        if (cin.fail() || value <= 0) // if it is not kind of integer
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
    friend class Exam; // Exam class needs to access private info of User

    // some function for auth needs access to private members
    friend User performLogin();
    friend User performRegister();
};

/* QUESTIONS */

class Question
{
    // Basic class for representing question data
protected:
    string question;

public:
    Question(){};
    // not implemented yet, after inheriting they will be
    void start();
    void checkAnswer();
};

class ShortAnswerQuestion : public Question
{
    // Every object stores correct answer and the answer that user entered
    // It helps for evaluation of total score
private:
    bool correctAnswered = 0;
    string correct_answer, user_answer;

public:
    string type = "written";
    void debugDetails()
    {
        cout << endl
             << endl
             << "[DEBUG]" << endl;
        cout << "Question text " << question << endl;
        cout << "User answer " << user_answer << endl;
        cout << "Correct Answer " << correct_answer << endl;
        cout << "Correct Answered " << boolalpha << correctAnswered << endl;
    }

    void input()
    {
        // called in every question creation proccess

        cout << "Enter the question: ";
        cin.ignore();

        getline(cin, this->question);

        string answer;
        cout << "Enter the short answer: ";
        getline(cin, answer);

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
    void start()
    {
        char user_ans[200];

        cout << "Short Answer question: " << this->question << "\n";
        cout << "Your answer: ";
        cin.getline(user_ans,sizeof(user_ans));
        // getline(cin, _user_answer);

        this->setUserAnswer(user_ans);
        this->correctAnswered = this->checkAnswer();
        this->debugDetails();
    }
    bool checkAnswer()
    {
        return (this->correct_answer == this->user_answer);
    }
    friend class Exam;
};

class MultipleChoice : public Question
{
private:
    vector<string> variants;
    int variants_count, correct_option, user_option{0};
    bool correctAnswered = 0;

public:
    string type = "multi";
    MultipleChoice() : variants_count(DEFAULT_VARIANTS_NUMBER){};
    MultipleChoice(int _variants_count) : variants_count(_variants_count){};

    void debugDetails()
    {
        cout << endl
             << "[DEBUG]" << endl;
        cout << "Question text " << question << endl;
        cout << "Variant Count " << variants_count << endl;
        cout << "Correct Option " << correct_option << endl;
        cout << "User option " << user_option << endl;
        cout << "Correct Answered " << boolalpha << correctAnswered << endl;
    }

    void input()
    {
        cout << "Enter the question text: ";
        cin.ignore();
        getline(cin, this->question);
        this->setVariants();
        char correct_answer;
    enterCorrectVariant:
        cout << "Enter correct variant: ";
        cin >> correct_answer;
        if (!this->checkVariantExists(correct_answer))
        {
            goto enterCorrectVariant;
        }

        this->setAnswer(correct_answer);
    }

    bool checkVariantExists(char &variant)
    {
        cout << variant << endl;
        char option = 'A';

        cout << this->variants.size() << endl;
        for (int iter = 0; iter < this->variants.size(); iter++)
        {
            if (option == toupper(variant))
            {
                return true;
            }
            option++;
        }
        cout << "Variant does not exist!" << endl;
        return false;
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
    void start()
    {
        cout << "HELLO WORLD!"
             << "\n";
        cout << this->question << "\n";
        this->showVariants();

        char user_answer;
        cout << "Your answer is: ";
        cin >> user_answer;
        this->setUserAnswer(user_answer);
        this->correctAnswered = this->checkAnswer();

        this->debugDetails();
    }

    bool checkAnswer()
    {
        return (this->correct_option == this->user_option);
    }
    friend class Exam;
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
    Exam()
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
    void insertQuestions(User author)
    {
        int type, number_of_questions, number_of_variants;

        this->author = author;

        cout << "Title of exam: ";
        cin.ignore();
        getline(cin, this->title);

        integerInput("Enter number of questions that you want", number_of_questions);

        for (int counter = 0; counter < number_of_questions; counter++)
        {
            integerInput("Type of question -> Multiple Choice(1) or Written(2)", type);
            if (type == 1)
            {
                integerInput("Enter number of variants: ", number_of_variants);
                MultipleChoice question(number_of_variants);
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

    void displayResults()
    {
        cout << "Displaying Results" << endl;
        //     int total_score = 0;
        //     for (ShortAnswerQuestion question : short_answer_questions)
        //     {
        //         question.display();
        //         total_score += question.correctAnswered;
        //     }
        //     for (MultipleChoice question : multi_questions)
        //     {
        //         total_score += question.correctAnswered;
        //     }
        //     cout << "[ RESULTS ]" << endl;
        //     cout << "Total Score: " << total_score << "/" << getTotalNumberOfQuestions() << endl;
        //     if (!short_answer_questions.empty())
        //     {
        //         cout << "Answers for short questions:" << endl;
        //         for (int i = 0; i < short_answer_questions.size(); i++)
        //         {
        //             cout << i + 1 << ". " << short_answer_questions[i].correct_answer << endl;
        //         }
        //     }
        //     if (!multi_questions.empty())
        //     {
        //         cout << "Answers for multiple choice questions: " << endl;
        //         for (int i = 0; i < multi_questions.size(); i++)
        //         {
        //             cout << i + 1 << ". " << multi_questions[i].correct_option << endl;
        //         }
        //     }
    }
};

/* Menu */

User performRegister()
{
    clear();
    cout << BOLDBLUE << registerBanner << RESET << endl;

    User user;
    int role_option;
    string name, role, password;

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

selectRole:
    if (role_option == 1)
    {
        role = TEACHER;
    }
    else if (role_option == 2)
    {
        role = STUDENT;
    }
    else
    {
        cout << "Input again!"
             << "\n";
        goto selectRole;
    }

    user.setData(name, role, password);
    cout << "[ Save that information ]"
         << "\n";
    cout << "Your ID: " << user.getID() << "\n";
    cout << "Your Password: " << user.password << "\n";
    User::addUser(user);
    return user;
}

User performLogin()
{
    clear();
    cout << BOLDYELLOW << loginBanner << RESET << endl;
    User *user = NULL;

login:
    string ID, password;
    cout << "Your ID: ";
    cin >> ID;
    cout << "Your Password: ";
    cin >> password;
    vector<User> users = User::loadUsers();

    bool isExist = false;

    for (User user : users)
    {
        if (user.ID == ID && user.password == password)
        {
            isExist = true;
            return user;
        }
    }

    if (!isExist)
    {
        cout << "ID or password is incorrect. Please try again." << endl;
        goto login;
    }
    return *user; // will not be executed
}

User performAuth()
{
    string answer;
    bool isCompleted = false;

    cout << "Do you have an account? (y/n)";
    getline(cin, answer);

    toLowerCase(answer);
    bool hasAccount = (answer == "y" || answer == "yes");

    return (hasAccount) ? performLogin() : performRegister();
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
            Exam exam;
            exam.insertQuestions(user);
            cout << "\n";
            exam.start(user);
            exam.displayResults();
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
    int option;
    bool isRunning = true;

    while (isRunning)
    {
        cout << "\n[ MENU ]"
             << "\n";
        cout << "1. Start exam"
             << "\n";
        cout << "2. Results"
             << "\n";
        cout << "3. User info"
             << "\n";
        cout << "(other) Quit"
             << "\n";
        integerInput("Your option", option);
        clear();

        if (option == 1)
        {
            Exam exam;
            exam.start(user);
        }
        else if (option == 2)
        {
            cout << "Results: "
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

int main()
{
    cout << BOLDGREEN << banner << RESET;
    cout << "The number of users: " << User::loadUsers().size() << endl
         << endl;

    for (User user : User::loadUsers())
    {
        user.display();
        cout << endl;
    }

    User user = User::loadUsers().at(0); // FOR TESTING PURPOSE ONLY

    cout << "Display user: ";
    user.display();
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
