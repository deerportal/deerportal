#include "game.h"

#include "board-initialization-animator.h"
#include "error-handler.h"
#include "game-assets.h"
#include "game-core.h"
#include "game-input.h"
#include "game-renderer.h"
#include "safe-asset-loader.h"

// Include all headers that were moved from game.h
#include <algorithm>
#include <stdexcept>

#include "animatedsprite.h"
#include "banner.h"
#include "boarddiamondseq.h"
#include "bubble.h"
#include "calendar.h"
#include "cardnotification.h"
#include "cardsdeck.h"
#include "character.h"
#include "command.h"
#include "credits.h"
#include "grouphud.h"
#include "guirounddice.h"
#include "guiwindow.h"
#include "hover.h"
#include "introshader.h"
#include "playerhud.h"
#include "rotateelem.h"
#include "rounddice.h"
#include "selector.h"
#include "soundfx.h"
#include "textureholder.h"
#include "tilemap.h"

namespace DP {

int initScreenX = 1360;
int initScreenY = 768;
int currentSeason = 1;
int month = 0;

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
/*!
 * \brief Game::setTxtEndGameAmount orders the results text for the end of the game.
 */
void Game::setTxtEndGameAmount() {
  std::string elementNames[4] = {"Water", "Earth", "Fire", "Air"};

  int width = 1360;
  //    int height = 768;
  //    int startHeight = 100;
  int separator = 40;
  std::array<ResultTable, 4> results = {
      {ResultTable(0, players[0].cash, players[0].reachedPortal, players[0].reachedPortalFirst),
       ResultTable(1, players[1].cash, players[1].reachedPortal, players[1].reachedPortalFirst),
       ResultTable(2, players[2].cash, players[2].reachedPortal, players[2].reachedPortalFirst),
       ResultTable(3, players[3].cash, players[3].reachedPortal, players[3].reachedPortalFirst)}};

  std::vector<ResultTable> resultsVector;
  txtSurvivors.clear();
  txtLoosers.clear();

  for (int i = 0; i < 4; i++) {
    resultsVector.push_back(results[i]);
  };

  std::sort(resultsVector.begin(), resultsVector.end());
  txtWinner->setFont(gameFont);
  txtWinner->setCharacterSize(40);
  for (int i = 0; i < 4; i++) {

    int playerNumber = resultsVector[i].playerNumber;
    std::string elementName = elementNames[playerNumber];
    sf::Text tmpText(gameFont);
    tmpText.setCharacterSize(25);
    tmpText.setString(elementName + " " + std::to_string(players[playerNumber].cash));
    sf::FloatRect rectTxt = tmpText.getLocalBounds();

    if (players[playerNumber].reachedPortal == true) {
      int counter = txtSurvivors.size();

      tmpText.setPosition(
          sf::Vector2f((1360 / 2) - (rectTxt.size.x / 2), 200 + (counter * separator)));
      txtSurvivors.push_back(std::make_unique<sf::Text>(tmpText));
    } else {
      int counter = txtLoosers.size();

      tmpText.setPosition(
          sf::Vector2f((width / 2) - (rectTxt.size.x / 2), 540 + (counter * separator)));
      txtLoosers.push_back(std::make_unique<sf::Text>(tmpText));
    }
  }
  if (txtSurvivors.size() > 0) {
    txtWinner->setString("Winner: " + txtSurvivors[0]->getString());
    txtSurvivors.erase(txtSurvivors.begin() + 0);
  }
  txtWinner->setCharacterSize(40);
  sf::FloatRect rectTxt = txtWinner->getLocalBounds();
  txtWinner->setPosition(sf::Vector2f((1360 / 2) - (rectTxt.size.x / 2), 120));
}

void Game::initBoard() {

  time_t t = time(0);
  struct tm* now = localtime(&t);
  int month = now->tm_mon + 1;
  int day = now->tm_mday;
  paganHolidayString = getHoliday(month, day);
  paganHolidayTxt->setString(paganHolidayString);

  sfxClick.setBuffer(sfxClickBuffer);
  sfxDone.setBuffer(sfxDoneBuffer);
  spriteBackgroundDark->setTexture(textures.backgroundDark);
  spriteBackgroundDark->setPosition(sf::Vector2f(0, 0));
  spriteLestBegin->setTexture(textures.textureLetsBegin);
  viewTiles.setViewport(sf::FloatRect({0.15f, 0.1f}, {1.0f, 1.0f}));
  viewGui.setViewport(sf::FloatRect({0.806f, 0.066f}, {1, 1}));

  // FULLSCREEN FIX: Position BoardDiamondSeq with same offset as other board elements
  // This ensures diamonds use the same coordinate system as characters and board elements
  boardDiamonds.setPosition(sf::Vector2f(202, 76));

  groupHud.setFont(&gameFont);
  groupHud.setSeason(currentSeason);
  groupHud.setRoundName(roundNumber);

  cardsDeck.setFonts(&gameFont);
  spriteBigDiamond->setTexture(textures.textureBigDiamond);
  spriteBigDiamond->setPosition(sf::Vector2f(474, 342)); // Original 0.8.2 position coordinates
  spriteBigDiamond->setColor(sf::Color(255, 255, 255, 196));
  restartGame();
  launchNextPlayer();

  endGameTxt->setFont(gameFont);
  endGameTxt->setString("End of the Game");
  endGameTxt->setCharacterSize(30);

  sf::FloatRect ss = endGameTxt->getLocalBounds();
  endGameTxt->setPosition(sf::Vector2f((1360 / 2) - (ss.size.x / 2), 60));

  setTxtEndGameAmount();
  bubble.setPosition(players[turn].characters[0].getPosition().x - 30,
                     players[turn].characters[0].getPosition().y - 45);

  txtSurvivorsLabel->setString("Survivors");
  txtSurvivorsLabel->setFont(gameFont);
  txtSurvivorsLabel->setCharacterSize(30);
  sf::FloatRect rectSurvivors = txtSurvivorsLabel->getLocalBounds();
  txtSurvivorsLabel->setPosition(sf::Vector2f((1360 / 2) - (rectSurvivors.size.x / 2), 200));

  txtLoosersLabel->setString("Digested by The Elements");
  txtLoosersLabel->setFont(gameFont);
  txtLoosersLabel->setCharacterSize(30);
  sf::FloatRect rectLoosers = txtLoosersLabel->getLocalBounds();
  txtLoosersLabel->setPosition(sf::Vector2f((1360 / 2) - (rectLoosers.size.x / 2), 500));
  credits.setTxt(0);
}

/*!
 * \brief Game::restartGame sets all the variables to the default value
 */
void Game::restartGame() {
  // Reset lighting states
  boardAnimationLightingInitialized = false;
  letsBeginLightingInitialized = false;
  if (lightingManager) {
    lightingManager->cleanup();
#ifndef NDEBUG
    std::cout << "LIGHTING: Reset lighting states in restartGame()" << std::endl;
#endif
  }

  Player playerHud1(&textures, &gameFont, 0);
  Player playerHud2(&textures, &gameFont, 1);
  Player playerHud3(&textures, &gameFont, 2);
  Player playerHud4(&textures, &gameFont, 3);
  players[0] = std::move(playerHud1);
  players[1] = std::move(playerHud2);
  players[3] = std::move(playerHud3);
  players[2] = std::move(playerHud4);
  players[0].setActive(true);
  setCurrentNeighbours();
  diceResultPlayer = 6;
  players[turn].characters[0].diceResult = diceResultPlayer;
  roundDice.setColor(turn);

  for (int i = 0; i < 4; i++) {
    players[i].restartPlayer();
    players[i].reachedPortal = false;

    boardDiamonds.reorder(i);
    bubble.setPosition(players[i].characters[0].getPosition().x - 30,
                       players[i].characters[0].getPosition().y - 45);
    players[i].done = false;
  }
  
  // NOTE: Do NOT release animated diamonds here - they should persist longer
  numberFinishedPlayers = 0;
  turn = 0;
  currentSeason = 1;
  roundNumber = 1;
  month = 0;
  cardsDeck.reloadCards();
  deerModeActive = false;
  deerModeCounter = 16;
  bigDiamondActive = true; // Ensure big diamond is visible on game restart
  cardNotification.dismiss();
}

void Game::setCurrentNeighbours() {
  currentNeighbours = players[turn].getNeighbours();
}

/*!
 * \brief Game::loadAssets
 */
void Game::loadAssets() {

  try {
    DeerPortal::SafeAssetLoader::loadFont(gameFont, ASSETS_PATH "fnt/metal-mania.regular.ttf");
  } catch (const DeerPortal::AssetLoadException& e) {
    DeerPortal::ErrorHandler::getInstance().handleException(e);
    if (!DeerPortal::SafeAssetLoader::loadFallbackFont(gameFont)) {
      throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::FONT,
                                           "metal-mania.regular.ttf",
                                           "Failed to load primary font and no fallback available");
    }
  }

