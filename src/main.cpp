/*!
 * \file main.cpp
 * \brief Main entry point for DeerPortal game
 * 
 * DeerPortal - A hybrid board and card game with elemental mechanics
 * This file contains the main() function that initializes and runs the game.
 */

#include <iostream>
#include <string>
#include "game.h"
#include "exceptions.h"
#include "error-handler.h"

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

        // Initialize error handler
        DeerPortal::ErrorHandler::getInstance().setErrorCallback(
            [](const DeerPortal::GameException& e) {
                std::cerr << "Game Error: " << e.what() << std::endl;
                if (!e.getContext().empty()) {
                    std::cerr << "Context: " << e.getContext() << std::endl;
                }
            }
        );

        // Create and run the game  
        DP::Game game(testMode);
        return game.run();
    }
    catch (const DeerPortal::AssetLoadException& e) 
    {
        std::cerr << "Critical Asset Error: " << e.what() << std::endl;
        std::cerr << "Failed to load: " << e.getFilename() << std::endl;
        return 1;
    }
    catch (const DeerPortal::SystemResourceException& e) 
    {
        std::cerr << "System Resource Error: " << e.what() << std::endl;
        return 2;
    }
    catch (const DeerPortal::GameException& e) 
    {
        std::cerr << "Game Error: " << e.what() << std::endl;
        if (!e.getContext().empty()) {
            std::cerr << "Context: " << e.getContext() << std::endl;
        }
        return 3;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return 4;
    }
    catch (...) 
    {
        std::cerr << "Unknown error occurred" << std::endl;
        return 5;
    }
}

