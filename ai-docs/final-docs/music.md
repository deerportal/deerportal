# Music System Documentation  

**Source Knowledge**: Commit 60a939ef8d8de62a00d82cdf3da244d8a17463ea

## Overview

DeerPortal implements a context-aware music system that plays different tracks based on game state. Music is managed through the `GameStateManager` and loaded via the `GameAssets` module.

## Music Asset Files

### Core Music Tracks

#### Menu Music
- **File**: `assets/audio/menu.ogg`
- **Usage**: Main menu background music
- **Loading**: `musicMenu.openFromFile()` in `GameAssets`
- **State**: Plays during `state_menu`

#### Game Music  
- **File**: `assets/audio/game.ogg`
- **Usage**: Primary gameplay background music
- **Loading**: `musicGame.openFromFile()` in `GameAssets`
- **State**: Plays during active gameplay states
- **Backup**: `assets/audio/game.old.ogg` (legacy version)

#### Secondary Menu Track
- **File**: `assets/audio/menu-2.ogg`
- **Usage**: Alternative menu music track
- **Purpose**: Provides variety in menu experience

## Music Loading Implementation

### Asset Loading Process
Located in `src/game-assets.cpp`:

```cpp
// Game music loading with error handling
if (!musicGame.openFromFile(get_full_path(ASSETS_PATH "audio/game.ogg"))) {
  throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, "game.ogg", 
                                       get_full_path(ASSETS_PATH "audio/game.ogg"));
}

// Menu music loading with error handling  
if (!musicMenu.openFromFile(get_full_path(ASSETS_PATH "audio/menu.ogg"))) {
  throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, "menu.ogg",
                                       get_full_path(ASSETS_PATH "audio/menu.ogg"));
}
```

### Cross-Platform Path Resolution
- `get_full_path()` function handles different platforms
- macOS app bundle asset detection
- Linux AppImage `APPDIR` support  
- Windows relative path resolution

## Music State Management

### State-Based Music Control
The `GameStateManager` handles music transitions:

#### Menu Music Control
- `startMenuMusic()`: Begins menu background music
- `stopMenuMusic()`: Stops menu music during transitions
- Activated during `state_menu` and related states

#### Game Music Control
- `startGameMusic()`: Begins gameplay background music
- `stopGameMusic()`: Stops game music when leaving gameplay
- Activated during `state_game` and active gameplay phases

### Audio Transition Coordination
- `handleStateAudio(int newState)`: Manages music switching
- `playStateMusic(int state)`: Starts appropriate track for state
- `stopStateMusic(int state)`: Stops current state music

## Music Integration Points

### Game State Synchronization
Music changes coordinate with:
- Menu system activation/deactivation
- Game start sequences
- End game transitions
- State machine changes

### Volume and Playback Control
- SFML `sf::Music` objects provide volume control
- Looping enabled for continuous background music
- Smooth transitions between different tracks

## Technical Implementation

### SFML Music Objects
- `sf::Music musicGame`: Game background music player
- `sf::Music musicMenu`: Menu background music player
- Stream-based playback for large audio files
- Automatic resource management

### Error Handling
- `AssetLoadException` thrown for missing music files
- Graceful degradation if music files unavailable
- Debug logging for music loading issues

### Performance Considerations
- Streaming audio prevents large memory usage
- Efficient SFML audio pipeline
- Background loading prevents gameplay interruption

## Music File Specifications

### Audio Format
- **Format**: OGG Vorbis
- **Advantages**: Good compression, open source, SFML compatible
- **Quality**: Optimized for background music playback

### File Organization
- Centralized in `assets/audio/` directory
- Consistent naming convention
- Version management (`.old` variants for legacy support)

The music system provides atmospheric background audio that enhances the gaming experience while maintaining efficient resource usage and smooth state transitions.