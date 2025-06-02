# Deer Portal - Cards Reference

## Overview

The Deer Portal game features a sophisticated card system with four types of cards across four classical elements. Each element (Water, Earth, Fire, Air) has its own deck of 32 cards that players can interact with during gameplay.

## Card Distribution System

Each element deck contains exactly:
- **8 "Stop" cards** - Freeze opponent players
- **8 "Card" cards** - Remove opponent's cards/diamonds  
- **8 "Diamond" cards** - Steal one diamond from opponent
- **8 "Diamond x 2" cards** - Steal two diamonds from opponent

**Total**: 32 cards per element × 4 elements = **128 cards** in the game

## How Cards Work

### Card Activation
1. **Own Element**: When you land on your own element's rune, the card is simply discarded and the next card in the pile is revealed
2. **Other Elements**: When you land on another element's rune, the card effect is **executed against that element's area** and then the card is discarded

### Important Rules
- You **cannot** use cards against your own element
- Cards target the **element area** where the rune was collected, not specific players
- After a card is used, the deck advances to the next card
- When a deck runs out of cards, that element pile becomes inactive

## Card Types by Element

### Water Element Cards (Blue)

#### Stop Card
![Water Stop Card](assets/img/cards/card-water-stop.small.png)
- **Effect**: Freezes the target player for 1 turn (`frozenLeft += 1`)
- **Usage**: Land on a Water rune from another element's area
- **Mechanic**: Prevents the targeted player from moving on their next turn

#### Remove Card  
![Water Remove Card](assets/img/cards/card-water-remove-card.small.png)
- **Effect**: Removes a random diamond/card from target area
- **Usage**: Eliminates strategic resources from opponent's board
- **Mechanic**: Randomly selects and removes one item from the target element's area

#### Diamond Card
![Water Diamond Card](assets/img/cards/card-water-diam.small.png)  
- **Effect**: Steals 1 diamond from target area + 1 cash to current player
- **Usage**: Direct resource theft
- **Mechanic**: `removeDiamond()` + `cash += 1`

#### Diamond x 2 Card
![Water Diamond x2 Card](assets/img/cards/card-water-2-diam.small.png)
- **Effect**: Steals 2 diamonds from target area + 2 cash to current player  
- **Usage**: Maximum resource theft per card
- **Mechanic**: `removeDiamond()` called twice + `cash += 2`

### Earth Element Cards (Green)

#### Stop Card
![Earth Stop Card](assets/img/cards/card-earth-stop.small.png)
- **Effect**: Freezes the target player for 1 turn
- **Visual**: Shows yellow stop symbol on green background with earth textures

#### Remove Card
![Earth Remove Card](assets/img/cards/card-earth-remove-card.small.png)  
- **Effect**: Removes a random diamond/card from target area
- **Visual**: Shows crossed-out diamond on green earth-themed background

#### Diamond Card  
![Earth Diamond Card](assets/img/cards/card-earth-diam.small.png)
- **Effect**: Steals 1 diamond from target area + 1 cash
- **Visual**: Single diamond on green earth-themed background

#### Diamond x 2 Card
![Earth Diamond x2 Card](assets/img/cards/card-earth-2-diam.small.png)
- **Effect**: Steals 2 diamonds from target area + 2 cash
- **Visual**: Two diamonds on green earth-themed background

### Fire Element Cards (Red)

#### Stop Card
![Fire Stop Card](assets/img/cards/card-fire-stop.small.png)
- **Effect**: Freezes the target player for 1 turn  
- **Visual**: Yellow stop symbol on red fiery background

#### Remove Card
![Fire Remove Card](assets/img/cards/card-fire-remove-card.small.png)
- **Effect**: Removes a random diamond/card from target area
- **Visual**: Crossed-out diamond on red fire-themed background

#### Diamond Card
![Fire Diamond Card](assets/img/cards/card-fire-diam.small.png)
- **Effect**: Steals 1 diamond from target area + 1 cash
- **Visual**: Single diamond on red fire-themed background

