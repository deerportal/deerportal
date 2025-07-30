#include "board-spawn-regions.h"

#include <cmath>
#include <iostream>

// Big diamond center coordinates based on position (474, 342) and size (94x102)
constexpr float BIG_DIAMOND_CENTER_X = 474.0f + 47.0f; // 521
constexpr float BIG_DIAMOND_CENTER_Y = 342.0f + 51.0f; // 393

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
  std::cout << "[DEBUG] Diamond " << diamondIndex << " assigned to quadrant " << quadrant
            << std::endl;
#endif

  return quadrant;
}

sf::Vector2f BoardSpawnRegions::getQuadrantCenter(int quadrant, const sf::RenderWindow& window) {
  // For center spawning, all quadrants return the big diamond center
  return sf::Vector2f(BIG_DIAMOND_CENTER_X, BIG_DIAMOND_CENTER_Y);
}

sf::Vector2f BoardSpawnRegions::getSpawnPoint(int quadrant, const sf::RenderWindow& window) {
  // All diamonds spawn from the center of the big diamond
  sf::Vector2f center(BIG_DIAMOND_CENTER_X, BIG_DIAMOND_CENTER_Y);

#ifndef NDEBUG
  std::cout << "[DEBUG] Center spawn point for all diamonds: (" << center.x << ", " << center.y
            << ")" << std::endl;
#endif

  return center;
}