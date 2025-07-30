#pragma once
#include <SFML/Graphics.hpp>

struct BoardAnimationConfig {
  float animationDuration = 2.5f;
  float rotationSpeed = 180.0f;
  float spawnRadius = 200.0f;
  float startScale = 0.3f;
  float endScale = 1.0f;
  float staggerDelay = 0.05f;
  bool enableRotation = true;
};

class AnimatedBoardItem {
private:
  sf::Vector2f spawnPoint, targetPoint;
  sf::Vector2f bezierPoints[4];
  float progress = 0.0f;
  float rotationAngle = 0.0f;
  float currentScale = 0.3f;
  int diamondId = 0;
  int textureId = 0; // For selecting correct sprite from texture atlas
  bool finished = false;

  void initializeBezierPath(sf::Vector2f spawn, sf::Vector2f target);
  sf::Vector2f calculateBezierPosition(float t) const;
  float easeOutCubic(float t) const;

public:
  void initialize(int id, sf::Vector2f spawn, sf::Vector2f target, int texId = 0);
  void update(sf::Time deltaTime, const BoardAnimationConfig& config);
  void addToVertexArray(sf::VertexArray& vertices, int vertexOffset,
                        const sf::Texture& texture) const;
  bool isFinished() const { return finished; }
  sf::Vector2f getCurrentPosition() const;
  float getCurrentScale() const { return currentScale; }
  float getCurrentRotation() const { return rotationAngle; }
  float getProgress() const { return progress; }
  sf::Vector2f getSpawnPoint() const { return spawnPoint; }
  int getTextureId() const { return textureId; }
};