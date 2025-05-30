#ifndef GROUPHUD_H
#define GROUPHUD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace DP {
/*!
 * \brief The GroupHud is a drawable type describing a season and a round.
 */
class GroupHud: public sf::Drawable, public sf::Transformable
{
public:
    GroupHud();
    void setFont(sf::Font *gameFont);
    std::unique_ptr<sf::Text> seasonName;
    std::unique_ptr<sf::Text> roundName;
    std::unique_ptr<sf::Text> monthName;

    void setSeason(int seasonNumber);
    void setRoundName(int roundNumber);
    void setMonthName(int monthNumber);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void setDeerModeCounter(int deerModeCounter);
    void setDeerModeActive();
};

}
#endif // GROUPHUD_H