  try {
    DeerPortal::SafeAssetLoader::loadFont(menuFont, ASSETS_PATH "fnt/metal-macabre.regular.ttf");
  } catch (const DeerPortal::AssetLoadException& e) {
    DeerPortal::ErrorHandler::getInstance().handleException(e);
    menuFont = gameFont; // Use game font as fallback
  }

  // Initialize sprites with textures for SFML 3.0
  menuBackground = std::make_unique<sf::Sprite>(textures.textureMenu);

  spriteDeerGod = std::make_unique<sf::Sprite>(textures.textureDeerGod);

  if (!shaderBlur.loadFromFile(get_full_path(ASSETS_PATH "shaders/blur.frag"),
                               sf::Shader::Type::Fragment)) {
    DeerPortal::ErrorHandler::getInstance().logError(
        DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SHADER, "blur.frag",
                                       "Failed to load blur shader - visual effects disabled"));
  }

  if (!shaderPixel.loadFromFile(get_full_path(ASSETS_PATH "shaders/pixelate.frag"),
                                sf::Shader::Type::Fragment)) {
    DeerPortal::ErrorHandler::getInstance().logError(
        DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SHADER, "pixelate.frag",
                                       "Failed to load pixelate shader - visual effects disabled"));
  }
  if (!shaderDark.loadFromFile(get_full_path(ASSETS_PATH "shaders/dark.frag"),
                               sf::Shader::Type::Fragment)) {
    DeerPortal::ErrorHandler::getInstance().logError(
        DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SHADER, "dark.frag",
                                       "Failed to load dark shader - visual effects disabled"));
  }

  try {
    DeerPortal::SafeAssetLoader::loadTexture(textureBackgroundArt,
                                             ASSETS_PATH "img/background_land.png");
  } catch (const DeerPortal::AssetLoadException& e) {
    DeerPortal::ErrorHandler::getInstance().handleException(e);
    DeerPortal::SafeAssetLoader::createFallbackTexture(textureBackgroundArt, sf::Color(0, 100, 0),
                                                       sf::Vector2u(800, 600));
  }

  try {
    DeerPortal::SafeAssetLoader::loadTexture(textureIntroMenu, ASSETS_PATH "img/dp_intro_menu.png");
  } catch (const DeerPortal::AssetLoadException& e) {
    DeerPortal::ErrorHandler::getInstance().handleException(e);
    DeerPortal::SafeAssetLoader::createFallbackTexture(textureIntroMenu, sf::Color(50, 50, 50),
                                                       sf::Vector2u(800, 600));
  }

  if (!musicGame.openFromFile(get_full_path(ASSETS_PATH "audio/game.ogg"))) {
    DeerPortal::ErrorHandler::getInstance().logError(
        DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, "game.ogg",
                                       "Failed to load game music - audio disabled"));
  }
  //    if (!musicBackground.openFromFile(ASSETS_PATH"assets/audio/wind2.ogg"))
  //        std::exit(1);
  if (!musicMenu.openFromFile(get_full_path(ASSETS_PATH "audio/menu.ogg"))) {
    DeerPortal::ErrorHandler::getInstance().logError(
        DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, "menu.ogg",
                                       "Failed to load menu music - audio disabled"));
  }

  try {
    DeerPortal::SafeAssetLoader::loadSound(sfxClickBuffer, ASSETS_PATH "audio/click.ogg");
  } catch (const DeerPortal::AssetLoadException& e) {
    DeerPortal::ErrorHandler::getInstance().handleException(e);
  }

  try {
    DeerPortal::SafeAssetLoader::loadSound(sfxDoneBuffer, ASSETS_PATH "audio/done.ogg");
  } catch (const DeerPortal::AssetLoadException& e) {
    DeerPortal::ErrorHandler::getInstance().handleException(e);
  }
  //    if (!textureBackground.loadFromFile(ASSETS_PATH"assets/img/background.png"))
  //        std::exit(1);

  // Initialize sprites that depend on loaded textures
  spriteBackgroundArt = std::make_unique<sf::Sprite>(textureBackgroundArt);
  spriteBackgroundDark = std::make_unique<sf::Sprite>(textures.backgroundDark);
  spriteLestBegin = std::make_unique<sf::Sprite>(textures.textureLetsBegin);
  spriteBigDiamond = std::make_unique<sf::Sprite>(textures.textureBigDiamond);

  menuTxt->setFont(gameFont);
  menuTxt->setCharacterSize(60);
  menuTxt->setString(gameTitle);
  int width = menuTxt->getLocalBounds().size.x;
  int height = menuTxt->getLocalBounds().size.y;
  menuTxt->setPosition(sf::Vector2f(1050 - (width / 2), 750 - (height / 2) - 150));
  menuTxt->setFillColor(sf::Color(255, 255, 255, 85));
  cardsDeck.setFonts(&gameFont);

  paganHolidayTxt->setFont(gameFont);
  paganHolidayTxt->setCharacterSize(20);
  paganHolidayTxt->setPosition(sf::Vector2f(20, 20));

  // Setup FPS display text (NEW 0.8.2 FEATURE)
  textFPS->setFont(gameFont);
  textFPS->setCharacterSize(20);
  textFPS->setPosition(sf::Vector2f(0, 60));
  textFPS->setFillColor(sf::Color::Yellow);
  textFPS->setString("FPS: --");

  // Load and set window icon (NEW 0.8.2 FEATURE)
  sf::Image icon;
  if (icon.loadFromFile(get_full_path(ASSETS_PATH "img/deerportal.png"))) {
    window.setIcon(icon.getSize(), icon.getPixelsPtr());
  }

  for (int i = 0; i < 4; i++) {
    // Initialize player sprites with texture
    playersSprites[i] = std::make_unique<sf::Sprite>(textureBackgroundArt);
    seasons[i] = std::make_unique<sf::Sprite>(textureBackgroundArt); // Use appropriate texture

    playersSprites[i]->setTextureRect(
        sf::IntRect({playersSpritesCords[i][0], playersSpritesCords[i][1]}, {280, 280}));
    playersSprites[i]->setPosition(
        sf::Vector2f(playersSpritesCords[i][0], playersSpritesCords[i][1]));
  }
}

