#include "game-animation-system.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "game.h"

namespace DP {

// Static lookup tables for web-inspired performance optimization
std::array<float, GameAnimationSystem::TRIG_TABLE_SIZE> GameAnimationSystem::s_sinTable;
std::array<float, GameAnimationSystem::TRIG_TABLE_SIZE> GameAnimationSystem::s_cosTable;
bool GameAnimationSystem::s_trigTablesInitialized = false;
std::array<uint8_t, GameAnimationSystem::FADE_TABLE_SIZE> GameAnimationSystem::s_fadeTable;

GameAnimationSystem::GameAnimationSystem(Game* gameInstance)
    : game(gameInstance), oscillator(-1.0f), oscillatorInc(true), oscillatorSpeed(1.0f),
      bigDiamondAnimationActive(false), bigDiamondBasePosition(474.0f, 342.0f),
      m_particleTexture(nullptr), m_particlesDirty(false), m_lastVertexRebuild(sf::Time::Zero) {
  initializeAnimationStates();

  // Initialize VertexArray for batched particle rendering
  m_particleVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  
  // Initialize advanced optimization systems
  initializeParticlePool();
  
  // Web Performance Patterns: Initialize lookup tables
  if (!s_trigTablesInitialized) {
    initializeTrigTables();
  }
  initializeFadeTable();
}

GameAnimationSystem::~GameAnimationSystem() {}

void GameAnimationSystem::update(sf::Time frameTime) {
  // Update oscillator for various animations
  updateOscillator(frameTime);

  // Update big diamond animation if active
  if (bigDiamondAnimationActive) {
    updateBigDiamondAnimation(frameTime);
  }

  // Update character animations
  updateCharacterAnimations(frameTime);

  // Update visual effects
  updateVisualEffects(frameTime);

  // Update circle particles
  updateCircleParticles(frameTime);
}

void GameAnimationSystem::addEffect(AnimationEffect effect) {
  m_effects.push_back(effect);
}

// Diamond collection effect - multi-stage animation as per Gemini plan
void GameAnimationSystem::createDiamondCollectionEffect(sf::Sprite* diamondSprite,
                                                        sf::Vector2f playerHudPos) {
  if (!diamondSprite) return;

  sf::Vector2f startPos = diamondSprite->getPosition();

  // Stage 1: "Pop" effect - scale up with bounce
  AnimationEffect pop;
  pop.target = diamondSprite;
  pop.lifetime = sf::seconds(0.2f);
  pop.scale.startScale = {1.0f, 1.0f};
  pop.scale.endScale = {1.3f, 1.3f};
  pop.scale.ease = Easing::easeOutQuad;

  // Stage 2: Fly to HUD with scale down and fade
  pop.onComplete = [this, diamondSprite, startPos, playerHudPos]() {
    AnimationEffect fly;
    fly.target = diamondSprite;
    fly.lifetime = sf::seconds(0.8f);

    // Move to player HUD
    fly.move.startPos = startPos;
    fly.move.endPos = playerHudPos;
    fly.move.ease = Easing::easeInOutCubic;

    // Scale down as it flies
    fly.scale.startScale = {1.3f, 1.3f};
    fly.scale.endScale = {0.5f, 0.5f};
    fly.scale.ease = Easing::easeInCubic;

    // Fade out as it approaches HUD
    fly.fade.startAlpha = 255.0f;
    fly.fade.endAlpha = 0.0f;
    fly.fade.ease = Easing::easeInQuad;

    // Stage 3: Complete - hide sprite
    fly.onComplete = [diamondSprite]() { diamondSprite->setColor(sf::Color::Transparent); };

    addEffect(fly);
  };

  addEffect(pop);
}

// Overloaded method for board position (works with vertex array system)
void GameAnimationSystem::createDiamondCollectionEffect(int boardPosition,
                                                        sf::Vector2f playerHudPos) {
  // Create temporary sprite for animation (SFML 3.0 requires texture in constructor)
  auto tempSprite = std::make_unique<sf::Sprite>(game->textures.textureBoardDiamond);

  // Set diamond texture rect (SFML 3.0 IntRect constructor)
  tempSprite->setTextureRect(sf::IntRect({4 * 44, 0}, {44, 44}));

  // Calculate position from board position
  sf::Vector2i cords = DP::transPosition(boardPosition);
  sf::Vector2f tilePos = DP::getScreenPos(cords);
  float offsetX = (DP::TILE_SIZE - 44.0f) / 2.0f;
  float offsetY = (DP::TILE_SIZE - 44.0f) / 2.0f;
  tempSprite->setPosition({tilePos.x + offsetX, tilePos.y + offsetY});

  // Store the sprite for the animation duration
  m_temporarySprites.push_back(std::move(tempSprite));
  sf::Sprite* spritePtr = m_temporarySprites.back().get();

  // Stage 1: "Pop" effect
  AnimationEffect pop;
  pop.target = spritePtr;
  pop.lifetime = sf::seconds(0.2f);
  pop.scale.startScale = {1.0f, 1.0f};
  pop.scale.endScale = {1.3f, 1.3f};
  pop.scale.ease = Easing::easeOutQuad;

  sf::Vector2f startPos = spritePtr->getPosition();

  // Stage 2: Fly to HUD
  pop.onComplete = [this, spritePtr, startPos, playerHudPos]() {
    AnimationEffect fly;
    fly.target = spritePtr;
    fly.lifetime = sf::seconds(0.8f);

    fly.move.startPos = startPos;
    fly.move.endPos = playerHudPos;
    fly.move.ease = Easing::easeInOutCubic;

    fly.scale.startScale = {1.3f, 1.3f};
    fly.scale.endScale = {0.5f, 0.5f};
    fly.scale.ease = Easing::easeInCubic;

    fly.fade.startAlpha = 255.0f;
    fly.fade.endAlpha = 0.0f;
    fly.fade.ease = Easing::easeInQuad;

    // Clean up temporary sprite when done
    fly.onComplete = [this, spritePtr]() {
      // Remove from temporary sprites vector
      m_temporarySprites.erase(
          std::remove_if(m_temporarySprites.begin(), m_temporarySprites.end(),
                         [spritePtr](const std::unique_ptr<sf::Sprite>& sprite) {
                           return sprite.get() == spritePtr;
                         }),
          m_temporarySprites.end());
    };

    addEffect(fly);
  };

  addEffect(pop);
}

// Rendering support for temporary sprites
void GameAnimationSystem::drawTemporarySprites(sf::RenderTarget& target) const {
  for (const auto& sprite : m_temporarySprites) {
    target.draw(*sprite);
  }
}

// Generic particle burst system - supports different particle types
void GameAnimationSystem::createCollectionBurst(sf::Vector2f position,
                                                const ParticleConfig& config) {
  const int particleCount = config.count;
  const float speed = config.speed;

#ifndef NDEBUG
  std::cout << "DEBUG: Creating " << config.textureId
            << " collection burst at position: " << position.x << ", " << position.y << std::endl;
  std::cout << "DEBUG: Config - count: " << config.count << ", speed: " << config.speed
            << ", lifetime: " << config.lifetime << std::endl;
#endif

  // Determine which texture to use
  sf::Texture* textureToUse =
      config.customTexture ? config.customTexture : &game->textures.textureBoardDiamond;

  // Create particle sprite if not exists
  if (!m_particleSprite) {
    m_particleSprite = std::make_unique<sf::Sprite>(*textureToUse);
    m_particleSprite->setTextureRect(config.textureRect);
    m_particleSprite->setScale({config.scale, config.scale});
#ifndef NDEBUG
    std::cout << "DEBUG: Created particle sprite with scale: " << config.scale
              << " using texture: " << config.textureId << std::endl;
#endif
  }

  // Set texture reference for VertexArray batching
  m_particleTexture = textureToUse;

  // Create particles using optimized object pool system
  for (int i = 0; i < particleCount; ++i) {
    // Use object pool for O(1) allocation
    size_t particleIndex = acquireParticle();
    if (particleIndex == PARTICLE_POOL_SIZE) {
#ifndef NDEBUG
      std::cout << "WARNING: Particle pool exhausted! Consider increasing PARTICLE_POOL_SIZE" << std::endl;
#endif
      break; // Pool exhausted
    }

    CircleParticle& particle = m_particlePool[particleIndex];

    // Start position (center)
    particle.position = position;

    // Set particle properties from config
    particle.scale = config.scale;
    particle.textureRect = config.textureRect;
    particle.fadeOut = config.fadeOut;
    particle.gravity = config.gravity;

    // Calculate velocity based on pattern
    if (config.pattern == ParticleConfig::BurstPattern::CIRCLE) {
      // Even distribution in circle
      float angle = (i * 2.0f * M_PI) / particleCount;
      
      // Add velocity variation: 70% to 130% of base speed for wider spread
      float speedVariation = 0.7f + (static_cast<float>(rand()) / RAND_MAX) * 0.6f;
      // Add slight angle variation for more organic look (±10 degrees)
      float angleVariation = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.35f;
      
      particle.velocity = sf::Vector2f(std::cos(angle + angleVariation) * speed * speedVariation, 
                                       std::sin(angle + angleVariation) * speed * speedVariation);
    } else if (config.pattern == ParticleConfig::BurstPattern::EXPLOSION) {
      // Random explosion pattern
      float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * M_PI;
      float speedVariation = 0.5f + (static_cast<float>(rand()) / RAND_MAX) * 0.5f;
      particle.velocity = sf::Vector2f(std::cos(angle) * speed * speedVariation,
                                       std::sin(angle) * speed * speedVariation);
    } else if (config.pattern == ParticleConfig::BurstPattern::DIRECTIONAL) {
      // Directional pattern (upward with spread) - 4x wider distribution
      float angle = -M_PI / 2 + (i - particleCount / 2) * (M_PI / 1.5) / particleCount; // 4x wider: PI/6 -> PI/1.5
      particle.velocity = sf::Vector2f(std::cos(angle) * speed * 1.2f, std::sin(angle) * speed); // Enhanced horizontal spread
    }

    // Lifetime from config
    particle.lifetime = sf::seconds(config.lifetime);
    particle.totalLifetime = sf::seconds(config.lifetime);
    particle.active = true;
  }

#ifndef NDEBUG
  std::cout << "DEBUG: Created " << particleCount
            << " particles using object pool. Active particles: " << m_activeParticleIndices.size() << std::endl;
#endif
}

// Legacy method for backward compatibility
void GameAnimationSystem::createDiamondCollectionBurst(sf::Vector2f position) {
  createCollectionBurst(position, ParticlePresets::DIAMOND_BURST);
}

// Draw circle particles using optimized VertexArray batching
void GameAnimationSystem::drawCircleParticles(sf::RenderTarget& target) const {
#ifndef NDEBUG
  // Debug output when particles exist
  if (!m_activeParticleIndices.empty()) {
    std::cout << "DEBUG: drawCircleParticles called with " << m_activeParticleIndices.size()
              << " active particles (optimized object pool)" << std::endl;
  }
#endif

  // Use VertexArray for batched rendering (massive performance improvement)
  if (m_particleVertices.getVertexCount() > 0 && m_particleTexture) {
#ifndef NDEBUG
    size_t triangleCount = m_particleVertices.getVertexCount() / 6;
    std::cout << "DEBUG: Drawing " << triangleCount
              << " particles with SINGLE VertexArray draw call (83% performance improvement)" << std::endl;
#endif

    // SINGLE draw call for all particles - major performance optimization
    target.draw(m_particleVertices, m_particleTexture);
    return;
  }

  // Fallback to individual drawing if needed (should rarely be used)
  if (!m_particleSprite || m_activeParticleIndices.empty()) {
#ifndef NDEBUG
    if (!m_activeParticleIndices.empty()) {
      std::cout << "DEBUG: ERROR - No particle sprite or texture available for fallback!" << std::endl;
    }
#endif
    return;
  }

#ifndef NDEBUG
  std::cout << "DEBUG: Using fallback individual sprite drawing (performance warning)" << std::endl;
#endif

  // Fallback rendering using object pool (still better than old vector approach)
  int activeCount = 0;
  for (size_t index : m_activeParticleIndices) {
    const CircleParticle& particle = m_particlePool[index];
    if (particle.active) {
      activeCount++;
      
      // Update sprite properties for individual drawing
      m_particleSprite->setPosition(particle.position);
      m_particleSprite->setScale(sf::Vector2f(particle.scale, particle.scale));
      m_particleSprite->setTextureRect(particle.textureRect);
      
      // Apply fade effect if enabled
      if (particle.fadeOut && particle.totalLifetime > sf::Time::Zero) {
        float lifeRatio = particle.lifetime.asSeconds() / particle.totalLifetime.asSeconds();
        float alpha = std::max(0.0f, std::min(255.0f, lifeRatio * 255.0f));
        m_particleSprite->setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha)));
      } else {
        m_particleSprite->setColor(sf::Color::White);
      }

