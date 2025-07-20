#ifndef GAME_ANIMATION_SYSTEM_H
#define GAME_ANIMATION_SYSTEM_H

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <queue>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "easing.h"

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameAnimationSystem handles all animation logic and visual effects
 *
 * This class manages oscillators, sprite animations,
 * and other time-based visual effects in the game.
 */
class GameAnimationSystem {
public:
  // Generic particle configuration for different collectible types
  struct ParticleConfig {
    // Basic properties
    int count = 6;
    float speed = 120.0f;
    float lifetime = 1.2f;
    float scale = 0.5f;

    // Texture configuration
    sf::IntRect textureRect = sf::IntRect(sf::Vector2i(4 * 44, 0), sf::Vector2i(44, 44));
    const char* textureId = "diamond";    // Use const char* for constexpr compatibility
    sf::Texture* customTexture = nullptr; // Override texture (for cards)

    // Visual effects
    bool fadeOut = true;
    bool scaleDown = false;
    float gravity = 0.0f;

    // Pattern configuration
    enum class BurstPattern { CIRCLE, EXPLOSION, DIRECTIONAL };
    BurstPattern pattern = BurstPattern::CIRCLE;
  };
  struct AnimationEffect {
    // Core properties
    sf::Time lifetime = sf::seconds(1.0f);
    sf::Time elapsed = sf::Time::Zero;
    bool active = true;

    // Target (what is being animated)
    sf::Transformable* target = nullptr; // e.g., a sprite or text

    // Modules for different kinds of animation
    struct Move {
      sf::Vector2f startPos;
      sf::Vector2f endPos;
      std::function<float(float)> ease = Easing::linear;
    } move;

    struct Scale {
      sf::Vector2f startScale;
      sf::Vector2f endScale;
      std::function<float(float)> ease = Easing::linear;
    } scale;

    struct Fade {
      float startAlpha = 255.f;
      float endAlpha = 0.f;
      std::function<float(float)> ease = Easing::linear;
    } fade;

    // Particle burst on completion
    struct ParticleBurst {
      int count = 0;
      std::string textureId; // From GameAssets
      sf::Time particleLifetime = sf::seconds(1.0f);
    } particleBurst;

    // Chaining: what to trigger when this effect finishes
    std::function<void()> onComplete;
  };

  GameAnimationSystem(Game* gameInstance);
  ~GameAnimationSystem();

  // Main update method
  void update(sf::Time frameTime);

  // A new, generic way to add effects
  void addEffect(AnimationEffect effect);

  // Convenience methods for common effects
  void createDiamondCollectionEffect(sf::Sprite* diamondSprite, sf::Vector2f playerHudPos);
  void createDiamondCollectionEffect(int boardPosition, sf::Vector2f playerHudPos);

  // Generic particle burst system (replaces createDiamondCollectionBurst)
  void createCollectionBurst(sf::Vector2f position, const ParticleConfig& config);

  // Legacy method for backward compatibility
  void createDiamondCollectionBurst(sf::Vector2f position);

  // Animation control
  void updateBigDiamondAnimation(sf::Time frameTime);
  void updateCharacterAnimations(sf::Time frameTime);
  void updateIntroShaderAnimation(sf::Time frameTime);

  // Oscillator management
  void updateOscillator(sf::Time frameTime);
  float getOscillatorValue() const;
  void resetOscillator();

  // Animation state control
  void startBigDiamondAnimation();
  void stopBigDiamondAnimation();
  void setBigDiamondPosition(sf::Vector2f basePosition);

  // Character animation control
  void playCharacterAnimation(int playerId, const std::string& animationType);
  void stopCharacterAnimation(int playerId);
  void updatePlayerAnimations(sf::Time frameTime);

  // Visual effects
  void createVisualEffect(sf::Vector2f position, const std::string& effectType);
  void updateVisualEffects(sf::Time frameTime);

  // Animation queries
  bool isBigDiamondAnimating() const;
  bool isCharacterAnimating(int playerId) const;
  bool hasActiveAnimations() const;

