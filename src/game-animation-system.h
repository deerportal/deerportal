#ifndef GAME_ANIMATION_SYSTEM_H
#define GAME_ANIMATION_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
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
  
  // Your preferred approach: Simple circle burst effect
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
  };
  std::vector<CircleParticle> m_circleParticles;
  std::unique_ptr<sf::Sprite> m_particleSprite; // Reused for all particles
  
  // VertexArray optimization for batched particle rendering
  sf::VertexArray m_particleVertices;
  sf::Texture* m_particleTexture;

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
  void addParticleToVertexArray(const CircleParticle& particle);
};

} // namespace DP

#endif // GAME_ANIMATION_SYSTEM_H