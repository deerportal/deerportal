#include <iostream>

#include "game.h"
#define _CRT_SECURE_NO_WARNINGS
#include <filesystem>

using namespace std;
using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::current_path;


int main(int argc,      // Number of strings in array argv
         char *argv[]   // Array of command-line argument strings
         )
{
    cout << "Current working directory: " << current_path() << endl;

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

