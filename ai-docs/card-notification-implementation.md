# Card Notification System Implementation

This document outlines the implementation details of the card notification system in Deer Portal.

## Overview

The card notification system displays overlay notifications when players pick up cards, showing:
1. **Card Image**: Visual representation of the picked card on the left
2. **Player Portraits**: Character sprites positioned inline with text 
3. **Notification Text**: Context-aware messages describing the card effect

## Core Components

### CardNotification Class
- **File**: `src/cardnotification.h`, `src/cardnotification.cpp`
- **Purpose**: Manages display of card pickup notifications with visual feedback
- **Features**: 
  - Inline player portrait rendering
  - Card sprite display
  - Content-first overlay sizing
  - Proper centering and positioning

### Key Methods

#### `showCardNotification(cardType, playerNumber, targetPlayer, cardPileNumber)`
- Triggers notification display for card pickup events
- **Parameters**:
  - `cardType`: Type of card ("stop", "card", "diamond", "diamond x 2")
  - `playerNumber`: Player who picked up the card (0-3)
  - `targetPlayer`: Player affected by the card effect (-1 if self-target)
  - `cardPileNumber`: Element pile number (0-3) for texture selection

#### `setupCardSprite(text, cardPileNumber)`
- Configures card sprite based on card type and element
- Maps card types to texture indices: stop=0, card=1, diamond=2, diamond x 2=3
- Uses original texture size (no scaling) to match deck cards
- Uses `textures->cardsTextures[pileNumber][cardTypeIndex]` texture array

## Layout System

### Content-First Sizing Algorithm
The notification system uses a sophisticated content-first approach:

1. **Content Rectangle Calculation**: 
   - Measure card dimensions from actual texture: `cardSprite->getTexture().getSize()`
   - Measure text content dimensions by parsing inline segments
   - Add padding around text content: `textContentWidth + (textPadding * 2)`
   - Calculate total: `cardWidth + cardSpacing + textAreaWidth`

2. **Overlay Sizing**:
   - Create overlay 15% larger than content rectangle
   - `overlayWidth = contentRectWidth * 1.15f`
   - `overlayHeight = contentRectHeight * 1.15f`

3. **Text Area Layout**:
   - Text area gets padding: 10px on each side (20px total)
   - Card spacing: 30px between card and text area
   - Text content centered within padded text area

4. **Constraint Handling**:
   - Check if overlay exceeds board area (`boardSize * MAX_SCREEN_RATIO`)
   - If too large, calculate text-only scale factor (card remains original size)
   - Apply scale only to text elements, portraits, labels - NEVER the card

5. **Positioning Hierarchy**:
   - Center content rectangle within overlay
   - Position card on left side of content rectangle (always original size)
   - Position text area after card + spacing
   - Center text content within text area (respecting padding)

### Scaling Implementation (SFML 3.0)
```cpp
// Content measurement - card always original size
sf::Vector2u textureSize = cardSprite->getTexture().getSize();
float originalCardWidth = static_cast<float>(textureSize.x);
float originalCardHeight = static_cast<float>(textureSize.y);

// NEVER scale the card - always enforce original size
cardSprite->setScale(sf::Vector2f(1.0f, 1.0f));

// Text-only scaling when needed
if (overlayWidth > maxAllowedSize || overlayHeight > maxAllowedSize) {
    // Calculate available space for text (card size fixed)
    float availableTextAreaWidth = maxContentWidth - originalCardWidth - cardSpacing;
    float textScale = std::min(availableTextAreaWidth / textAreaWidth, 1.0f);
    
    // Scale text elements only - NEVER the card
    for (auto& textSeg : textSegments) {
        unsigned int newSize = static_cast<unsigned int>(TEXT_SIZE * textScale);
        textSeg->setCharacterSize(std::max(newSize, 12u));
    }
    
    // Scale portraits and labels
    for (auto& portrait : inlinePortraits) {
        portrait->setScale(sf::Vector2f(textScale, textScale));
    }
    
    // Card remains untouched at original size
}
```

### SFML 3.0 Compliance
- **Texture Size**: Uses `sf::Vector2u textureSize = texture.getSize()`
- **Bounds Access**: Uses `bounds.size.x` and `bounds.size.y` instead of `.width/.height`
- **Vector Construction**: Uses `sf::Vector2f(x, y)` syntax
- **Texture Rectangles**: Uses `sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(w, h))`

