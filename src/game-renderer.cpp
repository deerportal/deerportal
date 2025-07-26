#include "game-renderer.h"

#include <algorithm>
#include <cmath> // For sin() function
#include <stdexcept>

#include "board-initialization-animator.h"
#include "game.h"

namespace DP {

/*!
 * \brief The ResultTable struct contains data for the end of the game screen.
 */
struct ResultTable {
  int playerNumber;
  int playerResult;
  bool reachedPortal;
  bool reachedPortalFirst;
  ResultTable(int number, int result, bool portal, bool portalFirst)
      : playerNumber(number), playerResult(result), reachedPortal(portal),
        reachedPortalFirst(portalFirst) {}
  bool operator<(const ResultTable& otherResult) const {
    if ((playerResult != otherResult.playerResult) ||
        ((reachedPortal == false) && (otherResult.reachedPortal == false)))
      return (playerResult > otherResult.playerResult);
    else if (reachedPortalFirst == true) {
      return true;

    } else {
      return false;
    }

    ;
  }
};

GameRenderer::GameRenderer(Game* gameInstance)
    : game(gameInstance), runningCounter(0.0f), fpsDisplayUpdateTimer(0.0f), v1(0.0f),
      enableShaders(false) // Performance: Disable shaders by default
      ,
      useDirectRendering(true) // Performance: Skip render-to-texture for gameplay
{}

GameRenderer::~GameRenderer() {}

void GameRenderer::render(float deltaTime) {
  std::cout << "RENDER ENTRY: GameRenderer::render called, state=" << game->currentState
            << ", useDirectRendering=" << useDirectRendering << std::endl;
  clearBuffers();

  // PERFORMANCE OPTIMIZATION: Direct rendering for gameplay states
  if (useDirectRendering &&
      (game->currentState == Game::state_game || game->currentState == Game::state_roll_dice)) {
#ifndef NDEBUG
    std::cout << "DEBUG: Using direct rendering path" << std::endl;
#endif
    renderDirectToWindow(deltaTime);
    return;
  }

#ifndef NDEBUG
  std::cout << "DEBUG: Using render-to-texture path, state: " << game->currentState << std::endl;
#endif

  // Route to state-specific rendering (with post-processing)
  std::cout << "RENDER DEBUG: Current state is " << game->currentState;
  switch (game->currentState) {
  case Game::state_game:
  case Game::state_roll_dice:
    std::cout << " (state_game/roll_dice)" << std::endl;
    renderStateGame();
    break;

  case Game::state_setup_players:
    std::cout << " (state_setup_players)" << std::endl;
    renderStateSetup();
    break;

  case Game::state_board_animation:
    std::cout << " (state_board_animation) - CALLING renderStateBoardAnimation!" << std::endl;
    renderStateBoardAnimation();
    break;

  case Game::state_gui_elem:
    renderStateGuiElem();
    break;

  case Game::state_menu:
    renderStateMenu();
    break;

  case Game::state_lets_begin:
    renderStateLetsBegin();
    break;

  case Game::state_gui_end_round:
    renderStateGuiEndRound();
    break;

  case Game::state_end_game:
    renderStateEndGame();
    break;

  default:
    break;
  }

  // Common UI elements
  if (game->banner.active) game->renderTexture.draw(game->banner);

  drawUIElements();
  finalizeFrame(deltaTime);
}

void GameRenderer::clearBuffers() {
  game->window.clear();
  game->renderTexture.clear();
}

void GameRenderer::renderStateGame() {
  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(*game->spriteBackgroundDark);

  game->renderTexture.setView(game->viewTiles);
  drawBaseGame();

  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(game->groupHud);

  game->renderTexture.setView(game->viewTiles);
  game->renderTexture.draw(game->boardDiamonds);
  drawCharacters();
  game->renderTexture.draw(game->bubble);

  // Draw particles LAST so they appear on top
#ifndef NDEBUG
  std::cout << "DEBUG: About to draw particles in render-to-texture path" << std::endl;
#endif
  game->getAnimationSystem()->drawCircleParticles(game->renderTexture);

  game->renderTexture.setView(game->viewFull);
  drawPlayersGui();
  game->renderTexture.setView(game->viewFull);

  // Draw Big Diamond ONLY when game board is active
  if (game->bigDiamondActive) game->renderTexture.draw(*game->spriteBigDiamond);
}

void GameRenderer::renderStateSetup() {
  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(*game->spriteDeerGod);

  for (int i = 0; i < 4; i++) {
    game->renderTexture.draw(*game->players[i].spriteAI);
  }
}

void GameRenderer::renderStateBoardAnimation() {
  // Debug output to confirm we're in animation state
  std::cout << "ANIMATION STATE: Rendering board animation" << std::endl;

  // Initialize lighting manager if needed
  static bool lightingInitialized = false;
  static int lastAnimationId = -1;
  
  // Check if we need to initialize lighting for this animation
  bool animationActive = game->boardAnimator && !game->boardAnimator->isComplete();
  
  if (animationActive && !lightingInitialized) {
    sf::Vector2u windowSize = game->renderTexture.getSize();
    if (lightingManager.initialize(windowSize)) {
      lightingInitialized = true;
      std::cout << "LIGHTING: Initialized lighting system for board animation" << std::endl;
    } else {
      std::cout << "LIGHTING: Failed to initialize lighting system" << std::endl;
    }
  }
  
  // Reset lighting when animation completes
  if (!animationActive && lightingInitialized) {
    std::cout << "LIGHTING: Animation completed, resetting lighting system" << std::endl;
    lightingInitialized = false;
  }

  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(*game->spriteBackgroundDark);

  game->renderTexture.setView(game->viewTiles);
  drawBaseGame(); // Draw board elements but NOT static diamonds

  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(game->groupHud);

  game->renderTexture.setView(game->viewTiles);
  // NOTE: We do NOT draw static boardDiamonds here - only animated ones

  // The moving diamonds are rendered LAST to ensure they appear on top
  game->boardAnimator->render(game->renderTexture, game->textures.textureBoardDiamond);

  // Apply lighting effects if animation is active
  if (lightingInitialized && animationActive) {
    std::cout << "LIGHTING: Applying lighting effects to board animation" << std::endl;
    
    // Begin lighting frame with dark ambient color
    lightingManager.beginFrame(sf::Color(20, 20, 30, 255)); // Dark ambient lighting
    
    // Update lights from animated diamonds
    game->boardAnimator->updateLights(lightingManager);
    
    // Render lighting effects
    lightingManager.render(game->renderTexture);
    
    std::cout << "LIGHTING: Lighting effects applied successfully" << std::endl;
  } else {
    std::cout << "LIGHTING: Skipping lighting - initialized=" << lightingInitialized 
              << ", animationActive=" << animationActive << std::endl;
  }

  // Other effects and characters join the fray
  game->getAnimationSystem()->drawCircleParticles(game->renderTexture);

  // Characters and UI elements should NOT cover diamonds during animation
  // drawCharacters();
  // game->renderTexture.draw(game->bubble);
}

void GameRenderer::renderStateGuiElem() {
  game->renderTexture.setView(game->viewFull);
  game->shaderBlur.setUniform("blur_radius", 2.0f);
  game->renderTexture.draw(*game->spriteBackgroundDark, &game->shaderBlur);

  drawBaseGame();
  drawCharacters();

  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(game->groupHud);
}

void GameRenderer::renderStateMenu() {
  game->renderTexture.draw(*game->menuBackground);
  game->renderTexture.draw(*game->paganHolidayTxt);
  game->renderTexture.draw(game->credits);
}

void GameRenderer::renderStateLetsBegin() {
  game->renderTexture.setView(game->viewFull);
  game->shaderBlur.setUniform("blur_radius", 4.0f);
  game->renderTexture.draw(*game->spriteBackgroundDark, &game->shaderBlur);

  game->renderTexture.setView(game->viewTiles);
  drawBaseGame();
  game->renderTexture.draw(game->boardDiamonds, &game->shaderBlur);
  game->getAnimationSystem()->drawCircleParticles(game->renderTexture);
  drawCharacters();

  game->renderTexture.setView(game->viewFull);
  drawPlayersGui();
  game->renderTexture.draw(*game->spriteLestBegin, &game->shaderBlur);
}

void GameRenderer::renderStateGuiEndRound() {
  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(*game->spriteBackgroundDark);

  drawBaseGame();

  game->shaderBlur.setUniform("blur_radius", 0.05f);
  game->renderTexture.draw(game->guiRoundDice, &game->shaderBlur);

  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(game->groupHud);
}

void GameRenderer::renderStateEndGame() {
  game->renderTexture.setView(game->viewFull);
  game->renderTexture.draw(*game->spriteBackgroundDark);
  game->renderTexture.draw(*game->spriteLestBegin, &game->shaderBlur);
  game->renderTexture.draw(*game->endGameTxt);

  game->renderTexture.draw(*game->txtWinner);
  game->renderTexture.draw(*game->txtSurvivorsLabel);

  for (unsigned int i = 0; i < game->txtSurvivors.size(); i++) {
    game->renderTexture.draw(*game->txtSurvivors[i]);
  }

  game->renderTexture.draw(*game->txtLoosersLabel);

  for (unsigned int i = 0; i < game->txtLoosers.size(); i++) {
    game->renderTexture.draw(*game->txtLoosers[i]);
  }
}

void GameRenderer::drawPlayersGui() {
  for (int i = 0; i < 4; i++) {
    game->renderTexture.draw(game->players[i]);
  }
}

void GameRenderer::drawSquares() {
  if (game->showPlayerBoardElems) {
    game->renderTexture.draw(game->selector);
  }
}

void GameRenderer::drawBaseGame() {
  game->renderTexture.setView(game->viewTiles);

  for (int i = 0; i < 4; i++) {
    game->renderTexture.draw(game->players[i].elems);
  }

  drawSquares();
  game->renderTexture.setView(game->viewGui);
  game->renderTexture.setView(game->viewTiles);
  game->renderTexture.setView(game->viewTiles); // Yeah Katia's inspiration

  updateShaderParameters();

  game->renderTexture.setView(game->viewFull);
  game->shaderBlur.setUniform("blur_radius", 0.01f);

  float v = sin(game->runningCounter * 0.005f);
  game->shaderPixel.setUniform("pixel_threshold", v);

  game->renderTexture.draw(*game->spriteBackgroundArt);
  game->spriteBackgroundArt->setColor(sf::Color(255, 255, 255, 255)); // Full opacity

  v = sin(game->runningCounter * 0.05f) / 2;
  game->shaderBlur.setUniform("blur_radius", v);

  game->renderTexture.draw(game->cardsDeck);

  if (game->currentState == Game::state_roll_dice) {
    game->spriteBackgroundArt->setColor(sf::Color(255, 255, 255));

    // Simply draw the dice - setFaces(6) sets it to the distinctive 45-degree waiting sprite
    // (position 6)
    game->renderTexture.draw(*game->roundDice.spriteDice);
  } else {
    // Normal dice rendering (shows result)
    game->renderTexture.draw(*game->roundDice.spriteDice);
  }

  game->renderTexture.setView(game->viewTiles);
  drawSquares();
}

void GameRenderer::drawCharacters() {
  if (game->currentState == Game::state_game) {
    std::array<int, 2> currentMovements =
        game->players[game->turn].characters[0].getMovements(game->diceResultPlayer);

    if (currentMovements[1] > -1) {
      if (game->nextRotateElem.active) game->renderTexture.draw(game->nextRotateElem);
    }

    if (currentMovements[0] > -1) {
      if (game->prevRotateElem.active) game->renderTexture.draw(game->prevRotateElem);
    }
  }

  game->renderTexture.setView(game->viewFull);
  game->shaderBlur.setUniform("blur_radius", 0.005f);

  for (int i = 0; i < 4; i++) {
    for (auto&& j : game->players[i].characters) {
      if (game->currentState == Game::state_game)
        j.drawMovements = true;
      else
        j.drawMovements = false;

      game->renderTexture.draw(j);
    }
  }
}

void GameRenderer::setTxtEndGameAmount() {
  std::string elementNames[4] = {"Water", "Earth", "Fire", "Air"};
  int width = 1360;
  int separator = 40;

  // Create result table for sorting
  std::array<ResultTable, 4> results = {
      {ResultTable(0, game->players[0].cash, game->players[0].reachedPortal,
                   game->players[0].reachedPortalFirst),
       ResultTable(1, game->players[1].cash, game->players[1].reachedPortal,
                   game->players[1].reachedPortalFirst),
       ResultTable(2, game->players[2].cash, game->players[2].reachedPortal,
                   game->players[2].reachedPortalFirst),
       ResultTable(3, game->players[3].cash, game->players[3].reachedPortal,
                   game->players[3].reachedPortalFirst)}};

  std::vector<ResultTable> resultsVector;
  game->txtSurvivors.clear();
  game->txtLoosers.clear();

  for (int i = 0; i < 4; i++) {
    resultsVector.push_back(results[i]);
  }

  std::sort(resultsVector.begin(), resultsVector.end());
  game->txtWinner->setFont(game->gameFont);
  game->txtWinner->setCharacterSize(40);

  for (int i = 0; i < 4; i++) {
    int playerNumber = resultsVector[i].playerNumber;
    std::string elementName = elementNames[playerNumber];
    sf::Text tmpText(game->gameFont);
    tmpText.setCharacterSize(25);
    tmpText.setString(elementName + " " + std::to_string(game->players[playerNumber].cash));
    sf::FloatRect rectTxt = tmpText.getLocalBounds();

    if (game->players[playerNumber].reachedPortal == true) {
      int counter = game->txtSurvivors.size();
      tmpText.setPosition(
          sf::Vector2f((1360 / 2) - (rectTxt.size.x / 2), 200 + (counter * separator)));
      game->txtSurvivors.push_back(std::make_unique<sf::Text>(tmpText));
    } else {
      int counter = game->txtLoosers.size();
      tmpText.setPosition(
          sf::Vector2f((width / 2) - (rectTxt.size.x / 2), 540 + (counter * separator)));
      game->txtLoosers.push_back(std::make_unique<sf::Text>(tmpText));
    }
  }

  if (game->txtSurvivors.size() > 0) {
    game->txtWinner->setString("Winner: " + game->txtSurvivors[0]->getString());
    game->txtSurvivors.erase(game->txtSurvivors.begin() + 0);
  }

  game->txtWinner->setCharacterSize(40);
  sf::FloatRect rectTxt = game->txtWinner->getLocalBounds();
  game->txtWinner->setPosition(sf::Vector2f((1360 / 2) - (rectTxt.size.x / 2), 120));
}

void GameRenderer::updateFPSDisplay(float deltaTime) {
  fpsDisplayUpdateTimer += deltaTime;
  if (fpsDisplayUpdateTimer >= 0.25f) // Update FPS display every 0.25 seconds
  {
    float fps = 1.0f / deltaTime;
    game->textFPS->setString("FPS: " + std::to_string(static_cast<int>(fps)));
    fpsDisplayUpdateTimer = 0.0f;
  }
}

void GameRenderer::updateShaderParameters() {
  float v = sin(game->runningCounter * 0.01f);
  game->shaderBlur.setUniform("blur_radius", v);
}

void GameRenderer::drawUIElements() {
  game->renderTexture.setView(game->viewFull); // Ensure GUI view for correct positioning

  // Show FPS counter if DEERPORTAL_SHOW_FPS_COUNTER is defined (via CMake option)
  // OR if it's a Debug build (NDEBUG is not defined)
#if defined(DEERPORTAL_SHOW_FPS_COUNTER) || !defined(NDEBUG)
  game->renderTexture.draw(*game->textFPS);
#endif

  // Show version info ONLY in Debug builds (NDEBUG is not defined)
#ifndef NDEBUG
  game->renderTexture.draw(*game->gameVersion);
#endif
}

void GameRenderer::finalizeFrame(float deltaTime) {
  game->renderTexture.display();
  game->renderSprite->setTexture(game->renderTexture.getTexture());

  v1 = sin(deltaTime) * 0.015f;
  game->shaderBlur.setUniform("blur_radius", v1);
  game->shaderBlur.setUniform("blur_radius", 0.0003f);
  game->window.draw(*game->renderSprite, &game->shaderBlur);

  game->window.display();
}

void GameRenderer::setView(int viewType) {
  // Utility method to set different views
  switch (viewType) {
  case 0: // viewFull
    game->renderTexture.setView(game->viewFull);
    break;
  case 1: // viewTiles
    game->renderTexture.setView(game->viewTiles);
    break;
  case 2: // viewGui
    game->renderTexture.setView(game->viewGui);
    break;
  }
}

void GameRenderer::applyShader(int shaderType, float intensity) {
  switch (shaderType) {
  case 0: // Blur
    applyBlurEffect(intensity);
    break;
  case 1: // Pixel
    applyPixelEffect(intensity);
    break;
  }
}

void GameRenderer::applyBlurEffect(float intensity) {
  game->shaderBlur.setUniform("blur_radius", intensity);
}

void GameRenderer::applyPixelEffect(float threshold) {
  game->shaderPixel.setUniform("pixel_threshold", threshold);
}

void GameRenderer::setupShaders() {
  // Initialize shader parameters if needed
  // This method can be expanded for shader setup
}

void GameRenderer::renderDirectToWindow(float deltaTime) {
  // PERFORMANCE: Direct rendering to window, skipping render-to-texture pipeline
  game->window.clear();

  // Set appropriate views and draw directly
  game->window.setView(game->viewFull);
  game->window.draw(*game->spriteBackgroundDark);

  game->window.setView(game->viewTiles);
  drawBaseGameDirect();

  game->window.setView(game->viewFull);
  game->window.draw(game->groupHud);

  game->window.setView(game->viewTiles);
  drawCharactersDirect();
  game->window.draw(game->boardDiamonds);
  game->window.draw(game->bubble);

  game->window.setView(game->viewFull);
  drawPlayersGuiDirect();

  // Draw Big Diamond ONLY when active (no post-processing)
  if (game->bigDiamondActive) game->window.draw(*game->spriteBigDiamond);

  // Draw UI elements directly
  drawUIElementsDirect();
  // Draw particles LAST so they appear on top of everything
#ifndef NDEBUG
  std::cout << "DEBUG: About to draw particles in direct rendering path" << std::endl;
#endif
  game->getAnimationSystem()->drawCircleParticles(game->window);
  game->window.display();
}

void GameRenderer::drawBaseGameDirect() {
  // Direct drawing without render-to-texture overhead
  for (int i = 0; i < 4; i++) {
    game->window.draw(game->players[i].elems);
  }

  if (game->showPlayerBoardElems) {
    game->window.draw(game->selector);
  }

  // Background without excessive shaders
  game->window.draw(*game->spriteBackgroundArt);

  game->window.draw(game->cardsDeck);

  // IMPORTANT: Preserve dice waiting state visual effects from v0.8.2
  if (game->currentState == Game::state_roll_dice) {
    // Simply draw the dice - setFaces(6) sets it to the distinctive 45-degree waiting sprite
    // (position 6)
    game->window.draw(*game->roundDice.spriteDice);
  } else {
    // Normal dice rendering (shows result)
    game->window.draw(*game->roundDice.spriteDice);
  }
}

void GameRenderer::drawCharactersDirect() {
  if (game->currentState == Game::state_game) {
    std::array<int, 2> currentMovements =
        game->players[game->turn].characters[0].getMovements(game->diceResultPlayer);

    if (currentMovements[1] > -1) {
      if (game->nextRotateElem.active) game->window.draw(game->nextRotateElem);
    }

    if (currentMovements[0] > -1) {
      if (game->prevRotateElem.active) game->window.draw(game->prevRotateElem);
    }
  }

  for (int i = 0; i < 4; i++) {
    for (auto&& j : game->players[i].characters) {
      if (game->currentState == Game::state_game)
        j.drawMovements = true;
      else
        j.drawMovements = false;

      game->window.draw(j);
    }
  }
}

void GameRenderer::drawPlayersGuiDirect() {
  for (int i = 0; i < 4; i++) {
    game->window.draw(game->players[i]);
  }
}

void GameRenderer::drawUIElementsDirect() {
  // Show FPS counter if enabled
#if defined(DEERPORTAL_SHOW_FPS_COUNTER) || !defined(NDEBUG)
  game->window.draw(*game->textFPS);
#endif

  // Show version info ONLY in Debug builds
#ifndef NDEBUG
  game->window.draw(*game->gameVersion);
#endif

  if (game->banner.active) game->window.draw(game->banner);
}

void GameRenderer::conditionalShaderRender(sf::Drawable& drawable, sf::Shader* shader) {
  // PERFORMANCE: Only use shaders when explicitly enabled and beneficial
  if (enableShaders && shader) {
    game->renderTexture.draw(drawable, shader);
  } else {
    game->renderTexture.draw(drawable); // Direct rendering without shader overhead
  }
}

} // namespace DP