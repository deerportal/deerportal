#include "game-animation-system.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstdint>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "game.h"

namespace DP {

GameAnimationSystem::GameAnimationSystem(Game* gameInstance)
    : game(gameInstance), oscillator(-1.0f), oscillatorInc(true), oscillatorSpeed(1.0f),
      bigDiamondAnimationActive(false), bigDiamondBasePosition(474.0f, 342.0f)
{
  initializeAnimationStates();
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
  
  // Update circle particles (your preferred approach)
  updateCircleParticles(frameTime);

  // Note: Particle system intentionally skipped as per user request
}

void GameAnimationSystem::addEffect(AnimationEffect effect) {
    m_effects.push_back(effect);
}

// Diamond collection effect - multi-stage animation as per Gemini plan
void GameAnimationSystem::createDiamondCollectionEffect(sf::Sprite* diamondSprite, sf::Vector2f playerHudPos) {
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
        fly.onComplete = [diamondSprite]() {
            diamondSprite->setColor(sf::Color::Transparent);
        };
        
        addEffect(fly);
    };
    
    addEffect(pop);
}

// Overloaded method for board position (works with vertex array system)
void GameAnimationSystem::createDiamondCollectionEffect(int boardPosition, sf::Vector2f playerHudPos) {
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
                m_temporarySprites.end()
            );
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

// Your preferred approach: Simple circle burst effect
void GameAnimationSystem::createDiamondCollectionBurst(sf::Vector2f position) {
    const int particleCount = 6;
    const float radius = 30.0f;
    const float speed = 120.0f; // Increased speed for better visibility
    
#ifndef NDEBUG
    std::cout << "DEBUG: Creating diamond collection burst at position: " << position.x << ", " << position.y << std::endl;
#endif
    
    // Create particle sprite if not exists
    if (!m_particleSprite) {
        m_particleSprite = std::make_unique<sf::Sprite>(game->textures.textureBoardDiamond);
        m_particleSprite->setTextureRect(sf::IntRect({4 * 44, 0}, {44, 44})); // Diamond texture
        m_particleSprite->setScale({0.5f, 0.5f}); // Larger for better visibility
#ifndef NDEBUG
        std::cout << "DEBUG: Created particle sprite" << std::endl;
#endif
    }
    
    // Create particles in a circle
    for (int i = 0; i < particleCount; ++i) {
        CircleParticle particle;
        
        // Calculate angle for even distribution
        float angle = (i * 2.0f * M_PI) / particleCount;
        
        // Start position (center)
        particle.position = position;
        
        // Velocity in circle pattern
        particle.velocity = sf::Vector2f(
            std::cos(angle) * speed,
            std::sin(angle) * speed
        );
        
        // Lifetime - longer for better visibility
        particle.lifetime = sf::seconds(1.2f);
        particle.totalLifetime = sf::seconds(1.2f);
        particle.active = true;
        
        m_circleParticles.push_back(particle);
    }
    
#ifndef NDEBUG
    std::cout << "DEBUG: Created " << particleCount << " particles. Total particles: " << m_circleParticles.size() << std::endl;
#endif
}

// Draw circle particles (called from event loop)
void GameAnimationSystem::drawCircleParticles(sf::RenderTarget& target) const {
#ifndef NDEBUG
    // Debug output every time when particles exist
    if (!m_circleParticles.empty()) {
        std::cout << "DEBUG: drawCircleParticles called with " << m_circleParticles.size() << " particles" << std::endl;
    }
#endif
    
    if (!m_particleSprite) {
#ifndef NDEBUG
        if (!m_circleParticles.empty()) {
            std::cout << "DEBUG: ERROR - No particle sprite available but have " << m_circleParticles.size() << " particles!" << std::endl;
        }
#endif
        return;
    }
    
    int activeCount = 0;
    for (const auto& particle : m_circleParticles) {
        if (particle.active) {
            activeCount++;
            // Update sprite position
            m_particleSprite->setPosition(particle.position);
            
            // Make particles VERY visible - bright yellow, no fade
            m_particleSprite->setColor(sf::Color::Yellow);
            
#ifndef NDEBUG
            std::cout << "DEBUG: Drawing particle at " << particle.position.x << ", " << particle.position.y << std::endl;
#endif
            
            // Draw the particle
            target.draw(*m_particleSprite);
        }
    }
    
#ifndef NDEBUG
    if (activeCount > 0) {
        std::cout << "DEBUG: Drawing " << activeCount << " active particles" << std::endl;
    }
#endif
}

// Update circle particles in the event loop (your preferred approach)
void GameAnimationSystem::updateCircleParticles(sf::Time frameTime) {
    // Update all particles
    int activeCount = 0;
    for (auto& particle : m_circleParticles) {
        if (particle.active) {
            activeCount++;
            // Update position
            particle.position += particle.velocity * frameTime.asSeconds();
            
            // Update lifetime
            particle.lifetime -= frameTime;
            
            // Deactivate if expired
            if (particle.lifetime <= sf::Time::Zero) {
                particle.active = false;
            }
        }
    }
    
#ifndef NDEBUG
    if (activeCount > 0) {
        std::cout << "DEBUG: Updating " << activeCount << " active particles" << std::endl;
    }
#endif
    
    // Clean up inactive particles
    size_t beforeSize = m_circleParticles.size();
    m_circleParticles.erase(
        std::remove_if(m_circleParticles.begin(), m_circleParticles.end(),
            [](const CircleParticle& p) { return !p.active; }),
        m_circleParticles.end()
    );
    
#ifndef NDEBUG
    if (beforeSize != m_circleParticles.size()) {
        std::cout << "DEBUG: Cleaned up particles, before: " << beforeSize << ", after: " << m_circleParticles.size() << std::endl;
    }
#endif
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
                sf::Vector2f newPos = effect.move.startPos + (effect.move.endPos - effect.move.startPos) * effect.move.ease(t);
                effect.target->setPosition(newPos);
            }

            // Scale
            if (effect.scale.endScale != effect.scale.startScale) {
                sf::Vector2f newScale = effect.scale.startScale + (effect.scale.endScale - effect.scale.startScale) * effect.scale.ease(t);
                effect.target->setScale(newScale);
            }

            // Fade
            if (effect.fade.endAlpha != effect.fade.startAlpha) {
                float newAlpha = effect.fade.startAlpha + (effect.fade.endAlpha - effect.fade.startAlpha) * effect.fade.ease(t);
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

} // namespace DP