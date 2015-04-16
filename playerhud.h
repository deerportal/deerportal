#ifndef PLAYERHUD_H
#define PLAYERHUD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class PlayerHud : public sf::Drawable, public sf::Transformable
{
public:
    PlayerHud(int faceNumber, sf::Texture *faces);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private:
    sf::Sprite spriteFace;
};



#endif // PLAYERHUD_H
