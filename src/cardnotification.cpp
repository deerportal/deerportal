#include "cardnotification.h"
#include <sstream>
#include <algorithm>
#include <cmath>

CardNotification::CardNotification(sf::Font *gameFont):
    font(gameFont),
    active(false),
    blinkTimer(0.0f)
{
    notificationText = std::make_unique<sf::Text>(*font);
    notificationText->setCharacterSize(TEXT_SIZE);
    notificationText->setFillColor(sf::Color::White);
    
    backgroundRect = std::make_unique<sf::RectangleShape>();
    backgroundRect->setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(BACKGROUND_ALPHA)));
}

void CardNotification::showCardNotification(const std::string& cardType, int playerNumber, int targetPlayer)
{
    std::string notificationMessage = generateNotificationText(cardType, playerNumber, targetPlayer);
    setupNotification(notificationMessage);
    
    active = true;
    blinkTimer = 0.0f;
}

void CardNotification::dismiss()
{
    active = false;
    blinkTimer = 0.0f;
}

std::string CardNotification::generateNotificationText(const std::string& cardType, int playerNumber, int targetPlayer)
{
    std::stringstream message;
    
    // Convert player numbers to display names (assuming 0-based indexing)
    std::string playerName = "Player " + std::to_string(playerNumber + 1);
    std::string targetName = (targetPlayer >= 0) ? "Player " + std::to_string(targetPlayer + 1) : "";
    
    if (cardType == "diamond") {
        message << playerName << " picked up a DIAMOND card!\n";
        if (targetPlayer >= 0 && targetPlayer != playerNumber) {
            message << targetName << " lost a diamond and\n" << playerName << " gained cash!";
        } else {
            message << "Gained 1 cash from diamond theft!";
        }
    }
    else if (cardType == "stop") {
        message << playerName << " picked up a STOP card!\n";
        if (targetPlayer >= 0 && targetPlayer != playerNumber) {
            message << targetName << " has been frozen for 1 turn!";
        } else {
            message << "A player has been frozen!";
        }
    }
    else if (cardType == "card") {
        message << playerName << " picked up a REMOVE CARD!\n";
        if (targetPlayer >= 0 && targetPlayer != playerNumber) {
            message << "Removed a random card from " << targetName << "!";
        } else {
            message << "A random card was removed from a player!";
        }
    }
    else if (cardType == "diamond x 2") {
        message << playerName << " picked up a DOUBLE DIAMOND card!\n";
        if (targetPlayer >= 0 && targetPlayer != playerNumber) {
            message << targetName << " lost 2 diamonds and\n" << playerName << " gained 2 cash!";
        } else {
            message << "Gained 2 cash from double diamond theft!";
        }
    }
    else {
        // Fallback for unknown card types
        message << playerName << " picked up a " << cardType << " card!";
    }
    
    return message.str();
}

void CardNotification::setupNotification(const std::string& text)
{
    notificationText->setString(text);
    
    // Get screen dimensions (use game resolution from existing code)
    float screenWidth = 1360.0f;  // From game resolution
    float screenHeight = 768.0f;
    
    // Calculate game board area positioning
    // viewTiles.setViewport(sf::FloatRect({0.15f,0.1f}, {1.0f, 1.0f}));
    // Board size: 16x16 tiles, 40px per tile = 640px
    float boardStartX = screenWidth * 0.15f;  // 15% from left (204px)
    float boardStartY = screenHeight * 0.1f;  // 10% from top (76.8px)
    float boardSize = 16 * 40;  // 640px (16 tiles * 40px per tile)
    
    // Center of the game board (where portal is located)
    float boardCenterX = boardStartX + (boardSize / 2.0f);  // ~524px
    float boardCenterY = boardStartY + (boardSize / 2.0f);  // ~396.8px
    
    // Calculate maximum allowed size (75% of board area, not full screen)
    float maxWidth = boardSize * MAX_SCREEN_RATIO;   // 75% of board width
    float maxHeight = boardSize * MAX_SCREEN_RATIO;  // 75% of board height
    
    // Ensure the notification fits within board constraints
    ensureScreenFit(maxWidth, maxHeight);
    
    // Get final text bounds after potential font scaling
    sf::FloatRect textBounds = notificationText->getLocalBounds();
    
    // Set up background rectangle with padding
    sf::Vector2f backgroundSize(
        std::min(textBounds.size.x + PADDING * 2, maxWidth),
        std::min(textBounds.size.y + PADDING * 2, maxHeight)
    );
    backgroundRect->setSize(backgroundSize);
    
    // Center the notification on the game board center (portal area)
    sf::Vector2f position(
        boardCenterX - (backgroundSize.x / 2.0f),
        boardCenterY - (backgroundSize.y / 2.0f)
    );
    
    backgroundRect->setPosition(position);
    
    // Position text with padding offset, ensuring it's centered in the background
    sf::Vector2f textPosition(
        position.x + (backgroundSize.x - textBounds.size.x) / 2.0f,
        position.y + (backgroundSize.y - textBounds.size.y) / 2.0f
    );
    
    notificationText->setPosition(textPosition);
}

