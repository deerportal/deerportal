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
    seasonName.setFont(*gameFont);
    roundName.setFont(*gameFont);
    monthName.setFont(*gameFont);
    seasonName.setColor(sf::Color(255,255,255));
    seasonName.setCharacterSize(30);
    roundName.setCharacterSize(30);
    monthName.setCharacterSize(30);
    roundName.setColor(sf::Color(255,255,250));
//    monthName.move(400, 0);
    setMonthName(1);
}

void GroupHud::setSeason(int seasonNumber){
   seasonName.setString("Season: " + efc::seasonsNames[seasonNumber]);
   setPosition(440,12);


}

void GroupHud::setDeerModeActive(){
   seasonName.setString("Deer Mode");
   setPosition(455,12);


}
void GroupHud::setRoundName(int roundNumber){
   roundName.setString("Round: " + std::to_string(roundNumber));
   roundName.setPosition(30, 700);

}



void GroupHud::setDeerModeCounter(int deerModeCounter)
{
    roundName.setString(std::to_string(deerModeCounter));
    roundName.setPosition(50, 700);
}

//void

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

