#include "character.h"

/*!
 * \brief Character::getMovements
 * \return
 */
std::array<int,2> Character::getMovements(int howFar)
{

    std::array<int,2> myArray;

    if (howFar==7)
    {
        return std::array<int,2> {-1,-1};
    }

    if (active==true)
    {
//        std::cout << "howfar: " <<howFar <<std::endl;
            int indexRight = boardPosition;
            for (int i=boardPosition;i<=boardPosition+howFar-1;i++)
            {

                moveRight = indexRight;
                if (indexRight==-1)
                    break;
                indexRight = efc::boards[indexRight][1];
//                            std::cout << " howfar" << howFar << " index R" << indexRight
//                                       << " move L " << moveLeft <<  " move R " << moveRight << " dice " << diceResult
//                                      << std::endl;
                if (indexRight==-2)
                {
                    indexRight = moveRight;
                    break;
                }

                if (indexRight==-1)
                    break;
            }

            moveRight = indexRight;

            int indexLeft = boardPosition;
            for (int i=boardPosition;i>=boardPosition-howFar+1;i--)
            {
                moveLeft = indexLeft;

                if (indexLeft==-1)
                    break;
                indexLeft = efc::boards[indexLeft][0];
//                            std::cout << " howfar" << howFar  << " index l " << indexLeft
//                                      << " move L " << moveLeft << " move R " << moveRight << " dice " << diceResult
//                                      << std::endl;
                if (indexLeft==-2)
                {
                    indexLeft = moveLeft;
                    break;
                }
                if (indexLeft==-1)
                    break;
            }
            moveLeft = indexLeft;



        myArray = {moveLeft,moveRight};
    }
    else
    {
        myArray = {-1,-1};
    }
//    std::cout << "howfar: " <<howFar << std::endl;
    return myArray;

}

void Character::setBoardPosition(int playerNumber)
{
    Elem::setBoardPosition(playerNumber);
    move(0,-20);

}



void Character::setDir(int direction)
{
    if (direction==efc::DIR_LEFT)
        currentAnimation = &walkingAnimationLeft;
}



void Character::setDir()
{
    setDir(currentAnimationIndex);
}

void Character::setDirIndex(int direction)
{
    currentAnimationIndex = direction;
}


void Character::play()
{
    //    animatedSprite.play(*currentAnimation);
    animatedSprite.play(animations[currentAnimationIndex]);
    sf::Vector2f a(getPosition());
    sf::Vector2i position(efc::getCords(a));

//    std::cout << a.x << "    " << a.y << "           "
//              << position.x << "       " << position.y << " pos > "
//              << getBoardPosition() << std::endl;
}

Character::Character(TextureHolder *textures, int playerNumber):
    animatedSprite(sf::seconds(0.2), true, false),
    nextRedirect(0.f)
{

    drawMovements = false;
    this->textures = textures;
    int offset = playerNumber*16;
    active = false;


    rectangleLeft.setFillColor(sf::Color(12, 12, 12,120));
    rectangleLeft.setOutlineColor(sf::Color(24,24,40, 255));
    rectangleRight.setFillColor(sf::Color(240, 240, 240,98));
    rectangleRight.setOutlineColor(sf::Color(24,40,24, 90));



    walkingAnimationDown.setSpriteSheet(textures->textureCharacters);
    walkingAnimationDown.addFrame(sf::IntRect(offset, 0, 16, 24));
    walkingAnimationDown.addFrame(sf::IntRect(offset, 24, 16, 24));

    walkingAnimationRight.setSpriteSheet(textures->textureCharacters);
    walkingAnimationRight.addFrame(sf::IntRect(offset, 48, 16, 24));
    walkingAnimationRight.addFrame(sf::IntRect(offset, 72, 16, 24));

    walkingAnimationLeft.setSpriteSheet(textures->textureCharacters);
    walkingAnimationLeft.addFrame(sf::IntRect(offset, 96, 16, 24));
    walkingAnimationLeft.addFrame(sf::IntRect(offset, 120, 16, 24));

    walkingAnimationUp.setSpriteSheet(textures->textureCharacters);
    walkingAnimationUp.addFrame(sf::IntRect(offset, 144, 16, 24));
    walkingAnimationUp.addFrame(sf::IntRect(offset, 168, 16, 24));

    currentAnimation = &walkingAnimationRight;


    leftChar.setTexture(textures->textureCharacters);
    leftChar.setTextureRect(sf::IntRect(offset, 96, 16, 24));


    rightChar.setTexture(textures->textureCharacters);
    rightChar.setTextureRect(sf::IntRect(offset, 48, 16, 24));

    leftChar.scale(0.75,0.75);
    rightChar.scale(0.75,0.75);

    leftChar.move(202,76);
    rightChar.move(202,76);

    animations[efc::DIR_LEFT] = walkingAnimationLeft;
    animations[efc::DIR_RIGHT] = walkingAnimationRight;
    animations[efc::DIR_UP] = walkingAnimationUp;
    animations[efc::DIR_DOWN] = walkingAnimationDown;


    setDirIndex(efc::DIR_LEFT);
    setDir();

//    sf::Vector2f positions[4] = {
//        sf::Vector2f(0, 0),
//        sf::Vector2f(20, 40),
//        sf::Vector2f(20, 240),
//        sf::Vector2f(40, 240)
//    };

    std::array<int, 4> boardPositions{0,15,255-15,255};
//    std::cout << "define " << playerNumber << std::endl;
    setBoardPosition(boardPositions[playerNumber]);


}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

    states.transform *= getTransform();
