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
