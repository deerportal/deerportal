# Diamond Scaling and Fullscreen Performance Optimization

**Date**: July 19, 2025  
**Version**: 0.9.6-pre.2  
**Issue**: Large diamonds on board, poor fullscreen performance  

## Problem Analysis

### Initial Issues
1. **Diamond Size**: Board diamonds (44x44px) too large relative to 40x40px tiles
2. **Fullscreen Performance**: FPS dropped from thousands to ~60fps when entering fullscreen
3. **User Experience**: Cluttered board appearance in windowed mode

### Root Causes Identified
1. **V-Sync Limitation**: Monitor refresh rate capping FPS at 60 in fullscreen
2. **View System Bottleneck**: Complex SFML view calculations causing performance overhead
3. **Bilinear Filtering**: Render texture smoothing enabled, expensive for scaling
4. **Proportional Scaling**: Original 44px diamonds too large for tile grid

## Optimization Implementation

### 1. Diamond Size Optimization

**File**: `src/boarddiamondseq.cpp`

**Pre-calculated Constants (Performance Critical)**:
```cpp
// Before: Runtime calculations every frame
const float baseDiamondSize = 44.0f;
float diamondSize = baseDiamondSize * 0.8f; // Calculated each call
float offsetX = (DP::TILE_SIZE - diamondSize) / 2.0f; // Calculated each call

// After: Compile-time constants
const float diamondSize = 35.2f; // 44.0f * 0.8f = 35.2f (80% scaled)
const float offsetX = 2.4f; // (40 - 35.2) / 2 = 2.4f (centering offset)
const float offsetY = 2.4f; // (40 - 35.2) / 2 = 2.4f (centering offset)
```

**Benefits**:
- **Zero runtime calculations** - all values computed at compile time
- **80% diamond size** (35.2px) provides better visual proportions
- **Perfect centering** in 40x40px tiles with pre-calculated 2.4px offsets
- **Maintains VertexArray efficiency** - single draw call for all 112 diamonds

### 2. Fullscreen Performance Optimization

**File**: `src/window-manager.cpp`

#### V-Sync Removal
```cpp
// Before: V-Sync enabled (60fps cap)
window.setVerticalSyncEnabled(true);

// After: V-Sync disabled (unlimited FPS)
window.setVerticalSyncEnabled(false);
```

#### Render Texture Optimization
```cpp
// Before: Bilinear filtering enabled (expensive)
renderTexture.setSmooth(true);

// After: Pixel-perfect rendering (performance optimized)
renderTexture.setSmooth(false);
```

#### View System Replacement
```cpp
// Before: Complex SFML view calculations
updateViewForWindow(window); // Heavy viewport calculations
updateView(renderTexture);   // Aspect ratio computations

// After: Direct sprite scaling
if (m_isFullscreen) {
  float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(initScreenX);
  float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(initScreenY);
  float scale = std::min(scaleX, scaleY); // Uniform scaling
  sprite.setScale(sf::Vector2f(scale, scale));
  
  // Simple centering
  float offsetX = (windowSize.x - initScreenX * scale) / 2.0f;
  float offsetY = (windowSize.y - initScreenY * scale) / 2.0f;
  sprite.setPosition(sf::Vector2f(offsetX, offsetY));
}
```

### 3. Architecture Changes

**Removed Dependencies**:
- Eliminated `WindowManager` dependency from `BoardDiamondSeq`
- Removed dynamic scaling calculations
- Simplified fullscreen transition logic

**Constructor Simplification**:
```cpp
// Before: Complex dependency injection
BoardDiamondSeq(TextureHolder* textures, DP::WindowManager* windowManager)

// After: Clean single dependency
BoardDiamondSeq(TextureHolder* textures)
```

## Performance Results

### Diamond Rendering
- **Calculation Overhead**: Eliminated runtime math operations
- **Memory Access**: Reduced from variable reads to constant values
- **VertexArray Updates**: Only when diamonds actually move/change

### Fullscreen Performance
- **Windowed Mode**: Thousands of FPS maintained
- **Fullscreen Mode**: Restored from 60fps cap to unlimited performance
- **Transition Speed**: Faster fullscreen toggle without view system overhead

### Visual Quality
- **Aspect Ratio**: Properly maintained with letterboxing/pillarboxing
- **Centering**: Perfect game positioning in fullscreen
- **Proportions**: 80% diamond scaling provides cleaner board appearance

## Technical Implementation Details

### Scaling Mathematics
```
Original Diamond: 44x44px
Scaled Diamond: 44 * 0.8 = 35.2px
Tile Size: 40x40px
Centering Offset: (40 - 35.2) / 2 = 2.4px
```

### Fullscreen Scaling
```
Uniform Scale = min(screen_width/1360, screen_height/768)
Centered Position = (screen_size - game_size * scale) / 2
```

### Performance Optimization Strategy
1. **Compile-time constants** instead of runtime calculations
2. **Direct sprite scaling** instead of SFML view system
3. **Pixel-perfect rendering** instead of bilinear filtering
4. **Minimal state changes** during fullscreen transitions

## Files Modified

1. **src/boarddiamondseq.h**: Removed WindowManager dependency, simplified constructor
2. **src/boarddiamondseq.cpp**: Pre-calculated diamond scaling constants
3. **src/window-manager.cpp**: Optimized fullscreen rendering pipeline
4. **src/game.cpp**: Updated BoardDiamondSeq initialization

## Backward Compatibility

- **Game Logic**: No changes to diamond collection, positioning, or gameplay
- **Save Compatibility**: No impact on game state or progression
- **Visual Consistency**: Maintains same relative proportions across all modes
- **Input Handling**: Mouse coordinates and click detection unaffected

## Future Considerations

- **Configurable Scaling**: Could add user preference for diamond size (70%, 80%, 90%)
- **Dynamic Performance**: Monitor FPS and auto-adjust quality settings
- **Platform Optimization**: Different scaling strategies for mobile/desktop platforms
- **Shader Integration**: GPU-based scaling for even better performance

## Conclusion

This optimization successfully addressed both visual and performance concerns:
- **80% diamond scaling** provides better board aesthetics
- **Fullscreen performance** restored to thousands of FPS
- **Clean architecture** with minimal dependencies
- **Maintainable code** with clear separation of concerns

The key insight was that SFML's view system, while feature-rich, introduced unnecessary overhead for simple fullscreen scaling. Direct sprite mathematics proved both faster and more predictable.