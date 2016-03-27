#ifndef COMMAND_H
#define COMMAND_H
#include "game.h"


namespace efc {
    class Game;
}

class Command
{
public:
    Command(efc::Game &currentGame);

    /*!
     * \brief Removes a diamond from the player's field.
     * \param player
     */
    void removeDiamond(int playerNumber);
    efc::Game &game;
};

#endif // COMMAND_H
