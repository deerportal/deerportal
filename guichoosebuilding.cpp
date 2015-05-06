#include "guichoosebuilding.h"
#include "tilemap.h"

GuiChooseBuilding::GuiChooseBuilding(TextureHolder *textures)
    :GuiWindow(textures),
      textureTiles(textures->textureTiles)
{
    //    this->textureTiles = &textures->textureGui;
    //    std::cout << "constructor" <<std::endl;
    addBuilding("elem_651", 651);
    addBuilding("elem_442", 442);
    addBuilding("elem_585", 585);
    addBuilding("elem_1100", 1100);


    description = "Description";
    guiElemTxt.setFont(guiElemFont);
    guiElemTxt.setCharacterSize(10);
    guiElemTxt.setColor(sf::Color(255,255,255,200));
    updateDescription();
    guiElemTxt.move(2,50);
    descriptionActive = false;
    rectangle.setSize(sf::Vector2f(400,200));
    bgdDark.setTextureRect((sf::IntRect)rectangle.getLocalBounds());
    spriteClose.setPosition(sf::Vector2f(rectangle.getLocalBounds().width-16,0));


}

void GuiChooseBuilding::updateDescription() {
    guiElemTxt.setString(description);
}

void GuiChooseBuilding::setDescriptionTxt(std::string newDescription) {
    description = newDescription;
    updateDescription();

}

void GuiChooseBuilding::addBuilding(std::string name, int pos){
    sf::Sprite newBuilding;

    newBuilding.setTexture(textureTiles);
    newBuilding.setTextureRect(efc::transPosIntoRect(pos));
    sf::Vector2f mod((buildings.size()*efc::TILE_SIZE)+4*(buildings.size()+1)+4,efc::TILE_SIZE);
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
        if (descriptionActive)
            target.draw(guiElemTxt, states2);
    }
}

std::string GuiChooseBuilding::getElem(sf::Vector2f mousePosition) {
    std::string result = GuiWindow::getElem(mousePosition);
    if (result=="close_gui")
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
            return i.first;
        }
    }
    return result;
}

