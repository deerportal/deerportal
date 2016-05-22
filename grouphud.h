#ifndef GROUPHUD_H
#define GROUPHUD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace efc {

class GroupHud: public sf::Drawable, public sf::Transformable
{
public:
    explicit GroupHud( sf::Font *gameFont);
    GroupHud();
    void setFont(sf::Font *gameFont);
    sf::Text seasonName;
    sf::Text roundName;
    sf::Text monthName;


    void setSeason(int seasonNumber);
    void setRoundName(int roundNumber);
    void setMonthName(int monthNumber);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

}
#endif // GROUPHUD_H
