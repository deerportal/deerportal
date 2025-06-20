#include "error-handler.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace DeerPortal {

ErrorHandler& ErrorHandler::getInstance() {
    static ErrorHandler instance;
    return instance;
}

void ErrorHandler::setErrorCallback(ErrorCallback&& callback) {
    errorCallback = std::move(callback);
}

bool ErrorHandler::handleException(const GameException& e) {
    // Update statistics
    stats.totalErrors++;
    
    if (auto assetError = dynamic_cast<const AssetLoadException*>(&e)) {
        stats.assetLoadErrors++;
    } else if (auto systemError = dynamic_cast<const SystemResourceException*>(&e)) {
        stats.systemResourceErrors++;
    } else if (auto gameError = dynamic_cast<const GameStateException*>(&e)) {
        stats.gameStateErrors++;
    }
    
    // Log the error
    logError(e);
    
    // Call user-defined callback if set
    if (errorCallback) {
        errorCallback(e);
    }
    
    // For now, always return true (continue execution)
    // In the future, this could be configurable based on error type
    return true;
}

void ErrorHandler::logError(const GameException& e) {
    if (!loggingInitialized) {
        initializeLogging();
    }
    
    if (errorLog && errorLog->is_open()) {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        // Format: [2025-06-20 14:30:15] ERROR: message
        *errorLog << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") 
                  << "] ERROR: " << e.what();
        
        if (!e.getContext().empty()) {
            *errorLog << " (Context: " << e.getContext() << ")";
        }
        
        // Add specific information for asset errors
        if (auto assetError = dynamic_cast<const AssetLoadException*>(&e)) {
            const char* typeStr = "UNKNOWN";
            switch (assetError->getAssetType()) {
                case AssetLoadException::TEXTURE: typeStr = "TEXTURE"; break;
                case AssetLoadException::FONT: typeStr = "FONT"; break;
                case AssetLoadException::SOUND: typeStr = "SOUND"; break;
                case AssetLoadException::SHADER: typeStr = "SHADER"; break;
            }
            *errorLog << " [Type: " << typeStr << ", File: " << assetError->getFilename() << "]";
        }
        
        *errorLog << std::endl;
        errorLog->flush();
    }
    
    // Also log to stderr for immediate visibility
    std::cerr << "DeerPortal Error: " << e.what();
    if (!e.getContext().empty()) {
        std::cerr << " (Context: " << e.getContext() << ")";
    }
    std::cerr << std::endl;
}

void ErrorHandler::initializeLogging(const std::string& logPath) {
    if (loggingInitialized) {
        return;
    }
    
    errorLog = std::make_unique<std::ofstream>(logPath, std::ios::app);
    if (errorLog->is_open()) {
        // Write session header
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        *errorLog << "\n=== DeerPortal Error Log Session Started: " 
                  << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") 
                  << " ===" << std::endl;
        errorLog->flush();
        
        loggingInitialized = true;
    } else {
        std::cerr << "Warning: Could not initialize error logging to " << logPath << std::endl;
    }
}

} // namespace DeerPortal