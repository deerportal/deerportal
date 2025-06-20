#include "safe-asset-loader.h"
#include "error-handler.h"
#include "filetools.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace DeerPortal {

std::vector<std::string> SafeAssetLoader::searchPaths;
bool SafeAssetLoader::searchPathsInitialized = false;

void SafeAssetLoader::loadTexture(sf::Texture& texture, const std::string& filename) {
    std::string fullPath = getFullPath(filename);
    
    if (!texture.loadFromFile(fullPath)) {
        throw AssetLoadException(
            AssetLoadException::TEXTURE,
            filename,
            "Failed to load texture file: " + fullPath
        );
    }
}

void SafeAssetLoader::loadFont(sf::Font& font, const std::string& filename, 
                               const std::string& fallbackFont) {
    std::string fullPath = getFullPath(filename);
    
    if (!font.openFromFile(fullPath)) {
        if (!fallbackFont.empty()) {
            // Try fallback font
            std::string fallbackPath = getFullPath(fallbackFont);
            if (font.openFromFile(fallbackPath)) {
                // Log that we're using fallback
                ErrorHandler::getInstance().logError(
                    AssetLoadException(
                        AssetLoadException::FONT,
                        filename,
                        "Primary font failed, using fallback: " + fallbackFont
                    )
                );
                return;
            }
        }
        
        // Try system default font
        if (loadFallbackFont(font)) {
            ErrorHandler::getInstance().logError(
                AssetLoadException(
                    AssetLoadException::FONT,
                    filename,
                    "Primary font failed, using system default font"
                )
            );
            return;
        }
        
        throw AssetLoadException(
            AssetLoadException::FONT,
            filename,
            "Failed to load font file and no fallback available: " + fullPath
        );
    }
}

void SafeAssetLoader::loadSound(sf::SoundBuffer& buffer, const std::string& filename) {
    std::string fullPath = getFullPath(filename);
    
    if (!buffer.loadFromFile(fullPath)) {
        throw AssetLoadException(
            AssetLoadException::SOUND,
            filename,
            "Failed to load sound file: " + fullPath
        );
    }
}

std::string SafeAssetLoader::findAssetPath(const std::string& filename) {
    if (!searchPathsInitialized) {
        initializeSearchPaths();
    }
    
    // Try the get_full_path function first
    std::string fullPath = getFullPath(filename);
    if (fileExists(fullPath)) {
        return fullPath;
    }
    
    // Try each search path
    for (const auto& path : searchPaths) {
        std::string candidate = path + "/" + filename;
        if (fileExists(candidate)) {
            return candidate;
        }
    }
    
    // Return original path if not found
    return fullPath;
}

void SafeAssetLoader::createFallbackTexture(sf::Texture& texture, 
                                            sf::Color color,
                                            sf::Vector2u size) {
    sf::Image fallbackImage(size, color);
    
    if (!texture.loadFromImage(fallbackImage)) {
        throw SystemResourceException(
            SystemResourceException::MEMORY,
            "Failed to create fallback texture from image"
        );
    }
}

bool SafeAssetLoader::loadFallbackFont(sf::Font& font) {
    // Try common system font paths
    std::vector<std::string> systemFonts = {
        // macOS
        "/System/Library/Fonts/Arial.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        // Linux
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/arial.ttf",
        // Windows (if running under Wine or similar)
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf"
    };
    
    for (const auto& fontPath : systemFonts) {
        if (fileExists(fontPath) && font.openFromFile(fontPath)) {
            return true;
        }
    }
    
    return false;
}

void SafeAssetLoader::addSearchPath(const std::string& path) {
    if (!searchPathsInitialized) {
        initializeSearchPaths();
    }
    searchPaths.push_back(path);
}

void SafeAssetLoader::clearSearchPaths() {
    searchPaths.clear();
    searchPathsInitialized = false;
}

bool SafeAssetLoader::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void SafeAssetLoader::initializeSearchPaths() {
    if (searchPathsInitialized) {
        return;
    }
    
    searchPaths = {
        ".",
        "./assets",
        "../assets",
        "./Resources",  // macOS app bundle
        "../Resources"
    };
    
    searchPathsInitialized = true;
}

std::string SafeAssetLoader::getFullPath(const std::string& filename) {
    return get_full_path(filename);
}

} // namespace DeerPortal