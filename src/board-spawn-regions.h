#pragma once
#include <SFML/Graphics.hpp>

class BoardSpawnRegions {
private:
  static constexpr float SPAWN_RADIUS = 200.0f;
  
public:
  // Fixed quadrant mapping with player 2/3 correction
  int getQuadrantForDiamond(int diamondIndex);
  sf::Vector2f getSpawnPoint(int quadrant, const sf::RenderWindow& window);
  sf::Vector2f getQuadrantCenter(int quadrant, const sf::RenderWindow& window);
};