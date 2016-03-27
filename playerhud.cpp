#include "playerhud.h"
#include "textureholder.h"
#include "boardelem.h"

bool PlayerHud::addElem(int pos, int type) {

    int price = textures->tilesDescription[type][0];
    return false;
}

std::set<int> PlayerHud::getTerrainSet(){
    std::set<int> terrain;
    for (int i: efc::terrainArray)
    {
        terrain.insert(i);
    }
    return terrain;

}
std::set<int> PlayerHud::getBusy(){

    std::set<int> busyTiles;
    for (std::pair<int, efc::BoardElem> i: elems.items_map)
    {
        busyTiles.insert(i.first);
    }

    return busyTiles;
}

std::set<int> PlayerHud::getNeighbours(){
    std::set<int> neighbours;
    for (std::pair<int, efc::BoardElem> i: elems.items_map)
    {
        std::set<int> terrain = getTerrainSet();

        std::set<int>  neighboursVector(efc::getNeighbours(i.second.pos));
        for (int j: neighboursVector)
        {
            if ((elems.items_map.count(j) == 0) && (terrain.count(j)==0))
            {
//                std::cout << j << " " << terrain.count(j) << std::endl;
                neighbours.insert(j);
            }
        }
    }

//    // Fill in s1 and s2 with values
//    std::set<int> result;
//    std::set_difference(neighbours.begin(), neighbours.end(), terrain.begin(), terrain.end(),
//                        std::inserter(result, result.end()));


//    for (int i: result)
//    {
//        std::cout << i << std::endl;
//    }
    return neighbours;
}

void PlayerHud::updateTxt(){
    txtCash.setString(std::to_string(cash));
//    txtFood.setString(  "Food: " + std::to_string(food));
//    txtEnergy.setString("Enrg: " + std::to_string(energy));
//    txtFaith.setString( "Gods: " + std::to_string(faith));

}


void PlayerHud::updatePlayer(){

    updateTxt();

}

PlayerHud::PlayerHud()
{

}

void PlayerHud::setActive(bool newState){
    active = newState;
    elems.active = newState;
    for (auto&& i: characters)
    {

        i.active = newState;
}
}

PlayerHud::PlayerHud(TextureHolder *textures, int faceNumber,  sf::Font *gameFont, int faceSize, int pos)
{
    static int startPlayers[4] = {0,15,240,255};


    done = false;

    active = false;
    this->textures = textures;
    Character character(this->textures, pos);
    characters.push_back(Character (this->textures, pos));
    efc::BoardElem startElem(textures, startPlayers[pos],444);
    startElem.setColor(efc::playersColors[pos]);

    elems.items.push_back(startElem);
    elems.items_map.insert({startPlayers[pos], startElem});
    this->faceSize = faceSize;
    spriteFace.setTexture(textures->textureFaces);
    this->pos = pos;


    symbol.setTexture(this->textures->textureSymbols);


    sf::IntRect symbolsRect[4] = {sf::IntRect(0,0,255,255), sf::IntRect(256,0,512,255), sf::IntRect(0,255, 255, 512), sf::IntRect(255,255,512, 512)};


    symbol.setTextureRect(symbolsRect[pos]);
    symbol.setScale(sf::Vector2f(0.2f, 0.20f));
    symbol.setColor(sf::Color(25, 25, 25, 105));
    symbol.setPosition(60, (pos*100)+40);


    food = 0;
    cash = 0;
    energy = 0;
    faith = 0;

    int x = faceNumber % 10;
    int y = (int) faceNumber /10;

    txtCash.setFont(*gameFont);
    txtCash.setCharacterSize(20);

    txtFood.setFont(*gameFont);
    txtEnergy.setFont(*gameFont);
    txtFaith.setFont(*gameFont);

    txtNextRound.setFont(*gameFont);
    txtNextRound.setString("End Turn");
    txtNextRound.setCharacterSize(12);
    txtNextRound.setPosition(40,(pos*100)+10);


    int posX1 = 80;
    int posX2 = 960;
    int posY1 = 184;
    int posY2 = 579;
    std::array<std::array<int,2>,4> textPos =
    {
        {
            {posX1,posY1}, {posX2,posY1},
            {posX1,posY2}, {posX2, posY2}
        }
    };

    txtCash.setPosition(textPos[pos][0],textPos[pos][1] );
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
    states.transform *= getTransform();
    target.draw(txtCash, states);
}
void PlayerHud::play()
{
    for (auto&& i: characters)
    {
        i.play();
    }
}
void PlayerHud::update(sf::Time deltaTime, std::set<int>& busyTiles)
{
    updateTxt();
    for (auto&& i: characters)
    {

        sf::Vector2f movement(0.f, 0.f);
        if  (i.currentAnimationIndex==efc::DIR_LEFT)
            movement = sf::Vector2f (-10.f, 0.f);
        else if  (i.currentAnimationIndex==efc::DIR_RIGHT)
            movement = sf::Vector2f (10.f, 0.f);
        else if  (i.currentAnimationIndex==efc::DIR_UP)
            movement = sf::Vector2f (0.f, -10.f);
        else if  (i.currentAnimationIndex==efc::DIR_DOWN)
            movement = sf::Vector2f (0.f, 10.f);

//        i.move(movement * deltaTime.asSeconds());
        i.update(deltaTime, busyTiles);


    }

}

std::array<int,2> PlayerHud::getMovements(int diceResult)
{
    return characters[0].getMovements(diceResult);
}

void PlayerHud::setFigurePos(int pos)
{
    characters[0].setBoardPosition(pos);
}
