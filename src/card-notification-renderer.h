#ifndef CARD_NOTIFICATION_RENDERER_H
#define CARD_NOTIFICATION_RENDERER_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "textureholder.h"

/*! 
 * \brief CardNotificationRenderer handles all rendering aspects of card notifications
 * 
 * This class is responsible for drawing card notifications, including:
 * - Background rendering
 * - Text rendering with inline player portraits
 * - Card sprite rendering
 * - Visual effects and animations
 */
class CardNotificationRenderer : public sf::Drawable
{
public:
    CardNotificationRenderer(sf::Font* gameFont, TextureHolder* textures);
    ~CardNotificationRenderer();

    // Main rendering interface
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    // Setup methods for rendering components
    void setupPlayerPortraits(int playerNumber, int targetPlayer);
    void setupCardSprite(const std::string& text, int cardPileNumber);
    void setupBackground(sf::Vector2f position, sf::Vector2f size);
    
    // Layout and positioning
    void createInlineLayout(const std::string& text, int playerNumber, int targetPlayer);
    void positionElements(sf::Vector2f basePosition);
    void ensureScreenFit(float maxWidth, float maxHeight);
    
    // Visual state management
    void setVisible(bool visible);
    void setBlinkState(bool blinking);
    void updateVisualEffects(sf::Time frameTime);
    
    // Access to rendered elements (for positioning/layout)
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);

private:
    // Constants for rendering
    static constexpr int TEXT_SIZE = 20;
    static constexpr int LABEL_SIZE = 16;
    static constexpr int BACKGROUND_ALPHA = 200;
    static constexpr float PORTRAIT_SCALE = 1.0f;
    static constexpr float CARD_SCALE = 0.8f;
    
    // Core rendering resources
    sf::Font* font;
    TextureHolder* textures;
    
    // Background rendering
    std::unique_ptr<sf::RectangleShape> backgroundRect;
    
    // Text rendering components
    std::unique_ptr<sf::Text> notificationText;
    std::vector<std::unique_ptr<sf::Text>> textSegments;
    
    // Player portrait rendering
    std::unique_ptr<sf::Sprite> playerPortrait;
    std::unique_ptr<sf::Sprite> targetPlayerPortrait;
    std::unique_ptr<sf::Text> playerLabel;
    std::unique_ptr<sf::Text> targetPlayerLabel;
    std::vector<std::unique_ptr<sf::Sprite>> inlinePortraits;
    std::vector<std::unique_ptr<sf::Text>> inlineLabels;
    
    // Card sprite rendering
    std::unique_ptr<sf::Sprite> cardSprite;
    
    // Visual state
    bool visible;
    bool blinking;
    float blinkTimer;
    
    // Helper methods
    void drawBackground(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawTextElements(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawPlayerPortraits(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawCardSprite(sf::RenderTarget& target, sf::RenderStates states) const;
    void drawInlineElements(sf::RenderTarget& target, sf::RenderStates states) const;
    
    // Layout helpers
    void calculateElementPositions();
    void adjustForScreenBounds(float maxWidth, float maxHeight);
};

#endif // CARD_NOTIFICATION_RENDERER_H 