void Game::throwDiceMove() {
  // Dismiss any active card notification when dice is thrown
  if (cardNotification.isActive()) {
    cardNotification.dismiss();
  }

  // Throw a dice action
  diceResultPlayer = roundDice.throwDiceSix();
  players[turn].characters[0].diceResult = diceResultPlayer;
  currentState = state_game;
  bubble.state = BubbleState::MOVE;
  nextRotateElem.reset();
  prevRotateElem.reset();
}
/*!
 * \brief Game::playerMakeMove move the player into the position on the map
 * \param mousePos
 */
void Game::playerMakeMove(int mousePos) {
  players[turn].setFigurePos(mousePos);
  commandManager.processField(mousePos);
  const int* possibleExit =
      std::find(std::begin(DP::endPlayers), std::end(DP::endPlayers), mousePos);
  if (possibleExit != DP::endPlayers + 4) {
    players[turn].done = true;
    players[turn].reachedPortal = true;
    commandManager.removeAllItems(turn);
    if (numberFinishedPlayers == 0) {
      players[turn].reachedPortalFirst = true;
      int turnover = (rand() % 2) + 5;

      players[turn].cash += turnover;
      players[turn].updatePlayer();
      startDeerMode();
    }

    sfx.soundPortal.play();
    numberFinishedPlayers += 1;
    if (numberFinishedPlayers > 3) {
      stateManager->endGame();
      return;
    }
  }
  nextPlayer();
  return;
}

int Game::mostDiamonds() const {
  std::array<int, 4> results = {
      {players[0].cash, players[1].cash, players[2].cash, players[3].cash}};
  auto minmax = std::minmax_element(std::begin(results), std::end(results));
  int maxResult = *(minmax.second);
  int result = 0;
  int pos = -1;
  for (int i = 0; i < 4; i++) {
    if (players[i].cash == maxResult) {
      result += 1;
      pos = i;
    }
  };
  if (result == 1) {
    return pos;
  }
  return -1;
}

/*!
 * \brief Game::handleLeftClick
 * \param pos
 * \param posFull
 * \param mousePos
 */
void Game::handleLeftClick(sf::Vector2f pos, sf::Vector2f posFull, int mousePos) {
  if (currentState == state_game) {
    if (players[turn].human) {
      std::array<int, 2> movements = players[turn].getMovements(diceResultPlayer);
      if ((mousePos == movements[0]) || (mousePos == movements[1])) {
        playerMakeMove(mousePos);
      }
    }
  }

  if (currentState == state_setup_players) {
    for (int i = 0; i < 4; i++) {
      sf::FloatRect spriteHumanRect(players[i].spriteAI->getGlobalBounds());
      if (spriteHumanRect.contains(posFull)) {
        players[i].swapHuman();
      }
    }
    sf::IntRect startGameRect({580, 640}, {180, 80});
    if (startGameRect.contains(sf::Vector2i((int)posFull.x, (int)posFull.y))) {
      bigDiamondActive = true;
      // Don't set banner text during animation to avoid covering the animation
      // banner.setText("start game");
      
      // Initialize animation with final diamond positions
      boardAnimator->initializeAnimation(boardDiamonds, window);
      
      // Transition to animation state instead of going directly to roll_dice
      stateManager->transitionToBoardAnimation();
      return;
    }
  } else if (currentState == state_board_animation) {
    // Animation input handling is now done in GameInput class
    // This section can be removed to avoid duplicate processing
    return;
  } else if (currentState == state_roll_dice) {
    if (players[turn].human) {
      sf::FloatRect diceRect(roundDice.spriteDice->getGlobalBounds());
      if (diceRect.contains(posFull)) {
        // Only allow dice throwing if notification delay has expired
        if (cardNotificationDelay <= 0) {
          throwDiceMove();
        }
      }
    }
  }

  if (currentState == state_menu) {
    downTimeCounter = 0;
    stateManager->hideMenu();
    stateManager->showGameBoard();
    return;
  }

  if (currentState == state_intro_shader) {
    // Allow skipping the intro shader by clicking
    stateManager->showMenu();
    return;
  }

  if (currentState == state_gui_end_round) {
    std::string resultCommand = guiRoundDice.getElem(pos);
    command(resultCommand);
  }

  if (currentState == state_lets_begin) {
    if (boardAnimator) {
        boardAnimator->releaseDiamonds();
    }
    if (lightingManager && boardAnimationLightingInitialized) {
        lightingManager->cleanup();
        boardAnimationLightingInitialized = false;
    }
    currentState = state_roll_dice;
    launchNextPlayer();
    return;
  }

  if (currentState == state_end_game) {
    if (downTimeCounter > 2) {
      currentState = state_menu;
      musicMenu.play();
      restartGame();
      return;
      //            restartGame();
    }
  }
}

