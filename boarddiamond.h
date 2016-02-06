#ifndef BOARDDIAMOND_H
#define BOARDDIAMOND_H
#include <set>
#include "textureholder.h"

#include <SFML/Graphics.hpp>

class BoardDiamond: public sf::Drawable, public sf::Transformable
{
public:
    BoardDiamond();
    BoardDiamond(TextureHolder *textures, int idNumber);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime, std::set<int>& busyTiles);

    TextureHolder *textures;
    int idNumber;

    sf::Sprite spriteHolder;
};

#endif // BOARDDIAMOND_H
