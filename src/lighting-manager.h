#pragma once
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace DP {

/*!
 * \brief LightingManager implements the GEMINI25pro light map approach with performance
 * optimizations
 *
 * Uses SFML's native BlendAdd and BlendMultiply for optimal performance.
 * Renders lights to an off-screen texture then applies with multiplicative blending.
 * Includes vertex array batching and spatial culling for 100+ light performance.
 */
class LightingManager {
public:
  LightingManager();
  ~LightingManager();

  // Core functionality
  bool initialize(sf::Vector2u windowSize);
  void beginFrame(sf::Color ambientColor = sf::Color(20, 20, 30, 255));
  void addLight(sf::Vector2f position, float radius = 80.0f, float intensity = 1.0f,
                sf::Color color = sf::Color::White);
  void render(sf::RenderTarget& target);
  void cleanup();

  // Configuration
  void setEnabled(bool enabled) { lightingEnabled = enabled; }
  bool isEnabled() const { return lightingEnabled; }
  void setAmbientColor(sf::Color color) { ambientColor = color; }
  void setBaseLightRadius(float radius) { baseLightRadius = radius; }

  // Performance optimization controls
  void setVertexArrayBatching(bool enabled) { useVertexArrayBatching = enabled; }
  void setSpatialCulling(bool enabled) { useSpatialCulling = enabled; }
  bool isUsingBatching() const { return useVertexArrayBatching; }

  // Performance metrics
  size_t getLightCount() const { return lights.size(); }
  size_t getVisibleLightCount() const { return visibleLights; }

  // Debug functions
  void setDebugMode(bool debug) { debugMode = debug; }
  void renderDebugInfo(sf::RenderTarget& target);

private:
  // Core rendering components
  sf::RenderTexture lightMap;
  std::unique_ptr<sf::Sprite> lightSprite;
  sf::Texture lightTexture;

  // Performance optimization components
  sf::VertexArray lightVertices;
  sf::FloatRect viewBounds;
  size_t visibleLights;
  static constexpr size_t VERTICES_PER_LIGHT = 6; // 2 triangles for quad

  // Configuration
  sf::Color ambientColor;
  float baseLightRadius;
  bool lightingEnabled;
  bool needsUpdate;
  bool debugMode;
  bool useVertexArrayBatching;
  bool useSpatialCulling;

  // Light data
  struct LightSource {
    sf::Vector2f position;
    float radius;
    float intensity;
    sf::Color color;

    // Performance optimization data
    bool isVisible;
    sf::FloatRect bounds;
  };
  std::vector<LightSource> lights;

  // Internal methods
  bool createLightTexture();
  void renderLights();
  void renderLightsBatched();
  void performSpatialCulling(sf::Vector2u windowSize);
  void buildLightVertexArray();
};

} // namespace DP