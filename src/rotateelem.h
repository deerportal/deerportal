#ifndef ROTATEELEM_H
#define ROTATEELEM_H
#include <SFML/Graphics.hpp>
#include "data.h"
#include "filetools.h"

class TextureHolder; // Forward declaration

class RotateElem : public sf::Drawable, public sf::Transformable
{
public:
    RotateElem(TextureHolder *textures);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime);

    sf::Texture textureRotate;
    std::unique_ptr<sf::Sprite> spriteRotate;
    void setColor();

    float timeCounter;
    bool active;
    void reset();
};

#endif // ROTATEELEM_H
