#include "boardelems.h"

BoardElems::BoardElems()
{
    active = false;
    displayNeighbours = true;
}



void BoardElems::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    std::set<int> neighboursAll;

    states.transform *= getTransform();
    sf::BlendMode blendmode = sf::BlendAlpha;

    states.blendMode = blendmode;

    for (const DP::BoardElem &i: items)
    {
       target.draw(i, states);
        std::set<int>  neighbours(DP::getNeighbours(i.pos));
        if ((active==true) && (displayNeighbours==true))
        {
            for (int j: neighbours)
            {
                if (items_map.count(j)==0)
                {
                    neighboursAll.insert(j);
//                    sf::RectangleShape sprite(DP::createNeighbour(j));
//                    target.draw(sprite, states);
                }
            }
        }
    }
    if (active==true)
    {
        for (int j: neighboursAll)
        {
            if (items_map.count(j)==0)
            {
                sf::RectangleShape sprite(DP::createNeighbour(j));
                target.draw(sprite, states);
            }
        }
    }

}
