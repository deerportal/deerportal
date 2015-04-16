#ifndef SELECTOR_H
#define SELECTOR_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Selector : public sf::Drawable, public sf::Transformable
{
public:
    Selector(int squareSize);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    int squareSize;
};

#endif // SELECTOR_H
