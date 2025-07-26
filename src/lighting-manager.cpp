#include "lighting-manager.h"
#include <cmath>
#include <algorithm>

namespace DP {

LightingManager::LightingManager() 
  : ambientColor(20, 20, 30, 255)
  , baseLightRadius(80.0f)
  , lightingEnabled(true)
  , needsUpdate(true)
  , debugMode(false) {
}

LightingManager::~LightingManager() {
  cleanup();
}

bool LightingManager::initialize(sf::Vector2u windowSize) {
  // Create render texture for light map
  if (!lightMap.resize(windowSize)) {
    return false;
  }
  lightMap.setSmooth(true);
  
  // Create procedural light texture
  if (!createLightTexture()) {
    return false;
  }
  
  // Initialize light sprite
  lightSprite = std::make_unique<sf::Sprite>(lightTexture);
  lightSprite->setOrigin(sf::Vector2f(lightTexture.getSize().x / 2.0f, lightTexture.getSize().y / 2.0f));
  
  return true;
}

bool LightingManager::createLightTexture() {
  // Create 128x128 radial gradient texture programmatically
  const unsigned int size = 128;
  const unsigned int centerX = size / 2;
  const unsigned int centerY = size / 2;
  const float maxRadius = size / 2.0f;
  
  // Create pixel array
  std::vector<uint8_t> pixels(size * size * 4); // RGBA
  
  for (unsigned int y = 0; y < size; ++y) {
    for (unsigned int x = 0; x < size; ++x) {
      // Calculate distance from center
      float dx = static_cast<float>(x) - centerX;
      float dy = static_cast<float>(y) - centerY;
      float distance = std::sqrt(dx * dx + dy * dy);
      
      // Calculate alpha based on distance (radial gradient)
      float normalizedDistance = distance / maxRadius;
      normalizedDistance = std::min(1.0f, normalizedDistance);
      
      // Smooth falloff curve (exponential)
      float alpha = 1.0f - normalizedDistance;
      alpha = alpha * alpha; // Quadratic falloff for smoother gradient
      alpha = std::max(0.0f, std::min(1.0f, alpha));
      
      // Set pixel (white with varying alpha)
      unsigned int index = (y * size + x) * 4;
      pixels[index + 0] = 255; // R
      pixels[index + 1] = 255; // G
      pixels[index + 2] = 255; // B
      pixels[index + 3] = static_cast<uint8_t>(alpha * 255); // A
    }
  }
  
  // Create and load texture
  if (!lightTexture.resize({size, size})) {
    return false;
  }
  
  lightTexture.update(pixels.data());
  lightTexture.setSmooth(true);
  return true;
}

void LightingManager::beginFrame(sf::Color ambient) {
  ambientColor = ambient;
  lights.clear();
  needsUpdate = true;
}

void LightingManager::addLight(sf::Vector2f position, float radius, float intensity, sf::Color color) {
  lights.push_back({position, radius, intensity, color});
}

void LightingManager::renderLights() {
  // Clear light map to ambient color (GEMINI25pro approach)
  lightMap.clear(ambientColor);
  
  // Set additive blending for light accumulation
  sf::RenderStates lightStates;
  lightStates.blendMode = sf::BlendAdd;
  
  // Render all lights
  for (const auto& light : lights) {
    // Scale sprite based on light radius
    float scale = (light.radius * 2.0f) / lightTexture.getSize().x;
    lightSprite->setScale(sf::Vector2f(scale, scale));
    lightSprite->setPosition(light.position);
    
    // Set light color and intensity
    sf::Color lightColor = light.color;
    lightColor.r = static_cast<uint8_t>(lightColor.r * light.intensity);
    lightColor.g = static_cast<uint8_t>(lightColor.g * light.intensity);
    lightColor.b = static_cast<uint8_t>(lightColor.b * light.intensity);
    lightSprite->setColor(lightColor);
    
    // Draw light to accumulation buffer
    lightMap.draw(*lightSprite, lightStates);
  }
  
  lightMap.display();
}

void LightingManager::render(sf::RenderTarget& target) {
  if (!lightingEnabled || lights.empty()) {
    return;
  }
  
  // Render lights to light map
  renderLights();
  
  // Apply light map with multiplicative blending (GEMINI25pro approach)
  sf::Sprite lightMapSprite(lightMap.getTexture());
  sf::RenderStates lightMapStates;
  lightMapStates.blendMode = sf::BlendMultiply; // Multiply lights with existing scene
  
  target.draw(lightMapSprite, lightMapStates);
}

void LightingManager::renderDebugInfo(sf::RenderTarget& target) {
  if (!debugMode) {
    return;
  }
  
  // Draw light positions as small circles
  sf::CircleShape debugCircle(5.0f);
  debugCircle.setFillColor(sf::Color::Red);
  debugCircle.setOrigin(sf::Vector2f(5.0f, 5.0f));
  
  for (const auto& light : lights) {
    debugCircle.setPosition(light.position);
    target.draw(debugCircle);
    
    // Draw light radius
    sf::CircleShape radiusCircle(light.radius);
    radiusCircle.setFillColor(sf::Color::Transparent);
    radiusCircle.setOutlineColor(sf::Color(255, 255, 0, 100));
    radiusCircle.setOutlineThickness(1.0f);
    radiusCircle.setOrigin(sf::Vector2f(light.radius, light.radius));
    radiusCircle.setPosition(light.position);
    target.draw(radiusCircle);
  }
}

void LightingManager::cleanup() {
  lights.clear();
}

} // namespace DP