Game::Game(bool newTestMode)
    : screenSize(DP::initScreenX, DP::initScreenY),
      viewFull(sf::FloatRect({0, 0}, {(float)screenSize.x, (float)screenSize.y})),
      viewGui(sf::FloatRect({0, 0}, {(float)screenSize.x, (float)screenSize.y})),
      viewTiles(sf::FloatRect({0, 0}, {1360, 768})), currentSeason(1), month(0),
      selector(DP::TILE_SIZE), character(&textures, 3), gameTitle("deerportal"), roundDice(players),
      roundNumber(1), guiRoundDice(&textures), boardDiamonds(&textures),
      window(sf::VideoMode(sf::Vector2u(DP::initScreenX, DP::initScreenY)),
             "Deerportal - game about how human can be upgraded to the Deer"),
      turn(0), commandManager(*this), cardsDeck(&textures, &menuFont, &commandManager),
      banner(&gameFont), cardNotification(&gameFont, &textures), bigDiamondActive(false),
      credits(&gameFont), sfxClick(sfxClickBuffer), sfxDone(sfxDoneBuffer),
      nextRotateElem(&textures), prevRotateElem(&textures), cpuTimeThinkingInterval(1.0f),
      cardNotificationDelay(0.0f), deerModeCounter(4), deerModeActive(false), v1(0.0f),
      fpsDisplayUpdateTimer(0.0f) {
  testMode = newTestMode;
  // Initialize unique_ptr text members (these have font constructors)
  txtWinner = std::make_unique<sf::Text>(gameFont);
  txtSurvivorsLabel = std::make_unique<sf::Text>(gameFont);
  txtLoosersLabel = std::make_unique<sf::Text>(gameFont);
  paganHolidayTxt = std::make_unique<sf::Text>(gameFont);
  gameVersion = std::make_unique<sf::Text>(gameFont);
  menuTxt = std::make_unique<sf::Text>(gameFont);
  endGameTxt = std::make_unique<sf::Text>(gameFont);
  textLoading = std::make_unique<sf::Text>(menuFont);
  textFPS = std::make_unique<sf::Text>(gameFont); // Initialize FPS text

  // Sprite initialization will be done in loadAssets() where textures are available
  // renderSprite will be initialized after renderTexture is ready

  // Initialize text arrays
  for (int i = 0; i < 4; i++) {
    endGameTxtAmount[i] = std::make_unique<sf::Text>(gameFont);
  }

  // Restoring playersSpritesCords assignments to the constructor body
  playersSpritesCords[0][0] = 202;
  playersSpritesCords[0][1] = 76;
  playersSpritesCords[1][0] = 562;
  playersSpritesCords[1][1] = 76;
  playersSpritesCords[3][0] = 202;
  playersSpritesCords[3][1] = 436;
  playersSpritesCords[2][0] = 562;
  playersSpritesCords[2][1] = 436;

  textLoading->setString("loading...");
  textLoading->setFont(menuFont);
  textLoading->setPosition(sf::Vector2f(200, 200));
  textLoading->setFillColor(sf::Color::White);
  textLoading->setCharacterSize(10);
  if (!renderTexture.resize(sf::Vector2u(1360, 768))) {
    throw std::runtime_error("Failed to resize render texture in Game constructor");
  }
  renderTexture.clear(sf::Color::White);
  renderTexture.draw(*textLoading);
  renderTexture.display();

  // Initialize renderSprite with renderTexture after renderTexture is ready
  renderSprite = std::make_unique<sf::Sprite>(renderTexture.getTexture());
  numberFinishedPlayers = 0;
  // frameClock is now a member variable
  guiRoundDice.active = true;
  showPlayerBoardElems = false;
  // V-Sync is now properly configured in window creation and fullscreen toggle

  std::srand(time(NULL));
  window.clear(sf::Color(55, 55, 55));
  renderTexture.draw(*textLoading);
  // window.display();

  // Initialize modules
  assets = std::make_unique<GameAssets>(this);
  input = std::make_unique<GameInput>(this);
  renderer = std::make_unique<GameRenderer>(this);
  core = std::make_unique<GameCore>(this);
  stateManager = std::make_unique<GameStateManager>(this);
  animationSystem = std::make_unique<GameAnimationSystem>(this);

  // Initialize board animation system
  boardAnimator = std::make_unique<BoardInitializationAnimator>();
  
  // Initialize lighting system
  lightingManager = std::make_unique<DP::LightingManager>();

  loadAssets();
  textLoading->setFont(menuFont);
  textLoading->setPosition(sf::Vector2f(200, 200));
  textLoading->setFillColor(sf::Color::White);
  textLoading->setCharacterSize(10);
  renderTexture.clear(sf::Color::Black);
  renderTexture.draw(*textLoading);
  window.display();

  gameVersion->setString("version: " + std::string(DEERPORTAL_VERSION) + "-" +
                         std::string(BASE_PATH));
  gameVersion->setFont(gameFont);
  gameVersion->setPosition(sf::Vector2f(10, 10));
  gameVersion->setFillColor(sf::Color::White);
  gameVersion->setCharacterSize(15);

  initBoard();
  renderTexture.clear(sf::Color::Black);
  renderTexture.draw(*textLoading);
  renderTexture.display();

  stateManager->showIntroShader();

  // Constructor initialization complete
  // Game loop moved to run() method for proper RAII

  // Initialize window manager
  windowManager.initialize(window, "Deerportal - game about how human can be upgraded to the Deer");
  windowManager.updateView(renderTexture);
  windowManager.updateSpriteScaling(*renderSprite, window);
}

