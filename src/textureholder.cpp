#include "textureholder.h"

#include <iostream>

#include "exceptions.h"

namespace DP {

std::set<int> getTerrainSet() {
  std::set<int> terrain;
  for (int i : DP::terrainArray) {
    terrain.insert(i);
  }
  return terrain;
}

} // namespace DP
TextureHolder::TextureHolder() {
  //     "stop", "card", "diamond", "diamond x 2"

  std::string cardsImages[4][4] = {{"card-water-stop.small.png", "card-water-remove-card.small.png",
                                    "card-water-diam.small.png", "card-water-2-diam.small.png"},
                                   {"card-earth-stop.small.png", "card-earth-remove-card.small.png",
                                    "card-earth-diam.small.png", "card-earth-2-diam.small.png"},
                                   {"card-fire-stop.small.png", "card-fire-remove-card.small.png",
                                    "card-fire-diam.small.png", "card-fire-2-diam.small.png"},
                                   {"card-air-stop.small.png", "card-air-remove-card.small.png",
                                    "card-air-diam.small.png", "card-air-2-diam.small.png"}

  };

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (!cardsTextures[i][j].loadFromFile(
              get_full_path(ASSETS_PATH "img/cards/" + cardsImages[i][j]))) {
        //                std::cout << "assets/img/cards/" << cardsImages[i][j];
        throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                             "img/cards/" + cardsImages[i][j],
                                             "Failed to load card texture");
      } else {
        //                std::cout << "assets/img/cards/" << cardsImages[i][j] << " ok " << i << "
        //                " <<  j << " " << std::endl;
      }
    }
  }

  if (!textureCardBases[0].loadFromFile(get_full_path(ASSETS_PATH "img/card-water-2-diam_m.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/card-water-2-diam_m.png",
                                         "Failed to load water card base texture");

  if (!textureCardBases[1].loadFromFile(get_full_path(ASSETS_PATH "img/card-earth-2-diam_m.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/card-earth-2-diam_m.png",
                                         "Failed to load earth card base texture");

  if (!textureCardBases[2].loadFromFile(get_full_path(ASSETS_PATH "img/card-fire-2-diam_m.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/card-fire-2-diam_m.png",
                                         "Failed to load fire card base texture");

  if (!textureCardBases[3].loadFromFile(get_full_path(ASSETS_PATH "img/card-air-2-diam_m.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/card-air-2-diam_m.png",
                                         "Failed to load air card base texture");

  //    if (!textureGameBackground.loadFromFile(ASSETS_PATH"assets/img/game-ackground.png"))
  //        std::exit(1);

  //    if (!textureTiles.loadFromFile(ASSETS_PATH"assets/img/zw-tilesets/_MAP.png"))
  //        std::exit(1);
  //    if (!textureFaces.loadFromFile(ASSETS_PATH"assets/img/faces.jpg"))
  //        std::exit(1);
  //    if (!textureGui.loadFromFile(ASSETS_PATH"assets/img/gui.png"))
  //        std::exit(1);

  //    if (!textureSymbols.loadFromFile(ASSETS_PATH"assets/img/symbols.png"))
  //        std::exit(1);
  //    if (!textureSeasons.loadFromFile(ASSETS_PATH"assets/img/seasons.png"))
  //        std::exit(1);

  if (!textureCharacters.loadFromFile(get_full_path(ASSETS_PATH "img/characters-new.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/characters-new.png",
                                         "Failed to load characters texture");

  if (!backgroundDark.loadFromFile(get_full_path(ASSETS_PATH "img/background_dark.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/background_dark.png",
                                         "Failed to load dark background texture");
  if (!textureBoardDiamond.loadFromFile(get_full_path(ASSETS_PATH "img/board_diamonds.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/board_diamonds.png",
                                         "Failed to load board diamonds texture");

  if (!textureMenu.loadFromFile(get_full_path(ASSETS_PATH "img/dp_intro_menu.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/dp_intro_menu.png",
                                         "Failed to load intro menu texture");

  if (!textureLetsBegin.loadFromFile(get_full_path(ASSETS_PATH "img/letsbegin.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/letsbegin.png", "Failed to load lets begin texture");

  if (!textureButtonCpu.loadFromFile(get_full_path(ASSETS_PATH "img/button-cpu.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/button-cpu.png", "Failed to load CPU button texture");

  if (!textureButtonHuman.loadFromFile(get_full_path(ASSETS_PATH "img/button-human.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/button-human.png",
                                         "Failed to load human button texture");

  if (!textureDeerGod.loadFromFile(get_full_path(ASSETS_PATH "img/deer-god.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/deer-god.png", "Failed to load deer god texture");

  if (!textureBigDiamond.loadFromFile(get_full_path(ASSETS_PATH "img/diamond-big.png")))
    throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::TEXTURE,
                                         "img/diamond-big.png",
                                         "Failed to load big diamond texture");

  int defaultArray[5][8] = {
      // Cash   Food    Energy  Faith
      {10, 2, 0, 0, 0, 0, 0, 0}, // base
      {10, 1, 2, 0, 0, 5, 0, 0}, // windmill
      {15, 0, 0, 2, 2, 0, 0, 0}, // granary
      {20, 5, 4, 0, 4, 0, 0, 0}, // marketplace
      {5, 0, 2, 0, 0, 0, 0, 2}   // monasterium
  };
  int defaultFields[5] = {443, 651, 442, 585, 1100};

  /*
   * Array with description of the field
   * global rule = even indexes are price, odd - monthly cost
   * [0] - price in cash
   * [1] - monthly cash cost
   * [2] - price - in food
   * [3] - monthly food cost
   * [4] - price in energy
   * [5] - monthly energy cost
   * [6] - price in faith
   * [7] - monthly cost in faith
   *
   */

  int counter = 0;
  for (int i : defaultFields) {
    std::map<int, int> params;
    for (int j = 0; j < 8; j++) {
      params.insert({j, defaultArray[counter][j]});
    }
    //            params.insert({0, 10});
    //            params.insert({1, 2});
    //            params.insert({2, 10});
    //            params.insert({3, 0});
    //            params.insert({4, 10});
    //            params.insert({5, 0});
    //            params.insert({6, 0});
    //            params.insert({7, 0});
    tilesDescription.insert({i, params});
    counter++;
  };
  tilesTxt.insert({443, "Your base."});
  tilesTxt.insert({651, "Windmill, produces energy.\nEnergy is a basic resource in a game,\nneeded "
                        "by other buildings to running them."});
  tilesTxt.insert({442, "Granary, food storehouse.\nFood gives your people ability to "
                        "live.\nWithout the food your people will face\na death by starvation."});
  tilesTxt.insert({585, "Marketplace, generates cash.\nProvides your tribe trading area.\nToday's "
                        "offer - dog's bone."});
  tilesTxt.insert({1100, "Monasterium, increase your faith.\nAs we all know, our world is being "
                         "ruled\nby four ancient gods..."});
}
