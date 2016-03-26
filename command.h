#ifndef COMMAND_H
#define COMMAND_H
#include "game.h"


namespace efc {
    class Game;
}

class Command
{
public:
    Command(efc::Game &game);
};

#endif // COMMAND_H
