#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>
#include <unistd.h>

using namespace std;

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

        if (cin.fail() || value < 0) // if it is not kind of integer
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

string generateRandomString(const int length)
{
    // Function for generating random alpha numeric string
    // Stolen from stackoverflow

    srand((unsigned)time(NULL) * getpid());
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string temp_string;
    temp_string.reserve(length);

    for (int iterations = 0; iterations < length; ++iterations)
    {
        temp_string += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return temp_string;
}

string generateRandomIDwithPrefix(char prefix)
{
    // Returns random string with pattern like:

    // ROLE-RANDOMSTRING
    // e.g T-54as56d4sa
    const int string_length = 8;

    string ID = "";
    string randomString = generateRandomString(string_length);
    ID += prefix;
    ID += '-';
    ID.append(randomString);

    return ID;
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

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__)
#include <sys/ioctl.h>
#endif

pair<int, int> getTerminalSize()
{

    pair<int, int> window_size;

    // stolen from stackoverflow
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    window_size.first = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    window_size.second = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__)
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    window_size.first = (int)(w.ws_col);
    window_size.second = (int)(w.ws_row);
#endif // Windows/Linux

    return window_size;
}

void quit()
{
    cout << "Good bye!" << endl;
    exit(0);
}

bool confirm()
{
    string answer;

    cout << "Are you sure (y/n)? ";
    cin.ignore();
    getline(cin, answer);
    toLowerCase(answer);

    return (answer == "y" || answer == "yes");
}