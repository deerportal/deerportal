#include "rotateelem.h"

RotateElem::RotateElem():
    timeCounter(0),
    active(true)
{
    if (!textureRotate.loadFromFile(ASSETS_PATH"assets/img/rotate.png"))
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
	const float timestep = 1.0f / 60.0f;
    speed = 2.0f;
	float accumulator = 0;
	accumulator += deltaTime.asSeconds();
	spriteRotate.rotate(speed * accumulator / timestep);

    timeCounter += deltaTime.asSeconds();

    if (timeCounter>0.40f)
        if (timeCounter>0.40f)
    {
        if (active)
        {
            active = false;

        } else {
            active = true;
        };
        timeCounter = 0;
    }



}

void RotateElem::setColor()
{
//    spriteRotate.setColor(DP::playersColors[playerNumber]);
    spriteRotate.setColor(sf::Color::White);
}

void RotateElem::reset()
{
    active = true;
    timeCounter = 0;
}
