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
    sf::Vector2f getMousePos();

    enum states {
        state_init,
        state_menu,
        state_game,
        state_gui_elem,
        state_select_building,
        state_quit
    };

    states currentState;

    sf::Texture textureTiles;
    sf::Texture textureFaces;
    sf::Font gameFont;

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

    GuiChooseBuilding guiSelectBuilding;
    void setCurrentNeighbours ();
    void nextPlayer();

    void drawSquares();


    sf::Music musicGame;
    sf::SoundBuffer sfxClickBuffer;
    sf::Sound sfxClick;

};
}
#endif // GAME_H
