#include "bubble.h"

Bubble::Bubble():
    state(BubbleState::DICE)
{
    sf::Texture texture;
    sf::Sprite sprite;

    if (!texture.loadFromFile("assets/img/bubble_dice.png"))
        std::exit(1);

    sprite.setTexture(texture);
    sprites[0] = sprite;

    if (!texture.loadFromFile("assets/img/bubble_footsteps.png"))
        std::exit(1);
    sprite.setTexture(texture);
    sprites[1] = sprite;
}
