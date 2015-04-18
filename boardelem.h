#ifndef BOARDELEM_H
#define BOARDELEM_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "tilemap.h"
#include "textureholder.h"

class BoardElem: public sf::Sprite
{
public:
    BoardElem();
    BoardElem(TextureHolder *textures,int pos, int type, const sf::Texture *textureElem);



    int pos;
    int type;



private:
//    sf::Sprite spriteElem;
    const sf::Texture* textureElem;
    static const sf::Texture& textureElem2;
    TextureHolder *textures;
    TextureHolder myHolder;
};

#endif // BOARDELEM_H