bool Game::toggleFullscreen() {
  // Use window manager to toggle fullscreen with render texture and sprite support
  return windowManager.toggleFullscreen(window, renderTexture, *renderSprite);
}

int Game::run() {
  // Handle test mode
  if (testMode) {
    return 0; // Exit gracefully for test mode
  }

  // Main game loop
  while (window.isOpen()) {
    sf::Time frameTime = frameClock.restart();

    // handle events - SFML 3.0 variant-based event system
    float xpos = 320.0f;
    float ypos = 240.0f;
    float xgrv = 0.0f;
    float ygrv = 0.0f;

    // Let GameInput handle ALL events (including F11 fullscreen and Escape)
    input->processEvents(window);

    // All event handling (including mouse) is now managed by GameInput
    update(frameTime);
    render(frameTime.asSeconds());
  }

  return 0; // Game ended normally
}

void Game::update(sf::Time frameTime) {
  // FPS calculation and display update (NEW 0.8.2 FEATURE)
  fpsDisplayUpdateTimer += frameTime.asSeconds();
  if (fpsDisplayUpdateTimer >= 0.25f) { // Update FPS display every 0.25 seconds
    float fps = 1.0f / frameTime.asSeconds();
    textFPS->setString("FPS: " + std::to_string(static_cast<int>(fps)));
    fpsDisplayUpdateTimer = 0.0f;
  }

  runningCounter += frameTime.asSeconds();

  // Update animation system for all states
  animationSystem->update(frameTime);

  // PERFORMANCE OPTIMIZATION: State-aware conditional updates
  switch (currentState) {
  case state_game:
  case state_roll_dice:
    // Full game updates only during gameplay
    updateGameplayElements(frameTime);
    break;

  case state_intro_shader:
    // Update intro shader animation
    introShader.update(frameTime.asSeconds());
    if (introShader.isFinished()) {
      // Clear any previous render state before transition
      window.clear();
      // Transition to menu after intro finishes
      stateManager->showMenu();
      return; // Skip further processing this frame
    }
    break;

  case state_menu:
    // Only update menu-specific elements
    credits.update(frameTime);
    break;

  case state_board_animation:
#ifndef NDEBUG
    std::cout << "UPDATE DEBUG: In state_board_animation, updating animation" << std::endl;
#endif
    // Update board initialization animation
    boardAnimator->update(frameTime);
    
    // Check for automatic transition after fade-out completes
    if (boardAnimator->isComplete()) {
#ifndef NDEBUG
      std::cout << "TRANSITION: Board animation and fade-out complete, starting game directly" << std::endl;
#endif
      // Clean up lighting system before transitioning
      if (boardAnimationLightingInitialized) {
        boardAnimationLightingInitialized = false;
        lightingManager->cleanup();
      }
      boardAnimator->releaseDiamonds();
      // Skip state_lets_begin and go directly to roll dice
      currentState = state_roll_dice;
      launchNextPlayer();
    }
    
    updateMinimalElements(frameTime);
    break;

  case state_setup_players:
  case state_lets_begin:
  case state_end_game:
    // Minimal updates for transition states
    updateMinimalElements(frameTime);
    break;

  default:
    // Skip expensive updates for other states
    break;
  }
}

