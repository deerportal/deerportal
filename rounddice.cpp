#include "rounddice.h"

RoundDice::RoundDice(PlayerHud (&players)[4])
{
    playersHud = players;
    diceResult = 0;

    if (!sfxDiceBuffer.loadFromFile("assets/audio/dice.ogg"))
        std::exit(1);
    sfxDice.setBuffer(sfxDiceBuffer);
    sfxDice.setVolume(12);

}

std::string RoundDice::drawRound(){
    throwDice();
    if (diceResult<33)
    {
        return "end_of_round_extra_grow";
    } else if (diceResult<66)
    {
        return "end_of_round_extra_energy";
    } else
    {
        eventExtraCash();
        return "end_of_round_extra_cash";
    }
    return "end_of_round_"+std::to_string(diceResult);
}

int RoundDice::throwDice(){
    sfxDice.play();
    int result = random()%100;
    diceResult = result;
    return result;
}

int RoundDice::throwDiceSix(){
    sfxDice.play();
    int result = random()%5;
    diceResult = result;
    return result+1;
}

void RoundDice::eventExtraCash(){

    for (int i=0;i<4;i++)
    {
        playersHud[i].cash += 20;
        playersHud[i].updateTxt();
    }
}
