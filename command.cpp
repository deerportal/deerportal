#include "command.h"

Command::Command(efc::Game& currentGame) :
    game(currentGame)
{

}

void Command::removeDiamond(int playerNumber)
{
    /* This should works as following:
     * - iterate over BoardDiamondSeq
     * - find diamonds belonging to the playerNumber
     * - create an array from the aboves
     * - get a random element
     * - remove from the board - set not active
     */
    std::vector<int> diamonds;
    for (auto&& i: game.boardDiamonds.diamonds)
    {
        int areaNumber = i.playerNumber;
        if (areaNumber==playerNumber)
        {
            diamonds.push_back(areaNumber);
        }
    }
    int numberDiamonds = diamonds.size();
    int elemToRemove = rand() %  numberDiamonds;
}
