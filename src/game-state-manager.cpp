#include "game-state-manager.h"

#include <iostream>

#include "board-initialization-animator.h"
#include "game.h"

namespace DP {

GameStateManager::GameStateManager(Game* gameInstance) : game(gameInstance) {}

GameStateManager::~GameStateManager() {}

// Primary state transition methods
void GameStateManager::transitionToMenu() {
  if (game->currentState != Game::state_menu) {
    handleStateChange(game->currentState, Game::state_menu);
    showMenu();
  }
}

void GameStateManager::transitionToGame() {
  if (game->currentState != Game::state_game) {
    handleStateChange(game->currentState, Game::state_game);
    game->currentState = Game::state_game;
  }
}

void GameStateManager::transitionToSetup() {
  if (game->currentState != Game::state_setup_players) {
    handleStateChange(game->currentState, Game::state_setup_players);
    showGameBoard();
  }
}

void GameStateManager::transitionToIntroShader() {
  if (game->currentState != Game::state_intro_shader) {
    handleStateChange(game->currentState, Game::state_intro_shader);
    showIntroShader();
  }
}

void GameStateManager::transitionToEndGame() {
  if (game->currentState != Game::state_end_game) {
    handleStateChange(game->currentState, Game::state_end_game);
    endGame();
  }
}

void GameStateManager::transitionToRollDice() {
  if (game->currentState != Game::state_roll_dice) {
    handleStateChange(game->currentState, Game::state_roll_dice);
    game->currentState = Game::state_roll_dice;
  }
}

void GameStateManager::transitionToBoardAnimation() {
#ifndef NDEBUG
  std::cout << "[DEBUG] Transition: " << getCurrentStateName() << " -> state_board_animation"
            << std::endl;
#endif
  if (game->currentState != Game::state_board_animation) {
    handleStateChange(game->currentState, Game::state_board_animation);
    game->currentState = Game::state_board_animation;
    game->boardAnimator->startAnimation();
  }
}

void GameStateManager::transitionFromBoardAnimationToLetsBegin() {
  if (game->currentState == Game::state_board_animation) {
#ifndef NDEBUG
    std::cout << "[DEBUG] Transitioning from board_animation to lets_begin with cleanup" << std::endl;
#endif
    
    handleStateChange(game->currentState, Game::state_lets_begin);
    game->currentState = Game::state_lets_begin;

    // Reset the timer for the 'Let's Begin' message fade-out
    game->downTimeCounter = 0;

    // Handoff complete, release animator resources
    if (game->boardAnimator) {
      game->boardAnimator->releaseDiamonds();
#ifndef NDEBUG
      std::cout << "[DEBUG] Released animated diamonds after state transition" << std::endl;
#endif
    }

    // Clean up lighting from the animation state
    if (game->lightingManager && game->boardAnimationLightingInitialized) {
      game->lightingManager->cleanup();
      game->boardAnimationLightingInitialized = false;
#ifndef NDEBUG
      std::cout << "[DEBUG] Cleaned up lighting system after animation" << std::endl;
#endif
    }

    // Prepare for the first player's turn
    game->launchNextPlayer();
  }
}

void GameStateManager::transitionToLetsBegin() {
  if (game->currentState != Game::state_lets_begin) {
    handleStateChange(game->currentState, Game::state_lets_begin);
    game->currentState = Game::state_lets_begin;
  }
}

// State management utilities
void GameStateManager::handleStateChange(int fromState, int toState) {
  if (!validateStateTransition(toState)) {
    std::cerr << "Invalid state transition from " << fromState << " to " << toState << std::endl;
    return;
  }

  logStateTransition(fromState, toState);
  handleStateAudio(toState);
}

bool GameStateManager::validateStateTransition(int newState) const {
  return isValidState(newState);
}

// Menu system management (extracted from game.cpp)
void GameStateManager::showMenu() {
  startMenuMusic();
  game->currentState = Game::state_menu;
  // Clear any active card notifications when returning to menu
  game->cardNotification.dismiss();
}

void GameStateManager::hideMenu() {
  stopMenuMusic();
}

void GameStateManager::showIntroShader() {
  // Start intro shader animation with menu music
  startMenuMusic();

  // Initialize intro shader with the pre-loaded intro menu image
  if (!game->introShader.initialize(sf::Vector2u(game->screenSize.x, game->screenSize.y),
                                    &game->textureIntroMenu)) {
    std::cerr << "Failed to initialize intro shader, going to menu" << std::endl;
    showMenu();
    return;
  }

  game->currentState = Game::state_intro_shader;
}

void GameStateManager::showGameBoard() {
  // Stop menu music and start game music
  stopMenuMusic();
  startGameMusic();

  // Reset game state when returning from menu
  game->restartGame(); // This sets the final positions for the diamonds

  // Go to setup_players state first (original flow)
  game->currentState = Game::state_setup_players;

  game->sfx.playLetsBegin();
}

// Game flow control (extracted from game.cpp)
void GameStateManager::endGame() {
  stopGameMusic();
  game->currentState = Game::state_end_game;
  game->downTimeCounter = 0;
  game->numberFinishedPlayers = 4;
  game->setTxtEndGameAmount();
}

void GameStateManager::startGame() {
  transitionToSetup();
}

void GameStateManager::restartGame() {
  // Reset game state and restart
  game->restartGame();
  transitionToRollDice();
}

// State queries
int GameStateManager::getCurrentState() const {
  return game->currentState;
}

std::string GameStateManager::getCurrentStateName() const {
  switch (game->currentState) {
  case Game::state_init:
    return "state_init";
  case Game::state_menu:
    return "state_menu";
  case Game::state_intro_shader:
    return "state_intro_shader";
  case Game::state_setup_players:
    return "state_setup_players";
  case Game::state_board_animation:
    return "state_board_animation";
  case Game::state_lets_begin:
    return "state_lets_begin";
  case Game::state_roll_dice:
    return "state_roll_dice";
  case Game::state_game:
    return "state_game";
  case Game::state_gui_elem:
    return "state_gui_elem";
  case Game::state_select_building:
    return "state_select_building";
  case Game::state_gui_end_round:
    return "state_gui_end_round";
  case Game::state_end_game:
    return "state_end_game";
  case Game::state_quit:
    return "state_quit";
  default:
    return "unknown_state";
  }
}

bool GameStateManager::isInGameState() const {
  return (game->currentState == Game::state_game || game->currentState == Game::state_roll_dice ||
          game->currentState == Game::state_setup_players ||
          game->currentState == Game::state_board_animation ||
          game->currentState == Game::state_lets_begin);
}

bool GameStateManager::isInMenuState() const {
  return (game->currentState == Game::state_menu || game->currentState == Game::state_intro_shader);
}

bool GameStateManager::canTransitionTo(int targetState) const {
  return validateStateTransition(targetState);
}

// Audio management for states
void GameStateManager::playStateMusic(int state) {
  handleStateAudio(state);
}

void GameStateManager::stopStateMusic(int state) {
  if (state == Game::state_menu || state == Game::state_intro_shader) {
    stopMenuMusic();
  } else if (isInGameState()) {
    stopGameMusic();
  }
}

void GameStateManager::handleStateAudio(int newState) {
  if (newState == Game::state_menu || newState == Game::state_intro_shader) {
    startMenuMusic();
  } else if (newState == Game::state_setup_players || newState == Game::state_board_animation ||
             newState == Game::state_lets_begin || newState == Game::state_game ||
             newState == Game::state_roll_dice) {
    startGameMusic();
  } else if (newState == Game::state_end_game) {
    stopGameMusic();
  }
}

// Private helper methods

void GameStateManager::prepareMenuState() {
  // Additional menu preparation logic can go here
}

void GameStateManager::prepareGameState() {
  // Additional game state preparation logic can go here
}

void GameStateManager::prepareSetupState() {
  // Additional setup state preparation logic can go here
}

void GameStateManager::prepareEndGameState() {
  // Additional end game state preparation logic can go here
}

// Audio state management (extracted from game.cpp)
void GameStateManager::startMenuMusic() {
  game->musicMenu.play();
  game->musicMenu.setLooping(true);
}

void GameStateManager::stopMenuMusic() {
  game->musicMenu.stop();
}

void GameStateManager::startGameMusic() {
  game->musicGame.play();
  game->musicGame.setLooping(true);
}

void GameStateManager::stopGameMusic() {
  game->musicGame.stop();
}

// State validation
bool GameStateManager::isValidState(int state) const {
  return (state >= Game::state_init && state <= Game::state_quit);
}

void GameStateManager::logStateTransition(int from, int to) const {
  // Optional: Log state transitions for debugging
  // std::cout << "State transition: " << from << " -> " << to << std::endl;
}

} // namespace DP