void Game::updateGameplayElements(sf::Time frameTime) {
  // Banner only when active
  if (banner.active) {
    banner.update(frameTime);
  }

  // Card notification updates
  if (cardNotification.isActive()) {
    cardNotification.update(frameTime);
  }

  cpuTimeThinking -= frameTime.asSeconds();

  // Update card notification delay for computer players
  if (cardNotificationDelay > 0) {
    cardNotificationDelay -= frameTime.asSeconds();
  }

  // Big diamond animation handled by animation system
  if (bigDiamondActive) {
    animationSystem->startBigDiamondAnimation();
    animationSystem->updateBigDiamondAnimation(frameTime);
  } else {
    animationSystem->stopBigDiamondAnimation();
  }

  // AI logic for dice rolling
  if (currentState == state_roll_dice) {
    if ((cpuTimeThinking < 0) && (players[turn].human == false) && (cardNotificationDelay <= 0)) {
      cpuTimeThinking = cpuTimeThinkingInterval;
      throwDiceMove();
    }
  }

  // Game state updates and AI logic
  if (currentState == state_game) {
    std::array<int, 2> currentMovements = players[turn].getMovements(diceResultPlayer);

    if ((cpuTimeThinking < 0) && (players[turn].human == false) && (cardNotificationDelay <= 0)) {
      std::vector<int> listRandomPos;

      if (currentMovements[0] > -1) {
        listRandomPos.push_back(currentMovements[0]);
      }
      if (currentMovements[1] > -1) {
        listRandomPos.push_back(currentMovements[1]);
      }

      unsigned int sizeRndPos = listRandomPos.size();
      if (sizeRndPos == 0) {
        // No moves available
      } else if (sizeRndPos == 1) {
        playerMakeMove(listRandomPos[0]);
      } else if (sizeRndPos == 2) {
        if (deerModeActive) {
          playerMakeMove(listRandomPos[1]);
        } else {
          if (players[turn].reachPortalMode == true) {
            playerMakeMove(listRandomPos[1]);
          } else {
            if (boardDiamonds.ifFieldIsEmpty(listRandomPos[1]) == false) {
              playerMakeMove(listRandomPos[1]);
              return;
            }
            if (boardDiamonds.ifFieldIsEmpty(listRandomPos[0]) == false) {
              playerMakeMove(listRandomPos[0]);
              return;
            }
            if ((boardDiamonds.ifFieldIsEmpty(listRandomPos[0]) == false) &&
                (boardDiamonds.ifFieldIsEmpty(listRandomPos[1]) == false)) {
              playerMakeMove(listRandomPos[1]);
              return;
            }
            int randPos = rand() % 2;
            playerMakeMove(listRandomPos[randPos]);
          }
        }
      }
    }

    // Rotation elements only when visible and active
    if (currentMovements[0] > -1) {
      prevRotateElem.spriteRotate->setPosition(players[turn].characters[0].leftChar->getPosition());
      prevRotateElem.spriteRotate->move(sf::Vector2f(10, 20));
      prevRotateElem.spriteRotate->move(sf::Vector2f(-202, -76));
      prevRotateElem.update(frameTime);
      prevRotateElem.setColor();
    }
    if (currentMovements[1] > -1) {
      nextRotateElem.spriteRotate->setPosition(
          players[turn].characters[0].rightChar->getPosition());
      nextRotateElem.spriteRotate->move(sf::Vector2f(10, 20));
      nextRotateElem.spriteRotate->move(sf::Vector2f(-202, -76));
      nextRotateElem.update(frameTime);
      nextRotateElem.setColor();
    }
  }

  // Cards deck only during game states where cards are relevant
  cardsDeck.update(frameTime);

  // PERFORMANCE: Only update current player instead of all 4
  players[turn].play();
  players[turn].update(frameTime);

  // Update other players only if they need minimal processing (idle animations, etc.)
  for (int i = 0; i < 4; i++) {
    if (i != turn) {
      // Minimal update for non-active players (only if needed for idle animations)
      // players[i].updateIdle(frameTime); // Uncomment if such method exists
    }
  }

  // Game state transitions
  
  // Auto-transition from board animation to gameplay when fade-out completes
  if (currentState == state_board_animation && boardAnimator && boardAnimator->isComplete()) {
#ifndef NDEBUG
    std::cout << "TRANSITION: Board animation and fade-out complete, starting game automatically" << std::endl;
#endif
    // Clean up lighting system before transitioning
    if (boardAnimationLightingInitialized) {
      boardAnimationLightingInitialized = false;
      lightingManager->cleanup();
    }
    boardAnimator->releaseDiamonds();
    currentState = state_roll_dice;
    launchNextPlayer();
    return;
  }
  
  // Also handle automatic transition if somehow we ended up in state_lets_begin
  if (currentState == state_lets_begin && boardAnimator && boardAnimator->isComplete()) {
#ifndef NDEBUG
    std::cout << "TRANSITION: In lets_begin but fade-out complete, starting game immediately" << std::endl;
#endif
    // Clean up lighting system before transitioning
    if (boardAnimationLightingInitialized) {
      boardAnimationLightingInitialized = false;
      lightingManager->cleanup();
    }
    boardAnimator->releaseDiamonds();
    currentState = state_roll_dice;
    launchNextPlayer();
    return;
  }
  
  if (currentState == state_lets_begin) {
    downTimeCounter += frameTime.asSeconds();
    spriteLestBegin->setColor(sf::Color(255, 255, 255, 255 - (downTimeCounter * 35)));
    // DISABLED: Automatic transition - let user manually trigger
    // if (downTimeCounter > 5) {
    //   currentState = state_roll_dice;
    //   bubble.state = BubbleState::DICE;
    // }
  }

  if (currentState == state_end_game) {
    downTimeCounter += frameTime.asSeconds();
  }

  bubble.update(frameTime);
}

void Game::updateMinimalElements(sf::Time frameTime) {
  // Only essential updates for transition states
  if (banner.active) {
    banner.update(frameTime);
  }

  // Card notification updates
  if (cardNotification.isActive()) {
    cardNotification.update(frameTime);
  }

  if (currentState == state_lets_begin) {
    downTimeCounter += frameTime.asSeconds();
    spriteLestBegin->setColor(sf::Color(255, 255, 255, 255 - (downTimeCounter * 35)));
    if (downTimeCounter > 5) {
      currentState = state_roll_dice;
      bubble.state = BubbleState::DICE;
    }
  }

  if (currentState == state_end_game) {
    downTimeCounter += frameTime.asSeconds();
  }
}

/*!
 * \brief Game::nextRound is happening each every 4 months
 */
void Game::nextRound() {
  turn = 0;
  //    std::string result = roundDice.drawRound();
  roundNumber += 1;
  month++;
  if (month == 13) month = 1;
  if (month % 4 == 0) currentSeason++;
  if (currentSeason > 3) currentSeason = 0;
  //    if (players[turn].done==true)
  //        nextPlayer();

  launchNextPlayer();
}

/*!
 * \brief Game::nextPlayer calculates which player should play
 */
void Game::nextPlayer() {

  // End of game - we don't calculate more players
  if (currentState == state_end_game) {
    return;
  }

  // End of game - we don't calculate more players
  if (numberFinishedPlayers == 4) {
    stateManager->endGame();
    return;
  }

  // Update old player

  players[turn].updatePlayer();
  if (turn > 2) {
    nextRound();
    return;
  }
  turn++;
  launchNextPlayer();
}
void Game::launchNextPlayer() {
  if (deerModeActive) {
    deerModeCounter -= 1;
  }

  if (deerModeCounter < 0) {
    stateManager->endGame();
    return;
  }
  // Just control
  if (players[turn].done == true) {
    nextPlayer();
    return;
  }

  // Frozen player
  if (players[turn].frozenLeft > 0) {
    players[turn].frozenLeft -= 1;
    nextPlayer();
    return;
  }

  // Here we process new player

  selector.changeColor(turn);
  for (int i = 0; i < 4; i++) {
    if (i == turn) {
      players[i].setActive(true);
      currentNeighbours = players[i].getNeighbours();
    } else
      players[i].setActive(false);
  }

  sfxClick.play();
  diceResultPlayer = 6;
  roundDice.setDiceTexture(diceResultPlayer);

  players[turn].characters[0].diceResult = diceResultPlayer;
  groupHud.setRoundName(roundNumber);
  if (!deerModeActive) {
    if (mostDiamonds() == turn) {
      players[turn].reachPortalMode = true;
      bigDiamondActive = true; // Show diamond for player with most diamonds
    } else {
      players[turn].reachPortalMode = false;
      // bigDiamondActive remains unchanged here, allowing it to persist through ties if previously
      // active
    }
  } else {
    // In Deer Mode: bigDiamond is already false (set by startDeerMode)
    // Ensure players cannot enter portal mode through the 'most diamonds' mechanic here.
    players[turn].reachPortalMode = false;
  }

  if (deerModeActive == false) {
    groupHud.setSeason(currentSeason);
  } else {
    groupHud.setDeerModeActive();
    int number = (deerModeCounter / 4);

    if (deerModeCounter < 16) number += 1;
    if (number < 0) {
      stateManager->endGame();
      return;
    }
    groupHud.setDeerModeCounter(number);
  }
  currentState = state_roll_dice;
  bubble.state = BubbleState::DICE;
  roundDice.setColor(turn);
  roundDice.setFaces(
      6); // Set dice to show "waiting to click" face (face 6 - the distinctive 45-degree sprite)
  bubble.setPosition(players[turn].characters[0].getPosition().x - 30,
                     players[turn].characters[0].getPosition().y - 45);

  cpuTimeThinking = cpuTimeThinkingInterval;
}

