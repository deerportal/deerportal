#include "window-manager.h"
#include <iostream>

namespace DP {

// External variables for screen dimensions
extern int initScreenX;
extern int initScreenY;

WindowManager::WindowManager() 
    : m_isFullscreen(false), m_windowedPosition(0, 0) {
    initializeVideoModes();
}

WindowManager::~WindowManager() {
    // Nothing to clean up
}

void WindowManager::initialize(sf::RenderWindow& window, const std::string& windowTitle) {
    m_windowTitle = windowTitle;
    
    // Store current window position if window is already created
    if (window.isOpen()) {
        m_windowedPosition = window.getPosition();
    }
    
    std::cout << "WindowManager initialized with title: " << windowTitle << std::endl;
}

void WindowManager::initializeVideoModes() {
    // Set up windowed mode using game's initial screen dimensions
    m_windowedMode = sf::VideoMode(sf::Vector2u(initScreenX, initScreenY));
    
    // Set up fullscreen mode using desktop resolution
    m_fullscreenMode = sf::VideoMode::getDesktopMode();
    
    std::cout << "Window modes initialized:" << std::endl;
    std::cout << "  Windowed: " << m_windowedMode.size.x << "x" << m_windowedMode.size.y << std::endl;
    std::cout << "  Fullscreen: " << m_fullscreenMode.size.x << "x" << m_fullscreenMode.size.y << std::endl;
}

bool WindowManager::toggleFullscreen(sf::RenderWindow& window) {
    try {
        if (m_isFullscreen) {
            // Switch to windowed mode
            std::cout << "Switching to windowed mode..." << std::endl;
            
            // Close current window
            window.close();
            
            // Create new windowed window
            createWindow(window, m_windowedMode, sf::Style::Titlebar | sf::Style::Close);
            
            // Restore window position
            restoreWindowPosition(window);
            
            m_isFullscreen = false;
            std::cout << "Switched to windowed mode successfully" << std::endl;
            
        } else {
            // Switch to fullscreen mode
            std::cout << "Switching to fullscreen mode..." << std::endl;
            
            // Save current window position
            saveWindowPosition(window);
            
            // Close current window
            window.close();
            
            // Create new fullscreen window
            createWindow(window, m_fullscreenMode, sf::Style::Fullscreen);
            
            m_isFullscreen = true;
            std::cout << "Switched to fullscreen mode successfully" << std::endl;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Fullscreen toggle failed: " << e.what() << std::endl;
        
        // Attempt recovery to windowed mode
        if (m_isFullscreen) {
            forceWindowedMode(window);
        }
        
        return false;
    }
}

bool WindowManager::isFullscreen() const {
    return m_isFullscreen;
}

void WindowManager::forceWindowedMode(sf::RenderWindow& window) {
    std::cout << "Forcing windowed mode for recovery..." << std::endl;
    
    try {
        window.close();
        createWindow(window, m_windowedMode, sf::Style::Titlebar | sf::Style::Close);
        restoreWindowPosition(window);
        m_isFullscreen = false;
        std::cout << "Recovery to windowed mode successful" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Recovery to windowed mode failed: " << e.what() << std::endl;
    }
}

sf::VideoMode WindowManager::getWindowedMode() const {
    return m_windowedMode;
}

sf::VideoMode WindowManager::getFullscreenMode() const {
    return m_fullscreenMode;
}

void WindowManager::saveWindowPosition(sf::RenderWindow& window) {
    if (window.isOpen()) {
        m_windowedPosition = window.getPosition();
        std::cout << "Saved window position: " << m_windowedPosition.x << ", " << m_windowedPosition.y << std::endl;
    }
}

void WindowManager::restoreWindowPosition(sf::RenderWindow& window) {
    if (window.isOpen()) {
        window.setPosition(m_windowedPosition);
        std::cout << "Restored window position: " << m_windowedPosition.x << ", " << m_windowedPosition.y << std::endl;
    }
}

void WindowManager::createWindow(sf::RenderWindow& window, const sf::VideoMode& videoMode, sf::Uint32 style) {
    // Create the window
    window.create(videoMode, m_windowTitle, style);
    
    if (!window.isOpen()) {
        throw std::runtime_error("Failed to create window");
    }
    
    // Set window properties
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    
    std::cout << "Window created: " << videoMode.size.x << "x" << videoMode.size.y 
              << " (Style: " << style << ")" << std::endl;
}

} // namespace DP 