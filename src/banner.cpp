#include "banner.h"

Banner::Banner(sf::Font *gameFont):
    active(false),
    textStr(""),
    timeDownCounter(0)
{

    text.setFont(*gameFont);
    text.setCharacterSize(100);
}

void Banner::setText(const std::string& newText)
{
    textStr = newText;
    text.setString(textStr);
    active = true;
    timeDownCounter = 1.5;
    // 204,76
    // 840, 718
    //    In [1]: 840 - 204
    //    Out[1]: 636

    //    In [2]: 718 - 76
    //    Out[2]: 642

    //    In [3]:


    sf::FloatRect rectTxt = text.getLocalBounds();
    text.setPosition(((636/2)-(rectTxt.width/2))+204,((642/2)-(rectTxt.height/2))+20);


}

void Banner::update(sf::Time frameTime)
{
    if (active){
        if (timeDownCounter>0){
            timeDownCounter -= frameTime.asSeconds();
        }

        if (timeDownCounter<=0){
            active = false;
            setText("");
        }
    }
}

void Banner::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(text, states);
}
