#include "window-manager.h"

#include <cstdint>
#include <iostream>

namespace DP {

// External variables for screen dimensions
extern int initScreenX;
extern int initScreenY;

WindowManager::WindowManager() : m_isFullscreen(false), m_windowedPosition(0, 0) {
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

#ifndef NDEBUG
  std::cout << "WindowManager initialized with title: " << windowTitle << std::endl;
#endif
}

void WindowManager::initializeVideoModes() {
  // Set up windowed mode using game's initial screen dimensions
  m_windowedMode = sf::VideoMode(sf::Vector2u(initScreenX, initScreenY));

  // Set up fullscreen mode using desktop resolution
  m_fullscreenMode = sf::VideoMode::getDesktopMode();

#ifndef NDEBUG
  std::cout << "Window modes initialized:" << std::endl;
  std::cout << "  Windowed: " << m_windowedMode.size.x << "x" << m_windowedMode.size.y << std::endl;
  std::cout << "  Fullscreen: " << m_fullscreenMode.size.x << "x" << m_fullscreenMode.size.y
            << std::endl;
#endif
}

bool WindowManager::toggleFullscreen(sf::RenderWindow& window) {
  try {
    if (m_isFullscreen) {
      // Switch to windowed mode
#ifndef NDEBUG
      std::cout << "Switching to windowed mode..." << std::endl;
#endif

      // Close current window
      window.close();

      // Create new windowed window
      createWindow(window, m_windowedMode, 1 | 4); // Titlebar | Close

      // Restore window position
      restoreWindowPosition(window);

      m_isFullscreen = false;
#ifndef NDEBUG
      std::cout << "Switched to windowed mode successfully" << std::endl;
#endif

    } else {
      // Switch to fullscreen mode
#ifndef NDEBUG
      std::cout << "Switching to fullscreen mode..." << std::endl;
#endif

      // Save current window position
      saveWindowPosition(window);

      // Close current window
      window.close();

      // Create new fullscreen window
      createWindow(window, m_fullscreenMode, 8); // Fullscreen

      m_isFullscreen = true;
#ifndef NDEBUG
      std::cout << "Switched to fullscreen mode successfully" << std::endl;
#endif
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
#ifndef NDEBUG
  std::cout << "Forcing windowed mode for recovery..." << std::endl;
#endif

  try {
    window.close();
    createWindow(window, m_windowedMode, 1 | 4); // Titlebar | Close
    restoreWindowPosition(window);
    m_isFullscreen = false;
#ifndef NDEBUG
    std::cout << "Recovery to windowed mode successful" << std::endl;
#endif
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
#ifndef NDEBUG
    std::cout << "Saved window position: " << m_windowedPosition.x << ", " << m_windowedPosition.y
              << std::endl;
#endif
  }
}

void WindowManager::restoreWindowPosition(sf::RenderWindow& window) {
  if (window.isOpen()) {
    window.setPosition(m_windowedPosition);
#ifndef NDEBUG
    std::cout << "Restored window position: " << m_windowedPosition.x << ", "
              << m_windowedPosition.y << std::endl;
#endif
  }
}

void WindowManager::createWindow(sf::RenderWindow& window, const sf::VideoMode& videoMode,
                                 std::uint32_t style) {
  // Create the window
  window.create(videoMode, m_windowTitle, style);

  if (!window.isOpen()) {
    throw std::runtime_error("Failed to create window");
  }

  // Set window properties - use V-Sync for optimal performance
  window.setVerticalSyncEnabled(true);  // Sync to monitor refresh rate
  // Remove framerate limit when using V-Sync to avoid conflicts

#ifndef NDEBUG
  std::cout << "Window created: " << videoMode.size.x << "x" << videoMode.size.y
            << " (Style: " << style << ")" << std::endl;
#endif
}

} // namespace DP