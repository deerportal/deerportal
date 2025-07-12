#include "introshader.h"

#include <iostream>

#include "tilemap.h" // For get_full_path

namespace DP {

IntroShader::IntroShader()
    : initialized(false), finished(false), duration(8.0f), currentTime(0.0f) {}

IntroShader::~IntroShader() {}

bool IntroShader::initialize(sf::Vector2u screenSize) {
  return initialize(screenSize, nullptr);
}

bool IntroShader::initialize(sf::Vector2u screenSize, const sf::Texture* backgroundTexture) {
  this->screenSize = screenSize;

  // New grid-based reveal shader that unveils the intro screen rectangle by rectangle
  const std::string fragmentShaderGrid = R"(
#version 120

// Grid-based reveal shader for DeerPortal intro
// Unveils the intro screen through random rectangular reveals

uniform float iTime;
uniform vec2 iResolution;
uniform sampler2D introTexture;
uniform bool useIntroTexture;

// Grid configuration
const float GRID_COLS = 95.0;  // Much denser grid for tiny rectangles (1/10 size)
const float GRID_ROWS = 63.0;  // Much denser grid for tiny rectangles (1/10 size)
const float FRAMES_PER_RECT = 5.0;
const float START_OFFSET = 0.5;  // Even faster offset for the huge number of rectangles

// Simple pseudo-random function for deterministic randomness
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float getGridAnimationTime(vec2 gridPos) {
    // Calculate when this grid cell should start animating
    // Use random order instead of sequential
    float cellIndex = gridPos.y * GRID_COLS + gridPos.x;
    float totalCells = GRID_COLS * GRID_ROWS;
    
    // Create random order by using grid position as seed
    float randomOrder = random(gridPos) * totalCells;
    
    // Stagger start times based on random order
    float startTime = randomOrder * (START_OFFSET / 60.0);  // Convert frames to seconds (assuming 60fps)
    
    // Calculate animation progress for this cell (0.0 to 1.0)
    float animTime = iTime - startTime;
    float progress = clamp(animTime / (FRAMES_PER_RECT / 60.0), 0.0, 1.0);
    
    return progress;
}

vec3 getRevealColor(vec2 uv, float progress) {
    if (!useIntroTexture) {
        // Fallback: simple color transition
        vec3 deerportalColor = vec3(0.4, 0.3, 0.2);  // Earthy brown
        return mix(vec3(0.0), deerportalColor, progress);
    }
    
    // Calculate zoom effect: start at 4x zoom, scale down to 1x as progress goes from 0 to 1
    float zoomFactor = mix(4.0, 1.0, progress);
    
    // Calculate the center point of the current grid cell
    vec2 gridPos = floor(uv * vec2(GRID_COLS, GRID_ROWS));
    vec2 cellCenter = (gridPos + 0.5) / vec2(GRID_COLS, GRID_ROWS);
    
    // Apply zoom effect relative to the cell center
    vec2 zoomedUV = cellCenter + (uv - cellCenter) / zoomFactor;
    
    // Create strong pixelated effect: much more pronounced pixelation
    // Start with very large pixels (8x8), end with normal resolution
    float pixelSize = mix(8.0, 1.0, progress * progress);  // Quadratic easing for smoother transition
    
    // Apply pixelation by snapping to pixel grid
    vec2 pixelGrid = floor(zoomedUV * iResolution / pixelSize);
    vec2 pixelatedUV = pixelGrid * pixelSize / iResolution;
    
    // Fix mirroring by flipping Y coordinate
    vec2 fixedUV = vec2(pixelatedUV.x, 1.0 - pixelatedUV.y);
    
    // Sample the intro screen texture with pixelated zoom effect
    vec3 introColor = texture2D(introTexture, fixedUV).rgb;
    
    // Transition from black to the actual intro screen color
    return mix(vec3(0.0), introColor, progress);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    
    // Determine which grid cell this pixel belongs to
    vec2 gridPos = floor(uv * vec2(GRID_COLS, GRID_ROWS));
    
    // Get animation progress for this grid cell
    float progress = getGridAnimationTime(gridPos);
    
    // Add some easing to make the transition smoother
    float easedProgress = smoothstep(0.0, 1.0, progress);
    
    // Get the final color
    vec3 finalColor = getRevealColor(uv, easedProgress);
    
    gl_FragColor = vec4(finalColor, 1.0);
}
)";

