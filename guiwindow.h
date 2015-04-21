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
    void setTextureHolder(TextureHolder *textures);

    sf::Texture* textureGui;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void init();
    virtual std::string getElem(sf::Vector2f mousePosition);



private:
    sf::Sprite spriteClose;

};

#endif // GUIWINDOW_H
