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
    Command(efc::Game &currentGame);

    /*!
     * \brief Removes a diamond from the player's field.
     * \param player
     */
    void removeDiamond(int playerNumber);

    void freezePlayer(int playerNumber);
    efc::Game &game;

    void processField(int pos);
};

#endif // COMMAND_H
