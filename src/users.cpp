#include <iostream>
#include <string>

using namespace std;

/*
TODO

We have to create class for storing and manipulating user data.
Also, there should be three roles:

- Teacher
* Can:
    Create and delete own exams
    Get list of students from exam results

- student
* Can:
    Take exams
    See own results

- Administrator
* Full access
*/

class User
{
private:
    int age, id;
    string first_name, last_name;

public:
    User(){};
};

class Teacher : public User
{
};
class Administrator : public User
{
};
class Student : public User
{
};