  // Rendering support
  void drawTemporarySprites(sf::RenderTarget& target) const;
  void drawCircleParticles(sf::RenderTarget& target) const;

private:
  Game* game; // Reference to main game instance

  std::vector<AnimationEffect> m_effects;
  std::vector<std::unique_ptr<sf::Sprite>> m_temporarySprites; // For diamond collection animations

  // Simple circle burst particles (your preferred approach)
  struct CircleParticle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Time lifetime;
    sf::Time totalLifetime;
    bool active = true;

    // Enhanced properties for generic system
    float scale = 0.5f;
    sf::IntRect textureRect = sf::IntRect(sf::Vector2i(4 * 44, 0), sf::Vector2i(44, 44));
    bool fadeOut = true;
    float gravity = 0.0f;
  };
  std::vector<CircleParticle> m_circleParticles;
  std::unique_ptr<sf::Sprite> m_particleSprite; // Reused for all particles

  // Advanced Optimization Patterns (Web/SFML 3.0.1/OpenGL inspired)
  
  // 1. Object Pooling Pattern (inspired by web browser memory management)
  static constexpr size_t PARTICLE_POOL_SIZE = 512; // Pre-allocated pool
  std::array<CircleParticle, PARTICLE_POOL_SIZE> m_particlePool;
  std::queue<size_t> m_availableParticleIndices; // Free list for O(1) allocation
  std::vector<size_t> m_activeParticleIndices;   // Active particles for iteration
  
  // 2. Dirty Flag Optimization (inspired by web browser dirty rectangles)
  bool m_particlesDirty = false;         // Track if VertexArray needs rebuilding
  sf::Time m_lastVertexRebuild = sf::Time::Zero; // Prevent excessive rebuilds
  static constexpr float VERTEX_REBUILD_THROTTLE = 1.0f / 120.0f; // Max 120 rebuilds/sec
  
  // 3. VertexArray Double Buffering (inspired by OpenGL front/back buffer)
  sf::VertexArray m_particleVertices;    // Current frame vertices
  sf::VertexArray m_particleVerticesBack; // Next frame vertices (for smooth updates)
  sf::Texture* m_particleTexture;
  
  // 4. Spatial Partitioning for Culling (inspired by game engines)
  struct SpatialCell {
    std::vector<size_t> particleIndices;
    sf::FloatRect bounds;
    bool visible = true; // Frustum culling flag
  };
  static constexpr int SPATIAL_GRID_SIZE = 8; // 8x8 grid
  std::array<std::array<SpatialCell, SPATIAL_GRID_SIZE>, SPATIAL_GRID_SIZE> m_spatialGrid;
  sf::FloatRect m_viewBounds; // Current camera view for culling
  
  // 5. Instanced Rendering Optimization (inspired by OpenGL instancing)
  struct ParticleInstanceData {
    sf::Vector2f position;
    sf::Vector2f scale;
    float rotation;
    sf::Color color;
  };
  std::vector<ParticleInstanceData> m_instanceData; // GPU-friendly data layout

  // Oscillator state
  float oscillator;
  bool oscillatorInc;
  float oscillatorSpeed;

  // Animation states
  bool bigDiamondAnimationActive;
  sf::Vector2f bigDiamondBasePosition;

  // Character animation tracking
  struct CharacterAnimationState {
    bool isAnimating;
    std::string currentAnimation;
    float animationTime;
  };
  std::array<CharacterAnimationState, 4> characterAnimations;

  // Visual effect tracking
  struct VisualEffect {
    sf::Vector2f position;
    std::string type;
    float lifetime;
    float maxLifetime;
    bool active;
  };
  std::vector<VisualEffect> activeEffects;

  // Helper methods
  void initializeAnimationStates();
  void updateSingleCharacterAnimation(int playerId, sf::Time frameTime);
  void cleanupFinishedEffects();
  float calculateOscillatorModifier() const;
  void updateCircleParticles(sf::Time frameTime);
  
  // Advanced Optimization Methods (inspired by modern web/game engines)
  
  // Object Pooling Pattern
  void initializeParticlePool();
  size_t acquireParticle(); // O(1) allocation from pool
  void releaseParticle(size_t index); // O(1) deallocation to pool
  
  // Dirty Flag & RequestAnimationFrame-style throttling
  void markParticlesDirty() { m_particlesDirty = true; }
  bool shouldRebuildVertices(sf::Time currentTime) const;
  void throttledVertexRebuild(sf::Time currentTime);
  
  // Spatial Partitioning & Frustum Culling
  void initializeSpatialGrid();
  void updateSpatialPartitioning();
  void cullInvisibleParticles(const sf::FloatRect& viewBounds);
  std::pair<int, int> getGridCell(const sf::Vector2f& position) const;
  
  // Instanced Rendering Data Preparation
  void prepareInstanceData();
  void addParticleToVertexArrayOptimized(const CircleParticle& particle, size_t index);
  
  // Simple particle metrics (no redundant FPS counter)
  void logPerformanceMetrics() const;
  void addParticleToVertexArray(const CircleParticle& particle);
};

