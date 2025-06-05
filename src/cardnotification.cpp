#include "cardnotification.h"
#include <sstream>
#include <algorithm>
#include <cmath>

CardNotification::CardNotification(sf::Font *gameFont, TextureHolder *textures):
    font(gameFont),
    textures(textures),
    active(false),
    blinkTimer(0.0f)
{
    notificationText = std::make_unique<sf::Text>(*font);
    notificationText->setCharacterSize(TEXT_SIZE);
    notificationText->setFillColor(sf::Color::White);
    
    backgroundRect = std::make_unique<sf::RectangleShape>();
    backgroundRect->setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(BACKGROUND_ALPHA)));
    
    // Initialize player portrait sprites and labels if textures are available
    if (textures) {
        // Initialize card sprite with a default texture (will be set properly later)
        cardSprite = std::make_unique<sf::Sprite>(textures->cardsTextures[0][0]);
        
        playerPortrait = std::make_unique<sf::Sprite>(textures->textureCharacters);
        targetPlayerPortrait = std::make_unique<sf::Sprite>(textures->textureCharacters);
        
        playerLabel = std::make_unique<sf::Text>(*font);
        playerLabel->setCharacterSize(LABEL_SIZE);
        playerLabel->setFillColor(sf::Color::White);
        
        targetPlayerLabel = std::make_unique<sf::Text>(*font);
        targetPlayerLabel->setCharacterSize(LABEL_SIZE);
        targetPlayerLabel->setFillColor(sf::Color::White);
    }
}

