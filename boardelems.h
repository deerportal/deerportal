#ifndef BOARDELEMS_H
#define BOARDELEMS_H
#include "boardelem.h"

class BoardElems: public sf::Drawable, public sf::Transformable
{
public:
    BoardElems();
    std::vector<BoardElem> items;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // BOARDELEMS_H
