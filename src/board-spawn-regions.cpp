#include "board-spawn-regions.h"
#include <iostream>
#include <cmath>

int BoardSpawnRegions::getQuadrantForDiamond(int diamondIndex) {
  // Player distribution with 2/3 swap correction:
  // Player 0: diamonds 0-27 -> Q0 (top-left)
  // Player 1: diamonds 28-55 -> Q1 (top-right) 
  // Player 3: diamonds 56-83 -> Q2 (bottom-left)
  // Player 2: diamonds 84-111 -> Q3 (bottom-right)
  
  int quadrant;
  if (diamondIndex >= 0 && diamondIndex <= 27) {
    quadrant = 0; // Q0 - top-left
  } else if (diamondIndex >= 28 && diamondIndex <= 55) {
    quadrant = 1; // Q1 - top-right
  } else if (diamondIndex >= 56 && diamondIndex <= 83) {
    quadrant = 2; // Q2 - bottom-left (Player 3)
  } else if (diamondIndex >= 84 && diamondIndex <= 111) {
    quadrant = 3; // Q3 - bottom-right (Player 2)
  } else {
    quadrant = 0; // Default fallback
  }
  
#ifndef NDEBUG
  std::cout << "[DEBUG] Diamond " << diamondIndex << " assigned to quadrant " << quadrant << std::endl;
#endif
  
  return quadrant;
}

sf::Vector2f BoardSpawnRegions::getQuadrantCenter(int quadrant, const sf::RenderWindow& window) {
  sf::Vector2u windowSize = window.getSize();
  
  sf::Vector2f center;
  switch (quadrant) {
    case 0: // Q0 - top-left corner
      center = sf::Vector2f(0, 0);
      break;
    case 1: // Q1 - top-right corner
      center = sf::Vector2f(windowSize.x, 0);
      break;
    case 2: // Q2 - bottom-left corner
      center = sf::Vector2f(0, windowSize.y);
      break;
    case 3: // Q3 - bottom-right corner
      center = sf::Vector2f(windowSize.x, windowSize.y);
      break;
    default:
      center = sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f);
      break;
  }
  
  return center;
}

sf::Vector2f BoardSpawnRegions::getSpawnPoint(int quadrant, const sf::RenderWindow& window) {
  sf::Vector2f corner = getQuadrantCenter(quadrant, window);
  
  // Move spawn points slightly inward from corners so they're visible
  sf::Vector2f result = corner;
  const float inset = 50.0f; // Move 50 pixels inward from each edge
  
  switch (quadrant) {
    case 0: // Top-left corner
      result.x += inset;
      result.y += inset;
      break;
    case 1: // Top-right corner  
      result.x -= inset;
      result.y += inset;
      break;
    case 2: // Bottom-left corner
      result.x += inset;
      result.y -= inset;
      break;
    case 3: // Bottom-right corner
      result.x -= inset;
      result.y -= inset;
      break;
  }
  
#ifndef NDEBUG
  std::cout << "[DEBUG] Quadrant " << quadrant << " spawn point: (" 
            << result.x << ", " << result.y << ")" << std::endl;
#endif
  
  return result;
}