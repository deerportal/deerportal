#ifndef CARDNOTIFICATION_H
#define CARDNOTIFICATION_H

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

#include "card.h"
#include "textureholder.h"

/*!
 * \brief The CardNotification class handles displaying notifications when cards are picked
 * This is a modular system that can be easily replaced with better graphics implementations
 */
class CardNotification : public sf::Drawable, public sf::Transformable {
public:
  explicit CardNotification(sf::Font* gameFont, TextureHolder* textures = nullptr);

  /*!
   * \brief Shows a notification for a specific card type and player
   * \param cardType The type of card ("stop", "card", "diamond", "diamond x 2")
   * \param playerNumber The player who triggered the card effect
   * \param targetPlayer The player affected by the card (for targeted effects)
   * \param cardPileNumber The pile number (0-3) that determines card element and texture
   */
  void showCardNotification(const std::string& cardType, int playerNumber, int targetPlayer = -1,
                            int cardPileNumber = -1);

  /*!
   * \brief Updates the notification display timing
   * \param frameTime Time since last frame
   */
  void update(sf::Time frameTime);

  /*!
   * \brief Dismisses the notification (called when next player throws dice)
   */
  void dismiss();

  /*!
   * \brief Checks if notification is currently active
   * \return true if notification is being displayed
   */
  bool isActive() const { return active; }

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  /*!
   * \brief Generates notification text based on card type and context
   * \param cardType The type of card
   * \param playerNumber The player who picked the card
   * \param targetPlayer The affected player
   * \return Formatted notification message
   */
  std::string generateNotificationText(const std::string& cardType, int playerNumber,
                                       int targetPlayer);

  /*!
   * \brief Sets up the visual appearance of the notification with size constraints
   * \param text The text to display
   * \param playerNumber The player who triggered the effect
   * \param targetPlayer The affected player (for targeted effects)
   * \param cardPileNumber The pile number for card texture selection
   */
  void setupNotification(const std::string& text, int playerNumber, int targetPlayer,
                         int cardPileNumber);

  /*!
   * \brief Ensures notification fits within screen constraints
   * \param maxWidth Maximum allowed width (75% of board area)
   * \param maxHeight Maximum allowed height (75% of board area)
   */
  void ensureScreenFit(float maxWidth, float maxHeight);

  /*!
   * \brief Sets up player portrait sprites from character texture
   * \param playerNumber The player number (0-3)
   * \param targetPlayer The target player number (0-3, or -1 if none)
   */
  void setupPlayerPortraits(int playerNumber, int targetPlayer);

  /*!
   * \brief Creates inline text layout with portraits positioned before player mentions
   * \param text The full notification text
   * \param playerNumber The acting player
   * \param targetPlayer The target player (-1 if none)
   */
  void createInlineLayout(const std::string& text, int playerNumber, int targetPlayer);

  /*!
   * \brief Sets up the card sprite based on pile number and card type
   * \param cardType The type of card picked up
   * \param cardPileNumber The pile number (0-3) for texture selection
   */
  void setupCardSprite(const std::string& cardType, int cardPileNumber);

  // Visual components
  std::unique_ptr<sf::Text> notificationText;
  std::unique_ptr<sf::RectangleShape> backgroundRect;
  sf::Font* font;
  TextureHolder* textures;

  // Card sprite component
  std::unique_ptr<sf::Sprite> cardSprite;

  // Player portrait components
  std::unique_ptr<sf::Sprite> playerPortrait;
  std::unique_ptr<sf::Sprite> targetPlayerPortrait;
  std::unique_ptr<sf::Text> playerLabel;
  std::unique_ptr<sf::Text> targetPlayerLabel;

  // Inline layout components
  std::vector<std::unique_ptr<sf::Text>> textSegments;
  std::vector<std::unique_ptr<sf::Sprite>> inlinePortraits;
  std::vector<std::unique_ptr<sf::Text>> inlineLabels;

  // State management
  bool active;
  float blinkTimer; // For visual feedback (optional blinking effect)

  // Configuration
  static constexpr int TEXT_SIZE = 32;     // Reduced from 40 to 32 for better fit
  static constexpr int PORTRAIT_SIZE = 32; // Keep original character size to avoid stretching
  static constexpr int LABEL_SIZE = 18;    // Reduced from 20 to 18 to match
  static constexpr float BACKGROUND_ALPHA = 180.0f;
  static constexpr float PADDING = 20.0f;          // Keep good spacing
  static constexpr float PORTRAIT_SPACING = 8.0f;  // Keep good spacing
  static constexpr float MAX_SCREEN_RATIO = 0.85f; // Increased from 0.75 to 0.85 for bigger overlay
  static constexpr float BLINK_INTERVAL = 1.5f;    // Gentle hint blinking every 1.5 seconds
};

#endif // CARDNOTIFICATION_H