#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include <iostream>
#include <time.h>       /* time */
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "tilemap.h"
#include "selector.h"
#include "playerhud.h"
#include "textureholder.h"
#include "hover.h"
#include "guichoosebuilding.h"
#include "guiwindow.h"
#include "rounddice.h"
#include "guirounddice.h"

namespace efc {






class Game
{
public:
    Game();
    sf::RenderWindow window;
    sf::View viewTiles;


private:
    void initBoard();
    void loadAssets();
    void drawPlayersGui();
    void drawSquares();

    void drawMenu();

    sf::Vector2f getMousePos();

    enum states {
        state_init,
        state_menu,
        state_game,
        state_gui_elem,
        state_select_building,
        state_gui_end_round,
        state_quit
    };

    states currentState;

    sf::Texture textureBackground;
    sf::Sprite spriteBackground;
    sf::Sprite spriteBackgroundDark;


    sf::Texture textureTiles;
    sf::Texture textureFaces;
    sf::Font gameFont;
    sf::Font menuFont;
    sf::Text menuTxt;

    std::string gameTitle;


    TileMap map;
    PlayerHud players[4];

    int mapSize;
    int level[256];
    int levelElems[256];
    TextureHolder textures;
    std::set<int> currentNeighbours;
    void command(std::string command);
    int selectedPos;
    int turn;
    void update();
    void render();
    sf::View viewGui;
    Selector selector;
    sf::View viewFull;

    RoundDice roundDice;
    int roundNumber;


    GuiChooseBuilding guiSelectBuilding;
    GuiRoundDice guiRoundDice;
    void setCurrentNeighbours ();
    void nextPlayer();

    void nextRound();


    sf::Sprite menuBackground;


    sf::Music musicGame;
    sf::Music musicBackground;
    sf::Music musicMenu;

    sf::SoundBuffer sfxClickBuffer;
    sf::Sound sfxClick;
    void showMenu();
    void hideMenu();
    void showGameBoard();
    void hideGameBoard();




    bool showPlayerBoardElems;
    void drawBaseGame();
};
}
#endif // GAME_H