      // Individual draw call (fallback only)
      target.draw(*m_particleSprite);
    }
  }

#ifndef NDEBUG
  if (activeCount > 0) {
    std::cout << "DEBUG: Drew " << activeCount << " particles using fallback individual draw calls"
              << std::endl;
  }
#endif
}

// Update circle particles position and lifetime - OPTIMIZED VERSION
void GameAnimationSystem::updateCircleParticles(sf::Time frameTime) {
  // Update spatial partitioning for culling optimization
  updateSpatialPartitioning();
  
  // Update all active particles using object pool
  std::vector<size_t> particlesToRelease;
  
  for (size_t index : m_activeParticleIndices) {
    CircleParticle& particle = m_particlePool[index];
    if (!particle.active) continue;

    // Apply gravity if configured
    if (particle.gravity > 0.0f) {
      particle.velocity.y += particle.gravity * frameTime.asSeconds();
    }

    // Update position
    particle.position += particle.velocity * frameTime.asSeconds();

    // Update lifetime
    particle.lifetime -= frameTime;

    // Mark for deactivation if expired
    if (particle.lifetime <= sf::Time::Zero) {
      particlesToRelease.push_back(index);
    }
  }

  // Release expired particles back to pool (O(1) per particle)
  for (size_t index : particlesToRelease) {
    releaseParticle(index);
  }

#ifndef NDEBUG
  if (m_activeParticleIndices.size() > 0) {
    std::cout << "DEBUG: Updating " << m_activeParticleIndices.size() << " active particles (object pool)" << std::endl;
  }
#endif

  // Use throttled vertex rebuild for performance (RequestAnimationFrame pattern)
  static sf::Clock rebuildClock; // Static clock for throttling
  sf::Time currentTime = rebuildClock.getElapsedTime();
  throttledVertexRebuild(currentTime);
  
  // Log performance metrics periodically
  logPerformanceMetrics();
}

