#ifndef BUBBLE_H
#define BUBBLE_H

#include <SFML/Graphics.hpp>

enum BubbleState {DICE, MOVE};

class Bubble: public sf::Drawable, public sf::Transformable
{
public:
    Bubble();

private:
//    sf::Texture textureDice;
//    sf::Texture textureFootSteps;
//    sf::Sprite spriteDice;
//    sf::Sprite spriteFootSteps;

    std::array<sf::Sprite,2> sprites;
    BubbleState state;


};

#endif // BUBBLE_H
