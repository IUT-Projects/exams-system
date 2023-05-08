#include <iostream>
#include <string.h>
#define MAX_PASSWORD_LENGTH 20

using namespace std;

#if defined _WIN32
#include <conio.h>
void getPassword(char password[MAX_PASSWORD_LENGTH + 1])
{
    int length = 0;
    cout << "Enter password: ";
    char ch;
    while ((ch = getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (length > 0)
            {
                length--;
                cout << "\b \b";
            }
        }
        else
        {
            if (length < MAX_PASSWORD_LENGTH)
            { // if there is space in the array
                password[length] = ch;
                length++;
                cout << '*';
            }
        }
    }
    password[length] = '\0'; // add the null terminator
    cout << endl
         << "Password entered: " << password << endl;
}
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined(__APPLE__)
#include <termios.h>
#include <unistd.h>

void getPassword(char password[MAX_PASSWORD_LENGTH + 1])
{
    int length = 0;
    cout << "Enter password: ";
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch;
    while ((ch = getchar()) != '\n')
    {
        if (ch == '\b')
        {
            if (length > 0)
            {
                length--;
                cout << "\b \b";
            }
        }
        else
        {
            if (length < MAX_PASSWORD_LENGTH)
            {
                password[length] = ch;
                length++;
                cout << '*';
            }
        }
    }
    password[length] = '\0';
    cout << endl
         << "Password entered: " << password << endl;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
#endif