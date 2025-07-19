#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace DP {

/*!
 * \brief WindowManager handles window state transitions and fullscreen toggling
 */
class WindowManager {
public:
  WindowManager();
  ~WindowManager();

  /*!
   * \brief Initialize window manager with game window reference
   * \param window Reference to the game's render window
   * \param windowTitle Title for the window
   */
  void initialize(sf::RenderWindow& window, const std::string& windowTitle);

  /*!
   * \brief Toggle between fullscreen and windowed mode
   * \param window Reference to the game's render window
   * \param renderTexture Reference to the game's render texture (for resizing)
   * \param renderSprite Reference to the render sprite (for scaling)
   * \return true if toggle was successful, false otherwise
   */
  bool toggleFullscreen(sf::RenderWindow& window, sf::RenderTexture& renderTexture, sf::Sprite& renderSprite);

  /*!
   * \brief Check if currently in fullscreen mode
   * \return true if fullscreen, false if windowed
   */
  bool isFullscreen() const;

  /*!
   * \brief Force window to windowed mode (recovery function)
   * \param window Reference to the game's render window
   */
  void forceWindowedMode(sf::RenderWindow& window);

  /*!
   * \brief Get current window mode for display
   * \return Current windowed mode video mode
   */
  sf::VideoMode getWindowedMode() const;

  /*!
   * \brief Get current fullscreen mode for display
   * \return Current fullscreen mode video mode
   */
  sf::VideoMode getFullscreenMode() const;

  /*!
   * \brief Save current window position (before going fullscreen)
   * \param window Reference to the game's render window
   */
  void saveWindowPosition(sf::RenderWindow& window);

  /*!
   * \brief Restore saved window position (after returning from fullscreen)
   * \param window Reference to the game's render window
   */
  void restoreWindowPosition(sf::RenderWindow& window);

  /*!
   * \brief Update view for proper fullscreen scaling with letterboxing/pillarboxing
   * \param renderTexture Reference to the game's render texture
   */
  void updateView(sf::RenderTexture& renderTexture);

  /*!
   * \brief Update view for proper fullscreen scaling using window size
   * \param window Reference to the game's render window
   */
  void updateViewForWindow(sf::RenderWindow& window);

  /*!
   * \brief Update render texture size for fullscreen scaling
   * \param renderTexture Reference to the game's render texture
   * \param window Reference to the game's render window
   */
  void updateRenderTextureSize(sf::RenderTexture& renderTexture, sf::RenderWindow& window);

  /*!
   * \brief Calculate sprite scaling and position for proper fullscreen rendering
   * \param sprite Reference to the render sprite
   * \param window Reference to the game's render window
   */
  void updateSpriteScaling(sf::Sprite& sprite, sf::RenderWindow& window);

  sf::View getView() const;

private:
  bool m_isFullscreen;
  sf::VideoMode m_windowedMode;
  sf::VideoMode m_fullscreenMode;
  sf::Vector2i m_windowedPosition;
  std::string m_windowTitle;
  sf::View m_view;  // View for proper fullscreen scaling
  

  /*!
   * \brief Initialize video modes for windowed and fullscreen
   */
  void initializeVideoModes();

  /*!
   * \brief Create window with specified parameters
   * \param window Reference to the game's render window
   * \param videoMode Video mode to use
   * \param style Window style flags
   * \param state Window state (floating or fullscreen)
   */
  void createWindow(sf::RenderWindow& window, const sf::VideoMode& videoMode,
                    std::uint32_t style, sf::State state = sf::State::Windowed);
  
};

} // namespace DP

#endif // WINDOW_MANAGER_H