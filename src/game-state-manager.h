#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include <SFML/Audio.hpp>

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameStateManager handles all state transitions and state-related logic
 *
 * This class manages the game's state machine, handling transitions between
 * different game states like menu, game, setup, etc. It also manages
 * associated audio and visual effects for state changes.
 */
class GameStateManager {
public:
  GameStateManager(Game* gameInstance);
  ~GameStateManager();

  // Primary state transition methods
  void transitionToMenu();
  void transitionToGame();
  void transitionToSetup();
  void transitionToIntroShader();
  void transitionToBoardAnimation();
  void transitionFromBoardAnimationToLetsBegin();
  void transitionToEndGame();
  void transitionToRollDice();
  void transitionToLetsBegin();

  // State management utilities
  void handleStateChange(int fromState, int toState);
  bool validateStateTransition(int newState) const;

  // Menu system management
  void showMenu();
  void hideMenu();
  void showIntroShader();
  void showGameBoard();

  // Game flow control
  void endGame();
  void startGame();
  void restartGame();

  // State queries
  int getCurrentState() const;
  std::string getCurrentStateName() const;
  bool isInGameState() const;
  bool isInMenuState() const;
  bool canTransitionTo(int targetState) const;

  // Audio management for states
  void playStateMusic(int state);
  void stopStateMusic(int state);
  void handleStateAudio(int newState);

private:
  Game* game; // Reference to main game instance

  // State transition helpers
  void prepareMenuState();
  void prepareGameState();
  void prepareSetupState();
  void prepareEndGameState();

  // Audio state management
  void startMenuMusic();
  void stopMenuMusic();
  void startGameMusic();
  void stopGameMusic();

  // State validation
  bool isValidState(int state) const;
  void logStateTransition(int from, int to) const;
};

} // namespace DP

#endif // GAME_STATE_MANAGER_H