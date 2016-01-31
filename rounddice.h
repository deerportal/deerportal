#ifndef ROUNDDICE_H
#define ROUNDDICE_H
#include <random>
#include <SFML/Audio.hpp>
#include "playerhud.h"



class RoundDice
{
public:
    RoundDice(PlayerHud (&players)[4]);
    PlayerHud *playersHud;


    std::string drawRound();
    int diceResult;
    int throwDiceSix();

private:
    int throwDice();
    void eventExtraCash();

    sf::SoundBuffer sfxDiceBuffer;
    sf::Sound sfxDice;

};

#endif // ROUNDDICE_H
