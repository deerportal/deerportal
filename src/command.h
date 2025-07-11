#ifndef COMMAND_H
#define COMMAND_H
#include "vector"

namespace DP {
class Game;
}

class Command {
public:
  //    Command();
  explicit Command(DP::Game& currentGame);

  /*!
   * \brief Removes a diamond from the player's field.
   * \param player
   */
  bool removeDiamond(int playerNumber);
  bool removeCard(int playerNumber);
  void freezePlayer(int playerNumber);
  void removeAllItems(int playerNumber) const;
  DP::Game& game;

  void processField(int pos);
  void processCard(int pos);
  void removeAllCardElement(int elementNumber) const;
};

#endif // COMMAND_H
