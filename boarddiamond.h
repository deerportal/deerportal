#ifndef BOARDDIAMOND_H
#define BOARDDIAMOND_H
#include <set>
#include "textureholder.h"
#include "elem.h"
#include <SFML/Graphics.hpp>

class   BoardDiamond: public Elem
{
public:
    BoardDiamond();
    BoardDiamond(TextureHolder *textures, int idNumber,
                 int element, int boardPosition);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime, std::set<int>& busyTiles);

    TextureHolder *textures;
    int idNumber; // number from the sprite sheet

    int element; // 0-3 elements area on the board
    sf::Sprite spriteHolder;
};

#endif // BOARDDIAMOND_H
