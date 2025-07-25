# Sprites and Graphics Assets Documentation

**Source Knowledge**: Commit f172dfbb606f81674834d0862092697acdeb19fa

## Overview

DeerPortal uses SFML 3.0 sprite system with texture atlases and individual sprite files for game graphics. Assets are managed through the `TextureHolder` class and loaded via `SafeAssetLoader`.

## Asset Directory Structure

### Main Asset Locations
- `assets/img/`: Primary image assets
- `assets/img/cards/`: Card sprite collection
- `assets/img/gui/`: GUI elements
- `assets/d1.png` to `d5.png`: Dice face sprites

## Core Sprite Assets

### Background Graphics
- `background_dark.png`: Dark theme background
- `background_land.png`: Land/nature themed background

### Board Elements
- `board_diamonds.png`: Diamond positions on game board
- `diamond-big.png`: Large diamond sprite for special effects

### Character Sprites
- `characters.png`: Original character sprite sheet
- `characters-new.png`: Updated character sprites
- Animation support through `AnimatedSprite` class

### Dice Graphics
- `diceWhite.png`: Standard dice texture
- `d1.png` to `d5.png`: Individual dice face sprites
- `dicesWhite.xcf`: GIMP source file for dice graphics

### UI Elements
- `button-cpu.png`: CPU player button
- `button-human.png`: Human player button
- `bubble_dice.png`: Dice roll information bubble
- `bubble_footsteps.png`: Movement indicator bubble
- `rotate.png`: Rotation indicator sprite
- `letsbegin.png`: Game start indicator
- `gui/close.png`: Window close button

### Branding
- `deerportal.png`: Main game logo
- `deer-god.png`: Deity character sprite
- `dp_intro_menu.png`: Intro menu background

## Card Sprite System

### Card Types and Elements
The card system uses a comprehensive sprite collection in `assets/img/cards/`:

#### Air Element Cards
- `card-air-diam.png/.small.png`: Air diamond collection
- `card-air-2-diam.png/.small.png`: Air double diamond
- `card-air-remove-card.png/.small.png`: Air removal effect
- `card-air-stop.png/.small.png`: Air stop effect

#### Earth Element Cards  
- `card-earth-diam.png/.small.png`: Earth diamond collection
- `card-earth-2-diam.png/.small.png`: Earth double diamond
- `card-earth-remove-card.png/.small.png`: Earth removal effect
- `card-earth-stop.png/.small.png`: Earth stop effect

#### Fire Element Cards
- `card-fire-diam.png/.small.png`: Fire diamond collection
- `card-fire-2-diam.png/.small.png`: Fire double diamond
- `card-fire-remove-card.png/.small.png`: Fire removal effect
- `card-fire-stop.png/.small.png`: Fire stop effect

#### Water Element Cards
- `card-water-diam.png/.small.png`: Water diamond collection
- `card-water-2-diam.png/.small.png`: Water double diamond
- `card-water-remove-card.png/.small.png`: Water removal effect
- `card-water-stop.png/.small.png`: Water stop effect

### Card Sprite Usage
- Full-size sprites for normal display
- `.small.png` variants for reduced UI contexts
- Duplicate sprites in main `img/` directory for backward compatibility
- `_m` suffix variants (e.g., `card-air-2-diam_m.png`) for specific uses

## Texture Atlas Implementation

### Diamond Sprite Atlas
The particle system references diamond sprites through texture coordinates:
```cpp
sf::IntRect(sf::Vector2i(4 * 44, 0), sf::Vector2i(44, 44))
```
This indicates a 44x44 pixel sprite at position (176, 0) in the texture atlas.

### Texture Coordinate System
- 44x44 pixel tiles in atlas arrangement
- Zero-based indexing for atlas positions
- Support for multiple sprite variations per tile

## Sprite Loading and Management

### Asset Loading Process
1. Assets loaded through `SafeAssetLoader::loadTexture()`
2. Textures stored in `TextureHolder` for reuse
3. Sprites created with texture references
4. Multi-platform path resolution for different distributions

### Cross-Platform Asset Support
- macOS app bundle asset loading
- Linux AppImage `APPDIR` support
- Windows relative path resolution
- Fallback path mechanisms for missing assets

## Animation System Integration

### Animated Sprites
- `AnimatedSprite` class for character animations
- Frame-based animation support
- Integration with `GameAnimationSystem`

### Particle Sprite Usage
- Diamond collection particles use atlas coordinates
- Card particles reference card textures
- Custom texture support through `ParticleConfig`

## Graphics Quality and Optimization

### Sprite Formats
- PNG format for transparency support
- Optimized for SFML texture loading
- Various resolutions for different UI contexts

### Memory Management
- Texture sharing through `TextureHolder`
- RAII pattern with `std::unique_ptr` for SFML objects
- Efficient sprite reuse for particles

The sprite system supports the game's visual identity while maintaining performance through efficient texture management and atlas-based rendering.