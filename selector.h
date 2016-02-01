#ifndef SELECTOR_H
#define SELECTOR_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "data.h"

class Selector : public sf::Drawable, public sf::Transformable
{
public:
    Selector(int squareSize);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void changeColor(int colorNumber);

    int squareSize;
private:
    sf::RectangleShape rectangle;
};

#endif // SELECTOR_H
