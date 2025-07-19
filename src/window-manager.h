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
   * \return true if toggle was successful, false otherwise
   */
  bool toggleFullscreen(sf::RenderWindow& window);

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
   * \brief Update the view to scale and center the content
   * \param window Reference to the game's render window
   */
  void updateView(sf::RenderTexture& renderTexture);

private:
  bool m_isFullscreen;
  sf::VideoMode m_windowedMode;
  sf::VideoMode m_fullscreenMode;
  sf::Vector2i m_windowedPosition;
  std::string m_windowTitle;
  sf::View m_view;

  /*!
   * \brief Initialize video modes for windowed and fullscreen
   */
  void initializeVideoModes();

  /*!
   * \brief Create window with specified parameters
   * \param window Reference to the game's render window
   * \param videoMode Video mode to use
   * \param style Window style flags
   */
  void createWindow(sf::RenderWindow& window, const sf::VideoMode& videoMode,
                    std::uint32_t style);
  
};

} // namespace DP

#endif // WINDOW_MANAGER_H