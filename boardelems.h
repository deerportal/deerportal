#ifndef BOARDELEMS_H
#define BOARDELEMS_H
#include "boardelem.h"
#include "elemsdescription.h"

class BoardElems: public sf::Drawable, public sf::Transformable
{
public:
    BoardElems();
    std::vector<efc::BoardElem> items;
    std::map<int, efc::BoardElem> items_map;
    bool active;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif // BOARDELEMS_H
