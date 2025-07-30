#include "game-input.h"

#include "board-initialization-animator.h"
#include "game.h"
#include "tilemap.h" // For TILE_SIZE, BOARD_SIZE, transCords

namespace DP {

GameInput::GameInput(Game* gameInstance)
    : game(gameInstance), mouseOverBoard(false), currentInputMode(0) {}

GameInput::~GameInput() {}

void GameInput::processEvents(sf::RenderWindow& window) {
  while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
    const sf::Event& event = *eventOpt;

    if (event.is<sf::Event::Closed>()) {
      window.close();
      return;
    }

    handleKeyboardInput(event);
    handleMouseInput(event);
  }

  // Update mouse position and selector continuously (not just on events)
  sf::Vector2i localPositionTmp = sf::Mouse::getPosition(window);
  sf::Vector2f localPosition = window.mapPixelToCoords(localPositionTmp, game->viewTiles);
  updateSelector(localPosition);
}

void GameInput::handleKeyboardInput(const sf::Event& event) {
  if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
    // Handle board animation skip functionality
    if (game->currentState == Game::state_board_animation) {
      if (keyPressed->code == sf::Keyboard::Key::Space ||
          keyPressed->code == sf::Keyboard::Key::Enter) {
        game->boardAnimator->skipAnimation();
        game->stateManager->transitionFromBoardAnimationToLetsBegin();

#ifndef NDEBUG
        sf::Vector2f mousePos = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window));
        std::cout << "[DEBUG] Animation skipped via key press in state: "
                  << game->stateManager->getCurrentStateName() << std::endl;
#endif
        return;
      }
    }

    if (keyPressed->code == sf::Keyboard::Key::Escape) {
      // Context-aware Escape behavior
      if (game->currentState == Game::state_menu) {
        // If in menu, exit the game
        game->window.close();
      } else {
        // If in game, go back to menu
        game->stateManager->showMenu();
      }
    }

    // Fullscreen toggle - multiple key combinations for better UX
    bool shouldToggleFullscreen = false;

    // F key (simple)
    if (keyPressed->code == sf::Keyboard::Key::F) {
      shouldToggleFullscreen = true;
    }

    // F11 key (standard fullscreen key)
    if (keyPressed->code == sf::Keyboard::Key::F11) {
      shouldToggleFullscreen = true;
    }

    // Cmd+Enter (macOS pattern) - check if Cmd key is held
    if (keyPressed->code == sf::Keyboard::Key::Enter &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem)) {
      shouldToggleFullscreen = true;
    }

    if (shouldToggleFullscreen) {
      bool success = game->toggleFullscreen();
#ifndef NDEBUG
      if (success) {
        std::cout << "Fullscreen toggled successfully" << std::endl;
      } else {
        std::cout << "Fullscreen toggle failed" << std::endl;
      }
#endif
    }

    // Additional keyboard handling can be added here
  }
}

void GameInput::handleMouseInput(const sf::Event& event) {
  // Get mouse position and convert to different coordinate systems
  sf::Vector2i localPositionTmp = sf::Mouse::getPosition(game->window);
  sf::Vector2f localPosition = game->window.mapPixelToCoords(localPositionTmp, game->viewTiles);
  sf::Vector2f localPositionFull = game->window.mapPixelToCoords(localPositionTmp, game->viewFull);

  int mousePosX = (int)localPosition.x / DP::TILE_SIZE;
  int mousePosY = (int)localPosition.y / DP::TILE_SIZE;
  int mousePos = DP::transCords(sf::Vector2i(mousePosX, mousePosY));

  // Handle mouse click events
  if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
#ifndef NDEBUG
      std::cout << "[DEBUG] Mouse click at (" << localPositionFull.x << ", " << localPositionFull.y
                << ") in state: " << game->stateManager->getCurrentStateName() << std::endl;
#endif
      handleLeftClick(localPosition, localPositionFull, mousePos);
    }
  }
}

