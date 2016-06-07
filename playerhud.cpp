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
    human(false),
    reachPortalMode(false)
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
    number = 0;
}

void Player::setActive(bool newState){
    active = newState;
    elems.active = newState;
    for (auto&& i: characters)
    {

        i.active = newState;
    }
}

Player::Player(TextureHolder *textures,  sf::Font *gameFont, int playerNumber):
    reachedPortal(false),
    reachedPortalFirst(false),
    human(false),
    reachPortalMode(false)
{
    number = playerNumber;

    frozenLeft = 0;
    done = false;

    active = false;
    this->textures = textures;
    characters.push_back(Character (this->textures, playerNumber));

    this->pos = playerNumber;

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
    txtNextRound.setPosition(40,(playerNumber*100)+10);


    int posX1 = 82;
    int posX2 = 962;
    int posY1 = 22;
    int posY2 = 720;
    std::array<std::array<int,2>,4> textPos =
    {
        {
            {{posX1,posY1}}, {{posX2,posY1}},
            {{posX1,posY2}}, {{posX2, posY2}}
        }
    };

    txtCash.setPosition(textPos[playerNumber][0],textPos[playerNumber][1] );
    buttons.insert({"end_turn",rectangle});

    setSpriteAI();

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

void Player::setHuman(bool newHuman)
{
    human = newHuman;
    setSpriteAI();
}

void Player::swapHuman()
{
    if (human)
    {
        setHuman(false);
        return;
    }  else {
        setHuman(true);
        return;
    }
}

void Player::setSpriteAI()
{
    if (human)
        spriteAI.setTexture(textures->textureButtonHuman);
    else
        spriteAI.setTexture(textures->textureButtonCpu);


    std::array<std::array<int,2>,4> spriteHumanPos =
    {
        {
            {{220,450}}, {{1050,450}},
            {{140,600}}, {{1110, 600}}
        }
    };

    sf::Vector2f spriteHumanPosNew(spriteHumanPos[number][0],spriteHumanPos[number][1]);
    spriteAI.setPosition(spriteHumanPosNew);

}
