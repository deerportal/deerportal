#include "animated-board-item.h"

#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void AnimatedBoardItem::initialize(int id, sf::Vector2f spawn, sf::Vector2f target, int texId) {
  diamondId = id;
  textureId = texId;
  spawnPoint = spawn;
  progress = 0.0f;
  rotationAngle = 0.0f;
  currentScale = 0.3f;
  finished = false;

  // GEMINI FIX: The provided `target` is the TOP-LEFT position.
  // The animation logic (Bezier, rotation) uses the CENTER point.
  // We must convert the target to be the center for internal calculations.
  const float finalSize = 35.2f; // Must match final size in BoardDiamondSeq rendering
  const float finalHalfSize = finalSize / 2.0f;
  targetPoint = target + sf::Vector2f(finalHalfSize, finalHalfSize);

  initializeBezierPath(spawn, targetPoint);

#ifndef NDEBUG
  // Only debug first 3 diamonds to avoid excessive output
  if (diamondId < 3) {
    std::cout << "[DEBUG] Diamond " << diamondId << " initialized: spawn(" << spawn.x << ", "
              << spawn.y << ") target(" << target.x << ", " << target.y << ")" << std::endl;
  }
#endif
}

void AnimatedBoardItem::initializeBezierPath(sf::Vector2f spawn, sf::Vector2f target) {
  // Create curved path using cubic Bezier curve
  bezierPoints[0] = spawn;  // Start point
  bezierPoints[3] = target; // End point

  // Calculate control points for natural arc movement
  sf::Vector2f direction = target - spawn;
  sf::Vector2f midPoint = spawn + direction * 0.5f;

  // Create arc by offsetting control points perpendicular to direct path
  sf::Vector2f perpendicular(-direction.y, direction.x);
  float pathLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);

  // Normalize perpendicular and scale for arc height (20% of path length)
  if (pathLength > 0) {
    perpendicular = perpendicular / pathLength * (pathLength * 0.2f);
  }

  bezierPoints[1] = midPoint + perpendicular * 0.5f; // First control point
  bezierPoints[2] = midPoint + perpendicular * 0.5f; // Second control point

#ifndef NDEBUG
  // Only debug first 3 diamonds to avoid excessive output
  if (diamondId < 3) {
    std::cout << "[DEBUG] Diamond " << diamondId << " Bezier path: "
              << "P0(" << bezierPoints[0].x << ", " << bezierPoints[0].y << ") "
              << "P1(" << bezierPoints[1].x << ", " << bezierPoints[1].y << ") "
              << "P2(" << bezierPoints[2].x << ", " << bezierPoints[2].y << ") "
              << "P3(" << bezierPoints[3].x << ", " << bezierPoints[3].y << ")" << std::endl;
  }
#endif
}

sf::Vector2f AnimatedBoardItem::calculateBezierPosition(float t) const {
  // Cubic Bezier curve calculation: B(t) = (1-t)³P₀ + 3(1-t)²tP₁ + 3(1-t)t²P₂ + t³P₃
  float invT = 1.0f - t;
  float invT2 = invT * invT;
  float invT3 = invT2 * invT;
  float t2 = t * t;
  float t3 = t2 * t;

  return invT3 * bezierPoints[0] + 3.0f * invT2 * t * bezierPoints[1] +
         3.0f * invT * t2 * bezierPoints[2] + t3 * bezierPoints[3];
}

float AnimatedBoardItem::easeOutCubic(float t) const {
  return 1.0f - std::pow(1.0f - t, 3.0f);
}

void AnimatedBoardItem::update(sf::Time deltaTime, const BoardAnimationConfig& config) {
  if (finished) return;

  float progressIncrement = deltaTime.asSeconds() / config.animationDuration;
  progress += progressIncrement;

  if (progress >= 1.0f) {
    progress = 1.0f;
    finished = true;
  }

  // Apply easing for smooth deceleration
  float easedProgress = easeOutCubic(progress);

  // Update scale with easing
  currentScale = config.startScale + (config.endScale - config.startScale) * easedProgress;

  // Update rotation if enabled - smoothly transition to 0.0 degrees at end
  if (config.enableRotation) {
    if (progress < 1.0f) {
      // During animation: rotate normally
      rotationAngle += config.rotationSpeed * deltaTime.asSeconds();
      if (rotationAngle > 360.0f) {
        rotationAngle -= 360.0f;
      }
    } else {
      // Animation finished: ensure rotation is exactly 0.0 to match static diamonds
      rotationAngle = 0.0f;
    }
  }
}

sf::Vector2f AnimatedBoardItem::getCurrentPosition() const {
  float easedProgress = easeOutCubic(progress);
  return calculateBezierPosition(easedProgress);
}

void AnimatedBoardItem::addToVertexArray(sf::VertexArray& vertices, int vertexOffset,
                                         const sf::Texture& texture) const {
  if (finished && progress >= 1.0f) {
    return; // Don't render if animation is complete
  }

  sf::Vector2f position = getCurrentPosition();
  sf::Vector2u textureSize = texture.getSize();

  // Diamond sprite size (must match BoardDiamondSeq: 35.2f = 44.0f * 0.8f)
  float size = 35.2f * currentScale;
  float halfSize = size * 0.5f;

  // Calculate rotated vertices
  float cosAngle = std::cos(rotationAngle * M_PI / 180.0f);
  float sinAngle = std::sin(rotationAngle * M_PI / 180.0f);

  // Define quad corners
  sf::Vector2f corners[4] = {
      sf::Vector2f(-halfSize, -halfSize), // Top-left
      sf::Vector2f(halfSize, -halfSize),  // Top-right
      sf::Vector2f(halfSize, halfSize),   // Bottom-right
      sf::Vector2f(-halfSize, halfSize)   // Bottom-left
  };

  // Apply rotation to corners
  sf::Vector2f rotatedCorners[4];
  for (int i = 0; i < 4; i++) {
    rotatedCorners[i] = sf::Vector2f(corners[i].x * cosAngle - corners[i].y * sinAngle,
                                     corners[i].x * sinAngle + corners[i].y * cosAngle) +
                        position;
  }

  // Define texture coordinates based on textureId (same as BoardDiamond)
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

  // Create two triangles to form a quad (6 vertices total)
  // Note: vertexOffset should be calculated for 6 vertices per item, not 4

  // First triangle: 0, 1, 2
  vertices[vertexOffset + 0].position = rotatedCorners[0];
  vertices[vertexOffset + 0].texCoords = texCoords[0];
  vertices[vertexOffset + 0].color = sf::Color::White;

  vertices[vertexOffset + 1].position = rotatedCorners[1];
  vertices[vertexOffset + 1].texCoords = texCoords[1];
  vertices[vertexOffset + 1].color = sf::Color::White;

  vertices[vertexOffset + 2].position = rotatedCorners[2];
  vertices[vertexOffset + 2].texCoords = texCoords[2];
  vertices[vertexOffset + 2].color = sf::Color::White;

  // Second triangle: 0, 2, 3
  vertices[vertexOffset + 3].position = rotatedCorners[0];
  vertices[vertexOffset + 3].texCoords = texCoords[0];
  vertices[vertexOffset + 3].color = sf::Color::White;

  vertices[vertexOffset + 4].position = rotatedCorners[2];
  vertices[vertexOffset + 4].texCoords = texCoords[2];
  vertices[vertexOffset + 4].color = sf::Color::White;

  vertices[vertexOffset + 5].position = rotatedCorners[3];
  vertices[vertexOffset + 5].texCoords = texCoords[3];
  vertices[vertexOffset + 5].color = sf::Color::White;
}