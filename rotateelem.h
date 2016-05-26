#ifndef ROTATEELEM_H
#define ROTATEELEM_H
#include <SFML/Graphics.hpp>
#include "data.h"
class RotateElem : public sf::Drawable, public sf::Transformable
{
public:
    RotateElem();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime);

    sf::Texture textureRotate;
    sf::Sprite spriteRotate;
    void setColor(int playerNumber);

    float timeCounter;
    bool active;
    void reset();
};

#endif // ROTATEELEM_H