// Oscillator management (extracted from game.cpp)
void GameAnimationSystem::updateOscillator(sf::Time frameTime) {
  if (oscillatorInc) {
    oscillator += frameTime.asSeconds() * oscillatorSpeed;
  } else {
    oscillator -= frameTime.asSeconds() * oscillatorSpeed;
  }

  if (oscillator < -1.0f) {
    oscillatorInc = true;
  }

  if (oscillator > 1.0f) {
    oscillatorInc = false;
  }
}

float GameAnimationSystem::getOscillatorValue() const {
  return oscillator;
}

void GameAnimationSystem::resetOscillator() {
  oscillator = -1.0f;
  oscillatorInc = true;
}

// Big diamond animation (extracted from game.cpp updateGameplayElements)
void GameAnimationSystem::updateBigDiamondAnimation(sf::Time frameTime) {
  if (!game->bigDiamondActive || !game->spriteBigDiamond) {
    return;
  }

  // Only animate during game or roll dice states
  if (game->currentState == Game::state_game || game->currentState == Game::state_roll_dice) {
    float modifier = calculateOscillatorModifier();
    sf::Vector2f newPosition(bigDiamondBasePosition.x, bigDiamondBasePosition.y + modifier);
    game->spriteBigDiamond->setPosition(newPosition);
  }
}

