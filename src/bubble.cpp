#include <cmath>
#include <array>
#include "bubble.h"


Bubble::Bubble():
    state(BubbleState::DICE),
    timeCounter(0),
    posY(0)
{
    spritesBubbles = {{spriteDice, spriteFootSteps}};

    if (!textureDice.loadFromFile(ASSETS_PATH"assets/img/bubble_dice.png"))
        std::exit(1);

    if (!textureFootSteps.loadFromFile(ASSETS_PATH"assets/img/bubble_footsteps.png"))
        std::exit(1);

    spritesBubbles[0].setTexture(textureDice);
    spritesBubbles[1].setTexture(textureFootSteps);

}

void  Bubble::setPosition(float x, float y) {
    Transformable::setPosition(x, y);
    posY = y;
}


void Bubble::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(spritesBubbles[state], states);
}

void Bubble::update(sf::Time deltaTime)
{

    timeCounter += deltaTime.asSeconds()*10;
    float modifier = sin(timeCounter)*5;
    Transformable::setPosition(getPosition().x, posY+modifier);
}
