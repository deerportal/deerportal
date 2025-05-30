#ifndef CREDITS_H
#define CREDITS_H
#include <SFML/Graphics.hpp>


class Credits: public sf::Drawable, public sf::Transformable
{
public:
    Credits(sf::Font *gameFont);
    void update(sf::Time deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::unique_ptr<sf::Text> text;
    float timeDownCounter;

    std::string textStr;
    std::array<std::string,4> creditsTxt;
    float txtInterval;
    float timeCounter;

    enum state {
        state_nothing,
        state_fade_in,
        state_text,
        state_fade_out
    };
    state txtState;

    int txtNumber;

    void setTxt(int number);

    void changeState();
};

#endif // CREDITS_H
