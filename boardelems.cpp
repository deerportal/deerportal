#include "boardelems.h"

BoardElems::BoardElems()
{
    active = false;
}



void BoardElems::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    states.transform *= getTransform();



    for (const efc::BoardElem &i: items)
    {
       target.draw(i, states);
        std::vector<int>  neighbours(efc::getNeighbours(i.pos));
        if (active==true)
        {
            for (int j: neighbours)
            {
                if (items_map.count(j)==0)
                {
                    sf::RectangleShape sprite(efc::createNeighbour(j));
                    target.draw(sprite, states);
                }
            }
        }
//        std::cout << "POS: "<< i.getPosition().x << " " << i.getPosition().y << std::endl;
    }


}
