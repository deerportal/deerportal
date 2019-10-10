#include "guiwindow.h"



void GuiWindow::setTitle(std::string newTitle) {
    title = newTitle;
    guiTitleTxt.setString(title);
}

GuiWindow::GuiWindow(TextureHolder *textures)
{
//    spriteClose.setTexture(textures->textureGui);
//    spriteClose.setTextureRect(sf::IntRect(0,0,16,16));
//    spriteClose.move(sf::Vector2f(150-16,0));
    if (!guiElemFont.loadFromFile(get_full_path(ASSETS_PATH"assets/fnt/metal-macabre.regular.ttf")))
    {
        std::exit(1);
    };


    bgdDark.setTexture(textures->backgroundDark);
//    bgdDark.setColor(sf::Color(160, 160, 160, 200));

    bgdDark.setTextureRect((sf::IntRect)rectangle.getLocalBounds());
    title = "Choose building:";
    guiTitleTxt.setFont(guiElemFont);
    guiTitleTxt.setCharacterSize(58);
    // guiTitleTxt.setColor(sf::Color(255,255,255,200));
	guiTitleTxt.setFillColor(sf::Color(255, 255, 255, 200));
	guiTitleTxt.setOutlineColor(sf::Color(255, 255, 255, 200));
    guiTitleTxt.move(2,0);
    guiTitleTxt.setString(title);
    setPosition(150, 100);
}

GuiWindow::GuiWindow(){
    if (!guiElemFont.loadFromFile(get_full_path(ASSETS_PATH"assets/fnt/metal-macabre.regular.ttf")))
    {
        std::exit(1);
    };
    setPosition(150, 100);
}

//void GuiWindow::setTextureHolder(TextureHolder *textures)
//{

////    spriteClose.setTexture(textures->textureGui);

//}


void GuiWindow::init(){
}

void GuiWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const{

    sf::RenderStates states2 = states;
    states2.transform *= getTransform();

    Hover::draw(target, states);
    target.draw(bgdDark, states2);
        target.draw(spriteClose, states2);
        target.draw(guiTitleTxt, states2);

}

std::string GuiWindow::getElem(sf::Vector2f mousePosition)
{
    sf::Vector2f hoverPos = getPosition();
    sf::FloatRect spriteBounds = spriteClose.getLocalBounds();
    sf::FloatRect closeRect;
    closeRect.left = spriteClose.getPosition().x;
    closeRect.top = spriteClose.getPosition().y;
    closeRect.width = spriteBounds.width;
    closeRect.height = spriteBounds.height;
    if (closeRect.contains(mousePosition.x - hoverPos.x,mousePosition.y - hoverPos.y))
    {
        return "close_gui";
    }
    else
        return "";
}
