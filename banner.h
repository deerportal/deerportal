#ifndef BANNER_H
#define BANNER_H
#include <string>
#include <SFML/Graphics.hpp>

class Banner: public sf::Drawable, public sf::Transformable
{
public:
    Banner(sf::Font *gameFont);
    void setText(std::string newText);
    bool active;

    std::string textStr;
    sf::Text text;
    float timeDownCounter;
    void update(sf::Time frameTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // BANNER_H
