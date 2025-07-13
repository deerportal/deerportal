# DeerPortal Fullscreen Implementation Plan

## Overview
Implement cross-platform fullscreen toggle functionality using the 'F' key in DeerPortal with SFML 3.0.1.

## Requirements
- **Trigger Key**: F (not F11 due to macOS complications)
- **Platforms**: Windows, macOS, Linux
- **SFML Version**: 3.0.1
- **Behavior**: Toggle between windowed and fullscreen modes
- **State Persistence**: Remember user preference between sessions

## Architecture Design

### 1. State Management
```cpp
// In src/game.h
class Game {
private:
    bool m_isFullscreen = false;
    sf::VideoMode m_windowedMode;
    sf::VideoMode m_fullscreenMode;
    
    // Store window position for restoration
    sf::Vector2i m_windowedPosition;
};
```

### 2. Configuration Storage
- Add fullscreen preference to game settings
- Store in configuration file (JSON/INI format)
- Location: User's home directory or game data folder

### 3. Implementation Pattern

#### 3.1 Window Recreation Approach (Recommended for SFML 3.0.1)
SFML 3.0.1 requires window recreation for style changes:

```cpp
void Game::toggleFullscreen() {
    if (m_isFullscreen) {
        // Switch to windowed
        window.close();
        window.create(m_windowedMode, 
                     "Deerportal - game about how human can be upgraded to the Deer",
                     sf::Style::Titlebar | sf::Style::Close);
        window.setPosition(m_windowedPosition);
    } else {
        // Store current window position
        m_windowedPosition = window.getPosition();
        
        // Switch to fullscreen
        window.close();
        window.create(m_fullscreenMode,
                     "Deerportal - game about how human can be upgraded to the Deer",
                     sf::Style::Fullscreen);
    }
    
    m_isFullscreen = !m_isFullscreen;
    
    // Restore view and other window properties
    restoreWindowProperties();
}
```

### 4. Platform-Specific Considerations

#### 4.1 Windows
- Use primary monitor's desktop mode
- Handle multi-monitor setups gracefully
- Consider DPI scaling issues

#### 4.2 macOS
- Respect macOS fullscreen conventions
- Handle Spaces and Mission Control properly
- Test on both Intel and Apple Silicon

#### 4.3 Linux
- Test on major desktop environments (GNOME, KDE, XFCE)
- Handle window manager quirks
- Consider Wayland vs X11 differences

### 5. Input Handling Integration

```cpp
// In game-input.cpp handleKeyboardInput()
case sf::Keyboard::Key::F:
    if (event.type == sf::Event::KeyPressed && !event.key.repeat) {
        toggleFullscreen();
    }
    break;
```

### 6. Resource Management

#### 6.1 Context Preservation
- Save OpenGL context state before window recreation
- Reload textures if necessary (SFML 3.0.1 should handle this)
- Preserve audio states

#### 6.2 View Adjustment
```cpp
void Game::restoreWindowProperties() {
    // Adjust view to new window size
    sf::Vector2u size = window.getSize();
    sf::View view(sf::FloatRect(0, 0, size.x, size.y));
    window.setView(view);
    
    // Update HUD positions
    updateHudPositions();
    
    // Restore framerate limit
    window.setFramerateLimit(60);
    
    // Restore vertical sync if enabled
    window.setVerticalSyncEnabled(true);
}
```

### 7. Error Handling

```cpp
bool Game::tryToggleFullscreen() {
    try {
        toggleFullscreen();
        return true;
    } catch (const std::exception& e) {
        // Log error
        std::cerr << "Fullscreen toggle failed: " << e.what() << std::endl;
        
        // Attempt recovery to windowed mode
        if (m_isFullscreen) {
            forceWindowedMode();
        }
        return false;
    }
}
```

### 8. Testing Strategy

#### 8.1 Unit Tests
- Mock window creation/destruction
- Test state transitions
- Verify configuration persistence

#### 8.2 Integration Tests
- Test on all target platforms
- Verify resource preservation
- Check memory leaks during transitions

#### 8.3 Edge Cases
- Rapid toggle spam
- Toggle during animations
- Toggle during card animations
- Alt+Tab behavior in fullscreen
- Multiple monitor configurations

### 9. Implementation Steps

1. **Phase 1: Core Infrastructure**
   - Add fullscreen state to Game class
   - Implement basic toggle function
   - Add configuration storage

2. **Phase 2: Window Management**
   - Implement safe window recreation
   - Add resource preservation
   - Handle view adjustments

3. **Phase 3: Input Integration**
   - Add 'F' key handler
   - Prevent toggle during critical operations
   - Add visual feedback

4. **Phase 4: Platform Testing**
   - Windows 10/11 testing
   - macOS 12+ testing
   - Ubuntu/Fedora/Arch testing

5. **Phase 5: Polish**
   - Add smooth transitions (fade effect)
   - Optimize performance
   - Add user notifications

### 10. Future Enhancements

- **Borderless Windowed Mode**: Alternative to true fullscreen
- **Resolution Selection**: Allow custom fullscreen resolutions
- **Multi-Monitor Support**: Choose which monitor for fullscreen
- **Hotkey Customization**: Let users choose toggle key

### 11. Code Organization

```
src/
├── window-manager.h       // New: Window state management
├── window-manager.cpp     // New: Implementation
├── game.h                 // Modified: Add fullscreen members
├── game.cpp              // Modified: Integrate window manager
├── game-input.cpp        // Modified: Add F key handler
└── config-manager.cpp    // New/Modified: Save preferences
```

### 12. Performance Considerations

- Minimize texture reloading
- Cache window states
- Avoid unnecessary recreations
- Profile transition time

### 13. Accessibility

- Provide alternative toggle method (menu option)
- Clear visual indication of current mode
- Respect system accessibility settings

### 14. Documentation Updates

- Update README with fullscreen controls
- Add to in-game help/controls screen
- Document in user manual

## Risk Mitigation

1. **Window Recreation Failures**
   - Always have fallback to windowed mode
   - Save state before attempting toggle

2. **Resource Loss**
   - Implement robust resource reloading
   - Test extensively on all platforms

3. **Platform Incompatibilities**
   - Abstract platform-specific code
   - Use conditional compilation where needed

## Success Criteria

- [ ] F key toggles fullscreen on all platforms
- [ ] No resource leaks during transitions
- [ ] Smooth user experience (< 500ms transition)
- [ ] Settings persist between sessions
- [ ] Graceful error handling
- [ ] No visual artifacts after toggle 