#include "command.h"

#include <algorithm>
#include <iterator>

#include "data.h"
#include "game.h"

Command::Command(DP::Game& currentGame) : game(currentGame) {}

/*!
 * \brief Command::removeCard
 * \param playerNumber number of the player which cards will be removed
 * \return
 */
bool Command::removeCard(int playerNumber) {
  /*! This should works as following:
   * - iterate over BoardDiamondSeq
   * - find diamonds belonging to the playerNumber
   * - create an array from the aboves
   * - get a random element
   * - remove from the board - set not active
   */

  std::vector<int> diamonds;
  for (auto&& i : game.boardDiamonds.diamonds) {
    int fieldNumber = i.playerNumber;
    if ((fieldNumber == playerNumber) && (i.boardPosition > -1) && (i.idNumber < 4)) {
      diamonds.push_back(i.boardPosition);
    }
  }
  int numberDiamonds = diamonds.size();
  if (numberDiamonds > 0) {
    int elemToRemove = rand() % numberDiamonds;
    game.boardDiamonds.collectField(diamonds[elemToRemove]);
    return true;
  }
  return false;
}

void Command::removeAllCardElement(int elementNumber) const {
  /*! This should works as following:
   * - iterate over BoardDiamondSeq
   * - find diamonds belonging to the playerNumber
   * - create an array from the aboves
   * - get a random element
   * - remove from the board - set not active
   */

  for (auto&& i : game.boardDiamonds.diamonds) {
    int cardElementNumber = i.idNumber;
    if ((i.boardPosition > -1) && (cardElementNumber == elementNumber)) {
      i.boardPosition = -1;
    };
  }
}
/*!
 * \brief Command::removeDiamond
 * \param playerNumber
 * \return if diamond had been removed
 */
bool Command::removeDiamond(int playerNumber) {
  /*! This should works as following:
   * - iterate over BoardDiamondSeq
   * - find diamonds belonging to the playerNumber
   * - create an array from the aboves
   * - get a random element
   * - remove from the board - set not active
   */

  std::vector<int> diamonds;
  for (auto&& i : game.boardDiamonds.diamonds) {
    int fieldNumber = i.playerNumber;
    if ((fieldNumber == playerNumber) && (i.boardPosition > -1) && (i.idNumber == 4)) {
      diamonds.push_back(i.boardPosition);
    }
  }
  int numberDiamonds = diamonds.size();
  if (numberDiamonds > 0) {
    int elemToRemove = rand() % numberDiamonds;
    game.boardDiamonds.collectField(diamonds[elemToRemove]);
    return true;
  }
  return false;
}

void Command::freezePlayer(int playerNumber) {
  game.players[playerNumber].frozenLeft += 1;
}

void Command::removeAllItems(int playerNumber) const {
  for (auto&& i : game.boardDiamonds.diamonds) {
    int itemPlayerNumber = i.playerNumber;
    if ((i.boardPosition > -1) && (itemPlayerNumber == playerNumber)) {
      i.boardPosition = -1;
    };
  }
}

/*!
 * \brief Command::processField when the player enters the field
 * \param pos
 */
void Command::processField(int pos) {
  bool startField = std::find(std::begin(DP::startPlayers), std::end(DP::startPlayers), pos) !=
                    std::end(DP::startPlayers);

  if ((startField) && (DP::startPlayers[game.turn] == pos)) {
    game.banner.setText("meditation");
    game.boardDiamonds.reorder(game.turn);
    game.sfx.soundMeditation.play();
  }

  // Hide big diamond when player enters center position (like in 0.8.2)
  if (pos == 136 && game.bigDiamondActive) {
    game.bigDiamondActive = false;
    game.players[game.turn].cash += 3; // Bonus for collecting center diamond
  }

  if (game.boardDiamonds.ifFieldIsEmpty(pos) == false) {
    game.sfx.playCollect();
    if (game.boardDiamonds.getNumberForField(pos) == 4) {
      game.players[game.turn].cash += 1;
      
      // YOUR PREFERRED APPROACH: Simple circle burst effect - ONLY for diamonds!
      // Use character CENTER position for perfect particle placement
      sf::Vector2f characterPos = game.players[game.turn].characters[0].getPosition();
      
      // Character dimensions from character.cpp: 32x58 pixels
      const float charWidth = 32.0f;
      const float charHeight = 58.0f;
      
      // Center the particles on the character sprite
      sf::Vector2f centerPos(
        characterPos.x + charWidth / 2.0f,
        characterPos.y + charHeight / 2.0f
      );
      
#ifndef NDEBUG
      std::cout << "DEBUG: DIAMOND COLLECTED - Character center: " << centerPos.x << ", " << centerPos.y << std::endl;
#endif
      game.getAnimationSystem()->createDiamondCollectionBurst(centerPos);

    } else if (game.boardDiamonds.getNumberForField(pos) < 4) {
      processCard(pos);
#ifndef NDEBUG
      std::cout << "DEBUG: CARD collected (not diamond) at position " << pos << std::endl;
#endif
    }

    game.boardDiamonds.collectField(pos);
  }
}

