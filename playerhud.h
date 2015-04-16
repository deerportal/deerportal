#ifndef PLAYERHUD_H
#define PLAYERHUD_H
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class PlayerHud : public sf::Drawable, public sf::Transformable
{
public:
    PlayerHud(int faceNumber, sf::Texture *faces, int tileSize, int pos);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    int pos;

private:
    sf::Sprite spriteFace;
    int tileSize;
};



#endif // PLAYERHUD_H
