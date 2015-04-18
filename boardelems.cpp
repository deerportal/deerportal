#include "boardelems.h"

BoardElems::BoardElems()
{
}
void BoardElems::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    // apply the transform
    states.transform *= getTransform();
    // apply the tileset texture

    // draw the vertex array
    for (BoardElem i: items)
    {
        target.draw(i, states);
//        std::cout << "POS: "<< i.getPosition().x << " " << i.getPosition().y << std::endl;
    }


}
