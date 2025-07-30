#pragma once
#include <vector>

#include <SFML/Graphics.hpp>

#include "animated-board-item.h"
#include "board-spawn-regions.h"
#include "boarddiamondseq.h"

// Forward declaration for lighting
namespace DP {
class LightingManager;
}

class BoardInitializationAnimator {
private:
  std::vector<AnimatedBoardItem> animatedItems;
  BoardSpawnRegions spawnRegions;
  BoardAnimationConfig config;
  sf::VertexArray animationVertices;
  bool animationComplete = true;
  bool holdingDiamonds = false; // NEW: Hold diamonds after animation completes
  bool fadingOut = false; // NEW: Fade out dark overlay after animation
  float totalElapsedTime = 0.0f;
  float fadeOutElapsed = 0.0f;
  float fadeOutDuration = 2.0f; // 2 seconds to fade out dark overlay

  void updateVertexArray();
  void initializeVertexArrayAtSpawn();

public:
  void initializeAnimation(const BoardDiamondSeq& diamonds, sf::RenderWindow& window);
  void startAnimation();
  void update(sf::Time deltaTime);
  void render(sf::RenderTarget& target, const sf::Texture& texture) const;
  void skipAnimation();
  bool isComplete() const { return animationComplete && !fadingOut; } // Complete when fade-out is done
  bool isAnimationPhaseComplete() const { return animationComplete; } // NEW: Check if animation phase is done
  bool isHoldingDiamonds() const { return holdingDiamonds; } // NEW: Check if diamonds are being held
  bool isFadingOut() const { return fadingOut; } // NEW: Check if fade-out is active
  void releaseDiamonds() { holdingDiamonds = false; fadingOut = false; } // NEW: Release diamonds for game start

  // Configuration access
  void setAnimationConfig(const BoardAnimationConfig& newConfig) { config = newConfig; }
  const BoardAnimationConfig& getAnimationConfig() const { return config; }
  
  // Lighting integration
  void updateLights(DP::LightingManager& lightingManager) const;
  sf::Color getCurrentAmbientColor() const; // NEW: Get current ambient color for fade-out
};