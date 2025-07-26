#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace DP {

/*!
 * \brief LightingManager implements the GEMINI25pro light map approach
 * 
 * Uses SFML's native BlendAdd and BlendMultiply for optimal performance.
 * Renders lights to an off-screen texture then applies with multiplicative blending.
 */
class LightingManager {
public:
  LightingManager();
  ~LightingManager();

  // Core functionality
  bool initialize(sf::Vector2u windowSize);
  void beginFrame(sf::Color ambientColor = sf::Color(20, 20, 30, 255));
  void addLight(sf::Vector2f position, float radius = 80.0f, float intensity = 1.0f, sf::Color color = sf::Color::White);
  void render(sf::RenderTarget& target);
  void cleanup();

  // Configuration
  void setEnabled(bool enabled) { lightingEnabled = enabled; }
  bool isEnabled() const { return lightingEnabled; }
  void setAmbientColor(sf::Color color) { ambientColor = color; }
  void setBaseLightRadius(float radius) { baseLightRadius = radius; }

  // Debug functions
  void setDebugMode(bool debug) { debugMode = debug; }
  void renderDebugInfo(sf::RenderTarget& target);

private:
  // Core rendering components
  sf::RenderTexture lightMap;
  std::unique_ptr<sf::Sprite> lightSprite;
  sf::Texture lightTexture;
  
  // Configuration
  sf::Color ambientColor;
  float baseLightRadius;
  bool lightingEnabled;
  bool needsUpdate;
  bool debugMode;
  
  // Light data
  struct LightSource {
    sf::Vector2f position;
    float radius;
    float intensity;
    sf::Color color;
  };
  std::vector<LightSource> lights;
  
  // Internal methods
  bool createLightTexture();
  void renderLights();
};

} // namespace DP