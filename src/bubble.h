#ifndef BUBBLE_H
#define BUBBLE_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <array>
#include "filetools.h"

enum BubbleState {DICE=0, MOVE=1};

class Bubble: public sf::Drawable, public sf::Transformable
{
public:
    Bubble();

public:
    sf::Texture textureDice;
    sf::Texture textureFootSteps;
    std::unique_ptr<sf::Sprite> spriteDice;
    std::unique_ptr<sf::Sprite> spriteFootSteps;

    std::array<std::unique_ptr<sf::Sprite>,2> spritesBubbles;
    BubbleState state;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime);
    float timeCounter;
    float posY;
    void setPosition(float x, float y);

};

#endif // BUBBLE_H