void Game::drawPlayersGui() {
  for (int i = 0; i < 4; i++) {
    renderTexture.draw(players[i]);
  }
}

void Game::drawSquares() {
  if (showPlayerBoardElems) {
    renderTexture.draw(selector);
  }
}

void Game::drawBaseGame() {
  renderTexture.setView(viewTiles);
  for (int i = 0; i < 4; i++) {
    renderTexture.draw(players[i].elems);
  }
  drawSquares();
  renderTexture.setView(viewGui);
  renderTexture.setView(viewTiles);
  renderTexture.setView(viewTiles); // Yeah Katia's inspiration

  float v;
  v = sin(runningCounter * 0.01f);
  shaderBlur.setUniform("blur_radius", v);

  //    renderTexture.draw(gameBackground);
  renderTexture.setView(viewFull);
  //    renderTexture.draw(*spriteBackgroundArt,  &shaderDark);
  //    spriteBackgroundArt.setColor(sf::Color(255, 255, 255, 208));
  shaderBlur.setUniform("blur_radius", 0.01f);

  //    shaderBlur.setParameter("blur_radius", sin(runningCounter*0.01) );
  //    shaderBlur.setParameter("blur_radius", sin(runningCounter*0.01) );

  v = sin(runningCounter * 0.005f);
  shaderPixel.setUniform("pixel_threshold", v);

  renderTexture.draw(*spriteBackgroundArt);
  spriteBackgroundArt->setColor(sf::Color(255, 255, 255, 255)); // Full opacity
  v = sin(runningCounter * 0.05f) / 2;
  shaderBlur.setUniform("blur_radius", v);

  renderTexture.draw(cardsDeck);
  if (currentState == state_roll_dice) {
    spriteBackgroundArt->setColor(sf::Color(255, 255, 255));

    v = sin(runningCounter * 0.5f) / 4;
    shaderBlur.setUniform("blur_radius", v);
    renderTexture.draw(*roundDice.spriteDice);
  } else
    renderTexture.draw(*roundDice.spriteDice);
  renderTexture.setView(viewTiles);
  drawSquares();
}

void Game::drawCharacters() {
  if (currentState == state_game) {
    std::array<int, 2> currentMovements =
        players[turn].characters[0].getMovements(diceResultPlayer);
    if (currentMovements[1] > -1) {
      if (nextRotateElem.active) renderTexture.draw(nextRotateElem);
    }
    if (currentMovements[0] > -1) {
      if (prevRotateElem.active) renderTexture.draw(prevRotateElem);
    }
  }
  renderTexture.setView(viewFull);
  shaderBlur.setUniform("blur_radius", 0.005f);
  for (int i = 0; i < 4; i++) {
    for (auto&& j : players[i].characters) {
      if (currentState == state_game)
        j.drawMovements = true;
      else
        j.drawMovements = false;
      renderTexture.draw(j);
      //            renderTexture.draw(j, &shaderBlur);
    }
  }
}

/*!
 * \brief Game::render main function responsible for drawing all elements
 * \param deltaTime
 */
