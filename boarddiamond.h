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
                 int playerNumber, int boardPosition);

    TextureHolder *textures;
    /*!
     * \brief number from the sprite sheet
     */
    int idNumber;

    /*!
     * \brief 0-3 elements area on the board
     */
    int playerNumber;
    sf::Sprite spriteHolder;


    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime, std::set<int>& busyTiles);
};

#endif // BOARDDIAMOND_H
