#include <iostream>

#include "game.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

int main(int argc,      // Number of strings in array argv
         char *argv[]   // Array of command-line argument strings
         )
{
    if (argc>1)
    {
        std::string commandMe;
        commandMe = argv[1];
        if ((argc==2) && (commandMe=="--test"))
        {
            DP::Game game(true);

        } else{
            DP::Game game(false);
        }
    } else
    {
        DP::Game game(false);
    }

    return 0;
}

