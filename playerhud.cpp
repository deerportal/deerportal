#include "playerhud.h"
#include "textureholder.h"
#include "boardelem.h"

bool PlayerHud::addElem(int pos, int type) {
    int price = textures->tilesDescription[type][0];


    if (price<=cash)
    {
        efc::BoardElem startElem(textures, pos,type);
        startElem.setColor(efc::playersColors[this->pos]);
        elems.items.push_back(startElem);
        elems.items_map.insert({pos, startElem});

        cash -= price;
        updateTxt();
        return true;
    }
    return false;
}

std::set<int> PlayerHud::getNeighbours(){
    std::set<int> neighbours;
    for (std::pair<int, efc::BoardElem> i: elems.items_map)
    {

        std::set<int>  neighboursVector(efc::getNeighbours(i.second.pos));
        for (int j: neighboursVector)
        {
            if (elems.items_map.count(j) == 0)
            {
                neighbours.insert(j);
            }
        }
    }
    return neighbours;
}

void PlayerHud::updateTxt(){
    txtCash.setString(  "Cash: " + std::to_string(cash));
    txtFood.setString(  "Food: " + std::to_string(food));
    txtEnergy.setString("Enrg: " + std::to_string(energy));
    txtFaith.setString( "Gods: " + std::to_string(faith));

}


void PlayerHud::updatePlayer(){
    for (const efc::BoardElem &i: elems.items)
    {
        int cashUpd  = textures->tilesDescription[i.type][1];
        int foodUpd  = textures->tilesDescription[i.type][3];
        int enrgUpd  = textures->tilesDescription[i.type][5];
        int faithUpd  = textures->tilesDescription[i.type][7];

        cash += cashUpd;
        energy += enrgUpd;
        food += foodUpd;
        faith += faithUpd;
        updateTxt();
    }
}

PlayerHud::PlayerHud()
{
    active = false;
    food = 0;
    cash = 10;
    energy = 0;
    faith = 0;
}

void PlayerHud::setActive(bool newState){
    active = newState;
    elems.active = newState;
}

PlayerHud::PlayerHud(TextureHolder *textures, int faceNumber,  sf::Font *gameFont, int faceSize, int pos)
{
    active = false;
    this->textures = textures;
    efc::BoardElem startElem(textures, startPlayers[pos],444);
    startElem.setColor(efc::playersColors[pos]);

    elems.items.push_back(startElem);
    elems.items_map.insert({startPlayers[pos], startElem});
    this->faceSize = faceSize;
    spriteFace.setTexture(textures->textureFaces);
    this->pos = pos;

    food = 0;
    cash = 10;
    energy = 0;
    faith = 0;

    int x = faceNumber % 10;
    int y = (int) faceNumber /10;

    txtCash.setFont(*gameFont);
    txtFood.setFont(*gameFont);
    txtEnergy.setFont(*gameFont);
    txtFaith.setFont(*gameFont);

    txtNextRound.setFont(*gameFont);
    txtNextRound.setString("End Turn");
    txtNextRound.setScale(sf::Vector2f(0.25f, 1.f));
    txtNextRound.setCharacterSize(10);
    txtNextRound.setPosition(9,(pos*100)+10);

    txtCash.setPosition(1,(pos*100)+40);
//    txtCash.setString("Cash: " + std::to_string(cash));
    txtCash.setCharacterSize(10);
    txtCash.setScale(sf::Vector2f(0.25f, 1.f));

    txtFood.setPosition(1,(pos*100)+55);
//    txtFood.setString("Food: " + std::to_string(food));
    txtFood.setCharacterSize(10);
    txtFood.setScale(sf::Vector2f(0.25f, 1.f));

    txtEnergy.setPosition(1,(pos*100)+70);
//    txtEnergy.setString("Enrg: " + std::to_string(energy));
    txtEnergy.setCharacterSize(10);
    txtEnergy.setScale(sf::Vector2f(0.25f, 1.f));

    txtFaith.setPosition(1,(pos*100)+85);
//    txtEnergy.setString("Enrg: " + std::to_string(energy));
    txtFaith.setCharacterSize(10);
    txtFaith.setScale(sf::Vector2f(0.25f, 1.f));
    updateTxt();

    spriteFace.setTextureRect(sf::IntRect(x*faceSize, y*faceSize, faceSize, faceSize));
    spriteFace.setScale(sf::Vector2f(0.25f, 1.f));
    spriteFace.setPosition(0,pos*100);


    rectangle.setSize(sf::Vector2f(this->faceSize, this->faceSize));
    rectangle2.setSize(sf::Vector2f(this->faceSize, (this->faceSize*2)+3));

    if (pos==0)
    {
        rectangle.setFillColor(sf::Color(50, 50, 150,168));
        rectangle2.setFillColor(sf::Color(100, 100, 150,168));
        rectangle.setOutlineColor(sf::Color(0,0,128));
    }
    else if (pos==1)
    {
        rectangle.setFillColor(sf::Color(50, 150, 50,168));
        rectangle2.setFillColor(sf::Color(100, 150,100,168));
        rectangle.setOutlineColor(sf::Color(0,128,0));
    }

    else if (pos==2)
    {
        rectangle.setFillColor(sf::Color(150, 50, 50,168));
        rectangle2.setFillColor(sf::Color(150, 100,100,168));
        rectangle.setOutlineColor(sf::Color(128,0,0));
    }
    else if (pos==3)
    {
        rectangle.setFillColor(sf::Color(150, 150, 150,168));
        rectangle2.setFillColor(sf::Color(200, 200,200,168));
        rectangle.setOutlineColor(sf::Color(128,128,128));
    }


    rectangle.setOutlineThickness(1);
    rectangle.setPosition(0, pos*100);
    rectangle2.setPosition(0, (pos*100)+faceSize+1);

    buttons.insert({"end_turn",rectangle});

}

std::string PlayerHud::getElem(sf::Vector2f mousePosition) {
    std::string result = "";
    sf::Vector2f hoverPos = getPosition();
    for (std::pair<std::string, sf::RectangleShape> i: buttons)
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

void PlayerHud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //   // apply the transform
    states.transform *= getTransform();
    // Color rectangles making the gui on the right side
//    sf::RectangleShape rectangle(sf::Vector2f(faceSize, faceSize));
//    sf::RectangleShape rectangle2(sf::Vector2f(faceSize, (faceSize*2)+3));


    target.draw(rectangle, states);
    target.draw(rectangle2, states);
    target.draw(txtCash, states);
    target.draw(txtFood, states);
    target.draw(txtEnergy, states);
    target.draw(txtFaith, states);

    if (active)
        target.draw(txtNextRound, states);
    target.draw(spriteFace, states);
}
