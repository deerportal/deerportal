#ifndef GAME_CORE_H
#define GAME_CORE_H

#include <array>
#include <set>
#include <string>

// Forward declarations for SFML
namespace sf {
class Time;
}

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameCore handles core game logic, state management, and game flow
 */
class GameCore {
public:
  GameCore(Game* gameInstance);
  ~GameCore();

  // Game initialization and lifecycle
  void initBoard();
  void restartGame();
  void endGame();

  // Player and turn management
  void nextPlayer();
  void nextRound();
  void launchNextPlayer();
  void setCurrentNeighbours();

  // Player actions and game mechanics
  void playerMakeMove(int mousePos);
  void throwDiceMove();
  void startDeerMode();
  int mostDiamonds() const;

  // Game state updates
  void update(const sf::Time& frameTime);

  // Command system
  void command(const std::string& command);

  // State queries
  bool isGameEnded() const;
  int getCurrentPlayer() const;
  bool isDeerModeActive() const;

private:
  Game* game; // Reference to main game instance

  // Helper methods for game logic
  void updatePlayerTimers(const sf::Time& frameTime);
  void updateVisualEffects(const sf::Time& frameTime);
  void updateAIBehavior(const sf::Time& frameTime);
  void processGameStateUpdates(const sf::Time& frameTime);
  void handlePlayerMovement(const sf::Time& frameTime);

  // Internal game flow helpers
  void checkEndGameConditions();
  void processPlayerTurn();
  void updateGameClock(const sf::Time& frameTime);

  // Game state variables
  int turn;
  int roundNumber;
  int currentSeason;
  int month;
  int diceResultPlayer;
  int numberFinishedPlayers;
  bool deerModeActive;
  int deerModeCounter;
  bool bigDiamondActive;

  // Timing
  float cpuTimeThinking;
  float cpuTimeThinkingInterval;
  float downTimeCounter;

  // Game state tracking
  std::set<int> currentNeighbours;

  // Helper methods
  void updateGameTimers(const sf::Time& frameTime);
  void processAITurn();
  void updateOscillator(const sf::Time& frameTime);
  void handlePlayerFinished(int playerIndex);

  // AI logic
  void processAIMovement();
  int selectAIMove(std::array<int, 2> movements);
  bool shouldMoveToPosition(int position) const;
};

} // namespace DP

#endif // GAME_CORE_H