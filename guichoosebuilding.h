#ifndef GUICHOOSEBUILDING_H
#define GUICHOOSEBUILDING_H
#include "guiwindow.h"

class GuiChooseBuilding : public GuiWindow
{
public:
    GuiChooseBuilding(TextureHolder *textures);
    std::map<std::string, sf::Sprite> buildings;
    void addBuilding(std::string name, int pos);
    const sf::Texture &textureTiles;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::string getElem(sf::Vector2f mousePosition);


};

#endif // GUICHOOSEBUILDING_H
