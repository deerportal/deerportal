# Sound Effects System Documentation

**Source Knowledge**: Commit f172dfbb606f81674834d0862092697acdeb19fa

## Overview

DeerPortal implements a comprehensive sound effects system through the `SoundFX` class (`src/soundfx.h/cpp`) that provides audio feedback for game interactions and events.

## Sound Effect Files

### Core Sound Assets

#### User Interface Sounds
- **File**: `assets/audio/click.ogg`
- **Usage**: Button clicks and UI interactions
- **Loading**: Via `SafeAssetLoader::loadSound()` in `GameAssets`
- **Buffer**: `sfxClickBuffer`

- **File**: `assets/audio/done.ogg`  
- **Usage**: Completion confirmations and success feedback
- **Loading**: Via `SafeAssetLoader::loadSound()` in `GameAssets`
- **Buffer**: `sfxDoneBuffer`

#### Gameplay Sounds
- **File**: `assets/audio/collect.ogg`
- **Usage**: Diamond and item collection feedback
- **Implementation**: `SoundFX::playCollect()` method
- **Buffer**: `soundCollectBuffer`
- **Alternative**: `assets/audio/collect.mp3` (backup format)

- **File**: `assets/audio/card.ogg`
- **Usage**: Card play and card-related actions
- **Implementation**: `SoundFX::playCard()` method  
- **Buffer**: `soundCardBuffer`

- **File**: `assets/audio/dice.ogg`
- **Usage**: Dice rolling sound effects
- **Location**: Loaded in `RoundDice` class
- **Buffer**: `sfxDiceBuffer`

#### Special Event Sounds
- **File**: `assets/audio/letsbegin.ogg`
- **Usage**: Game start sequence audio
- **Implementation**: `SoundFX::playLetsBegin()` method
- **Buffer**: `soundLetsBeginBuffer`

#### Thematic Audio Effects
- **File**: `assets/audio/dp-deermode.ogg`
- **Usage**: Deer mode activation sound
- **Buffer**: `bufferDeerMode`
- **Sound**: `soundDeerMode`

- **File**: `assets/audio/dp-meditation.ogg`
- **Usage**: Meditation or special state audio
- **Buffer**: `bufferMeditation`
- **Sound**: `soundMeditation`

- **File**: `assets/audio/dp-ok.ogg`  
- **Usage**: Confirmation and acknowledgment sound
- **Buffer**: `bufferPortal`
- **Sound**: `soundPortal`

## SoundFX Class Implementation

### Core Structure (`src/soundfx.h`)
```cpp
class SoundFX {
public:
  // Sound buffers for audio data
  sf::SoundBuffer soundCollectBuffer;
  sf::SoundBuffer soundCardBuffer;
  sf::SoundBuffer bufferDeerMode;
  sf::SoundBuffer bufferMeditation;
  sf::SoundBuffer bufferPortal;
  sf::SoundBuffer soundLetsBeginBuffer;
  
  // Sound objects for playback
  sf::Sound soundPortal;
  sf::Sound soundDeerMode;
  sf::Sound soundMeditation;
  sf::Sound soundCollect;
  sf::Sound soundCard;
  sf::Sound soundLetsBegin;
  
  // Playback methods
  void playCollect();
  void playCard();
  void playLetsBegin();
};
```

### Loading Implementation (`src/soundfx.cpp`)
Sound effects are loaded with error handling:

```cpp
// Collection sound loading
if (!soundCollectBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/collect.ogg"))) {
  throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/collect.ogg", "SoundFX constructor");
}

// Card sound loading  
if (!soundCardBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/card.ogg"))) {
  throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND,
                                       "audio/card.ogg", "SoundFX constructor");
}
```

## Sound Integration Points

### Game Event Triggers
- **Diamond Collection**: `playCollect()` called when diamonds collected
- **Card Actions**: `playCard()` triggered on card play
- **Game Start**: `playLetsBegin()` during game initialization
- **UI Interactions**: Click sounds on button presses
- **Action Completion**: Done sounds on successful actions

### Dice Sound System (`src/rounddice.cpp`)
Specialized dice sound handling:
```cpp
if (!sfxDiceBuffer.loadFromFile(get_full_path(ASSETS_PATH "audio/dice.ogg"))) {
  throw DeerPortal::AssetLoadException(DeerPortal::AssetLoadException::SOUND, 
                                       "audio/dice.ogg", "RoundDice constructor");
}
```

## Audio Asset Management

### Loading Strategy
- Sound buffers loaded during initialization
- `get_full_path()` handles cross-platform asset locations
- Exception-based error handling for missing files
- Centralized loading through `SafeAssetLoader`

### Memory Management
- SFML sound buffer/sound object pattern
- Buffers store audio data
- Sound objects reference buffers for playback
- Automatic cleanup through RAII

### Cross-Platform Support
- Asset path resolution for different distributions
- macOS app bundle compatibility
- Linux AppImage `APPDIR` support
- Windows relative path handling

## Audio Format Specifications

### File Format
- **Primary Format**: OGG Vorbis (`.ogg`)
- **Backup Format**: MP3 (`.mp3`) for collect sound
- **Advantages**: Good compression, SFML compatibility, open source

### Audio Quality
- Optimized for sound effect playback
- Short duration files for responsive feedback
- Balanced file size vs. quality

## Performance Considerations

### Efficient Playback
- Pre-loaded sound buffers prevent runtime loading delays
- SFML's optimized audio pipeline
- Minimal memory footprint for short sound effects

### Concurrent Audio
- Multiple sound objects can play simultaneously
- No interference between different sound effects
- Proper resource sharing between buffer and sound objects

## Error Handling

### Asset Loading Exceptions
- `AssetLoadException` thrown for missing sound files
- Detailed error messages with file paths
- Graceful degradation if optional sounds unavailable

### Debug Support
- Exception messages include context information
- File path validation for troubleshooting
- Cross-platform path debugging support

The sound effects system provides immediate audio feedback that enhances player interaction and game immersion while maintaining efficient resource usage and cross-platform compatibility.