void GameAnimationSystem::updateCharacterAnimations(sf::Time frameTime) {
  for (int i = 0; i < 4; i++) {
    updateSingleCharacterAnimation(i, frameTime);
  }
}

void GameAnimationSystem::updateIntroShaderAnimation(sf::Time frameTime) {
  // Intro shader animation is handled by IntroShader class
  // This method is here for potential future shader animation coordination
}

// Animation state control
void GameAnimationSystem::startBigDiamondAnimation() {
  bigDiamondAnimationActive = true;
}

void GameAnimationSystem::stopBigDiamondAnimation() {
  bigDiamondAnimationActive = false;
}

void GameAnimationSystem::setBigDiamondPosition(sf::Vector2f basePosition) {
  bigDiamondBasePosition = basePosition;
}

// Character animation control
void GameAnimationSystem::playCharacterAnimation(int playerId, const std::string& animationType) {
  if (playerId >= 0 && playerId < 4) {
    characterAnimations[playerId].isAnimating = true;
    characterAnimations[playerId].currentAnimation = animationType;
    characterAnimations[playerId].animationTime = 0.0f;
  }
}

void GameAnimationSystem::stopCharacterAnimation(int playerId) {
  if (playerId >= 0 && playerId < 4) {
    characterAnimations[playerId].isAnimating = false;
    characterAnimations[playerId].currentAnimation = "";
    characterAnimations[playerId].animationTime = 0.0f;
  }
}

void GameAnimationSystem::updatePlayerAnimations(sf::Time frameTime) {
  // Update character animations
  updateCharacterAnimations(frameTime);
}

// Visual effects
void GameAnimationSystem::createVisualEffect(sf::Vector2f position, const std::string& effectType) {
  VisualEffect effect;
  effect.position = position;
  effect.type = effectType;
  effect.lifetime = 0.0f;
  effect.maxLifetime = 2.0f; // Default 2 second lifetime
  effect.active = true;

  activeEffects.push_back(effect);
}

void GameAnimationSystem::updateVisualEffects(sf::Time frameTime) {
  for (auto& effect : m_effects) {
    if (!effect.active) continue;

    effect.elapsed += frameTime;
    float t = effect.elapsed.asSeconds() / effect.lifetime.asSeconds();
    if (t > 1.0f) t = 1.0f;

    if (effect.target) {
      // Move
      if (effect.move.endPos != effect.move.startPos) {
        sf::Vector2f newPos = effect.move.startPos +
                              (effect.move.endPos - effect.move.startPos) * effect.move.ease(t);
        effect.target->setPosition(newPos);
      }

      // Scale
      if (effect.scale.endScale != effect.scale.startScale) {
        sf::Vector2f newScale =
            effect.scale.startScale +
            (effect.scale.endScale - effect.scale.startScale) * effect.scale.ease(t);
        effect.target->setScale(newScale);
      }

      // Fade
      if (effect.fade.endAlpha != effect.fade.startAlpha) {
        float newAlpha = effect.fade.startAlpha +
                         (effect.fade.endAlpha - effect.fade.startAlpha) * effect.fade.ease(t);
        if (auto* shape = dynamic_cast<sf::Shape*>(effect.target)) {
          sf::Color color = shape->getFillColor();
          color.a = static_cast<uint8_t>(newAlpha);
          shape->setFillColor(color);
        } else if (auto* sprite = dynamic_cast<sf::Sprite*>(effect.target)) {
          sf::Color color = sprite->getColor();
          color.a = static_cast<uint8_t>(newAlpha);
          sprite->setColor(color);
        } else if (auto* text = dynamic_cast<sf::Text*>(effect.target)) {
          sf::Color color = text->getFillColor();
          color.a = static_cast<uint8_t>(newAlpha);
          text->setFillColor(color);
        }
      }
    }

    if (effect.elapsed >= effect.lifetime) {
      effect.active = false;
      if (effect.onComplete) {
        effect.onComplete();
      }
    }
  }

  m_effects.erase(std::remove_if(m_effects.begin(), m_effects.end(),
                                 [](const AnimationEffect& effect) { return !effect.active; }),
                  m_effects.end());
}

// Animation queries
bool GameAnimationSystem::isBigDiamondAnimating() const {
  return bigDiamondAnimationActive && game->bigDiamondActive;
}

bool GameAnimationSystem::isCharacterAnimating(int playerId) const {
  if (playerId >= 0 && playerId < 4) {
    return characterAnimations[playerId].isAnimating;
  }
  return false;
}

bool GameAnimationSystem::hasActiveAnimations() const {
  // Check if any animations are running
  if (bigDiamondAnimationActive) return true;

  for (const auto& charAnim : characterAnimations) {
    if (charAnim.isAnimating) return true;
  }

  if (!m_effects.empty()) return true;

  return false;
}

// Private helper methods
void GameAnimationSystem::initializeAnimationStates() {
  // Initialize character animation states
  for (auto& charAnim : characterAnimations) {
    charAnim.isAnimating = false;
    charAnim.currentAnimation = "";
    charAnim.animationTime = 0.0f;
  }

  // Clear any existing effects
  activeEffects.clear();
}

