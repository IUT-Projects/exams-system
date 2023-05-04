// imports
#include <fstream>
#include <iomanip>
#include <sstream>
// constants
#define DEFAULT_VARIANTS_NUMBER 3
#define MAX_USER_ANSWER_LENGTH 128
#define USER_DATA_PATH "./data/users.txt"
#define EXAMS_DATA_PATH "./data/exams.txt"
#define RESULTS_DATA_PATH "./data/results.txt"
#define MULTI_CHOICE_QUESTIONS_DATA_PATH "./data/questions/multi.txt"
#define SHORT_ANSWER_QUESTIONS_DATA_PATH "./data/questions/short.txt"

#include "components/utils.h"
#include "components/banners.h"

using namespace std;

// Get terminal size

const int WIDTH = getTerminalSize().first;
const int HEIGHT = getTerminalSize().second;

// User roles as variable
string TEACHER = "teacher", STUDENT = "student", ADMIN = "admin";

/*
Definition of all classes.
*/

class User
{
private:
    string name, role, ID, password;

public:
    User(){};
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
    void display();
    static string createUserID(User *user);
    static void addUser(User user);
    static vector<User> loadUsers();
    friend class Exam; // Exam class needs to access private info of User
    void setData(string name, string role, string password, string ID);

    // some function for auth needs access to private members
    friend User performLogin();
    friend User performRegister();
};

class Result
{
    /*
    Class for representing exam results of user.
    It supports file operations.
    */
private:
    string exam_id, user_id;
    int total_score{0}, max_possible_score;

public:
    Result(){};
    Result(string _exam_id, string _user_id) : exam_id(_exam_id), user_id(_user_id) {}
    void incrementScore()
    {
        this->total_score++;
    }
    int getTotalScore()
    {
        return this->total_score;
    }

    void setTotalScore(int score)
    {
        this->total_score = score; // only for file operations
    }

    void setMaxPossibleScore(int score)
    {
        this->max_possible_score = score;
    }

    void display();
    void writeToFile();
    static vector<Result> loadResults();
};

class Question
{
    // Basic class for representing question data
protected:
    string question, exam_id;

public:
    Question(){};
    // not implemented yet, after inheriting they will be
    void start(Result result);
    void checkAnswer();
    void setExamID(string ID);
    void setQuestion(string question);
    friend class Exam;
};

class ShortAnswerQuestion : public Question
{
private:
    string correct_answer, user_answer;

public:
    string type = "written";

    void input();
    void start(Result &result);
    void setAnswer(string answer);
    void setUserAnswer(string answer);
    bool checkAnswer();

    void writeToFile();
    static vector<ShortAnswerQuestion> load();
};

class MultipleChoice : public Question
{
private:
    vector<string> variants;
    int variants_count, correct_option, user_option;

public:
    string type = "multi";
    MultipleChoice() : variants_count(DEFAULT_VARIANTS_NUMBER){};
    MultipleChoice(int _variants_count) : variants_count(_variants_count){};

    void input();
    void start(Result &result);
    void setVariants();
    bool checkVariantExists(char &variant);
    void showVariants();
    void setUserAnswer(char user_answer);
    void setAnswer(char correct_answer);
    bool checkAnswer();

    void writeToFile();
    static vector<MultipleChoice> load();
};

class Exam
{
private:
    string ID;
    User author;
    string title;
    vector<MultipleChoice> multi_questions;
    vector<ShortAnswerQuestion> short_answer_questions;

public:
    Exam()
    {
        // If user provides id, then init with this id
        // If not, create it
        this->ID = ID.empty() ? Exam::createExamID(this) : ID;
    };

    void start(User user);
    void insertQuestions(User author);
    // Utility functions
    static string createExamID(Exam *exam);
    void info();
    template <typename T> // T can be MultipleChoice || ShortAnswer object
    void shuffleQuestions(vector<T> &questions);
    // Setters getters(for including questions)
    void includeMultipleChoiceQuestions(vector<MultipleChoice> questions);
    void includeShortAnswerQuestions(vector<ShortAnswerQuestion> questions);
    void includeMultipleChoiceQuestion(MultipleChoice question);
    void includeShortAnswerQuestion(ShortAnswerQuestion question);
    int getTotalNumberOfQuestions();
    void displayResults(Result result);