void Game::render(float deltaTime) {
  // All drawing is done to the renderTexture at a fixed 1360x768 resolution.
  // The final result is then scaled to the window by the WindowManager's view.

  window.clear(sf::Color::Black); // Clear window with black for letterboxing
  renderTexture.clear();

  // --- Begin Drawing to RenderTexture ---

  if ((currentState == state_game) || (currentState == state_roll_dice)) {
    renderTexture.setView(viewFull);
    renderTexture.draw(*spriteBackgroundDark);
    renderTexture.setView(viewTiles);
    drawBaseGame();
    renderTexture.setView(viewFull);
    renderTexture.draw(groupHud);
    renderTexture.setView(viewTiles);
    renderTexture.draw(boardDiamonds);
    drawCharacters();
    renderTexture.draw(bubble);
    getAnimationSystem()->drawCircleParticles(renderTexture);
    renderTexture.setView(viewFull);
    drawPlayersGui();
    renderTexture.setView(viewFull);
    if (bigDiamondActive) renderTexture.draw(*spriteBigDiamond);

  } else if (currentState == state_intro_shader) {
    // The intro shader has its own direct-to-window rendering path
    introShader.render(window);
    window.display();
    return;

  } else if (currentState == state_setup_players) {
    renderTexture.setView(viewFull);
    renderTexture.draw(*spriteDeerGod);
    for (int i = 0; i < 4; i++) {
      renderTexture.draw(*players[i].spriteAI);
    }
  } else if (currentState == state_board_animation) {
    renderTexture.setView(viewFull);
    renderTexture.draw(*spriteBackgroundDark);
    renderTexture.setView(viewTiles);
    drawBaseGame();
    renderTexture.setView(viewFull);
    renderTexture.draw(groupHud);

    // Render animated diamonds if animation is active or holding
    if (!boardAnimator->isComplete() || boardAnimator->isHoldingDiamonds()) {
      boardAnimator->render(renderTexture, textures.textureBoardDiamond);
    } else {
      // Animation skipped - render static diamonds immediately
      renderTexture.setView(viewTiles);
      renderTexture.draw(boardDiamonds);
      renderTexture.setView(viewFull);
    }
    
    if (boardAnimator && (!boardAnimator->isAnimationPhaseComplete() || boardAnimator->isHoldingDiamonds())) {
#ifndef NDEBUG
      std::cout << "LIGHTING: Applying lighting effects in state_board_animation" << std::endl;
#endif
      // Initialize lighting manager if needed
      if (!boardAnimationLightingInitialized) {
        if (lightingManager->initialize(renderTexture.getSize())) {
          boardAnimationLightingInitialized = true;
#ifndef NDEBUG
          std::cout << "LIGHTING: Initialized lighting system for board animation" << std::endl;
#endif
        }
      }
      
      // Begin lighting frame with dynamic ambient color (fades from dark to bright)
      sf::Color ambientColor = boardAnimator->getCurrentAmbientColor();
      lightingManager->beginFrame(ambientColor);
      
      // Update lights from animated diamonds
      boardAnimator->updateLights(*lightingManager);
      
      // Render lighting effects
      lightingManager->render(renderTexture);
      
#ifndef NDEBUG
      std::cout << "LIGHTING: Lighting effects applied successfully in game.cpp" << std::endl;
#endif
    }
    
    // DON'T reset lighting during fade-out - let it complete naturally
    // Lighting cleanup will happen during state transition
    
    drawCharacters();
    getAnimationSystem()->drawCircleParticles(renderTexture);
  } else if (currentState == state_gui_elem) {
    renderTexture.setView(viewFull);
    shaderBlur.setUniform("blur_radius", 2.0f);
    renderTexture.draw(*spriteBackgroundDark, &shaderBlur);
    drawBaseGame();
    drawCharacters();
    renderTexture.setView(viewFull);
    renderTexture.draw(groupHud);

  } else if (currentState == state_menu) {
    renderTexture.draw(*menuBackground);
    renderTexture.draw(*paganHolidayTxt);
    renderTexture.draw(credits);

  } else if (currentState == state_lets_begin) {
    renderTexture.setView(viewFull);
    shaderBlur.setUniform("blur_radius", 4.0f);
    renderTexture.draw(*spriteBackgroundDark, &shaderBlur);
    renderTexture.setView(viewTiles);
    drawBaseGame();
    
    // Render animated diamonds if still holding them, otherwise render real diamonds
    if (boardAnimator && boardAnimator->isHoldingDiamonds()) {
#ifndef NDEBUG
      std::cout << "GAME RENDER: Rendering animated diamonds in state_lets_begin" << std::endl;
#endif
      renderTexture.setView(viewFull);
      boardAnimator->render(renderTexture, textures.textureBoardDiamond);
      
      // Continue lighting effects for animated diamonds
      if (lightingManager) {
        if (!letsBeginLightingInitialized) {
          if (lightingManager->initialize(renderTexture.getSize())) {
            letsBeginLightingInitialized = true;
#ifndef NDEBUG
            std::cout << "LIGHTING: Initialized lighting system for lets_begin state" << std::endl;
#endif
          }
        }
        lightingManager->beginFrame(sf::Color(10, 10, 20, 255));
        boardAnimator->updateLights(*lightingManager);
        lightingManager->render(renderTexture);
      }
    } else {
#ifndef NDEBUG
      std::cout << "GAME RENDER: Rendering real diamonds in state_lets_begin" << std::endl;
#endif
      renderTexture.draw(boardDiamonds, &shaderBlur);
    }
    
    drawCharacters();
    renderTexture.setView(viewFull);
    drawPlayersGui();
    renderTexture.draw(*spriteLestBegin, &shaderBlur);

  } else if (currentState == state_gui_end_round) {
    renderTexture.setView(viewFull);
    renderTexture.draw(*spriteBackgroundDark);
    drawBaseGame();
    shaderBlur.setUniform("blur_radius", 0.05f);
    renderTexture.draw(guiRoundDice, &shaderBlur);
    renderTexture.setView(viewFull);
    renderTexture.draw(groupHud);

  } else if (currentState == state_end_game) {
    renderTexture.setView(viewFull);
    renderTexture.draw(*spriteBackgroundDark);
    renderTexture.draw(*spriteLestBegin, &shaderBlur);
    renderTexture.draw(*endGameTxt);
    renderTexture.draw(*txtWinner);
    renderTexture.draw(*txtSurvivorsLabel);
    for (unsigned int i = 0; i < txtSurvivors.size(); i++) {
      renderTexture.draw(*txtSurvivors[i]);
    }
    renderTexture.draw(*txtLoosersLabel);
    for (unsigned int i = 0; i < txtLoosers.size(); i++) {
      renderTexture.draw(*txtLoosers[i]);
    }
  }

  if (banner.active) renderTexture.draw(banner);
  if (cardNotification.isActive()) renderTexture.draw(cardNotification);

  renderTexture.setView(viewFull);

#if defined(DEERPORTAL_SHOW_FPS_COUNTER) || !defined(NDEBUG)
  renderTexture.draw(*textFPS);
#endif
#ifndef NDEBUG
  renderTexture.draw(*gameVersion);
#endif

  // --- End Drawing to RenderTexture ---

  // Finalize the texture
  renderTexture.display();

  // Set the final texture to the main render sprite
  renderSprite->setTexture(renderTexture.getTexture());

  // Apply blur shader to final render for performance
  v1 = sin(deltaTime) * 0.015f;
  shaderBlur.setUniform("blur_radius", 0.0003f);

  // The WindowManager handles scaling and letterboxing via sprite positioning
  // Apply shader to final scaled render for best performance
  window.draw(*renderSprite, &shaderBlur);
  window.display();
}

void Game::command(std::string command) {
  if (command.compare("end_of_round") == 0) {
    std::string subResult = command.substr(13);
    guiRoundDice.active = true;
    guiRoundDice.setTitle(subResult);
    currentState = state_gui_end_round;
  }

  if (command == "end_turn") nextPlayer();
}

/*!
 * \brief Game::startDeerMode launches last episode of the game
 */
void Game::startDeerMode() {
  deerModeActive = true;
  deerModeCounter = 16;
  banner.setText("deer mode");
  bigDiamondActive = false;
  sfx.soundDeerMode.play();
}
} // namespace DP