#include "elem.h"

Elem::Elem() {
  boardPosition = -1;
}

/*!
 * \brief Character::getBoardPosition
 * \return
 */
int Elem::getBoardPosition() const {
  sf::Vector2f currentPos(getPosition());
  sf::Vector2i currentCords(DP::getCords(currentPos));
  int currentBoardPosition = DP::transCords(currentCords);
  return currentBoardPosition;
}

/*!
 * \brief Character::setBoardPosition
 * \param boardPosition
 */

void Elem::setBoardPosition(int newBoardPosition) {
  boardPosition = newBoardPosition;
  sf::Vector2i cords = DP::transPosition(boardPosition);
  sf::Vector2f newPos = DP::getScreenPos(cords);
  setPosition(sf::Vector2f(newPos.x, newPos.y));

  //   sf::FloatRect newSize(getGlobalBounds());
  //   float newX = newPos.x;
  //   newX = newX - (newSize.size.x);
  setPosition(sf::Vector2f(newPos.x, newPos.y));
  move(sf::Vector2f(202, 76));
}
