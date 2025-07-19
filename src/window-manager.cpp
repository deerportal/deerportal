#include "window-manager.h"

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

  // Set up fullscreen mode using desktop resolution
  m_fullscreenMode = sf::VideoMode::getDesktopMode();

#ifndef NDEBUG
  std::cout << "Window modes initialized:" << std::endl;
  std::cout << "  Windowed: " << m_windowedMode.size.x << "x"
            << m_windowedMode.size.y << std::endl;
  std::cout << "  Fullscreen: " << m_fullscreenMode.size.x << "x"
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

      // Close current window
      window.close();

      // Create new fullscreen window using SFML 3.0.1 true fullscreen API
      createWindow(window, m_fullscreenMode, sf::Style::Default, sf::State::Fullscreen);

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
  // Implementation based on the best practice pattern from the document
  sf::Vector2u windowSize = window.getSize();
  sf::Vector2u gameResolution(initScreenX, initScreenY);
  
  // Calculate aspect ratios
  float windowAspect = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
  float gameAspect = static_cast<float>(gameResolution.x) / static_cast<float>(gameResolution.y);
  
  // Calculate viewport for letterboxing/pillarboxing
  int viewportWidth, viewportHeight, viewportLeft, viewportTop;
  
  if (windowAspect > gameAspect) {
    // Window is wider - pillarboxing (black bars on sides)
    viewportHeight = static_cast<int>(windowSize.y);
    viewportWidth = static_cast<int>(windowSize.y * gameAspect);
    viewportLeft = (static_cast<int>(windowSize.x) - viewportWidth) / 2;
    viewportTop = 0;
  } else {
    // Window is taller - letterboxing (black bars on top/bottom)
    viewportWidth = static_cast<int>(windowSize.x);
    viewportHeight = static_cast<int>(windowSize.x / gameAspect);
    viewportLeft = 0;
    viewportTop = (static_cast<int>(windowSize.y) - viewportHeight) / 2;
  }
  
  // Calculate scaling factor
  float scale = static_cast<float>(viewportWidth) / static_cast<float>(gameResolution.x);
  
  // Apply scaling and positioning to sprite using SFML 3.0.1 API
  sprite.setScale(sf::Vector2f(scale, scale));
  sprite.setPosition(sf::Vector2f(static_cast<float>(viewportLeft), static_cast<float>(viewportTop)));

#ifndef NDEBUG
  std::cout << "Sprite scaling updated:" << std::endl;
  std::cout << "  Window: " << windowSize.x << "x" << windowSize.y 
            << " (aspect: " << windowAspect << ")" << std::endl;
  std::cout << "  Game: " << gameResolution.x << "x" << gameResolution.y 
            << " (aspect: " << gameAspect << ")" << std::endl;
  std::cout << "  Viewport: " << viewportLeft << ", " << viewportTop 
            << ", " << viewportWidth << ", " << viewportHeight << std::endl;
  std::cout << "  Scale: " << scale << std::endl;
#endif
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