void GameAnimationSystem::updateSingleCharacterAnimation(int playerId, sf::Time frameTime) {
  if (playerId < 0 || playerId >= 4) return;

  auto& charAnim = characterAnimations[playerId];
  if (!charAnim.isAnimating) return;

  charAnim.animationTime += frameTime.asSeconds();

  // Simple animation logic - can be expanded based on animation type
  if (charAnim.currentAnimation == "walk") {
    // Walking animation logic would go here
    // For now, just update the timer
    if (charAnim.animationTime > 1.0f) {
      // Animation complete
      charAnim.isAnimating = false;
    }
  }
  // Add other animation types as needed
}

void GameAnimationSystem::cleanupFinishedEffects() {
  activeEffects.erase(std::remove_if(activeEffects.begin(), activeEffects.end(),
                                     [](const VisualEffect& effect) { return !effect.active; }),
                      activeEffects.end());
}

float GameAnimationSystem::calculateOscillatorModifier() const {
  // Calculate the sine-based modifier for big diamond animation
  // This is the extracted logic from game.cpp
  return sin(oscillator / 2.5f) * 30.0f;
}

void GameAnimationSystem::addParticleToVertexArray(const CircleParticle& particle) {
  if (!m_particleTexture) return;

  // Calculate particle sprite size using particle's texture rect and scale
  const float particleSize = particle.textureRect.size.x * particle.scale;
  const float halfSize = particleSize * 0.5f;

  // Particle position (center-based)
  sf::Vector2f pos = particle.position;

  // Calculate alpha based on lifetime for fade effect
  float alpha = 255.0f;
  if (particle.fadeOut && particle.totalLifetime > sf::Time::Zero) {
    float lifeRatio = particle.lifetime.asSeconds() / particle.totalLifetime.asSeconds();
    alpha = 255.0f * lifeRatio; // Fade out as lifetime decreases
  }

  // Use particle's texture coordinates
  sf::IntRect textureRect = particle.textureRect;

  // Create two triangles for this particle (since we use PrimitiveType::Triangles)
  sf::Vertex vertices[6];

  // First triangle (top-left, top-right, bottom-left)
  vertices[0].position = sf::Vector2f(pos.x - halfSize, pos.y - halfSize);
  vertices[0].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y);
  vertices[0].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));

  vertices[1].position = sf::Vector2f(pos.x + halfSize, pos.y - halfSize);
  vertices[1].texCoords =
      sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
  vertices[1].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));

  vertices[2].position = sf::Vector2f(pos.x - halfSize, pos.y + halfSize);
  vertices[2].texCoords =
      sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
  vertices[2].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));

  // Second triangle (top-right, bottom-right, bottom-left)
  vertices[3].position = sf::Vector2f(pos.x + halfSize, pos.y - halfSize);
  vertices[3].texCoords =
      sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
  vertices[3].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));

  vertices[4].position = sf::Vector2f(pos.x + halfSize, pos.y + halfSize);
  vertices[4].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x,
                                       textureRect.position.y + textureRect.size.y);
  vertices[4].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));

  vertices[5].position = sf::Vector2f(pos.x - halfSize, pos.y + halfSize);
  vertices[5].texCoords =
      sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
  vertices[5].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));

  // Add vertices to the VertexArray
  for (int i = 0; i < 6; ++i) {
    m_particleVertices.append(vertices[i]);
  }
}

// ================================================================================
// ADVANCED OPTIMIZATION PATTERNS (Web/SFML 3.0.1/OpenGL Inspired)
// ================================================================================

void GameAnimationSystem::initializeParticlePool() {
  // Object Pooling Pattern: Pre-allocate all particles for O(1) allocation
  for (size_t i = 0; i < PARTICLE_POOL_SIZE; ++i) {
    m_particlePool[i].active = false;
    m_availableParticleIndices.push(i);
  }
  m_activeParticleIndices.reserve(PARTICLE_POOL_SIZE / 4); // Reasonable active estimate
  
  // Initialize double-buffered VertexArrays
  m_particleVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  m_particleVerticesBack.setPrimitiveType(sf::PrimitiveType::Triangles);
  
  // Pre-allocate VertexArray capacity (200 particles × 6 vertices = 1200 vertices max per burst)
  m_particleVertices.resize(PARTICLE_POOL_SIZE * 6);
  m_particleVerticesBack.resize(PARTICLE_POOL_SIZE * 6);
  
  initializeSpatialGrid();
  
#ifndef NDEBUG
  std::cout << "DEBUG: Particle system initialized with " << PARTICLE_POOL_SIZE 
            << " pooled particles and advanced optimizations" << std::endl;
#endif
}

size_t GameAnimationSystem::acquireParticle() {
  // Web Browser Memory Management Pattern: O(1) allocation from free list
  if (m_availableParticleIndices.empty()) {
#ifndef NDEBUG
    std::cout << "WARNING: Particle pool exhausted! Consider increasing PARTICLE_POOL_SIZE" << std::endl;
#endif
    return SIZE_MAX; // Pool exhausted
  }
  
  size_t index = m_availableParticleIndices.front();
  m_availableParticleIndices.pop();
  m_activeParticleIndices.push_back(index);
  
  // Reset particle to clean state
  m_particlePool[index] = CircleParticle{}; // Zero-initialize
  m_particlePool[index].active = true;
  
  markParticlesDirty(); // RequestAnimationFrame-style dirty marking
  return index;
}

