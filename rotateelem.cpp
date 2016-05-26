#include "rotateelem.h"

RotateElem::RotateElem()
{
    if (!textureRotate.loadFromFile("assets/img/rotate.png"))
        std::exit(1);

    spriteRotate.setTexture(textureRotate);
    spriteRotate.scale(0.7f,0.7f);
    spriteRotate.setOrigin(32, 32);

}

void RotateElem::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();
target.draw(spriteRotate, states);
}
void RotateElem::update(sf::Time deltaTime)
{

    float speed;

    speed = 0.05f/deltaTime.asSeconds();

    spriteRotate.rotate(1.0f*speed);
}

void RotateElem::setColor(int playerNumber)
{
//    spriteRotate.setColor(efc::playersColors[playerNumber]);
    spriteRotate.setColor(sf::Color::White);
}
