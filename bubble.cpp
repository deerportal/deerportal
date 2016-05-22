#include "bubble.h"

Bubble::Bubble():
    state(BubbleState::DICE)
{
    spritesBubbles = {{spriteDice, spriteFootSteps}};

    if (!textureDice.loadFromFile("assets/img/bubble_dice.png"))
        std::exit(1);

    if (!textureFootSteps.loadFromFile("assets/img/bubble_footsteps.png"))
        std::exit(1);

    spritesBubbles[0].setTexture(textureDice);
    spritesBubbles[1].setTexture(textureFootSteps);

//    sprites[0] = spriteDice;
//    sprites[1] = spriteFootSteps;
//
}


void Bubble::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(spritesBubbles[state], states);
}
