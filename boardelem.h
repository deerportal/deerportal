#ifndef BOARDELEM_H
#define BOARDELEM_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tilemap.h"
#include "textureholder.h"

namespace efc {
sf::RectangleShape createNeighbour(int pos);

class BoardElem: public sf::Sprite
{
public:
    BoardElem();
    BoardElem(TextureHolder *textures,int pos, int type, const sf::Texture *textureElem);


    int pos;
    int type;

    sf::Texture* textureTiles;
private:

    const sf::Texture* textureElem;
    static const sf::Texture& textureElem2;




    TextureHolder *textures;
    TextureHolder myHolder;
};
}

#endif // BOARDELEM_H