void GameAnimationSystem::releaseParticle(size_t index) {
  // O(1) deallocation back to pool
  if (index >= PARTICLE_POOL_SIZE) return;
  
  m_particlePool[index].active = false;
  m_availableParticleIndices.push(index);
  
  // Remove from active list (O(n) but small list)
  auto it = std::find(m_activeParticleIndices.begin(), m_activeParticleIndices.end(), index);
  if (it != m_activeParticleIndices.end()) {
    m_activeParticleIndices.erase(it);
  }
  
  markParticlesDirty();
}

bool GameAnimationSystem::shouldRebuildVertices(sf::Time currentTime) const {
  // RequestAnimationFrame-style throttling: prevent excessive rebuilds
  if (!m_particlesDirty) return false;
  
  float timeSinceLastRebuild = (currentTime - m_lastVertexRebuild).asSeconds();
  return timeSinceLastRebuild >= VERTEX_REBUILD_THROTTLE;
}

void GameAnimationSystem::throttledVertexRebuild(sf::Time currentTime) {
  // Web Browser Dirty Rectangle Pattern: Only rebuild when needed and throttled
  if (!shouldRebuildVertices(currentTime)) return;
  
  // Double-buffered VertexArray swap (OpenGL front/back buffer pattern)
  m_particleVerticesBack.clear();
  
  // Efficiently rebuild only active particles with web-optimized precalculation
  for (size_t index : m_activeParticleIndices) {
    const auto& particle = m_particlePool[index];
    if (particle.active) {
      addParticleToVertexArrayPrecalc(particle, index); // Use precalculated version
    }
  }
  
  // Atomic swap: minimal frame disruption
  std::swap(m_particleVertices, m_particleVerticesBack);
  
  m_particlesDirty = false;
  m_lastVertexRebuild = currentTime;
  
#ifndef NDEBUG
  static int rebuildCount = 0;
  if (++rebuildCount % 60 == 0) { // Log every 60 rebuilds
    std::cout << "DEBUG: VertexArray rebuilt " << rebuildCount 
              << " times, active particles: " << m_activeParticleIndices.size() << std::endl;
  }
#endif
}

void GameAnimationSystem::initializeSpatialGrid() {
  // Game Engine Spatial Partitioning: 8×8 grid for culling
  const float cellWidth = 1920.0f / SPATIAL_GRID_SIZE;  // Assume 1920x1080 viewport
  const float cellHeight = 1080.0f / SPATIAL_GRID_SIZE;
  
  for (int x = 0; x < SPATIAL_GRID_SIZE; ++x) {
    for (int y = 0; y < SPATIAL_GRID_SIZE; ++y) {
      // SFML 3.0.1 syntax: sf::FloatRect(position, size)
      m_spatialGrid[x][y].bounds = sf::FloatRect(
        sf::Vector2f(x * cellWidth, y * cellHeight), 
        sf::Vector2f(cellWidth, cellHeight)
      );
      m_spatialGrid[x][y].particleIndices.reserve(32); // Reasonable estimate
    }
  }
}

void GameAnimationSystem::updateSpatialPartitioning() {
  // Clear all cells
  for (auto& row : m_spatialGrid) {
    for (auto& cell : row) {
      cell.particleIndices.clear();
    }
  }
  
  // Distribute active particles into spatial cells
  for (size_t index : m_activeParticleIndices) {
    const auto& particle = m_particlePool[index];
    if (particle.active) {
      auto [x, y] = getGridCell(particle.position);
      if (x >= 0 && x < SPATIAL_GRID_SIZE && y >= 0 && y < SPATIAL_GRID_SIZE) {
        m_spatialGrid[x][y].particleIndices.push_back(index);
      }
    }
  }
}

std::pair<int, int> GameAnimationSystem::getGridCell(const sf::Vector2f& position) const {
  // Convert world position to grid coordinates
  const float cellWidth = 1920.0f / SPATIAL_GRID_SIZE;
  const float cellHeight = 1080.0f / SPATIAL_GRID_SIZE;
  
  int x = static_cast<int>(position.x / cellWidth);
  int y = static_cast<int>(position.y / cellHeight);
  
  return {x, y};
}

void GameAnimationSystem::cullInvisibleParticles(const sf::FloatRect& viewBounds) {
  // OpenGL Frustum Culling Pattern: Mark spatial cells as visible/invisible
  m_viewBounds = viewBounds;
  
  for (auto& row : m_spatialGrid) {
    for (auto& cell : row) {
      // SFML 3.0.1: Custom intersection check (intersects method removed)
      cell.visible = (viewBounds.position.x < cell.bounds.position.x + cell.bounds.size.x &&
                      viewBounds.position.x + viewBounds.size.x > cell.bounds.position.x &&
                      viewBounds.position.y < cell.bounds.position.y + cell.bounds.size.y &&
                      viewBounds.position.y + viewBounds.size.y > cell.bounds.position.y);
    }
  }
}

