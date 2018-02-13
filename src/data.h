#ifndef DATA_H
#define DATA_H
#include <map>
#include <string>


#include <string>
#include <SFML/Graphics.hpp>



namespace DP {
    extern std::string seasonsNames[4];
    enum directions {
        DIR_LEFT,
        DIR_RIGHT,
        DIR_UP,
        DIR_DOWN
    };

    enum cardType {
        FREEZE,
        COLLECT_DIAMOND
    };


    extern int terrainArray[24];
    extern sf::Color playersColors[4];
    extern std::array<std::array<int,2>,256> boards;

    extern int possibleExits[4]; // number of portal's exit where player stops to play
    const static int diamondsNumber = 112; // number of all cards / diamonds on the board
    extern std::array<std::array<int,3>,DP::diamondsNumber> DIAMONDS_SETUP;
    extern std::array<std::array<int,39>,4> occupiedFields;
    const static int numberSteps = 39;
    const static int startPlayers[4] = {0,15,255,240};
    const static int endPlayers[4] = {119,120,135,136};

    const static std::map<std::string, std::string> cardTypes = {
        {"stop",    "Freezes a player for the one turn time"},
        {"diamond", "Collect a diamond from the player area"},
        {"card",    "Collect a card from the player area"},
    };


}
#endif // DATA_H