    // For file operations
    void setTitle(string title);
    void setID(string ID);
    void getAuthorFromFile(string author_id);

    void loadQuestions();

    void writeToFile();
    static vector<Exam> loadExams();
};

// USER

void User::display()
{
    cout << "ID: " << ID << "\n";
    cout << "Name: " << name << "\n";
    cout << "Role: " << role << "\n";
}
string User::createUserID(User *user)
{
    // From mem. addr it creates unique user ID
    string ID = objectMemoryAddrAsString<User>(user);
    // (char)toupper(object->role[0]) - is first letter of role
    // if role is student, we have "S"
    return (char)toupper(user->role[0]) + ID.substr(5, ID.size());
}

void User::addUser(User user)
{
    // Write user object to data
    fstream file;
    file.open(USER_DATA_PATH, ios::app);

    file << user.name << "|" << user.role << "|" << user.password << "|" << user.ID << "\n";

    file.close();
}
void User::setData(string name, string role, string password, string ID = "")
{
    this->name = name;
    this->role = role;
    this->password = password;
    // If user provides id, then init with this id
    // If not, create it
    this->ID = ID.empty() ? User::createUserID(this) : ID;
}

vector<User> User::loadUsers()
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

// RESULTS

void Result::display()
{
    cout << "[RESULT]" << endl;
    cout << "Exam ID: " << exam_id << endl;
    cout << "User ID: " << user_id << endl;
    cout << "Total score: " << total_score << "/" << max_possible_score << endl;
}

void Result::writeToFile()
{
    fstream file;
    file.open(RESULTS_DATA_PATH, ios::app);

    file << exam_id << "|" << user_id << "|" << total_score << "|" << max_possible_score << "\n";

    file.close();
}

vector<Result> Result::loadResults()
{
    string line;
    vector<Result> results; // saving output result

    fstream file;
    file.open(RESULTS_DATA_PATH, ios::in);

    while (getline(file, line)) // one result_data IN one single line
    {
        vector<string> result_data = split(line, "|"); // split line by | and get vector of strings

        Result result(result_data[0], result_data[1]);
        result.setTotalScore(stoi(result_data[2]));
        result.setMaxPossibleScore(stoi(result_data[3]));
        results.push_back(result); // add to results
    }
    file.close();
    return results;
}

// QUESTIONS

void Question::setExamID(string ID)
{
    this->exam_id = ID;
}
void Question::setQuestion(string question_text)
{
    this->question = question_text;
}
void ShortAnswerQuestion::input()
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

void ShortAnswerQuestion::writeToFile()
{
    fstream file;
    file.open(SHORT_ANSWER_QUESTIONS_DATA_PATH, ios::app);

    file << exam_id << "|" << question << "|" << correct_answer << "\n";

    file.close();
}

vector<ShortAnswerQuestion> ShortAnswerQuestion::load()
{
    string line;
    vector<ShortAnswerQuestion> questions; // saving output result

    fstream file;
    file.open(SHORT_ANSWER_QUESTIONS_DATA_PATH, ios::in);

    while (getline(file, line)) // one result_data IN one single line
    {
        vector<string> question_data = split(line, "|"); // split line by | and get vector of strings

        ShortAnswerQuestion question;

        question.setExamID(question_data[0]);
        question.setQuestion(question_data[1]);
        question.setAnswer(question_data[2]);

        questions.push_back(question); // add to results
    }
    file.close();

    return questions;
}