target.draw(animatedSprite, states);

    if ((active==true) && (drawMovements==true))
    {
        if (moveLeft>-1)
        {
            target.draw(leftChar);
        }

        if (moveRight>-1)
        {
            target.draw(rightChar);
        }
    }
}

void Character::update(sf::Time deltaTime)
{
    sf::Vector2f a(getPosition());
    sf::Vector2i position(efc::getCords(a));
    nextRedirect -= deltaTime.asSeconds();
    if (nextRedirect<0)
    {
        int number = rand() % 2;
        if ((currentAnimationIndex==efc::DIR_LEFT) || (currentAnimationIndex==efc::DIR_RIGHT))
        {
            if (number==0){
                setDirIndex(efc::DIR_DOWN);
                setDir();
            } else if (number==1)
            {
                setDirIndex(efc::DIR_UP);
                setDir();
            }
        } else if ((currentAnimationIndex==efc::DIR_UP) || (currentAnimationIndex==efc::DIR_DOWN))
        {
            if (number==0){
                setDirIndex(efc::DIR_LEFT);
                setDir();
            } else if (number==1)
            {
                setDirIndex(efc::DIR_RIGHT);
                setDir();
            }
        }

        nextRedirect = rand() % 4;

    }

    if (currentAnimationIndex==efc::DIR_UP)
    {
        if (position.y<2)
        {
            setDirIndex(efc::DIR_DOWN);
            setDir();
        }
    }  else   if (currentAnimationIndex==efc::DIR_DOWN)
    {
        if (position.y>efc::BOARD_SIZE-1)
        {
            setDirIndex(efc::DIR_UP);
            setDir();
        }
    } else   if (currentAnimationIndex==efc::DIR_LEFT)
    {

        if (position.x<2)
        {
            setDirIndex(efc::DIR_RIGHT);
            setDir();
        }
    }  else   if (currentAnimationIndex==efc::DIR_RIGHT)
    {
        if (position.x>efc::BOARD_SIZE-1)
        {
            setDirIndex(efc::DIR_LEFT);
            setDir();
        }
    }
    animatedSprite.update(deltaTime);

    std::array<int,2> movements(getMovements(diceResult));
//    int left = movements[0];
//    int right = movements[1];


    if (active==true)
    {
        if (moveLeft>-1)
        {
//            sf::Vector2i cordsLeft(efc::transPosition(moveLeft));
            sf::Vector2i neededCords(efc::transPosition(moveLeft));

            sf::Vector2f newPos(efc::getScreenPos(neededCords));
            leftChar.setPosition(newPos.x+efc::TILE_SIZE/4,newPos.y);

            leftChar.move(202,76);
        }


        if (moveRight>-1)
        {
//            sf::Vector2i cordsRight(efc::transPosition(moveRight));
            sf::Vector2i neededCords(efc::transPosition(moveRight));

            sf::Vector2f newPos(efc::getScreenPos(neededCords));
            rightChar.setPosition(newPos.x+efc::TILE_SIZE/4,newPos.y);
            rightChar.move(202,76);

        }

//        std::cout << " dice " << diceResult<< moveLeft << " " << moveRight  << std::endl;
    }


}

sf::FloatRect Character::getLocalBounds() const
{

    return sf::FloatRect(0.f, 0.f, 0, 0);
}

sf::FloatRect Character::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