  // Try the new grid shader first
  if (backgroundTexture && shader.loadFromMemory(fragmentShaderGrid, sf::Shader::Type::Fragment)) {
    std::cout << "Grid reveal intro shader loaded successfully with intro texture" << std::endl;
    shader.setUniform("introTexture",
                      *backgroundTexture); // Temporarily use background, will be replaced
    shader.setUniform("useIntroTexture", true);
  } else if (shader.loadFromMemory(fragmentShaderGrid, sf::Shader::Type::Fragment)) {
    std::cout << "Grid reveal intro shader loaded successfully with fallback colors" << std::endl;
    shader.setUniform("useIntroTexture", false);
  } else {
    // Final fallback to simple shader
    const std::string simpleShader = R"(
uniform float iTime;
uniform vec2 iResolution;

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    
    // Simple grid reveal fallback with tiny rectangles
    vec2 gridPos = floor(uv * vec2(95.0, 63.0));
    float cellIndex = gridPos.y * 95.0 + gridPos.x;
    float startTime = cellIndex * 0.002;  // Very fast for massive grid
    float progress = clamp((iTime - startTime) / 0.08, 0.0, 1.0);
    
    vec3 color = vec3(0.4, 0.3, 0.2) * progress;
    gl_FragColor = vec4(color, 1.0);
}
)";

    if (shader.loadFromMemory(simpleShader, sf::Shader::Type::Fragment)) {
      std::cout << "Simple grid intro shader loaded successfully" << std::endl;
    } else {
      std::cerr << "Failed to load any intro shader version" << std::endl;
      return false;
    }
  }

  // Create render texture
  if (!renderTexture.resize(screenSize)) {
    std::cerr << "Failed to create intro shader render texture" << std::endl;
    return false;
  }

  // Setup sprite - initialize after render texture is created
  sprite = std::make_unique<sf::Sprite>(renderTexture.getTexture());

  initialized = true;
  return true;
}

void IntroShader::setIntroTexture(const sf::Texture* introTexture) {
  if (introTexture && initialized) {
    shader.setUniform("introTexture", *introTexture);
    shader.setUniform("useIntroTexture", true);
    std::cout << "Intro texture set successfully" << std::endl;
  }
}

void IntroShader::update(float deltaTime) {
  if (!initialized || finished) return;

  currentTime += deltaTime;

  // Calculate total animation time based on grid size and timing
  // 95x63 grid = 5,985 rectangles! With 0.5 frame offset
  // Total time: (5984 * 0.5 + 5) frames / 60fps = about 50 seconds (way too long!)
  // Let's use a much shorter duration and fast overlap for tiny rectangles
  float totalAnimationTime = 8.0f; // Reasonable duration for massive tiny grid

  if (currentTime >= totalAnimationTime) {
    finished = true;
  }
}

void IntroShader::render(sf::RenderWindow& window) {
  if (!initialized || finished || !sprite) return;

  // Set shader uniforms
  shader.setUniform("iTime", currentTime);
  shader.setUniform("iResolution", sf::Vector2f(screenSize.x, screenSize.y));

  // Clear render texture and render fullscreen quad with shader
  renderTexture.clear(sf::Color::Black);

  // Create a fullscreen rectangle for the shader
  sf::RectangleShape fullscreenQuad(sf::Vector2f(screenSize.x, screenSize.y));
  fullscreenQuad.setPosition(sf::Vector2f(0.0f, 0.0f));

  // Render with shader
  renderTexture.draw(fullscreenQuad, &shader);
  renderTexture.display();

  // Draw to window
  window.draw(*sprite);
}

void IntroShader::reset() {
  currentTime = 0.0f;
  finished = false;
  clock.restart();
}

} // namespace DP