#### Diamond x 2 Card  
![Fire Diamond x2 Card](assets/img/cards/card-fire-2-diam.small.png)
- **Effect**: Steals 2 diamonds from target area + 2 cash
- **Visual**: Two diamonds on red fire-themed background with flames

### Air Element Cards (Yellow)

#### Stop Card
![Air Stop Card](assets/img/cards/card-air-stop.small.png)
- **Effect**: Freezes the target player for 1 turn
- **Visual**: Yellow stop symbol on yellow/white airy background

#### Remove Card  
![Air Remove Card](assets/img/cards/card-air-remove-card.small.png)
- **Effect**: Removes a random diamond/card from target area
- **Visual**: Crossed-out diamond on yellow air-themed background

#### Diamond Card
![Air Diamond Card](assets/img/cards/card-air-diam.small.png) 
- **Effect**: Steals 1 diamond from target area + 1 cash
- **Visual**: Single diamond on yellow air-themed background

#### Diamond x 2 Card
![Air Diamond x2 Card](assets/img/cards/card-air-2-diam.small.png)
- **Effect**: Steals 2 diamonds from target area + 2 cash  
- **Visual**: Two diamonds on yellow air-themed background

## Strategic Considerations

### Card Priority
1. **"Diamond x 2"** - Highest value, maximum resource gain
2. **"Diamond"** - Solid resource gain
3. **"Stop"** - Strategic disruption, delays opponents
4. **"Remove Card"** - Tactical disruption, reduces opponent options

### Timing Strategy
- **Early Game**: Focus on diamond cards to build resource advantage
- **Mid Game**: Use stop cards to disrupt leading players  
- **Late Game**: Remove card effects to eliminate opponent threats before portal rush

### Element Targeting
- Target elements with the most valuable visible cards/diamonds
- Avoid using cards when target area is empty (wasted effect)
- Consider which players control which elements for optimal disruption

## Technical Implementation

### Card Processing Code
```cpp
void Command::processCard(int pos) {
    int tokenNumber = game.boardDiamonds.getNumberForField(pos);
    std::string cardType = game.cardsDeck.getTitle(tokenNumber);
    
    if (tokenNumber != game.turn) {  // Can't target own element
        if (cardType == "diamond") {
            removeDiamond(game.boardDiamonds.getNumberForField(pos));
            game.players[game.turn].cash += 1;
        } else if (cardType == "stop") {
            freezePlayer(tokenNumber);
        } else if (cardType == "card") {
            removeCard(game.boardDiamonds.getNumberForField(pos));
        } else if (cardType == "diamond x 2") {
            if (removeDiamond(game.boardDiamonds.getNumberForField(pos)))
                game.players[game.turn].cash += 1;
            if (removeDiamond(game.boardDiamonds.getNumberForField(pos)))
                game.players[game.turn].cash += 1;
        }
    }
    game.cardsDeck.nextCard(tokenNumber);  // Advance to next card
}
```

### Card States
- **Active**: Card is visible and can be triggered
- **Invisible**: Card is temporarily hidden after use (`invisibleLeft = 0.75f`)
- **Inactive**: Deck is exhausted, no more cards available

## Related Game Mechanics

### Meditation System
- Returns to starting position regenerates **all diamonds and cards** in your area
- Requires exact dice roll to reach starting position
- Triggered by: `game.boardDiamonds.reorder(game.turn)`

### Deck Exhaustion
- When a deck runs out of cards: `cardsList[pileNumber].active = false`
- All remaining items of that element are removed: `removeAllCardElement(pileNumber)`
- Element becomes strategically less valuable

### Sound Effects
- Card activation: `game.sfx.playCard()`
- Each card type triggers the same sound effect
- Visual feedback through temporary card invisibility

## Card Asset Files

All card images are stored in `assets/img/cards/` with the naming convention:
- `card-{element}-{type}.small.png` (displayed in game)
- `card-{element}-{type}.png` (high resolution versions)

Where:
- `{element}` = water, earth, fire, air  
- `{type}` = stop, remove-card, diam, 2-diam 