/*!
 * \brief Command::processCard when the user enters the field
 * \param pos
 */
void Command::processCard(int pos) {
  int tokenNumber = game.boardDiamonds.getNumberForField(pos);
  std::string cardType = game.cardsDeck.getTitle(tokenNumber);
  
  // Calculate character center position for particle effects
  sf::Vector2f characterPos = game.players[game.turn].characters[0].getPosition();
  const float charWidth = 32.0f;
  const float charHeight = 58.0f;
  sf::Vector2f centerPos(
    characterPos.x + charWidth / 2.0f,
    characterPos.y + charHeight / 2.0f
  );
  
  if (tokenNumber != game.turn) {
    if (cardType == "diamond") {
      removeDiamond(game.boardDiamonds.getNumberForField(pos));
      game.players[game.turn].cash += 1;
      
      // Add particle effect for diamond card collection
      game.getAnimationSystem()->createCollectionBurst(centerPos, DP::ParticlePresets::CARD_COLLECT);
      
      // Show notification for diamond card
      game.cardNotification.showCardNotification(cardType, game.turn, tokenNumber, tokenNumber);
      // Set delay for all players to ensure proper notification viewing time
      game.cardNotificationDelay = game.CARD_NOTIFICATION_DELAY_TIME;

    } else if (cardType == "stop") {
      freezePlayer(tokenNumber);
      
      // Add particle effect for stop card (falling particles)
      game.getAnimationSystem()->createCollectionBurst(centerPos, DP::ParticlePresets::STOP_CARD);
      
      // Show notification for stop card
      game.cardNotification.showCardNotification(cardType, game.turn, tokenNumber, tokenNumber);
      // Set delay for all players to ensure proper notification viewing time
      game.cardNotificationDelay = game.CARD_NOTIFICATION_DELAY_TIME;
    } else if (cardType == "card") {
      removeCard(game.boardDiamonds.getNumberForField(pos));
      
      // Add particle effect for card collection
      game.getAnimationSystem()->createCollectionBurst(centerPos, DP::ParticlePresets::CARD_COLLECT);
      
      // Show notification for remove card
      game.cardNotification.showCardNotification(cardType, game.turn, tokenNumber, tokenNumber);
      // Set delay for all players to ensure proper notification viewing time
      game.cardNotificationDelay = game.CARD_NOTIFICATION_DELAY_TIME;

    } else if (cardType == "diamond x 2") {
      if (removeDiamond(game.boardDiamonds.getNumberForField(pos)))
        game.players[game.turn].cash += 1;
      if (removeDiamond(game.boardDiamonds.getNumberForField(pos)))
        game.players[game.turn].cash += 1;
      
      // Add enhanced particle effect for double diamond card (more particles)
      DP::GameAnimationSystem::ParticleConfig doubleConfig = DP::ParticlePresets::DIAMOND_BURST;
      doubleConfig.count = 10;  // More particles for double effect
      doubleConfig.speed = 140.0f;  // Slightly faster
      game.getAnimationSystem()->createCollectionBurst(centerPos, doubleConfig);
      
      // Show notification for double diamond card
      game.cardNotification.showCardNotification(cardType, game.turn, tokenNumber, tokenNumber);
      // Set delay for all players to ensure proper notification viewing time
      game.cardNotificationDelay = game.CARD_NOTIFICATION_DELAY_TIME;
    }
  }
  game.cardsDeck.nextCard(tokenNumber);
  game.sfx.playCard();
}

// int Command::processGui(sf::Vector2f posGui)
//{
//     return posGui.x + posGui.y; /*! TODO: add processing some actions */
// }
