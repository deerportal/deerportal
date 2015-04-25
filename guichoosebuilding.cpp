#include "guichoosebuilding.h"
#include "tilemap.h"

GuiChooseBuilding::GuiChooseBuilding(TextureHolder *textures)
    :GuiWindow(textures),
      textureTiles(textures->textureTiles)
{
    //    this->textureTiles = &textures->textureGui;
    std::cout << "constructor" <<std::endl;
    addBuilding("build_8", 8);
    addBuilding("build_88", 88);
    addBuilding("build_328", 328);
}

void GuiChooseBuilding::addBuilding(std::string name, int pos){
    sf::Sprite newBuilding;

    newBuilding.setTexture(textureTiles);
    newBuilding.setTextureRect(efc::transPosIntoRect(pos));
    sf::Vector2f mod((buildings.size()*efc::TILE_SIZE),efc::TILE_SIZE);
    newBuilding.move(mod);
    buildings.insert({name, newBuilding});
//    std::cout << "size " << buildings.size() <<std::endl;
}

void GuiChooseBuilding::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    if (active==true)
    {
        sf::RenderStates states2 = states;
        states2.transform *= getTransform();
        GuiWindow::draw(target, states);
        for (std::pair<std::string, sf::Sprite> i: buildings)
        {
            target.draw(i.second, states2);
        }
    }
}

std::string GuiChooseBuilding::getElem(sf::Vector2f mousePosition) {
    std::string result = GuiWindow::getElem(mousePosition);
    if (result=="close")
        return result;
    sf::Vector2f hoverPos = getPosition();
    for (std::pair<std::string, sf::Sprite> i: buildings)
    {
        sf::FloatRect spriteBounds = i.second.getLocalBounds();
        sf::FloatRect closeRect;
        closeRect.left = i.second.getPosition().x;
        closeRect.top = i.second.getPosition().y;
        closeRect.width = spriteBounds.width;
        closeRect.height = spriteBounds.height;
        if (closeRect.contains(mousePosition.x - hoverPos.x,mousePosition.y - hoverPos.y))
        {
            active = false;
            return i.first;
        }
    }
    return result;
}

