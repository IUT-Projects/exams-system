#include <string>
#include <iostream>

using std::string;
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


// BG colors
const string BG_RED = "\033[41m";                /* Red */
const string BG_GREEN = "\033[42m";              /* Green */
const string BG_YELLOW = "\033[43m";             /* Yellow */
const string BG_BLUE = "\033[44m";               /* Blue */
const string BG_MAGENTA = "\033[45m";            /* Magenta */
const string BG_CYAN = "\033[46m";               /* Cyan */
const string BG_WHITE = "\033[47m";              /* White */
