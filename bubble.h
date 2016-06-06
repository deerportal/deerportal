#ifndef BUBBLE_H
#define BUBBLE_H
#include <array>
#include <SFML/Graphics.hpp>

enum BubbleState {DICE=0, MOVE=1};

class Bubble: public sf::Drawable, public sf::Transformable
{
public:
    Bubble();

public:
    sf::Texture textureDice;
    sf::Texture textureFootSteps;
    sf::Sprite spriteDice;
    sf::Sprite spriteFootSteps;

    std::array<sf::Sprite,2> spritesBubbles;
    BubbleState state;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime);
    float timeCounter;
    float posY;
    void setPosition(float x, float y);

};

#endif // BUBBLE_H
