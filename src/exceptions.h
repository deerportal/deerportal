#pragma once
#include <exception>
#include <string>
#include <string_view>

namespace DeerPortal {

// Base exception class for all DeerPortal errors
class GameException : public std::exception {
protected:
    std::string message;
    std::string context;
    
public:
    GameException(std::string_view msg, std::string_view ctx = "") 
        : message(msg), context(ctx) {}
    
    const char* what() const noexcept override { 
        return message.c_str(); 
    }
    
    const std::string& getContext() const { return context; }
};

// Asset-related exceptions
class AssetLoadException : public GameException {
public:
    enum AssetType { TEXTURE, FONT, SOUND, SHADER };
    
private:
    std::string filename;
    AssetType type;
    
public:
    AssetLoadException(AssetType t, std::string_view file, std::string_view msg)
        : GameException(msg, file), filename(file), type(t) {}
        
    const std::string& getFilename() const { return filename; }
    AssetType getAssetType() const { return type; }
};

// System resource exceptions
class SystemResourceException : public GameException {
public:
    enum ResourceType { WINDOW, OPENGL_CONTEXT, MEMORY };
    
private:
    ResourceType type;
    
public:
    SystemResourceException(ResourceType t, std::string_view msg)
        : GameException(msg), type(t) {}
        
    ResourceType getResourceType() const { return type; }
};

// Configuration exceptions
class ConfigurationException : public GameException {
public:
    ConfigurationException(std::string_view msg) : GameException(msg) {}
};

// Game state exceptions
class GameStateException : public GameException {
public:
    GameStateException(std::string_view msg) : GameException(msg) {}
};

} // namespace DeerPortal