# Lighting Effects Implementation Plan for DeerPortal

**Date**: January 2025  
**Version**: 0.10.0-pre.1  
**Technology**: SFML 3.0.1, C++17  
**Target**: Board initialization animation with darkness and dynamic lighting

## Executive Summary

This document outlines a comprehensive plan to implement lighting effects for DeerPortal's board initialization animation. The goal is to create a dark overlay with diamonds acting as light sources that illuminate the darkness as they appear and move across the board. The implementation prioritizes performance and visual appeal using established game development patterns.

## Visual Concept

### Desired Effect
1. **Initial State**: Game board covered in darkness (semi-transparent black overlay)
2. **Diamond Appearance**: Each diamond emits a soft light radius
3. **Animation Progress**: Light trails follow diamond movement paths
4. **Final State**: Gradual fade from darkness to normal lighting

### Technical Goals
- 60+ FPS performance during animation
- Smooth light blending between multiple sources
- Configurable light parameters (radius, intensity, color)
- Minimal memory overhead

## Architecture Overview

### Core Components

#### 1. Lighting System Manager
```cpp
class LightingSystem {
    sf::RenderTexture darknessMask;
    sf::RenderTexture lightAccumulator;
    std::vector<LightSource> activeLights;
    LightingConfig config;
};
```

#### 2. Light Source Component
```cpp
struct LightSource {
    sf::Vector2f position;
    float radius;
    float intensity;
    sf::Color color;
    float falloffExponent;
    bool active;
};
```

#### 3. Darkness Overlay Renderer
```cpp
class DarknessOverlay {
    sf::VertexArray darknessQuad;
    float opacity;
    sf::BlendMode blendMode;
};
```

## Implementation Strategy

### Phase 1: Foundation (2 days)

#### 1.1 Create Lighting System Infrastructure
- Create `src/lighting-system.h/cpp`
- Implement basic `LightSource` structure
- Set up render texture management
- Integrate with existing `BoardInitializationAnimator`

#### 1.2 Shader Development
- Create `assets/shaders/radial_light.frag` for light sources
- Modify existing `dark.frag` for darkness overlay
- Implement shader loading in `GameAssets`

### Phase 2: Core Lighting Implementation (3 days)

#### 2.1 Darkness Layer
- Implement full-screen darkness overlay
- Configure opacity and blend modes
- Add fade-in/fade-out capabilities

#### 2.2 Light Source Rendering
- Implement radial gradient light rendering
- Support multiple light source blending
- Add light color and intensity parameters

#### 2.3 Performance Optimization
- Implement light culling (only render visible lights)
- Use vertex arrays for batch rendering
- Cache light textures for reuse

### Phase 3: Integration with Board Animation (2 days)

#### 3.1 Diamond Light Binding
- Attach light sources to `AnimatedBoardItem` instances
- Synchronize light position with diamond movement
- Scale light intensity with diamond appearance

#### 3.2 Animation Enhancements
- Add light trail effects for moving diamonds
- Implement smooth light fade-in as diamonds spawn
- Configure per-diamond light properties

### Phase 4: Polish and Configuration (1 day)

#### 4.1 Configuration System
```cpp
struct LightingConfig {
    float darknessOpacity = 0.85f;
    float baseLightRadius = 80.0f;
    float lightIntensity = 1.0f;
    bool enableLightTrails = true;
    float trailFadeSpeed = 2.0f;
};
```

#### 4.2 Visual Refinements
- Fine-tune light falloff curves
- Add subtle light flickering
- Implement color temperature variations

## Technical Implementation Details

### Rendering Pipeline

#### Current Pipeline (from analysis)
```
1. Clear buffers
2. Render game elements to RenderTexture
3. Apply shaders (if enabled)
4. Draw to window
```

#### Enhanced Pipeline with Lighting
```
1. Clear buffers
2. Render game elements to RenderTexture
3. Render darkness overlay to darknessMask
4. For each light source:
   - Render light to lightAccumulator (additive blend)
5. Combine layers:
   - Base game render
   - Apply darkness with subtractive alpha
   - Apply accumulated lights
6. Draw final composite to window
```

### Shader Implementation

