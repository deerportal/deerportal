#include "playerhud.h"
#include "textureholder.h"
#include "boardelem.h"



std::set<int> Player::getTerrainSet(){
    std::set<int> terrain;
    for (int i: efc::terrainArray)
    {
        terrain.insert(i);
    }
    return terrain;

}
std::set<int> Player::getBusy(){

    std::set<int> busyTiles;
    for (std::pair<int, efc::BoardElem> i: elems.items_map)
    {
        busyTiles.insert(i.first);
    }

    return busyTiles;
}

std::set<int> Player::getNeighbours(){
    std::set<int> neighbours;
    for (std::pair<int, efc::BoardElem> i: elems.items_map)
    {
        std::set<int> terrain = getTerrainSet();
        std::set<int>  neighboursVector(efc::getNeighbours(i.second.pos));
        for (int j: neighboursVector)
        {
            if ((elems.items_map.count(j) == 0) && (terrain.count(j)==0))
            {
                neighbours.insert(j);
            }
        }
    }

    return neighbours;
}

void Player::updateTxt(){
    txtCash.setString(std::to_string(cash));
}


void Player::updatePlayer(){
    updateTxt();
}

Player::Player():
    frozenLeft(0),
    reachedPortal(false),
    reachedPortalFirst(false),
    human(false)
{
    pos = 0;
    cash = 0;
    energy = 0;
    food = 0;
    faith = 0;
    active = false;
    done = false;
    tileSize = 0;
    textures = nullptr;
}

void Player::setActive(bool newState){
    active = newState;
    elems.active = newState;
    for (auto&& i: characters)
    {

        i.active = newState;
    }
}

Player::Player(TextureHolder *textures,  sf::Font *gameFont, int pos):
    reachedPortal(false),
    reachedPortalFirst(false)

{


    frozenLeft = 0;
    done = false;

    active = false;
    this->textures = textures;
//    Character character(this->textures, pos);
    characters.push_back(Character (this->textures, pos));
//    efc::BoardElem startElem(textures, efc::startPlayers[pos],444);
//    startElem.setColor(efc::playersColors[pos]);

//    elems.items.push_back(startElem);
//    elems.items_map.insert({efc::startPlayers[pos], startElem});
//    this->faceSize = faceSize;
//    spriteFace.setTexture(textures->textureFaces);
    this->pos = pos;


//    symbol.setTexture(this->textures->textureSymbols);


//    sf::IntRect symbolsRect[4] = {sf::IntRect(0,0,255,255), sf::IntRect(256,0,512,255), sf::IntRect(0,255, 255, 512), sf::IntRect(255,255,512, 512)};


//    symbol.setTextureRect(symbolsRect[pos]);
//    symbol.setScale(sf::Vector2f(0.2f, 0.20f));
//    symbol.setColor(sf::Color(25, 25, 25, 105));
//    symbol.setPosition(60, (pos*100)+40);

    food = 0;
    cash = 0;
    energy = 0;
    faith = 0;

    txtCash.setFont(*gameFont);
    txtCash.setCharacterSize(20);

    txtFood.setFont(*gameFont);
    txtEnergy.setFont(*gameFont);
    txtFaith.setFont(*gameFont);

    txtNextRound.setFont(*gameFont);
    txtNextRound.setString("End Turn");
    txtNextRound.setCharacterSize(12);
    txtNextRound.setPosition(40,(pos*100)+10);


    int posX1 = 82;
    int posX2 = 962;
    int posY1 = 22;
    int posY2 = 720;
    std::array<std::array<int,2>,4> textPos =
    {
        {
            {{posX1,posY1}}, {posX2,posY1},
            {{posX1,posY2}}, {posX2, posY2}
        }
    };

    txtCash.setPosition(textPos[pos][0],textPos[pos][1] );
    buttons.insert({"end_turn",rectangle});

}

std::string Player::getElem(sf::Vector2f mousePosition) {
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

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(txtCash, states);
}
void Player::play()
{
    for (auto&& i: characters)
    {
        i.play();
    }
}
void Player::update(sf::Time deltaTime)
{
    updateTxt();
    if (frozenLeft==0)
    {
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
            i.update(deltaTime);
        }
    } else{
        characters[0].currentAnimationIndex=efc::DIR_DOWN;
    }
}

std::array<int,2> Player::getMovements(int diceResult)
{
    return characters[0].getMovements(diceResult);
}

void Player::setFigurePos(int pos)
{
    characters[0].setBoardPosition(pos);
}

void Player::restartPlayer(){
    food = 0;
    cash = 0;
    energy = 0;
    faith = 0;
}
