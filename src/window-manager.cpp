#include "window-manager.h"

#include <algorithm>
#include <cstdint>
#include <iostream>

namespace DP {

// External variables for screen dimensions
extern int initScreenX;
extern int initScreenY;

WindowManager::WindowManager()
    : m_isFullscreen(false), m_windowedPosition(0, 0) {
  initializeVideoModes();
  m_view.setSize(sf::Vector2f(initScreenX, initScreenY));
  m_view.setCenter(sf::Vector2f(initScreenX / 2.f, initScreenY / 2.f));
}

WindowManager::~WindowManager() {
  // Nothing to clean up
}

void WindowManager::initialize(sf::RenderWindow& window,
                               const std::string& windowTitle) {
  m_windowTitle = windowTitle;

  // Store current window position if window is already created
  if (window.isOpen()) {
    m_windowedPosition = window.getPosition();
  }

#ifndef NDEBUG
  std::cout << "WindowManager initialized with title: " << windowTitle
            << std::endl;
#endif
}

void WindowManager::initializeVideoModes() {
  // Set up windowed mode using game's initial screen dimensions
  m_windowedMode = sf::VideoMode(sf::Vector2u(initScreenX, initScreenY));

  // Set up initial fullscreen mode using desktop resolution
  // This will be updated dynamically during fullscreen toggle
  m_fullscreenMode = sf::VideoMode::getDesktopMode();

#ifndef NDEBUG
  std::cout << "Window modes initialized:" << std::endl;
  std::cout << "  Windowed: " << m_windowedMode.size.x << "x"
            << m_windowedMode.size.y << std::endl;
  std::cout << "  Initial Fullscreen: " << m_fullscreenMode.size.x << "x"
            << m_fullscreenMode.size.y << std::endl;
#endif
}

bool WindowManager::toggleFullscreen(sf::RenderWindow& window, sf::RenderTexture& renderTexture, sf::Sprite& renderSprite) {
  try {
    if (m_isFullscreen) {
      // Switch to windowed mode
#ifndef NDEBUG
      std::cout << "Switching to windowed mode..." << std::endl;
#endif

      // Close current window
      window.close();

      // Create new windowed window
      createWindow(window, m_windowedMode, sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);

      // Restore window position
      restoreWindowPosition(window);

      m_isFullscreen = false;
      
      // Update view, render texture, and sprite scaling for windowed mode
      updateViewForWindow(window);
      updateRenderTextureSize(renderTexture, window);
      updateSpriteScaling(renderSprite, window);
      
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

      // Dynamically detect which monitor the window is on
      sf::VideoMode targetFullscreenMode = detectCurrentMonitorMode(window);
      
#ifndef NDEBUG
      std::cout << "Detected target monitor: " << targetFullscreenMode.size.x 
                << "x" << targetFullscreenMode.size.y << std::endl;
#endif

      // Close current window
      window.close();

      // Create new fullscreen window using SFML 3.0.1 true fullscreen API
      createWindow(window, targetFullscreenMode, sf::Style::Default, sf::State::Fullscreen);

      m_isFullscreen = true;
      
      // Update view, render texture, and sprite scaling for fullscreen mode
      updateViewForWindow(window);
      updateRenderTextureSize(renderTexture, window);
      updateSpriteScaling(renderSprite, window);
      
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

bool WindowManager::isFullscreen() const { return m_isFullscreen; }

void WindowManager::forceWindowedMode(sf::RenderWindow& window) {
#ifndef NDEBUG
  std::cout << "Forcing windowed mode for recovery..." << std::endl;
#endif

  try {
    window.close();
    createWindow(window, m_windowedMode, sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);
    restoreWindowPosition(window);
    m_isFullscreen = false;
#ifndef NDEBUG
    std::cout << "Recovery to windowed mode successful" << std::endl;
#endif
  } catch (const std::exception& e) {
    std::cerr << "Recovery to windowed mode failed: " << e.what() << std::endl;
  }
}

sf::VideoMode WindowManager::getWindowedMode() const { return m_windowedMode; }

sf::VideoMode WindowManager::getFullscreenMode() const {
  return m_fullscreenMode;
}

void WindowManager::saveWindowPosition(sf::RenderWindow& window) {
  if (window.isOpen()) {
    m_windowedPosition = window.getPosition();
#ifndef NDEBUG
    std::cout << "Saved window position: " << m_windowedPosition.x << ", "
              << m_windowedPosition.y << std::endl;
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

void WindowManager::createWindow(sf::RenderWindow& window,
                                 const sf::VideoMode& videoMode,
                                 std::uint32_t style, sf::State state) {
  // Create the window using SFML 3.0.1 API
  window.create(videoMode, m_windowTitle, style, state);

  if (!window.isOpen()) {
    throw std::runtime_error("Failed to create window");
  }

  // Set window properties - use V-Sync for optimal performance
  window.setVerticalSyncEnabled(true); // Sync to monitor refresh rate
  // Remove framerate limit when using V-Sync to avoid conflicts

#ifndef NDEBUG
  std::cout << "Window created: " << videoMode.size.x << "x"
            << videoMode.size.y << " (Style: " << style << ")" << std::endl;
#endif
}

void WindowManager::updateView(sf::RenderTexture& renderTexture) {
  // Get the current window size (actual screen resolution in fullscreen)
  sf::Vector2u windowSize = renderTexture.getSize();
  
  // Game's native resolution
  float gameWidth = static_cast<float>(initScreenX);   // 1360
  float gameHeight = static_cast<float>(initScreenY);  // 768
  float gameAspectRatio = gameWidth / gameHeight;
  
  // Current window aspect ratio
  float windowWidth = static_cast<float>(windowSize.x);
  float windowHeight = static_cast<float>(windowSize.y);
  float windowAspectRatio = windowWidth / windowHeight;
  
  // Calculate viewport for letterboxing/pillarboxing
  float viewportX = 0.0f;
  float viewportY = 0.0f;
  float viewportWidth = 1.0f;
  float viewportHeight = 1.0f;
  
  if (windowAspectRatio > gameAspectRatio) {
    // Window is wider than game - add pillarboxing (black bars on left/right)
    viewportWidth = gameAspectRatio / windowAspectRatio;
    viewportX = (1.0f - viewportWidth) / 2.0f;
    // viewportY and viewportHeight remain default (0.0, 1.0)
  } else if (windowAspectRatio < gameAspectRatio) {
    // Window is taller than game - add letterboxing (black bars on top/bottom)
    viewportHeight = windowAspectRatio / gameAspectRatio;
    viewportY = (1.0f - viewportHeight) / 2.0f;
    // viewportX and viewportWidth remain default (0.0, 1.0)
  }
  // If aspect ratios match exactly, use full viewport (1.0, 1.0)
  
  // Set the view to use the entire game area
  m_view.setSize(sf::Vector2f(gameWidth, gameHeight));
  m_view.setCenter(sf::Vector2f(gameWidth / 2.0f, gameHeight / 2.0f));
  
  // Apply calculated viewport with letterboxing/pillarboxing
  m_view.setViewport(sf::FloatRect(sf::Vector2f(viewportX, viewportY), sf::Vector2f(viewportWidth, viewportHeight)));
  
  // Apply the view to the render texture
  renderTexture.setView(m_view);

#ifndef NDEBUG
  std::cout << "Fullscreen scaling updated:" << std::endl;
  std::cout << "  Window: " << windowWidth << "x" << windowHeight 
            << " (aspect: " << windowAspectRatio << ")" << std::endl;
  std::cout << "  Game: " << gameWidth << "x" << gameHeight 
            << " (aspect: " << gameAspectRatio << ")" << std::endl;
  std::cout << "  Viewport: " << viewportX << ", " << viewportY 
            << ", " << viewportWidth << ", " << viewportHeight << std::endl;
#endif
}

sf::View WindowManager::getView() const {
    return m_view;
}

void WindowManager::updateRenderTextureSize(sf::RenderTexture& renderTexture, sf::RenderWindow& window) {
  // Always keep render texture at native game resolution (1360x768)
  // This follows the best practice pattern from the document
  sf::Vector2u gameResolution(initScreenX, initScreenY);
  
  if (!renderTexture.resize(gameResolution)) {
    std::cerr << "Failed to resize render texture to game resolution" << std::endl;
    return;
  }
  
  // Enable bilinear filtering for better scaling quality
  renderTexture.setSmooth(true);
  
  // Reset to default view - scaling will be handled when drawing to window
  renderTexture.setView(renderTexture.getDefaultView());

#ifndef NDEBUG
  std::cout << "Render texture maintained at native resolution: " 
            << gameResolution.x << "x" << gameResolution.y << std::endl;
#endif
}

void WindowManager::updateSpriteScaling(sf::Sprite& sprite, sf::RenderWindow& window) {
  // Fix for double-scaling issue: Let SFML view system handle all scaling
  // Remove sprite-level scaling to prevent compound scaling effects
  
  // Always use 1:1 sprite scaling - the view system handles fullscreen scaling
  sprite.setScale(sf::Vector2f(1.0f, 1.0f));
  sprite.setPosition(sf::Vector2f(0.0f, 0.0f));
  
#ifndef NDEBUG
  if (m_isFullscreen) {
    std::cout << "Fullscreen mode: Using view-only scaling (sprite at 1:1)" << std::endl;
  } else {
    std::cout << "Windowed mode: Using 1:1 scaling" << std::endl;
  }
#endif
}

sf::VideoMode WindowManager::detectCurrentMonitorMode(sf::RenderWindow& window) {
  if (!window.isOpen()) {
    // Fallback to desktop mode if window is not open
    return sf::VideoMode::getDesktopMode();
  }
  
  // Get current window position and size
  sf::Vector2i windowPos = window.getPosition();
  sf::Vector2u windowSize = window.getSize();
  
  // Calculate window center point
  sf::Vector2i windowCenter(
    windowPos.x + static_cast<int>(windowSize.x) / 2,
    windowPos.y + static_cast<int>(windowSize.y) / 2
  );
  
  // Get all available fullscreen modes (represents different monitors/resolutions)
  std::vector<sf::VideoMode> availableModes = sf::VideoMode::getFullscreenModes();
  
  if (availableModes.empty()) {
    // Fallback to desktop mode if no fullscreen modes available
    return sf::VideoMode::getDesktopMode();
  }
  
  // For cross-platform compatibility, we use a heuristic approach:
  // - On single monitor systems, use the highest resolution mode
  // - On multi-monitor systems, try to detect based on window position
  
  // Sort modes by resolution (highest first)
  std::sort(availableModes.begin(), availableModes.end(),
    [](const sf::VideoMode& a, const sf::VideoMode& b) {
      if (a.size.x != b.size.x) return a.size.x > b.size.x;
      return a.size.y > b.size.y;
    });
  
  // Simple heuristic: if window center is beyond primary monitor bounds,
  // assume we're on a secondary monitor and use a different resolution
  sf::VideoMode primaryMode = sf::VideoMode::getDesktopMode();
  
  if (windowCenter.x >= static_cast<int>(primaryMode.size.x) || 
      windowCenter.y >= static_cast<int>(primaryMode.size.y) ||
      windowCenter.x < 0 || windowCenter.y < 0) {
    
    // Window appears to be on a secondary monitor
    // Look for a different resolution in available modes
    for (const auto& mode : availableModes) {
      if (mode.size != primaryMode.size) {
        #ifndef NDEBUG
        std::cout << "Window detected on secondary monitor, using mode: " 
                  << mode.size.x << "x" << mode.size.y << std::endl;
        #endif
        return mode;
      }
    }
  }
  
  // Default: use the primary monitor mode (desktop mode)
  #ifndef NDEBUG
  std::cout << "Using primary monitor mode: " 
            << primaryMode.size.x << "x" << primaryMode.size.y << std::endl;
  #endif
  return primaryMode;
}

void WindowManager::updateViewForWindow(sf::RenderWindow& window) {
  // Get the current window size (actual screen resolution in fullscreen)
  sf::Vector2u windowSize = window.getSize();
  
  // Game's native resolution
  float gameWidth = static_cast<float>(initScreenX);   // 1360
  float gameHeight = static_cast<float>(initScreenY);  // 768
  float gameAspectRatio = gameWidth / gameHeight;
  
  // Current window aspect ratio
  float windowWidth = static_cast<float>(windowSize.x);
  float windowHeight = static_cast<float>(windowSize.y);
  float windowAspectRatio = windowWidth / windowHeight;
  
  // Calculate viewport for letterboxing/pillarboxing
  float viewportX = 0.0f;
  float viewportY = 0.0f;
  float viewportWidth = 1.0f;
  float viewportHeight = 1.0f;
  
  if (windowAspectRatio > gameAspectRatio) {
    // Window is wider than game - add pillarboxing (black bars on left/right)
    viewportWidth = gameAspectRatio / windowAspectRatio;
    viewportX = (1.0f - viewportWidth) / 2.0f;
  } else if (windowAspectRatio < gameAspectRatio) {
    // Window is taller than game - add letterboxing (black bars on top/bottom)
    viewportHeight = windowAspectRatio / gameAspectRatio;
    viewportY = (1.0f - viewportHeight) / 2.0f;
  }
  
  // Set the view to use the entire game area
  m_view.setSize(sf::Vector2f(gameWidth, gameHeight));
  m_view.setCenter(sf::Vector2f(gameWidth / 2.0f, gameHeight / 2.0f));
  
  // Apply calculated viewport with letterboxing/pillarboxing
  m_view.setViewport(sf::FloatRect(sf::Vector2f(viewportX, viewportY), sf::Vector2f(viewportWidth, viewportHeight)));
  
  // Apply the view to the window
  window.setView(m_view);

#ifndef NDEBUG
  std::cout << "Window view scaling updated:" << std::endl;
  std::cout << "  Window: " << windowWidth << "x" << windowHeight 
            << " (aspect: " << windowAspectRatio << ")" << std::endl;
  std::cout << "  Game: " << gameWidth << "x" << gameHeight 
            << " (aspect: " << gameAspectRatio << ")" << std::endl;
  std::cout << "  Viewport: " << viewportX << ", " << viewportY 
            << ", " << viewportWidth << ", " << viewportHeight << std::endl;
#endif
}

} // namespace DP
