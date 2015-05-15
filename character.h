#ifndef CHARACTER_H
#define CHARACTER_H
#include <random>
#include <iostream>
#include "data.h"
#include "textureholder.h"
#include "animatedsprite.h"
#include "tilemap.h"


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

};

#endif // CHARACTER_H
