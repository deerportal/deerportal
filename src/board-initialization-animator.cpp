#include "board-initialization-animator.h"

#include <cmath>
#include <cstdint>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "data.h"

void BoardInitializationAnimator::initializeAnimation(const BoardDiamondSeq& diamonds,
                                                      sf::RenderWindow& window) {
  animatedItems.clear();
  animatedItems.reserve(DP::diamondsNumber);

  // Initialize VertexArray for efficient rendering (6 vertices per diamond - 2 triangles)
  animationVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  animationVertices.resize(DP::diamondsNumber * 6);

  animationComplete = false;
  totalElapsedTime = 0.0f;

#ifndef NDEBUG
  std::cout << "[DEBUG] Initializing animation for " << DP::diamondsNumber << " diamonds"
            << std::endl;
#endif

  // Create animated items for each diamond
  for (int i = 0; i < DP::diamondsNumber; i++) {
    // Get quadrant and spawn point
    int quadrant = spawnRegions.getQuadrantForDiamond(i);
    sf::Vector2f spawnPoint = spawnRegions.getSpawnPoint(quadrant, window);

    // CRITICAL FIX: Use EXACT same position calculation as BoardDiamondSeq
    const BoardDiamond& diamond = diamonds.diamonds[i];
    sf::Vector2i cords = DP::transPosition(diamond.boardPosition);
    sf::Vector2f tilePos = DP::getScreenPos(cords);

    // Use IDENTICAL calculation as BoardDiamondSeq::updateSingleDiamond() line 64
    const float offsetX = 2.4f; // (40 - 35.2) / 2 = 2.4f (centering offset)
    const float offsetY = 2.4f; // (40 - 35.2) / 2 = 2.4f (centering offset)
    sf::Vector2f staticPosition(tilePos.x + offsetX, tilePos.y + offsetY);

    // GROK4 FIX: Add board offset to align with global coordinate system
    // Static diamonds now use transform, so animation targets must match global coordinates
    sf::Vector2f targetPos = staticPosition + sf::Vector2f(202.f, 76.f);

    // Initialize animated item with diamond's idNumber for correct sprite
    AnimatedBoardItem item;
    item.initialize(i, spawnPoint, targetPos, diamond.idNumber);
    animatedItems.push_back(item);

#ifndef NDEBUG
    // Only debug first 3 diamonds to avoid excessive output
    if (i < 3) {
      std::cout << "[DEBUG] Diamond " << i << " spawn: (" << spawnPoint.x << ", " << spawnPoint.y
                << ") target: (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
    }
#endif
  }

  // Initialize vertex array with diamonds at spawn positions (visible)
  initializeVertexArrayAtSpawn();
}

void BoardInitializationAnimator::startAnimation() {
  animationComplete = false;
  totalElapsedTime = 0.0f;

#ifndef NDEBUG
  std::cout << "[DEBUG] Starting board initialization animation" << std::endl;
#endif
}

void BoardInitializationAnimator::update(sf::Time deltaTime) {
  if (animationComplete) return;

  totalElapsedTime += deltaTime.asSeconds();

  bool allFinished = true;

  // Update each animated item with staggered start times
  for (size_t i = 0; i < animatedItems.size(); i++) {
    float staggeredStartTime = i * config.staggerDelay;

    if (totalElapsedTime >= staggeredStartTime) {
      // Calculate effective delta time for this item
      sf::Time effectiveDelta = sf::seconds(deltaTime.asSeconds());
      if (totalElapsedTime - staggeredStartTime < deltaTime.asSeconds()) {
        // First frame for this item, adjust delta time
        effectiveDelta = sf::seconds(totalElapsedTime - staggeredStartTime);
      }

      animatedItems[i].update(effectiveDelta, config);
    }

    if (!animatedItems[i].isFinished()) {
      allFinished = false;
    }
  }

  // Update vertex array with current positions
  updateVertexArray();

  // Check if animation is complete
  if (allFinished) {
    animationComplete = true;

#ifndef NDEBUG
    std::cout << "[DEBUG] Board initialization animation completed after " << totalElapsedTime
              << " seconds" << std::endl;
#endif
  }
}