### Final Layout Structure
```
[Card Image: Original/Scaled] [15px spacing] [Player Portraits + Text Content]
                                   
Overlay: Content rectangle + 15% margin, centered on board
Card: Left side, vertically centered within content area
Text: Inline portraits before "Player X", centered within text area
Scaling: Proportional when overlay exceeds board constraints
```

### Content Measurement Process
1. **Setup Card Sprite**: Configure texture and get dimensions
2. **Parse Text Segments**: Split into lines, measure each line width
3. **Calculate Content Bounds**: `max(cardHeight, textHeight)` x `cardWidth + spacing + textWidth`
4. **Apply Constraints**: Scale proportionally if needed
5. **Position Elements**: Center overlay, then center content within overlay

### Card Sizing Strategy
- **Original Size**: Cards use their native texture dimensions (no scaling applied)
- **Consistency**: Matches the size of cards displayed in the deck on the right side
- **Dynamic Measurement**: Card dimensions retrieved from texture at runtime
- **Responsive Layout**: Overlay adjusts to accommodate actual card size

### Portrait Integration
- **Character Sprites**: Extracted from `characters-new.png` (32x58 pixels each)
- **Player Mapping**: Player offset = playerNumber * 32 pixels
- **Inline Positioning**: Portraits appear directly before each "Player X" text mention
- **Labels**: "P1", "P2", etc. displayed below each portrait

## Card Texture System

### Texture Organization
Cards are stored in `textures->cardsTextures[element][cardType]`:
- **Elements**: 0=Water, 1=Earth, 2=Fire, 3=Air
- **Card Types**: 0=Stop, 1=Remove Card, 2=Diamond, 3=Diamond x2

### Card Type Extraction
Text parsing to identify card type from notification message:
- "DOUBLE DIAMOND" or "DIAMOND X 2" → `diamond x 2`
- "DIAMOND" → `diamond`  
- "STOP" → `stop`
- "REMOVE" → `card`

## Integration Points

### Command System
**File**: `src/command.cpp`, method `processCard()`
- Triggers notifications when players land on card fields
- Passes `tokenNumber` as both `targetPlayer` and `cardPileNumber`
- Called for each card type: diamond, stop, card, diamond x 2

### Visual Rendering
**File**: `src/cardnotification.cpp`, method `draw()`
- Renders background rectangle first
- Draws card sprite with alpha blending
- Renders inline text segments and portraits  
- Applies gentle blinking effect (100% to 90% opacity)

## Constants and Configuration

```cpp
const unsigned int TEXT_SIZE = 32;        // Main text size
const unsigned int LABEL_SIZE = 18;       // Portrait label size  
const unsigned int PORTRAIT_SIZE = 32;    // Portrait display size
const unsigned int PORTRAIT_SPACING = 8;  // Space between portrait and text
const float PADDING = 20.0f;             // Overlay internal padding
const float MAX_SCREEN_RATIO = 0.85f;    // Max overlay size vs board area
const float BACKGROUND_ALPHA = 180.0f;   // Semi-transparent background
const float BLINK_INTERVAL = 1.5f;       // Blinking effect timing
```

## Technical Implementation Details

### SFML 3.0 Compatibility
- Sprites require texture in constructor: `sf::Sprite(texture)`
- Uses `sf::Vector2i` for texture rectangles
- Proper texture binding for card sprites

### Memory Management
- Smart pointers (`std::unique_ptr`) for all visual components
- Automatic cleanup when notification dismissed
- Safe texture access with null checks

### Performance Considerations
- Pre-calculated layout measurements
- Minimal redraw operations
- Efficient text segmentation
- Single-pass positioning algorithm

## Usage Example

```cpp
// Player 0 picks up a diamond card from pile 2 (fire element), targeting player 1
game.cardNotification.showCardNotification("diamond", 0, 1, 2);
```

This creates a notification showing:
- Fire diamond card image on the left (original texture size)
- "Player 1 picked up a DIAMOND card!" with Player 1's portrait
- "Player 2 lost a diamond and Player 1 gained cash!" with both portraits
- Overlay sized to content bounds + 15% margin, centered on game board

## Future Enhancements

### Potential Improvements
1. **Animation Effects**: Slide-in animations for card appearance
2. **Sound Integration**: Unique sound effects per card type
3. **Enhanced Styling**: Card-specific background colors or effects
4. **Multi-language Support**: Localized text with proper layout handling

### Configuration Options
- Adjustable card display size
- Customizable spacing parameters  
- Configurable overlay transparency
- Optional animation toggles

## Related Files

- `src/cardnotification.h` - Class declaration and constants
- `