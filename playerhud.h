#ifndef PLAYERHUD_H
#define PLAYERHUD_H
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "boardelems.h"
#include "textureholder.h"
class PlayerHud : public sf::Drawable, public sf::Transformable
{
public:
    PlayerHud();
    PlayerHud(TextureHolder *textures, int faceNumber, sf::Font *gameFont, int tileSize, int pos);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    int pos;
    int cash;
    int energy;
    int food;
    BoardElems elems;



    std::vector<int> properties;



private:
//    sf::Font gameFont;
    sf::Sprite spriteFace;
    sf::Text txtCash;
    sf::Text txtEnergy;
    sf::Text txtFood;
    int faceSize;
    int tileSize;
    TextureHolder *textures;
};



#endif // PLAYERHUD_H
