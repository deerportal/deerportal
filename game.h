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

namespace efc {

extern int initScreenX;
extern int initScreenY;

/*!
 * \brief Game is a main class of the Deer Portal - contains most logic but also rendering.
 */
class Game
{
public:
    sf::Vector2i screenSize;
    sf::View viewFull;
    sf::View viewGui;
    sf::View viewTiles;
    void setTxtEndGameAmount();


private:
    Selector selector;


    Character character;
    std::string gameTitle;
    std::string paganHolidayString;
    RoundDice roundDice;

    int roundNumber;

    GuiRoundDice guiRoundDice;


public:
    Game();
    BoardDiamondSeq boardDiamonds;
    sf::RenderWindow window;
    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;

    PlayerHud players[4];
    SoundFX sfx;



public:
    int turn;


private:
    void initBoard();
    void restartGame();
    void loadAssets();
    void drawPlayersGui();
    void drawSquares();

    void drawMenu();
    float runningCounter;
    sf::Sprite playersSprites[4]  ;
    sf::Vector2f getMousePos();

    int playersSpritesCords[4][2];

    enum states {
        state_init,
        state_menu,
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

    sf::Sprite spriteBackground;
    sf::Sprite gameBackground;
    sf::Sprite spriteBackgroundDark;
    sf::Sprite spriteLestBegin;
    sf::Texture textureBackgroundArt;
    sf::Sprite spriteBackgroundArt;
    sf::Texture textureTiles;
    sf::Texture textureFaces;
    sf::Font gameFont;
    sf::Font menuFont;
    sf::Text menuTxt;
    sf::Text endGameTxt;
    sf::Text endGameTxtAmount[4];
    sf::Text paganHolidayTxt;

    sf::Shader shaderBlur;
    sf::Shader shaderPixel;
    sf::Shader shaderDark;

    TileMap map;

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

    void nextRound();


    sf::Sprite menuBackground;
    sf::Sprite seasons[4];

    sf::Music musicGame;
    sf::Music musicMenu;

    sf::SoundBuffer sfxClickBuffer;
    sf::Sound sfxClick;

    sf::SoundBuffer sfxDoneBuffer;
    sf::Sound sfxDone;

    void showMenu();

    void hideMenu();
    void showGameBoard();
    void hideGameBoard();


    GroupHud groupHud;

    /*!
     * \brief showPlayerBoardElems defines if show mouse hover for the player
     */
    bool showPlayerBoardElems;

    void drawBaseGame();
    int month;

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
    sf::Text textLoading;
public:
    CardsDeck cardsDeck;
    void startDeerMode();

private:
    Bubble bubble;
    int deerModeCounter;
    bool deerModeActive;
};
}
#endif // GAME_H
