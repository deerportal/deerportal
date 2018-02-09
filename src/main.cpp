#include <iostream>

#include "game.h"

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
            efc::Game game(true);

        } else{
            efc::Game game(false);
        }
    } else
    {
        efc::Game game(false);
    }

    return 0;
}

