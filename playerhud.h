#ifndef PLAYERHUD_H
#define PLAYERHUD_H
#include <iostream>
#include <set>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "boardelems.h"
#include "textureholder.h"
#include "elemsdescription.h"
#include "guiwindow.h"

static int startPlayers[4] = {0,15,240,255};

class PlayerHud : public sf::Drawable, public sf::Transformable
{
public:
    PlayerHud();
    PlayerHud(TextureHolder *textures, int faceNumber, sf::Font *gameFont, int tileSize, int pos);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::set<int> getNeighbours();

    int pos;
    int cash;
    int energy;
    int food;
    bool active;
    BoardElems elems;
    void setActive(bool newState);
    std::vector<int> properties;

    void addElem(int pos, int type);


    void updatePlayer();
    std::string getElem(sf::Vector2f mousePosition);
    std::map<std::string, sf::RectangleShape> buttons;



private:

    sf::Sprite spriteFace;
    sf::Text txtCash;
    sf::Text txtEnergy;
    sf::Text txtFood;
    sf::Text txtNextRound;
    int faceSize;
    int tileSize;
    TextureHolder *textures;
    sf::RectangleShape rectangle;
    sf::RectangleShape rectangle2;
};



#endif // PLAYERHUD_H