void CardNotification::showCardNotification(const std::string& cardType, int playerNumber, int targetPlayer, int cardPileNumber)
{
    std::string notificationMessage = generateNotificationText(cardType, playerNumber, targetPlayer);
    setupNotification(notificationMessage, playerNumber, targetPlayer, cardPileNumber);
    
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
    
    // Use simple format for inline portrait display
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

void CardNotification::setupPlayerPortraits(int playerNumber, int targetPlayer)
{
    if (!textures || !playerPortrait || !targetPlayerPortrait) return;
    
    // Each player character is 32x58 pixels, with player offset = playerNumber * 32
    // Keep original size to avoid stretching
    int charWidth = 32;
    int charHeight = 58;
    
    // Set up main player portrait (no scaling to maintain aspect ratio)
    if (playerNumber >= 0 && playerNumber < 4) {
        int playerOffset = playerNumber * charWidth;
        playerPortrait->setTextureRect(sf::IntRect(sf::Vector2i(playerOffset, 0), sf::Vector2i(charWidth, charHeight)));
        // No scaling - use original size to avoid distortion
        
        if (playerLabel) {
            playerLabel->setString("P" + std::to_string(playerNumber + 1));
        }
    }
    
    // Set up target player portrait (if different from main player)
    if (targetPlayer >= 0 && targetPlayer < 4 && targetPlayer != playerNumber) {
        int targetOffset = targetPlayer * charWidth;
        targetPlayerPortrait->setTextureRect(sf::IntRect(sf::Vector2i(targetOffset, 0), sf::Vector2i(charWidth, charHeight)));
        // No scaling - use original size to avoid distortion
        
        if (targetPlayerLabel) {
            targetPlayerLabel->setString("P" + std::to_string(targetPlayer + 1));
        }
    }
}

void CardNotification::createInlineLayout(const std::string& text, int playerNumber, int targetPlayer)
{
    if (!textures) {
        // Fallback to simple text if no textures available
        notificationText->setString(text);
        return;
    }
    
    // Clear previous inline components
    textSegments.clear();
    inlinePortraits.clear();
    inlineLabels.clear();
    
    // Character dimensions
    int charWidth = 32;
    int charHeight = 58;
    
    // Split text into lines first
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    // Process each line separately
    for (const auto& currentLine : lines) {
        std::string remainingText = currentLine;
        
        while (!remainingText.empty()) {
            // Find next "Player X" mention
            size_t playerPos = remainingText.find("Player ");
            
            if (playerPos == std::string::npos) {
                // No more player mentions, add remaining text
                if (!remainingText.empty()) {
                    auto textSegment = std::make_unique<sf::Text>(*font);
                    textSegment->setCharacterSize(TEXT_SIZE);
                    textSegment->setFillColor(sf::Color::White);
                    textSegment->setString(remainingText);
                    textSegments.push_back(std::move(textSegment));
                }
                break;
            } else {
                // Add text before "Player X"
                if (playerPos > 0) {
                    std::string beforePlayer = remainingText.substr(0, playerPos);
                    auto textSegment = std::make_unique<sf::Text>(*font);
                    textSegment->setCharacterSize(TEXT_SIZE);
                    textSegment->setFillColor(sf::Color::White);
                    textSegment->setString(beforePlayer);
                    textSegments.push_back(std::move(textSegment));
                }
                
                // Extract player number
                size_t playerNumPos = playerPos + 7; // "Player " is 7 characters
                if (playerNumPos < remainingText.length()) {
                    char playerChar = remainingText[playerNumPos];
                    if (playerChar >= '1' && playerChar <= '4') {
                        int currentPlayerNum = playerChar - '1'; // Convert to 0-based
                        
                        // Create portrait for this player
                        auto portrait = std::make_unique<sf::Sprite>(textures->textureCharacters);
                        int playerOffset = currentPlayerNum * charWidth;
                        portrait->setTextureRect(sf::IntRect(sf::Vector2i(playerOffset, 0), sf::Vector2i(charWidth, charHeight)));
                        
                        auto label = std::make_unique<sf::Text>(*font);
                        label->setCharacterSize(LABEL_SIZE);
                        label->setFillColor(sf::Color::White);
                        label->setString("P" + std::to_string(currentPlayerNum + 1));
                        
                        inlinePortraits.push_back(std::move(portrait));
                        inlineLabels.push_back(std::move(label));
                    }
                }
                
                // Find end of "Player X" (next space or end of string)
                size_t nextSpace = remainingText.find(' ', playerNumPos + 1);
                if (nextSpace == std::string::npos) {
                    nextSpace = remainingText.length();
                }
                
                // Add "Player X" text
                std::string playerText = remainingText.substr(playerPos, nextSpace - playerPos);
                auto textSegment = std::make_unique<sf::Text>(*font);
                textSegment->setCharacterSize(TEXT_SIZE);
                textSegment->setFillColor(sf::Color::White);
                textSegment->setString(playerText);
                textSegments.push_back(std::move(textSegment));
                
                // Continue with remaining text
                remainingText = remainingText.substr(nextSpace);
            }
        }
        
        // Add a line break marker (empty text segment) to indicate end of line
        auto lineBreak = std::make_unique<sf::Text>(*font);
        lineBreak->setCharacterSize(TEXT_SIZE);
        lineBreak->setFillColor(sf::Color::White);
        lineBreak->setString("LINEBREAK"); // Special marker for line breaks
        textSegments.push_back(std::move(lineBreak));
    }
}

void CardNotification::setupNotification(const std::string& text, int playerNumber, int targetPlayer, int cardPileNumber)
{
    // Create inline layout instead of simple text
    createInlineLayout(text, playerNumber, targetPlayer);
    
    // Setup card sprite first
    setupCardSprite(text, cardPileNumber);
    
    // Get screen dimensions
    float screenWidth = 1360.0f;
    float screenHeight = 768.0f;
    
    // Calculate game board area positioning
    float boardStartX = screenWidth * 0.15f;
    float boardStartY = screenHeight * 0.1f;
    float boardSize = 16 * 40;
    
    // Center of the game board
    float boardCenterX = boardStartX + (boardSize / 2.0f);
    float boardCenterY = boardStartY + (boardSize / 2.0f);
    
    // STEP 1: Calculate actual content rectangle (card + text + portraits)
    
    // Get card dimensions - NEVER SCALE THE CARD, ALWAYS ORIGINAL SIZE
    float originalCardWidth = 64.0f;  // Default fallback
    float originalCardHeight = 64.0f; // Default fallback
    if (textures && cardSprite) {
        sf::Vector2u textureSize = cardSprite->getTexture().getSize();
        originalCardWidth = static_cast<float>(textureSize.x);
        originalCardHeight = static_cast<float>(textureSize.y);
        // NEVER apply scaling to card - it must match cards on right side
        cardSprite->setScale(sf::Vector2f(1.0f, 1.0f));
    }
    
    // Measure text content dimensions
    float textContentWidth = 0.0f;
    float textContentHeight = 0.0f;
    float lineHeight = std::max(static_cast<float>(PORTRAIT_SIZE + LABEL_SIZE + 5), static_cast<float>(TEXT_SIZE)) + 10;
    
    // Parse text segments into lines and measure each line
    std::vector<std::vector<size_t>> lineSegments;
    std::vector<std::vector<size_t>> linePortraits;
    std::vector<float> lineWidths;
    
    std::vector<size_t> currentLineSegments;
    std::vector<size_t> currentLinePortraits;
    float currentLineWidth = 0;
    size_t portraitIndex = 0;
    
    for (size_t i = 0; i < textSegments.size(); i++) {
        std::string segText = textSegments[i]->getString();
        
        // Check for line break marker
        if (segText == "LINEBREAK") {
            if (!currentLineSegments.empty()) {
                lineSegments.push_back(currentLineSegments);
                linePortraits.push_back(currentLinePortraits);
                lineWidths.push_back(currentLineWidth);
                textContentWidth = std::max(textContentWidth, currentLineWidth);
                
                currentLineSegments.clear();
                currentLinePortraits.clear();
                currentLineWidth = 0;
            }
            continue; // Skip LINEBREAK segments completely
        }
        
        sf::FloatRect bounds = textSegments[i]->getLocalBounds();
        
        // Check if this is a "Player X" segment that needs a portrait
        if (segText.find("Player ") == 0 && portraitIndex < inlinePortraits.size()) {
            currentLineWidth += PORTRAIT_SIZE + PORTRAIT_SPACING + bounds.size.x;
            currentLineSegments.push_back(i);
            currentLinePortraits.push_back(portraitIndex);
            portraitIndex++;
        } else {
            currentLineWidth += bounds.size.x;
            currentLineSegments.push_back(i);
        }
    }
    
    // Add final line if there are remaining segments
    if (!currentLineSegments.empty()) {
        lineSegments.push_back(currentLineSegments);
        linePortraits.push_back(currentLinePortraits);
        lineWidths.push_back(currentLineWidth);
        textContentWidth = std::max(textContentWidth, currentLineWidth);
    }
    
    textContentHeight = lineSegments.size() * lineHeight;
    
    // STEP 2: Calculate content bounding rectangle with proper text area padding
    float cardSpacing = 30.0f; // Space between card and text area
    float textPadding = 10.0f;  // Padding around text content (10px each side = 20px total)
    
    // Text area dimensions (content + padding on both sides)
    float textAreaWidth = textContentWidth + (textPadding * 2);
    float textAreaHeight = std::max(textContentHeight, originalCardHeight); // Ensure at least as tall as card
    
    // Total content rectangle - CARD NEVER CHANGES SIZE
    float contentRectWidth = originalCardWidth + cardSpacing + textAreaWidth;
    float contentRectHeight = std::max(originalCardHeight, textAreaHeight);
    
    // STEP 3: Create overlay 15% bigger than content rectangle
    float overlayWidth = contentRectWidth * 1.15f;
    float overlayHeight = contentRectHeight * 1.15f;
    
    // STEP 4: Handle size constraints by scaling TEXT ONLY, NEVER THE CARD
    float maxAllowedSize = boardSize * MAX_SCREEN_RATIO;
    bool needsTextScaling = false;
    float textScale = 1.0f;
    
    if (overlayWidth > maxAllowedSize || overlayHeight > maxAllowedSize) {
        needsTextScaling = true;
        
        // Calculate how much we need to scale the TEXT content to fit
        // Keep card at original size, only scale text elements
        float maxContentWidth = maxAllowedSize / 1.15f; // Reverse the 15% increase
        float maxContentHeight = maxAllowedSize / 1.15f;
        
        // Available space for text after card and spacing (card stays original size)
        float availableTextAreaWidth = maxContentWidth - originalCardWidth - cardSpacing;
        float availableTextAreaHeight = maxContentHeight;
        
        // Calculate text scaling needed
        float textScaleX = availableTextAreaWidth / textAreaWidth;
        float textScaleY = availableTextAreaHeight / textAreaHeight;
        textScale = std::min(textScaleX, textScaleY);
        textScale = std::min(textScale, 1.0f); // Never scale text larger than original
        
        // Apply scaling to text elements only
        for (auto& textSeg : textSegments) {
            if (textSeg->getString() != "LINEBREAK") {
                unsigned int newSize = static_cast<unsigned int>(TEXT_SIZE * textScale);
                textSeg->setCharacterSize(std::max(newSize, 12u)); // Minimum readable size
            }
        }
        
        // Scale portraits and labels
        for (auto& portrait : inlinePortraits) {
            portrait->setScale(sf::Vector2f(textScale, textScale));
        }
        for (auto& label : inlineLabels) {
            unsigned int newSize = static_cast<unsigned int>(LABEL_SIZE * textScale);
            label->setCharacterSize(std::max(newSize, 10u)); // Minimum readable size
        }
        
        // Recalculate text dimensions with scaling
        textContentWidth *= textScale;
        textContentHeight *= textScale;
        textAreaWidth = textContentWidth + (textPadding * textScale * 2);
        textAreaHeight = std::max(textContentHeight, originalCardHeight);
        
        // Recalculate content rectangle (card stays same size)
        contentRectWidth = originalCardWidth + cardSpacing + textAreaWidth;
        contentRectHeight = std::max(originalCardHeight, textAreaHeight);
        
        // Recalculate overlay
        overlayWidth = contentRectWidth * 1.15f;
        overlayHeight = contentRectHeight * 1.15f;
        
        // Update line widths with scaling
        for (size_t lineIdx = 0; lineIdx < lineWidths.size(); lineIdx++) {
            lineWidths[lineIdx] *= textScale;
        }
    }
    
    // Ensure minimum size but respect maximum constraints
    overlayWidth = std::max(overlayWidth, 200.0f);
    overlayHeight = std::max(overlayHeight, 100.0f);
    overlayWidth = std::min(overlayWidth, maxAllowedSize);
    overlayHeight = std::min(overlayHeight, maxAllowedSize);
    
    sf::Vector2f backgroundSize(overlayWidth, overlayHeight);
    backgroundRect->setSize(backgroundSize);
    
    // STEP 5: Center the overlay on the game board
    sf::Vector2f overlayPosition(
        boardCenterX - (overlayWidth / 2.0f),
        boardCenterY - (overlayHeight / 2.0f)
    );
    backgroundRect->setPosition(overlayPosition);
    
    // STEP 6: Position content within the overlay
    float contentAreaWidth = overlayWidth - PADDING * 2;
    float contentAreaHeight = overlayHeight - PADDING * 2;
    
    float contentStartX = overlayPosition.x + PADDING;
    float contentStartY = overlayPosition.y + PADDING;
    
    // Center content rectangle within overlay
    float contentOffsetX = (contentAreaWidth - contentRectWidth) / 2.0f;
    float contentOffsetY = (contentAreaHeight - contentRectHeight) / 2.0f;
    
    // Position card sprite on the left side of content rectangle - ORIGINAL SIZE
    if (textures && cardSprite) {
        float cardX = contentStartX + contentOffsetX;
        float cardY = contentStartY + contentOffsetY + (contentRectHeight - originalCardHeight) / 2.0f;
        cardSprite->setPosition(sf::Vector2f(cardX, cardY));
    }
    
    // Position text content within its allocated text area (with padding)
    float textAreaStartX = contentStartX + contentOffsetX + originalCardWidth + cardSpacing;
    float textAreaStartY = contentStartY + contentOffsetY + (contentRectHeight - textAreaHeight) / 2.0f;
    
    // Text content positioning within the text area (centered within padding)
    float effectiveTextPadding = textPadding * (needsTextScaling ? textScale : 1.0f);
    float textContentStartX = textAreaStartX + effectiveTextPadding;
    float textContentStartY = textAreaStartY + (textAreaHeight - textContentHeight) / 2.0f;
    
    // Position all text elements line by line, centered within text content area
    float currentY = textContentStartY;
    for (size_t lineIdx = 0; lineIdx < lineSegments.size(); lineIdx++) {
        float lineWidth = lineWidths[lineIdx];
        
        // Center each line horizontally within the text content area
        float lineOffsetX = (textContentWidth - lineWidth) / 2.0f;
        float currentX = textContentStartX + lineOffsetX;
        
        // Position segments in this line
        for (size_t segIdx = 0; segIdx < lineSegments[lineIdx].size(); segIdx++) {
            size_t textSegmentIdx = lineSegments[lineIdx][segIdx];
            std::string segText = textSegments[textSegmentIdx]->getString();
            
            // Skip LINEBREAK markers completely
            if (segText == "LINEBREAK") {
                continue;
            }
            
            // Check if this segment has a corresponding portrait
            bool hasPortrait = false;
            size_t portraitIdx = 0;
            
            for (size_t pIdx = 0; pIdx < linePortraits[lineIdx].size(); pIdx++) {
                if (linePortraits[lineIdx][pIdx] < inlinePortraits.size()) {
                    if (segText.find("Player ") == 0) {
                        hasPortrait = true;
                        portraitIdx = linePortraits[lineIdx][pIdx];
                        break;
                    }
                }
            }
            
            if (hasPortrait && portraitIdx < inlinePortraits.size()) {
                // Position portrait
                inlinePortraits[portraitIdx]->setPosition(sf::Vector2f(currentX, currentY));
                
                // Position label below portrait
                if (portraitIdx < inlineLabels.size()) {
                    sf::FloatRect labelBounds = inlineLabels[portraitIdx]->getLocalBounds();
                    float portraitWidth = PORTRAIT_SIZE * (needsTextScaling ? textScale : 1.0f);
                    inlineLabels[portraitIdx]->setPosition(sf::Vector2f(
                        currentX + (portraitWidth - labelBounds.size.x) / 2.0f,
                        currentY + PORTRAIT_SIZE * (needsTextScaling ? textScale : 1.0f) + 3
                    ));
                }
                
                // Position text after portrait
                float portraitWidth = PORTRAIT_SIZE * (needsTextScaling ? textScale : 1.0f);
                float portraitMiddle = currentY + (PORTRAIT_SIZE * (needsTextScaling ? textScale : 1.0f)) / 2.0f;
                float textY = portraitMiddle - textSegments[textSegmentIdx]->getCharacterSize() / 2.0f;
                textSegments[textSegmentIdx]->setPosition(sf::Vector2f(
                    currentX + portraitWidth + PORTRAIT_SPACING, 
                    textY
                ));
                
                currentX += portraitWidth + PORTRAIT_SPACING + textSegments[textSegmentIdx]->getLocalBounds().size.x;
            } else {
                // Regular text segment
                float portraitMiddle = currentY + (PORTRAIT_SIZE * (needsTextScaling ? textScale : 1.0f)) / 2.0f;
                float textY = portraitMiddle - textSegments[textSegmentIdx]->getCharacterSize() / 2.0f;
                textSegments[textSegmentIdx]->setPosition(sf::Vector2f(currentX, textY));
                currentX += textSegments[textSegmentIdx]->getLocalBounds().size.x;
            }
        }
        
        currentY += lineHeight * (needsTextScaling ? textScale : 1.0f);
    }
    
    // Hide the original notification text since we're using segments
    notificationText->setString("");
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
        
        // Draw semi-transparent background first
        target.draw(*backgroundRect, states);
        
        // Draw card sprite if available
        if (textures && cardSprite) {
            sf::Color cardColor = cardSprite->getColor();
            cardColor.a = static_cast<std::uint8_t>(255 * alpha);
            cardSprite->setColor(cardColor);
            target.draw(*cardSprite, states);
            cardColor.a = 255;
            cardSprite->setColor(cardColor);
        }
        
        // Draw inline layout elements
        if (textures && !textSegments.empty()) {
            // Draw all text segments
            for (const auto& textSeg : textSegments) {
                // Skip LINEBREAK markers
                if (textSeg->getString() == "LINEBREAK") {
                    continue;
                }
                
                sf::Color textColor = textSeg->getFillColor();
                textColor.a = static_cast<std::uint8_t>(255 * alpha);
                textSeg->setFillColor(textColor);
                target.draw(*textSeg, states);
                textColor.a = 255;
                textSeg->setFillColor(textColor);
            }
            
            // Draw all inline portraits
            for (const auto& portrait : inlinePortraits) {
                sf::Color portraitColor = portrait->getColor();
                portraitColor.a = static_cast<std::uint8_t>(255 * alpha);
                portrait->setColor(portraitColor);
                target.draw(*portrait, states);
                portraitColor.a = 255;
                portrait->setColor(portraitColor);
            }
            
            // Draw all inline labels
            for (const auto& label : inlineLabels) {
                sf::Color labelColor = label->getFillColor();
                labelColor.a = static_cast<std::uint8_t>(255 * alpha);
                label->setFillColor(labelColor);
                target.draw(*label, states);
                labelColor.a = 255;
                label->setFillColor(labelColor);
            }
        } else {
            // Fallback to original notification text if inline layout failed
            sf::Color textColor = notificationText->getFillColor();
            textColor.a = static_cast<std::uint8_t>(255 * alpha);
            notificationText->setFillColor(textColor);
            target.draw(*notificationText, states);
            textColor.a = 255;
            notificationText->setFillColor(textColor);
        }
        
        // Restore original background color for next frame
        bgColor.a = static_cast<std::uint8_t>(BACKGROUND_ALPHA);
        backgroundRect->setFillColor(bgColor);
    }
}

