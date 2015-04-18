#ifndef GAME_H
#define GAME_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tilemap.h"
#include "selector.h"
#include "playerhud.h"
#include "textureholder.h"

namespace efc {
class Game
{
public:
    Game();
    sf::RenderWindow window;

private:
    void initBoard();
    void loadAssets();
    enum state {
        state_menu,
        state_game,
        state_quit
    };

    sf::Texture textureTiles;
    sf::Texture textureFaces;
    sf::Font gameFont;

    TileMap map;
    PlayerHud players[4];

    int mapSize;
    int level[256];
    int levelElems[256];
    TextureHolder textures;

};
}
#endif // GAME_H
