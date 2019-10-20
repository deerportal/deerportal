#include "elem.h"

Elem::Elem()
{
    boardPosition = -1;
}

/*!
 * \brief Character::getBoardPosition
 * \return
 */
int Elem::getBoardPosition()
{
    sf::Vector2f currentPos(getPosition());
    sf::Vector2i currentCords(DP::getCords(currentPos));
    int currentBoardPosition = DP::transCords(currentCords);
    return currentBoardPosition;
}

/*!
 * \brief Character::setBoardPosition
 * \param boardPosition
 */

void Elem::setBoardPosition(int boardPosition)
{
    sf::Vector2i neededCords(DP::transPosition(boardPosition));
	sf::Vector2f newPos(DP::getScreenPos(neededCords));

    setPosition(newPos.x, newPos.y);
    this->boardPosition = boardPosition;

//    std::array<int,2> movements(getMovements(diceResult));
//    std::cout << "board pos >> " << boardPosition << " cords >>" << neededCords.x << " "   << neededCords.y
//              << "newpos >> " << newPos.x << " " << newPos.y << " "
//              << "movements >> " << movements[0] << " " << movements[1]
//              << std::endl;

	sf::Vector2i newVecPos(getPosition());
//   sf::FloatRect newSize(getGlobalBounds());
	// float newX = newVecPos.x + (DP::TILE_SIZE / 3);
	float newX =  newVecPos.x + (DP::TILE_SIZE/4);
//   newX = newX - (newSize.width);
   setPosition(newX, newPos.y);
   move(202,76);

}
