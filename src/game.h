#ifndef GAME_H
#define GAME_H
#include <array>
#include <ctime>
#include <iostream>
#include <memory>
#include <set>
#include <stdlib.h>
#include <time.h> /* time */
#include <vector>

// Essential SFML includes that are used directly in the header
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Include headers for classes used as direct member variables (cannot be forward declared)
#include "animatedsprite.h"   // For AnimatedSprite animatedSprite;
#include "animation.h"        // For Animation members;
#include "banner.h"           // For Banner banner;
#include "boarddiamondseq.h"  // For BoardDiamondSeq boardDiamonds;
#include "bubble.h"           // For Bubble bubble;
#include "cardnotification.h" // For CardNotification cardNotification;
#include "cardsdeck.h"        // For CardsDeck cardsDeck;
#include "character.h"        // For Character character;
#include "command.h"          // For Command commandManager;
#include "credits.h"          // For Credits credits;
#include "data.h"             // For Player struct
#include "grouphud.h"         // For GroupHud groupHud;
#include "guirounddice.h"     // For GuiRoundDice guiRoundDice;
#include "introshader.h"      // For IntroShader introShader;
#include "rotateelem.h"       // For RotateElem members;
#include "rounddice.h"        // For RoundDice roundDice;
#include "selector.h"         // For Selector selector;
#include "soundfx.h"          // For SoundFX sfx;
#include "textureholder.h"    // For TextureHolder textures;
#include "window-manager.h"   // For WindowManager windowManager;

// Forward declarations for classes only used as pointers or in function signatures
// Include module headers for complete type information
#include "game-animation-system.h"
#include "game-assets.h"
#include "game-core.h"
#include "game-input.h"
#include "game-renderer.h"
#include "game-state-manager.h"

namespace DP {

extern int initScreenX;
extern int initScreenY;

/*!
 * \brief Game is a main class of the Deer Portal - contains most logic but also rendering.
 */
class Game {
  // Friend classes for module access
  friend class GameAssets;
  friend class GameInput;
  friend class GameRenderer;
  friend class GameCore;
  friend class GameStateManager;
  friend class GameAnimationSystem;

public:
  sf::Vector2i screenSize;
  sf::View viewFull;
  sf::View viewGui;
  sf::View viewTiles;
  void setTxtEndGameAmount();

  // Game state variables used by modules
  int currentSeason;
  int month;

private:
  Selector selector;
  Character character;
  std::string gameTitle;
  std::string paganHolidayString;
  RoundDice roundDice;
  int roundNumber;
  GuiRoundDice guiRoundDice;
  float v1;

public:
  Game(bool newTestMode);
  int run(); // Main game loop extracted from constructor
  
  /*!
   * \brief Toggle fullscreen mode
   * \return true if toggle was successful, false otherwise
   */
  bool toggleFullscreen();
  
  /*!
   * \brief Restore window properties after fullscreen toggle
   */
  void restoreWindowProperties();
  
  BoardDiamondSeq boardDiamonds;
  sf::RenderWindow window;
  WindowManager windowManager;  // NEW: Window manager for fullscreen handling
  sf::RenderTexture renderTexture;
  std::unique_ptr<sf::Sprite> renderSprite;
  Player players[4];
  SoundFX sfx;
  int turn;

private:
  void initBoard();
  void restartGame();
  void loadAssets();
  void drawPlayersGui();
  void drawSquares();
  void drawMenu();
  float runningCounter;
  std::array<std::unique_ptr<sf::Sprite>, 4> playersSprites;
  int playersSpritesCords[4][2];

  // Game state enum moved to public for module access
  enum states {
    state_init,
    state_menu,
    state_intro_shader, // NEW: Intro shader animation state
    state_setup_players,
    state_lets_begin,
    state_roll_dice,
    state_game,
    state_gui_elem,
    state_select_building,
    state_gui_end_round,
    state_end_game,
    state_quit
  };
  states currentState;

