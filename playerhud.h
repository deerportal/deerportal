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
#include "character.h"
#include "data.h"



class PlayerHud : public sf::Drawable, public sf::Transformable
{
public:
    PlayerHud();
    PlayerHud(TextureHolder *textures,  sf::Font *gameFont, int tileSize, int pos);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::set<int> getNeighbours();
    int pos;
    int cash;
    int energy;
    int food;
    int faith;
    bool active;
    BoardElems elems;
    void setActive(bool newState);
    std::vector<int> properties;

    void updatePlayer();
    std::string getElem(sf::Vector2f mousePosition);
    std::map<std::string, sf::RectangleShape> buttons;
    void updateTxt();

    std::vector<Character> characters;

    void update(sf::Time deltaTime);
    void play();

    std::set<int> getBusy();
    std::array<int,2> getMovements(int diceResult);
    void setFigurePos(int pos);
    bool done;

    int frozenLeft;

private:
    sf::Sprite spriteFace;
    sf::Text txtCash;
    sf::Text txtEnergy;
    sf::Text txtFood;
    sf::Text txtFaith;
    sf::Text txtNextRound;
    int faceSize;
    int tileSize;
    TextureHolder *textures;
    sf::RectangleShape rectangle;
    sf::RectangleShape rectangle2;

    sf::Sprite symbol;

    std::set<int> getTerrainSet();

};


#endif // PLAYERHUD_H
