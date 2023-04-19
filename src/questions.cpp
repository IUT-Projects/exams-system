#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
TODO

Classes for representing questions. There is one main class called question.
In future I will update all list of requirements for TODO.

*/

class Question
{
private:
    string question, answer, user_answer;
public:
    Question(){};
    Question(string question,  string answer){
        this->question = question;  this->answer = answer; 
    }
    string getQuestion(){return question;}
    string getAnswer(){return answer;}
    string getUser_Answer(){return user_answer;}
    void setQuestion(string question){this->question=question;}
    void setAnswer(string answer){this->answer=answer;}
    void setUser_Answer(string user_answer){this->user_answer=user_answer;}
};
class Test: private Question
{
private:
    string* variants;
    int num_variants;
public:
    Test(int num_variants) {
    this->num_variants = num_variants;
    variants = new string[num_variants];
    }
    void resize_variants(int um_variants) {   //we need this shit to change the amount of variants, by defolt we will put 4 and we will ask if teacher wants to change it
    delete[] variants;
    variants = new string[um_variants];
    this->num_variants = um_variants;
    }
    void setVariants(){
        cout<<"Enter the variants"<<endl;
        for(int i = 0; i>num_variants;i++){
            char variant = 'A';
            cout<<"Variant "<< variant <<":";
            cin>>variants[i];
            variant++;
        }
    }
    void getVariants(){
        for(int i = 0; i>num_variants;i++){
        char variant = 'A';
        cout<<"Variant "<< variant <<": "<<variants[i]<<endl;
        variant++;
        }
    }
    void userAnswer(char answer){
        int user_answer = answer % 65;
        setUser_Answer(variants[user_answer]);
    }
    int checkAnswer(char answer){
        int user_answer = answer % 65;
        if (variants[user_answer] == getUser_Answer()){
            return 1;
        }else {return 0;}
    }
    ~Test() {
        delete[] variants;
    }
};

class QuestionSet
{
};
