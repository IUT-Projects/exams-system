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
vector<string> split_word(string answer){
vector<string> new_answer;
        string word;
        char space = ' ';
        for(char c : answer){
            if(c==space){
                new_answer.push_back(word);
                word.clear();}else{word += c;}
                new_answer.push_back(word);
                return new_answer;
        }}
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
class User;
string createID(User *object)
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

    return ID.substr(5, ID.size());
}

/* QUESTIONS */

class Question
{
    // Base class for all questions
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
    string getUserAnswer() { return user_answer; }
    void setQuestion(string question) { this->question = question; }
    void setAnswer(string answer) { this->answer = answer; }
    void setUserAnswer(string user_answer) { this->user_answer = user_answer; }
};

class WrittenAnswer : public Question
{
    private:
    string answer;
    vector<string> real_answer;
    public:
    void getReal_answer(){
        int number_of_possible_answers;
        string possible_answer;
        cout<<"Enter how many possible answers do you want to include:";
        cin>>number_of_possible_answers;
        for(int i = 0; i < number_of_possible_answers; i++){
            cout<<"Enter popssible answer number "<<i;
            cin >>possible_answer;
            real_answer.push_back(possible_answer);
            possible_answer.clear();
        }
    }
    void compareAnswer(string answer){
        //spliting all possible answers into words and then putting it into vector of vectors
        vector<vector<string > > new_real_answer, matching_words;
        for(string possible_answer : real_answer){
            new_real_answer.push_back(split_word(possible_answer));
        }
        //finding matching words with possible answers
        vector<string>new_answer = split_word(answer),matching_words_1; 
        for(vector<string> possible_answer : new_real_answer){
            for(string word1 : possible_answer){
                for(string word2 :new_answer ){
                    if(word2 == word1){
                        matching_words_1.push_back(word2);
                        matching_words.push_back(matching_words_1);
                        break;}}  matching_words_1.clear();  }}
        //printing matching words with all possible answers
        for(int i =0; i < new_real_answer.size();i++){
            cout<<"\nMatching words with possible answer number "<<i<<" : ";
            for(string word : matching_words[i]){
                cout<<word<<" ";
            }
        }

    }
    bool checkAnswer(string answer){
        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
        if(this->answer==answer){return true;}else{return false;}
    }
};

class MultipleChoice : public Question
{
private:
    int variants_count;
    vector<string> variants;

public:
    MultipleChoice() : variants_count(DEFAULT_VARIANTS_NUMBER) {} ;
    MultipleChoice(int variants_count) : variants_count(variants_count) {};

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
        for (int i = 0; i < variants_count; i++)
        {
            cout << option << ") " << variants[i] << endl;
            option++;
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
        setUserAnswer(variants[user_answer]);
    }
    bool checkAnswer(char answer)
    {
        int user_answer = answer % 65;
        if (variants[user_answer] == getUserAnswer())
        {
            return true;
        }
        return false;
    }
};

class Exam
{
    // TODO
    // Implement exam class that contains info about
    // users (who took)
    // author
    // questions
};

/* USERS */

string TEACHER = "teacher", STUDENT = "student", ADMIN = "admin";

class User
{
private:
    string name, role, ID;
    int age;

public:
    User()
    {
        this->ID = createID(this);
    };
    void setData(string name, int age, string role)
    {
        this->age = age;
        this->name = name;
        this->role = role;
    }
    void display()
    {
        cout << "ID:" << ID << endl;
        cout << "Name:" << name << endl;
        cout << "Age:" << age << endl;
        cout << "Role:" << role << endl;
    }
};
// TODO implement functions that stores all user data in file
// Implement user search from file


void registerUser()
{
    User user1, user2;
    string name, role;
    int age;
    cout << "Enter name, age and role: ";
    cin >> name >> age >> role;
    user1.setData(name, age, role);
    user2.setData(name, age, role);
    user1.display();
    user2.display();
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
        cout << "\n[ MENU ] " << endl;
        cout << " 1. Test user add" << endl;
        cout << " 0. Quit" << endl;

        cout << "Your Option> ";
        cin >> user_option;

        clear();
        switch (user_option)
        {
        case 1:
            registerUser();
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