void ShortAnswerQuestion::start(Result &result)
{
    char userAnswer[MAX_USER_ANSWER_LENGTH];

    cout << endl
         << "Short Answer question: " << this->question << "\n";
    cout << "Your answer: ";
    cin.getline(userAnswer, sizeof(userAnswer));

    this->setUserAnswer(userAnswer);

    if (this->checkAnswer())
    {
        result.incrementScore();
    }
}
bool ShortAnswerQuestion::checkAnswer()
{
    return (this->correct_answer == this->user_answer);
}
void ShortAnswerQuestion::setAnswer(string answer)
{
    // Convert all answers to lowercase string
    toLowerCase(answer);
    this->correct_answer = answer;
}
void ShortAnswerQuestion::setUserAnswer(string answer)
{
    toLowerCase(answer);
    this->user_answer = answer;
}

void MultipleChoice::writeToFile()
{
    fstream file;
    file.open(MULTI_CHOICE_QUESTIONS_DATA_PATH, ios::app);

    file << exam_id << "|" << question << "|" << correct_option << "|" << variants_count;

    for (int iter = 0; iter < variants_count; iter++)
    {
        file << "|" << variants.at(iter);
    }

    file << endl;

    file.close();
}

vector<MultipleChoice> MultipleChoice::load()
{
    string line;
    vector<MultipleChoice> questions; // saving output result

    fstream file;
    file.open(MULTI_CHOICE_QUESTIONS_DATA_PATH, ios::in);

    while (getline(file, line)) // one result_data IN one single line
    {
        vector<string> question_data = split(line, "|"); // split line by | and get vector of strings

        MultipleChoice question;

        question.setExamID(question_data[0]);
        question.setQuestion(question_data[1]);
        question.setAnswer(stoi(question_data[2]));
        question.variants_count = stoi(question_data[3]);

        for (int iter = 0; iter < question.variants_count; iter++)
        {
            question.variants.push_back(question_data[4 + iter]);
            // explanation: after 3 index(from 4), there will be variants
        }

        questions.push_back(question); // add to results
    }
    file.close();
    return questions;
}

void MultipleChoice::input()
{
    integerInput("Enter number of variants", this->variants_count);
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

void MultipleChoice::start(Result &result)
{
    cout << endl
         << this->question << "\n";
    this->showVariants();

    char user_answer;
    cout << "Your answer is: ";
    cin >> user_answer;
    this->setUserAnswer(user_answer);

    if (this->checkAnswer())
    {
        result.incrementScore();
    }
}
void MultipleChoice::setVariants()
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

bool MultipleChoice::checkVariantExists(char &variant)
{
    char option = 'A';

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

void MultipleChoice::showVariants()
{
    char option = 'A';
    for (string variant : variants)
    {
        cout << option << ")" << variant << "\n";
        option++;
    }
}
void MultipleChoice::setUserAnswer(char user_answer)
{
    this->user_option = toupper(user_answer) % 65;
}
void MultipleChoice::setAnswer(char correct_answer)
{
    this->correct_option = toupper(correct_answer) % 65;
}

bool MultipleChoice::checkAnswer()
{
    return (this->correct_option == this->user_option);
}

// Exam related things

void Exam::start(User user)
{
    cout << "Exam is started by "
         << "\n";
    user.display();

    this->shuffleQuestions(this->multi_questions);
    this->shuffleQuestions(this->short_answer_questions);

    Result result(this->ID, user.getID());

    result.setMaxPossibleScore(this->getTotalNumberOfQuestions());

    for (MultipleChoice question : multi_questions)
    {
        question.start(result);
    }
    cin.ignore();

    for (ShortAnswerQuestion question : short_answer_questions)
    {
        question.start(result);
    }

    this->displayResults(result);
    result.writeToFile();
}
void Exam::insertQuestions(User author)
{
    int type, number_of_questions;

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
            MultipleChoice question;
            question.setExamID(this->ID);
            question.input();
            this->includeMultipleChoiceQuestion(question);
            question.writeToFile();
        }
        else if (type == 2)
        {
            ShortAnswerQuestion question;
            question.setExamID(this->ID);
            question.input();
            this->includeShortAnswerQuestion(question);
            question.writeToFile();
        }
        else
        {
            cout << "Wrong input, try again!"
                 << "\n";
        }
    }
}

