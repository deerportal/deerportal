#include <array>
#include "credits.h"
#include <iostream>

Credits::Credits(sf::Font *gameFont):
    txtInterval(2),
    timeCounter(0),
    txtState(state_nothing),
    txtNumber(0)
{

    creditsTxt[0] = "    devcarpet proudly presents   ";
    creditsTxt[1] = "game design, code, audio: bluszcz";
    creditsTxt[2] = "game design, visual art: ukata";
    creditsTxt[3] = "game design, tests: igor & olaf";

    text.setFont(*gameFont);

    text.setCharacterSize(30);
    setTxt(0);
    text.setString("asdasd");
    text.setFont(*gameFont);

    text.setFillColor(sf::Color(sf::Color::White));
}

void Credits::setTxt(int number)
{
    text.setString(creditsTxt[number]);
    sf::FloatRect rectTxt = text.getLocalBounds();
    text.setPosition(((1360/2)-(rectTxt.width/2)),((768*0.85)-(rectTxt.height/2)));
}

void Credits::update(sf::Time frameTime)
{
            timeCounter += frameTime.asSeconds();
            if (timeCounter>txtInterval){
                changeState();
            }

            float ratio = 255/2;

            if (txtState==state_fade_in)
            {
                text.setFillColor(sf::Color(255,255,255,ratio*timeCounter));
            } else  if (txtState==state_fade_out)
            {
                text.setFillColor(sf::Color(255,255,255,255-(ratio*timeCounter)));
            } else  if (txtState==state_text)
            {
                text.setFillColor(sf::Color(255,255,255,255));
            }

}

void Credits::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    if (txtState!=state_nothing)
    {
        states.transform *= getTransform();
        target.draw(text, states);
    }


}

void Credits::changeState(){
    timeCounter = 0;


    if (txtState==state_nothing)
    {
        txtState = state_fade_in;
        return ;
    }

    if (txtState==state_fade_in)
    {
        txtState = state_text;
        return ;
    }
    if (txtState==state_text)
    {
        txtState = state_fade_out;
        return ;
    }
    if (txtState==state_fade_out)
    {
        txtState = state_nothing;
        txtNumber += 1;

        if  (txtNumber>3)
        {
            txtNumber = 0;
        }
        setTxt(txtNumber);
        return ;
    }
}