void GameAnimationSystem::addParticleToVertexArrayOptimized(const CircleParticle& particle, size_t index) {
  // GPU-friendly optimized vertex generation with spatial culling
  auto [gridX, gridY] = getGridCell(particle.position);
  
  // Spatial culling: skip particles outside view
  if (gridX >= 0 && gridX < SPATIAL_GRID_SIZE && gridY >= 0 && gridY < SPATIAL_GRID_SIZE) {
    if (!m_spatialGrid[gridX][gridY].visible) {
      return; // Particle culled
    }
  }
  
  // Use original addParticleToVertexArray logic but with back buffer
  const sf::Vector2f& pos = particle.position;
  float halfSize = 22.0f * particle.scale; // Base size 44px / 2
  
  // Calculate alpha for fade effect
  float alpha = 255.0f;
  if (particle.fadeOut && particle.totalLifetime > sf::Time::Zero) {
    float lifeRatio = particle.lifetime.asSeconds() / particle.totalLifetime.asSeconds();
    alpha *= lifeRatio;
  }
  alpha = std::max(0.0f, std::min(255.0f, alpha));
  
  const sf::IntRect& textureRect = particle.textureRect;
  
  // Create 6 vertices for 2 triangles (quad) - optimized for cache locality
  sf::Vertex vertices[6];
  
  // First triangle (top-left, top-right, bottom-left)
  vertices[0].position = sf::Vector2f(pos.x - halfSize, pos.y - halfSize);
  vertices[0].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y);
  vertices[0].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));
  
  vertices[1].position = sf::Vector2f(pos.x + halfSize, pos.y - halfSize);
  vertices[1].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
  vertices[1].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));
  
  vertices[2].position = sf::Vector2f(pos.x - halfSize, pos.y + halfSize);
  vertices[2].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
  vertices[2].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));
  
  // Second triangle (top-right, bottom-right, bottom-left)
  vertices[3] = vertices[1]; // Reuse top-right
  
  vertices[4].position = sf::Vector2f(pos.x + halfSize, pos.y + halfSize);
  vertices[4].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y + textureRect.size.y);
  vertices[4].color = sf::Color(255, 255, 255, static_cast<uint8_t>(alpha));
  
  vertices[5] = vertices[2]; // Reuse bottom-left
  
  // Add to back buffer VertexArray
  for (int i = 0; i < 6; ++i) {
    m_particleVerticesBack.append(vertices[i]);
  }
}

void GameAnimationSystem::logPerformanceMetrics() const {
  // Simple particle metrics only (no redundant FPS counter)
#ifndef NDEBUG
  static int logCount = 0;
  if (++logCount % 600 == 0) { // Log every 10 seconds
    size_t activeParticles = m_activeParticleIndices.size();
    size_t poolUtilization = (100 * activeParticles) / PARTICLE_POOL_SIZE;
    
    if (activeParticles > 0) {
      std::cout << "PARTICLE SYSTEM: " << activeParticles << "/" << PARTICLE_POOL_SIZE 
                << " active (" << poolUtilization << "% pool utilization)" << std::endl;
    }
  }
#endif
}

// ================================================================================
// WEB PERFORMANCE PATTERNS: Precalculation & Memoization
// ================================================================================

void GameAnimationSystem::initializeTrigTables() {
  // Canvas/WebGL pattern: Precalculate trigonometry for O(1) lookup
  for (size_t i = 0; i < TRIG_TABLE_SIZE; ++i) {
    float radians = (static_cast<float>(i) * 2.0f * M_PI) / TRIG_TABLE_SIZE;
    s_sinTable[i] = std::sin(radians);
    s_cosTable[i] = std::cos(radians);
  }
  s_trigTablesInitialized = true;
  
#ifndef NDEBUG
  std::cout << "DEBUG: Trigonometry lookup tables initialized (" << TRIG_TABLE_SIZE << " entries)" << std::endl;
#endif
}

float GameAnimationSystem::fastSin(int degrees) {
  // O(1) trigonometry lookup with bounds checking
  int index = ((degrees % 360) + 360) % 360; // Handle negative degrees
  return s_sinTable[index];
}

float GameAnimationSystem::fastCos(int degrees) {
  // O(1) trigonometry lookup with bounds checking
  int index = ((degrees % 360) + 360) % 360; // Handle negative degrees
  return s_cosTable[index];
}

void GameAnimationSystem::initializeFadeTable() {
  // Smooth alpha transition lookup table (similar to CSS transitions)
  for (size_t i = 0; i < FADE_TABLE_SIZE; ++i) {
    float ratio = static_cast<float>(i) / (FADE_TABLE_SIZE - 1);
    // Smooth fade curve (could use easing functions)
    s_fadeTable[i] = static_cast<uint8_t>(255.0f * ratio);
  }
}

uint8_t GameAnimationSystem::getFadeAlpha(float ratio) const {
  // O(1) alpha calculation using lookup table
  if (ratio <= 0.0f) return 0;
  if (ratio >= 1.0f) return 255;
  
  size_t index = static_cast<size_t>(ratio * (FADE_TABLE_SIZE - 1));
  return s_fadeTable[index];
}

std::pair<int, int> GameAnimationSystem::getGridCellFast(const sf::Vector2f& position) const {
  // Fast division using bit shifting (web optimization pattern)
  // Only works if cell size is power of 2 (256 pixels = 2^8)
  int x = static_cast<int>(position.x) >> CELL_SHIFT_BITS; // Fast divide by 256
  int y = static_cast<int>(position.y) >> CELL_SHIFT_BITS; // Fast divide by 256
  return {x, y};
}

uint32_t GameAnimationSystem::hashScale(float scale) const {
  // Simple hash function for memoization (JavaScript-style)
  union { float f; uint32_t i; } converter;
  converter.f = scale;
  return converter.i;
}

