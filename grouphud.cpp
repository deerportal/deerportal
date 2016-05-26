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
    move(440,12);
    seasonName.setFont(*gameFont);
    roundName.setFont(*gameFont);
    monthName.setFont(*gameFont);
    seasonName.setColor(sf::Color(255,255,255));
    seasonName.setCharacterSize(30);
    roundName.setCharacterSize(30);
    monthName.setCharacterSize(30);
    roundName.setColor(sf::Color(255,255,250));
    roundName.move(30, 700);
//    monthName.move(400, 0);
    setMonthName(1);
}

void GroupHud::setSeason(int seasonNumber){
   seasonName.setString("Season: " + efc::seasonsNames[seasonNumber]);

}

void GroupHud::setRoundName(int roundNumber){
   roundName.setString("Round: " + std::to_string(roundNumber));

}

void GroupHud::setMonthName(int monthNumber){
   monthName.setString("Month: " + std::to_string(monthNumber));

}
void GroupHud::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    states.transform *= getTransform();


        target.draw(seasonName, states);
        target.draw(roundName, states);
//        target.draw(monthName, states);


}
}

