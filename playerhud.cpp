#include "playerhud.h"
#include "textureholder.h"
#include "boardelem.h"


void PlayerHud::addElem(int pos, int type) {
    efc::BoardElem startElem(textures, pos,type);
    elems.items.push_back(startElem);
    elems.items_map.insert({pos, startElem});
}

std::set<int> PlayerHud::getNeighbours(){

    std::set<int> neighbours;

    for (std::pair<int, efc::BoardElem> i: elems.items_map)
    {
     std::cout << "adding " << i.first << std::endl;

    std::vector<int>  neighboursVector(efc::getNeighbours(i.second.pos));
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


PlayerHud::PlayerHud()
{
    active = false;
}

void PlayerHud::setActive(bool newState){
    active = newState;
    elems.active = newState;

}

PlayerHud::PlayerHud(TextureHolder *textures, int faceNumber,  sf::Font *gameFont, int faceSize, int pos)
{
    active = false;
    int startPositions[4] = {0,15,240, 255};
    this->textures = textures;
    efc::BoardElem startElem(textures, startPositions[pos],441);
    elems.items.push_back(startElem);
    elems.items_map.insert({pos, startElem});
    this->faceSize = faceSize;
    spriteFace.setTexture(textures->textureFaces);
    this->pos = pos;

    food = 0;
    cash = 0;
    energy = 0;

    int x = faceNumber % 10;
    int y = (int) faceNumber /10;

    txtCash.setFont(*gameFont);
    txtFood.setFont(*gameFont);
    txtEnergy.setFont(*gameFont);

    txtCash.setPosition(1,(pos*100)+40);
    txtCash.setString("Cash: " + std::to_string(cash));
    txtCash.setCharacterSize(10);
    txtCash.setScale(sf::Vector2f(0.25f, 1.f));

    txtFood.setPosition(1,(pos*100)+55);
    txtFood.setString("Food: " + std::to_string(food));
    txtFood.setCharacterSize(10);
    txtFood.setScale(sf::Vector2f(0.25f, 1.f));

    txtEnergy.setPosition(1,(pos*100)+70);
    txtEnergy.setString("Enrg: " + std::to_string(energy));
    txtEnergy.setString("Enrg: " + std::to_string(energy));
    txtEnergy.setCharacterSize(10);
    txtEnergy.setScale(sf::Vector2f(0.25f, 1.f));

    std::cout << "playerHud" << faceNumber << " " << x << " " << y << std::endl;

    spriteFace.setTextureRect(sf::IntRect(x*faceSize, y*faceSize, faceSize, faceSize));
    spriteFace.setScale(sf::Vector2f(0.25f, 1.f));
    spriteFace.setPosition(0,pos*100);
}

void PlayerHud::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //   // apply the transform
    states.transform *= getTransform();
    // Color rectangles making the gui on the right side
    sf::RectangleShape rectangle(sf::Vector2f(faceSize, faceSize));
    sf::RectangleShape rectangle2(sf::Vector2f(faceSize, (faceSize*2)+3));
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

    target.draw(rectangle, states);
    target.draw(rectangle2, states);
    target.draw(txtCash, states);
    target.draw(txtFood, states);
    target.draw(txtEnergy, states);
    target.draw(spriteFace, states);
}
