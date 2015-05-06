#include "grouphud.h"
#include "data.h"

namespace efc {

GroupHud::GroupHud(sf::Font *gameFont)
{
    seasonName.setFont(*gameFont);

}

GroupHud::GroupHud(){

}

void GroupHud::setFont(sf::Font *gameFont) {
    move(250,2);
    seasonName.setFont(*gameFont);
    roundName.setFont(*gameFont);
    monthName.setFont(*gameFont);

    seasonName.setCharacterSize(10);
    roundName.setCharacterSize(10);
    monthName.setCharacterSize(10);
    roundName.move(100, 0);
    monthName.move(200, 0);
    setMonthName(1);
}

void GroupHud::setSeason(int seasonNumber){
   seasonName.setString("Season: " + efc::seasonsNames[seasonNumber]);

};

void GroupHud::setRoundName(int roundNumber){
   roundName.setString("Round: " + std::to_string(roundNumber));

};

void GroupHud::setMonthName(int monthNumber){
   monthName.setString("Month: " + std::to_string(monthNumber));

};
void GroupHud::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    states.transform *= getTransform();


        target.draw(seasonName, states);
        target.draw(roundName, states);
        target.draw(monthName, states);


}
}

