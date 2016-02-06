#ifndef CHARACTER_H
#define CHARACTER_H
#include <random>
#include <iostream>
#include "data.h"
#include "textureholder.h"
#include "animatedsprite.h"
#include "tilemap.h"
#include "character.h"


class Character: public sf::Drawable, public sf::Transformable
{
public:
    Character(TextureHolder *textures, int playerNumber);
    TextureHolder *textures;
    Animation walkingAnimationDown;
    Animation walkingAnimationUp;
    Animation walkingAnimationLeft;
    Animation walkingAnimationRight;
    Animation animations[4];
    Animation* currentAnimation;
    int currentAnimationIndex;

    sf::Sprite leftChar;
    sf::Sprite rightChar;

    AnimatedSprite animatedSprite;

    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;
    void setDirUp();
    void setDirDown();
    void setDirLeft();
    void setDirRight();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(sf::Time deltaTime, std::set<int>& busyTiles);
    void play();
    void setDir();
    void setDir(int direction);
    void setDirIndex(int direction);
    float nextRedirect;

    std::array<int,2> getMovements(int howFar);
    void setBoardPosition(int boardPosition);
    int getBoardPosition();

    int boardPosition;
    int diceResult;

    sf::RectangleShape rectangleLeft;
    sf::RectangleShape rectangleRight;

    int moveLeft;
    int moveRight;

    bool active;
    bool drawMovements;


};

#endif // CHARACTER_H