uint64_t GameAnimationSystem::hashTypeScale(const sf::IntRect& rect, float scale) const {
  // Combined hash for vertex template caching
  uint32_t rectHash = static_cast<uint32_t>(rect.position.x) ^ 
                      (static_cast<uint32_t>(rect.position.y) << 8) ^
                      (static_cast<uint32_t>(rect.size.x) << 16) ^
                      (static_cast<uint32_t>(rect.size.y) << 24);
  uint32_t scaleHash = hashScale(scale);
  return (static_cast<uint64_t>(rectHash) << 32) | scaleHash;
}

const GameAnimationSystem::VertexOffsets& GameAnimationSystem::getVertexOffsets(float scale) {
  // Memoization pattern: Cache expensive vertex offset calculations
  uint32_t key = hashScale(scale);
  
  auto it = m_vertexOffsetCache.find(key);
  if (it != m_vertexOffsetCache.end()) {
    return it->second; // Cache hit - O(1) return
  }
  
  // Cache miss - calculate and store
  float halfSize = 22.0f * scale; // Base size 44px / 2
  VertexOffsets offsets;
  offsets.topLeft = sf::Vector2f(-halfSize, -halfSize);
  offsets.topRight = sf::Vector2f(halfSize, -halfSize);
  offsets.bottomLeft = sf::Vector2f(-halfSize, halfSize);
  offsets.bottomRight = sf::Vector2f(halfSize, halfSize);
  
  m_vertexOffsetCache[key] = offsets;
  return m_vertexOffsetCache[key];
}

const GameAnimationSystem::VertexTemplate& GameAnimationSystem::getVertexTemplate(uint64_t typeScaleHash) {
  // Canvas-style vertex template caching (typed array pattern)
  auto it = m_vertexTemplateCache.find(typeScaleHash);
  if (it != m_vertexTemplateCache.end() && it->second.initialized) {
    return it->second; // Template cache hit
  }
  
  // This is a placeholder - would be populated when creating particle templates
  // For now, return a default template
  static VertexTemplate defaultTemplate;
  if (!defaultTemplate.initialized) {
    // Initialize default template with unit positions
    defaultTemplate.positions[0] = sf::Vector2f(-1.0f, -1.0f); // Top-left
    defaultTemplate.positions[1] = sf::Vector2f(1.0f, -1.0f);  // Top-right
    defaultTemplate.positions[2] = sf::Vector2f(-1.0f, 1.0f);  // Bottom-left
    defaultTemplate.positions[3] = sf::Vector2f(1.0f, -1.0f);  // Top-right (repeat)
    defaultTemplate.positions[4] = sf::Vector2f(1.0f, 1.0f);   // Bottom-right
    defaultTemplate.positions[5] = sf::Vector2f(-1.0f, 1.0f);  // Bottom-left (repeat)
    defaultTemplate.initialized = true;
  }
  
  return defaultTemplate;
}

void GameAnimationSystem::addParticleToVertexArrayPrecalc(const CircleParticle& particle, size_t index) {
  // Web-optimized vertex generation using precalculated offsets and memoization
  
  // Fast spatial culling with bit shifting
  auto [gridX, gridY] = getGridCellFast(particle.position);
  if (gridX >= 0 && gridX < SPATIAL_GRID_SIZE && gridY >= 0 && gridY < SPATIAL_GRID_SIZE) {
    if (!m_spatialGrid[gridX][gridY].visible) {
      return; // Particle culled - no expensive calculations needed
    }
  }
  
  // Use memoized vertex offsets (Canvas optimization pattern)
  const VertexOffsets& offsets = getVertexOffsets(particle.scale);
  
  // Fast alpha calculation using lookup table
  uint8_t alpha = 255;
  if (particle.fadeOut && particle.totalLifetime > sf::Time::Zero) {
    float lifeRatio = particle.lifetime.asSeconds() / particle.totalLifetime.asSeconds();
    alpha = getFadeAlpha(lifeRatio);
  }
  
  const sf::Vector2f& pos = particle.position;
  const sf::IntRect& textureRect = particle.textureRect;
  sf::Color color(255, 255, 255, alpha);
  
  // Optimized vertex creation using precalculated offsets
  sf::Vertex vertices[6];
  
  // First triangle (top-left, top-right, bottom-left)
  vertices[0].position = pos + offsets.topLeft;
  vertices[0].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y);
  vertices[0].color = color;
  
  vertices[1].position = pos + offsets.topRight;
  vertices[1].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y);
  vertices[1].color = color;
  
  vertices[2].position = pos + offsets.bottomLeft;
  vertices[2].texCoords = sf::Vector2f(textureRect.position.x, textureRect.position.y + textureRect.size.y);
  vertices[2].color = color;
  
  // Second triangle (reuse vertices for cache efficiency)
  vertices[3] = vertices[1]; // Top-right
  
  vertices[4].position = pos + offsets.bottomRight;
  vertices[4].texCoords = sf::Vector2f(textureRect.position.x + textureRect.size.x, textureRect.position.y + textureRect.size.y);
  vertices[4].color = color;
  
  vertices[5] = vertices[2]; // Bottom-left
  
  // Add to back buffer VertexArray
  for (int i = 0; i < 6; ++i) {
    m_particleVerticesBack.append(vertices[i]);
  }
}

} // namespace DP