  std::unique_ptr<sf::Sprite> spriteDeerGod;
  std::unique_ptr<sf::Sprite> spriteBackgroundDark;
  std::unique_ptr<sf::Sprite> spriteLestBegin;
  sf::Texture textureBackgroundArt;
  std::unique_ptr<sf::Sprite> spriteBackgroundArt;
  sf::Texture textureIntroMenu; // NEW: Texture for intro menu image
  sf::Texture textureTiles;
  sf::Texture textureFaces;
  sf::Font gameFont;
  sf::Font menuFont;
  std::unique_ptr<sf::Text> menuTxt;
  std::unique_ptr<sf::Text> endGameTxt;
  std::array<std::unique_ptr<sf::Text>, 4> endGameTxtAmount;
  std::unique_ptr<sf::Text> paganHolidayTxt;
  std::unique_ptr<sf::Text> gameVersion;
  sf::Shader shaderBlur;
  sf::Shader shaderPixel;
  sf::Shader shaderDark;
  int mapSize;
  int level[256];
  int levelElems[256];
  TextureHolder textures;
  std::set<int> currentNeighbours;
  void command(std::string command);
  int selectedPos;

  void update(sf::Time frameTime);
  void render(float deltaTime);


  void setCurrentNeighbours();
  void nextPlayer();
  void launchNextPlayer();
  void nextRound();

  std::unique_ptr<sf::Sprite> menuBackground;
  std::array<std::unique_ptr<sf::Sprite>, 4> seasons;

  sf::Music musicGame;
  sf::Music musicMenu;

  sf::SoundBuffer sfxClickBuffer;
  sf::Sound sfxClick;

  sf::SoundBuffer sfxDoneBuffer;
  sf::Sound sfxDone;

  GroupHud groupHud;

  /*!
   * \brief showPlayerBoardElems defines if show mouse hover for the player
   */
  bool showPlayerBoardElems;

  void drawBaseGame();

  Animation walkingAnimationDown;
  Animation walkingAnimationUp;
  Animation walkingAnimationLeft;
  Animation walkingAnimationRight;
  Animation* currentAnimation;
  AnimatedSprite animatedSprite;

  void drawCharacters();
  void handleLeftClick(sf::Vector2f pos, sf::Vector2f posFull, int mousePos);

  std::set<int> busyTiles;

  int diceResultPlayer;
  int numberFinishedPlayers;
  RotateElem nextRotateElem;
  RotateElem prevRotateElem;

  float downTimeCounter;
  Command commandManager;
  std::unique_ptr<sf::Text> textLoading;

  // NEW 0.8.2 FEATURES
  std::unique_ptr<sf::Text> textFPS; // FPS display text
  float fpsDisplayUpdateTimer;       // Timer for FPS display updates
  sf::Clock frameClock;              // Frame timing for game loop

public:
  CardsDeck cardsDeck;
  void startDeerMode();

  std::unique_ptr<sf::Text> txtWinner;

  std::unique_ptr<sf::Text> txtSurvivorsLabel;
  std::unique_ptr<sf::Text> txtLoosersLabel;
  std::vector<std::unique_ptr<sf::Text>> txtSurvivors;
  std::vector<std::unique_ptr<sf::Text>> txtLoosers;

  void throwDiceMove();
  void playerMakeMove(int mousePos);

  float cpuTimeThinking;
  Banner banner;
  CardNotification cardNotification;

  std::unique_ptr<sf::Sprite> spriteBigDiamond;
  bool bigDiamondActive;

  int mostDiamonds() const;

  Credits credits;

  float cpuTimeThinkingInterval;

private:
  Bubble bubble;
  int deerModeCounter;
  bool deerModeActive;

public:
  bool testMode;

  // Module instances
  std::unique_ptr<GameAssets> assets;
  std::unique_ptr<GameInput> input;
  std::unique_ptr<GameRenderer> renderer;
  std::unique_ptr<GameCore> core;
  std::unique_ptr<GameStateManager> stateManager;
  std::unique_ptr<GameAnimationSystem> animationSystem;

  // NEW: Intro shader instance
  IntroShader introShader;

  // Performance optimization methods
  void updateGameplayElements(sf::Time frameTime);
  void updateMinimalElements(sf::Time frameTime);
};
} // namespace DP
#endif // GAME_H
