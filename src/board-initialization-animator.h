#pragma once
#include "animated-board-item.h"
#include "boarddiamondseq.h"
#include "board-spawn-regions.h"
#include <vector>
#include <SFML/Graphics.hpp>

class BoardInitializationAnimator {
private:
  std::vector<AnimatedBoardItem> animatedItems;
  BoardSpawnRegions spawnRegions;
  BoardAnimationConfig config;
  sf::VertexArray animationVertices;
  bool animationComplete = true;
  float totalElapsedTime = 0.0f;
  
  void updateVertexArray();
  void initializeVertexArrayAtSpawn();
  
public:
  void initializeAnimation(const BoardDiamondSeq& diamonds, sf::RenderWindow& window);
  void startAnimation();
  void update(sf::Time deltaTime);
  void render(sf::RenderTarget& target, const sf::Texture& texture) const;
  void skipAnimation();
  bool isComplete() const { return animationComplete; }
  
  // Configuration access
  void setAnimationConfig(const BoardAnimationConfig& newConfig) { config = newConfig; }
  const BoardAnimationConfig& getAnimationConfig() const { return config; }
};