void CardNotification::setupCardSprite(const std::string& text, int cardPileNumber)
{
    if (!textures || !cardSprite || cardPileNumber < 0 || cardPileNumber >= 4) return;
    
    // Extract card type from text - look for the card type keywords
    std::string cardType = "";
    if (text.find("DOUBLE DIAMOND") != std::string::npos || text.find("DIAMOND X 2") != std::string::npos) {
        cardType = "diamond x 2";
    } else if (text.find("DIAMOND") != std::string::npos) {
        cardType = "diamond";
    } else if (text.find("STOP") != std::string::npos) {
        cardType = "stop";
    } else if (text.find("REMOVE") != std::string::npos) {
        cardType = "card";
    }
    
    // Map card type strings to indices
    int cardTypeIndex = -1;
    if (cardType == "stop") {
        cardTypeIndex = 0;
    } else if (cardType == "card") {
        cardTypeIndex = 1;
    } else if (cardType == "diamond") {
        cardTypeIndex = 2;
    } else if (cardType == "diamond x 2") {
        cardTypeIndex = 3;
    }
    
    if (cardTypeIndex >= 0 && cardTypeIndex < 4) {
        // Set the appropriate card texture based on pile number and card type
        cardSprite->setTexture(textures->cardsTextures[cardPileNumber][cardTypeIndex]);
        
        // Use original texture size (same as cards on the right side)
        // No scaling - this will match the cards displayed in the deck
        cardSprite->setScale(sf::Vector2f(1.0f, 1.0f));
    }
} 