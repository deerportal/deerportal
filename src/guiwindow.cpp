#include "data.h" // For ASSETS_PATH
#include "guiwindow.h"
#include "filetools.h" // For get_full_path
#include <iostream> // For std::cerr

void GuiWindow::setTitle(std::string newTitle) {
    title = newTitle;
    if (guiTitleTxt) { // Only set if guiTitleTxt is valid
        guiTitleTxt->setString(title);
    }
}

GuiWindow::GuiWindow(TextureHolder *textures)
{
    if (!guiElemFont.openFromFile(get_full_path(std::string(ASSETS_PATH) + "fnt/metal-macabre.regular.ttf"))) {
        std::cerr << "Failed to load font metal-macabre.regular.ttf" << std::endl;
        std::exit(1);
    }

    // Initialize text and sprites that depend on loaded resources
    guiTitleTxt = std::make_unique<sf::Text>(guiElemFont); // Pass font here
    bgdDark = std::make_unique<sf::Sprite>(textures->backgroundDark);
    // spriteClose would also need its texture set here if it's from TextureHolder
    // if (textures->textureClose) spriteClose = std::make_unique<sf::Sprite>(*textures->textureClose); 
    // For now, assuming spriteClose is handled by derived classes or needs its own texture loading.
    if (!textureClose.loadFromFile(get_full_path(std::string(ASSETS_PATH) + "img/gui/close.png"))) {
        std::cerr << "Failed to load texture assets/img/gui/close.png" << std::endl;
        spriteClose = nullptr;
    } else {
        spriteClose = std::make_unique<sf::Sprite>(this->textureClose);
    }

    title = "Choose building:"; // Default title
    guiTitleTxt->setCharacterSize(58);
    guiTitleTxt->setFillColor(sf::Color(255, 255, 255, 200));
    guiTitleTxt->setOutlineColor(sf::Color(255, 255, 255, 200));
    guiTitleTxt->move(sf::Vector2f(2,0));
    guiTitleTxt->setString(title);
    setPosition(sf::Vector2f(150, 100));
}

GuiWindow::GuiWindow()
{
    if (!guiElemFont.openFromFile(get_full_path(std::string(ASSETS_PATH) + "fnt/metal-macabre.regular.ttf"))) {
        std::cerr << "Failed to load font metal-macabre.regular.ttf" << std::endl;
        std::exit(1);
    }
    guiTitleTxt = std::make_unique<sf::Text>(guiElemFont); // Pass font here

    // Load texture for bgdDark (member textureBgdDark)
    if (!textureBgdDark.loadFromFile(get_full_path(std::string(ASSETS_PATH) + "img/bgd-dark.png"))) {
        std::cerr << "Failed to load texture assets/img/bgd-dark.png" << std::endl;
        // bgdDark will remain nullptr (default unique_ptr state)
    } else {
        bgdDark = std::make_unique<sf::Sprite>(this->textureBgdDark);
    }

    // Load texture for spriteClose (member textureClose)
    if (!textureClose.loadFromFile(get_full_path(std::string(ASSETS_PATH) + "img/gui/close.png"))) {
        std::cerr << "Failed to load texture assets/img/gui/close.png" << std::endl;
        // spriteClose will remain nullptr (default unique_ptr state)
    } else {
        spriteClose = std::make_unique<sf::Sprite>(this->textureClose);
    }
    
    setPosition(sf::Vector2f(150, 100));
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
    if(bgdDark) target.draw(*bgdDark, states2);
    if(spriteClose) target.draw(*spriteClose, states2);
    if(guiTitleTxt) target.draw(*guiTitleTxt, states2);
}

std::string GuiWindow::getElem(sf::Vector2f mousePosition)
{
    sf::Vector2f hoverPos = getPosition();
    if (!spriteClose) return "";
    sf::FloatRect spriteBounds = spriteClose->getLocalBounds();
    sf::FloatRect closeRect(spriteClose->getPosition(), spriteBounds.size);
    if (closeRect.contains(mousePosition - hoverPos))
        return "close_gui";
    else
        return "";
}