void BoardInitializationAnimator::updateVertexArray() {
  for (size_t i = 0; i < animatedItems.size(); i++) {
    // Only update vertex data for items that have started animating
    float staggeredStartTime = i * config.staggerDelay;
    if (totalElapsedTime >= staggeredStartTime) {
      // Note: We don't have direct access to texture here,
      // so we'll use placeholder texture coordinates
      // The actual texture will be provided during rendering
      int vertexOffset = i * 6; // 6 vertices per diamond (2 triangles)

      sf::Vector2f position = animatedItems[i].getCurrentPosition();
      float scale = animatedItems[i].getCurrentScale();
      float rotation = animatedItems[i].getCurrentRotation();

      // Diamond sprite size (must match BoardDiamondSeq: 35.2f = 44.0f * 0.8f)
      float size = 35.2f * scale;
      float halfSize = size * 0.5f;

      // Calculate rotated vertices
      float cosAngle = std::cos(rotation * M_PI / 180.0f);
      float sinAngle = std::sin(rotation * M_PI / 180.0f);

      // Define quad corners
      sf::Vector2f corners[4] = {
          sf::Vector2f(-halfSize, -halfSize), // Top-left
          sf::Vector2f(halfSize, -halfSize),  // Top-right
          sf::Vector2f(halfSize, halfSize),   // Bottom-right
          sf::Vector2f(-halfSize, halfSize)   // Bottom-left
      };

      // Apply rotation to corners
      sf::Vector2f rotatedCorners[4];
      for (int j = 0; j < 4; j++) {
        rotatedCorners[j] = sf::Vector2f(corners[j].x * cosAngle - corners[j].y * sinAngle,
                                         corners[j].x * sinAngle + corners[j].y * cosAngle) +
                            position;
      }

      // Define texture coordinates based on diamond's textureId
      int textureId = animatedItems[i].getTextureId();
      float texLeft = textureId * 44.0f;
      float texRight = texLeft + 44.0f;
      float texTop = 0.0f;
      float texBottom = 44.0f;

      sf::Vector2f texCoords[4] = {
          sf::Vector2f(texLeft, texTop),     // Top-left
          sf::Vector2f(texRight, texTop),    // Top-right
          sf::Vector2f(texRight, texBottom), // Bottom-right
          sf::Vector2f(texLeft, texBottom)   // Bottom-left
      };

      // Set color with alpha based on animation progress
      float alpha = 255.0f;
      if (totalElapsedTime < staggeredStartTime + 0.2f) {
        // Fade in during first 0.2 seconds
        alpha = ((totalElapsedTime - staggeredStartTime) / 0.2f) * 255.0f;
        alpha = std::max(0.0f, std::min(255.0f, alpha));
      }
      sf::Color color(255, 255, 255, static_cast<std::uint8_t>(alpha));

      // Create two triangles to form a quad (6 vertices total)
      int vertexIndex = i * 6;

      // First triangle: 0, 1, 2
      animationVertices[vertexIndex + 0].position = rotatedCorners[0];
      animationVertices[vertexIndex + 0].texCoords = texCoords[0];
      animationVertices[vertexIndex + 0].color = color;

      animationVertices[vertexIndex + 1].position = rotatedCorners[1];
      animationVertices[vertexIndex + 1].texCoords = texCoords[1];
      animationVertices[vertexIndex + 1].color = color;

      animationVertices[vertexIndex + 2].position = rotatedCorners[2];
      animationVertices[vertexIndex + 2].texCoords = texCoords[2];
      animationVertices[vertexIndex + 2].color = color;

      // Second triangle: 0, 2, 3
      animationVertices[vertexIndex + 3].position = rotatedCorners[0];
      animationVertices[vertexIndex + 3].texCoords = texCoords[0];
      animationVertices[vertexIndex + 3].color = color;

      animationVertices[vertexIndex + 4].position = rotatedCorners[2];
      animationVertices[vertexIndex + 4].texCoords = texCoords[2];
      animationVertices[vertexIndex + 4].color = color;

      animationVertices[vertexIndex + 5].position = rotatedCorners[3];
      animationVertices[vertexIndex + 5].texCoords = texCoords[3];
      animationVertices[vertexIndex + 5].color = color;
    } else {
      // Item hasn't started yet, make it invisible
      int vertexIndex = i * 6;
      for (int j = 0; j < 6; j++) {
        animationVertices[vertexIndex + j].color = sf::Color(255, 255, 255, 0);
      }
    }
  }
}

