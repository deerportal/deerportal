#ifndef BOARDELEM_H
#define BOARDELEM_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "tilemap.h"
#include "textureholder.h"

namespace DP {
sf::RectangleShape createNeighbour(int pos);

class BoardElem: public sf::Sprite
{
public:
    BoardElem();
    BoardElem(int pos, int type);
    
    static sf::Texture& getDefaultTexture();

    int pos;
    int type;

    sf::Texture* textureTiles;
};
}

#endif // BOARDELEM_H
