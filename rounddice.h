#ifndef ROUNDDICE_H
#define ROUNDDICE_H
#include <random>
#include "playerhud.h"


class RoundDice
{
public:
    RoundDice(PlayerHud (&players)[4]);
    PlayerHud *playersHud;


    std::string drawRound();
    int diceResult;

private:
    int throwDice();
    void eventExtraCash();


};

#endif // ROUNDDICE_H
