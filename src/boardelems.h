#ifndef BOARDELEMS_H
#define BOARDELEMS_H
#include "boardelem.h"
#include "elemsdescription.h"

class BoardElems : public sf::Drawable, public sf::Transformable {
public:
  BoardElems();
  std::vector<DP::BoardElem> items;
  std::map<int, DP::BoardElem> items_map;
  bool active;
  bool displayNeighbours;

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // BOARDELEMS_H
