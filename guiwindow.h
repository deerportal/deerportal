#ifndef GUIWINDOW_H
#define GUIWINDOW_H
#include <iostream>
#include "hover.h"
#include "textureholder.h"

class GuiWindow: public Hover
{
public:
    GuiWindow(TextureHolder *textures);
    GuiWindow();
//    void setTextureHolder(TextureHolder *textures);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void init();
    virtual std::string getElem(sf::Vector2f mousePosition);
    sf::Font guiElemFont;
    std::string title;
    std::string description;
    sf::Text guiTitleTxt;
    sf::Texture* textureGui;

    sf::Sprite bgdDark;

    void setTitle(std::string newTitle);
    sf::Sprite spriteClose;

private:



};

#endif // GUIWINDOW_H