void GameInput::updateSelector(sf::Vector2f localPosition) {
  // Update mouse hover state for game board
  if (game->currentState == Game::state_game) {
    if ((localPosition.x > DP::TILE_SIZE * DP::BOARD_SIZE) || (localPosition.x < 0) ||
        (localPosition.y > DP::TILE_SIZE * DP::BOARD_SIZE) || (localPosition.y < 0)) {
      game->showPlayerBoardElems = false;
    } else {
      game->showPlayerBoardElems = true;
    }
  }

  // Update selector position if within valid board bounds
  if ((localPosition.x >= 0) && (localPosition.y >= 0) &&
      (localPosition.x <= DP::BOARD_SIZE * DP::TILE_SIZE) &&
      (localPosition.y <= DP::BOARD_SIZE * DP::TILE_SIZE)) {
    game->selector.setPosition(
        sf::Vector2f((int)(localPosition.x / DP::TILE_SIZE) * DP::TILE_SIZE,
                     (int)(localPosition.y / DP::TILE_SIZE) * DP::TILE_SIZE));
  }
}

void GameInput::handleLeftClick(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  std::cout << "CLICK DEBUG: Left click at (" << posFull.x << ", " << posFull.y
            << ") in state: " << game->currentState << std::endl;

  switch (game->currentState) {
  case Game::state_game:
    processGameInput(pos, posFull, mousePos);
    break;

  case Game::state_setup_players:
    processSetupInput(pos, posFull, mousePos);
    break;

  case Game::state_roll_dice:
    processRollDiceInput(pos, posFull, mousePos);
    break;

  case Game::state_menu:
    processMenuInput(pos, posFull, mousePos);
    break;

  case Game::state_gui_end_round:
    processGuiEndRoundInput(pos, posFull, mousePos);
    break;

  case Game::state_lets_begin:
    processLetsBeginInput(pos, posFull, mousePos);
    break;

  case Game::state_end_game:
    processEndGameInput(pos, posFull, mousePos);
    break;

  case Game::state_board_animation:
    // Skip animation and immediately start game on any click
    if (!game->boardAnimator->isComplete()) {
#ifndef NDEBUG
      std::cout << "INPUT: User clicked to skip animation and start game immediately" << std::endl;
#endif
      // Skip animation and force all diamonds to final positions
      game->boardAnimator->skipAnimation();
      // Release diamonds immediately to transition to static rendering
      game->boardAnimator->releaseDiamonds();
      // Clean up lighting
      if (game->lightingManager && game->boardAnimationLightingInitialized) {
        game->lightingManager->cleanup();
        game->boardAnimationLightingInitialized = false;
      }
      // Transition directly to roll_dice to start game
      game->currentState = Game::state_roll_dice;
      game->launchNextPlayer();
    } else {
      // Animation already complete, proceed normally
#ifndef NDEBUG
      std::cout << "INPUT: Animation complete, user clicked to proceed to lets_begin" << std::endl;
#endif
      game->stateManager->transitionFromBoardAnimationToLetsBegin();
    }
    break;

  case Game::state_intro_shader:
    // Skip intro and go to main menu on mouse click
    game->stateManager->showMenu();
    break;

  default:
    // Handle other states if needed
    break;
  }
}

void GameInput::processGameInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  if (game->players[game->turn].human) {
    std::array<int, 2> movements = game->players[game->turn].getMovements(game->diceResultPlayer);
    if ((mousePos == movements[0]) || (mousePos == movements[1])) {
      game->playerMakeMove(mousePos);
    }
  }
}