void BoardInitializationAnimator::render(sf::RenderTarget& target,
                                         const sf::Texture& texture) const {
#ifndef NDEBUG
  std::cout << "ANIMATION RENDER: Called with " << animatedItems.size()
            << " items, complete=" << animationComplete << std::endl;
#endif

  if (animatedItems.empty()) {
#ifndef NDEBUG
    std::cout << "ANIMATION RENDER: Skipped - no items to render" << std::endl;
#endif
    return;
  }

  if (animationComplete) {
#ifndef NDEBUG
    std::cout << "ANIMATION RENDER: Skipped - animation complete" << std::endl;
#endif
    return;
  }

#ifndef NDEBUG
  std::cout << "ANIMATION RENDER: Rendering " << animatedItems.size() << " animated items with "
            << animationVertices.getVertexCount() << " vertices" << std::endl;
#endif

  // Render all animated diamonds in a single draw call
  sf::RenderStates states;
  states.texture = &texture;

  target.draw(animationVertices, states);
}

void BoardInitializationAnimator::initializeVertexArrayAtSpawn() {
  // Initialize all diamonds as visible at their spawn positions
  for (size_t i = 0; i < animatedItems.size(); i++) {
    sf::Vector2f spawnPos = animatedItems[i].getSpawnPoint();
    float scale = 1.0f;    // Initial scale
    float rotation = 0.0f; // Initial rotation

    // Diamond sprite size (must match BoardDiamondSeq: 35.2f = 44.0f * 0.8f)
    float size = 35.2f * scale;
    float halfSize = size * 0.5f;

    // Calculate rotated vertices
    float cosAngle = std::cos(rotation * M_PI / 180.0f);
    float sinAngle = std::sin(rotation * M_PI / 180.0f);

    // Define quad corners
    sf::Vector2f corners[4] = {
        sf::Vector2f(-halfSize, -halfSize), // Top-left
        sf::Vector2f(halfSize, -halfSize),  // Top-right
        sf::Vector2f(halfSize, halfSize),   // Bottom-right
        sf::Vector2f(-halfSize, halfSize)   // Bottom-left
    };

    // Apply rotation to corners
    sf::Vector2f rotatedCorners[4];
    for (int j = 0; j < 4; j++) {
      rotatedCorners[j] = sf::Vector2f(corners[j].x * cosAngle - corners[j].y * sinAngle,
                                       corners[j].x * sinAngle + corners[j].y * cosAngle) +
                          spawnPos;
    }

    // Define texture coordinates based on diamond's textureId
    int textureId = animatedItems[i].getTextureId();
    float texLeft = textureId * 44.0f;
    float texRight = texLeft + 44.0f;
    float texTop = 0.0f;
    float texBottom = 44.0f;

    sf::Vector2f texCoords[4] = {
        sf::Vector2f(texLeft, texTop),     // Top-left
        sf::Vector2f(texRight, texTop),    // Top-right
        sf::Vector2f(texRight, texBottom), // Bottom-right
        sf::Vector2f(texLeft, texBottom)   // Bottom-left
    };

    // Set color as fully visible
    sf::Color color(255, 255, 255, 255);

    // Create two triangles to form a quad (6 vertices total)
    int vertexIndex = i * 6;

    // First triangle: 0, 1, 2
    animationVertices[vertexIndex + 0].position = rotatedCorners[0];
    animationVertices[vertexIndex + 0].texCoords = texCoords[0];
    animationVertices[vertexIndex + 0].color = color;

    animationVertices[vertexIndex + 1].position = rotatedCorners[1];
    animationVertices[vertexIndex + 1].texCoords = texCoords[1];
    animationVertices[vertexIndex + 1].color = color;

    animationVertices[vertexIndex + 2].position = rotatedCorners[2];
    animationVertices[vertexIndex + 2].texCoords = texCoords[2];
    animationVertices[vertexIndex + 2].color = color;

    // Second triangle: 0, 2, 3
    animationVertices[vertexIndex + 3].position = rotatedCorners[0];
    animationVertices[vertexIndex + 3].texCoords = texCoords[0];
    animationVertices[vertexIndex + 3].color = color;

    animationVertices[vertexIndex + 4].position = rotatedCorners[2];
    animationVertices[vertexIndex + 4].texCoords = texCoords[2];
    animationVertices[vertexIndex + 4].color = color;

    animationVertices[vertexIndex + 5].position = rotatedCorners[3];
    animationVertices[vertexIndex + 5].texCoords = texCoords[3];
    animationVertices[vertexIndex + 5].color = color;
  }
}

void BoardInitializationAnimator::skipAnimation() {
  animationComplete = true;

  // Set all items to finished state
  for (auto& item : animatedItems) {
    // Force completion by updating with large time step
    item.update(sf::seconds(config.animationDuration + 1.0f), config);
  }

#ifndef NDEBUG
  std::cout << "[DEBUG] Board initialization animation skipped" << std::endl;
#endif
}