#ifndef GAME_ANIMATION_SYSTEM_H
#define GAME_ANIMATION_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace DP {

// Forward declarations
class Game;

/*!
 * \brief GameAnimationSystem handles all animation logic and visual effects
 *
 * This class manages oscillators, particle systems, sprite animations,
 * and other time-based visual effects in the game.
 */
class GameAnimationSystem {
public:
  GameAnimationSystem(Game* gameInstance);
  ~GameAnimationSystem();

  // Main update method
  void update(sf::Time frameTime);

  // Animation control
  void updateBigDiamondAnimation(sf::Time frameTime);
  void updateCharacterAnimations(sf::Time frameTime);
  void updateParticleEffects(sf::Time frameTime);
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

  // Particle system control (currently disabled for performance)
  void enableParticleSystem(bool enable);
  void updateParticleSystem(sf::Time frameTime);

  // Animation queries
  bool isBigDiamondAnimating() const;
  bool isCharacterAnimating(int playerId) const;
  bool hasActiveAnimations() const;

private:
  Game* game; // Reference to main game instance

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

  // Particle system control
  bool particleSystemEnabled;

  // Helper methods
  void initializeAnimationStates();
  void updateSingleCharacterAnimation(int playerId, sf::Time frameTime);
  void cleanupFinishedEffects();
  float calculateOscillatorModifier() const;
};

} // namespace DP

#endif // GAME_ANIMATION_SYSTEM_H