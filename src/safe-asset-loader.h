#pragma once
#include <string>
#include <vector>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "exceptions.h"

namespace DeerPortal {

class SafeAssetLoader {
public:
  // Load texture with proper error handling
  static void loadTexture(sf::Texture& texture, const std::string& filename);

  // Load font with fallback support
  static void loadFont(sf::Font& font, const std::string& filename,
                       const std::string& fallbackFont = "");

  // Load sound with error recovery
  static void loadSound(sf::SoundBuffer& buffer, const std::string& filename);

  // Try multiple paths for asset loading
  static std::string findAssetPath(const std::string& filename);

  // Create fallback texture (colored rectangle)
  static void createFallbackTexture(sf::Texture& texture, sf::Color color = sf::Color::Magenta,
                                    sf::Vector2u size = sf::Vector2u(64, 64));

  // Create fallback font (system default)
  static bool loadFallbackFont(sf::Font& font);

  // Set asset search paths
  static void addSearchPath(const std::string& path);
  static void clearSearchPaths();

  // Check if file exists
  static bool fileExists(const std::string& filename);

private:
  static std::vector<std::string> searchPaths;
  static bool searchPathsInitialized;

  // Initialize default search paths
  static void initializeSearchPaths();

  // Get full path using get_full_path function
  static std::string getFullPath(const std::string& filename);
};

} // namespace DeerPortal