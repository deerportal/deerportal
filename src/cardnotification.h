#ifndef CARDNOTIFICATION_H
#define CARDNOTIFICATION_H

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "card.h"

/*!
 * \brief The CardNotification class handles displaying notifications when cards are picked
 * This is a modular system that can be easily replaced with better graphics implementations
 */
class CardNotification: public sf::Drawable, public sf::Transformable
{
public:
    explicit CardNotification(sf::Font *gameFont);
    
    /*!
     * \brief Shows a notification for a specific card type and player
     * \param cardType The type of card ("stop", "card", "diamond", "diamond x 2")
     * \param playerNumber The player who triggered the card effect
     * \param targetPlayer The player affected by the card (for targeted effects)
     */
    void showCardNotification(const std::string& cardType, int playerNumber, int targetPlayer = -1);
    
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
    std::string generateNotificationText(const std::string& cardType, int playerNumber, int targetPlayer);
    
    /*!
     * \brief Sets up the visual appearance of the notification with size constraints
     * \param text The text to display
     */
    void setupNotification(const std::string& text);
    
    /*!
     * \brief Ensures notification fits within screen constraints
     * \param maxWidth Maximum allowed width (75% of board area)
     * \param maxHeight Maximum allowed height (75% of board area)
     */
    void ensureScreenFit(float maxWidth, float maxHeight);

    // Visual components
    std::unique_ptr<sf::Text> notificationText;
    std::unique_ptr<sf::RectangleShape> backgroundRect;
    sf::Font* font;
    
    // State management
    bool active;
    float blinkTimer;  // For visual feedback (optional blinking effect)
    
    // Configuration
    static constexpr int TEXT_SIZE = 30;  // Reduced from 60 to 30 (half size)
    static constexpr float BACKGROUND_ALPHA = 180.0f;
    static constexpr float PADDING = 15.0f;  // Slightly reduced padding
    static constexpr float MAX_SCREEN_RATIO = 0.75f;  // 75% of screen max
    static constexpr float BLINK_INTERVAL = 1.5f;  // Gentle hint blinking every 1.5 seconds
};

#endif // CARDNOTIFICATION_H 