void CardNotification::ensureScreenFit(float maxWidth, float maxHeight)
{
    // Get initial text bounds
    sf::FloatRect textBounds = notificationText->getLocalBounds();
    
    // Check if text is too wide and needs adjustment
    if (textBounds.size.x + PADDING * 2 > maxWidth) {
        // Reduce font size if necessary to fit width
        unsigned int currentSize = notificationText->getCharacterSize();
        unsigned int minSize = 16; // Minimum readable size
        
        while (currentSize > minSize && textBounds.size.x + PADDING * 2 > maxWidth) {
            currentSize -= 2;
            notificationText->setCharacterSize(currentSize);
            textBounds = notificationText->getLocalBounds();
        }
    }
    
    // Check height constraint
    if (textBounds.size.y + PADDING * 2 > maxHeight) {
        // If still too tall, reduce font size further
        unsigned int currentSize = notificationText->getCharacterSize();
        unsigned int minSize = 14;
        
        while (currentSize > minSize && textBounds.size.y + PADDING * 2 > maxHeight) {
            currentSize -= 2;
            notificationText->setCharacterSize(currentSize);
            textBounds = notificationText->getLocalBounds();
        }
    }
}

void CardNotification::update(sf::Time frameTime)
{
    if (active) {
        blinkTimer += frameTime.asSeconds();
        
        // Optional: gentle blinking effect to hint that notification will dismiss on next dice throw
        if (blinkTimer >= BLINK_INTERVAL * 2) {
            blinkTimer = 0.0f;
        }
    }
}

void CardNotification::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (active) {
        states.transform *= getTransform();
        
        // Optional gentle blinking effect
        float alpha = 1.0f;
        if (blinkTimer > BLINK_INTERVAL) {
            // Gentle fade effect: from 100% to 90% opacity (more subtle)
            float fadePhase = (blinkTimer - BLINK_INTERVAL) / BLINK_INTERVAL;
            alpha = 1.0f - (0.1f * std::sin(fadePhase * 3.14159f));
        }
        
        // Apply alpha to background
        sf::Color bgColor = backgroundRect->getFillColor();
        bgColor.a = static_cast<std::uint8_t>(BACKGROUND_ALPHA * alpha);
        backgroundRect->setFillColor(bgColor);
        
        // Apply alpha to text
        sf::Color textColor = notificationText->getFillColor();
        textColor.a = static_cast<std::uint8_t>(255 * alpha);
        notificationText->setFillColor(textColor);
        
        // Draw semi-transparent background first
        target.draw(*backgroundRect, states);
        
        // Draw text on top
        target.draw(*notificationText, states);
        
        // Restore original colors for next frame
        bgColor.a = static_cast<std::uint8_t>(BACKGROUND_ALPHA);
        backgroundRect->setFillColor(bgColor);
        textColor.a = 255;
        notificationText->setFillColor(textColor);
    }
} 