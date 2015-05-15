#include <data.h>

namespace efc {

std::string seasonsNames[4] = {"spring", "summer", "fall", "winter"};

// This array defines fields occupied by the river
int terrainArray[24] = {
    8, 24, 40, 56, 72, 88,
    113, 114, 115, 116, 117, 118,
    138, 139, 140, 141, 142, 143,
    167, 183, 199, 215, 231, 247
};


//{8,813},   {24, 803},  {40, 803}, {56, 803}, {72, 803} ,{88, 801},
//{167,809}, {183, 803}, {199, 803}, {215, 803}, {231, 803} ,{247, 8},
//{8,813}, {24, 803}, {40, 803}, {56, 803}, {72, 803} ,{88, 801},
//{8,813}, {24, 803}, {40, 803}, {56, 803}, {72, 803} ,{88, 801},

//level[8] = 813;
//level[24] = 803;
//level[40] = 803;
//level[56] = 803;
//level[72] = 803;
//level[88] = 801;

//level[167] = 809;
//level[183] = 803;
//level[199] = 803;
//level[215] = 803;
//level[231] = 803;
//level[247] = 812;

//level[112] = 811;
//for (int i=113;i<117;i++)
//    level[i] = 816;
//level[117] = 815;

//level[138] = 800;
//for (int i=139;i<143;i++)
//    level[i] = 816;
//level[143] = 814;

// Colors for the board elements
sf::Color playersColors[4] = {
    sf::Color(0, 150,255,255),
    sf::Color(50, 230,50,255),
    sf::Color(230, 50,50,255),
    sf::Color(150, 150,150,255)
};

}