void GameInput::processSetupInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  std::cout << "SETUP INPUT: processSetupInput called with click at (" << posFull.x << ", "
            << posFull.y << ")" << std::endl;

  // Check for clicks on player AI/Human toggle buttons
  for (int i = 0; i < 4; i++) {
    sf::FloatRect spriteHumanRect(game->players[i].spriteAI->getGlobalBounds());
    if (spriteHumanRect.contains(posFull)) {
      game->players[i].swapHuman();
    }
  }

  // Check for start game button click
  sf::IntRect startGameRect({580, 640}, {180, 80});
  std::cout << "BUTTON DEBUG: Click at (" << posFull.x << ", " << posFull.y
            << ") vs button rect (580,640) to (760,720)" << std::endl;
  std::cout << "BUTTON DEBUG: Contains check result: "
            << startGameRect.contains(sf::Vector2i((int)posFull.x, (int)posFull.y)) << std::endl;

  if (startGameRect.contains(sf::Vector2i((int)posFull.x, (int)posFull.y))) {
    game->bigDiamondActive = true;
    // Don't set banner text during animation to avoid covering the animation
    // game->banner.setText("start game");

#ifndef NDEBUG
    std::cout << "[DEBUG] Start game button clicked, initializing animation" << std::endl;
#endif

    // Initialize animation with final diamond positions
    game->boardAnimator->initializeAnimation(game->boardDiamonds, game->window);

    // Transition to animation state instead of going directly to roll_dice
    game->stateManager->transitionToBoardAnimation();

#ifndef NDEBUG
    std::cout << "[DEBUG] Transitioned to board animation state" << std::endl;
#endif
  }
}

void GameInput::processRollDiceInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  if (game->players[game->turn].human) {
    sf::FloatRect diceRect(game->roundDice.spriteDice->getGlobalBounds());
    if (diceRect.contains(posFull)) {
      // Only allow dice throwing if notification delay has expired
      if (game->cardNotificationDelay <= 0) {
        game->throwDiceMove();
      }
    }
  }
}

void GameInput::processMenuInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  game->downTimeCounter = 0;
  game->stateManager->hideMenu();
  game->stateManager->showGameBoard();
}

void GameInput::processGuiEndRoundInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  std::string resultCommand = game->guiRoundDice.getElem(pos);
  game->command(resultCommand);
}

void GameInput::processLetsBeginInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  if (game->downTimeCounter > 1) {
    // Release animated diamonds before transitioning to roll_dice
    if (game->boardAnimator) {
      game->boardAnimator->releaseDiamonds();
#ifndef NDEBUG
      std::cout << "INPUT: Released animated diamonds on user click in state_lets_begin" << std::endl;
#endif
    }
    
    game->currentState = Game::state_roll_dice;
    game->restartGame();
    game->launchNextPlayer();
  }
}

void GameInput::processEndGameInput(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  if (game->downTimeCounter > 2) {
    game->currentState = Game::state_menu;
    game->musicMenu.play();
    game->restartGame();
  }
}

sf::Vector2f GameInput::getMousePosition(sf::RenderWindow& window) const {
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  return window.mapPixelToCoords(mousePos, game->viewFull);
}

sf::Vector2f GameInput::mapToTileCoords(sf::Vector2f screenPos, sf::RenderWindow& window) {
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  return window.mapPixelToCoords(mousePos, game->viewTiles);
}

sf::Vector2f GameInput::mapToGuiCoords(sf::Vector2f screenPos, sf::RenderWindow& window) {
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  return window.mapPixelToCoords(mousePos, game->viewGui);
}

int GameInput::convertToTileIndex(sf::Vector2f tileCoords) {
  int mousePosX = (int)tileCoords.x / DP::TILE_SIZE;
  int mousePosY = (int)tileCoords.y / DP::TILE_SIZE;
  return DP::transCords(sf::Vector2i(mousePosX, mousePosY));
}

bool GameInput::isInputAllowed() const {
  // Add logic to determine if input should be processed based on current game state
  return true; // For now, always allow input
}

void GameInput::setInputMode(int gameState) {
  currentInputMode = gameState;
}

void GameInput::validateMousePosition(sf::Vector2f position) {
  // Add validation logic if needed
  lastMousePosition = position;
}

} // namespace DP