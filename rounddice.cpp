#include "rounddice.h"

RoundDice::RoundDice(PlayerHud (&players)[4])
{
    playersHud = players;
    diceResult = 1;
    diceResultSix = 6;
    diceSize = 150;
    if (!sfxDiceBuffer.loadFromFile("assets/audio/dice.ogg"))
        std::exit(1);

    if (!textureDices.loadFromFile("assets/img/dicesWhite.png"))
        std::exit(1);

    spriteDice.setTexture(textureDices);

    sfxDice.setBuffer(sfxDiceBuffer);
    sfxDice.setVolume(12);
    spriteDice.setPosition(1140,550);
    setDiceTexture();
}

void RoundDice::setDiceTexture(){
    sf::IntRect diceRect(diceSize*diceResultSix, 0, diceSize, diceSize);
    spriteDice.setTextureRect(diceRect);
}

void RoundDice::setColor(int playerNumber){
    sf::Color color(efc::playersColors[playerNumber]);
    spriteDice.setColor(color);
}

void RoundDice::setDiceTexture(int diceResult){
    this->diceResultSix = diceResult;
    setDiceTexture();
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
    int result = random()%6;
    diceResultSix = result;
    setDiceTexture();
    return result+1;
}

void RoundDice::eventExtraCash(){

    for (int i=0;i<4;i++)
    {
        playersHud[i].cash += 20;
        playersHud[i].updateTxt();
    }
}
