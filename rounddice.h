#ifndef ROUNDDICE_H
#define ROUNDDICE_H
#include <random>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "playerhud.h"



class RoundDice
{
public:
    RoundDice(PlayerHud (&players)[4]);
    PlayerHud *playersHud;


    std::string drawRound();
    int diceResult;
    int diceResultSix;
    int throwDiceSix();
    sf::Texture textureDices;
    sf::Sprite spriteDice;



private:
    int throwDice();


    int diceSize;

    void eventExtraCash();

    sf::SoundBuffer sfxDiceBuffer;
    sf::Sound sfxDice;
    void setDiceTexture();

};

#endif // ROUNDDICE_H
