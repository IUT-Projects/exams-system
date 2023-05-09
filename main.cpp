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
#include "components/password_masking.h"

using namespace std;

// User roles as variable
string TEACHER = "teacher", STUDENT = "student", ADMIN = "admin";

/*
Definition of all classes.
*/

class Exam;

class User
{
private:
    string name, role, ID, password;
    vector<Exam> exams;

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
    vector<Exam> getExams();
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

    string getUserId()
    {
        return user_id;
    }

    string getExamId()
    {
        return exam_id;
    }

    void display();
    void writeToFile();
    static vector<Result> loadResults();
    static vector<Result> loadResultsByExam(Exam exam);
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
        this->ID = ID.empty() ? Exam::createExamID() : ID;
    };

    void start(User user);
    void insertQuestions(User author);
    // Utility functions
    static string createExamID();
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
    string getTitle();
    User getAuthor();

    // For file operations
    void setTitle(string title);
    void setID(string ID);
    void getAuthorFromFile(string author_id);
    void loadQuestions();

    void writeToFile();
    void removeFromFile();
    static vector<Exam> loadExams();
    friend vector<Result> Result::loadResultsByExam(Exam exam);
};

// USER

void User::display()
{
    cout << "ID: " << ID << "\n";
    cout << "Name: " << name << "\n";
    cout << "Role: " << role << "\n";
}

vector<Exam> User::getExams()
{
    this->exams.clear();
    for (Exam exam : Exam::loadExams())
    {
        if (exam.getAuthor().getID() == this->getID())
        {
            this->exams.push_back(exam);
        }
    }

    return this->exams;
}

string User::createUserID(User *user)
{
    char prefix = (char)toupper(user->role[0]);
    return generateRandomIDwithPrefix(prefix);
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

        if (line.size() == 0)
        {
            break;
        }

        Result result(result_data[0], result_data[1]);
        result.setTotalScore(stoi(result_data[2]));
        result.setMaxPossibleScore(stoi(result_data[3]));
        results.push_back(result); // add to results
    }
    file.close();
    return results;
}

vector<Result> Result::loadResultsByExam(Exam exam)
{

    vector<Result> results;

    for (Result result : Result::loadResults())
    {
        if (result.exam_id == exam.ID)
        {
            results.push_back(result);
        }
    }

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
    vector<Result> results;

    for (Result result : Result::loadResultsByExam(*this))
    {
        if (result.getUserId() == user.getID())
        {
            cout << "You already participated to this exam!" << endl;
            return;
        }
    }
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

User Exam::getAuthor()
{
    return this->author;
}

string Exam::getTitle()
{
    return this->title;
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

void Exam::removeFromFile()
{
    // load all data before deleting
    vector<Exam> exams = Exam::loadExams();

    ofstream file;
    file.open(EXAMS_DATA_PATH, ios::out | ios::trunc);
    file.close();

    for (Exam exam : exams)
    {
        if (exam.ID != this->ID)
        {
            exam.writeToFile();
        }
    }
}
string Exam::createExamID()
{
    // Add E latter at the beginning of random string
    return generateRandomIDwithPrefix('E');
}

void Exam::info()
{
    cout << "\nTitle: " << this->getTitle() << endl;
    cout << "Exam ID: " << this->ID << endl;
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

    User user;
    int role_option;
    string name, role, password;

    cout << "What is your name? ";
    getline(cin, name);

    password = getPassword();

    cout << "\nSelect your role: "
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
    User *user = NULL;

login:
    string ID, password;
    cout << "Enter ID: ";
    cin >> ID;
    cin.ignore();
    password = getPassword();
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
        cout << "\nID or password is incorrect. Please try again." << endl;
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

void displayResults(vector<Result> results)
{

    cout << "\n[ RESULTS ]\n";
    cout << "№ \tUser ID\t  Score" << endl;

    int counter{1};
    for (Result result : results)
    {
        cout << counter << ".\t" << result.getUserId() << "    " << result.getTotalScore() << endl;
        counter++;
    }
    cout << "\n\n";
}

void teacherMenu(User user)
{
    vector<Exam> myExams;
    bool isRunning = true;
    int option, counter{0};

    while (isRunning)
    {
        cout << "\n[ MENU ]"
             << "\n";
        cout << "1. Create exam"
             << "\n";
        cout << "2. List of your exams"
             << "\n";
        cout << "3. User info"
             << "\n";
        cout << "4. Logout" << endl;
        cout << "5. Quit"
             << "\n";
        integerInput("Your option", option);
        clear();

        if (option == 1)
        {
            // Option for creating new EXAM
            Exam *exam = new Exam;
            exam->insertQuestions(user);
            exam->writeToFile();
            delete exam;
        }
        else if (option == 2)
        {
            // Print list of all exams
            myExams = user.getExams();
            cout << BOLDMAGENTA << "[ MY EXAMS ]\n"
                 << RESET;
            if (myExams.size())
            {
                for (int index = 0; index < myExams.size(); index++)
                {
                    cout << index + 1 << ". " << myExams.at(index).getTitle() << endl;
                }
                integerInput("Select the exam", option);
            }

            // check user input range
            if (1 <= option && option <= myExams.size())
            {
                // Get exam by array index
                Exam exam = myExams.at(option - 1);
                exam.info();
            selectAction:

                cout << "\n[ ACTIONS ]\n";
                cout << "1. Load stats" << endl;
                cout << "2. Test examination process" << endl;
                cout << "3. Delete Exam" << endl;
                cout << "4. Exam info" << endl;
                cout << "(other). Back" << endl;
                integerInput("Your Option ", option);

                if (option == 1)
                {
                    // load all results from filesystem
                    vector<Result> results = Result::loadResultsByExam(exam);

                    if (results.size() == 0)
                    {
                        clear();
                        cout << "Results not found :(" << endl;
                    }
                    else
                    {
                        // Print all results like table
                        clear();
                        displayResults(results);
                    }
                    goto selectAction;
                }
                else if (option == 2)
                {
                    // Test your exam
                    clear();
                    exam.start(user);
                    goto selectAction;
                }
                else if (option == 3)
                {

                    // Delete your exam
                    string answer;

                    cout << "Are you sure? (y/n)";
                    cin.ignore();
                    getline(cin, answer);
                    toLowerCase(answer);
                    bool approve = (answer == "y" || answer == "yes");
                    cout << boolalpha << approve << endl;
                    if (approve)
                    {
                        exam.removeFromFile();
                        cout << "Exam deleted!" << endl;
                    }
                }
                else if (option == 4)
                {
                    exam.info();
                }
            }
            else
            {
                cout << "There are no exams!\n";
            }
        }

        else if (option == 3)
        {
            user.display();
        }
        else if (option == 4)
        {
            isRunning = false;
        }
        else if (option == 5)
        {
            cout << "Good bye!" << endl;
            exit(0);
        }
        else
        {
            cout << "Wrong input!" << endl;
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
        else if (option == 4)
        {
            isRunning = false;
        }
        else
        {
            cout << "Wrong input!" << endl;
        }
    }
}

int main()
{
authProcess:
    // User user = User::loadUsers().at(0); // FOR TESTING PURPOSE ONLY
    User user = performAuth();
    cout << endl;
    
    if (user.Role() == TEACHER)
    {
        teacherMenu(user);
    }
    else if (user.Role() == STUDENT)
    {
        studentMenu(user);
    }
    else
    {
        cout << "Good bye!"
             << "\n";
        exit(0);
    }
    goto authProcess;

    return 0;
};