// Predefined particle configurations for different collectible types
namespace ParticlePresets {
// Diamond collection burst (current default) - enhanced for spectacular visibility
constexpr GameAnimationSystem::ParticleConfig DIAMOND_BURST = {
    12,                                                         // count (doubled back for spectacular effects)
    180.0f,                                                     // speed (enhanced for visibility)
    1.8f,                                                       // lifetime (enhanced for visibility)
    0.8f,                                                       // scale (enhanced for visibility)
    sf::IntRect(sf::Vector2i(4 * 44, 0), sf::Vector2i(44, 44)), // textureRect
    "diamond",                                                  // textureId
    nullptr,                                                    // customTexture
    true,                                                       // fadeOut
    false,                                                      // scaleDown
    0.0f,                                                       // gravity
    GameAnimationSystem::ParticleConfig::BurstPattern::CIRCLE   // pattern
};

// Card collection effect - enhanced for spectacular visibility
constexpr GameAnimationSystem::ParticleConfig CARD_COLLECT = {
    8,                                                        // count (doubled back for spectacular effects)
    140.0f,                                                   // speed (enhanced for visibility)
    1.4f,                                                     // lifetime (enhanced for visibility)
    0.6f,                                                     // scale (enhanced for visibility)
    sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)),    // textureRect
    "card",                                                   // textureId
    nullptr,                                                  // customTexture
    true,                                                     // fadeOut
    true,                                                     // scaleDown
    0.0f,                                                     // gravity
    GameAnimationSystem::ParticleConfig::BurstPattern::CIRCLE // pattern
};

// Card collection effect (random explosion pattern) - enhanced for spectacular visibility
constexpr GameAnimationSystem::ParticleConfig CARD_COLLECT_RANDOM = {
    8,                                                           // count (doubled back for spectacular effects)
    140.0f,                                                      // speed (enhanced for visibility)
    1.4f,                                                        // lifetime (enhanced for visibility)
    0.6f,                                                        // scale (enhanced for visibility)
    sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)),       // textureRect
    "card",                                                      // textureId
    nullptr,                                                     // customTexture
    true,                                                        // fadeOut
    true,                                                        // scaleDown
    0.0f,                                                        // gravity
    GameAnimationSystem::ParticleConfig::BurstPattern::EXPLOSION // pattern
};

// Stop card effect (falling particles) - enhanced for spectacular visibility
constexpr GameAnimationSystem::ParticleConfig STOP_CARD = {
    16,                                                            // count (doubled back for spectacular effects)
    100.0f,                                                        // speed (enhanced for visibility)
    1.6f,                                                          // lifetime (enhanced for visibility)
    0.7f,                                                          // scale (enhanced for visibility)
    sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)),         // textureRect
    "stop",                                                        // textureId
    nullptr,                                                       // customTexture
    true,                                                          // fadeOut
    false,                                                         // scaleDown
    98.0f,                                                         // gravity
    GameAnimationSystem::ParticleConfig::BurstPattern::DIRECTIONAL // pattern
};
} // namespace ParticlePresets

} // namespace DP

#endif // GAME_ANIMATION_SYSTEM_H