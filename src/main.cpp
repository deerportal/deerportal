/*!
 * \file main.cpp
 * \brief Main entry point for DeerPortal game
 * 
 * DeerPortal - A hybrid board and card game with elemental mechanics
 * This file contains the main() function that initializes and runs the game.
 */

#include <iostream>
#include <stdexcept>

// Forward declaration to avoid header include issues
namespace DP {
    class Game;
}

// Function declaration - implementation will be in game.cpp
DP::Game* createGame(bool testMode);
void runGame(DP::Game* game);
void destroyGame(DP::Game* game);

/*!
 * \brief Main entry point for the DeerPortal application
 * \param argc Number of command line arguments
 * \param argv Array of command line arguments
 * \return Exit code (0 for success)
 */
int main(int argc, char* argv[])
{
    try 
    {
        // Check for test mode flag
        bool testMode = false;
        if (argc > 1) 
        {
            std::string arg(argv[1]);
            if (arg == "--test" || arg == "-t") 
            {
                testMode = true;
                std::cout << "Running in test mode..." << std::endl;
            }
        }

        // Create and run the game
        DP::Game* game = createGame(testMode);
        runGame(game);
        destroyGame(game);
        
        return 0;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) 
    {
        std::cerr << "Unknown error occurred" << std::endl;
        return 2;
    }
}

