#include "game.h"
#include "command.h"

//Command::Command()
//{

//}

Command::Command(efc::Game &currentGame) :
    game(currentGame)
{
//    this->game = currentGame;
}

void Command::removeDiamond(int playerNumber)
{
    /*! This should works as following:
     * - iterate over BoardDiamondSeq
     * - find diamonds belonging to the playerNumber
     * - create an array from the aboves
     * - get a random element
     * - remove from the board - set not active
     */

    std::cout << "removing diamond number " << playerNumber << std::endl;

    std::vector<int> diamonds;
    for (auto&& i: game.boardDiamonds.diamonds)
    {
        int fieldNumber = i.playerNumber;
        if ((fieldNumber==playerNumber) && (i.boardPosition>-1)
                && (i.idNumber==4))
        {
            std::cout << "pushing found " << fieldNumber << std::endl;

            diamonds.push_back(i.boardPosition);
        }
    }
    int numberDiamonds = diamonds.size();
    std::cout << "found elements " << numberDiamonds << std::endl;
    if (numberDiamonds>=0)
    {
//        std::cout << "pushing found " << fieldNumber << std::endl;
        int elemToRemove = rand() %  numberDiamonds;
        std::cout << "random found " << elemToRemove << " " << diamonds[elemToRemove] << std::endl;

        game.boardDiamonds.collectField(diamonds[elemToRemove]);
    }


}