string Exam::createExamID(Exam *exam)
{
    // From mem. addr it creates unique user ID
    string ID = objectMemoryAddrAsString<Exam>(exam);
    // It will return E3921893 like unique ID.
    return "E" + ID.substr(5, ID.size());
}

void Exam::info()
{
    cout << "\n"
         << "\n["
         << this->title << " exam ]"
         << "\n";
    cout << "Author: " << this->author.Name() << "\n";
    cout << "Total number of questions: " << this->getTotalNumberOfQuestions() << "\n";
    cout << "Number of written questions: " << this->short_answer_questions.size() << "\n";
    cout << "Number of multi-choice questions: " << this->multi_questions.size() << "\n"
                                                                                    "\n";
}

template <typename T> // T can be MultipleChoice || ShortAnswer object
void Exam::shuffleQuestions(vector<T> &questions)
{
    // code below stolen from stackoverlow
    auto random_engine = default_random_engine{};
    shuffle(begin(questions), end(questions), random_engine);
}

// Setters getters(for including questions)
void Exam::includeMultipleChoiceQuestions(vector<MultipleChoice> questions)
{
    for (MultipleChoice question : questions)
    {

        this->multi_questions.push_back(question);
    }
}
void Exam::includeShortAnswerQuestions(vector<ShortAnswerQuestion> questions)
{
    for (ShortAnswerQuestion question : questions)
    {

        this->short_answer_questions.push_back(question);
    }
}
void Exam::includeMultipleChoiceQuestion(MultipleChoice question)
{
    this->multi_questions.push_back(question);
}
void Exam::includeShortAnswerQuestion(ShortAnswerQuestion question)
{
    this->short_answer_questions.push_back(question);
}

int Exam::getTotalNumberOfQuestions()
{
    return this->multi_questions.size() + this->short_answer_questions.size();
}

void Exam::writeToFile()
{
    // Write user object to data
    fstream file;
    file.open(EXAMS_DATA_PATH, ios::app);

    file << ID << "|" << title << "|" << author.getID() << "|\n";

    file.close();
};

void Exam::setTitle(string title)
{
    this->title = title;
}
void Exam::setID(string ID)
{
    this->ID = ID;
}

void Exam::getAuthorFromFile(string author_id)
{
    for (User user : User::loadUsers())
    {
        if (user.getID() == author_id)
        {
            this->author = user;
        }
    }
}

void Exam::loadQuestions()
{
    for (MultipleChoice question : MultipleChoice::load())
    {
        if (question.exam_id == this->ID)
        {
            this->includeMultipleChoiceQuestion(question);
        }
    }

    for (ShortAnswerQuestion question : ShortAnswerQuestion::load())
    {
        if (question.exam_id == this->ID)
        {
            this->includeShortAnswerQuestion(question);
        }
    }
}

vector<Exam>
Exam::loadExams()
{
    string line;
    vector<Exam> exams; // saving output result

    fstream file;
    file.open(EXAMS_DATA_PATH, ios::in);

    while (getline(file, line))
    {
        vector<string> exam_data = split(line, "|");
        Exam exam;

        exam.setID(exam_data[0]);
        exam.setTitle(exam_data[1]);

        exam.getAuthorFromFile(exam_data[2]);

        exam.loadQuestions();

        exams.push_back(exam);
    }
    file.close();
    return exams;
}

void Exam::displayResults(Result result)
{
    cout << endl
         << BOLDGREEN << setw(25) << "[ RESULTS ]" << RESET << setw(25) << endl;

    cout << "Total score: " << result.getTotalScore() << "/" << this->getTotalNumberOfQuestions() << endl;

    // not implemented yet!
}

/* Menu and forms */

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
            exam.writeToFile();

            cout << "\n";
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
    cout << "W is " << WIDTH << " H is " << HEIGHT << endl;

    cout << "The number of users: " << User::loadUsers().size() << endl
         << endl;

    for (User user : User::loadUsers())
    {
        user.display();
        cout << endl;
    }

    User user = User::loadUsers().at(0); // FOR TESTING PURPOSE ONLY

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