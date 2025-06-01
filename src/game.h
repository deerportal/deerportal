#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <iostream>
#include <ctime>

#include <time.h>       /* time */

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "command.h"
#include "tilemap.h"
#include "selector.h"
#include "playerhud.h"
#include "textureholder.h"
#include "hover.h"
#include "guiwindow.h"
#include "rounddice.h"
#include "guirounddice.h"
#include "grouphud.h"
#include "animatedsprite.h"
#include "character.h"
#include "rotateelem.h"
#include "boarddiamondseq.h"
#include "soundfx.h"
#include "cardsdeck.h"
#include "calendar.h"
#include "particle.h"
#include "bubble.h"
//class Command;
#include "banner.h"
#include "credits.h"

namespace DP {

extern int initScreenX;
extern int initScreenY;

// Forward declarations for modules
class GameAssets;
class GameInput;
class GameRenderer;
class GameCore;

/*!
 * \brief Game is a main class of the Deer Portal - contains most logic but also rendering.
 */
class Game
{
    // Friend classes for module access
    friend class GameAssets;
    friend class GameInput; 
    friend class GameRenderer;
    friend class GameCore;

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
    BoardDiamondSeq boardDiamonds;
    sf::RenderWindow window;
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
    float oscilator;
    bool oscilatorInc;
    std::array<std::unique_ptr<sf::Sprite>, 4> playersSprites;
    int playersSpritesCords[4][2];

    // Game state enum moved to public for module access
    enum states {
        state_init,
        state_menu,
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

    ParticleSystem particleSystem;

    void setCurrentNeighbours ();
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

    void showMenu();

    void hideMenu();
    void showGameBoard();


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
    void endGame();

    float downTimeCounter;
    Command commandManager;
    std::unique_ptr<sf::Text> textLoading;
    
    // NEW 0.8.2 FEATURES
    std::unique_ptr<sf::Text> textFPS;  // FPS display text
    float fpsDisplayUpdateTimer;        // Timer for FPS display updates

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

    // Performance optimization methods
    void updateGameplayElements(sf::Time frameTime);
    void updateMinimalElements(sf::Time frameTime);
};
}
#endif // GAME_H
