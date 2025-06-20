#include "game-animation-system.h"
#include "game.h"
#include <cmath>
#include <iostream>

namespace DP {

GameAnimationSystem::GameAnimationSystem(Game* gameInstance)
    : game(gameInstance)
    , oscillator(-1.0f)
    , oscillatorInc(true)
    , oscillatorSpeed(1.0f)
    , bigDiamondAnimationActive(false)
    , bigDiamondBasePosition(474.0f, 342.0f)
    , particleSystemEnabled(false)  // Disabled as requested
{
    initializeAnimationStates();
}

GameAnimationSystem::~GameAnimationSystem()
{
}

void GameAnimationSystem::update(sf::Time frameTime)
{
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
    
    // Note: Particle system intentionally skipped as per user request
}

// Oscillator management (extracted from game.cpp)
void GameAnimationSystem::updateOscillator(sf::Time frameTime)
{
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

float GameAnimationSystem::getOscillatorValue() const
{
    return oscillator;
}

void GameAnimationSystem::resetOscillator()
{
    oscillator = -1.0f;
    oscillatorInc = true;
}

// Big diamond animation (extracted from game.cpp updateGameplayElements)
void GameAnimationSystem::updateBigDiamondAnimation(sf::Time frameTime)
{
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

void GameAnimationSystem::updateCharacterAnimations(sf::Time frameTime)
{
    for (int i = 0; i < 4; i++) {
        updateSingleCharacterAnimation(i, frameTime);
    }
}

void GameAnimationSystem::updateParticleEffects(sf::Time frameTime)
{
    // Particle system intentionally not implemented as per user request
    // This method is here for interface completeness
}

void GameAnimationSystem::updateIntroShaderAnimation(sf::Time frameTime)
{
    // Intro shader animation is handled by IntroShader class
    // This method is here for potential future shader animation coordination
}

// Animation state control
void GameAnimationSystem::startBigDiamondAnimation()
{
    bigDiamondAnimationActive = true;
}

void GameAnimationSystem::stopBigDiamondAnimation()
{
    bigDiamondAnimationActive = false;
}

void GameAnimationSystem::setBigDiamondPosition(sf::Vector2f basePosition)
{
    bigDiamondBasePosition = basePosition;
}

// Character animation control
void GameAnimationSystem::playCharacterAnimation(int playerId, const std::string& animationType)
{
    if (playerId >= 0 && playerId < 4) {
        characterAnimations[playerId].isAnimating = true;
        characterAnimations[playerId].currentAnimation = animationType;
        characterAnimations[playerId].animationTime = 0.0f;
    }
}

void GameAnimationSystem::stopCharacterAnimation(int playerId)
{
    if (playerId >= 0 && playerId < 4) {
        characterAnimations[playerId].isAnimating = false;
        characterAnimations[playerId].currentAnimation = "";
        characterAnimations[playerId].animationTime = 0.0f;
    }
}

void GameAnimationSystem::updatePlayerAnimations(sf::Time frameTime)
{
    // Update character animations
    updateCharacterAnimations(frameTime);
}

// Visual effects
void GameAnimationSystem::createVisualEffect(sf::Vector2f position, const std::string& effectType)
{
    VisualEffect effect;
    effect.position = position;
    effect.type = effectType;
    effect.lifetime = 0.0f;
    effect.maxLifetime = 2.0f; // Default 2 second lifetime
    effect.active = true;
    
    activeEffects.push_back(effect);
}

void GameAnimationSystem::updateVisualEffects(sf::Time frameTime)
{
    for (auto& effect : activeEffects) {
        if (effect.active) {
            effect.lifetime += frameTime.asSeconds();
            if (effect.lifetime >= effect.maxLifetime) {
                effect.active = false;
            }
        }
    }
    
    // Clean up finished effects
    cleanupFinishedEffects();
}

// Particle system control (disabled as requested)
void GameAnimationSystem::enableParticleSystem(bool enable)
{
    particleSystemEnabled = false; // Always disabled as per user request
}

void GameAnimationSystem::updateParticleSystem(sf::Time frameTime)
{
    // Intentionally empty - particle system not used
}

// Animation queries
bool GameAnimationSystem::isBigDiamondAnimating() const
{
    return bigDiamondAnimationActive && game->bigDiamondActive;
}

bool GameAnimationSystem::isCharacterAnimating(int playerId) const
{
    if (playerId >= 0 && playerId < 4) {
        return characterAnimations[playerId].isAnimating;
    }
    return false;
}

bool GameAnimationSystem::hasActiveAnimations() const
{
    // Check if any animations are running
    if (bigDiamondAnimationActive) return true;
    
    for (const auto& charAnim : characterAnimations) {
        if (charAnim.isAnimating) return true;
    }
    
    for (const auto& effect : activeEffects) {
        if (effect.active) return true;
    }
    
    return false;
}

// Private helper methods
void GameAnimationSystem::initializeAnimationStates()
{
    // Initialize character animation states
    for (auto& charAnim : characterAnimations) {
        charAnim.isAnimating = false;
        charAnim.currentAnimation = "";
        charAnim.animationTime = 0.0f;
    }
    
    // Clear any existing effects
    activeEffects.clear();
}

void GameAnimationSystem::updateSingleCharacterAnimation(int playerId, sf::Time frameTime)
{
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

void GameAnimationSystem::cleanupFinishedEffects()
{
    activeEffects.erase(
        std::remove_if(activeEffects.begin(), activeEffects.end(),
                      [](const VisualEffect& effect) { return !effect.active; }),
        activeEffects.end()
    );
}

float GameAnimationSystem::calculateOscillatorModifier() const
{
    // Calculate the sine-based modifier for big diamond animation
    // This is the extracted logic from game.cpp
    return sin(oscillator / 2.5f) * 30.0f;
}

} // namespace DP 