#### Radial Light Fragment Shader
```glsl
uniform vec2 lightPosition;
uniform float lightRadius;
uniform float lightIntensity;
uniform vec3 lightColor;

void main() {
    vec2 coord = gl_FragCoord.xy;
    float distance = length(coord - lightPosition);
    float attenuation = 1.0 - smoothstep(0.0, lightRadius, distance);
    attenuation = pow(attenuation, 2.2); // Gamma correction
    
    vec4 color = vec4(lightColor * lightIntensity * attenuation, attenuation);
    gl_FragColor = color;
}
```

### Performance Optimization Strategies

#### 1. Spatial Partitioning
- Divide screen into grid cells
- Only process lights affecting visible cells
- Cull lights outside viewport

#### 2. Light Pooling
```cpp
class LightPool {
    std::array<LightSource, MAX_LIGHTS> pool;
    std::queue<size_t> available;
    std::vector<size_t> active;
};
```

#### 3. Texture Caching
- Pre-render common light patterns
- Reuse light textures for similar configurations
- Implement dirty flag system for updates

#### 4. Level of Detail
- Reduce light quality for distant/small lights
- Skip subtle effects under performance pressure
- Dynamic quality adjustment based on FPS

### Memory Management

#### Texture Requirements
- Darkness mask: 1360x768 (screen size)
- Light accumulator: 1360x768
- Light texture cache: ~512x512 per unique light
- Total estimated: ~8-12 MB

#### Optimization Techniques
- Share light textures between similar sources
- Use smaller textures for distant lights
- Clear unused cache entries periodically

## Integration Points

### Board Animation System
```cpp
// In animated-board-item.cpp
void AnimatedBoardItem::update(sf::Time deltaTime) {
    // Existing animation code...
    
    // Update attached light source
    if (lightSource) {
        lightSource->position = currentPosition;
        lightSource->intensity = currentScale; // Fade with diamond
    }
}
```

### Game Renderer
```cpp
// In game-renderer.cpp
void GameRenderer::renderStateBoardAnimation() {
    // Render base animation
    game->boardAnimator->render(renderTarget, texture);
    
    // Apply lighting effects
    if (lightingSystem.isEnabled()) {
        lightingSystem.renderLighting(renderTarget);
    }
}
```

## Configuration and Tuning

### Exposed Parameters
```cpp
// In game settings or config file
lighting.darkness_opacity = 0.85
lighting.light_radius_base = 80.0
lighting.light_intensity = 1.0
lighting.enable_trails = true
lighting.trail_length = 0.5
lighting.fade_in_duration = 0.3
lighting.fade_out_duration = 1.0
```

### Debug Visualization
- Toggle lighting system on/off
- Show light source positions
- Display performance metrics
- Render individual lighting layers

## Performance Benchmarks

### Target Metrics
- 60+ FPS with 112 active light sources
- < 5ms lighting render time
- < 10MB additional memory usage
- Smooth animation without stuttering

### Measurement Points
1. Frame time with/without lighting
2. Number of draw calls
3. GPU memory usage
4. Light culling efficiency

## Risk Mitigation

### Potential Issues
1. **Performance degradation with many lights**
   - Solution: Aggressive culling and LOD system
   
2. **Visual artifacts from overlapping lights**
   - Solution: Proper blend modes and accumulation buffer

3. **Memory pressure on older GPUs**
   - Solution: Dynamic quality reduction

4. **Shader compatibility issues**
   - Solution: Fallback to texture-based lighting

## Implementation Timeline

### Week 1
- Day 1-2: Foundation and infrastructure
- Day 3-5: Core lighting implementation
- Day 6-7: Integration and initial testing

### Week 2
- Day 1-2: Performance optimization
- Day 3-4: Visual polish and configuration
- Day 5: Testing and documentation

## Future Enhancements

### Potential Extensions
1. **Dynamic shadows**: Add shadow casting from diamonds
2. **Colored lighting**: Element-specific light colors
3. **Particle lighting**: Light-emitting particles
4. **Environment lighting**: Ambient light changes

### Advanced Effects
- Light reflection on board surfaces
- Volumetric light shafts
- Heat distortion near fire elements
- Caustic effects for water elements

## Conclusion

This lighting system will enhance DeerPortal's visual appeal while maintaining performance. The modular design allows for incremental implementation and easy configuration. The system leverages established 2D lighting patterns while being tailored to DeerPortal's specific needs.

The implementation focuses on the board initialization animation but provides a foundation for extending lighting effects throughout the game. With careful optimization and intelligent culling, the system can deliver impressive visual results without compromising the game's performance on target hardware.