#ifndef COMMAND_H
#define COMMAND_H
#include "vector"

namespace efc {
    class Game;
}

class Command
{
public:
//    Command();
    explicit Command(efc::Game &currentGame);

    /*!
     * \brief Removes a diamond from the player's field.
     * \param player
     */
    bool removeDiamond(int playerNumber);
    bool removeCard(int playerNumber);
    void freezePlayer(int playerNumber);
    void removeAllItems(int playerNumber);
    efc::Game &game;

    void processField(int pos);
    void processCard(int pos);

//    int processGui(sf::Vector2f posGui);

    void removeAllCardElement(int elementNumber);
};

#endif // COMMAND_H
