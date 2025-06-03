#ifndef ELEM_H
#define ELEM_H
#include <SFML/Graphics.hpp>
#include "data.h"
#include "tilemap.h"

class Elem: public sf::Drawable, public sf::Transformable
{
public:
    Elem();
    void setBoardPosition(int boardPosition);
    int getBoardPosition() const;
    int boardPosition;
};

#endif // ELEM_H
